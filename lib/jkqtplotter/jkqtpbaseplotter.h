/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef JKQTPBASEPLOTTER_H
#define JKQTPBASEPLOTTER_H

#include "jkqtplotter/jkqtplotter_configmacros.h"

#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtpdatastorage.h"
#include "jkqtplotter/jkqtpbaseplotterstyle.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtcommon/jkqtpenhancedpainter.h"
#include "jkqtplotter/gui/jkqtpenhancedspinboxes.h"

#include <QObject>
#include <QAction>
#include <QActionGroup>
#include <QVector>
#include <QSettings>
#include <QColor>
#include <QMap>
#include <QVector>
#include <QPair>
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
#  include <QPrinter>
#  include <QtPrintSupport/QPrintPreviewWidget>
#endif
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPointer>
#include <QListWidget>
#include <QComboBox>

#include <vector>
#include <cmath>
#include <iostream>
#include "jkqtplotter/jkqtplotter_imexport.h"



class JKQTPGraphsModel; // forward
class JKQTPGraph; // forward
class JKQTPPlotElement; // forward

/** \brief initialized Qt-ressources necessary for JKQTBasePlotter
 * \ingroup jkqtpplottersupprt */
JKQTPLOTTER_LIB_EXPORT void initJKQTBasePlotterResources();

/** \brief virtual base-class for exporter classes that can be used to save data inot a file
 * \ingroup jkqtpplottersupprt */
class JKQTPLOTTER_LIB_EXPORT JKQTPSaveDataAdapter {
    public:
        virtual ~JKQTPSaveDataAdapter() ;
        virtual QString getFilter() const=0;
        virtual void saveJKQTPData(const QString& filename, const QList<QVector<double> >& data, const QStringList& columnNames) const=0;
};

/** \brief Service from this class to implement a special QPaintDevice as a plugin, that can be registered to JKQTBasePlotter/JKQTPlotter
 *         and then be used to export graphics, use registerPaintDeviceAdapter() to register such a plass
 * \ingroup jkqtpplottersupprt */
class JKQTPLOTTER_LIB_EXPORT JKQTPPaintDeviceAdapter {
    public:
        virtual ~JKQTPPaintDeviceAdapter()  {}
        virtual QString getFilter() const=0;
        virtual QString getFormatName() const=0;
        virtual QString getFormatID() const=0;
        virtual QStringList getFileExtension() const=0;
        virtual bool getSetAbsolutePaperSize() const=0;
        virtual double getPrintSizeXInMM() const =0;
        virtual double getPrintSizeYInMM() const =0;
        virtual bool isPrinter() const=0;
        /** \brief create a paint device with a given size in pt */
        virtual QPaintDevice* createPaintdevice(const QString& filename, int widthPix, int heightPix) const=0;
        /** \brief create a paint device with a given size in millimeters ... the default implementation call createPaintdevice(), assuming the standard logical resolution of the desktop!!!) */
        virtual QPaintDevice* createPaintdeviceMM(const QString& filename, double widthMM, double heightMM) const;
};

/** \brief base class for 2D plotter classes (used by the plotter widget JKQTPlotter)
 * \ingroup jkqtpplotterclasses
 *
 * This class implements basic functionalities for the plotter classes. Those are:
 *   -# data storage using a JKQTPDatastore object
 *   -# coordinate transforms
 *   -# a set of properties for the graphs (colors, widthes ...) and also a system (see getNextStyle() to automatically
 *     choose a drawing style for different graphs.
 *   -# drawing the coordinate axes, grids ... (logarithmic and linear)
 *   -# saveing and printing the resulting plots
 * .
 *
 * This class is NOT a widget, if you need a plotting widget, use JKQTPlotter. This class may be used to
 * plot using a JKQTPEnhancedPainter.
 *
 * \see JKQTPlotter a QWidget class that displays a JKQTBasePlotter plot on a Qt Window.
 *
 *
 * \tableofcontents
 *
 *
 * \section jkqtplotter_base_datastore Data Storage
 * As already mentioned this class does not provide means to draw graphs, but it contains a basic mechanism to associate
 * data with it. This is imeplemented, using a JKQTPDatastore object together with some convenience access methods. No further
 * data access is implemented and child classes are NOT forced to use this datastore. Hide it if you want. JKQTPlotter shows
 * how to use it. This class implement a graph management, where graphs simply point to a set of columns inside the datastore
 * that contain the actual plot data!
 *
 * If you call the JKQTBasePlotter::JKQTBasePlotter() constructor with no arguments, it will create an internal
 * datastore object and you can start adding data by using getDatastore(). If you have an external JKQTPDatastore object you can
 * give it as parameter to the constructor or use one of the other methods:
 *  - useExternalDatastore(): \copybrief JKQTBasePlotter::useExternalDatastore()
 *  - useAsInternalDatastore(): \copybrief JKQTBasePlotter::useAsInternalDatastore()
 *  - useInternalDatastore(): \copybrief JKQTBasePlotter::useInternalDatastore()
 *  - forceInternalDatastore(): \copybrief JKQTBasePlotter::forceInternalDatastore()
 *.
 *
 *
 * \section jkqtplotter_base_systems_baseplotter Coordinate Systems and Transformations
 * These topics are discussed in the help for JKQTPCoordinateAxis. There is a set of coordinate transform
 * methods (x2p(), y2p(), p2x(), p2y() which only call the respective methods in xAxis and yAxis objects.
 *
 *
 * \section jkqtplotter_base_grids_baseplotter Axis Ticks and Grids
 *    - The plotting of coordinate axes and grids, as well as coordinate transforms is done by
 *      JKQTPCoordinateAxis descendents (see documentation there)
 *  .
 *  If you want to set the axis properties, use getXAxis() or getYAxis() to get a pointer to the axis objects which then
 *  may be used to set the axis properties.
 *
 *
 * \section jkqtbaseplotter_appearance_and_style Appearance & Styling of the Graph
 *
 *
 * \subsection jkqtplotter_base_plotsize Plot Sizes & Borders
 *
 * JKQTBasePlotter itself is no QWidget. It is merely tool to represent, manage and draw a graph onto any durface.
 * JKQTPlotter then uses an internal JKQTBasePlotter instance to provide a QWidget.
 *
 * Still the JKQTBasePlotter needs a way to represent the location of the graph, the graph axes etc. inside
 * a drawing rectangle (of size widgetWidth * widgetHeight ). The plot borders are then offsets to the plot
 * indise that rectangele. This image illustrated the principle:
 *
 * \image html plot_widget_orientation.png
 *
 * When the plot is actually drawn, it may happen, that additional elements need to be positioned between the
 * actual coordinate rectangle and the widget worder (e.g. axis labels, ticks, color bars etc.). Therefore
 * this objects manages a set of internally calculated variables that also incorporate those dynamic elements.
 * These are e.g. internalPlotBorderLeft, internalPlotBorderTop, internalPlotBorderRight, ... Their names all
 * start with \c internal... .
 *
 *
 * \subsection jkqtplotter_base_aspectratios Aspect Ratios
 * First note that this functionality is only available and activated if both axes are linear!
 *
 * You can set two different aspect ratios:
 *   - The ratio of plotWidth/plotHeight (setAspectRatio(), setMaintainAspectRatio()) will keep the plots pixel-width and height at a certain value.
 *     \f[ \mbox{aspectRatio}=\frac{\mbox{plotWidth}}{\mbox{plotHeight}} \f]
 *   - The ratio of (xmax-xmin)/(ymax-ymin) (setAxisAspectRatio(), setMaintainAxisAspectRatio()) will keep the displayed axis ranges in a certain ratio.
 *     \f[ \mbox{axisAspectRatio}=\frac{\left|x_\text{max}-x_\text{min}\right|}{\left|y_\text{max}-y_\text{min}\right|} \f]
 * .
 * So to achieve different effects, use these combinations:
 *   - you have a 200x100 range where each 1x1-pixel should have an aspect ratio of 4:
 *     \code
 *        setAspectRatio(200.0/100.0);
 *        setAxisAspectRatio(4.0*getAspectRatio());
 *     \endcode
 * .
 *
 *
 * \subsection jkqtplotter_base_key Graph Keys (Legends)
 * This class provides means to plot a key together with the functions. The plotting is only partially implemented in this base class, and has to be
 * implemented by child classes, as no graph management, that could provide a title for a specific graph, is implemented here. Key Plotting is
 * performed in the plotKey() method. This method basically draws a frame and background and then calls plotKeyContent() to draw the actual contents.
 * The size of the key is determined by a call to getKeyExtent(), while it's position and plotting style is determined by the \c key_... parameters.
 * If getKeyExtent() returns 0 for height or width, no key is plotted. The \c key_... parameters are designed to be rather common for any usual kind
 * of key, so any implementation of key should be able to use them to parametrize the output. Note however that additional parameters might be defined
 * in child classes.
 *
 * \image html plot_key.png
 *
 * As it may be convenient to position the key completely outside the graph this class may change the plotBorders to make the key fit outside
 * SO if you use a keyPosition \b outside the graph you should NOT keep any space for the key in the margins, as the program will change them to
 * accomodate the size of the key. If you select an \b inside key position the key will be plotted OVER the graph, i.e. the margins won't be changed
 * Note that the margin change is internal and not visible in the class interface!
 *
 * There is also a possibility to determine the size of the key automatically, so all text fits in. This is activted by the property keyAutosize ( \copybrief JKQTPKeyStyle::autosize ). If this
 * is \c true the function getKeyExtent() has to check the width of every key item and take it into account when calculating the width and height of the
 * key content. By default this feature is switched ON.
 *
 *
 * \section jkqtbaseplotter_dataexport_print Printing, Saving & Exporting
 *
 * \subsection jkqtplotter_base_saveprint Saving and Printing Graphs
 * This class implement a set of function to save and print the graphs:
 *   - print() prints the graph on a QPrinter object
 *   - saveAsPixelImage() saves the plot into a pixel image file (PNG, TIFF, ... formats, as supported by Qt)
 *   - saveAsPDF() saves the graph as a PDF file (using the Qt printing engine)
 *   - saveAsSVG() saves the graph as a SVG file (using the Qt SVG library)
 *   - saveImage() saves the graph
 * .
 * You can also copy the contents of the plot into the clipboard:
 *   - copyPixelImage()
 * .
 *
 * \subsection jkqtplotter_base_dataexport Exporting Graph Data
 * This class implement a set of function to save the data of the graphs:
 *   - saveData() saves the data of the plot
 *   - saveAsCSV() saves the data of the plot as comma separated values
 *   - saveAsSYLK() saves the data of the plot as SYLK spreadsheet
 *   - saveAsDIF() saves the data of the plot as data interchange format file
 *   - saveAsMatlab() saves the data of the plot as a CSV file suitable for Matlab
 *   - saveAsSemicolonSV() saves the data of the plot as a Semicolon Separated Values (SSV)
 *   - saveAsTabSV() saves the data of the plot as a Tabulator Separated Values (CSV) file
 *   - saveAsGerExcelCSV()  saves the data of the plot as a Text file (SSV) suitable for german excel, i.e. with comma as decimal separator
 * .
 * You can also copy the graphs' data into the clipboard:
 *   - copyData()
 *   - copyDataMatlab()
 * .
 *
 *
 * \section  JKQTBASEPLOTTER_SYNCMULTIPLOT Synchronizing Several Plots
 *
 * Often a single plot is not sufficient, but several plots need to be aligned with respect to each other:
 *
 * \image html multiplot.png
 *
 * This can be achieved by putting several JKQTPlotter instances into a
 * <a href="http://doc.qt.io/qt-5/layout.html">Qt Layout</a>. Then you can fill each plot differently and
 * set the x-/y-range of each plot by hand. This method works for simple cases, but has several drawbacks:
 *   - Due to the independent and automatic layouting of each plot, the axes do not need to be aligned properly<br>
 *     \image html jkqtbaseplotter_synchronization_unequalaxes.png
 *   - When you print the plot, the printing does not know about the layout and the other plots in it. Therefor
 *     it will only print the plot itself.<br>
 *     \image html jkqtbaseplotter_synchronization_nogridprint.png "Printing with grid-printing-mode deactivated"<br>
 *     \image html jkqtbaseplotter_synchronization_withgridprint.png "Printing with grid-printing-mode activated"
 *   - when you zoom/pan in one of the plots (e.g. using the mouse), the other plots will not adapt their
 *     axes to match the new area, but especially in cases as in the image above it would be beneficial,
 *     that the x-axis of the plot at the bottom follows the x-axis of the plot above etc.<br>
 *     \image html jkqtbaseplotter_synchronization_nonsyncedxrange.png
 * .
 *
 *
 * To overcome these limitations, JKQTPlotter (and JKQTBasePlotter) offer an API with which you can declare relations between
 * different plots (one of them is made the master) and you can synchronize the axes of two plots, when
 * zooming (also when calling e.g. zoomToFit() or setXY() ). This API is:
 *   - \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_SYNC
 *   - \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_GRIDPRINT
 * .
 *
 * \subsection JKQTBASEPLOTTER_SYNCMULTIPLOT_SYNC Synchronizing Axis Range & Plot Width
 *
 * You can synchronize the plot width and axis range between two JKQTPlotter instaces using:
 *   - synchronizeToMaster() / JKQTBasePlotter::synchronizeToMaster() synchronizes the parent JKQTPlotter with another JKQTPlotter. With two boolean-parameters
 *         you can specify the axes to be synchronized. E.g. in the case above, you would call:
 *         \code
 *              // synchronize width/x-axis of plotResid to width/x-axis of plotMain
 *              plotResid->synchronizeToMaster(plotMain, JKQTBasePlotter::sdXAxis, true, true, true);
 *
 *              // synchronize y-axis of width/plotResidHist to y-axis of width/plotResid
 *              plotResidHist->synchronizeToMaster(plotResid, JKQTBasePlotter::sdYAxis, true, true, true);
 *         \endcode
 *         This will synchronize the x-axes of the top (\c plotMain ) and bottom-left plot (\c plotResid ),
 *         as well as the y-axes of the bottom-left (\c plotResid ) and bottom-right plot (\c plotResidHist ).
 *         After this call they will have the same size in screen pixels and always span the same range
 *         in plot coordinates.
 *   - synchronizeXToMaster() / JKQTBasePlotter::synchronizeXToMaster() like synchronizeToMaster() / JKQTBasePlotter::synchronizeToMaster(),
 *     but synchronizes only the x-axes
 *   - synchronizeYToMaster() / JKQTBasePlotter::synchronizeYToMaster() like synchronizeToMaster() / JKQTBasePlotter::synchronizeToMaster(),
 *     but synchronizes only the y-axes
 *   - resetMasterSynchronization() / JKQTBasePlotter::resetMasterSynchronization() deletes all synchronizations
 *       from the JKQTPlotter
 * .
 *
 * \see See \ref JKQTPlotterMultiPlotLayout for an extensive example of the functionality.
 *
 *
 * \subsection JKQTBASEPLOTTER_SYNCMULTIPLOT_GRIDPRINT Grid Printing/Desclaring Relations between Plots
 *
 * Usually each JKQTPlotter/JKQTBasePlotter prints or exports only itself. But in many cases you might want to
 * add several plots that form a grid layout and also want to export them as one image/print them on one page.
 * To do this, JKQTPlotter/JKQTBasePlotter offers the <b>"grid printing" mode</b>. In this mode, you can declare
 * relations between different JKQTPlotters/JKQTBasePlotters by putting them in a rectangular grid (like in a
 * <a reh="http://doc.qt.io/qt-5/qgridlayout.html">QGridLayout</a>). then any export/print action will draw all (explicitly)
 * decalred graphs.
 *
 * \image html jkqtplotter_gridprint.png
 *
 * The grid printing API is composed of these functions:
 *   - setGridPrinting() enables grid printing for this JKQTPlotter. If set to \c true , and you print afterwards,
 *     the printout (or export) will not only contain the plot itself, but also additional plots that were
 *     declared using addGridPrintingPlotter() (see below).
 *   - addGridPrintingPlotter() add a new plotter \a plotterOther for grid printing mode, at location \a x / \a y
 *     E.g. in the example shown above, you could call:
 *     \code
 *          plotMain->setGridPrinting(true);
 *          plotMain->addGridPrintingPlotter(0,1,plotResid);
 *          plotMain->addGridPrintingPlotter(1,1,plotResidHist);
 *     \endcode
 *   - setGridPrintingCurrentX() / setGridPrintingCurrentY() / setGridPrintingCurrentPos()
 *     sets the location of the calling plot inside the grid. <i>By default each plot assumes to be at (0,0).</i>
 *   - clearGridPrintingPlotters() clear all additional plotters for grid printing mode
 * .
 *
 * \see See \ref JKQTPlotterMultiPlotLayout for an extensive example of the functionality.
 *
 *
 * \section jkqtplotter_base_userprops User Properties
 * There is a subset of options that describe how the user interacted with the plotter (export/print scaling factors etc, save directories,
 * other export settings, ...). These are not stored/loaded using saveSettings() and loadSettings(), but using saveUserSettings() and loadUserSettings().
 * These methods MAY (strictly optional and turned off by default) be called by saveSettings() and loadSettings(), if the property userSettigsFilename ( \copybrief userSettigsFilename )is
 * set (not-empty). In this case the suer settings are stored/loaded also everytime they are changed by the user or programmatically.
 *
 * \section jkqtplotter_usage_baseplotter JKQTBasePlotter Usage
 *
 * \subsection jkqtplotter_usage_baseplotter_in_widget JKQTBasePlotter as Basis for JKQTPlotter
 *
 * Most commonly this invisible plotter class is used as basis for the widget JKQTPlotter.
 * \see JKQTPlotter
 *
 * \subsection jkqtplotter_usage_baseplotter_standalone JKQTBasePlotter Standalone Usage
 *
 * \copydetails jkqtplotter_general_usage_jkqtplotter
 *
 */
class JKQTPLOTTER_LIB_EXPORT JKQTBasePlotter: public QObject {
        Q_OBJECT
    public:
        typedef QMap<QString, QList<QPointer<QAction> > > AdditionalActionsMap;
        typedef QMapIterator<QString, QList<QPointer<QAction> > > AdditionalActionsMapIterator;

        /** \brief set a global preset/default value for the userSettigsFilename and userSettigsPrefix properties of JKQTBasePlotter
         *
         *  These presets are application global and will be used ONLY on initialization of a JKQTBasePlotter. You can overwrite them
         *  on a local-basis for each JKQTBasePrinter separately. The changed values from this function call will only take effect for
         *  newly created plotters AFTER the function call!
         */
        static void setDefaultJKQTBasePrinterUserSettings(QString userSettigsFilename, const QString& userSettigsPrefix);

        /** \brief register a user-defined QPaintDevice (with factory JKQTPPaintDeviceAdapter) as a plugin to JKQTBasePlotter/JKQTPlotter,
         *         which will use it to export graphics */
        static void registerPaintDeviceAdapter(JKQTPPaintDeviceAdapter* adapter);
        /** \brief de-register a  JKQTPPaintDeviceAdapter from JKQTBasePlotter/JKQTPlotter */
        static void deregisterPaintDeviceAdapter(JKQTPPaintDeviceAdapter* adapter);




        /** \brief register a JKQTPSaveDataAdapter with JKQTPlotter/JKQTBasePlotter that can be used to export data from the internal datastore into a file */
        static bool registerSaveDataAdapter(JKQTPSaveDataAdapter* adapter);
        /** \brief de-register a JKQTPSaveDataAdapter from JKQTPlotter/JKQTBasePlotter */
        static bool deregisterSaveDataAdapter(JKQTPSaveDataAdapter* adapter);



        /** \brief class constructor
         *
         * if \a datastore_internal is \c true then the constructor will create an internal datastore object. The datastore
         * will be managed (freed) by this class. If \a datastore_internal is \c false the class will use the datastore provided
         * in \a datast as an external datastore. You can modify this later by using useInternalDatastore() and useExternalDatastore().
         */
        explicit JKQTBasePlotter(bool datastore_internal, QObject* parent=nullptr, JKQTPDatastore* datast=nullptr);

        JKQTBasePlotter()=delete;

        /** \brief class destructor */
        virtual ~JKQTBasePlotter();

        /** \brief returns a pointer to the datastore used by this object */
        inline JKQTPDatastore* getDatastore() { return datastore; }

        /** \brief returns model representing all Plots in this plotter and showing their visible/invisible state */
        inline JKQTPGraphsModel* getPlotsModel() { return m_plotsModel; }


        /** \brief returns a pointer to the datastore used by this object */
        inline const JKQTPDatastore* getDatastore() const { return datastore; }

        /** \brief returns model representing all Plots in this plotter and showing their visible/invisible state */
        inline const JKQTPGraphsModel* getPlotsModel() const { return m_plotsModel; }

        /** \brief tells the plotter object to use the given external datastore.
         *
         * If the current datastore is internally managed, this method will free that object and use the supplied datastore
         * with external management. If the current datastore is already external, this method will simply replace it by the
         * new one.
         */
        void useExternalDatastore(JKQTPDatastore* newStore);

        /** \brief tells the plotter object to use the given external datastore and treat it as an internal one (i.e. free it
         *         when the plotter object ist destroyed.
         */
        void useAsInternalDatastore(JKQTPDatastore* newStore);

        /** \brief tells the plotter object to use an internal datastore. A new internal datastore object is generated only if
         *         the current datastore is not internal.
         */
        void useInternalDatastore();

        /** \brief tells the plotter object to use an internal datastore (just like useInternalDatastore() ), but forces the
         *         generation of a new datastore, even if the current one is already internal (the current one will be freed in
         *         the lather case */
        void forceInternalDatastore();

        /** \brief returns whether this class emits signals, like zoomChangedLocally() or beforePlotScalingRecaluclate() */
        bool isEmittingSignalsEnabled()const;
        /** \brief specifies whether this class emits signals, like zoomChangedLocally() or beforePlotScalingRecaluclate() */
        void setEmittingSignalsEnabled(bool enabled);


        /** \brief loads all the plot properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object */
        void loadSettings(const QSettings& settings, const QString& group=QString("plots/"));

        /** \brief saves the plot properties into a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object.
         *
         * This method only saves those properties that differ from their default value.
         */
        void saveSettings(QSettings& settings, const QString& group=QString("plots/")) const;


        /** \brief current style properties for this JKQTBasePlotter
         *
         * \see JKQTPSetSystemDefaultBaseStyle(), JKQTPSetSystemDefaultBaseStyle(), getCurrentPlotterStyle(), \ref jkqtpplotter_styling
         */
        const JKQTBasePlotterStyle& getCurrentPlotterStyle() const;

        /** \brief replace the current style properties for this JKQTBasePlotter
         *
         * \see JKQTPSetSystemDefaultBaseStyle(), JKQTPSetSystemDefaultBaseStyle(), getCurrentPlotterStyle(), \ref jkqtpplotter_styling
         */
        void setCurrentPlotterStyle(const JKQTBasePlotterStyle& style);
        /** \brief replace the current style properties for this JKQTBasePlotter with properties loaded from \a settings
         *
         * \param settings the QSettings object to read from
         * \param group group in \a settings to read from
         *
         * \see JKQTPSetSystemDefaultBaseStyle(), JKQTPSetSystemDefaultBaseStyle(), getCurrentPlotterStyle(), \ref jkqtpplotter_styling
         */
        void loadCurrentPlotterStyle(const QSettings& settings, const QString& group="plots/");
        /** \brief store the current style properties for this JKQTBasePlotter with properties loaded from \a settings
         *
         * \param settings the QSettings object to write to
         * \param group group in \a settings to write to
         *
         * \see JKQTPSetSystemDefaultBaseStyle(), JKQTPSetSystemDefaultBaseStyle(), getCurrentPlotterStyle(), \ref jkqtpplotter_styling
         */
        void saveCurrentPlotterStyle(QSettings& settings, const QString& group="plots/") const;



        /** \brief loads the plot user properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object */
        void loadUserSettings(const QSettings& settings, const QString& group=QString("plots_user/"));

        /** \brief saves the plot user properties into a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object. */
        void saveUserSettings(QSettings& settings, const QString& group=QString("plots_user/")) const;

        /** \brief gets the width of the plot widget */
        int getWidth();

        /** \brief gets the width of the plot widget */
        int getHeight();


        /** \brief returns the current x-axis min */
        double getXMin() const;

        /** \brief returns the current x-axis max */
        double getXMax() const;

        /** \brief returns the current y-axis min */
        double getYMin() const;

        /** \brief returns the current y-axis max */
        double getYMax() const;



        /** \brief returns the given size in Didot points (0.376 mm) to pixels (with given dots per inch) */
        inline double didot2px(double pt, double dpi) const {
            return dpi/25.4*0.376*pt;
        }
        /** \brief returns the given size in Didot points (0.376 mm) to pixels (with dpi from painter) */
        inline double didot2px(double pt, JKQTPEnhancedPainter& painter) const {
            return didot2px(pt, painter.device()->logicalDpiX());
        }
        /** \brief returns the given size in Didot points (0.376 mm) to pixels (with dpi from painter) */
        inline double didot2px(JKQTPEnhancedPainter& painter, double pt) const {
            return didot2px(pt, painter.device()->logicalDpiX());
        }

        /** \brief returns the given size in DTP points points (1/72inch = 0.353mm) to pixels (with given dots per inch) */
        inline double pt2px(double pt, double dpi) const {
            return dpi*pt/72.0;
        }
        /** \brief returns the given size in DTP points points (1/72inch = 0.353mm) to pixels (with given dots per inch) */
        inline double pt2px(double pt, JKQTPEnhancedPainter& painter) const {
            return pt2px(pt, painter.device()->logicalDpiX());
        }
        /** \brief returns the given size in DTP points points (1/72inch = 0.353mm) to pixels (with given dots per inch) */
        inline double pt2px(JKQTPEnhancedPainter& painter, double pt) const {
            return pt2px(pt, painter.device()->logicalDpiX());
        }

        /** \brief returns the given size in millimeters to pixels (with given dots per inch) */
        inline double mm2px(double mm, double dpi) const {
            return dpi*mm/25.4;
        }
        /** \brief returns the given size in millimeters to pixels (with given dots per inch) */
        inline double mm2px(double mm, JKQTPEnhancedPainter& painter) const {
            return mm2px(mm, painter.device()->logicalDpiX());
        }
        /** \brief returns the given size in millimeters to pixels (with given dots per inch) */
        inline double mm2px(JKQTPEnhancedPainter& painter, double mm) const {
            return mm2px(mm, painter.device()->logicalDpiX());
        }

        /** \brief returns the absolute x-axis min */
        double getAbsoluteXMin() const;

        /** \brief returns the absolute x-axis max */
        double getAbsoluteXMax() const;

        /** \brief returns the absolute y-axis min */
        double getAbsoluteYMin() const;

        /** \brief returns the absolute y-axis max */
        double getAbsoluteYMax() const;


        /** \brief add a new plotter for grid printing mode */
        void addGridPrintingPlotter(size_t x, size_t y, JKQTBasePlotter* plotter);

        /** \brief clear all additional plotters for grid printing mode */
        void clearGridPrintingPlotters();

        /** \brief return x-pixel coordinate from x coordinate */
        inline double x2p(double x) const {
            return xAxis->x2p(x);
        }

        /** \brief return y-pixel coordinate from y coordinate */
        inline double y2p(double y) const {
            return yAxis->x2p(y);
        }

        /** \brief return x coordinate from x-pixel */
        inline double p2x(double x) const {
            return xAxis->p2x(x);
        }

        /** \brief return y coordinate from y-pixel */
        inline double p2y(double y) const {
            return yAxis->p2x(y);
        }

        /** \brief gets the next unused style id, i.e. the smalles number >=0 which is not contained in usedStyles */
        int getNextStyle();


        /** \brief represents a pen, when plotting in JKQTPlotter/JKQTBasePlotter
         *   \ingroup jkqtpplottersupprt
         *
         * \see getPlotStyle()
         */
        class JKQTPLOTTER_LIB_EXPORT JKQTPPen {
            protected:
                QColor m_color;
                QColor m_fillColor;
                QColor m_symbolFillColor;
                QColor m_errorColor;
                QColor m_errorFillColor;
                double m_width;
                double m_errorWidth;
                double m_symbolLineWidth;
                Qt::PenStyle m_style;
                double m_symbolSize;
                Qt::BrushStyle m_fillStyle;
                Qt::BrushStyle m_errorFillStyle;
                JKQTPGraphSymbols m_symbol;

            public:
                JKQTPPen();
                void setSymbolSize(double w);
                void setSymbolLineWidthF(double w);
                void setWidthF(double w);
                void setWidth(double w);
                void setErrorLineWidth(double w);
                double width() const;
                double widthF() const;
                double errorWidthF() const;
                double symbolLineWidthF() const;
                double symbolSize() const;
                QColor color() const;
                QColor errorColor() const;
                QColor errorFillColor() const;
                void setColor(const QColor& col);
                Qt::PenStyle style() const;
                Qt::PenStyle errorStyle() const;
                void setStyle(Qt::PenStyle s);
                QColor fillColor() const;
                void setFillColor(const QColor& col);
                void setErrorFillColor(const QColor& col);
                void setErrorLineColor(const QColor& col);
                void setFillStyle(Qt::BrushStyle s);
                void setErrorFillStyle(Qt::BrushStyle s);
                Qt::BrushStyle fillStyle() const;
                Qt::BrushStyle errorFillStyle() const;
                JKQTPGraphSymbols symbol() const;
                void setSymbolType(JKQTPGraphSymbols symbol);
                void setSymbolFillColor(QColor c);
                QColor symbolFillColor() const;
        };



        /** \brief returns a QPen object for the i-th plot style */
        JKQTPPen getPlotStyle(int i, JKQTPPlotStyleType type=JKQTPPlotStyleType::Default) const;

        /*! \brief draw the contained graph (including grid prints) into the given JKQTPEnhancedPainter
            \param painter JKQTPEnhancedPainter to which the plot should be drawn
            \param rect rectangle to plot into
         */
        void draw(JKQTPEnhancedPainter& painter, const QRect& rect);

        /*! \brief draw the contained graph (including grid prints) into the given JKQTPEnhancedPainter
            \param painter JKQTPEnhancedPainter to which the plot should be drawn
            \param pos where to plot the painter (left-top corner)
         */
        void draw(JKQTPEnhancedPainter& painter, const QPoint& pos=QPoint(0,0));

        /*! \brief draw the contained graph (including grid prints) into the given JKQTPEnhancedPainter
            \param painter JKQTPEnhancedPainter to which the plot should be drawn
            \param rect rectangle to plot into
         */
        void drawNonGrid(JKQTPEnhancedPainter& painter, const QRect& rect);

        /*! \brief draw the contained graph (including grid prints) into the given JKQTPEnhancedPainter
            \param painter JKQTPEnhancedPainter to which the plot should be drawn
            \param pos where to plot the painter (left-top corner)
         */
        void drawNonGrid(JKQTPEnhancedPainter& painter, const QPoint& pos=QPoint(0,0));

        /** \brief emit plotUpdated() */
        void redrawPlot() { if (emitPlotSignals) emit plotUpdated(); }

        /** \brief controls, whether the signals plotUpdated() are emitted */
        void setEmittingPlotSignalsEnabled(bool __value);
        /** \brief returns, whether the signals plotUpdated() are emitted */
        bool isEmittingPlotSignalsEnabled() const;

        /** \copydoc JKQTBasePlotterStyle::plotBorderTop  */
        int getPlotBorderTop() const;
        /** \copydoc JKQTBasePlotterStyle::plotBorderLeft  */
        int getPlotBorderLeft() const;
        /** \copydoc JKQTBasePlotterStyle::plotBorderBottom  */
        int getPlotBorderBottom() const;
        /** \copydoc JKQTBasePlotterStyle::plotBorderRight  */
        int getPlotBorderRight() const;

        /** \brief returns whether the maintaining of the data aspect ratio is enabled or disabled \see aspectRatio */
        bool doesMaintainAspectRatio() const;
        /** \brief returns the data aspect ratio, enforced with setMaintainApsectRatio(true) \see aspectRatio */
        double getAspectRatio() const;

        /** \brief returns whether the maintaining of the axis aspect ratio is enabled or disabled \see axisAspectRatio */
        bool doesMaintainAxisAspectRatio() const;
        /** \brief returns the axis aspect ratio, enforced with setMaintainAxisApsectRatio(true) \see axisAspectRatio */
        double getAxisAspectRatio() const;
        /** \copydoc JKQTBasePlotterStyle::useAntiAliasingForSystem  */
        bool isUsingAntiAliasingForSystem() const;
        /** \copydoc JKQTGraphsBaseStyle::useAntiAliasingForGraphs  */
        bool isUsingAntiAliasingForGraphs() const;
        /** \copydoc JKQTBasePlotterStyle::useAntiAliasingForText  */
        bool isUsingAntiAliasingForText() const;
        /** \copydoc JKQTBasePlotterStyle::widgetBackgroundBrush  */
        QColor getBackgroundColor() const;
        /** \copydoc JKQTBasePlotterStyle::exportBackgroundBrush  */
        QColor getExportBackgroundColor() const;
        /** \copydoc JKQTBasePlotterStyle::plotBackgroundBrush  */
        QColor getPlotBackgroundColor() const;
        /** \copydoc JKQTBasePlotterStyle::widgetBackgroundBrush  */
        QBrush getBackgroundBrush() const;
        /** \copydoc JKQTBasePlotterStyle::exportBackgroundBrush  */
        QBrush getExportBackgroundBrush() const;
        /** \copydoc JKQTBasePlotterStyle::plotBackgroundBrush  */
        QBrush getPlotBackgroundBrush() const;
        /** \copydoc JKQTPKeyStyle::fontSize */
        double getKeyFontSize() const;
        /** \copydoc JKQTPKeyStyle::itemWidth */
        double getKeyItemWidth() const;
        /** \copydoc JKQTPKeyStyle::itemHeight */
        double getKeyItemHeight() const;
        /** \copydoc JKQTPKeyStyle::ySeparation */
        double getKeyYSeparation() const;
        /** \copydoc JKQTPKeyStyle::sampleLineLength */
        double getKeyLineLength() const;
        /** \copydoc JKQTPKeyStyle::xMargin */
        double getKeyXMargin() const;
        /** \copydoc JKQTPKeyStyle::yMargin */
        double getKeyYMargin() const;
        /** \copydoc JKQTPKeyStyle::xSeparation */
        double getKeyXSeparation() const;
        /** \copydoc JKQTPKeyStyle::xOffset */
        double getKeyXOffset() const;
        /** \copydoc JKQTPKeyStyle::yOffset */
        double getKeyYOffset() const;
        /** \copydoc JKQTPKeyStyle::visible */
        bool getShowKey() const;
        /** \copydoc JKQTPKeyStyle::frameVisible */
        bool getShowKeyFrame() const;
        /** \copydoc JKQTPKeyStyle::frameColor */
        QColor getKeyFrameColor() const;
        /** \copydoc JKQTPKeyStyle::backgroundBrush */
        QColor getKeyBackgroundColor() const;
        /** \copydoc JKQTPKeyStyle::backgroundBrush */
        QBrush getKeyBackgroundBrush() const;

        /** \copydoc JKQTPKeyStyle::textColor */
        QColor getKeyTextColor() const;
        /** \copydoc JKQTPKeyStyle::frameWidth */
        double getKeyFrameWidth() const;
        /** \copydoc JKQTPKeyStyle::frameRounding */
        double getKeyFrameRounding() const;
        /** \copydoc JKQTPKeyStyle::autosize */
        bool getKeyAutosize() const;
        /** \copydoc JKQTPKeyStyle::position */
        JKQTPKeyPosition getKeyPosition() const;
        /** \copydoc JKQTPKeyStyle::layout */
        JKQTPKeyLayout getKeyLayout() const;
        /** \copydoc JKQTBasePlotterStyle::defaultTextColor */
        QColor getDefaultTextColor() const;
        /** \copydoc JKQTBasePlotterStyle::defaultFontSize */
        double getDefaultTextSize() const;
        /** \copydoc JKQTBasePlotterStyle::defaultFontName */
        QString getDefaultTextFontName() const;
        /** \brief if set \c true (default: \c false ) the JKQTBasePlotter draws colored rectangles to indicate the different regions in the plot (border, axes, ...)
         *
         *
         * \image html debugShowRegionBoxes.png
         *
         * \see JKQTBasePlotterStyle::debugShowRegionBoxes, enableDebugShowRegionBoxes()
         */
        bool isDebugShowRegionBoxesEnabled() const;
        /** \brief if set \c true (default: \c false ) the JKQTBasePlotter draws colored rectangles to indicate the extent of text
         *
         * \image html debugShowTextBoxes.png
         *
         * \see JKQTBasePlotterStyle::debugShowTextBoxes, enableDebugShowTextBoxes()
         */
        bool isDebugShowTextBoxesEnabled() const;
        /** \copydoc JKQTBasePlotterStyle::plotFrameVisible */
        bool isPlotFrameVisible() const;
        /** \copydoc JKQTBasePlotterStyle::plotFrameColor */
        QColor getPlotFrameColor() const;
        /** \copydoc JKQTBasePlotterStyle::plotFrameWidth */
        double getPlotFrameWidth() const;
        /** \copydoc JKQTBasePlotterStyle::plotFrameRounding */
        double getPlotFrameRounding() const;

        /** \copydoc JKQTBasePlotterStyle::plotLabelFontSize */
        double getPlotLabelFontSize() const;
        /** \copydoc JKQTBasePlotterStyle::plotLabelFontName */
        QString getplotLabelFontName() const;

        /** \copydoc plotLabel */ 
        QString getPlotLabel() const;

        /** \copydoc gridPrinting */ 
        void setGridPrinting(bool __value);
        /** \copydoc gridPrinting */ 
        bool getGridPrinting() const;
        /** \copydoc gridPrintingCurrentX */ 
        void setGridPrintingCurrentX(size_t __value);
        /** \copydoc gridPrintingCurrentX */ 
        size_t getGridPrintingCurrentX() const;
        /** \copydoc gridPrintingCurrentY */ 
        void setGridPrintingCurrentY(size_t __value);
        /** \copydoc gridPrintingCurrentY */ 
        size_t getGridPrintingCurrentY() const;

        /** \brief set the x- and y-positions of this JKQTPlotter in the grid-printing grid
         *
         * \see setGridPrinting(), addGridPrintingPlotter(), clearGridPrintingPlotters(), setGridPrintingCurrentX(), setGridPrintingCurrentY()
         */
        void setGridPrintingCurrentPos(size_t x, size_t y);


        /** \brief returns the current directory in which to open SaveAs ... dialogs */
        QString getCurrentSaveDirectory() const;
        /** \brief return the last file format to use in SaveAs ... dialogs */
        QString getCurrentFileFormat() const;
        /** \brief returns the decimal separator used when exporting data to text files */
        QString getCSVdecimalSeparator() const;
        /** \brief set the string used to introduce comments in text output when exporting data */
        QString getCSVcommentInitializer() const;

        /** \copydoc internalPlotBorderTop */
        inline double getInternalPlotBorderTop() const { return this->internalPlotBorderTop; }
        /** \copydoc internalPlotBorderLeft */
        inline double getInternalPlotBorderLeft() const { return this->internalPlotBorderLeft; }
        /** \copydoc internalPlotBorderBottom */
        inline double getInternalPlotBorderBottom() const { return this->internalPlotBorderBottom; }
        /** \copydoc internalPlotBorderRight */
        inline double getInternalPlotBorderRight() const { return this->internalPlotBorderRight; }
        /** \copydoc internalPlotWidth */
        inline int getPlotWidth() const { return this->internalPlotWidth; }
        /** \copydoc internalPlotHeight */
        inline int getPlotHeight() const { return this->internalPlotHeight; }
        /** \brief returns the internal JKQTMathText, used to render text with LaTeX markup */
        JKQTMathText* getMathText();
        /** \brief returns the internal JKQTMathText, used to render text with LaTeX markup */
        const JKQTMathText *getMathText() const;
        /** \brief returns the x-axis objet of the plot */
        JKQTPHorizontalAxis *getXAxis();
        /** \brief returns the y-axis objet of the plot */
        JKQTPVerticalAxis* getYAxis();
        /** \brief returns the x-axis objet of the plot */
        const JKQTPHorizontalAxis* getXAxis() const;
        /** \brief returns the y-axis objet of the plot */
        const JKQTPVerticalAxis *getYAxis() const;


        /** \copydoc actSavePlot */ 
        QAction* getActionSavePlot() const;
        /** \copydoc actSaveData */ 
        QAction* getActionSaveData() const;
        /** \copydoc actCopyData */ 
        QAction* getActionCopyData() const;
        /** \copydoc actCopyPixelImage */ 
        QAction* getActionCopyPixelImage() const;
        /** \copydoc actCopyMatlab */ 
        QAction* getActionCopyMatlab() const;
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        /** \copydoc actSavePDF */
        QAction* getActionSavePDF() const;
        /** \copydoc actSaveSVG */
        QAction* getActionSaveSVG() const;
        /** \copydoc actPrint */
        QAction* getActionPrint() const;
#endif
        /** \copydoc actSavePix */
        QAction* getActionSavePix() const;
        /** \copydoc actSaveCSV */ 
        QAction* getActionSaveCSV() const;
        /** \copydoc actZoomAll */ 
        QAction* getActionZoomAll() const;
        /** \copydoc actZoomIn */ 
        QAction* getActionZoomIn() const;
        /** \copydoc actZoomOut */ 
        QAction *getActionZoomOut() const;
        /** \copydoc actShowPlotData */ 
        QAction *getActionShowPlotData() const;
        /** \copydoc lstAdditionalPlotterActions */ 
        AdditionalActionsMap getLstAdditionalPlotterActions() const;

        /** \brief this function registers additional actions to lstAdditionalPlotterActions, which are displayed in the context-menu */
        void registerAdditionalAction(const QString& key, QAction* act);
        /** \brief this function deregisters an additional action, previously added with registerAdditionalAction()
         *
         * \see registerAdditionalAction()
         */
        void deregisterAdditionalAction(QAction* act);

        /** \copydoc masterSynchronizeWidth */ 
        bool getMasterSynchronizeWidth() const;
        /** \copydoc masterSynchronizeHeight */ 
        bool getMasterSynchronizeHeight() const;
        /** \copydoc fontSizePrintMultiplier */ 
        void setFontSizePrintMultiplier(double __value);
        /** \copydoc fontSizePrintMultiplier */ 
        double getFontSizePrintMultiplier() const;
        /** \copydoc lineWidthPrintMultiplier */ 
        void setLineWidthPrintMultiplier(double __value);
        /** \copydoc lineWidthPrintMultiplier */ 
        double getLineWidthPrintMultiplier() const;
        /** \copydoc fontSizeMultiplier */ 
        double getFontSizeMultiplier() const;
        /** \copydoc lineWidthMultiplier */ 
        double getLineWidthMultiplier() const;


        /** \brief returns description of i'th graph */
        JKQTPPlotElement* getGraph(size_t i);

        /** \brief returns the number of graphs */
        size_t getGraphCount();

        /** \brief remove the i-th graph */
        void deleteGraph(size_t i, bool deletegraph=true);

        /** \brief remove the given graph, if it is contained */
        void deleteGraph(JKQTPPlotElement* gr, bool deletegraph=true);

        /** \brief remove all plots
         *
         *  \param deleteGraphs if set \c true (default) the graph objects will also be deleted
         */
        void clearGraphs(bool deleteGraphs=true);

        /** \brief set all graphs in the plotter invisible
         *
         * \see setAllGraphsVisible(), setGraphVisible(), JKQTPPlotElement::setVisible()  */
        void setAllGraphsInvisible();
        /** \brief set all graphs in the plotter visible
         *
         * \see setAllGraphsInvisible(), setGraphVisible(), JKQTPPlotElement::setVisible() */
        void setAllGraphsVisible();
        /** \brief switch visibility of the \a i -th graph to \a visible
         *
         * \see setAllGraphsInvisible(), setAllGraphsVisible(), JKQTPPlotElement::setVisible() */
        void setGraphVisible(int i, bool visible=true);

        /** \brief add a new graph, returns it's position in the graphs list, if the graph is already in the plot, this returns the index in the list */
        size_t addGraph(JKQTPPlotElement* gr);

        /** \brief returns \c true, if the given graph is in this plot */
        bool containsGraph(JKQTPPlotElement* gr) const;

        /** \brief move the given graph to the top, or add it, if it is not yet contained */
        size_t moveGraphTop(JKQTPPlotElement* gr);
        /** \brief move the given graph to the top, or add it, if it is not yet contained */
        size_t moveGraphBottom(JKQTPPlotElement* gr);

        /** \brief  add a new graphs from a QVector<JKQTPPlotElement*>, QList<JKQTPPlotElement*>, std::vector<JKQTPPlotElement*> ... or any standard-iterateable container with JKQTPPlotElement*-items
         *
         *  \tparam TJKQTPGraphContainer a container type with default C++-sytle iterator interface
         *                               (i.e. methods \c begin() and \c end() and an iterator, which may be
         *                               moved to the next element with the operator \c ++ .
         *  \param gr Container of type TJKQTPGraphContainer, which contains the graphs \b Note: The JKQTPlotter takes ownership of graphs in \a gr .
         *  \param[out] graphIDsOut optional output parameter, the vector will contain the IDs of each graph added to theis plot
         */
        template <class TJKQTPGraphContainer>
        inline void addGraphs(const TJKQTPGraphContainer& gr, QVector<size_t>* graphIDsOut=nullptr) {
            for (auto it=gr.begin(); it!=gr.end(); ++it) {
                const size_t g=addGraph(*it);
                if (graphIDsOut) graphIDsOut->push_back(g);
            }
        }

        /** \brief get the maximum and minimum x-value over all graphs in the plot
         *  \param[out] minx smallest x value
         *  \param[out] maxx largest x value
         *  \param[out] smallestGreaterZero smallest data point which is >0 (needed for scaling of logarithmic axes)
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        void getGraphsXMinMax(double& minx, double& maxx, double& smallestGreaterZero);
        /** \brief get the maximum and minimum y-value over all graphs in the plot
         *  \param[out] miny smallest y value
         *  \param[out] maxy largest y value
         *  \param[out] smallestGreaterZero smallest data point which is >0 (needed for scaling of logarithmic axes)
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        void getGraphsYMinMax(double& miny, double& maxy, double& smallestGreaterZero);


        /** \brief denotes, which axes to synchronize in synchronizeToMaster() */
        enum SynchronizationDirection {
            sdXAxis, /**< \brief x-axis only */
            sdYAxis, /**< \brief y-axis only */
            sdXYAxes /**< \brief x- and y-axis */
        };

        /*! \brief synchronize the plot borders (and zooming) with a given plotter (master --> slave/this)

            This function allows two plotters to draw a graph with exactly the same height or width
            as in another graph. For example if you want to have two plotters which are positioned one
            above the other (and have the same widget widths, which could be guaranteed by a QLayout)
            you may want to make sure that their plotWidth s are always the same. In this case call
            \code plotter2->synchronizeToMaster(plotter1, sdXAxis, true) \endcode of the lower plotter \c plotter2 .
            Now whenever the size of plotter1 changes, also plotter2 is redrawn with the changed
            borders.

            \param master the plotter widget to synchronize to
            \param synchronizeDirection direction in which to synchronize
            \param synchronizeAxisLength do you want the axis length to be synchronized?
            \param synchronizeZoomingMasterToSlave if set, also zooming in the master leads to a modification of the linked axes in the slave
            \param synchronizeZoomingSlaveToMaster if set, also zooming in the slave leads to a modification of the linked axes in the master

            \see synchronizeXToMaster(), synchronizeYToMaster(), \ref JKQTBASEPLOTTER_SYNCMULTIPLOT
        */
        void synchronizeToMaster(JKQTBasePlotter* master, SynchronizationDirection synchronizeDirection, bool synchronizeAxisLength=true, bool synchronizeZoomingMasterToSlave=true, bool synchronizeZoomingSlaveToMaster=true);

        /*! \brief synchronize the plot x-axis width (and x-zooming) with a given master plotter (master --> slave/this)

            \param master the plotter widget to synchronize to
            \param synchronizeAxisLength do you want the axis length to be synchronized?
            \param synchronizeZoomingMasterToSlave if set, also zooming in the master leads to a modification of the linked axes in the slave
            \param synchronizeZoomingSlaveToMaster if set, also zooming in the slave leads to a modification of the linked axes in the master

            \note This is a short-cut to synchronizeToMaster() with \c synchronizeDirection=csXAxis
            \see synchronizeToMaster(), synchronizeYToMaster(), \ref JKQTBASEPLOTTER_SYNCMULTIPLOT
        */
        void synchronizeXToMaster(JKQTBasePlotter* master, bool synchronizeAxisLength=true, bool synchronizeZoomingMasterToSlave=true, bool synchronizeZoomingSlaveToMaster=true);

        /*! \brief synchronize the plot y-axis height (and y-zooming) with a given master plotter (master --> slave/this)

            \param master the plotter widget to synchronize to
            \param synchronizeAxisLength do you want the axis length to be synchronized?
            \param synchronizeZoomingMasterToSlave if set, also zooming in the master leads to a modification of the linked axes in the slave
            \param synchronizeZoomingSlaveToMaster if set, also zooming in the slave leads to a modification of the linked axes in the master

            \note This is a short-cut to synchronizeToMaster() with \c synchronizeDirection=csXAxis
            \see synchronizeToMaster(), synchronizeXToMaster(), \ref JKQTBASEPLOTTER_SYNCMULTIPLOT
        */
        void synchronizeYToMaster(JKQTBasePlotter* master, bool synchronizeAxisLength=true, bool synchronizeZoomingMasterToSlave=true, bool synchronizeZoomingSlaveToMaster=true);

        /** \brief switches any synchronization off, that has been created by synchronizeToMaster() */
        void resetMasterSynchronization(SynchronizationDirection synchronizeDirection=sdXYAxes);








        /** \brief save the current plot data as a Comma Separated Values (CSV) file
         *
         * \param filename the file to save to, if \a filename is empty, a file open dialog will be shown
         * \param decimalSeparator decimal separator for outpu
         * \param commentInitializer line-start for comment lines
         */
        void saveAsCSV(const QString& filename, const QString& decimalSeparator, const QString& commentInitializer);

        /** \brief save the current plot data as a Semicolon Separated Values (SSV) file
         *
         * \param filename the file to save to, if \a filename is empty, a file open dialog will be shown
         * \param decimalSeparator decimal separator for outpu
         * \param commentInitializer line-start for comment lines
         */
        void saveAsSemicolonSV(const QString& filename, const QString& decimalSeparator, const QString& commentInitializer);        /** \brief save the current plot data as a Tabulator Separated Values (CSV) file
         *
         * \param filename the file to save to, if \a filename is empty, a file open dialog will be shown
         * \param decimalSeparator decimal separator for outpu
         * \param commentInitializer line-start for comment lines
         */
        void saveAsTabSV(const QString& filename, const QString& decimalSeparator, const QString& commentInitializer);



        /** \brief internal tool class for text sizes
         *   \ingroup jkqtpplottersupprt
         *  \internal */
        struct JKQTPLOTTER_LIB_EXPORT textSizeData {
            explicit textSizeData();
            double ascent, descent, width, strikeoutPos;
        };

        /** \brief internal tool class for text-sizess in a plot key
         *  \ingroup jkqtpplottersupprt
         *  \internal */
        struct JKQTPLOTTER_LIB_EXPORT textSizeKey {
            explicit textSizeKey(const QFont& f, const QString& text, QPaintDevice *pd);
            explicit textSizeKey(const QString& fontName, double fontSize, const QString& text, QPaintDevice *pd);
            QString text;
            QFont f;
            int ldpiX, ldpiY, pdpiX, pdpiY;

            bool operator==(const textSizeKey& other) const;
        };
        /** \brief calculates a textSizeData for the given string \a text if it would be drawn on \a painter with font \a fm
         * \internal
         */
        textSizeData getTextSizeDetail(const QFont &fm, const QString& text,  QPainter& painter);
        /** \brief calculates a textSizeData for the given string \a text if it would be drawn on \a painter with font \a fontName
         * \internal
         */
        textSizeData getTextSizeDetail(const QString& fontName, double fontSize, const QString& text,  QPainter &painter);
        /** \brief calculates a text-size details for the given string \a text if it would be drawn on \a painter with font \a fontName and font size \a fontSize
         * \internal
         */
        void getTextSizeDetail(const QString& fontName, double fontSize, const QString& text,  QPainter &painter, double& width, double& ascent, double& descent, double& strikeoutPos);
        /** \brief calculates a text-size details for the given string \a text if it would be drawn on \a painter with font \a fm
         * \internal
         */
        void getTextSizeDetail(const QFont& fm, const QString& text,  QPainter &painter, double& width, double& ascent, double& descent, double& strikeoutPos);
        /** \brief calculates a size of the given string \a text if it would be drawn on \a painter with font \a fm
         * \internal
         */
        QSizeF getTextSizeSize(const QFont &fm, const QString& text,   QPainter& painter);
        /** \brief calculates a size of the given string \a text if it would be drawn on \a painter with font \a fontName and font size \a fontSize
         * \internal
         */
        QSizeF getTextSizeSize(const QString& fontName, double fontSize, const QString& text,  QPainter &painter);


        /** \brief takes a new axis range \a xminn ... \a xmaxx and \a yminn ... \a ymaxx and corrects the values to match the currently set axisAspectRatio
         *
         *  This function is used by setXY(), setX(), setY().
         *
         *  \see axisAspectRatio, setXY(), setX(), setY()
         */
        void correctXYRangeForAspectRatio(double &xminn, double &xmaxx, double &yminn, double &ymaxx) const;
    signals:
        /** \brief signal: emitted whenever the user selects a new x-y zoom range (by mouse) */
        void zoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQTBasePlotter* sender);

        /** \brief emitted when the plot has to be updated */
        void plotUpdated();

        /** \brief emitted when the plot scaling had to be recalculated */
        void plotScalingRecalculated();

        /** \brief emitted before the plot scaling has been recalculated */
        void beforePlotScalingRecalculate();

        /** \brief emitted just before exporting the current plot as image, or just before the export preview dialog is shown
         *
         *  This signal can be used to e.g. modify the plotter settings before an export.
         *
         *  \see afterExporting(), beforePrinting(), afterPrinting()
         */
        void beforeExporting();

        /** \brief emitted just before exporting the current plot as image, or just before the export preview dialog is shown
         *
         *  This signal can be used to e.g. modify the plotter settings after an export.
         *
         *  \see beforeExporting(), beforePrinting(), afterPrinting()
         */
        void afterExporting();

        /** \brief emitted just before Printing the current plot as image, or just before the print preview dialog is shown
         *
         *  This signal can be used to e.g. modify the plotter settings before a print.
         *
         *  \see afterPrinting(), beforeExporting(), afterExporting()
         */
        void beforePrinting();

        /** \brief emitted just before Printing the current plot as image, or just before the print preview dialog is shown
         *
         *  This signal can be used to e.g. modify the plotter settings after a print.
         *
         *  \see beforePrinting(), beforeExporting(), afterExporting()
         */
        void afterPrinting();


    public slots:


        /** \brief sets the width of the plot widget
         *
         * \image html plot_widget_orientation.png
         *
         * \see setWidgetSize(), \ref jkqtplotter_base_plotsize
         */
        void setWidgetWidth(int wid);

        /** \brief resize the plot
         *
         * \image html plot_widget_orientation.png
         *
         * \see \ref jkqtplotter_base_plotsize
         */
        void setWidgetSize(int width, int height);

        /** \brief sets the width of the plot widget
         *
         * \image html plot_widget_orientation.png
         *
         * \see setWidgetSize(), \ref jkqtplotter_base_plotsize
         */
        void setWidgetHeight(int heigh);

        /** \brief sets the borders of the plot
         *
         * \image html plot_widget_orientation.png
         *
         * \see \ref jkqtplotter_base_plotsize
         */
        void setPlotBorder(int left, int right, int top, int bottom);

        /** \brief sets the left border of the plot
         *
         * \image html plot_widget_orientation.png
         *
         * \see \ref jkqtplotter_base_plotsize
         */
        void setPlotBorderLeft(int left);

        /** \brief sets the right border of the plot
         *
         * \image html plot_widget_orientation.png
         *
         * \see \ref jkqtplotter_base_plotsize
         */
        void setPlotBorderRight(int right);

        /** \brief sets the top border of the plot
         *
         * \image html plot_widget_orientation.png
         *
         * \see \ref jkqtplotter_base_plotsize
         */
        void setPlotBorderTop(int top);

        /** \brief sets the bottom border of the plot
         *
         * \image html plot_widget_orientation.png
         *
         * \see \ref jkqtplotter_base_plotsize
         */
        void setPlotBorderBottom(int bottom);

        /** \brief sets the x-range of the plot (minimum and maximum x-value on the x-axis)
         *
         *  \param xminn absolute minimum of x-axis
         *  \param xmaxx absolute maximum of x-axis
         *
         * \note You cannot expand the x-range outside the absolute x-range set e.g. by setAbsoluteX()!
         *       Also the range will be limited to possible values (e.g. to positive values if you use
         *       logarithmic axes).
         *
         * Uppon setting, this function emits the signal zoomChangedLocally(), if emitting signals
         * is activated at the moment (e.g. using setEmittingSignalsEnabled() ).
         *
         * \see setY(), setXY(), zoomToFit(), setAbsoluteXY(), JKQTPlotter::setY()
         */
        void setX(double xminn, double xmaxx);

        /** \brief sets the y-range of the plot (minimum and maximum y-value on the y-axis)
         *
         *  \param yminn absolute minimum of y-axis
         *  \param ymaxx absolute maximum of y-axis
         *
         * \note You cannot expand the y-range outside the absolute y-range set e.g. by setAbsoluteY()!
         *       Also the range will be limited to possible values (e.g. to positive values if you use
         *       logarithmic axes).
         *
         * Uppon setting, this function emits the signal zoomChangedLocally(), if emitting signals
         * is activated at the moment (e.g. using setEmittingSignalsEnabled() ).
         *
         * \see setX(), setXY(), zoomToFit(), setAbsoluteXY(), JKQTPlotter::setX()
         */
        void setY(double yminn, double ymaxx);

        /** \brief sets the x- and y-range of the plot (minimum and maximum values on the x-/y-axis)
         *
         *  \param xminn absolute minimum of x-axis
         *  \param xmaxx absolute maximum of x-axis
         *  \param yminn absolute minimum of y-axis
         *  \param ymaxx absolute maximum of y-axis
         *
         * \note You cannot expand the ranges outside the absolute ranges set e.g. by setAbsoluteXY()!
         *       Also the range will be limited to possible values (e.g. to positive values if you use
         *       logarithmic axes).
         *
         * Uppon setting, this function emits the signal zoomChangedLocally(), if emitting signals
         * is activated at the moment (e.g. using setEmittingSignalsEnabled() ).
         *
         * \see setX(), setX(), zoomToFit(), setAbsoluteXY(), JKQTPlotter::setXY()
         */
        void setXY(double xminn, double xmaxx, double yminn, double ymaxx);

        /** \brief sets absolutely limiting x-range of the plot
         *
         *  \param xminn absolute minimum of x-axis
         *  \param xmaxx absolute maximum of x-axis
         *
         * \note if the aspect ratio of this does not fit into the widget, it is possible that you don't see the complete contents!
         *
         * \see setAbsoluteXY(), setAbsoluteY(), JKQTPlotter::setAbsoluteX()
         */
        void setAbsoluteX(double xminn, double xmaxx);

        /** \brief sets absolute minimum and maximum y-value to plot
         *
         *  \param yminn absolute minimum of y-axis
         *  \param ymaxx absolute maximum of y-axis
         *
         * \note if the aspect ratio of this does not fit into the widget, it is possible that you don't see the complete contents!
         *
         * \see setAbsoluteXY(), setAbsoluteX(), JKQTPlotter::setAbsoluteY()
         */
        void setAbsoluteY(double yminn, double ymaxx);

        /** \brief sets absolutely limiting x- and y-range of the plot
         *
         * The user (or programmer) cannot zoom to a viewport that is larger than the range given to this function.
         *
         *  \param xminn absolute minimum of x-axis
         *  \param xmaxx absolute maximum of x-axis
         *  \param yminn absolute minimum of y-axis
         *  \param ymaxx absolute maximum of y-axis
         *
         * \note if the aspect ratio of this does not fit into the widget, it is possible that you don't see the complete contents!
         *
         * \see setAbsoluteX(), setAbsoluteY(), zoomToFit(), JKQTPlotter::setAbsoluteXY()
         */
        void setAbsoluteXY(double xminn, double xmaxx, double yminn, double ymaxx);


        /** \brief sets x/ymin and x/ymax to the supplied values and replots the graph (zoom operation!) */
        void zoom(double nxmin, double nxmax, double nymin, double nymax);

        /** \brief sets whether to plot grid lines or not */
        void setGrid(bool val);

        /** \brief sets the color of all Major grid lines
         * */
        void setGridColor(QColor color);

        /** \brief sets the color of all minor grid lines
         * */
        void setMinorGridColor(QColor color);

        /** \brief sets the width of all Major grid lines
         * */
        void setGridWidth(double __value);

        /** \brief sets the width of all minor grid lines
         * */
        void setMinorGridWidth(double __value);

        /** \brief sets the style of all Major grid lines
         * */
        void setGridStyle(Qt::PenStyle __value);

        /** \brief sets the style of all minor grid lines
         * */
        void setMinorGridStyle(Qt::PenStyle __value);


        /** \brief switches the visibility of the zero-axes associated with the x- and y-axis
         *
         * \param showX indicates whether to show the zero-axis associated with the x-axis (i.e. x==0 or the vertical zero-axis)
         * \param showY indicates whether to show the zero-axis associated with the y-axis (i.e. y==0 or the horizontal zero-axis)
         * */
        void setShowZeroAxes(bool showX, bool showY);
        /** \brief switches the visibility of the zero-axes associated with the x- and y-axis
         *
         * \param showXY indicates whether to show the zero-axis associated with the x- and y-axis
         * */
        void setShowZeroAxes(bool showXY);

        /** \brief save the current plot as a pixel image image (PNG ...), if filename is empty a file selection dialog is displayed */
        void saveAsPixelImage(const QString& filename=QString(""), bool displayPreview=true, const QByteArray &outputFormat=QByteArray());

        /** \brief save the current plot as a pixel image into a QImage with the given size */
        QImage grabPixelImage(QSize size=QSize(), bool showPreview=false);
        /** \brief copy the current plot as a pixel+svg image to the clipboard */
        void copyPixelImage(bool showPreview=true);

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        /** \brief save the current plot as a SVG file, with the current widget aspect ratio, if filename is empty a file selection dialog is displayed
         *
         *  \note Exporting to SVG requires QPrinter-support, if it is not available on your platform, this function will not be available either!
         */
        void saveAsSVG(const QString& filename=QString(""), bool displayPreview=true);

        /** \brief save the current plot as a PDF file, with the current widget aspect ratio, if filename is empty a file selection dialog is displayed
         *
         *  \note Exporting to PDF requires QPrinter-support, if it is not available on your platform, this function will not be available either!
         */
        void saveAsPDF(const QString& filename=QString(""), bool displayPreview=true);
#endif

        /** \brief save the current plot as an image file, with the current widget aspect ratio, if filename is empty a file selection dialog is displayed.
        *          The image format is extracted from the file extension (jpeg, tiff, png, pdf, ...) */
        void saveImage(const QString& filename=QString(""), bool displayPreview=true);

        /** \brief save the data used for the current plot. The file format is extracted from the file extension (csv, ...)
         *
         * The parameter \a format specifies the export format. if it is empty the format will be choosen according to the file extension, or
         * if \a filename is also empty the format will be choosen according to what is selected in the file selection dialog.
         *
         * If \a format is \c "slk" the output will be in SYLK format, if \a format is \c "csv" or \a "dat" the output will be comma separated values
         * and if \a format is \c "txt" the output will be tab separated values.
         */
        void saveData(const QString& filename=QString(""), const QString& jkqtp_format=QString(""));
        /** \brief copy the data used for the current plot to the clipboard
         *
         *  copies data as tab separated data with the system-decimal point.
         */
        void copyData();
        /** \brief copy the data used for the current plot to the clipboard in Matlab format
         */
        void copyDataMatlab();
        /** \brief save the current plot data as a Comma Separated Values (CSV) file
         *
         * \param filename the file to save to, if \a filename is empty, a file open dialog will be shown
         *
         * \note this function uses  CSVdecimalSeparator as decimal separator and CSVcommentInitializer to initialize content lines
         */
        void saveAsCSV(const QString& filename=QString(""));

        /** \brief save the current plot data as a Semicolon Separated Values (SSV) file
         *
         * \param filename the file to save to, if \a filename is empty, a file open dialog will be shown
         *
         * \note this function uses  CSVdecimalSeparator as decimal separator and CSVcommentInitializer to initialize content lines
         */
        void saveAsSemicolonSV(const QString& filename=QString(""));

        /** \brief save the current plot data as a Tabulator Separated Values (CSV) file
         *
         * \param filename the file to save to, if \a filename is empty, a file open dialog will be shown
         *
         * \note this function uses  CSVdecimalSeparator as decimal separator and CSVcommentInitializer to initialize content lines
         */
        void saveAsTabSV(const QString& filename=QString(""));

        /** \brief save the current plot data as a DIF file
         *
         * \param filename the file to save to, if \a filename is empty, a file open dialog will be shown
         */
        void saveAsDIF(const QString& filename=QString(""));

        /** \brief save the current plot data as a SYLK spreadsheet file
         *
         * \param filename the file to save to, if \a filename is empty, a file open dialog will be shown
         */
        void saveAsSYLK(const QString& filename=QString(""));

        /** \brief save the current plot data as a Matlab Script
         *
         * \param filename the file to save to, if \a filename is empty, a file open dialog will be shown
         */
        void saveAsMatlab(const QString& filename=QString(""));

        /** \brief save the current plot data as a Semicolon Separated Values (CSV) file for german Excel, i.e. with comma as decimal separator
         *
         * \param filename the file to save to, if \a filename is empty, a file open dialog will be shown
         */
        void saveAsGerExcelCSV(const QString& filename=QString(""));

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        /** \brief print the current plot, if printer is \c nullptr a printer selection dialog is displayed
         *
         *  \note This function is only available on platforms with QPrinter support!
         */
        void print(QPrinter* printer=nullptr, bool displayPreview=true);
#endif

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
        void zoomToFit(bool zoomX=true, bool zoomY=true, bool includeX0=false, bool includeY0=false, double scaleX=1.05, double scaleY=1.05);

        /** \brief zooms into the graph (the same as turning the mouse wheel) by the given factor */
        void zoomIn(double factor=2.0);
        /** \brief zooms out of the graph (the same as turning the mouse wheel) by the given factor */
        void zoomOut(double factor=2.0);

        /** \brief en-/disables the maintaining of the data aspect ratio \see aspectRatio */
        void setMaintainAspectRatio(bool value);

        /** \brief en-/disables the maintaining of the axis aspect ratio \see axisAspectRatio */
        void setMaintainAxisAspectRatio(bool value);

        /** \brief set filename and prefix, used by loadUserSettings() and saveUserSettings()
         *
         * \see loadUserSettings(), saveUserSettings(), setUserSettigsPrefix(), getUserSettigsFilename(), getUserSettigsPrefix()
         */
        void setUserSettigsFilename(const QString& filename, const QString& prefix);
        /** \brief set filename, used by loadUserSettings() and saveUserSettings()
         *
         * \see loadUserSettings(), saveUserSettings(), setUserSettigsPrefix(), getUserSettigsFilename(), getUserSettigsPrefix()
         */
        void setUserSettigsFilename(const QString& filename);
        /** \brief set prefix, used by loadUserSettings() and saveUserSettings()
         *
         * \see loadUserSettings(), saveUserSettings(), setUserSettigsPrefix(), getUserSettigsFilename(), getUserSettigsPrefix()
         */
        void setUserSettigsPrefix(const QString& prefix);
        /** \brief return the filename, used by loadUserSettings() and saveUserSettings()
         *
         * \see loadUserSettings(), saveUserSettings(), setUserSettigsPrefix(), getUserSettigsFilename(), getUserSettigsPrefix()
         */
        QString getUserSettigsFilename() const;
        /** \brief return the prefix, used by loadUserSettings() and saveUserSettings()
         *
         * \see loadUserSettings(), saveUserSettings(), setUserSettigsPrefix(), getUserSettigsFilename(), getUserSettigsPrefix()
         */
        QString getUserSettigsPrefix() const;

        /** \brief loads the plot user properties from the file sepcified by setUserSettigsFilename() and the prefix specified by setUserSettigsPrefix()
         *
         * \see  saveUserSettings(), setUserSettigsPrefix(), getUserSettigsFilename(), getUserSettigsPrefix()
         */
        void loadUserSettings();

        /** \brief saves the plot user properties to the file sepcified by setUserSettigsFilename() and the prefix specified by setUserSettigsPrefix()
         *
         * \see  loadUserSettings(), setUserSettigsPrefix(), getUserSettigsFilename(), getUserSettigsPrefix()
         */
        void saveUserSettings() const;

        /** \brief set all graphs invisible, except i */
        void setOnlyGraphVisible(int i);

        /** \brief set all graphs invisible, except graph start, start+n, start+2*n, ... */
        void setOnlyNthGraphsVisible(int start, int n);

        /** \brief sets the data aspect ratio, enforced with setMaintainApsectRatio(true) \see aspectRatio */
        void setAspectRatio(double __value);
        /** \brief sets the axis aspect ratio, enforced with setMaintainAxisApsectRatio(true) \see axisAspectRatio */
        void setAxisAspectRatio(double __value);
        /** \copydoc JKQTBasePlotterStyle::useAntiAliasingForSystem */
        void setUseAntiAliasingForSystem(bool __value);
        /** \copydoc JKQTGraphsBaseStyle::useAntiAliasingForGraphs */
        void setUseAntiAliasingForGraphs(bool __value);
        /** \copydoc JKQTBasePlotterStyle::useAntiAliasingForText */
        void setUseAntiAliasingForText(bool __value);
        /** \copydoc JKQTBasePlotterStyle::widgetBackgroundBrush */
        void setBackgroundColor(const QColor & __value);
        /** \copydoc JKQTBasePlotterStyle::exportBackgroundBrush */
        void setExportBackgroundColor(const QColor & __value);
        /** \copydoc JKQTBasePlotterStyle::plotBackgroundBrush */
        void setPlotBackgroundColor(const QColor & __value);
        /** \copydoc JKQTBasePlotterStyle::widgetBackgroundBrush */
        void setBackgroundBrush(const QBrush & __value);
        /** \copydoc JKQTBasePlotterStyle::exportBackgroundBrush */
        void setExportBackgroundBrush(const QBrush & __value);
        /** \copydoc JKQTBasePlotterStyle::plotBackgroundBrush */
        void setPlotBackgroundBrush(const QBrush & __value);
        /** \copydoc JKQTBasePlotterStyle::widgetBackgroundBrush */
        void setBackgroundGradient(const QGradient & __value);
        /** \copydoc JKQTBasePlotterStyle::exportBackgroundBrush */
        void setExportBackgroundGradient(const QGradient & __value);
        /** \copydoc JKQTBasePlotterStyle::plotBackgroundBrush */
        void setPlotBackgroundGradient(const QGradient & __value);
        /** \copydoc JKQTBasePlotterStyle::widgetBackgroundBrush */
        void setBackgroundTexture(const QPixmap & __value);
        /** \copydoc JKQTBasePlotterStyle::exportBackgroundBrush */
        void setExportBackgroundTexture(const QPixmap & __value);
        /** \copydoc JKQTBasePlotterStyle::plotBackgroundBrush */
        void setPlotBackgroundTexture(const QPixmap & __value);
        /** \copydoc JKQTBasePlotterStyle::widgetBackgroundBrush */
        void setBackgroundTexture(const QImage & __value);
        /** \copydoc JKQTBasePlotterStyle::exportBackgroundBrush */
        void setExportBackgroundTexture(const QImage & __value);
        /** \copydoc JKQTBasePlotterStyle::plotBackgroundBrush */
        void setPlotBackgroundTexture(const QImage & __value);
        /** \copydoc JKQTPKeyStyle::textColor */
        void setKeyTextColor(const QColor & __value);


        /** \copydoc JKQTBasePlotterStyle::plotFrameWidth */
        void setPlotFrameWidth(double __value);
        /** \copydoc JKQTBasePlotterStyle::plotFrameRounding */
        void setPlotFrameRounding(double __value);
        /** \copydoc JKQTBasePlotterStyle::plotFrameColor */
        void setPlotFrameColor(QColor col);
        /** \copydoc JKQTBasePlotterStyle::plotFrameVisible */
        void setPlotFrameVisible(bool enabled);


        /** \copydoc JKQTPKeyStyle::fontSize */
        void setKeyFontSize(double __value);
        /** \copydoc JKQTPKeyStyle::itemWidth */
        void setKeyItemWidth(double __value);
        /** \copydoc JKQTPKeyStyle::itemHeight */
        void setKeyItemHeight(double __value);
        /** \copydoc JKQTPKeyStyle::ySeparation */
        void setKeyYSeparation(double __value);
        /** \copydoc JKQTPKeyStyle::sampleLineLength */
        void setKeyLineLength(double __value);
        /** \copydoc JKQTPKeyStyle::xMargin */
        void setKeyXMargin(double __value);
        /** \copydoc JKQTPKeyStyle::yMargin */
        void setKeyYMargin(double __value);
        /** \copydoc JKQTPKeyStyle::xSeparation */
        void setKeyXSeparation(double __value);
        /** \copydoc JKQTPKeyStyle::yOffset */
        void setKeyXOffset(double __value);
        /** \copydoc JKQTPKeyStyle::xOffset */
        void setKeyYOffset(double __value);
        /** \copydoc JKQTPKeyStyle::visible */
        void setShowKey(bool __value);
        /** \copydoc JKQTPKeyStyle::frameVisible */
        void setShowKeyFrame(bool __value);
        /** \copydoc JKQTPKeyStyle::frameColor */
        void setKeyFrameColor(const QColor & __value);
        /** \copydoc JKQTPKeyStyle::backgroundBrush */
        void setKeyBackgroundColor(const QColor & __value, Qt::BrushStyle __style);
        /** \copydoc JKQTPKeyStyle::backgroundBrush */
        void setKeyBackgroundBrush(const QBrush & __value);
        /** \copydoc JKQTPKeyStyle::backgroundBrush */
        void setKeyBackgroundGradient(const QGradient & __value);
        /** \copydoc JKQTPKeyStyle::backgroundBrush */
        void setKeyBackgroundTexture(const QImage & __value);
        /** \copydoc JKQTPKeyStyle::backgroundBrush */
        void setKeyBackgroundTexture(const QPixmap & __value);
        /** \copydoc JKQTPKeyStyle::frameWidth */
        void setKeyFrameWidth(double __value);
        /** \copydoc JKQTPKeyStyle::frameRounding */
        void setKeyFrameRounding(double __value);
        /** \copydoc JKQTPKeyStyle::autosize */
        void setKeyAutosize(bool __value);
        /** \copydoc JKQTPKeyStyle::position */
        void setKeyPosition(const JKQTPKeyPosition & __value);
        /** \copydoc JKQTPKeyStyle::layout */
        void setKeyLayout(const JKQTPKeyLayout & __value);
        /** \copydoc JKQTBasePlotterStyle::plotLabelFontSize */
        void setPlotLabelFontSize(double __value);
        /** \copydoc JKQTBasePlotterStyle::plotLabelFontName */
        void setplotLabelFontName(const QString & __value);
        /** \brief set the plot label text */
        void setPlotLabel(const QString & __value);
        /** \copydoc JKQTBasePlotterStyle::defaultTextColor */
        void setDefaultTextColor(QColor __value) ;
        /** \copydoc JKQTBasePlotterStyle::defaultFontSize */
        void setDefaultTextSize(double __value) ;
        /** \copydoc JKQTBasePlotterStyle::defaultFontName */
        void setDefaultTextFontName(const QString& __value) ;
        /** \brief sets the current directory in which to open SaveAs ... dialogs */
        void setCurrentSaveDirectory(const QString & __value);
        /** \brief set the file format to use in SaveAs ... dialogs */
        void setCurrentFileFormat(const QString & __value);
        /** \brief set the decimal separator used when exporting data to text files */
        void setCSVdecimalSeparator(const QString & __value);
        /** \brief set the string used to introduce comments in text output when exporting data */
        void setCSVcommentInitializer(const QString & __value);

        /** \brief if set \c true (default: \c false ) the JKQTBasePlotter draws colored rectangles to indicate the different regions in the plot (border, axes, ...)
         *
         * \image html debugShowRegionBoxes.png
         *
         * \see JKQTBasePlotterStyle::debugShowRegionBoxes, isDebugShowRegionBoxesEnabled()
         */
        void enableDebugShowRegionBoxes(bool enabled=true);

        /** \brief if set \c true (default: \c false ) the JKQTBasePlotter draws colored rectangles to indicate the extent of text in the plot
         *
         * \image html debugShowTextBoxes.png
         *
         * \see JKQTBasePlotterStyle::debugShowTextBoxes, isDebugShowRegionTextEnabled()
         */
        void enableDebugShowTextBoxes(bool enabled=true);
    protected:
        /** \brief this is a cache for JKQTBasePlotter::textSizeData calculated with JKQTBasePlotter::getTextSizeDetail() */
        static QHash<JKQTBasePlotter::textSizeKey, JKQTBasePlotter::textSizeData> s_TextSizeDataCache;







        /** \brief paints the plot onto the given JKQTPEnhancedPainter object
         *
         *  \param painter JKQTPEnhancedPainter to draw on
         */
        void drawPlot(JKQTPEnhancedPainter& painter);
        /** \brief simply calls paintPlot() if grid printing mode is deactivated and prints the graph grid otherwise
         *         \a pageRect is used to determine the size of the page to draw on. If this does not coincide with
         *         the widget extents this function calculates a scaling factor so the graphs fit onto the page. This
         *         is especially usefull when printing!
         *
         *  \param painter JKQTPEnhancedPainter to draw on
         *  \param pageRect size of the page
         *  \param scaleIfTooLarge scale image if it is too large for pageRect
         *  \param scaleIfTooSmall scale image if it is smaller than pageRect
         */
        void gridPaint(JKQTPEnhancedPainter& painter, QSizeF pageRect, bool scaleIfTooLarge=true, bool scaleIfTooSmall=true);

        /** \brief This method goes through all registered plotters and calculates the width of every column and
         *      height of every row as the max over the row/column. The reults are stored in the private datamembers
         *      gridPrintingRows and gridPrintingColumns. gridPrintingSize will contain the size of all graphs together
         *      afterwards. */
        void gridPrintingCalc();

        /** \brief plot the grid */
        void drawSystemGrid(JKQTPEnhancedPainter& painter);
        /** \brief plot the x coordinate axis (incl. labels) */
        void drawSystemXAxis(JKQTPEnhancedPainter& painter);
        /** \brief plot the x coordinate axis (incl. labels) */
        void drawSystemYAxis(JKQTPEnhancedPainter& painter);
        /** \brief plot the graphs */
        void drawGraphs(JKQTPEnhancedPainter& painter);
        /** \brief plot a key */
        void drawKey(JKQTPEnhancedPainter& painter);

        /** \brief plot the key contents
         *
         * This function does not do anything and has to be overwritten in child classes if they want to plot a key.
         * The implementation should draw the contents of the key, not its frame as this is done in plotKey().
         *
         * \param painter draw the key on this painter
         * \param x left-most coordinate of the key [pixels]
         * \param y top-most coordinate of the key [pixels]
         * \param width width of the key [pixels]
         * \param height height of the key [pixels]
         *
         * So any implementation should only draw inside the rectangle <code>[x..x+width, y..y+hieght]</code>
         */
        void drawKeyContents(JKQTPEnhancedPainter& painter, double x, double y, double width, double height);

        /** \brief returns the size of the key (as call-by-reference parameters
         *
         * Any implementation of key plotting has to overwrite this function and use it to return the size of the key that would be
         * plotted by plotKeyContents(). This class will use the return values to plot the frame of the key and also supply them to
         * plotKeyContents(). If height or width are returned 0, no key is plotted
         *
         * The implementation in here returns zero size!
         */
        void getKeyExtent(JKQTPEnhancedPainter& painter, double *width, double *height, double *text_width=nullptr, double *text_height=nullptr, int *columns_count=nullptr, int* lines_count=nullptr);

        /** \brief show the print preview window for a given print \a p */
        bool printpreviewNew(QPaintDevice* paintDevice, bool setAbsolutePaperSize=false, double printsizeX_inMM=-1.0, double printsizeY_inMM=-1.0, bool displayPreview=true);

        /** \brief show the export preview window for a given page size \a pageSize, either in pixels (\a unitIsMM \c ==false ) or in millimeters (\a unitIsMM \c ==true ) */
        bool exportpreview(QSizeF pageSize, bool unitIsMM=false);

        /** \copydoc fontSizeMultiplier */
        void setFontSizeMultiplier(double __value);
        /** \copydoc lineWidthMultiplier */
        void setLineWidthMultiplier(double __value);
        /** \copydoc printMagnification */
        void setPrintMagnification(double __value);
        /** \copydoc printMagnification */
        double getPrintMagnification() const;
        /** \copydoc paintMagnification */
        void setPaintMagnification(double __value);
        /** \copydoc paintMagnification */
        double getPaintMagnification() const;


    protected slots:
        /** \brief internal function for print/export preview
         * \internal
         */
        void updatePreviewLabel();

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewPaintRequested(QPrinter *printer);
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewPaintRequestedNewPrinter(QPrinter *printer);
#endif
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewPaintRequestedNewPaintDevice(QPaintDevice *paintDevice);
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewSetZoom(double value);
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewSetSizeX(double value);
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewSetSizeY(double value);
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewSetSizeXNew(double value);
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewSetSizeYNew(double value);
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewSetMagnification(double value);
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewSetMagnificationNew(double value);
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewSetAspectRatio(bool checked);
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewSetKeepAbsFontsize(bool checked);
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewToggleMagnification(bool checked);
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewSetLineWidthMultiplier(double value);
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewSetFontSizeMultiplier(double value);
        /** \brief internal function for print preview
         * \internal
         */
        void printpreviewUpdate();

        /** \brief internal function for export preview
         * \internal
         */
        void exportpreviewPaintRequested(JKQTPEnhancedPainter& painter, QSize size);

        /** \brief internal function for getDataColumnsByUser()
         * \internal
         */
        void getDataColumnsByUserCheckAll();
        /** \brief internal function for getDataColumnsByUser()
         * \internal
         */
        void getDataColumnsByUserCheckNone();
        /** \brief internal function for getDataColumnsByUser()
         * \internal
         */
        void getDataColumnsByUserSave();
        /** \brief internal function for getDataColumnsByUser()
         * \internal
         */
        void getDataColumnsByUserComboBoxSelected(const QString& name);
        /** \brief internal function for getDataColumnsByUser()
         * \internal
         */
        void getDataColumnsByUserItemChanged(QListWidgetItem* widgetitem);
        /** \brief internal function for getDataColumnsByUser()
         * \internal
         */
        void showPlotData();
        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local x-axis to the other x-axis */
        void synchronizeXAxis(double newxmin, double newxmax, double newymin, double newymax, JKQTBasePlotter* sender);
        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local y-axis to the other y-axis */
        void synchronizeYAxis(double newxmin, double newxmax, double newymin, double newymax, JKQTBasePlotter* sender);
        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local x- and y-axis to the other x- and y-axis */
        void synchronizeXYAxis(double newxmin, double newxmax, double newymin, double newymax, JKQTBasePlotter* sender);

    protected:
        /** \brief current style properties for this JKQTBasePlotter instance
         *
         * \see JKQTPSetSystemDefaultBaseStyle(), JKQTPSetSystemDefaultBaseStyle(), getCurrentPlotterStyle(), \ref jkqtpplotter_styling
         */
        JKQTBasePlotterStyle plotterStyle;

        /** \brief copy sub-properties of plotterStyle to sub-objects that require it (axes, axes in graphs ...) and emit a plot update */
        void propagateStyle();

        /** \brief used to plot LaTeX markup */
        JKQTMathText mathText;

        /** \brief model representing all Plots in this plotter and showing their visible/invisible state */
        JKQTPGraphsModel* m_plotsModel;

        /** \brief object used for the x-axis */
        JKQTPHorizontalAxis* xAxis;
        /** \brief object used for the y-axis */
        JKQTPVerticalAxis* yAxis;

        /** \brief filename for the ini file in which to save the user settings
         *  \see jkqtplotter_base_userprops
         */
        QString userSettigsFilename;
        /** \brief prefix for the ini file in which to save the user settings
         *  \see jkqtplotter_base_userprops
         */
        QString userSettigsPrefix;


        /** \brief indicates whether the \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_GRIDPRINT "grid printing" is activated
         *
         * \see \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_GRIDPRINT
         */
        bool gridPrinting;
        /** \brief x position of the current graph in \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_GRIDPRINT "grid printing"
         *
         * \see \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_GRIDPRINT
         */
        size_t gridPrintingCurrentX;
        /** \brief y position of the current graph in \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_GRIDPRINT "grid printing"
         *
         * \see \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_GRIDPRINT
         */
        size_t gridPrintingCurrentY;
        /** \brief list that manages all the additional graphs for \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_GRIDPRINT "grid printing" mode */
        QList<JKQTPGridPrintingItem> gridPrintingList;
        /** \brief this list contains all the rows of the current \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_GRIDPRINT "grid printing" and stores its heights */
        QList<size_t> gridPrintingRows;
        /** \brief this list contains all the columns of the current \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_GRIDPRINT "grid printing" and stores its widths */
        QList<size_t> gridPrintingColumns;
        /** \brief size of all plots in \ref JKQTBASEPLOTTER_SYNCMULTIPLOT_GRIDPRINT "grid printing" mode, filled by gridPrintingCalc() */
        QSizeF gridPrintingSize;

        /** \brief this is an internal property that is used by the export/print system to control the "magnification". Usually this is 1.0
         *         but if set !=1.0, it is used to scale the widgetWidth and widgetHeight before painting! */
        double paintMagnification;

        /** \brief an object which manages all data columns for this plotter class */
        JKQTPDatastore* datastore;
        /** \brief indicates whether the datastore is managed (allocated/freed) internally or externally */
        bool datastoreInternal;


        /** \brief width of the plot widget
         *
         * \image html plot_widget_orientation.png
         *
         * \see setWidgetSize(), \ref jkqtplotter_base_plotsize
         */
        int widgetWidth;

        /** \brief height of the plot widget
         *
         * \image html plot_widget_orientation.png
         *
         * \see setWidgetSize(), \ref jkqtplotter_base_plotsize
         */
        int widgetHeight;



        /** \brief <b>calculated value:</b> free space between widget top border and plot top border, as used to plot the graph
         * \internal
         *
         * \note This property is an intermediate storage for calculated values. Do not change directly!
         */
        double internalPlotBorderTop;
        /** \brief <b>calculated value:</b> free space between widget top border and top border of the key/legend
         * \internal
         *
         * \note This property is an intermediate storage for calculated values. Do not change directly!
         */
        double internalPlotKeyBorderTop;
        /** \brief <b>calculated value:</b> height of the plot title (or 0 if no title)
         * \internal
         *
         * \note This property is an intermediate storage for calculated values. Do not change directly!
         */
        double internalTitleHeight;

        /** \brief <b>calculated value:</b> free space between widget top border and plot top border, as used to plot the graph
         * \internal
         *
         * \note This property is an intermediate storage for calculated values. Do not change directly!
          */
        double internalPlotBorderLeft;
        /** \brief <b>calculated value:</b> free space between widget left border and left border of the key/legend
         * \internal
         *
         * \note This property is an intermediate storage for calculated values. Do not change directly!
         */
        double internalPlotKeyBorderLeft;

        /** \brief <b>calculated value:</b> free space between widget top border and plot top border, as used to plot the graph
         * \internal
         *
         * \note This property is an intermediate storage for calculated values. Do not change directly!
          */
        double internalPlotBorderBottom;
        /** \brief <b>calculated value:</b> free space between widget bottom border and bottom border of the key/legend
         * \internal
         *
         * \note This property is an intermediate storage for calculated values. Do not change directly!
         */
        double internalPlotKeyBorderBottom;

        /** \brief <b>calculated value:</b> free space between widget top border and plot top border, as used to plot the graph
         * \internal
         *
         * \note This property is an intermediate storage for calculated values. Do not change directly!
          */
        double internalPlotBorderRight;
        /** \brief <b>calculated value:</b> free space between widget right border and right border of the key/legend
         * \internal
         *
         * \note This property is an intermediate storage for calculated values. Do not change directly!
         */
        double internalPlotKeyBorderRight;



        /** \brief <b>calculated value:</b> free space between widget top border and (plot+drawOutside) top border (including coordinate axes)
         * \internal
         *
         * \image html plot_widget_orientation.png
         *
         * \note This property is an intermediate storage for calculated values. Do not change directly!
         */
        double internalPlotBorderTop_notIncludingOutsidePlotSections;
        /** \brief <b>calculated value:</b> free space between widget left border and (plot+drawOutside) left border (including coordinate axes)
         * \internal
         *
         * \image html plot_widget_orientation.png
         *
         * \note This property is an intermediate storage for calculated values. Do not change directly!
          */
        double internalPlotBorderLeft_notIncludingOutsidePlotSections;
        /** \brief <b>calculated value:</b> free space between widget right border and (plot+drawOutside) right border (including coordinate axes)
         * \internal
         *
         * \image html plot_widget_orientation.png
         *
         * \note This property is an intermediate storage for calculated values. Do not change directly!
          */
        double internalPlotBorderBottom_notIncludingOutsidePlotSections;
        /** \brief <b>calculated value:</b> free space between widget bottom border and (plot+drawOutside) bottom border (including coordinate axes)
         * \internal
         *
         * \image html plot_widget_orientation.png
         *
         * \note This property is an intermediate storage for calculated values. Do not change directly!
          */
        double internalPlotBorderRight_notIncludingOutsidePlotSections;

        /** \brief <b>calculated value:</b> plot width in pixel inside the widget (calculated by calcPlotScaling() from plotBorderLeft, plotBorderRight and widgetWidth)
         *
         * \image html plot_widget_orientation.png
         *
         * \see \ref jkqtplotter_base_plotsize
         */
        int internalPlotWidth;
        /** \brief <b>calculated value:</b> plot height in pixel inside the widget (calculated by calcPlotScaling() from plotBorderTop, plotBorderBottom and widgetHeight)
         *
         * \image html plot_widget_orientation.png
         *
         * \see \ref jkqtplotter_base_plotsize
         */
        int internalPlotHeight;

        /** \brief indicates whether the widget should maintain an aspect ratio of plotwidth and plotheight
         *
         *  \see aspectRatio
         */
        bool maintainAspectRatio;
        /** \brief the aspect ratio of plotwidth and plotheight to maintain, if \c maintainAspectRatio==true
         *
         *  \f[ \mbox{aspectRatio}=\frac{\mbox{plotWidth}}{\mbox{plotHeight}} \f]
         *
         *  \see maintainAspectRatio
         */
        double aspectRatio;

        /** \brief indicates whether the axes should maintain an aspect ratio
         *
         *  When the axis aspect ratio is to be maintained and new axis ranges are set (e.g. when calling setXY() ),
         *  the given axis ranges are modified, so
         *    \f[ \mbox{axisAspectRatio}=\frac{\left|x_\text{max}-x_\text{min}\right|}{\left|y_\text{max}-y_\text{min}\right|} \f]
         *
         *  \note An axis aspect ratio is only well defined for linear axes (if both axes are linear).
         *        If both axes a logarithmic, the axis ration is defined for log(axismax)-log(axismin).
         *        For other combinations of axes, this function is deactivated
         *
         *  \see axisAspectRatio
         */
        bool maintainAxisAspectRatio;
        /** \brief the aspect ratio of axis widths to maintain, if \c maintainAxisAspectRatio==true
         *
         *  When the axis aspect ratio is to be maintained and new axis ranges are set (e.g. when calling setXY() ),
         *  the given axis ranges are modified, so
         *    \f[ \mbox{axisAspectRatio}=\frac{\left|x_\text{max}-x_\text{min}\right|}{\left|y_\text{max}-y_\text{min}\right|} \f]
         *
         *  \note An axis aspect ratio is only well defined for linear axes (if both axes are linear).
         *        If both axes a logarithmic, the axis ration is defined for log(axismax)-log(axismin).
         *        For other combinations of axes, this function is deactivated
         *
         *  \see maintainAxisAspectRatio
         */
        double axisAspectRatio;


        /** \brief the plot label text */
        QString plotLabel;


        /** \brief calculate the scaling and offset values from axis min/max values */
        void calcPlotScaling(JKQTPEnhancedPainter& painter);

        /** \brief set the standard settings (colors, line widths ...) */
        void initSettings();


        /** \brief specifies whether this class emits signals, like zoomChangedLocally() or beforePlotScalingRecaluclate() */
        bool emitSignals;

        /** \brief multiplier which is used for font sizes when the plot is exported/printed */
        double fontSizePrintMultiplier;

        /** \brief multiplier which is used for linewidths when the plot is exported/printed */
        double lineWidthPrintMultiplier;

        /** \brief multiplier for the font size */
        double fontSizeMultiplier;
        /** \brief multiplier or the line widths */
        double lineWidthMultiplier;

        /** \brief internal: used to store a list of all currently used plot styles */
        QList<int> usedStyles;


        /** \brief a vector that contains all graphs to be plottet in the system */
        QList<JKQTPPlotElement*> graphs;




        /** \brief indicates whether to use clipping (hack for printing, see print() ) */
        bool useClipping;




        /** \brief QAction which triggers saving of the plot as an image */
        QAction* actSavePlot;
        /** \brief QAction which triggers saving of the data used for the plot */
        QAction* actSaveData;
        /** \brief QAction which triggers copying of the data to the clipboard */
        QAction* actCopyData;
        /** \brief QAction which triggers copying of the image to the clipboard */
        QAction* actCopyPixelImage;
        /** \brief QAction which triggers copying of the data to the clipboard in Matlab format */
        QAction* actCopyMatlab;
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        /** \brief QAction which triggers the saving as PDF */
        QAction* actSavePDF;
        /** \brief QAction which triggers the saving as Scalable Vector Graphics (SVG) */
        QAction* actSaveSVG;
        /** \brief QAction which triggers the printing */
        QAction* actPrint;
#endif
        /** \brief QAction which triggers the saving as pixel image */
        QAction* actSavePix;
        /** \brief QAction which triggers the saving as CSV (data only) */
        QAction* actSaveCSV;
        /** \brief QAction which triggers zoom all */
        QAction* actZoomAll;
        /** \brief QAction which triggers zoom in */
        QAction* actZoomIn;
        /** \brief QAction which triggers zoom out */
        QAction* actZoomOut;
        /** \brief QAction to show a table with all the plot data in the datastore */
        QAction* actShowPlotData;
        /** \brief this list contains additional actions, that can be registered by registerAdditionalAction(). They are shown in the context menu only and are e.g. used to display "save image data" actions. Each action may be in a category (key of map), which is displayed as a submenu of the context-menu! */
        AdditionalActionsMap lstAdditionalPlotterActions;
        /** \brief the current directory in which to open SaveAs ... dialogs */
        QString currentSaveDirectory;
        /** \brief the current file format to use in SaveAs ... dialogs */
        QString currentFileFormat;
        /** \brief the current file format to use in SaveDataAs ... dialogs */
        QString currentDataFileFormat;
        /** \brief the currently selected printer */
        QString currentPrinter;

        /** \brief the master plotter for x-dimension, this plotter is connected to. */
        JKQTBasePlotter *masterPlotterX;
        /** \brief the master plotter for y-dimension, this plotter is connected to. */
        JKQTBasePlotter *masterPlotterY;
        /** \brief synchronize plot width with masterPlotterX */
        bool masterSynchronizeWidth;
        /** \brief synchronize plot height with masterPlotterY */
        bool masterSynchronizeHeight;


        /** \brief controls, whether the signals plotUpdated() are emitted */
        bool emitPlotSignals;





        QListWidget* dataColumnsListWidget;
        QComboBox* dataColumnsCombobox;
        QSet<int> getDataColumnsByUser();
        QMap<QString, QStringList> getDataColumnsByUserSaved;


    private:
        double printZoomFactor;
        double printSizeX_Millimeter;
        double printSizeY_Millimeter;
        double printMagnification;
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        QPointer<QPrintPreviewWidget> printPreview;
#endif
        QPointer<JKQTPEnhancedDoubleSpinBox> spinSizeX;
        QPointer<JKQTPEnhancedDoubleSpinBox> spinSizeY;
        QPointer<QLabel> exportPreviewLabel;
        QPointer<JKQTPEnhancedDoubleSpinBox> spinMagnification;
        bool printSetAbsolutePageSize;
        bool printSetAbsolutePlotSize;
        bool printKeepAbsoluteFontSizes;
        bool printScaleToPagesize;
        double printAspect;
        bool printKeepAspect;
        bool exportUnitInMM;
        QSizeF printPageSizeMM;
        bool printDoUpdate;





};

/** \brief qHash()-specialization
 *   \ingroup jkqtpplottersupprt
 */
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
inline size_t qHash(const JKQTBasePlotter::textSizeKey& data, size_t /*seed=0*/) {
#else
inline uint qHash(const JKQTBasePlotter::textSizeKey& data, uint /*seed=0*/) {
#endif
    return qHash(data.f.family())+qHash(data.text);
}

#endif // JKQTPBASEPLOTTER_H
