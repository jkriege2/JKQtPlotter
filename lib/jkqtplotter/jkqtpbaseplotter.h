/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



/** \file jkqtpbaseplotter.h
  * \ingroup jkqtpplotterclasses
  */

#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpdatastorage.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtpelementsoverlay.h"
#include "jkqtplottertools/jkqtpenhancedpainter.h"
#include "jkqtplottergui/jkqtpenhancedspinboxes.h"

#include <QObject>
#include <QAction>
#include <QActionGroup>
#include <QVector>
#include <QSettings>
#include <QColor>
#include <QMap>
#include <QVector>
#include <QPair>
#include <QPrintPreviewWidget>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPointer>
#include <QListWidget>
#include <QComboBox>

#include <vector>
#include <cmath>
#include <iostream>
 #include "jkqtplottertools/jkqtp_imexport.h"


#ifndef JKQTPBASEPLOTTER_H
#define JKQTPBASEPLOTTER_H


class JKQTPGraphsModel; // forward
class JKQTPGraph; // forward
class JKQTPPlotElement; // forward

/** \brief initialized Qt-ressources necessary for JKQTBasePlotter
 * \ingroup jkqtpplotterclasses */
LIB_EXPORT void initJKQTBasePlotterResources();

/** \brief virtual base-class for exporter classes that can be used to save data inot a file
 * \ingroup jkqtpplotterclasses */
class LIB_EXPORT JKQTPSaveDataAdapter {
    public:
        virtual ~JKQTPSaveDataAdapter() ;
        virtual QString getFilter() const=0;
        virtual void saveJKQTPData(const QString& filename, const QList<QVector<double> >& data, const QStringList& columnNames) const=0;
};

/** \brief Service from this class to implement a special QPaintDevice as a plugin, that can be registered to JKQTBasePlotter/JKQTPlotter
 *         and then be used to export graphics, use registerPaintDeviceAdapter() to register such a plass
 * \ingroup jkqtpplotterclasses*/
class LIB_EXPORT JKQTPPaintDeviceAdapter {
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
        virtual bool useLatexParser() const;
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
 *   -# plot a set of overlay elements (may be used for fast plotting of indicators onto a complex plot)
 *   -# drawing the coordinate axes, grids ... (logarithmic and linear)
 *   -# saveing and printing the resulting plots
 * .
 *
 * This class is NOT a widget, if you need a plotting widget, use JKQTPlotter. This class may be used to
 * plot using a JKQTPEnhancedPainter.
 *
 * \see JKQTPlotter
 *
 * \section jkqtplotter_base_datastore Data Storage
 * As already mentioned this class does not provide means to draw graphs, but it contains a basic mechanism to associate
 * data with it. This is imeplemented, using a JKQTPDatastore object together with some convenience access methods. No further
 * data access is implemented and child classes are NOT forced to use this datastore. Hide it if you want. JKQTPlotter shows
 * how to use it. This class implement a graph management, where graphs simply point to a set of columns inside the datastore
 * that contain the actual plot data!
 *
 * If you call the \link JKQTBasePlotter::JKQTBasePlotter() constructor \endlink with no arguments, it will create an internal
 * datastore object and you can start adding data by using getDatastore(). If you have an external JKQTPDatastore object you can
 * give it as parameter to the constructor or use one of the other methods:
 *  - useExternalDatastore(): \copybrief JKQTBasePlotter::useExternalDatastore()
 *  - useAsInternalDatastore(): \copybrief JKQTBasePlotter::useAsInternalDatastore()
 *  - useInternalDatastore(): \copybrief JKQTBasePlotter::useInternalDatastore()
 *  - forceInternalDatastore(): \copybrief JKQTBasePlotter::forceInternalDatastore()
 *.
 *
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
 * \subsection jkqtplotter_base_key Graph Keys
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
 * There is also a possibility to determine the size of the key automatically, so all text fits in. This is activted by the property keyAutosize ( \copybrief keyAutosize ). If this
 * is \c true the function getKeyExtent() has to check the width of every key item and take it into account when calculating the width and height of the
 * key content. By default this feature is switched ON.
 *
 *
 * \subsection jkqtplotter_base_aspectratios Aspect Ratios
 * First note that this functionality is only available and activated if both axes are linear!
 *
 * You can set two different aspect ratios:
 *   - The ratio of plotWidth/plotHeight (setAspectRatio(), setMaintainAspectRatio()) will keep the plots pixel-width and height at a certain value.
 *   - The ratio of (xmax-xmin)/(ymax-ymin) (setAxisAspectRatio(), setMaintainAxisAspectRatio()) will keep the displayed axis ranges in a certain ratio.
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
 * \section jkqtbaseplotter_dataexport_print Printing, Saving & Exporting
 *
 * \subsection jkqtplotter_base_saveprint Saving and Printing Graphs
 * This class implement a set of function to save and print the graphs:
 *   - print() prints the graph on a QPrinter object
 *   - saveAsPixelImage() saves the plot into a pixel image file (PNG, TIFF, ... formats, as supported by Qt)
 *   - saveAsPDF() saves the graph as a PDF file (using the Qt printing engine)
 *   - saveAsPS() saves the graph as a PDF file (using the Qt printing engine)
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
 * \subsection JKQTBASEPLOTTER_GRIDPRINTING Grid-Printing / Layouting Several Graphs
 * As one often want's to combine different graphs, there is a possibility to combine this graph with more other graphs.
 * To do so one can think of the graphs to be part of a grid where each graph is contained in one cell. By default this
 * mechanism is deactivated. You can activate it by calling setGridPrinting(true). Then you can set the position of the
 * current graph by calling setGridPrintingCurrentX() and setGridPrintingCurrentY(). Add additional graphs by calling
 * addGridPrintingPlotter(). The position of the current graph is 0,0 by default. Afterwards the save and print routines
 * will export/print all graphs, not just the current one. There will be no additional border between the graphs, as the
 * class expects the internal graph borders to be sufficient.
 *
 * \see \ref JKQTPLOTTER_SYNCMULTIPLOT
 *
 *
 * \section jkqtplotter_base_defaultvalues Default Properties
 * The plot is configured by a huge set of properties. For each property there is also a second protected variable which
 * contains its default value. This way it is possible to store only those parameters in an INI file which have changed with
 * respect to the default values. If the property is called \c property then the according default value is stored in
 * \c default_property. To reduce the code to be entered you can use the JKQTPPROPERTY() macro.
 *
 * Default values are available only for properties that control the appearance of the graphs (line widths, border widths,
 * color, fonts ...), not the type of the graph (xmin, xmax, logXAxis, ...)
 *
 * \section jkqtplotter_base_userprops User Properties
 * There is a subset of options that describe how the user interacted with the plotter (export/print scaling factors etc, save directories,
 * other export settings, ...). These are not stored/loaded using saveSettings() and loadSettings(), but using saveUserSettings() and loadUserSettings().
 * These methods MAY (strictly optional and turned off by default) be called by saveSettings() and loadSettings(), if the property userSettigsFilename ( \copybrief userSettigsFilename )is
 * set (not-empty). In this case the suer settings are stored/loaded also everytime they are changed by the user or programmatically.
 */
class LIB_EXPORT JKQTBasePlotter: public QObject {
        Q_OBJECT
    public:
        typedef QMap<QString, QList<QPointer<QAction> > > AdditionalActionsMap;
        typedef QMapIterator<QString, QList<QPointer<QAction> > > AdditionalActionsMapIterator;
    protected:

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


        /** \brief indicates whether the \ref jkqtplotter_base_saveprint  is activated .*/
        bool gridPrinting;
        /** \brief x position of the current graph in \ref jkqtplotter_base_saveprint  */
        size_t gridPrintingCurrentX;
        /** \brief y position of the current graph in \ref jkqtplotter_base_saveprint  */
        size_t gridPrintingCurrentY;
        /** \brief list that manages all the additional graphs for grid printing mode */
        QList<JKQTPGridPrintingItem> gridPrintingList;
        /** \brief this list contains all the rows of the current printing grid and stores its heights */
        QList<int> gridPrintingRows;
        /** \brief this list contains all the columns of the current printing grid and stores its widths */
        QList<int> gridPrintingColumns;
        /** \brief size of all plots in grid printing mode, filled by gridPrintingCalc() */
        QSizeF gridPrintingSize;

        /** \brief this is an internal property that is used by the export/print system to control the "magnification". Usually this is 1.0
         *         but if set !=1.0, it is used to scale the widgetWidth and widgetHeight before painting! */
        double paintMagnification;

        /** \brief the decimal separator used when exporting data to text files */
        QString CSVdecimalSeparator;
        /*! \brief default value for property property CSVdecimalSeparator. \see CSVdecimalSeparator for more information */
        QString default_CSVdecimalSeparator;
        /** \brief this string is used to introduce comments in text output when exporting data */
        QString CSVcommentInitializer;
        /*! \brief default value for property property CSVcommentInitializer. \see CSVcommentInitializer for more information */
        QString default_CSVcommentInitializer;

        /** \brief an object which manages all data columns for this plotter class */
        JKQTPDatastore* datastore;
        /** \brief indicates whether the datastore is managed (allocated/freed) internally or externally */
        bool datastoreInternal;


        /** \brief width of the plot widget */
        int widgetWidth;

        /** \brief height of the plot widget */
        int widgetHeight;


        /** \brief free space between widget top border and plot top border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to iplotBorderTop */
        int plotBorderTop;
        /*! \brief default value for property property plotBorderTop. \see plotBorderTop for more information */
        int default_plotBorderTop;
        /** \brief free space between widget top border and plot top border, as used to plot the graph (mnay be altered from user input ) */
        double iplotBorderTop;
        double iplotKeyBorderTop;
        /** \brief height of the plot title (or 0 if no title) */
        double iTitleHeight;
        /** \brief free space between widget left border and plot left border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to iplotBorderLeft  */
        int plotBorderLeft;
        /*! \brief default value for property property plotBorderLeft. \see plotBorderLeft for more information */
        int default_plotBorderLeft;
        /** \brief free space between widget top border and plot top border, as used to plot the graph (mnay be altered from user input ) */
        double iplotBorderLeft;
        double iplotKeyBorderLeft;
        /** \brief free space between widget bottom border and plot bottom border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to iplotBorderBottom  */
        int plotBorderBottom;
        /*! \brief default value for property property plotBorderBottom. \see plotBorderBottom for more information */
        int default_plotBorderBottom;
        /** \brief free space between widget top border and plot top border, as used to plot the graph (mnay be altered from user input ) */
        double iplotBorderBottom;
        double iplotKeyBorderBottom;
        /** \brief free space between widget right border and plot right border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to iplotBorderRight  */
        int plotBorderRight;
        /*! \brief default value for property property plotBorderRight. \see plotBorderRight for more information */
        int default_plotBorderRight;
        /** \brief free space between widget top border and plot top border, as used to plot the graph (mnay be altered from user input ) */
        double iplotBorderRight;
        double iplotKeyBorderRight;



        /** \brief free space between widget top border and plot top border (including coordinate axes) */
        double iplotBorderTop_nographs;
        /** \brief free space between widget left border and plot left border (including coordinate axes) */
        double iplotBorderLeft_nographs;
        /** \brief free space between widget right border and plot right border (including coordinate axes) */
        double iplotBorderBottom_nographs;
        /** \brief free space between widget bottom border and plot bottom border (including coordinate axes) */
        double iplotBorderRight_nographs;

        /** \brief indicates whether the widget should maintain an aspect ratio of plotwidth and plotheight */
        bool maintainAspectRatio;
        /*! \brief default value for property property maintainAspectRatio. \see maintainAspectRatio for more information */
        bool default_maintainAspectRatio;
        /** \brief the aspect ratio of plotwidth and plotheight to maintain, if \c maintainAspectRatio==true */
        double aspectRatio;
        /*! \brief default value for property property aspectRatio. \see aspectRatio for more information */
        double default_aspectRatio;

        /** \brief indicates whether the axes should maintain an aspect ratio */
        bool maintainAxisAspectRatio;
        /*! \brief default value for property property maintainAxisAspectRatio. \see maintainAxisAspectRatio for more information */
        bool default_maintainAxisAspectRatio;
        /** \brief the aspect ratio of axis widths to maintain, if \c maintainAxisAspectRatio==true */
        double axisAspectRatio;
        /*! \brief default value for property property axisAspectRatio. \see axisAspectRatio for more information */
        double default_axisAspectRatio;

        /** \brief plot width in pt inside the widget (calculated by calcPlotScaling() from plotBorderLeft, plotBorderRight and widgetWidth) */
        int plotWidth;
        /** \brief plot height in pt inside the widget (calculated by calcPlotScaling() from plotBorderTop, plotBorderBottom and widgetHeight) */
        int plotHeight;



        /** \brief color of the plotted graph */
        QColor graphColor;
        /*! \brief default value for property property graphColor. \see graphColor for more information */
        QColor default_graphColor;
        /** \brief colors used in modes where more than one graph is beeing plottet */
        QColor manyGraphsColor[50];
        QColor default_manyGraphsColor[50];
        /** \brief number of actually defined items in manyGraphsColor */
        int manyGraphsColorCount;
        /** \brief pen styles used in modes where more than one graph is beeing plottet */
        Qt::PenStyle manyGraphsStyle[5];
        /*! \brief default value for property property manyGraphsStyle. \see manyGraphsStyle[5] for more information */
        Qt::PenStyle default_manyGraphsStyle[5];
        /** \brief width of the plotted graph (in pixel) */
        double graphWidth;
        /*! \brief default value for property property graphWidth. \see graphWidth for more information */
        double default_graphWidth;



        /** \brief indicates whether to use clipping (hack for printing, see print() ) */
        bool useClipping;



        /** \brief color of the coordinate system */
        QColor systemColor;
        /*! \brief default value for property property systemColor. \see systemColor for more information */
        QColor default_systemColor;
        /** \brief width of the coordinate (in pixel) */
        double systemWidth;
        /*! \brief default value for property property systemWidth. \see systemWidth for more information */
        double default_systemWidth;
        /** \brief color of the background*/
        QColor backgroundColor;
        /*! \brief default value for property property backgroundColor. \see backgroundColor for more information */
        QColor default_backgroundColor;
        /** \brief color of the background when exporting*/
        QColor exportBackgroundColor;
        /*! \brief default value for property property exportBackgroundColor. \see exportBackgroundColor for more information */
        QColor default_exportBackgroundColor;
        /** \brief color of the plot's background*/
        QColor plotBackgroundColor;
        /*! \brief default value for property property plotBackgroundColor. \see plotBackgroundColor for more information */
        QColor default_plotBackgroundColor;


        /** \brief indicates whether to plot a frame around the key */
        bool showKeyFrame;
        /*! \brief default value for property property showKeyFrame. \see showKeyFrame for more information */
        bool default_showKeyFrame;
        /** \brief color of the key frame line */
        QColor keyFrameColor;
        /*! \brief default value for property property keyFrameColor. \see keyFrameColor for more information */
        QColor default_keyFrameColor;
        /** \brief width of the key frame line */
        double keyFrameWidth;
        /*! \brief default value for property property keyFrameWidth. \see keyFrameWidth for more information */
        double default_keyFrameWidth;
        /** \brief color of the key background */
        QColor keyBackgroundColor;
        /*! \brief default value for property property keyBackgroundColor. \see keyBackgroundColor for more information */
        QColor default_keyBackgroundColor;
        /** \brief indicates whether to plot a key */
        bool showKey;
        /*! \brief default value for property property showKey. \see showKey for more information */
        bool default_showKey;
        /** \brief font face for key labels */
        QString keyFont;
        /*! \brief default value for property property keyFont. \see keyFont for more information */
        QString default_keyFont;
        /** \brief font size for key labels [in points] */
        double keyFontSize;
        /*! \brief default value for property property keyFontSize. \see keyFontSize for more information */
        double default_keyFontSize;
        /** \brief width of a key item in pt [in units of width of 'X' set in keyFont, keyFontSize] */
        double key_item_width;
        /*! \brief default value for property property key_item_width. \see key_item_width for more information */
        double default_key_item_width;
        /** \brief height of a key item in pt [in units of height  keyFont, keyFontSize] */
        double key_item_height;
        /*! \brief default value for property property key_item_height. \see key_item_height for more information */
        double default_key_item_height;
        /** \brief length of the line samples in the key in pt [in units of width of 'X' set in keyFont, keyFontSize] */
        double key_line_length;
        /*! \brief default value for property property key_line_length. \see key_line_length for more information */
        double default_key_line_length;
        /** \brief x-distance between key frame and key content [in units of width of 'X' set in keyFont, keyFontSize] */
        double keyXMargin;
        /*! \brief default value for property property keyXMargin. \see keyXMargin for more information */
        double default_keyXMargin;
        /** \brief y-distance between key frame and key content [in units of width of 'x' set in keyFont, keyFontSize] */
        double keyYMargin;
        /*! \brief default value for property property keyYMargin. \see keyYMargin for more information */
        double default_keyYMargin;
        /** \brief x-offset of the key from the border of the plot [in units of width of 'X' set in keyFont, keyFontSize] */
        double keyXOffset;
        /*! \brief default value for property property keyXOffset. \see keyXOffset for more information */
        double default_keyXOffset;
        /** \brief y-offset of the key from the border of the plot [in units of width of 'x' set in keyFont, keyFontSize] */
        double keyYOffset;
        /*! \brief default value for property property keyYOffset. \see keyYOffset for more information */
        double default_keyYOffset;
        /** \brief distance between key line example and key text [in units of width of 'X' set in keyFont, keyFontSize] */
        double keyXSeparation;
        /*! \brief default value for property property keyXSeparation. \see keyXSeparation for more information */
        double default_keyXSeparation;
        /** \brief distance between two key entries [in units of height of keyFont, keyFontSize] */
        double keyYSeparation;
        /*! \brief default value for property property keyYSeparation. \see keyYSeparation for more information */
        double default_keyYSeparation;
        /** \brief key position */
        JKQTPKeyPosition keyPosition;
        /*! \brief default value for property property keyPosition. \see keyPosition for more information */
        JKQTPKeyPosition default_keyPosition;
        /** \brief the key layout */
        JKQTPKeyLayout keyLayout;
        /*! \brief default value for property property keyLayout. \see keyLayout for more information */
        JKQTPKeyLayout default_keyLayout;
        /** \brief determine width of the key automatically */
        bool keyAutosize;
        /*! \brief default value for property property keyAutosize. \see keyAutosize for more information */
        bool default_keyAutosize;

        /** \brief the plot label text */
        QString plotLabel;

        /** \brief the plot label font name */
        QString plotLabelFontname;
        /*! \brief default value for property property plotLabelFontname. \see plotLabelFontname for more information */
        QString default_plotLabelFontname;

        /** \brief the plot label font size */
        double plotLabelFontSize;
        /*! \brief default value for property property plotLabelFontSize. \see plotLabelFontSize for more information */
        double default_plotLabelFontSize;

        /** \brief calculate the scaling and offset values from axis min/max values */
        void calcPlotScaling(JKQTPEnhancedPainter& painter);

        /** \brief set the standard settings (colors, line widths ...) */
        void initSettings();


        /** \brief specifies whether this class emits signals, like zoomChangedLocally() or beforePlotScalingRecaluclate() */
        bool emitSignals;

        /** \brief specifies whether to use antialiasing for plotting the coordinate system */
        bool useAntiAliasingForSystem;
        /*! \brief default value for property property useAntiAliasingForSystem. \see useAntiAliasingForSystem for more information */
        bool default_useAntiAliasingForSystem;

        /** \brief specifies whether to use antialiasing for plotting the graphs
         *
         * \note You can set this property \c false to increase plotting speed of complex plots (with many graphs inside). You can reach a
         *       roughly three-fold speed improvement!
*/
        bool useAntiAliasingForGraphs;
        /*! \brief default value for property property useAntiAliasingForGraphs. \see useAntiAliasingForGraphs for more information */
        bool default_useAntiAliasingForGraphs;

        /** \brief specifies whether to use antialiasing when drawing any text
        * \note You can set this property \c false to increase plotting speed of complex plots (with many graphs inside). You can reach a
        *       roughly three-fold speed improvement!
*/
        bool useAntiAliasingForText;
        /*! \brief default value for property property useAntiAliasingForText. \see useAntiAliasingForText for more information */
        bool default_useAntiAliasingForText;

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


        QList<JKQTPOverlayElement*> overlays;


        /** \brief used as maximum/minimum pixel value for plotting */
        double plot_minmaxcoorinate;




        /** \brief hidden default constructor */
        JKQTBasePlotter();



        /** \brief paints the plot onto the given JKQTPEnhancedPainter object */
        void paintPlot(JKQTPEnhancedPainter& painter, bool drawOverlays=true);
        void paintOverlays(JKQTPEnhancedPainter& painter);
        /** \brief simply calls paintPlot() if grid printing mode is deactivated and prints the graph grid otherwise
         *         \a pageRect is used to determine the size of the page to draw on. If this does not coincide with
         *         the widget extents this function calculates a scaling factor so the graphs fit onto the page. This
         *         is especially usefull when printing!
         */
        void gridPaint(JKQTPEnhancedPainter& painter, QSizeF pageRect, bool drawOverlays=true, bool scaleIfTooLarge=true, bool scaleIfTooSmall=true);
        void gridPaintOverlays(JKQTPEnhancedPainter& painter, QSizeF pageRect);

        /** \brief This method goes through all registered plotters and calculates the width of every column and
         *      height of every row as the max over the row/column. The reults are stored in the private datamembers
         *      gridPrintingRows and gridPrintingColumns. gridPrintingSize will contain the size of all graphs together
         *      afterwards. */
        void gridPrintingCalc();

        /** \brief plot the grid */
        void plotSystemGrid(JKQTPEnhancedPainter& painter);
        /** \brief plot the x coordinate axis (incl. labels) */
        void plotSystemXAxis(JKQTPEnhancedPainter& painter);
        /** \brief plot the x coordinate axis (incl. labels) */
        void plotSystemYAxis(JKQTPEnhancedPainter& painter);
        /** \brief plot the graphs */
        void plotGraphs(JKQTPEnhancedPainter& painter);
        /** \brief plot a key */
        void plotKey(JKQTPEnhancedPainter& painter);
        /** \brief plot all overlay elements */
        void plotOverlays(JKQTPEnhancedPainter& painter);

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
        void plotKeyContents(JKQTPEnhancedPainter& painter, double x, double y, double width, double height);

        /** \brief returns the size of the key (as call-by-reference parameters
         *
         * Any implementation of key plotting has to overwrite this function and use it to return the size of the key that would be
         * plotted by plotKeyContents(). This class will use the return values to plot the frame of the key and also supply them to
         * plotKeyContents(). If height or width are returned 0, no key is plotted
         *
         * The implementation in here returns zero size!
         */
        void getKeyExtent(JKQTPEnhancedPainter& painter, double *width, double *height, double *text_width=nullptr, double *text_height=nullptr, int *columns_count=nullptr, int* lines_count=nullptr);
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
        /** \brief QAction which triggers the saving as PDF */
        QAction* actSavePDF;
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        /** \brief QAction which triggers the saving as PostScript */
        QAction* actSavePS;
#endif
        /** \brief QAction which triggers the saving as pixel image */
        QAction* actSavePix;
        /** \brief QAction which triggers the saving as Scalable Vector Graphics (SVG) */
        QAction* actSaveSVG;
        /** \brief QAction which triggers the printing */
        QAction* actPrint;
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
        /** \brief the directory in which to open SaveAs ... dialogs */
        QString currentSaveDirectory;
        QString currentFileFormat;
        QString currentDataFileFormat;
        QString currentPrinter;

        /** \brief the master plotter, this plotter is connected to. */
        JKQTBasePlotter* masterPlotter;
        /** \brief synchronize plot width with masterPlotter */
        bool masterSynchronizeWidth;
        /** \brief synchronize plot height with masterPlotter */
        bool masterSynchronizeHeight;


        /** \brief controls, whether the signals plotUpdated() and overlaysUpdated() are emitted */
        bool emitPlotSignals;


        void printpreview(QPrinter* p, bool setabsolutesize=false);
        bool printpreviewNew(QPaintDevice* paintDevice, bool setAbsolutePaperSize=false, double printsizeX_inMM=-1.0, double printsizeY_inMM=-1.0, bool displayPreview=true);
        bool exportpreview(QSizeF pageSize, bool unitIsMM=false);

        /*! \brief sets the property fontSizeMultiplier ( \copybrief fontSizeMultiplier ) to the specified \a __value. 
            \details Description of the parameter fontSizeMultiplier is:  <BLOCKQUOTE>\copydoc fontSizeMultiplier </BLOCKQUOTE> 
        	\see fontSizeMultiplier for more information */ 
        inline void setFontSizeMultiplier(double __value)
        { 
        	this->fontSizeMultiplier = __value; 
        }
        /*! \brief sets the property lineWidthMultiplier ( \copybrief lineWidthMultiplier ) to the specified \a __value. 
            \details Description of the parameter lineWidthMultiplier is:  <BLOCKQUOTE>\copydoc lineWidthMultiplier </BLOCKQUOTE> 
        	\see lineWidthMultiplier for more information */ 
        inline void setLineWidthMultiplier(double __value)
        { 
        	this->lineWidthMultiplier = __value; 
        }
        /*! \brief sets the property printMagnification ( \copybrief printMagnification ) to the specified \a __value. 
            \details Description of the parameter printMagnification is: <BLOCKQUOTE>\copydoc printMagnification </BLOCKQUOTE> 
            \see printMagnification for more information */ 
        inline void setPrintMagnification(double __value)
        {
            this->printMagnification = __value;
        } 
        /*! \brief returns the property printMagnification ( \copybrief printMagnification ). 
            \details Description of the parameter printMagnification is: <BLOCKQUOTE>\copydoc printMagnification </BLOCKQUOTE> 
            \see printMagnification for more information */ 
        inline double getPrintMagnification() const
        {
            return this->printMagnification; 
        }
        /*! \brief sets the property paintMagnification ( \copybrief paintMagnification ) to the specified \a __value. 
            \details Description of the parameter paintMagnification is: <BLOCKQUOTE>\copydoc paintMagnification </BLOCKQUOTE> 
            \see paintMagnification for more information */ 
        inline void setPaintMagnification(double __value)
        {
            this->paintMagnification = __value;
        } 
        /*! \brief returns the property paintMagnification ( \copybrief paintMagnification ). 
            \details Description of the parameter paintMagnification is: <BLOCKQUOTE>\copydoc paintMagnification </BLOCKQUOTE> 
            \see paintMagnification for more information */ 
        inline double getPaintMagnification() const
        {
            return this->paintMagnification; 
        }

        QListWidget* dataColumnsListWidget;
        QComboBox* dataColumnsCombobox;
        QSet<int> getDataColumnsByUser();
        QMap<QString, QStringList> getDataColumnsByUserSaved;


    private:
        double printZoomFactor;
        double printSizeX_Millimeter;
        double printSizeY_Millimeter;
        double printMagnification;
        QPointer<QPrintPreviewWidget> printPreview;
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






    protected slots:
        void updatePreviewLabel();
        void printpreviewPaintRequested(QPrinter *printer);
        void printpreviewPaintRequestedNew(QPrinter *printer);
        void printpreviewPaintRequestedNew(QPaintDevice *paintDevice);
        void exportpreviewPaintRequested(JKQTPEnhancedPainter& painter, QSize size);
        void printpreviewSetZoom(double value);
        void printpreviewSetSizeX(double value);
        void printpreviewSetSizeY(double value);
        void printpreviewSetSizeXNew(double value);
        void printpreviewSetSizeYNew(double value);
        void printpreviewSetMagnification(double value);
        void printpreviewSetMagnificationNew(double value);
        void printpreviewSetAspectRatio(bool checked);
        void printpreviewSetKeepAbsFontsize(bool checked);
        void printpreviewToggleMagnification(bool checked);
        void printpreviewSetLineWidthMultiplier(double value);
        void printpreviewSetFontSizeMultiplier(double value);
        void printpreviewUpdate();
        void getDataColumnsByUserCheckAll();
        void getDataColumnsByUserCheckNone();
        void getDataColumnsByUserSave();
        void getDataColumnsByUserComboBoxSelected(const QString& name);
        void getDataColumnsByUserItemChanged(QListWidgetItem* widgetitem);
        void showPlotData();
        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local x-axis to the other x-axis */
        void synchronizeXAxis(double newxmin, double newxmax, double newymin, double newymax, JKQTBasePlotter* sender);
        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local y-axis to the other y-axis */
        void synchronizeYAxis(double newxmin, double newxmax, double newymin, double newymax, JKQTBasePlotter* sender);
        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local x- and y-axis to the other x- and y-axis */
        void synchronizeXYAxis(double newxmin, double newxmax, double newymin, double newymax, JKQTBasePlotter* sender);

    public slots:




        /** \brief sets x/ymin and x/ymax to the supplied values and replots the graph (zoom operation!) */
        void zoom(double nxmin, double nxmax, double nymin, double nymax);

        /** \brief sets whether to plot grid lines or not */
        void setGrid(bool val);

        /** \brief save the current plot as a pixel image image (PNG ...), if filename is empty a file selection dialog is displayed */
        void saveAsPixelImage(const QString& filename=QString(""), bool displayPreview=true, const QByteArray &outputFormat=QByteArray());

        /** \brief copy the current plot as a pixel image to the clipboard */
        void copyPixelImage();

        /** \brief save the current plot as a SVG file, with the current widget aspect ratio, if filename is empty a file selection dialog is displayed  */
        void saveAsSVG(const QString& filename=QString(""), bool displayPreview=true);

        /** \brief save the current plot as a PDF file, with the current widget aspect ratio, if filename is empty a file selection dialog is displayed  */
        void saveAsPDF(const QString& filename=QString(""), bool displayPreview=true);

        /** \brief save the current plot as a PS file, with the current widget aspect ratio, if filename is empty a file selection dialog is displayed */
        void saveAsPS(const QString& filename=QString(""), bool displayPreview=true);

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
        /** \brief save the current plot data as a Comma Separated Values (CSV) file*/
        void saveAsCSV(const QString& filename=QString(""));

        /** \brief save the current plot data as a DIF file*/
        void saveAsDIF(const QString& filename=QString(""));

        /** \brief save the current plot data as a SYLK spreadsheet file*/
        void saveAsSYLK(const QString& filename=QString(""));

        /** \brief save the current plot data as a Matlab Script*/
        void saveAsMatlab(const QString& filename=QString(""));

        /** \brief save the current plot data as a Semicolon Separated Values (SSV) file*/
        void saveAsSemicolonSV(const QString& filename=QString(""));

        /** \brief save the current plot data as a Tabulator Separated Values (CSV) file*/
        void saveAsTabSV(const QString& filename=QString(""));

        /** \brief save the current plot data as a Semicolon Separated Values (CSV) file for german Excel, i.e. with comma as decimal separator*/
        void saveAsGerExcelCSV(const QString& filename=QString(""));

        /** \brief print the current plot, if printer is \c nullptr a printer selection dialog is displayed */
        void print(QPrinter* printer=nullptr, bool displayPreview=true);

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
        void zoomOut(double factor=2.0) {
            zoomIn(1.0/factor);
        }

        /** \brief set the datarange of all current graphs to the given values */
        void setGraphsDataRange(int datarange_start, int datarange_end);

        /** \brief en-/disables the maintaining of the data aspect ratio */
        void setMaintainAspectRatio(bool value) {
            maintainAspectRatio=value;
            redrawPlot();
        }

        /** \brief en-/disables the maintaining of the axis aspect ratio */
        void setMaintainAxisAspectRatio(bool value) {
            maintainAxisAspectRatio=value;
            redrawPlot();
        }

        void setUserSettigsFilename(const QString& filename, const QString& prefix);
        void setUserSettigsFilename(const QString& filename);
        void setUserSettigsPrefix(const QString& prefix);
        QString getUserSettigsFilename() const;
        QString getUserSettigsPrefix() const;

        /** \brief set all graphs invisible, except i */
        void setOnlyGraphVisible(int i);
        /** \brief set all graphs invisible, except graph start, start+n, start+2*n, ... */
        void setOnlyNthGraphsVisible(int start, int n);

        /** \brief saves the plot user properties if userSettingsFilename is set, does nothing otherwise */
        void loadUserSettings();

        /** \brief saves the plot user properties if userSettingsFilename is set, does nothing otherwise */
        void saveUserSettings() const;
    public:
        /** \brief class constructor
         *
         * if \a datastore_internal is \c true then the constructor will create an internal datastore object. The datastore
         * will be managed (freed) by this class. If \a datastore_internal is \c false the class will use tha datastore provided
         * in \a datast as an external datastore. You can modify this later by using useInternalDatastore() and useExternalDatastore().
         */
        JKQTBasePlotter(bool datastore_internal, QObject* parent=nullptr, JKQTPDatastore* datast=nullptr);

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


        /** \brief loads the plot properties from a QSettings object */
        void loadSettings(const QSettings& settings, const QString& group=QString("plots"));

        /** \brief saves the plot properties into a QSettings object.
         *
         * This method only saves those properties that differ from their default value.
         */
        void saveSettings(QSettings& settings, const QString& group=QString("plots")) const;



        /** \brief loads the plot user properties from a QSettings object */
        void loadUserSettings(const QSettings& settings, const QString& group=QString("plots_user"));

        /** \brief saves the plot user properties into a QSettings object. */
        void saveUserSettings(QSettings& settings, const QString& group=QString("plots_user")) const;

        /** \brief sets the width of the plot widget */
        void setWidth(int wid);

        /** \brief resize the plot */
        void resize(int width, int height);

        /** \brief gets the width of the plot widget */
        inline int getWidth() {return widgetWidth; }

        /** \brief gets the width of the plot widget */
        inline int getHeight() {return widgetHeight; }

        /** \brief sets the width of the plot widget */
        void setHeight(int heigh);

        /** \brief sets the borders of the plot, see also getPlotBorderTop(), plotBorderBottom(), plotBorderLeft(), plotBorderRight() */
        void setBorder(int left, int right, int top, int bottom);

        /** \brief sets minimum and maximum x-value to plot */
        void setX(double xminn, double xmaxx);

        /** \brief sets minimum and maximum y-value to plot */
        void setY(double yminn, double ymaxx);

        /** \brief sets minimum and maximum x- and y-values to plot */
        void setXY(double xminn, double xmaxx, double yminn, double ymaxx);

        /** \brief returns the current x-axis min */
        inline double getXMin() const {return xAxis->getMin(); }

        /** \brief returns the current x-axis max */
        inline double getXMax() const {return xAxis->getMax(); }

        /** \brief returns the current y-axis min */
        inline double getYMin() const {return yAxis->getMin(); }

        /** \brief returns the current y-axis max */
        inline double getYMax() const {return yAxis->getMax(); }



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

        /** \brief sets absolute minimum and maximum x-value to plot
         *  \note if the aspect ration of this does not fit into the widget, it is possible that you don't see the complete contents!
         */
        void setAbsoluteX(double xminn, double xmaxx);

        /** \brief sets absolute minimum and maximum y-value to plot
         *  \note if the aspect ration of this does not fit into the widget, it is possible that you don't see the complete contents!
         */
        void setAbsoluteY(double yminn, double ymaxx);

        /** \brief sets absolute minimum and maximum x- and y-values to plot
         *  \note if the aspect ration of this does not fit into the widget, it is possible that you don't see the complete contents!
         */
        void setAbsoluteXY(double xminn, double xmaxx, double yminn, double ymaxx);

        /** \brief returns the absolute x-axis min */
        inline double getAbsoluteXMin() const {return xAxis->getAbsoluteMin(); }

        /** \brief returns the absolute x-axis max */
        inline double getAbsoluteXMax() const {return xAxis->getAbsoluteMax(); }

        /** \brief returns the absolute y-axis min */
        inline double getAbsoluteYMin() const {return yAxis->getAbsoluteMin(); }

        /** \brief returns the absolute y-axis max */
        inline double getAbsoluteYMax() const {return yAxis->getAbsoluteMax(); }


        /** \brief add a new plotter for grid printing mode */
        inline void addGridPrintingPlotter(size_t x, size_t y, JKQTBasePlotter* plotter) {
            JKQTPGridPrintingItem i;
            i.x=x;
            i.y=y;
            i.plotter=plotter;
            gridPrintingList.push_back(i);
        }

        /** \brief clear all additional plotters for grid printing mode */
        inline void clearGridPrintingPlotters() {
            gridPrintingList.clear();
        }

        /** \brief return x-pixel coordinate from time coordinate */
        inline double x2p(double x) const {
            return xAxis->x2p(x);
        }

        /** \brief return y-pixel coordinate from y coordinate */
        inline double y2p(double y) const {
            return yAxis->x2p(y);
        }

        /** \brief return time coordinate coordinate from x-pixel */
        inline double p2x(double x) const {
            return xAxis->p2x(x);
        }

        /** \brief return y coordinate coordinate from y-pixel */
        inline double p2y(double y) const {
            return yAxis->p2x(y);
        }

        /** \brief gets the next unused style id, i.e. the smalles number >=0 which is not contained in usedStyles */
        int getNextStyle();


        /** \brief represents a pen, when plotting in JKQTPlotter/JKQTBasePlotter */
        struct JKQTPPen {
            QColor m_color;
            double m_width;
            Qt::PenStyle m_style;
            void setWidthF(double w) { m_width=w; }
            void setWidth(double w) { m_width=w; }
            double width() const { return m_width; }
            double widthF() const { return m_width; }
            QColor color() const { return m_color; }
            void setColor(const QColor& col) {m_color=col; }
            Qt::PenStyle style() const { return m_style; }
            void setStyle(Qt::PenStyle s) { m_style=s; }
        };


        /** \brief returns a QPen object for the i-th plot style */
        JKQTPPen getPlotStyle(int i) const;

        /*! \brief draw the contained graph (including grid prints) into the given JKQTPEnhancedPainter
            \param painter JKQTPEnhancedPainter to which the plot should be drawn
            \param rect rectangle to plot into
         */
        void draw(JKQTPEnhancedPainter& painter, const QRect& rect, bool drawOverlays=true);

        /*! \brief draw the contained graph (including grid prints) into the given JKQTPEnhancedPainter
            \param painter JKQTPEnhancedPainter to which the plot should be drawn
            \param rect rectangle to plot into
         */
        void drawOverlays(JKQTPEnhancedPainter& painter, const QRect& rect);

        /*! \brief draw the contained graph (including grid prints) into the given JKQTPEnhancedPainter
            \param painter JKQTPEnhancedPainter to which the plot should be drawn
            \param pos where to plot the painter (left-top corner)
         */
        void draw(JKQTPEnhancedPainter& painter, const QPoint& pos=QPoint(0,0), bool drawOverlays=true);

        /*! \brief draw the contained graph (including grid prints) into the given JKQTPEnhancedPainter
            \param painter JKQTPEnhancedPainter to which the plot should be drawn
            \param rect rectangle to plot into
         */
        void drawNonGrid(JKQTPEnhancedPainter& painter, const QRect& rect, bool drawOverlays=true);

        /*! \brief draw the contained graph (including grid prints) into the given JKQTPEnhancedPainter
            \param painter JKQTPEnhancedPainter to which the plot should be drawn
            \param pos where to plot the painter (left-top corner)
         */
        void drawNonGrid(JKQTPEnhancedPainter& painter, const QPoint& pos=QPoint(0,0), bool drawOverlays=true);
        void drawNonGridOverlays(JKQTPEnhancedPainter &painter, const QPoint& pos=QPoint(0,0));

        /** \brief emit plotUpdated() */
        void redrawPlot() { if (emitPlotSignals) emit plotUpdated(); }

        /** \brief controls, whether the signals plotUpdated() and overlaysUpdated() are emitted */
        void setEmittingPlotSignalsEnabled(bool __value);
        /** \brief returns, whether the signals plotUpdated() and overlaysUpdated() are emitted */
        bool isEmittingPlotSignalsEnabled() const;

        /*! \brief returns the property plotBorderTop ( \copybrief plotBorderTop ). \details Description of the parameter plotBorderTop is:  <BLOCKQUOTE>\copydoc plotBorderTop </BLOCKQUOTE>. \see plotBorderTop for more information */ 
        inline int getPlotBorderTop() const { return this->plotBorderTop; }
        /*! \brief returns the property plotBorderLeft ( \copybrief plotBorderLeft ). \details Description of the parameter plotBorderLeft is:  <BLOCKQUOTE>\copydoc plotBorderLeft </BLOCKQUOTE>. \see plotBorderLeft for more information */ 
        inline int getPlotBorderLeft() const { return this->plotBorderLeft; }
        /*! \brief returns the property plotBorderBottom ( \copybrief plotBorderBottom ). \details Description of the parameter plotBorderBottom is:  <BLOCKQUOTE>\copydoc plotBorderBottom </BLOCKQUOTE>. \see plotBorderBottom for more information */ 
        inline int getPlotBorderBottom() const { return this->plotBorderBottom; }
        /*! \brief returns the property plotBorderRight ( \copybrief plotBorderRight ). \details Description of the parameter plotBorderRight is:  <BLOCKQUOTE>\copydoc plotBorderRight </BLOCKQUOTE>. \see plotBorderRight for more information */ 
        inline int getPlotBorderRight() const { return this->plotBorderRight; }

        /*! \brief returns the property maintainAspectRatio ( \copybrief maintainAspectRatio ). \details Description of the parameter maintainAspectRatio is:  <BLOCKQUOTE>\copydoc maintainAspectRatio </BLOCKQUOTE>. \see maintainAspectRatio for more information */ 
        inline bool doesMaintainAspectRatio() const { return this->maintainAspectRatio; }
        /*! \brief sets the property aspectRatio ( \copybrief aspectRatio ) to the specified \a __value. 
            \details Description of the parameter aspectRatio is: <BLOCKQUOTE>\copydoc aspectRatio </BLOCKQUOTE> 
            \see aspectRatio for more information */ 
        inline void setAspectRatio(double __value)
        {
            if (this->aspectRatio != __value) {
                this->aspectRatio = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property aspectRatio ( \copybrief aspectRatio ). 
            \details Description of the parameter aspectRatio is: <BLOCKQUOTE>\copydoc aspectRatio </BLOCKQUOTE> 
            \see aspectRatio for more information */ 
        inline double getAspectRatio() const
        {
            return this->aspectRatio; 
        }

        /*! \brief returns the property maintainAxisAspectRatio ( \copybrief maintainAxisAspectRatio ). \details Description of the parameter maintainAxisAspectRatio is:  <BLOCKQUOTE>\copydoc maintainAxisAspectRatio </BLOCKQUOTE>. \see maintainAxisAspectRatio for more information */ 
        inline bool doesMaintainAxisAspectRatio() const { return this->maintainAxisAspectRatio; }
        /*! \brief sets the property axisAspectRatio ( \copybrief axisAspectRatio ) to the specified \a __value. 
            \details Description of the parameter axisAspectRatio is: <BLOCKQUOTE>\copydoc axisAspectRatio </BLOCKQUOTE> 
            \see axisAspectRatio for more information */ 
        inline void setAxisAspectRatio(double __value)
        {
            if (this->axisAspectRatio != __value) {
                this->axisAspectRatio = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property axisAspectRatio ( \copybrief axisAspectRatio ). 
            \details Description of the parameter axisAspectRatio is: <BLOCKQUOTE>\copydoc axisAspectRatio </BLOCKQUOTE> 
            \see axisAspectRatio for more information */ 
        inline double getAxisAspectRatio() const
        {
            return this->axisAspectRatio; 
        }

        /*! \brief sets the property useAntiAliasingForSystem ( \copybrief useAntiAliasingForSystem ) to the specified \a __value. 
            \details Description of the parameter useAntiAliasingForSystem is: <BLOCKQUOTE>\copydoc useAntiAliasingForSystem </BLOCKQUOTE> 
            \see useAntiAliasingForSystem for more information */ 
        inline void setUseAntiAliasingForSystem(bool __value)
        {
            if (this->useAntiAliasingForSystem != __value) {
                this->useAntiAliasingForSystem = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property useAntiAliasingForSystem ( \copybrief useAntiAliasingForSystem ). 
            \details Description of the parameter useAntiAliasingForSystem is: <BLOCKQUOTE>\copydoc useAntiAliasingForSystem </BLOCKQUOTE> 
            \see useAntiAliasingForSystem for more information */ 
        inline bool isUsingAntiAliasingForSystem() const
        {
            return this->useAntiAliasingForSystem; 
        }
        /*! \brief sets the property useAntiAliasingForGraphs ( \copybrief useAntiAliasingForGraphs ) to the specified \a __value. 
            \details Description of the parameter useAntiAliasingForGraphs is: <BLOCKQUOTE>\copydoc useAntiAliasingForGraphs </BLOCKQUOTE> 
            \see useAntiAliasingForGraphs for more information */ 
        inline void setUseAntiAliasingForGraphs(bool __value)
        {
            if (this->useAntiAliasingForGraphs != __value) {
                this->useAntiAliasingForGraphs = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property useAntiAliasingForGraphs ( \copybrief useAntiAliasingForGraphs ). 
            \details Description of the parameter useAntiAliasingForGraphs is: <BLOCKQUOTE>\copydoc useAntiAliasingForGraphs </BLOCKQUOTE> 
            \see useAntiAliasingForGraphs for more information */ 
        inline bool isUsingAntiAliasingForGraphs() const
        {
            return this->useAntiAliasingForGraphs; 
        }
        /*! \brief sets the property useAntiAliasingForText ( \copybrief useAntiAliasingForText ) to the specified \a __value. 
            \details Description of the parameter useAntiAliasingForText is: <BLOCKQUOTE>\copydoc useAntiAliasingForText </BLOCKQUOTE> 
            \see useAntiAliasingForText for more information */ 
        inline void setUseAntiAliasingForText(bool __value)
        {
            if (this->useAntiAliasingForText != __value) {
                this->useAntiAliasingForText = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property useAntiAliasingForText ( \copybrief useAntiAliasingForText ). 
            \details Description of the parameter useAntiAliasingForText is: <BLOCKQUOTE>\copydoc useAntiAliasingForText </BLOCKQUOTE> 
            \see useAntiAliasingForText for more information */ 
        inline bool isUsingAntiAliasingForText() const
        {
            return this->useAntiAliasingForText; 
        }

        /*! \brief sets the property graphColor ( \copybrief graphColor ) to the specified \a __value. 
            \details Description of the parameter graphColor is: <BLOCKQUOTE>\copydoc graphColor </BLOCKQUOTE> 
            \see graphColor for more information */ 
        inline void setGraphColor(const QColor & __value)
        {
            if (this->graphColor != __value) {
                this->graphColor = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property graphColor ( \copybrief graphColor ). 
            \details Description of the parameter graphColor is: <BLOCKQUOTE>\copydoc graphColor </BLOCKQUOTE> 
            \see graphColor for more information */ 
        inline QColor getGraphColor() const
        {
            return this->graphColor; 
        }
        /*! \brief sets the property graphWidth ( \copybrief graphWidth ) to the specified \a __value. 
            \details Description of the parameter graphWidth is: <BLOCKQUOTE>\copydoc graphWidth </BLOCKQUOTE> 
            \see graphWidth for more information */ 
        inline void setGraphWidth(double __value)
        {
            if (this->graphWidth != __value) {
                this->graphWidth = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property graphWidth ( \copybrief graphWidth ). 
            \details Description of the parameter graphWidth is: <BLOCKQUOTE>\copydoc graphWidth </BLOCKQUOTE> 
            \see graphWidth for more information */ 
        inline double getGraphWidth() const
        {
            return this->graphWidth; 
        }

        /*! \brief sets the property backgroundColor ( \copybrief backgroundColor ) to the specified \a __value. 
            \details Description of the parameter backgroundColor is: <BLOCKQUOTE>\copydoc backgroundColor </BLOCKQUOTE> 
            \see backgroundColor for more information */ 
        inline void setBackgroundColor(const QColor & __value)
        {
            if (this->backgroundColor != __value) {
                this->backgroundColor = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property backgroundColor ( \copybrief backgroundColor ). 
            \details Description of the parameter backgroundColor is: <BLOCKQUOTE>\copydoc backgroundColor </BLOCKQUOTE> 
            \see backgroundColor for more information */ 
        inline QColor getBackgroundColor() const
        {
            return this->backgroundColor; 
        }
        /*! \brief sets the property exportBackgroundColor ( \copybrief exportBackgroundColor ) to the specified \a __value. 
            \details Description of the parameter exportBackgroundColor is: <BLOCKQUOTE>\copydoc exportBackgroundColor </BLOCKQUOTE> 
            \see exportBackgroundColor for more information */ 
        inline void setExportBackgroundColor(const QColor & __value)
        {
            if (this->exportBackgroundColor != __value) {
                this->exportBackgroundColor = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property exportBackgroundColor ( \copybrief exportBackgroundColor ). 
            \details Description of the parameter exportBackgroundColor is: <BLOCKQUOTE>\copydoc exportBackgroundColor </BLOCKQUOTE> 
            \see exportBackgroundColor for more information */ 
        inline QColor getExportBackgroundColor() const
        {
            return this->exportBackgroundColor; 
        }
        /*! \brief sets the property plotBackgroundColor ( \copybrief plotBackgroundColor ) to the specified \a __value. 
            \details Description of the parameter plotBackgroundColor is: <BLOCKQUOTE>\copydoc plotBackgroundColor </BLOCKQUOTE> 
            \see plotBackgroundColor for more information */ 
        inline void setPlotBackgroundColor(const QColor & __value)
        {
            if (this->plotBackgroundColor != __value) {
                this->plotBackgroundColor = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property plotBackgroundColor ( \copybrief plotBackgroundColor ). 
            \details Description of the parameter plotBackgroundColor is: <BLOCKQUOTE>\copydoc plotBackgroundColor </BLOCKQUOTE> 
            \see plotBackgroundColor for more information */ 
        inline QColor getPlotBackgroundColor() const
        {
            return this->plotBackgroundColor; 
        }

        /*! \brief sets the property keyFont ( \copybrief keyFont ) to the specified \a __value. 
            \details Description of the parameter keyFont is: <BLOCKQUOTE>\copydoc keyFont </BLOCKQUOTE> 
            \see keyFont for more information */ 
        inline void setKeyFont(const QString & __value)
        {
            if (this->keyFont != __value) {
                this->keyFont = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property keyFont ( \copybrief keyFont ). 
            \details Description of the parameter keyFont is: <BLOCKQUOTE>\copydoc keyFont </BLOCKQUOTE> 
            \see keyFont for more information */ 
        inline QString getKeyFont() const
        {
            return this->keyFont; 
        }
        /*! \brief sets the property keyFontSize ( \copybrief keyFontSize ) to the specified \a __value. 
            \details Description of the parameter keyFontSize is: <BLOCKQUOTE>\copydoc keyFontSize </BLOCKQUOTE> 
            \see keyFontSize for more information */ 
        inline void setKeyFontSize(double __value)
        {
            if (this->keyFontSize != __value) {
                this->keyFontSize = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property keyFontSize ( \copybrief keyFontSize ). 
            \details Description of the parameter keyFontSize is: <BLOCKQUOTE>\copydoc keyFontSize </BLOCKQUOTE> 
            \see keyFontSize for more information */ 
        inline double getKeyFontSize() const
        {
            return this->keyFontSize; 
        }
        /*! \brief sets the property key_item_width ( \copybrief key_item_width ) to the specified \a __value. 
            \details Description of the parameter key_item_width is: <BLOCKQUOTE>\copydoc key_item_width </BLOCKQUOTE> 
            \see key_item_width for more information */ 
        inline void setKeyItemWidth(double __value)
        {
            if (this->key_item_width != __value) {
                this->key_item_width = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property key_item_width ( \copybrief key_item_width ). 
            \details Description of the parameter key_item_width is: <BLOCKQUOTE>\copydoc key_item_width </BLOCKQUOTE> 
            \see key_item_width for more information */ 
        inline double getKeyItemWidth() const
        {
            return this->key_item_width; 
        }
        /*! \brief sets the property key_item_height ( \copybrief key_item_height ) to the specified \a __value. 
            \details Description of the parameter key_item_height is: <BLOCKQUOTE>\copydoc key_item_height </BLOCKQUOTE> 
            \see key_item_height for more information */ 
        inline void setKeyItemHeight(double __value)
        {
            if (this->key_item_height != __value) {
                this->key_item_height = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property key_item_height ( \copybrief key_item_height ). 
            \details Description of the parameter key_item_height is: <BLOCKQUOTE>\copydoc key_item_height </BLOCKQUOTE> 
            \see key_item_height for more information */ 
        inline double getKeyItemHeight() const
        {
            return this->key_item_height; 
        }
        /*! \brief sets the property keyYSeparation ( \copybrief keyYSeparation ) to the specified \a __value. 
            \details Description of the parameter keyYSeparation is: <BLOCKQUOTE>\copydoc keyYSeparation </BLOCKQUOTE> 
            \see keyYSeparation for more information */ 
        inline void setKeyYSeparation(double __value)
        {
            if (this->keyYSeparation != __value) {
                this->keyYSeparation = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property keyYSeparation ( \copybrief keyYSeparation ). 
            \details Description of the parameter keyYSeparation is: <BLOCKQUOTE>\copydoc keyYSeparation </BLOCKQUOTE> 
            \see keyYSeparation for more information */ 
        inline double getKeyYSeparation() const
        {
            return this->keyYSeparation; 
        }
        /*! \brief sets the property key_line_length ( \copybrief key_line_length ) to the specified \a __value. 
            \details Description of the parameter key_line_length is: <BLOCKQUOTE>\copydoc key_line_length </BLOCKQUOTE> 
            \see key_line_length for more information */ 
        inline void setKeyLineLength(double __value)
        {
            if (this->key_line_length != __value) {
                this->key_line_length = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property key_line_length ( \copybrief key_line_length ). 
            \details Description of the parameter key_line_length is: <BLOCKQUOTE>\copydoc key_line_length </BLOCKQUOTE> 
            \see key_line_length for more information */ 
        inline double getKeyLineLength() const
        {
            return this->key_line_length; 
        }
        /*! \brief sets the property keyXMargin ( \copybrief keyXMargin ) to the specified \a __value. 
            \details Description of the parameter keyXMargin is: <BLOCKQUOTE>\copydoc keyXMargin </BLOCKQUOTE> 
            \see keyXMargin for more information */ 
        inline void setKeyXMargin(double __value)
        {
            if (this->keyXMargin != __value) {
                this->keyXMargin = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property keyXMargin ( \copybrief keyXMargin ). 
            \details Description of the parameter keyXMargin is: <BLOCKQUOTE>\copydoc keyXMargin </BLOCKQUOTE> 
            \see keyXMargin for more information */ 
        inline double getKeyXMargin() const
        {
            return this->keyXMargin; 
        }
        /*! \brief sets the property keyYMargin ( \copybrief keyYMargin ) to the specified \a __value. 
            \details Description of the parameter keyYMargin is: <BLOCKQUOTE>\copydoc keyYMargin </BLOCKQUOTE> 
            \see keyYMargin for more information */ 
        inline void setKeyYMargin(double __value)
        {
            if (this->keyYMargin != __value) {
                this->keyYMargin = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property keyYMargin ( \copybrief keyYMargin ). 
            \details Description of the parameter keyYMargin is: <BLOCKQUOTE>\copydoc keyYMargin </BLOCKQUOTE> 
            \see keyYMargin for more information */ 
        inline double getKeyYMargin() const
        {
            return this->keyYMargin; 
        }
        /*! \brief sets the property keyXSeparation ( \copybrief keyXSeparation ) to the specified \a __value. 
            \details Description of the parameter keyXSeparation is: <BLOCKQUOTE>\copydoc keyXSeparation </BLOCKQUOTE> 
            \see keyXSeparation for more information */ 
        inline void setKeyXSeparation(double __value)
        {
            if (this->keyXSeparation != __value) {
                this->keyXSeparation = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property keyXSeparation ( \copybrief keyXSeparation ). 
            \details Description of the parameter keyXSeparation is: <BLOCKQUOTE>\copydoc keyXSeparation </BLOCKQUOTE> 
            \see keyXSeparation for more information */ 
        inline double getKeyXSeparation() const
        {
            return this->keyXSeparation; 
        }

        /*! \brief sets the property keyXOffset ( \copybrief keyXOffset ) to the specified \a __value. 
            \details Description of the parameter keyXOffset is: <BLOCKQUOTE>\copydoc keyXOffset </BLOCKQUOTE> 
            \see keyXOffset for more information */ 
        inline void setKeyXOffset(double __value)
        {
            if (this->keyXOffset != __value) {
                this->keyXOffset = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property keyXOffset ( \copybrief keyXOffset ). 
            \details Description of the parameter keyXOffset is: <BLOCKQUOTE>\copydoc keyXOffset </BLOCKQUOTE> 
            \see keyXOffset for more information */ 
        inline double getKeyXOffset() const
        {
            return this->keyXOffset; 
        }
        /*! \brief sets the property keyYOffset ( \copybrief keyYOffset ) to the specified \a __value. 
            \details Description of the parameter keyYOffset is: <BLOCKQUOTE>\copydoc keyYOffset </BLOCKQUOTE> 
            \see keyYOffset for more information */ 
        inline void setKeyYOffset(double __value)
        {
            if (this->keyYOffset != __value) {
                this->keyYOffset = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property keyYOffset ( \copybrief keyYOffset ). 
            \details Description of the parameter keyYOffset is: <BLOCKQUOTE>\copydoc keyYOffset </BLOCKQUOTE> 
            \see keyYOffset for more information */ 
        inline double getKeyYOffset() const
        {
            return this->keyYOffset; 
        }
        /*! \brief sets the property showKey ( \copybrief showKey ) to the specified \a __value. 
            \details Description of the parameter showKey is: <BLOCKQUOTE>\copydoc showKey </BLOCKQUOTE> 
            \see showKey for more information */ 
        inline void setShowKey(bool __value)
        {
            if (this->showKey != __value) {
                this->showKey = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property showKey ( \copybrief showKey ). 
            \details Description of the parameter showKey is: <BLOCKQUOTE>\copydoc showKey </BLOCKQUOTE> 
            \see showKey for more information */ 
        inline bool getShowKey() const
        {
            return this->showKey; 
        }
        /*! \brief sets the property showKeyFrame ( \copybrief showKeyFrame ) to the specified \a __value. 
            \details Description of the parameter showKeyFrame is: <BLOCKQUOTE>\copydoc showKeyFrame </BLOCKQUOTE> 
            \see showKeyFrame for more information */ 
        inline void setShowKeyFrame(bool __value)
        {
            if (this->showKeyFrame != __value) {
                this->showKeyFrame = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property showKeyFrame ( \copybrief showKeyFrame ). 
            \details Description of the parameter showKeyFrame is: <BLOCKQUOTE>\copydoc showKeyFrame </BLOCKQUOTE> 
            \see showKeyFrame for more information */ 
        inline bool getShowKeyFrame() const
        {
            return this->showKeyFrame; 
        }
        /*! \brief sets the property keyFrameColor ( \copybrief keyFrameColor ) to the specified \a __value. 
            \details Description of the parameter keyFrameColor is: <BLOCKQUOTE>\copydoc keyFrameColor </BLOCKQUOTE> 
            \see keyFrameColor for more information */ 
        inline void setKeyFrameColor(const QColor & __value)
        {
            if (this->keyFrameColor != __value) {
                this->keyFrameColor = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property keyFrameColor ( \copybrief keyFrameColor ). 
            \details Description of the parameter keyFrameColor is: <BLOCKQUOTE>\copydoc keyFrameColor </BLOCKQUOTE> 
            \see keyFrameColor for more information */ 
        inline QColor getKeyFrameColor() const
        {
            return this->keyFrameColor; 
        }
        /*! \brief sets the property keyBackgroundColor ( \copybrief keyBackgroundColor ) to the specified \a __value. 
            \details Description of the parameter keyBackgroundColor is: <BLOCKQUOTE>\copydoc keyBackgroundColor </BLOCKQUOTE> 
            \see keyBackgroundColor for more information */ 
        inline void setKeyBackgroundColor(const QColor & __value)
        {
            if (this->keyBackgroundColor != __value) {
                this->keyBackgroundColor = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property keyBackgroundColor ( \copybrief keyBackgroundColor ). 
            \details Description of the parameter keyBackgroundColor is: <BLOCKQUOTE>\copydoc keyBackgroundColor </BLOCKQUOTE> 
            \see keyBackgroundColor for more information */ 
        inline QColor getKeyBackgroundColor() const
        {
            return this->keyBackgroundColor; 
        }
        /*! \brief sets the property keyFrameWidth ( \copybrief keyFrameWidth ) to the specified \a __value. 
            \details Description of the parameter keyFrameWidth is: <BLOCKQUOTE>\copydoc keyFrameWidth </BLOCKQUOTE> 
            \see keyFrameWidth for more information */ 
        inline void setKeyFrameWidth(double __value)
        {
            if (this->keyFrameWidth != __value) {
                this->keyFrameWidth = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property keyFrameWidth ( \copybrief keyFrameWidth ). 
            \details Description of the parameter keyFrameWidth is: <BLOCKQUOTE>\copydoc keyFrameWidth </BLOCKQUOTE> 
            \see keyFrameWidth for more information */ 
        inline double getKeyFrameWidth() const
        {
            return this->keyFrameWidth; 
        }
        /*! \brief sets the property keyAutosize ( \copybrief keyAutosize ) to the specified \a __value. 
            \details Description of the parameter keyAutosize is: <BLOCKQUOTE>\copydoc keyAutosize </BLOCKQUOTE> 
            \see keyAutosize for more information */ 
        inline void setKeyAutosize(bool __value)
        {
            if (this->keyAutosize != __value) {
                this->keyAutosize = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property keyAutosize ( \copybrief keyAutosize ). 
            \details Description of the parameter keyAutosize is: <BLOCKQUOTE>\copydoc keyAutosize </BLOCKQUOTE> 
            \see keyAutosize for more information */ 
        inline bool getKeyAutosize() const
        {
            return this->keyAutosize; 
        }
        /*! \brief sets the property keyPosition ( \copybrief keyPosition ) to the specified \a __value. 
            \details Description of the parameter keyPosition is: <BLOCKQUOTE>\copydoc keyPosition </BLOCKQUOTE> 
            \see keyPosition for more information */ 
        inline void setKeyPosition(const JKQTPKeyPosition & __value)
        {
            if (this->keyPosition != __value) {
                this->keyPosition = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property keyPosition ( \copybrief keyPosition ). 
            \details Description of the parameter keyPosition is: <BLOCKQUOTE>\copydoc keyPosition </BLOCKQUOTE> 
            \see keyPosition for more information */ 
        inline JKQTPKeyPosition getKeyPosition() const
        {
            return this->keyPosition; 
        }
        /*! \brief sets the property keyLayout ( \copybrief keyLayout ) to the specified \a __value. 
            \details Description of the parameter keyLayout is: <BLOCKQUOTE>\copydoc keyLayout </BLOCKQUOTE> 
            \see keyLayout for more information */ 
        inline void setKeyLayout(const JKQTPKeyLayout & __value)
        {
            if (this->keyLayout != __value) {
                this->keyLayout = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property keyLayout ( \copybrief keyLayout ). 
            \details Description of the parameter keyLayout is: <BLOCKQUOTE>\copydoc keyLayout </BLOCKQUOTE> 
            \see keyLayout for more information */ 
        inline JKQTPKeyLayout getKeyLayout() const
        {
            return this->keyLayout; 
        }

        /*! \brief sets the property plotLabelFontSize ( \copybrief plotLabelFontSize ) to the specified \a __value. 
            \details Description of the parameter plotLabelFontSize is: <BLOCKQUOTE>\copydoc plotLabelFontSize </BLOCKQUOTE> 
            \see plotLabelFontSize for more information */ 
        inline void setPlotLabelFontSize(double __value)
        {
            if (this->plotLabelFontSize != __value) {
                this->plotLabelFontSize = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property plotLabelFontSize ( \copybrief plotLabelFontSize ). 
            \details Description of the parameter plotLabelFontSize is: <BLOCKQUOTE>\copydoc plotLabelFontSize </BLOCKQUOTE> 
            \see plotLabelFontSize for more information */ 
        inline double getPlotLabelFontSize() const
        {
            return this->plotLabelFontSize; 
        }
        /*! \brief sets the property plotLabelFontname ( \copybrief plotLabelFontname ) to the specified \a __value. 
            \details Description of the parameter plotLabelFontname is: <BLOCKQUOTE>\copydoc plotLabelFontname </BLOCKQUOTE> 
            \see plotLabelFontname for more information */ 
        inline void setPlotLabelFontname(const QString & __value)
        {
            if (this->plotLabelFontname != __value) {
                this->plotLabelFontname = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property plotLabelFontname ( \copybrief plotLabelFontname ). 
            \details Description of the parameter plotLabelFontname is: <BLOCKQUOTE>\copydoc plotLabelFontname </BLOCKQUOTE> 
            \see plotLabelFontname for more information */ 
        inline QString getPlotLabelFontname() const
        {
            return this->plotLabelFontname; 
        }
        /*! \brief sets the property plotLabel ( \copybrief plotLabel ) to the specified \a __value. 
            \details Description of the parameter plotLabel is: <BLOCKQUOTE>\copydoc plotLabel </BLOCKQUOTE> 
            \see plotLabel for more information */ 
        inline void setPlotLabel(const QString & __value)
        {
            if (this->plotLabel != __value) {
                this->plotLabel = __value; 
                redrawPlot(); 
            } 
        } 
        /*! \brief returns the property plotLabel ( \copybrief plotLabel ). 
            \details Description of the parameter plotLabel is: <BLOCKQUOTE>\copydoc plotLabel </BLOCKQUOTE> 
            \see plotLabel for more information */ 
        inline QString getPlotLabel() const
        {
            return this->plotLabel; 
        }

        /*! \brief sets the property gridPrinting ( \copybrief gridPrinting ) to the specified \a __value. 
            \details Description of the parameter gridPrinting is: <BLOCKQUOTE>\copydoc gridPrinting </BLOCKQUOTE> 
            \see gridPrinting for more information */ 
        void setGridPrinting(bool __value);
        /*! \brief returns the property gridPrinting ( \copybrief gridPrinting ). 
            \details Description of the parameter gridPrinting is: <BLOCKQUOTE>\copydoc gridPrinting </BLOCKQUOTE> 
            \see gridPrinting for more information */ 
        bool getGridPrinting() const;
        /*! \brief sets the property gridPrintingCurrentX ( \copybrief gridPrintingCurrentX ) to the specified \a __value. 
            \details Description of the parameter gridPrintingCurrentX is: <BLOCKQUOTE>\copydoc gridPrintingCurrentX </BLOCKQUOTE> 
            \see gridPrintingCurrentX for more information */ 
        void setGridPrintingCurrentX(size_t __value);
        /*! \brief returns the property gridPrintingCurrentX ( \copybrief gridPrintingCurrentX ). 
            \details Description of the parameter gridPrintingCurrentX is: <BLOCKQUOTE>\copydoc gridPrintingCurrentX </BLOCKQUOTE> 
            \see gridPrintingCurrentX for more information */ 
        size_t getGridPrintingCurrentX() const;
        /*! \brief sets the property gridPrintingCurrentY ( \copybrief gridPrintingCurrentY ) to the specified \a __value. 
            \details Description of the parameter gridPrintingCurrentY is: <BLOCKQUOTE>\copydoc gridPrintingCurrentY </BLOCKQUOTE> 
            \see gridPrintingCurrentY for more information */ 
        void setGridPrintingCurrentY(size_t __value);
        /*! \brief returns the property gridPrintingCurrentY ( \copybrief gridPrintingCurrentY ). 
            \details Description of the parameter gridPrintingCurrentY is: <BLOCKQUOTE>\copydoc gridPrintingCurrentY </BLOCKQUOTE> 
            \see gridPrintingCurrentY for more information */ 
        size_t getGridPrintingCurrentY() const;

        /** \brief set the x- and y-positions of this JKQTPlotter in the grid-printing grid
         *
         * \see setGridPrinting(), addGridPrintingPlotter(), clearGridPrintingPlotters(), setGridPrintingCurrentX(), setGridPrintingCurrentY() \ref JKQTPBASELOTTER_SYNCMULTIPLOT
         */
        void setGridPrintingCurrentPos(size_t x, size_t y);



        /*! \brief sets the property currentSaveDirectory ( \copybrief currentSaveDirectory ) to the specified \a __value. 
            \details Description of the parameter currentSaveDirectory is: <BLOCKQUOTE>\copydoc currentSaveDirectory </BLOCKQUOTE> 
            \see currentSaveDirectory for more information */ 
        inline void setCurrentSaveDirectory(const QString & __value)
        {
            this->currentSaveDirectory = __value;
        } 
        /*! \brief returns the property currentSaveDirectory ( \copybrief currentSaveDirectory ). 
            \details Description of the parameter currentSaveDirectory is: <BLOCKQUOTE>\copydoc currentSaveDirectory </BLOCKQUOTE> 
            \see currentSaveDirectory for more information */ 
        inline QString getCurrentSaveDirectory() const
        {
            return this->currentSaveDirectory; 
        }
        /*! \brief sets the property currentFileFormat ( \copybrief currentFileFormat ) to the specified \a __value. 
            \details Description of the parameter currentFileFormat is: <BLOCKQUOTE>\copydoc currentFileFormat </BLOCKQUOTE> 
            \see currentFileFormat for more information */ 
        inline void setCurrentFileFormat(const QString & __value)
        {
            this->currentFileFormat = __value;
        } 
        /*! \brief returns the property currentFileFormat ( \copybrief currentFileFormat ). 
            \details Description of the parameter currentFileFormat is: <BLOCKQUOTE>\copydoc currentFileFormat </BLOCKQUOTE> 
            \see currentFileFormat for more information */ 
        inline QString getCurrentFileFormat() const
        {
            return this->currentFileFormat; 
        }
        /*! \brief sets the property CSVdecimalSeparator ( \copybrief CSVdecimalSeparator ) to the specified \a __value. 
            \details Description of the parameter CSVdecimalSeparator is: <BLOCKQUOTE>\copydoc CSVdecimalSeparator </BLOCKQUOTE> 
            \see CSVdecimalSeparator for more information */ 
        inline void setCSVdecimalSeparator(const QString & __value)
        {
            this->CSVdecimalSeparator = __value;
        } 
        /*! \brief returns the property CSVdecimalSeparator ( \copybrief CSVdecimalSeparator ). 
            \details Description of the parameter CSVdecimalSeparator is: <BLOCKQUOTE>\copydoc CSVdecimalSeparator </BLOCKQUOTE> 
            \see CSVdecimalSeparator for more information */ 
        inline QString getCSVdecimalSeparator() const
        {
            return this->CSVdecimalSeparator; 
        }
        /*! \brief sets the property CSVcommentInitializer ( \copybrief CSVcommentInitializer ) to the specified \a __value. 
            \details Description of the parameter CSVcommentInitializer is: <BLOCKQUOTE>\copydoc CSVcommentInitializer </BLOCKQUOTE> 
            \see CSVcommentInitializer for more information */ 
        inline void setCSVcommentInitializer(const QString & __value)
        {
            this->CSVcommentInitializer = __value;
        } 
        /*! \brief returns the property CSVcommentInitializer ( \copybrief CSVcommentInitializer ). 
            \details Description of the parameter CSVcommentInitializer is: <BLOCKQUOTE>\copydoc CSVcommentInitializer </BLOCKQUOTE> 
            \see CSVcommentInitializer for more information */ 
        inline QString getCSVcommentInitializer() const
        {
            return this->CSVcommentInitializer; 
        }

        /*! \brief returns the property iplotBorderTop ( \copybrief iplotBorderTop ). \details Description of the parameter iplotBorderTop is:  <BLOCKQUOTE>\copydoc iplotBorderTop </BLOCKQUOTE>. \see iplotBorderTop for more information */ 
        inline double getInternalPlotBorderTop() const { return this->iplotBorderTop; }
        /*! \brief returns the property iplotBorderLeft ( \copybrief iplotBorderLeft ). \details Description of the parameter iplotBorderLeft is:  <BLOCKQUOTE>\copydoc iplotBorderLeft </BLOCKQUOTE>. \see iplotBorderLeft for more information */ 
        inline double getInternalPlotBorderLeft() const { return this->iplotBorderLeft; }
        /*! \brief returns the property iplotBorderBottom ( \copybrief iplotBorderBottom ). \details Description of the parameter iplotBorderBottom is:  <BLOCKQUOTE>\copydoc iplotBorderBottom </BLOCKQUOTE>. \see iplotBorderBottom for more information */ 
        inline double getInternalPlotBorderBottom() const { return this->iplotBorderBottom; }
        /*! \brief returns the property iplotBorderRight ( \copybrief iplotBorderRight ). \details Description of the parameter iplotBorderRight is:  <BLOCKQUOTE>\copydoc iplotBorderRight </BLOCKQUOTE>. \see iplotBorderRight for more information */ 
        inline double getInternalPlotBorderRight() const { return this->iplotBorderRight; }
        /*! \brief returns the property plotWidth ( \copybrief plotWidth ). \details Description of the parameter plotWidth is:  <BLOCKQUOTE>\copydoc plotWidth </BLOCKQUOTE>. \see plotWidth for more information */ 
        inline int getPlotWidth() const { return this->plotWidth; }
        /*! \brief returns the property plotHeight ( \copybrief plotHeight ). \details Description of the parameter plotHeight is:  <BLOCKQUOTE>\copydoc plotHeight </BLOCKQUOTE>. \see plotHeight for more information */ 
        inline int getPlotHeight() const { return this->plotHeight; }
        /** \brief returns the internal JKQTMathText, used to render text with LaTeX markup */
        inline JKQTMathText* getMathText() { return &mathText; }
        /** \brief returns the internal JKQTMathText, used to render text with LaTeX markup */
        inline const JKQTMathText* getMathText() const { return &mathText; }
        /** \brief returns the x-axis objet of the plot */
        inline JKQTPHorizontalAxis* getXAxis() { return xAxis; }
        /** \brief returns the y-axis objet of the plot */
        inline JKQTPVerticalAxis* getYAxis() { return yAxis; }
        /** \brief returns the x-axis objet of the plot */
        inline const JKQTPHorizontalAxis* getXAxis() const { return xAxis; }
        /** \brief returns the y-axis objet of the plot */
        inline const JKQTPVerticalAxis* getYAxis() const { return yAxis; }


        /*! \brief returns the property actSavePlot ( \copybrief actSavePlot ). \details Description of the parameter actSavePlot is:  <BLOCKQUOTE>\copydoc actSavePlot </BLOCKQUOTE>. \see actSavePlot for more information */ 
        inline QAction* getActionSavePlot() const { return this->actSavePlot; }
        /*! \brief returns the property actSaveData ( \copybrief actSaveData ). \details Description of the parameter actSaveData is:  <BLOCKQUOTE>\copydoc actSaveData </BLOCKQUOTE>. \see actSaveData for more information */ 
        inline QAction* getActionSaveData() const { return this->actSaveData; }
        /*! \brief returns the property actCopyData ( \copybrief actCopyData ). \details Description of the parameter actCopyData is:  <BLOCKQUOTE>\copydoc actCopyData </BLOCKQUOTE>. \see actCopyData for more information */ 
        inline QAction* getActionCopyData() const { return this->actCopyData; }
        /*! \brief returns the property actCopyPixelImage ( \copybrief actCopyPixelImage ). \details Description of the parameter actCopyPixelImage is:  <BLOCKQUOTE>\copydoc actCopyPixelImage </BLOCKQUOTE>. \see actCopyPixelImage for more information */ 
        inline QAction* getActionCopyPixelImage() const { return this->actCopyPixelImage; }
        /*! \brief returns the property actCopyMatlab ( \copybrief actCopyMatlab ). \details Description of the parameter actCopyMatlab is:  <BLOCKQUOTE>\copydoc actCopyMatlab </BLOCKQUOTE>. \see actCopyMatlab for more information */ 
        inline QAction* getActionCopyMatlab() const { return this->actCopyMatlab; }
        /*! \brief returns the property actSavePDF ( \copybrief actSavePDF ). \details Description of the parameter actSavePDF is:  <BLOCKQUOTE>\copydoc actSavePDF </BLOCKQUOTE>. \see actSavePDF for more information */ 
        inline QAction* getActionSavePDF() const { return this->actSavePDF; }
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        /*! \brief returns the property actSavePS ( \copybrief actSavePS ). \details Description of the parameter actSavePS is:  <BLOCKQUOTE>\copydoc actSavePS </BLOCKQUOTE>. \see actSavePS for more information */ 
        inline QAction* getActionSavePS() const { return this->actSavePS; }
#endif
        /*! \brief returns the property actSavePix ( \copybrief actSavePix ). \details Description of the parameter actSavePix is:  <BLOCKQUOTE>\copydoc actSavePix </BLOCKQUOTE>. \see actSavePix for more information */ 
        inline QAction* getActionSavePix() const { return this->actSavePix; }
        /*! \brief returns the property actSaveSVG ( \copybrief actSaveSVG ). \details Description of the parameter actSaveSVG is:  <BLOCKQUOTE>\copydoc actSaveSVG </BLOCKQUOTE>. \see actSaveSVG for more information */ 
        inline QAction* getActionSaveSVG() const { return this->actSaveSVG; }
        /*! \brief returns the property actPrint ( \copybrief actPrint ). \details Description of the parameter actPrint is:  <BLOCKQUOTE>\copydoc actPrint </BLOCKQUOTE>. \see actPrint for more information */ 
        inline QAction* getActionPrint() const { return this->actPrint; }
        /*! \brief returns the property actSaveCSV ( \copybrief actSaveCSV ). \details Description of the parameter actSaveCSV is:  <BLOCKQUOTE>\copydoc actSaveCSV </BLOCKQUOTE>. \see actSaveCSV for more information */ 
        inline QAction* getActionSaveCSV() const { return this->actSaveCSV; }
        /*! \brief returns the property actZoomAll ( \copybrief actZoomAll ). \details Description of the parameter actZoomAll is:  <BLOCKQUOTE>\copydoc actZoomAll </BLOCKQUOTE>. \see actZoomAll for more information */ 
        inline QAction* getActionZoomAll() const { return this->actZoomAll; }
        /*! \brief returns the property actZoomIn ( \copybrief actZoomIn ). \details Description of the parameter actZoomIn is:  <BLOCKQUOTE>\copydoc actZoomIn </BLOCKQUOTE>. \see actZoomIn for more information */ 
        inline QAction* getActionZoomIn() const { return this->actZoomIn; }
        /*! \brief returns the property actZoomOut ( \copybrief actZoomOut ). \details Description of the parameter actZoomOut is:  <BLOCKQUOTE>\copydoc actZoomOut </BLOCKQUOTE>. \see actZoomOut for more information */ 
        inline QAction* getActionZoomOut() const { return this->actZoomOut; }
        /*! \brief returns the property actShowPlotData ( \copybrief actShowPlotData ). \details Description of the parameter actShowPlotData is:  <BLOCKQUOTE>\copydoc actShowPlotData </BLOCKQUOTE>. \see actShowPlotData for more information */ 
        inline QAction* getActionShowPlotData() const { return this->actShowPlotData; }
        /*! \brief returns the property lstAdditionalPlotterActions ( \copybrief lstAdditionalPlotterActions ). \details Description of the parameter lstAdditionalPlotterActions is:  <BLOCKQUOTE>\copydoc lstAdditionalPlotterActions </BLOCKQUOTE>. \see lstAdditionalPlotterActions for more information */ 
        inline AdditionalActionsMap getLstAdditionalPlotterActions() const { return this->lstAdditionalPlotterActions; }

        /** \brief this function registers additional actions to lstAdditionalPlotterActions, which are displayed in the context-menu */
        void registerAdditionalAction(const QString& key, QAction* act);
        void deregisterAdditionalAction(QAction* act);

        /*! \brief returns the property masterSynchronizeWidth ( \copybrief masterSynchronizeWidth ). \details Description of the parameter masterSynchronizeWidth is:  <BLOCKQUOTE>\copydoc masterSynchronizeWidth </BLOCKQUOTE>. \see masterSynchronizeWidth for more information */ 
        inline bool getMasterSynchronizeWidth() const { return this->masterSynchronizeWidth; }
        /*! \brief returns the property masterSynchronizeHeight ( \copybrief masterSynchronizeHeight ). \details Description of the parameter masterSynchronizeHeight is:  <BLOCKQUOTE>\copydoc masterSynchronizeHeight </BLOCKQUOTE>. \see masterSynchronizeHeight for more information */ 
        inline bool getMasterSynchronizeHeight() const { return this->masterSynchronizeHeight; }
        /*! \brief sets the property default_backgroundColor ( \copybrief default_backgroundColor ) to the specified \a __value. 
            \details Description of the parameter default_backgroundColor is:  <BLOCKQUOTE>\copydoc default_backgroundColor </BLOCKQUOTE> 
        	\see default_backgroundColor for more information */ 
        inline void setDefaultBackgroundColor(const QColor & __value)
        { 
        	this->default_backgroundColor = __value; 
        }
        /*! \brief sets the property default_plotBackgroundColor ( \copybrief default_plotBackgroundColor ) to the specified \a __value. 
            \details Description of the parameter default_plotBackgroundColor is:  <BLOCKQUOTE>\copydoc default_plotBackgroundColor </BLOCKQUOTE> 
        	\see default_plotBackgroundColor for more information */ 
        inline void setDefaultPlotBackgroundColor(const QColor & __value)
        { 
        	this->default_plotBackgroundColor = __value; 
        }

        /*! \brief sets the property fontSizePrintMultiplier ( \copybrief fontSizePrintMultiplier ) to the specified \a __value. 
            \details Description of the parameter fontSizePrintMultiplier is: <BLOCKQUOTE>\copydoc fontSizePrintMultiplier </BLOCKQUOTE> 
            \see fontSizePrintMultiplier for more information */ 
        inline void setFontSizePrintMultiplier(double __value)
        {
            this->fontSizePrintMultiplier = __value;
        } 
        /*! \brief returns the property fontSizePrintMultiplier ( \copybrief fontSizePrintMultiplier ). 
            \details Description of the parameter fontSizePrintMultiplier is: <BLOCKQUOTE>\copydoc fontSizePrintMultiplier </BLOCKQUOTE> 
            \see fontSizePrintMultiplier for more information */ 
        inline double getFontSizePrintMultiplier() const
        {
            return this->fontSizePrintMultiplier; 
        }
        /*! \brief sets the property lineWidthPrintMultiplier ( \copybrief lineWidthPrintMultiplier ) to the specified \a __value. 
            \details Description of the parameter lineWidthPrintMultiplier is: <BLOCKQUOTE>\copydoc lineWidthPrintMultiplier </BLOCKQUOTE> 
            \see lineWidthPrintMultiplier for more information */ 
        inline void setLineWidthPrintMultiplier(double __value)
        {
            this->lineWidthPrintMultiplier = __value;
        } 
        /*! \brief returns the property lineWidthPrintMultiplier ( \copybrief lineWidthPrintMultiplier ). 
            \details Description of the parameter lineWidthPrintMultiplier is: <BLOCKQUOTE>\copydoc lineWidthPrintMultiplier </BLOCKQUOTE> 
            \see lineWidthPrintMultiplier for more information */ 
        inline double getLineWidthPrintMultiplier() const
        {
            return this->lineWidthPrintMultiplier; 
        }
        /*! \brief returns the property fontSizeMultiplier ( \copybrief fontSizeMultiplier ). \details Description of the parameter fontSizeMultiplier is:  <BLOCKQUOTE>\copydoc fontSizeMultiplier </BLOCKQUOTE>. \see fontSizeMultiplier for more information */ 
        inline double getFontSizeMultiplier() const { return this->fontSizeMultiplier; }
        /*! \brief returns the property lineWidthMultiplier ( \copybrief lineWidthMultiplier ). \details Description of the parameter lineWidthMultiplier is:  <BLOCKQUOTE>\copydoc lineWidthMultiplier </BLOCKQUOTE>. \see lineWidthMultiplier for more information */ 
        inline double getLineWidthMultiplier() const { return this->lineWidthMultiplier; }


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

        void setAllGraphsInvisible();
        void setAllGraphsVisible();
        void setGraphVisible(int i, bool visible=true);

        /** \brief add a new graph, returns it's position in the graphs list, if the graph is already in the plot, this returns the index in the list */
        size_t addGraph(JKQTPPlotElement* gr);

        /** \brief returns \c true, if the given graph is in this plot */
        bool containsGraph(JKQTPPlotElement* gr) const;

        /** \brief move the given graph to the top, or add it, if it is not yet contained */
        size_t moveGraphTop(JKQTPPlotElement* gr);
        /** \brief move the given graph to the top, or add it, if it is not yet contained */
        size_t moveGraphBottom(JKQTPPlotElement* gr);

        /** \brief add a new graphs from a QVector */
        template <class TJKQTPGraphContainer>
        inline void addGraphs(const TJKQTPGraphContainer& gr) {
            for (auto it=gr.begin(); it!=gr.end(); ++it) {
                addGraph(*it);
            }
        }

        /** \brief add a new graph, returns it's position in the graphs list.
         *
         * \param xColumn      the column which contains the x-values of the datapoints
         * \param yColumn      the column which contains the y-values of the datapoints
         * \param title        a title for this specific graph which can be displayed in the key
         * \param graphStyle   the way how to plot the graph
         *
         * Both point to columns in the datastore explained in the JKQTBasePlotter class. The plotWidth, color, pen style ...
         * will be extracted from the automatic plot style creation mechanism implemented in JKQTBasePlotter::getNextStyle().
         * If you want to change them either use another overloaded version of addGraph(), or use getGraph() and setGraph():
         * \code
         * size_t i=addGraph(0,1,"graph1");
         * JKQTPPlotElement gr=getGraph(i);
         * gr.color=QColor("red");
         * setGraph(i, gr);
         * \endcode
         */
        size_t addGraph(size_t xColumn, size_t yColumn, const QString& title, JKQTPGraphPlotstyle graphStyle=JKQTPLines);

        /** \brief add a new graph, returns it's position in the graphs list.
         *
         * \param xColumn      the column which contains the x-values of the datapoints
         * \param yColumn      the column which contains the y-values of the datapoints
         * \param title        a title for this specific graph which can be displayed in the key
         * \param graphStyle   the way how to plot the graph
         * \param symbol       a symbol to use for the datapoints in some plot styles
         * \param color        the color of the graph
         * \param width        width (in pixel) of the graph
         * \param penstyle     the drawing style (solid, dashed ...) of the graph lines.
         *
         * Both point to columns in the datastore explained in the JKQTBasePlotter class.
         * The symbolSize is set to 10 and no error information is expected.
         */
        size_t addGraph(size_t xColumn, size_t yColumn, const QString& title, JKQTPGraphPlotstyle graphStyle, QColor color, JKQTPGraphSymbols symbol=JKQTPCross, Qt::PenStyle penstyle=Qt::SolidLine, double width=2);

        /** \brief add a new graph with x-error information, returns it's position in the graphs list.
         *
         *
         * \param xColumn      the column which contains the x-values of the datapoints
         * \param yColumn      the column which contains the y-values of the datapoints
         * \param xErrorColumn the column which contains the x-value errors of the datapoints
         * \param title        a title for this specific graph which can be displayed in the key
         * \param graphStyle   the way how to plot the graph
         * \param errorStyle   the drawing style (bars, lines ...) of the errors.
         *
         * Both point to columns in the datastore explained in the JKQTBasePlotter class.
         * The symbolSize is set to 10 and no error information is expected.
         */
        size_t addGraphWithXError(size_t xColumn, size_t yColumn, size_t xErrorColumn, const QString& title, JKQTPGraphPlotstyle graphStyle=JKQTPPoints, JKQTPErrorPlotstyle errorStyle=JKQTPErrorBars);

        /** \brief add a new graph with x-error information, returns it's position in the graphs list.
         *
         *
         * \param xColumn      the column which contains the x-values of the datapoints
         * \param yColumn      the column which contains the y-values of the datapoints
         * \param yErrorColumn the column which contains the y-value errors of the datapoints
         * \param title        a title for this specific graph which can be displayed in the key
         * \param graphStyle   the way how to plot the graph
         * \param errorStyle   the drawing style (bars, lines ...) of the errors.
         *
         * Both point to columns in the datastore explained in the JKQTBasePlotter class.
         * The symbolSize is set to 10 and no error information is expected.
         */
        size_t addGraphWithYError(size_t xColumn, size_t yColumn, size_t yErrorColumn, const QString& title, JKQTPGraphPlotstyle graphStyle=JKQTPPoints, JKQTPErrorPlotstyle errorStyle=JKQTPErrorBars);

        /** \brief add a new graph with x-error information, returns it's position in the graphs list.
         *
         *
         * \param xColumn      the column which contains the x-values of the datapoints
         * \param yColumn      the column which contains the y-values of the datapoints
         * \param xErrorColumn the column which contains the x-value errors of the datapoints
         * \param yErrorColumn the column which contains the y-value errors of the datapoints
         * \param title        a title for this specific graph which can be displayed in the key
         * \param graphStyle   the way how to plot the graph
         *
         * Both point to columns in the datastore explained in the JKQTBasePlotter class.
         * The symbolSize is set to 10 and no error information is expected. The errorStyle is set to JKQTPErrorBars
         * for both directions.
         */
        size_t addGraphWithXYError(size_t xColumn, size_t yColumn, size_t xErrorColumn, size_t yErrorColumn, const QString& title, JKQTPGraphPlotstyle graphStyle=JKQTPPoints);

        /** \brief add a boxplot graph to the plot
         *
         * \param title title of the plot
         * \param posColumn column containing the positions
         * \param medianColumn column containing the median values
         * \param minColumn column containing the minimum values
         * \param maxColumn column containing the maximum values
         * \param percentile25Column column containing the 25% percentiles
         * \param percentile75Column column containing the 75% percentiles
         * \param meanColumn column containing the mean value (this column is optional. If you don't want a mean symbol, set this to -1
         */
        size_t addHorizontalBoxplot(QString title, int posColumn, int medianColumn, int minColumn, int maxColumn, int percentile25Column, int percentile75Column, int meanColumn=-1);

        /** \brief add a boxplot graph to the plot
         *
         * \param title title of the plot
         * \param posColumn column containing the positions
         * \param medianColumn column containing the median values
         * \param minColumn column containing the minimum values
         * \param maxColumn column containing the maximum values
         * \param percentile25Column column containing the 25% percentiles
         * \param percentile75Column column containing the 75% percentiles
         * \param meanColumn column containing the mean value (this column is optional. If you don't want a mean symbol, set this to -1
         */
        size_t addVerticalBoxplot(QString title, int posColumn, int medianColumn, int minColumn, int maxColumn, int percentile25Column, int percentile75Column, int meanColumn=-1);

        /** \brief add one bargraph for each of the given set of \f$ f(x) \f$ -values which all use one column of x-values
         *
         *  \param xColumn column with the x-values, which are common to all bargraphs
         *  \param yColumns columns for the y-values
         *  \param titles titles of the plot
         *
         * Here is an example output:
         *   \image html plot_bargraphhorplot.png
         */
        void addHorizontalBargraph(size_t xColumn, QVector<size_t> yColumns, QStringList titles);

        /** \brief add a bargraph for the given set of \f$ f(x) \f$ -values for one column of x-values
         *
         *  \param xColumns columns with the x-values
         *  \param yColumn column for the y-values, which are common to all bargraphs
         *  \param titles titles of the plot
         *
         * Here is an example output:
         *   \image html plot_bargraphhorplot.png
         */
        void addVerticalBargraph(QVector<size_t> xColumns, size_t yColumn, QStringList titles);

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
            \param synchronizeZoomingMasterToSlave if set, also zooming in the master leads to a modification of the linked axes in the slave
            \param synchronizeZoomingSlaveToMaster if set, also zooming in the slave leads to a modification of the linked axes in the master
        */
        void synchronizeToMaster(JKQTBasePlotter* master, bool synchronizeWidth, bool synchronizeHeight, bool synchronizeZoomingMasterToSlave=false, bool synchronizeZoomingSlaveToMaster=false);

        /** \brief switches any synchronization off, that has been created by synchronizeToMaster() */
        void resetMasterSynchronization();







        /** \brief returns description of i'th overlay element */
        JKQTPOverlayElement* getOverlayElement(size_t i);

        /** \brief returns the number of overlay elements */
        size_t getOverlayElementCount();

        /** \brief remove the i-th overlay element */
        void deleteOverlayElement(size_t i, bool deletegraph=true);

        /** \brief remove the given overlay element, if it is contained */
        void deleteOverlayElement(JKQTPOverlayElement* gr, bool deletegraph=true);

        /** \brief remove all overlay elements
         *
         *  \param deleteGraphs if set \c true (default) the overlay element objects will also be deleted
         */
        void clearOverlayElement(bool deleteGraphs=true);

        /** \brief add a new overlay element, returns it's position in the overlay elements list, if the overlay element is already in the plot, this returns the index in the list */
        size_t addOverlayElement(JKQTPOverlayElement* gr);

        /** \brief returns \c true, if the given overlay element is in this plot */
        bool containsOverlayElement(JKQTPOverlayElement* gr) const;

        /** \brief move the given overlay element to the top, or add it, if it is not yet contained */
        size_t moveOverlayElementTop(JKQTPOverlayElement* gr);

        /** \brief add a new overlay elements from a QList */
        void addOverlayElements(const QList<JKQTPOverlayElement*>& gr);

        QVector<QLineF> getBoundingLinesX1Y1(QRectF *rect=nullptr) const;

    signals:
        /** \brief signal: emitted whenever the user selects a new x-y zoom range (by mouse) */
        void zoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQTBasePlotter* sender);

        /** \brief emitted when the plot has to be updated */
        void plotUpdated();

        /** \brief emitted when the overlay elements have to be updated */
        void overlaysUpdated();

        /** \brief emitted when the plot scaling had to be recalculated */
        void plotScalingRecalculated();

        /** \brief emitted before the plot scaling has been recalculated */
        void beforePlotScalingRecalculate();

    public:

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

        /** \brief internal tool class for text sizes
         *  \internal */
        struct LIB_EXPORT textSizeData {
            explicit textSizeData();
            double ascent, descent, width, strikeoutPos;
        };

        /** \brief internal tool class for text-sizess in a plot key
         *  \internal */
        struct LIB_EXPORT textSizeKey {
            explicit textSizeKey(const QFont& f, const QString& text, QPaintDevice *pd);
            explicit textSizeKey(const QString& fontName, double fontSize, const QString& text, QPaintDevice *pd);
            QString text;
            QFont f;
            int ldpiX, ldpiY, pdpiX, pdpiY;

            bool operator==(const textSizeKey& other) const;
        };
        textSizeData getTextSizeDetail(const QFont &fm, const QString& text,  QPainter& painter);
        textSizeData getTextSizeDetail(const QString& fontName, double fontSize, const QString& text,  QPainter &painter);
        void getTextSizeDetail(const QString& fontName, double fontSize, const QString& text,  QPainter &painter, double& width, double& ascent, double& descent, double& strikeoutPos);
        void getTextSizeDetail(const QFont& fm, const QString& text,  QPainter &painter, double& width, double& ascent, double& descent, double& strikeoutPos);
        QSizeF getTextSizeSize(const QFont &fm, const QString& text,   QPainter& painter);
        QSizeF getTextSizeSize(const QString& fontName, double fontSize, const QString& text,  QPainter &painter);

    protected:
        static QHash<JKQTBasePlotter::textSizeKey, JKQTBasePlotter::textSizeData> tbrh;

};

inline uint qHash(const JKQTBasePlotter::textSizeKey& data) {
    return qHash(data.f.family())+qHash(data.text);
}

#endif // JKQTPBASEPLOTTER_H
