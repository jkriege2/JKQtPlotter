/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    last modification: $LastChangedDate$  (revision $Rev$)

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



/**
 * \defgroup jkqtpbaseplotter Plotter Base Class
 * \ingroup jkqtplotter
 *
 *
 * This class implements the basis for plotter classes. It contains layout management code,
 * coordinate system management ... Use JKQPlotter if you need a widget
 */

/** \file jkqtpbaseplotter.h
  * \ingroup jkqtpbaseplotter
  */

//#include "../tools.h"
#include "jkqtptools.h"
#include "jkqtpdatastorage.h"
#include "jkqtmathtext.h"
#include "jkqtpbaseelements.h"
#include "jkqtpelements.h"
#include "jkqtpgeoelements.h"
#include "jkqtpimageelements.h"
#include "jkqtpoverlayelements.h"


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
 #include "jkqtp_imexport.h"


#ifndef JKQTPBASEPLOTTER_H
#define JKQTPBASEPLOTTER_H





/** \brief base class for 2D plotter classes
 * \ingroup jkqtplotter
 *
 * This class implements basic functionalities for the plotter classes. Those are:
 *   -# data storage using a JKQTPdatastore object
 *   -# coordinate transforms
 *   -# a set of properties for the graphs (colors, widthes ...) and also a system (see getNextStyle() to automatically
 *     choose a drawing style for different graphs.
 *   -# plot a set of overlay elements (may be used for fast plotting of indicators onto a complex plot)
 *   -# drawing the coordinate axes, grids ... (logarithmic and linear)
 *   -# saveing and printing the resulting plots
 * .
 *
 * This class is NOT a widget, if you need a plotting widget, use JKQtPlotter. This class may be used to
 * plot onto any JKQTPEnhancedPainter.
 *
 *
 *
 * \section jkqtplotter_base_datastore Data Storage
 * As already mentioned this class does not provide means to draw graphs, but it contains a basic mechanism to associate
 * data with it. This is imeplemented, using a JKQTPdatastore object together with some convenience access methods. No further
 * data access is implemented and child classes are NOT forced to use this datastore. Hide it if you want. JKQtPlotter shows
 * how to use it. This class implement a graph management, where graphs simply point to a set of columns inside the datastore
 * that contain the actual plot data!
 *
 * If you call the \link JKQtPlotterBase::JKQtPlotterBase() constructor \endlink with no arguments, it will create an internal
 * datastore object and you can start adding data by using get_datastore(). If you have an external JKQTPdatastore object you can
 * give it as parameter to the constructor or use one of the other methods:
 *  - useExternalDatastore(): \copybrief JKQtPlotterBase::useExternalDatastore()
 *  - useAsInternalDatastore(): \copybrief JKQtPlotterBase::useAsInternalDatastore()
 *  - useInternalDatastore(): \copybrief JKQtPlotterBase::useInternalDatastore()
 *  - forceInternalDatastore(): \copybrief JKQtPlotterBase::forceInternalDatastore()
 *.
 *
 *
 *
 * \section jkqtplotter_base_systems Coordinate Systems and Transformations
 * These topics are discussed in the help for JKQTPcoordinateAxis. There is a set of coordinate transform
 * methods (x2p(), y2p(), p2x(), p2y() which only call the respective methods in xAxis and yAxis objects.
 *
 *
 * \section jkqtplotter_base_grids Axis Ticks and Grids
 *    - The plotting of coordinate axes and grids, as well as coordinate transforms is done by
 *      JKQTPcoordinateAxis descendents (see documentation there)
 *  .
 *  If you want to set the axis properties, use getXAxis() or getYAxis() to get a pointer to the axis objects which then
 *  may be used to set the axis properties.
 *
 *
 * \section jkqtplotter_base_key Graph Keys
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
 * There is also a possibility to determine the size of the key automatically, so all text fits in. This is activted by the property keyAutosize. If this
 * is \c true the function getKeyExtent() has to check the width of every key item and take it into account when calculating the width and height of the
 * key content. By default this feature is switched ON.
 *
 *
 * \section jkqtplotter_base_saveprint Saving and Printing Graphs
 * This class implement a set of function to save and print the graphs:
 *   - print() prints the graph on a QPrinter object
 *   - saveAsPixelImage() saves the plot into a pixel image file (PNG, TIFF, ... formats, as supported by Qt)
 *   - saveAsPDF() saves the graph as a PDF file (using the Qt printing engine)
 *   - saveAsPS() saves the graph as a PDF file (using the Qt printing engine)
 *   - saveAsSVG() saves the graph as a SVG file (using the Qt SVG library)
 *   - saveAsCSV() saves the data of the plot as comma separated values
 *   - saveAsSYLK() saves the data of the plot as SYLK spreadsheet
 *   - saveAsDIF() saves the data of the plot as data interchange format file
 * .
 *
 * As one often want's to combine different graphs, there is a possibility to combine this graph with more other graphs.
 * To do so one can think of the graphs to be part of a grid where each graph is contained in one cell. By default this
 * mechanism is deactivated. You can activate it by calling set_gridPrinting(true). Then you can set the position of the
 * current graph by calling set_gridPrintingCurrentX() and set_gridPrintingCurrentY(). Add additional graphs by calling
 * addGridPrintingPlotter(). The position of the current graph is 0,0 by default. Afterwards the save and print routines
 * will export/print all graphs, not just the current one. There will be no additional border between the graphs, as the
 * class expects the internal graph borders to be sufficient.
 *
 *
 * \section jkqtplotter_base_defaultvalues Default Properties
 * The plot is configured by a huge set of properties. For each property there is also a second protected variable which
 * contains its default value. This way it is possible to store only those parameters in an INI file which have changed with
 * respect to the default values. If the property is called \c property then the according default value is stored in
 * \c def_property. To reduce the code to be entered you can use the JKQTPPROPERTY() macro.
 *
 * Default values are available only for properties that control the appearance of the graphs (line widths, border widths,
 * color, fonts ...), not the type of the graph (xmin, xmax, logXAxis, ...)
 *
 * \section jkqtplotter_base_userprops User Properties
 * There is a subset of options that describe how the user interacted with the plotter (export/print scaling factors etc, save directories,
 * other export settings, ...). These are not stored/loaded using saveSettings() and loadSettings(), but using saveUserSettings() and loadUserSettings().
 * These methods MAY (strictly optional and turned off by default) be called by saveSettings() and loadSettings(), if the property userSettigsFilename is
 * set (not-empty). In this case the suer settings are stored/loaded also everytime they are changed by the user or programmatically.
 *
 * \section jkqtplotter_base_aspectratios Aspect Ratios
 * First note that this functionality is only available and activated if both axes are linear!
 *
 * You can set two different aspect ratios:
 *   - The ratio of plotWidth/plotHeight (set_aspectRatio(), set_maintainAspectRatio()) will keep the plots pixel-width and height at a certain value.
 *   - The ratio of (xmax-xmin)/(ymax-ymin) (set_axisAspectRatio(), set_maintainAxisAspectRatio()) will keep the displayed axis ranges in a certain ratio.
 * .
 * So to achieve different effects, use these combinations:
 *   - you have a 200x100 range where each 1x1-pixel should have an aspect ratio of 4:
 *     \code
 *        set_aspectRatio(200.0/100.0);
 *        set_axisAspectRatio(4.0*get_aspectRatio());
 *     \endcode
 * .
 */
class LIB_EXPORT JKQtBasePlotter: public QObject {
        Q_OBJECT
    public:
        typedef QMap<QString, QList<QPointer<QAction> > > AdditionalActionsMap;
        typedef QMapIterator<QString, QList<QPointer<QAction> > > AdditionalActionsMapIterator;
    protected:

        /** \brief used to plot LaTeX markup */
        JKQTmathText mathText;

        /** \brief object used for the x-axis */
        JKQTPhorizontalAxis* xAxis;
        /** \brief object used for the y-axis */
        JKQTPverticalAxis* yAxis;

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
        QList<JKQTPgridPrintingItem> gridPrintingList;
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
        JKQTPPROPERTY(QString, CSVdecimalSeparator);
        /** \brief this string is used to introduce comments in text output when exporting data */
        JKQTPPROPERTY(QString, CSVcommentInitializer)

        /** \brief an object which manages all data columns for this plotter class */
        JKQTPdatastore* datastore;
        /** \brief indicates whether the datastore is managed (allocated/freed) internally or externally */
        bool datastoreInternal;


        /** \brief width of the plot widget */
        int widgetWidth;

        /** \brief height of the plot widget */
        int widgetHeight;


        /** \brief free space between widget top border and plot top border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to iplotBorderTop */
        JKQTPPROPERTY(int, plotBorderTop);
        /** \brief free space between widget top border and plot top border, as used to plot the graph (mnay be altered from user input ) */
        int iplotBorderTop;
        int iplotKeyBorderTop;
        /** \brief height of the plot title (or 0 if no title) */
        int iTitleHeight;
        /** \brief free space between widget left border and plot left border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to iplotBorderLeft  */
        JKQTPPROPERTY(int, plotBorderLeft);
        /** \brief free space between widget top border and plot top border, as used to plot the graph (mnay be altered from user input ) */
        int iplotBorderLeft;
        int iplotKeyBorderLeft;
        /** \brief free space between widget bottom border and plot bottom border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to iplotBorderBottom  */
        JKQTPPROPERTY(int, plotBorderBottom);
        /** \brief free space between widget top border and plot top border, as used to plot the graph (mnay be altered from user input ) */
        int iplotBorderBottom;
        int iplotKeyBorderBottom;
        /** \brief free space between widget right border and plot right border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to iplotBorderRight  */
        JKQTPPROPERTY(int, plotBorderRight);
        /** \brief free space between widget top border and plot top border, as used to plot the graph (mnay be altered from user input ) */
        int iplotBorderRight;
        int iplotKeyBorderRight;



        /** \brief free space between widget top border and plot top border (including coordinate axes) */
        int iplotBorderTop_nographs;
        /** \brief free space between widget left border and plot left border (including coordinate axes) */
        int iplotBorderLeft_nographs;
        /** \brief free space between widget right border and plot right border (including coordinate axes) */
        int iplotBorderBottom_nographs;
        /** \brief free space between widget bottom border and plot bottom border (including coordinate axes) */
        int iplotBorderRight_nographs;

        /** \brief indicates whether the widget should maintain an aspect ratio of plotwidth and plotheight */
        JKQTPPROPERTY(bool, maintainAspectRatio);
        /** \brief the aspect ratio of plotwidth and plotheight to maintain, if \c maintainAspectRatio==true */
        JKQTPPROPERTY(double, aspectRatio);

        /** \brief indicates whether the axes should maintain an aspect ratio */
        JKQTPPROPERTY(bool, maintainAxisAspectRatio);
        /** \brief the aspect ratio of axis widths to maintain, if \c maintainAxisAspectRatio==true */
        JKQTPPROPERTY(double, axisAspectRatio);

        /** \brief plot width in pixels inside the widget (calculated by calcPlotScaling() from plotBorderLeft, plotBorderRight and widgetWidth) */
        int plotWidth;
        /** \brief plot height in pixels inside the widget (calculated by calcPlotScaling() from plotBorderTop, plotBorderBottom and widgetHeight) */
        int plotHeight;



        /** \brief color of the plotted graph */
        JKQTPPROPERTY(QColor, graphColor);
        /** \brief colors used in modes where more than one graph is beeing plottet */
        QColor manyGraphsColor[50];
        QColor def_manyGraphsColor[50];
        /** \brief number of actually defined items in manyGraphsColor */
        int manyGraphsColorCount;
        /** \brief pen styles used in modes where more than one graph is beeing plottet */
        JKQTPPROPERTY(Qt::PenStyle, manyGraphsStyle[5]);
        /** \brief width of the plotted graph (in pixel) */
        JKQTPPROPERTY(double, graphWidth);



        /** \brief indicates whether to use clipping (hack for printing, see print() ) */
        bool useClipping;



        /** \brief color of the coordinate system */
        JKQTPPROPERTY(QColor, systemColor);
        /** \brief width of the coordinate (in pixel) */
        JKQTPPROPERTY(double, systemWidth);
        /** \brief color of the background*/
        JKQTPPROPERTY(QColor, backgroundColor);
        /** \brief color of the background when exporting*/
        JKQTPPROPERTY(QColor, exportBackgroundColor);
        /** \brief color of the plot's background*/
        JKQTPPROPERTY(QColor, plotBackgroundColor);


        /** \brief indicates whether to plot a frame around the key */
        JKQTPPROPERTY(bool, showKeyFrame);
        /** \brief color of the key frame line */
        JKQTPPROPERTY(QColor, keyFrameColor);
        /** \brief width of the key frame line */
        JKQTPPROPERTY(double, keyFrameWidth);
        /** \brief color of the key background */
        JKQTPPROPERTY(QColor, keyBackgroundColor);
        /** \brief indicates whether to plot a key */
        JKQTPPROPERTY(bool, showKey);
        /** \brief font face for key labels */
        JKQTPPROPERTY(QString, keyFont);
        /** \brief font size for key labels [in points] */
        JKQTPPROPERTY(double, keyFontSize);
        /** \brief width of a key item in pixels [in units of width of 'X' set in keyFont, keyFontSize] */
        JKQTPPROPERTY(double, key_item_width);
        /** \brief height of a key item in pixels [in units of height  keyFont, keyFontSize] */
        JKQTPPROPERTY(double, key_item_height);
        /** \brief length of the line samples in the key in pixels [in units of width of 'X' set in keyFont, keyFontSize] */
        JKQTPPROPERTY(double, key_line_length);
        /** \brief x-distance between key frame and key content [in units of width of 'X' set in keyFont, keyFontSize] */
        JKQTPPROPERTY(double, keyXMargin);
        /** \brief y-distance between key frame and key content [in units of width of 'x' set in keyFont, keyFontSize] */
        JKQTPPROPERTY(double, keyYMargin);
        /** \brief x-offset of the key from the border of the plot [in units of width of 'X' set in keyFont, keyFontSize] */
        JKQTPPROPERTY(double, keyXOffset);
        /** \brief y-offset of the key from the border of the plot [in units of width of 'x' set in keyFont, keyFontSize] */
        JKQTPPROPERTY(double, keyYOffset);
        /** \brief distance between key line example and key text [in units of width of 'X' set in keyFont, keyFontSize] */
        JKQTPPROPERTY(double, keyXSeparation);
        /** \brief distance between two key entries [in units of height of keyFont, keyFontSize] */
        JKQTPPROPERTY(double, keyYSeparation);
        /** \brief key position */
        JKQTPPROPERTY(JKQTPkeyPosition, keyPosition);
        /** \brief the key layout */
        JKQTPPROPERTY(JKQTPkeyLayout, keyLayout);
        /** \brief determine width of the key automatically */
        JKQTPPROPERTY(bool, keyAutosize);

        /** \brief the plot label text */
        QString plotLabel;

        /** \brief the plot label font name */
        JKQTPPROPERTY(QString, plotLabelFontname);

        /** \brief the plot label font size */
        JKQTPPROPERTY(double, plotLabelFontSize);

        /** \brief calculate the scaling and offset values from axis min/max values */
        void calcPlotScaling(JKQTPEnhancedPainter& painter);

        /** \brief set the standard settings (colors, line widths ...) */
        void initSettings();

        /** \brief specifies whether this class emits signals, like zoomChangedLocally() */
        bool emitSignals;

        /** \brief specifies whether to use antialiasing for plotting the coordinate system */
        JKQTPPROPERTY(bool, useAntiAliasingForSystem);

        /** \brief specifies whether to use antialiasing for plotting the graphs
         *
         * \note You can set this property \c false to increase plotting speed of complex plots (with many graphs inside). You can reach a
         *       roughly three-fold speed improvement!
         */
        JKQTPPROPERTY(bool, useAntiAliasingForGraphs);

        /** \brief specifies whether to use antialiasing when drawing any text */
        JKQTPPROPERTY(bool, useAntiAliasingForText);

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
        QList<JKQTPgraph*> graphs;


        QList<JKQTPoverlayElement*> overlays;


        /** \brief used as maximum/minimum pixel value for plotting */
        double plot_minmaxcoorinate;




        /** \brief hidden default constructor */
        JKQtBasePlotter() {};



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
        virtual void plotSystemGrid(JKQTPEnhancedPainter& painter);
        /** \brief plot the x coordinate axis (incl. labels) */
        virtual void plotSystemXAxis(JKQTPEnhancedPainter& painter);
        /** \brief plot the x coordinate axis (incl. labels) */
        virtual void plotSystemYAxis(JKQTPEnhancedPainter& painter);
        /** \brief plot the graphs */
        virtual void plotGraphs(JKQTPEnhancedPainter& painter);
        /** \brief plot a key */
        virtual void plotKey(JKQTPEnhancedPainter& painter);
        /** \brief plot all overlay elements */
        virtual void plotOverlays(JKQTPEnhancedPainter& painter);

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
        virtual void plotKeyContents(JKQTPEnhancedPainter& painter, double x, double y, double width, double height);

        /** \brief returns the size of the key (as call-by-reference parameters
         *
         * Any implementation of key plotting has to overwrite this function and use it to return the size of the key that would be
         * plotted by plotKeyContents(). This class will use the return values to plot the frame of the key and also supply them to
         * plotKeyContents(). If height or width are returned 0, no key is plotted
         *
         * The implementation in here returns zero size!
         */
        virtual void getKeyExtent(JKQTPEnhancedPainter& painter, double *width, double *height, double *text_width=NULL, double *text_height=NULL, int *columns_count=NULL, int* lines_count=NULL);
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
        JKQtBasePlotter* masterPlotter;
        /** \brief synchronize plot width with masterPlotter */
        bool masterSynchronizeWidth;
        /** \brief synchronize plot height with masterPlotter */
        bool masterSynchronizeHeight;



        bool emitPlotSignals;


        void printpreview(QPrinter* p, bool setabsolutesize=false);
        bool printpreviewNew(QPaintDevice* paintDevice, bool setAbsolutePaperSize=false, double printsizeX_inMM=-1.0, double printsizeY_inMM=-1.0, bool displayPreview=true);
        bool exportpreview(QSizeF pageSize, bool unitIsMM=false);

        JKQTPSET_MACRO(double, fontSizeMultiplier)
        JKQTPSET_MACRO(double, lineWidthMultiplier)
        JKQTPGET_SET_MACRO(double, printMagnification)
        JKQTPGET_SET_MACRO(double, paintMagnification)

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

    public slots:




        /** \brief sets x/ymin and x/ymax to the supplied values and replots the graph (zoom operation!) */
        void zoom(double nxmin, double nxmax, double nymin, double nymax);

        /** \brief sets whether to plot grid lines or not */
        void setGrid(bool val);

        /** \brief save the current plot as a pixel image image (PNG ...), if filename is empty a file selection dialog is displayed */
        void saveAsPixelImage(QString filename=QString(""), bool displayPreview=true, const QByteArray &outputFormat=QByteArray());

        /** \brief copy the current plot as a pixel image to the clipboard */
        void copyPixelImage();

        /** \brief save the current plot as a SVG file, with the current widget aspect ratio, if filename is empty a file selection dialog is displayed  */
        void saveAsSVG(QString filename=QString(""), bool displayPreview=true);

        /** \brief save the current plot as a PDF file, with the current widget aspect ratio, if filename is empty a file selection dialog is displayed  */
        void saveAsPDF(QString filename=QString(""), bool displayPreview=true);

        /** \brief save the current plot as a PS file, with the current widget aspect ratio, if filename is empty a file selection dialog is displayed */
        void saveAsPS(QString filename=QString(""), bool displayPreview=true);

        /** \brief save the current plot as an image file, with the current widget aspect ratio, if filename is empty a file selection dialog is displayed.
        *          The image format is extracted from the file extension (jpeg, tiff, png, pdf, ...) */
        void saveImage(QString filename=QString(""), bool displayPreview=true);

        /** \brief save the data used for the current plot. The file format is extracted from the file extension (csv, ...)
         *
         * The parameter \a format specifies the export format. if it is empty the format will be choosen according to the file extension, or
         * if \a filename is also empty the format will be choosen according to what is selected in the file selection dialog.
         *
         * If \a format is \c "slk" the output will be in SYLK format, if \a format is \c "csv" or \a "dat" the output will be comma separated values
         * and if \a format is \c "txt" the output will be tab separated values.
         */
        void saveData(QString filename=QString(""), QString jkqtp_format=QString(""));
        /** \brief copy the data used for the current plot to the clipboard
         *
         *  copies data as tab separated data with the system-decimal point.
         */
        void copyData();
        /** \brief copy the data used for the current plot to the clipboard in Matlab format
         */
        void copyDataMatlab();
        /** \brief save the current plot data as a Comma Separated Values (CSV) file*/
        virtual void saveAsCSV(QString filename=QString(""));

        /** \brief save the current plot data as a DIF file*/
        virtual void saveAsDIF(QString filename=QString(""));

        /** \brief save the current plot data as a SYLK spreadsheet file*/
        virtual void saveAsSYLK(QString filename=QString(""));

        /** \brief save the current plot data as a Matlab Script*/
        virtual void saveAsMatlab(QString filename=QString(""));

        /** \brief save the current plot data as a Semicolon Separated Values (SSV) file*/
        virtual void saveAsSemicolonSV(QString filename=QString(""));

        /** \brief save the current plot data as a Tabulator Separated Values (CSV) file*/
        virtual void saveAsTabSV(QString filename=QString(""));

        /** \brief save the current plot data as a Semicolon Separated Values (CSV) file for german Excel, i.e. with comma as decimal separator*/
        virtual void saveAsGerExcelCSV(QString filename=QString(""));

        /** \brief print the current plot, if printer is \c NULL a printer selection dialog is displayed */
        void print(QPrinter* printer=NULL, bool displayPreview=true);

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
        virtual void zoomToFit(bool zoomX=true, bool zoomY=true, bool includeX0=false, bool includeY0=false, double scaleX=1.05, double scaleY=1.05);

        /** \brief zooms into the graph (the same as turning the mouse wheel) by the given factor */
        void zoomIn(double factor=2.0);
        /** \brief zooms out of the graph (the same as turning the mouse wheel) by the given factor */
        void zoomOut(double factor=2.0) {
            zoomIn(1.0/factor);
        };

        /** \brief set the datarange of all current graphs to the given values */
        void setGraphsDataRange(long long datarange_start, long long datarange_end);

        void set_maintainAspectRatio(bool value) {
            maintainAspectRatio=value;
            update_plot();
        }

        void set_maintainAxisAspectRatio(bool value) {
            maintainAxisAspectRatio=value;
            update_plot();
        }

        void set_userSettigsFilename(const QString& filename, const QString& prefix);
        void set_userSettigsFilename(const QString& filename);
        void set_userSettigsPrefix(const QString& prefix);
        QString get_userSettigsFilename() const;
        QString get_userSettigsPrefix() const;

        /** \brief set all graphs invisible, except i */
        void setOnlyGraphVisible(int i);
        /** \brief set all graphs invisible, except graph start, start+n, start+2*n, ... */
        void setOnlyNthGraphsVisible(int start, int n);

        /** \brief saves the plot user properties if userSettingsFilename is set, does nothing otherwise */
        void loadUserSettings();

        /** \brief saves the plot user properties if userSettingsFilename is set, does nothing otherwise */
        void saveUserSettings();
    public:
        /** \brief class constructor
         *
         * if \a datastore_internal is \c true then the constructor will create an internal datastore object. The datastore
         * will be managed (freed) by this class. If \a datastore_internal is \c false the class will use tha datastore provided
         * in \a datast as an external datastore. You can modify this later by using useInternalDatastore() and useExternalDatastore().
         */
        JKQtBasePlotter(bool datastore_internal=true, QObject* parent=NULL, JKQTPdatastore* datast=NULL);

        /** \brief class destructor */
        virtual ~JKQtBasePlotter();

        /** \brief returns a pointer to the datastore used by this object */
        inline JKQTPdatastore* getDatastore() { return datastore; };

        /** \brief tells the plotter object to use the given external datastore.
         *
         * If the current datastore is internally managed, this method will free that object and use the supplied datastore
         * with external management. If the current datastore is already external, this method will simply replace it by the
         * new one.
         */
        void useExternalDatastore(JKQTPdatastore* newStore);

        /** \brief tells the plotter object to use the given external datastore and treat it as an internal one (i.e. free it
         *         when the plotter object ist destroyed.
         */
        void useAsInternalDatastore(JKQTPdatastore* newStore);

        /** \brief tells the plotter object to use an internal datastore. A new internal datastore object is generated only if
         *         the current datastore is not internal.
         */
        void useInternalDatastore();

        /** \brief tells the plotter object to use an internal datastore (just like useInternalDatastore() ), but forces the
         *         generation of a new datastore, even if the current one is already internal (the current one will be freed in
         *         the lather case */
        void forceInternalDatastore();

        JKQTPGET_MACRO(bool, emitSignals)

        void set_emitSignals(bool enabled);


        /** \brief loads the plot properties from a QSettings object */
        virtual void loadSettings(QSettings& settings, QString group=QString("plots"));

        /** \brief saves the plot properties into a QSettings object.
         *
         * This method only saves those properties that differ from their default value.
         */
        virtual void saveSettings(QSettings& settings, QString group=QString("plots"));



        /** \brief loads the plot user properties from a QSettings object */
        virtual void loadUserSettings(QSettings& settings, QString group=QString("plots_user"));

        /** \brief saves the plot user properties into a QSettings object. */
        virtual void saveUserSettings(QSettings& settings, QString group=QString("plots_user"));

        /** \brief sets the width of the plot widget */
        void setWidth(int wid);

        /** \brief resize the plot */
        void resize(int width, int height);

        /** \brief gets the width of the plot widget */
        inline int getWidth() {return widgetWidth; };

        /** \brief gets the width of the plot widget */
        inline int getHeight() {return widgetHeight; };

        /** \brief sets the width of the plot widget */
        void setHeight(int heigh);

        /** \brief sets the borders of the plot, see also get_plotBorderTop(), plotBorderBottom(), plotBorderLeft(), plotBorderRight() */
        void setBorder(int left, int right, int top, int bottom);

        /** \brief sets minimum and maximum x-value to plot */
        void setX(double xminn, double xmaxx);

        /** \brief sets minimum and maximum y-value to plot */
        void setY(double yminn, double ymaxx);

        /** \brief sets minimum and maximum x- and y-values to plot */
        void setXY(double xminn, double xmaxx, double yminn, double ymaxx);

        /** \brief returns the current x-axis min */
        inline double getXMin() const {return xAxis->getMin(); };

        /** \brief returns the current x-axis max */
        inline double getXMax() const {return xAxis->getMax(); };

        /** \brief returns the current y-axis min */
        inline double getYMin() const {return yAxis->getMin(); };

        /** \brief returns the current y-axis max */
        inline double getYMax() const {return yAxis->getMax(); };



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
        inline double getAbsoluteXMin() const {return xAxis->getAbsoluteMin(); };

        /** \brief returns the absolute x-axis max */
        inline double getAbsoluteXMax() const {return xAxis->getAbsoluteMax(); };

        /** \brief returns the absolute y-axis min */
        inline double getAbsoluteYMin() const {return yAxis->getAbsoluteMin(); };

        /** \brief returns the absolute y-axis max */
        inline double getAbsoluteYMax() const {return yAxis->getAbsoluteMax(); };


        /** \brief add a new plotter for grid printing mode */
        inline void addGridPrintingPlotter(size_t x, size_t y, JKQtBasePlotter* plotter) {
            JKQTPgridPrintingItem i;
            i.x=x;
            i.y=y;
            i.plotter=plotter;
            gridPrintingList.push_back(i);
        };

        /** \brief clear all additional plotters for grid printing mode */
        inline void clearGridPrintingPlotters() {
            gridPrintingList.clear();
        };

        /** \brief return x-pixel coordinate from time coordinate */
        inline double x2p(double x) const {
            return xAxis->x2p(x);
        };

        /** \brief return y-pixel coordinate from y coordinate */
        inline double y2p(double y) const {
            return yAxis->x2p(y);
        };

        /** \brief return time coordinate coordinate from x-pixel */
        inline double p2x(long x) const {
            return xAxis->p2x(x);
        };

        /** \brief return y coordinate coordinate from y-pixel */
        inline double p2y(long y) const {
            return yAxis->p2x(y);
        };

        /** \brief gets the next unused style id, i.e. the smalles number >=0 which is not contained in usedStyles */
        int getNextStyle();


        struct JKQTPPen {
            QColor m_color;
            double m_width;
            Qt::PenStyle m_style;
            void setWidthF(double w) { m_width=w; }
            void setWidth(double w) { m_width=w; }
            double width() const { return m_width; }
            double widthF() const { return m_width; }
            QColor color() const { return m_color; }
            void setColor(QColor& col) {m_color=col; }
            Qt::PenStyle style() const { return m_style; }
            void setStyle(Qt::PenStyle s) { m_style=s; }
        };


        /** \brief returns a QPen object for the i-th plot style */
        JKQTPPen getPlotStyle(int i);

        /*! \brief draw the contained graph (including grid prints) into the given JKQTPEnhancedPainter
            \param painter JKQTPEnhancedPainter to which the plot should be drawn
            \param rect rectangle to plot into
         */
        void draw(JKQTPEnhancedPainter& painter, QRect rect, bool drawOverlays=true);

        /*! \brief draw the contained graph (including grid prints) into the given JKQTPEnhancedPainter
            \param painter JKQTPEnhancedPainter to which the plot should be drawn
            \param rect rectangle to plot into
         */
        void drawOverlays(JKQTPEnhancedPainter& painter, QRect rect);

        /*! \brief draw the contained graph (including grid prints) into the given JKQTPEnhancedPainter
            \param painter JKQTPEnhancedPainter to which the plot should be drawn
            \param pos where to plot the painter (left-top corner)
         */
        void draw(JKQTPEnhancedPainter& painter, QPoint pos=QPoint(0,0), bool drawOverlays=true);

        /*! \brief draw the contained graph (including grid prints) into the given JKQTPEnhancedPainter
            \param painter JKQTPEnhancedPainter to which the plot should be drawn
            \param rect rectangle to plot into
         */
        void drawNonGrid(JKQTPEnhancedPainter& painter, QRect rect, bool drawOverlays=true);

        /*! \brief draw the contained graph (including grid prints) into the given JKQTPEnhancedPainter
            \param painter JKQTPEnhancedPainter to which the plot should be drawn
            \param pos where to plot the painter (left-top corner)
         */
        void drawNonGrid(JKQTPEnhancedPainter& painter, QPoint pos=QPoint(0,0), bool drawOverlays=true);
        void drawNonGridOverlays(JKQTPEnhancedPainter &painter, QPoint pos=QPoint(0,0));

        /** \brief emit plotUpdated() */
        void update_plot() { if (emitPlotSignals) emit plotUpdated(); }

        JKQTPGET_SET_MACRO(bool, emitPlotSignals)

        JKQTPGET_MACRO(int, plotBorderTop)
        JKQTPGET_MACRO(int, plotBorderLeft)
        JKQTPGET_MACRO(int, plotBorderBottom)
        JKQTPGET_MACRO(int, plotBorderRight)

        JKQTPGET_MACRO(bool, maintainAspectRatio)
        JKQTPGET_SET_MACRO_I(double, aspectRatio, update_plot())

        JKQTPGET_MACRO(bool, maintainAxisAspectRatio)
        JKQTPGET_SET_MACRO_I(double, axisAspectRatio, update_plot())

        JKQTPGET_SET_MACRO_I(bool, useAntiAliasingForSystem, update_plot())
        JKQTPGET_SET_MACRO_I(bool, useAntiAliasingForGraphs, update_plot())

        JKQTPGET_SET_MACRO_I(QColor, graphColor, update_plot())
        JKQTPGET_SET_MACRO_I(double, graphWidth, update_plot())

        JKQTPGET_SET_MACRO_I(QColor, backgroundColor, update_plot())
        JKQTPGET_SET_MACRO_I(QColor, exportBackgroundColor, update_plot())
        JKQTPGET_SET_MACRO_I(QColor, plotBackgroundColor, update_plot())

        JKQTPGET_SET_MACRO_I(QString, keyFont, update_plot())
        JKQTPGET_SET_MACRO_I(double, keyFontSize, update_plot())
        JKQTPGET_SET_MACRO_I(double, key_item_width, update_plot())
        JKQTPGET_SET_MACRO_I(double, key_item_height, update_plot())
        JKQTPGET_SET_MACRO_I(double, keyYSeparation, update_plot())
        JKQTPGET_SET_MACRO_I(double, key_line_length, update_plot())
        JKQTPGET_SET_MACRO_I(double, keyXMargin, update_plot())
        JKQTPGET_SET_MACRO_I(double, keyYMargin, update_plot())
        JKQTPGET_SET_MACRO_I(double, keyXSeparation, update_plot())

        JKQTPGET_SET_MACRO_I(double, keyXOffset, update_plot())
        JKQTPGET_SET_MACRO_I(double, keyYOffset, update_plot())
        JKQTPGET_SET_MACRO_I(bool, showKey, update_plot())
        JKQTPGET_SET_MACRO_I(bool, showKeyFrame, update_plot())
        JKQTPGET_SET_MACRO_I(QColor, keyFrameColor, update_plot())
        JKQTPGET_SET_MACRO_I(QColor, keyBackgroundColor, update_plot())
        JKQTPGET_SET_MACRO_I(double, keyFrameWidth, update_plot())
        JKQTPGET_SET_MACRO_I(bool, keyAutosize, update_plot())
        JKQTPGET_SET_MACRO_I(JKQTPkeyPosition, keyPosition, update_plot())
        JKQTPGET_SET_MACRO_I(JKQTPkeyLayout, keyLayout, update_plot())

        JKQTPGET_SET_MACRO_I(double, plotLabelFontSize, update_plot())
        JKQTPGET_SET_MACRO_I(QString, plotLabelFontname, update_plot())
        JKQTPGET_SET_MACRO_I(QString, plotLabel, update_plot())

        JKQTPGET_SET_MACRO(bool, gridPrinting)
        JKQTPGET_SET_MACRO(size_t, gridPrintingCurrentX)
        JKQTPGET_SET_MACRO(size_t, gridPrintingCurrentY)
        JKQTPGET_SET_MACRO(QString, currentSaveDirectory)
        JKQTPGET_SET_MACRO(QString, currentFileFormat)
        JKQTPGET_SET_MACRO(QString, CSVdecimalSeparator)
        JKQTPGET_SET_MACRO(QString, CSVcommentInitializer)

        JKQTPGET_MACRO(int, iplotBorderTop)
        JKQTPGET_MACRO(int, iplotBorderLeft)
        JKQTPGET_MACRO(int, iplotBorderBottom)
        JKQTPGET_MACRO(int, iplotBorderRight)
        JKQTPGET_MACRO(int, plotWidth)
        JKQTPGET_MACRO(int, plotHeight)
        inline JKQTmathText* get_mathText() { return &mathText; };
        inline JKQTPhorizontalAxis* getXAxis() { return xAxis; };
        inline JKQTPverticalAxis* getYAxis() { return yAxis; };
        inline JKQTPhorizontalAxis* get_xAxis() { return xAxis; };
        inline JKQTPverticalAxis* get_yAxis() { return yAxis; };


        JKQTPGET_MACRO(QAction*, actSavePlot)
        JKQTPGET_MACRO(QAction*, actSaveData)
        JKQTPGET_MACRO(QAction*, actCopyData)
        JKQTPGET_MACRO(QAction*, actCopyPixelImage)
        JKQTPGET_MACRO(QAction*, actCopyMatlab)
        JKQTPGET_MACRO(QAction*, actSavePDF)
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        JKQTPGET_MACRO(QAction*, actSavePS)
#endif
        JKQTPGET_MACRO(QAction*, actSavePix)
        JKQTPGET_MACRO(QAction*, actSaveSVG)
        JKQTPGET_MACRO(QAction*, actPrint)
        JKQTPGET_MACRO(QAction*, actSaveCSV)
        JKQTPGET_MACRO(QAction*, actZoomAll)
        JKQTPGET_MACRO(QAction*, actZoomIn)
        JKQTPGET_MACRO(QAction*, actZoomOut)
        JKQTPGET_MACRO(QAction*, actShowPlotData)
        JKQTPGET_MACRO(AdditionalActionsMap, lstAdditionalPlotterActions)

        /** \brief this function registers additional actions to lstAdditionalPlotterActions, which are displayed in the context-menu */
        void registerAdditionalAction(const QString& key, QAction* act);
        void deregisterAdditionalAction(QAction* act);

        JKQTPGET_MACRO(bool, masterSynchronizeWidth)
        JKQTPGET_MACRO(bool, masterSynchronizeHeight)
        JKQTPSET_MACRO(QColor, def_backgroundColor)
        JKQTPSET_MACRO(QColor, def_plotBackgroundColor)

        JKQTPGET_SET_MACRO(double, fontSizePrintMultiplier)
        JKQTPGET_SET_MACRO(double, lineWidthPrintMultiplier)
        JKQTPGET_MACRO(double, fontSizeMultiplier)
        JKQTPGET_MACRO(double, lineWidthMultiplier)


        /** \brief returns description of i'th graph */
        JKQTPgraph* getGraph(size_t i);

        /** \brief returns the number of graphs */
        size_t getGraphCount();

        /** \brief remove the i-th graph */
        void deleteGraph(size_t i, bool deletegraph=true);

        /** \brief remove the given graph, if it is contained */
        void deleteGraph(JKQTPgraph* gr, bool deletegraph=true);

        /** \brief remove all plots
         *
         *  \param deleteGraphs if set \c true (default) the graph objects will also be deleted
         */
        void clearGraphs(bool deleteGraphs=true);

        void setAllGraphsInvisible();
        void setAllGraphsVisible();
        void setGraphVisible(int i);

        /** \brief add a new graph, returns it's position in the graphs list, if the graph is already in the plot, this returns the index in the list */
        size_t addGraph(JKQTPgraph* gr);

        /** \brief returns \c true, if the given graph is in this plot */
        bool containsGraph(JKQTPgraph* gr) const;

        /** \brief move the given graph to the top, or add it, if it is not yet contained */
        size_t moveGraphTop(JKQTPgraph* gr);
        /** \brief move the given graph to the top, or add it, if it is not yet contained */
        size_t moveGraphBottom(JKQTPgraph* gr);

        /** \brief add a new graphs from a QVector */
        void addGraphs(const QList<JKQTPgraph*>& gr);

        /** \brief add a new graph, returns it's position in the graphs list.
         *
         * \param xColumn      the column which contains the x-values of the datapoints
         * \param yColumn      the column which contains the y-values of the datapoints
         * \param title        a title for this specific graph which can be displayed in the key
         * \param graphStyle   the way how to plot the graph
         *
         * Both point to columns in the datastore explained in the JKQtPlotterBase class. The plotWidth, color, pen style ...
         * will be extracted from the automatic plot style creation mechanism implemented in JKQtPlotterBase::getNextStyle().
         * If you want to change them either use another overloaded version of addGraph(), or use getGraph() and setGraph():
         * \code
         * size_t i=addGraph(0,1,"graph1");
         * JKQTPgraph gr=getGraph(i);
         * gr.color=QColor("red");
         * setGraph(i, gr);
         * \endcode
         */
        size_t addGraph(size_t xColumn, size_t yColumn, QString title, JKQTPgraphPlotstyle graphStyle=JKQTPlines);

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
         * Both point to columns in the datastore explained in the JKQtPlotterBase class.
         * The symbolSize is set to 10 and no error information is expected.
         */
        size_t addGraph(size_t xColumn, size_t yColumn, QString title, JKQTPgraphPlotstyle graphStyle, QColor color, JKQTPgraphSymbols symbol=JKQTPcross, Qt::PenStyle penstyle=Qt::SolidLine, double width=2);

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
         * Both point to columns in the datastore explained in the JKQtPlotterBase class.
         * The symbolSize is set to 10 and no error information is expected.
         */
        size_t addGraphWithXError(size_t xColumn, size_t yColumn, size_t xErrorColumn, QString title, JKQTPgraphPlotstyle graphStyle=JKQTPpoints, JKQTPerrorPlotstyle errorStyle=JKQTPerrorBars);

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
         * Both point to columns in the datastore explained in the JKQtPlotterBase class.
         * The symbolSize is set to 10 and no error information is expected.
         */
        size_t addGraphWithYError(size_t xColumn, size_t yColumn, size_t yErrorColumn, QString title, JKQTPgraphPlotstyle graphStyle=JKQTPpoints, JKQTPerrorPlotstyle errorStyle=JKQTPerrorBars);

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
         * Both point to columns in the datastore explained in the JKQtPlotterBase class.
         * The symbolSize is set to 10 and no error information is expected. The errorStyle is set to JKQTPerrorBars
         * for both directions.
         */
        size_t addGraphWithXYError(size_t xColumn, size_t yColumn, size_t xErrorColumn, size_t yErrorColumn, QString title, JKQTPgraphPlotstyle graphStyle=JKQTPpoints);

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
        */
        void synchronizeToMaster(JKQtBasePlotter* master, bool synchronizeWidth, bool synchronizeHeight);

        /** \brief switches any synchronization off, that has been created by synchronizeToMaster() */
        void resetMasterSynchronization();







        /** \brief returns description of i'th overlay element */
        JKQTPoverlayElement* getOverlayElement(size_t i);

        /** \brief returns the number of overlay elements */
        size_t getOverlayElementCount();

        /** \brief remove the i-th overlay element */
        void deleteOverlayElement(size_t i, bool deletegraph=true);

        /** \brief remove the given overlay element, if it is contained */
        void deleteOverlayElement(JKQTPoverlayElement* gr, bool deletegraph=true);

        /** \brief remove all overlay elements
         *
         *  \param deleteGraphs if set \c true (default) the overlay element objects will also be deleted
         */
        void clearOverlayElement(bool deleteGraphs=true);

        /** \brief add a new overlay element, returns it's position in the overlay elements list, if the overlay element is already in the plot, this returns the index in the list */
        size_t addOverlayElement(JKQTPoverlayElement* gr);

        /** \brief returns \c true, if the given overlay element is in this plot */
        bool containsOverlayElement(JKQTPoverlayElement* gr) const;

        /** \brief move the given overlay element to the top, or add it, if it is not yet contained */
        size_t moveOverlayElementTop(JKQTPoverlayElement* gr);

        /** \brief add a new overlay elements from a QList */
        void addOverlayElements(const QList<JKQTPoverlayElement*>& gr);

        QVector<QLineF> getBoundingLinesX1Y1(QRectF *rect=NULL) const;

    signals:
        /** \brief signal: emitted whenever the user selects a new x-y zoom range (by mouse) */
        void zoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQtBasePlotter* sender);

        /** \brief emitted when the plot has to be updated */
        void plotUpdated();

        /** \brief emitted when the overlay elements have to be updated */
        void overlaysUpdated();

        /** \brief emitted when the plot scaling had to be recalculated */
        void plotScalingRecalculated();

        /** \brief emitted before the plot scaling has been recalculated */
        void beforePlotScalingRecalculate();

    public:

        /** \brief set a global preset/default value for the userSettigsFilename and userSettigsPrefix properties of JKQtBasePlotter
         * \ingroup jkqtplotter
         *
         *  These presets are application global and will be used ONLY on initialization of a JKQtBasePlotter. You can overwrite them
         *  on a local-basis for each JKQtBasePrinter separately. The changed values from this function call will only take effect for
         *  newly created plotters AFTER the function call!
         */
        static void setDefaultJKQtBasePrinterUserSettings(QString userSettigsFilename, QString userSettigsPrefix);

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
                /** \brief create a paint device with a given size in pixels */
                virtual QPaintDevice* createPaintdevice(const QString& filename, int widthPix, int heightPix) const=0;
                /** \brief create a paint device with a given size in millimeters ... the default implementation call createPaintdevice(), assuming the standard logical resolution of the desktop!!!) */
                virtual QPaintDevice* createPaintdeviceMM(const QString& filename, double widthMM, double heightMM) const;
        };

        static void registerPaintDeviceAdapter(JKQTPPaintDeviceAdapter* adapter);
        static void deregisterPaintDeviceAdapter(JKQTPPaintDeviceAdapter* adapter);


        class LIB_EXPORT JKQTPSaveDataAdapter {
            public:
                virtual ~JKQTPSaveDataAdapter()  {}
                virtual QString getFilter() const=0;
                virtual void saveJKQTPData(const QString& filename, const QList<QVector<double> >& data, const QStringList& columnNames) const=0;
        };

        static bool registerSaveDataAdapter(JKQTPSaveDataAdapter* adapter);
        static bool deregisterSaveDataAdapter(JKQTPSaveDataAdapter* adapter);


        struct LIB_EXPORT textSizeData {
            explicit textSizeData();
            double ascent, descent, width, strikeoutPos;
        };

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
        static QHash<JKQtBasePlotter::textSizeKey, JKQtBasePlotter::textSizeData> tbrh;

};

inline uint qHash(const JKQtBasePlotter::textSizeKey& data) {
    return qHash(data.f.family())+qHash(data.text);
}

#endif // JKQTPBASEPLOTTER_H
