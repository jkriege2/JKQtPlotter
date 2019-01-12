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
  * \ingroup jkqtpbaseplotter
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


class JKQTPgraphsModel; // forward
class JKQTPgraph; // forward
class JKQTPplotElement; // forward

/** \brief initialized Qt-ressources necessary for JKQtBasePlotter */
LIB_EXPORT void initJKQtBasePlotterResources();


/** \brief base class for 2D plotter classes
 * \ingroup jkqtpbaseplotter
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
 * \section jkqtplotter_base_systems_baseplotter Coordinate Systems and Transformations
 * These topics are discussed in the help for JKQTPcoordinateAxis. There is a set of coordinate transform
 * methods (x2p(), y2p(), p2x(), p2y() which only call the respective methods in xAxis and yAxis objects.
 *
 *
 * \section jkqtplotter_base_grids_baseplotter Axis Ticks and Grids
 *    - The plotting of coordinate axes and grids, as well as coordinate transforms is done by
 *      JKQTPcoordinateAxis descendents (see documentation there)
 *  .
 *  If you want to set the axis properties, use get_xAxis() or get_yAxis() to get a pointer to the axis objects which then
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

        /** \brief model representing all Plots in this plotter and showing their visible/invisible state */
        JKQTPgraphsModel* m_plotsModel;

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
        QString CSVdecimalSeparator;
        /*! \brief default value for property property varname. \see CSVdecimalSeparator for more information */
        QString def_CSVdecimalSeparator;
        /** \brief this string is used to introduce comments in text output when exporting data */
        QString CSVcommentInitializer;
        /*! \brief default value for property property varname. \see CSVcommentInitializer for more information */
        QString def_CSVcommentInitializer;

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
        int plotBorderTop;
        /*! \brief default value for property property varname. \see plotBorderTop for more information */
        int def_plotBorderTop;
        /** \brief free space between widget top border and plot top border, as used to plot the graph (mnay be altered from user input ) */
        int iplotBorderTop;
        int iplotKeyBorderTop;
        /** \brief height of the plot title (or 0 if no title) */
        int iTitleHeight;
        /** \brief free space between widget left border and plot left border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to iplotBorderLeft  */
        int plotBorderLeft;
        /*! \brief default value for property property varname. \see plotBorderLeft for more information */
        int def_plotBorderLeft;
        /** \brief free space between widget top border and plot top border, as used to plot the graph (mnay be altered from user input ) */
        int iplotBorderLeft;
        int iplotKeyBorderLeft;
        /** \brief free space between widget bottom border and plot bottom border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to iplotBorderBottom  */
        int plotBorderBottom;
        /*! \brief default value for property property varname. \see plotBorderBottom for more information */
        int def_plotBorderBottom;
        /** \brief free space between widget top border and plot top border, as used to plot the graph (mnay be altered from user input ) */
        int iplotBorderBottom;
        int iplotKeyBorderBottom;
        /** \brief free space between widget right border and plot right border, this property may be set by the user and is possibly altered
         *         by the key positioning algorithm. The altered value is written to iplotBorderRight  */
        int plotBorderRight;
        /*! \brief default value for property property varname. \see plotBorderRight for more information */
        int def_plotBorderRight;
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
        bool maintainAspectRatio;
        /*! \brief default value for property property varname. \see maintainAspectRatio for more information */
        bool def_maintainAspectRatio;
        /** \brief the aspect ratio of plotwidth and plotheight to maintain, if \c maintainAspectRatio==true */
        double aspectRatio;
        /*! \brief default value for property property varname. \see aspectRatio for more information */
        double def_aspectRatio;

        /** \brief indicates whether the axes should maintain an aspect ratio */
        bool maintainAxisAspectRatio;
        /*! \brief default value for property property varname. \see maintainAxisAspectRatio for more information */
        bool def_maintainAxisAspectRatio;
        /** \brief the aspect ratio of axis widths to maintain, if \c maintainAxisAspectRatio==true */
        double axisAspectRatio;
        /*! \brief default value for property property varname. \see axisAspectRatio for more information */
        double def_axisAspectRatio;

        /** \brief plot width in pixels inside the widget (calculated by calcPlotScaling() from plotBorderLeft, plotBorderRight and widgetWidth) */
        int plotWidth;
        /** \brief plot height in pixels inside the widget (calculated by calcPlotScaling() from plotBorderTop, plotBorderBottom and widgetHeight) */
        int plotHeight;



        /** \brief color of the plotted graph */
        QColor graphColor;
        /*! \brief default value for property property varname. \see graphColor for more information */
        QColor def_graphColor;
        /** \brief colors used in modes where more than one graph is beeing plottet */
        QColor manyGraphsColor[50];
        QColor def_manyGraphsColor[50];
        /** \brief number of actually defined items in manyGraphsColor */
        int manyGraphsColorCount;
        /** \brief pen styles used in modes where more than one graph is beeing plottet */
        Qt::PenStyle manyGraphsStyle[5];
        /*! \brief default value for property property varname. \see manyGraphsStyle[5] for more information */
        Qt::PenStyle def_manyGraphsStyle[5];
        /** \brief width of the plotted graph (in pixel) */
        double graphWidth;
        /*! \brief default value for property property varname. \see graphWidth for more information */
        double def_graphWidth;



        /** \brief indicates whether to use clipping (hack for printing, see print() ) */
        bool useClipping;



        /** \brief color of the coordinate system */
        QColor systemColor;
        /*! \brief default value for property property varname. \see systemColor for more information */
        QColor def_systemColor;
        /** \brief width of the coordinate (in pixel) */
        double systemWidth;
        /*! \brief default value for property property varname. \see systemWidth for more information */
        double def_systemWidth;
        /** \brief color of the background*/
        QColor backgroundColor;
        /*! \brief default value for property property varname. \see backgroundColor for more information */
        QColor def_backgroundColor;
        /** \brief color of the background when exporting*/
        QColor exportBackgroundColor;
        /*! \brief default value for property property varname. \see exportBackgroundColor for more information */
        QColor def_exportBackgroundColor;
        /** \brief color of the plot's background*/
        QColor plotBackgroundColor;
        /*! \brief default value for property property varname. \see plotBackgroundColor for more information */
        QColor def_plotBackgroundColor;


        /** \brief indicates whether to plot a frame around the key */
        bool showKeyFrame;
        /*! \brief default value for property property varname. \see showKeyFrame for more information */
        bool def_showKeyFrame;
        /** \brief color of the key frame line */
        QColor keyFrameColor;
        /*! \brief default value for property property varname. \see keyFrameColor for more information */
        QColor def_keyFrameColor;
        /** \brief width of the key frame line */
        double keyFrameWidth;
        /*! \brief default value for property property varname. \see keyFrameWidth for more information */
        double def_keyFrameWidth;
        /** \brief color of the key background */
        QColor keyBackgroundColor;
        /*! \brief default value for property property varname. \see keyBackgroundColor for more information */
        QColor def_keyBackgroundColor;
        /** \brief indicates whether to plot a key */
        bool showKey;
        /*! \brief default value for property property varname. \see showKey for more information */
        bool def_showKey;
        /** \brief font face for key labels */
        QString keyFont;
        /*! \brief default value for property property varname. \see keyFont for more information */
        QString def_keyFont;
        /** \brief font size for key labels [in points] */
        double keyFontSize;
        /*! \brief default value for property property varname. \see keyFontSize for more information */
        double def_keyFontSize;
        /** \brief width of a key item in pixels [in units of width of 'X' set in keyFont, keyFontSize] */
        double key_item_width;
        /*! \brief default value for property property varname. \see key_item_width for more information */
        double def_key_item_width;
        /** \brief height of a key item in pixels [in units of height  keyFont, keyFontSize] */
        double key_item_height;
        /*! \brief default value for property property varname. \see key_item_height for more information */
        double def_key_item_height;
        /** \brief length of the line samples in the key in pixels [in units of width of 'X' set in keyFont, keyFontSize] */
        double key_line_length;
        /*! \brief default value for property property varname. \see key_line_length for more information */
        double def_key_line_length;
        /** \brief x-distance between key frame and key content [in units of width of 'X' set in keyFont, keyFontSize] */
        double keyXMargin;
        /*! \brief default value for property property varname. \see keyXMargin for more information */
        double def_keyXMargin;
        /** \brief y-distance between key frame and key content [in units of width of 'x' set in keyFont, keyFontSize] */
        double keyYMargin;
        /*! \brief default value for property property varname. \see keyYMargin for more information */
        double def_keyYMargin;
        /** \brief x-offset of the key from the border of the plot [in units of width of 'X' set in keyFont, keyFontSize] */
        double keyXOffset;
        /*! \brief default value for property property varname. \see keyXOffset for more information */
        double def_keyXOffset;
        /** \brief y-offset of the key from the border of the plot [in units of width of 'x' set in keyFont, keyFontSize] */
        double keyYOffset;
        /*! \brief default value for property property varname. \see keyYOffset for more information */
        double def_keyYOffset;
        /** \brief distance between key line example and key text [in units of width of 'X' set in keyFont, keyFontSize] */
        double keyXSeparation;
        /*! \brief default value for property property varname. \see keyXSeparation for more information */
        double def_keyXSeparation;
        /** \brief distance between two key entries [in units of height of keyFont, keyFontSize] */
        double keyYSeparation;
        /*! \brief default value for property property varname. \see keyYSeparation for more information */
        double def_keyYSeparation;
        /** \brief key position */
        JKQTPkeyPosition keyPosition;
        /*! \brief default value for property property varname. \see keyPosition for more information */
        JKQTPkeyPosition def_keyPosition;
        /** \brief the key layout */
        JKQTPkeyLayout keyLayout;
        /*! \brief default value for property property varname. \see keyLayout for more information */
        JKQTPkeyLayout def_keyLayout;
        /** \brief determine width of the key automatically */
        bool keyAutosize;
        /*! \brief default value for property property varname. \see keyAutosize for more information */
        bool def_keyAutosize;

        /** \brief the plot label text */
        QString plotLabel;

        /** \brief the plot label font name */
        QString plotLabelFontname;
        /*! \brief default value for property property varname. \see plotLabelFontname for more information */
        QString def_plotLabelFontname;

        /** \brief the plot label font size */
        double plotLabelFontSize;
        /*! \brief default value for property property varname. \see plotLabelFontSize for more information */
        double def_plotLabelFontSize;

        /** \brief calculate the scaling and offset values from axis min/max values */
        void calcPlotScaling(JKQTPEnhancedPainter& painter);

        /** \brief set the standard settings (colors, line widths ...) */
        void initSettings();

        /** \brief specifies whether this class emits signals, like zoomChangedLocally() */
        bool emitSignals;

        /** \brief specifies whether to use antialiasing for plotting the coordinate system */
        bool useAntiAliasingForSystem;
        /*! \brief default value for property property varname. \see useAntiAliasingForSystem for more information */
        bool def_useAntiAliasingForSystem;

        /** \brief specifies whether to use antialiasing for plotting the graphs
         *
         * \note You can set this property \c false to increase plotting speed of complex plots (with many graphs inside). You can reach a
         *       roughly three-fold speed improvement!
*/
        bool useAntiAliasingForGraphs;
        /*! \brief default value for property property varname. \see useAntiAliasingForGraphs for more information */
        bool def_useAntiAliasingForGraphs;

        /** \brief specifies whether to use antialiasing when drawing any text
        * \note You can set this property \c false to increase plotting speed of complex plots (with many graphs inside). You can reach a
        *       roughly three-fold speed improvement!
*/
        bool useAntiAliasingForText;
        /*! \brief default value for property property varname. \see useAntiAliasingForText for more information */
        bool def_useAntiAliasingForText;

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
        QList<JKQTPplotElement*> graphs;


        QList<JKQTPoverlayElement*> overlays;


        /** \brief used as maximum/minimum pixel value for plotting */
        double plot_minmaxcoorinate;




        /** \brief hidden default constructor */
        JKQtBasePlotter();



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
        virtual void getKeyExtent(JKQTPEnhancedPainter& painter, double *width, double *height, double *text_width=nullptr, double *text_height=nullptr, int *columns_count=nullptr, int* lines_count=nullptr);
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

        /*! \brief sets the property fontSizeMultiplier to the specified \a __value. 
            \details Description of the parameter fontSizeMultiplier is:  <BLOCKQUOTE>\copybrief fontSizeMultiplier </BLOCKQUOTE> 
        	\see fontSizeMultiplier for more information */ 
        inline virtual void set_fontSizeMultiplier(double __value)
        { 
        	this->fontSizeMultiplier = __value; 
        }
        /*! \brief sets the property lineWidthMultiplier to the specified \a __value. 
            \details Description of the parameter lineWidthMultiplier is:  <BLOCKQUOTE>\copybrief lineWidthMultiplier </BLOCKQUOTE> 
        	\see lineWidthMultiplier for more information */ 
        inline virtual void set_lineWidthMultiplier(double __value)
        { 
        	this->lineWidthMultiplier = __value; 
        }
        /*! \brief sets the property printMagnification to the specified \a __value. 
            \details Description of the parameter printMagnification is: <BLOCKQUOTE>\copybrief printMagnification </BLOCKQUOTE> 
            \see printMagnification for more information */ 
        inline virtual void set_printMagnification(double __value)
        {
            this->printMagnification = __value;
        } 
        /*! \brief returns the property printMagnification. 
            \details Description of the parameter printMagnification is: <BLOCKQUOTE>\copybrief printMagnification </BLOCKQUOTE> 
            \see printMagnification for more information */ 
        inline virtual double get_printMagnification() const  
        {
            return this->printMagnification; 
        }
        /*! \brief sets the property paintMagnification to the specified \a __value. 
            \details Description of the parameter paintMagnification is: <BLOCKQUOTE>\copybrief paintMagnification </BLOCKQUOTE> 
            \see paintMagnification for more information */ 
        inline virtual void set_paintMagnification(double __value)
        {
            this->paintMagnification = __value;
        } 
        /*! \brief returns the property paintMagnification. 
            \details Description of the parameter paintMagnification is: <BLOCKQUOTE>\copybrief paintMagnification </BLOCKQUOTE> 
            \see paintMagnification for more information */ 
        inline virtual double get_paintMagnification() const  
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
        void synchronizeXAxis(double newxmin, double newxmax, double newymin, double newymax, JKQtBasePlotter* sender);
        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local y-axis to the other y-axis */
        void synchronizeYAxis(double newxmin, double newxmax, double newymin, double newymax, JKQtBasePlotter* sender);
        /** \brief may be connected to zoomChangedLocally() of a different plot and synchronizes the local x- and y-axis to the other x- and y-axis */
        void synchronizeXYAxis(double newxmin, double newxmax, double newymin, double newymax, JKQtBasePlotter* sender);

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
        virtual void zoomToFit(bool zoomX=true, bool zoomY=true, bool includeX0=false, bool includeY0=false, double scaleX=1.05, double scaleY=1.05);

        /** \brief zooms into the graph (the same as turning the mouse wheel) by the given factor */
        void zoomIn(double factor=2.0);
        /** \brief zooms out of the graph (the same as turning the mouse wheel) by the given factor */
        void zoomOut(double factor=2.0) {
            zoomIn(1.0/factor);
        }

        /** \brief set the datarange of all current graphs to the given values */
        void setGraphsDataRange(int datarange_start, int datarange_end);

        /** \brief en-/disables the maintaining of the data aspect ratio */
        void set_maintainAspectRatio(bool value) {
            maintainAspectRatio=value;
            update_plot();
        }

        /** \brief en-/disables the maintaining of the axis aspect ratio */
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
        JKQtBasePlotter(bool datastore_internal, QObject* parent=nullptr, JKQTPdatastore* datast=nullptr);

        /** \brief class destructor */
        virtual ~JKQtBasePlotter();

        /** \brief returns a pointer to the datastore used by this object */
        inline JKQTPdatastore* getDatastore() { return datastore; }

        /** \brief returns model representing all Plots in this plotter and showing their visible/invisible state */
        inline JKQTPgraphsModel* getPlotsModel() { return m_plotsModel; }


        /** \brief returns a pointer to the datastore used by this object */
        inline const JKQTPdatastore* getDatastore() const { return datastore; }

        /** \brief returns model representing all Plots in this plotter and showing their visible/invisible state */
        inline const JKQTPgraphsModel* getPlotsModel() const { return m_plotsModel; }

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

        /*! \brief returns the property emitSignals. \details Description of the parameter emitSignals is:  <BLOCKQUOTE>\copybrief emitSignals </BLOCKQUOTE>. \see emitSignals for more information */ 
        inline bool get_emitSignals()const { return this->emitSignals; }

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
        inline int getWidth() {return widgetWidth; }

        /** \brief gets the width of the plot widget */
        inline int getHeight() {return widgetHeight; }

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
        inline void addGridPrintingPlotter(size_t x, size_t y, JKQtBasePlotter* plotter) {
            JKQTPgridPrintingItem i;
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
        inline double p2x(long x) const {
            return xAxis->p2x(x);
        }

        /** \brief return y coordinate coordinate from y-pixel */
        inline double p2y(long y) const {
            return yAxis->p2x(y);
        }

        /** \brief gets the next unused style id, i.e. the smalles number >=0 which is not contained in usedStyles */
        int getNextStyle();


        /** \brief represents a pen, when plotting in JKQtPlotter/JKQtBasePlotter */
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

        /*! \brief sets the property emitPlotSignals to the specified \a __value. 
            \details Description of the parameter emitPlotSignals is: <BLOCKQUOTE>\copybrief emitPlotSignals </BLOCKQUOTE> 
            \see emitPlotSignals for more information */ 
        inline virtual void set_emitPlotSignals(bool __value)
        {
            this->emitPlotSignals = __value;
        } 
        /*! \brief returns the property emitPlotSignals. 
            \details Description of the parameter emitPlotSignals is: <BLOCKQUOTE>\copybrief emitPlotSignals </BLOCKQUOTE> 
            \see emitPlotSignals for more information */ 
        inline virtual bool get_emitPlotSignals() const  
        {
            return this->emitPlotSignals; 
        }

        /*! \brief returns the property plotBorderTop. \details Description of the parameter plotBorderTop is:  <BLOCKQUOTE>\copybrief plotBorderTop </BLOCKQUOTE>. \see plotBorderTop for more information */ 
        inline int get_plotBorderTop() const { return this->plotBorderTop; }
        /*! \brief returns the property plotBorderLeft. \details Description of the parameter plotBorderLeft is:  <BLOCKQUOTE>\copybrief plotBorderLeft </BLOCKQUOTE>. \see plotBorderLeft for more information */ 
        inline int get_plotBorderLeft() const { return this->plotBorderLeft; }
        /*! \brief returns the property plotBorderBottom. \details Description of the parameter plotBorderBottom is:  <BLOCKQUOTE>\copybrief plotBorderBottom </BLOCKQUOTE>. \see plotBorderBottom for more information */ 
        inline int get_plotBorderBottom() const { return this->plotBorderBottom; }
        /*! \brief returns the property plotBorderRight. \details Description of the parameter plotBorderRight is:  <BLOCKQUOTE>\copybrief plotBorderRight </BLOCKQUOTE>. \see plotBorderRight for more information */ 
        inline int get_plotBorderRight() const { return this->plotBorderRight; }

        /*! \brief returns the property maintainAspectRatio. \details Description of the parameter maintainAspectRatio is:  <BLOCKQUOTE>\copybrief maintainAspectRatio </BLOCKQUOTE>. \see maintainAspectRatio for more information */ 
        inline bool get_maintainAspectRatio() const { return this->maintainAspectRatio; }
        /*! \brief sets the property aspectRatio to the specified \a __value. 
            \details Description of the parameter aspectRatio is: <BLOCKQUOTE>\copybrief aspectRatio </BLOCKQUOTE> 
            \see aspectRatio for more information */ 
        inline virtual void set_aspectRatio(double __value)
        {
            if (this->aspectRatio != __value) {
                this->aspectRatio = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property aspectRatio. 
            \details Description of the parameter aspectRatio is: <BLOCKQUOTE>\copybrief aspectRatio </BLOCKQUOTE> 
            \see aspectRatio for more information */ 
        inline virtual double get_aspectRatio() const  
        {
            return this->aspectRatio; 
        }

        /*! \brief returns the property maintainAxisAspectRatio. \details Description of the parameter maintainAxisAspectRatio is:  <BLOCKQUOTE>\copybrief maintainAxisAspectRatio </BLOCKQUOTE>. \see maintainAxisAspectRatio for more information */ 
        inline bool get_maintainAxisAspectRatio() const { return this->maintainAxisAspectRatio; }
        /*! \brief sets the property axisAspectRatio to the specified \a __value. 
            \details Description of the parameter axisAspectRatio is: <BLOCKQUOTE>\copybrief axisAspectRatio </BLOCKQUOTE> 
            \see axisAspectRatio for more information */ 
        inline virtual void set_axisAspectRatio(double __value)
        {
            if (this->axisAspectRatio != __value) {
                this->axisAspectRatio = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property axisAspectRatio. 
            \details Description of the parameter axisAspectRatio is: <BLOCKQUOTE>\copybrief axisAspectRatio </BLOCKQUOTE> 
            \see axisAspectRatio for more information */ 
        inline virtual double get_axisAspectRatio() const  
        {
            return this->axisAspectRatio; 
        }

        /*! \brief sets the property useAntiAliasingForSystem to the specified \a __value. 
            \details Description of the parameter useAntiAliasingForSystem is: <BLOCKQUOTE>\copybrief useAntiAliasingForSystem </BLOCKQUOTE> 
            \see useAntiAliasingForSystem for more information */ 
        inline virtual void set_useAntiAliasingForSystem(bool __value)
        {
            if (this->useAntiAliasingForSystem != __value) {
                this->useAntiAliasingForSystem = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property useAntiAliasingForSystem. 
            \details Description of the parameter useAntiAliasingForSystem is: <BLOCKQUOTE>\copybrief useAntiAliasingForSystem </BLOCKQUOTE> 
            \see useAntiAliasingForSystem for more information */ 
        inline virtual bool get_useAntiAliasingForSystem() const  
        {
            return this->useAntiAliasingForSystem; 
        }
        /*! \brief sets the property useAntiAliasingForGraphs to the specified \a __value. 
            \details Description of the parameter useAntiAliasingForGraphs is: <BLOCKQUOTE>\copybrief useAntiAliasingForGraphs </BLOCKQUOTE> 
            \see useAntiAliasingForGraphs for more information */ 
        inline virtual void set_useAntiAliasingForGraphs(bool __value)
        {
            if (this->useAntiAliasingForGraphs != __value) {
                this->useAntiAliasingForGraphs = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property useAntiAliasingForGraphs. 
            \details Description of the parameter useAntiAliasingForGraphs is: <BLOCKQUOTE>\copybrief useAntiAliasingForGraphs </BLOCKQUOTE> 
            \see useAntiAliasingForGraphs for more information */ 
        inline virtual bool get_useAntiAliasingForGraphs() const  
        {
            return this->useAntiAliasingForGraphs; 
        }
        /*! \brief sets the property useAntiAliasingForText to the specified \a __value. 
            \details Description of the parameter useAntiAliasingForText is: <BLOCKQUOTE>\copybrief useAntiAliasingForText </BLOCKQUOTE> 
            \see useAntiAliasingForText for more information */ 
        inline virtual void set_useAntiAliasingForText(bool __value)
        {
            if (this->useAntiAliasingForText != __value) {
                this->useAntiAliasingForText = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property useAntiAliasingForText. 
            \details Description of the parameter useAntiAliasingForText is: <BLOCKQUOTE>\copybrief useAntiAliasingForText </BLOCKQUOTE> 
            \see useAntiAliasingForText for more information */ 
        inline virtual bool get_useAntiAliasingForText() const  
        {
            return this->useAntiAliasingForText; 
        }

        /*! \brief sets the property graphColor to the specified \a __value. 
            \details Description of the parameter graphColor is: <BLOCKQUOTE>\copybrief graphColor </BLOCKQUOTE> 
            \see graphColor for more information */ 
        inline virtual void set_graphColor(const QColor & __value)  
        {
            if (this->graphColor != __value) {
                this->graphColor = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property graphColor. 
            \details Description of the parameter graphColor is: <BLOCKQUOTE>\copybrief graphColor </BLOCKQUOTE> 
            \see graphColor for more information */ 
        inline virtual QColor get_graphColor() const  
        {
            return this->graphColor; 
        }
        /*! \brief sets the property graphWidth to the specified \a __value. 
            \details Description of the parameter graphWidth is: <BLOCKQUOTE>\copybrief graphWidth </BLOCKQUOTE> 
            \see graphWidth for more information */ 
        inline virtual void set_graphWidth(double __value)
        {
            if (this->graphWidth != __value) {
                this->graphWidth = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property graphWidth. 
            \details Description of the parameter graphWidth is: <BLOCKQUOTE>\copybrief graphWidth </BLOCKQUOTE> 
            \see graphWidth for more information */ 
        inline virtual double get_graphWidth() const  
        {
            return this->graphWidth; 
        }

        /*! \brief sets the property backgroundColor to the specified \a __value. 
            \details Description of the parameter backgroundColor is: <BLOCKQUOTE>\copybrief backgroundColor </BLOCKQUOTE> 
            \see backgroundColor for more information */ 
        inline virtual void set_backgroundColor(const QColor & __value)  
        {
            if (this->backgroundColor != __value) {
                this->backgroundColor = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property backgroundColor. 
            \details Description of the parameter backgroundColor is: <BLOCKQUOTE>\copybrief backgroundColor </BLOCKQUOTE> 
            \see backgroundColor for more information */ 
        inline virtual QColor get_backgroundColor() const  
        {
            return this->backgroundColor; 
        }
        /*! \brief sets the property exportBackgroundColor to the specified \a __value. 
            \details Description of the parameter exportBackgroundColor is: <BLOCKQUOTE>\copybrief exportBackgroundColor </BLOCKQUOTE> 
            \see exportBackgroundColor for more information */ 
        inline virtual void set_exportBackgroundColor(const QColor & __value)  
        {
            if (this->exportBackgroundColor != __value) {
                this->exportBackgroundColor = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property exportBackgroundColor. 
            \details Description of the parameter exportBackgroundColor is: <BLOCKQUOTE>\copybrief exportBackgroundColor </BLOCKQUOTE> 
            \see exportBackgroundColor for more information */ 
        inline virtual QColor get_exportBackgroundColor() const  
        {
            return this->exportBackgroundColor; 
        }
        /*! \brief sets the property plotBackgroundColor to the specified \a __value. 
            \details Description of the parameter plotBackgroundColor is: <BLOCKQUOTE>\copybrief plotBackgroundColor </BLOCKQUOTE> 
            \see plotBackgroundColor for more information */ 
        inline virtual void set_plotBackgroundColor(const QColor & __value)  
        {
            if (this->plotBackgroundColor != __value) {
                this->plotBackgroundColor = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property plotBackgroundColor. 
            \details Description of the parameter plotBackgroundColor is: <BLOCKQUOTE>\copybrief plotBackgroundColor </BLOCKQUOTE> 
            \see plotBackgroundColor for more information */ 
        inline virtual QColor get_plotBackgroundColor() const  
        {
            return this->plotBackgroundColor; 
        }

        /*! \brief sets the property keyFont to the specified \a __value. 
            \details Description of the parameter keyFont is: <BLOCKQUOTE>\copybrief keyFont </BLOCKQUOTE> 
            \see keyFont for more information */ 
        inline virtual void set_keyFont(const QString & __value)  
        {
            if (this->keyFont != __value) {
                this->keyFont = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property keyFont. 
            \details Description of the parameter keyFont is: <BLOCKQUOTE>\copybrief keyFont </BLOCKQUOTE> 
            \see keyFont for more information */ 
        inline virtual QString get_keyFont() const  
        {
            return this->keyFont; 
        }
        /*! \brief sets the property keyFontSize to the specified \a __value. 
            \details Description of the parameter keyFontSize is: <BLOCKQUOTE>\copybrief keyFontSize </BLOCKQUOTE> 
            \see keyFontSize for more information */ 
        inline virtual void set_keyFontSize(double __value)
        {
            if (this->keyFontSize != __value) {
                this->keyFontSize = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property keyFontSize. 
            \details Description of the parameter keyFontSize is: <BLOCKQUOTE>\copybrief keyFontSize </BLOCKQUOTE> 
            \see keyFontSize for more information */ 
        inline virtual double get_keyFontSize() const  
        {
            return this->keyFontSize; 
        }
        /*! \brief sets the property key_item_width to the specified \a __value. 
            \details Description of the parameter key_item_width is: <BLOCKQUOTE>\copybrief key_item_width </BLOCKQUOTE> 
            \see key_item_width for more information */ 
        inline virtual void set_key_item_width(double __value)
        {
            if (this->key_item_width != __value) {
                this->key_item_width = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property key_item_width. 
            \details Description of the parameter key_item_width is: <BLOCKQUOTE>\copybrief key_item_width </BLOCKQUOTE> 
            \see key_item_width for more information */ 
        inline virtual double get_key_item_width() const  
        {
            return this->key_item_width; 
        }
        /*! \brief sets the property key_item_height to the specified \a __value. 
            \details Description of the parameter key_item_height is: <BLOCKQUOTE>\copybrief key_item_height </BLOCKQUOTE> 
            \see key_item_height for more information */ 
        inline virtual void set_key_item_height(double __value)
        {
            if (this->key_item_height != __value) {
                this->key_item_height = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property key_item_height. 
            \details Description of the parameter key_item_height is: <BLOCKQUOTE>\copybrief key_item_height </BLOCKQUOTE> 
            \see key_item_height for more information */ 
        inline virtual double get_key_item_height() const  
        {
            return this->key_item_height; 
        }
        /*! \brief sets the property keyYSeparation to the specified \a __value. 
            \details Description of the parameter keyYSeparation is: <BLOCKQUOTE>\copybrief keyYSeparation </BLOCKQUOTE> 
            \see keyYSeparation for more information */ 
        inline virtual void set_keyYSeparation(double __value)
        {
            if (this->keyYSeparation != __value) {
                this->keyYSeparation = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property keyYSeparation. 
            \details Description of the parameter keyYSeparation is: <BLOCKQUOTE>\copybrief keyYSeparation </BLOCKQUOTE> 
            \see keyYSeparation for more information */ 
        inline virtual double get_keyYSeparation() const  
        {
            return this->keyYSeparation; 
        }
        /*! \brief sets the property key_line_length to the specified \a __value. 
            \details Description of the parameter key_line_length is: <BLOCKQUOTE>\copybrief key_line_length </BLOCKQUOTE> 
            \see key_line_length for more information */ 
        inline virtual void set_key_line_length(double __value)
        {
            if (this->key_line_length != __value) {
                this->key_line_length = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property key_line_length. 
            \details Description of the parameter key_line_length is: <BLOCKQUOTE>\copybrief key_line_length </BLOCKQUOTE> 
            \see key_line_length for more information */ 
        inline virtual double get_key_line_length() const  
        {
            return this->key_line_length; 
        }
        /*! \brief sets the property keyXMargin to the specified \a __value. 
            \details Description of the parameter keyXMargin is: <BLOCKQUOTE>\copybrief keyXMargin </BLOCKQUOTE> 
            \see keyXMargin for more information */ 
        inline virtual void set_keyXMargin(double __value)
        {
            if (this->keyXMargin != __value) {
                this->keyXMargin = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property keyXMargin. 
            \details Description of the parameter keyXMargin is: <BLOCKQUOTE>\copybrief keyXMargin </BLOCKQUOTE> 
            \see keyXMargin for more information */ 
        inline virtual double get_keyXMargin() const  
        {
            return this->keyXMargin; 
        }
        /*! \brief sets the property keyYMargin to the specified \a __value. 
            \details Description of the parameter keyYMargin is: <BLOCKQUOTE>\copybrief keyYMargin </BLOCKQUOTE> 
            \see keyYMargin for more information */ 
        inline virtual void set_keyYMargin(double __value)
        {
            if (this->keyYMargin != __value) {
                this->keyYMargin = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property keyYMargin. 
            \details Description of the parameter keyYMargin is: <BLOCKQUOTE>\copybrief keyYMargin </BLOCKQUOTE> 
            \see keyYMargin for more information */ 
        inline virtual double get_keyYMargin() const  
        {
            return this->keyYMargin; 
        }
        /*! \brief sets the property keyXSeparation to the specified \a __value. 
            \details Description of the parameter keyXSeparation is: <BLOCKQUOTE>\copybrief keyXSeparation </BLOCKQUOTE> 
            \see keyXSeparation for more information */ 
        inline virtual void set_keyXSeparation(double __value)
        {
            if (this->keyXSeparation != __value) {
                this->keyXSeparation = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property keyXSeparation. 
            \details Description of the parameter keyXSeparation is: <BLOCKQUOTE>\copybrief keyXSeparation </BLOCKQUOTE> 
            \see keyXSeparation for more information */ 
        inline virtual double get_keyXSeparation() const  
        {
            return this->keyXSeparation; 
        }

        /*! \brief sets the property keyXOffset to the specified \a __value. 
            \details Description of the parameter keyXOffset is: <BLOCKQUOTE>\copybrief keyXOffset </BLOCKQUOTE> 
            \see keyXOffset for more information */ 
        inline virtual void set_keyXOffset(double __value)
        {
            if (this->keyXOffset != __value) {
                this->keyXOffset = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property keyXOffset. 
            \details Description of the parameter keyXOffset is: <BLOCKQUOTE>\copybrief keyXOffset </BLOCKQUOTE> 
            \see keyXOffset for more information */ 
        inline virtual double get_keyXOffset() const  
        {
            return this->keyXOffset; 
        }
        /*! \brief sets the property keyYOffset to the specified \a __value. 
            \details Description of the parameter keyYOffset is: <BLOCKQUOTE>\copybrief keyYOffset </BLOCKQUOTE> 
            \see keyYOffset for more information */ 
        inline virtual void set_keyYOffset(double __value)
        {
            if (this->keyYOffset != __value) {
                this->keyYOffset = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property keyYOffset. 
            \details Description of the parameter keyYOffset is: <BLOCKQUOTE>\copybrief keyYOffset </BLOCKQUOTE> 
            \see keyYOffset for more information */ 
        inline virtual double get_keyYOffset() const  
        {
            return this->keyYOffset; 
        }
        /*! \brief sets the property showKey to the specified \a __value. 
            \details Description of the parameter showKey is: <BLOCKQUOTE>\copybrief showKey </BLOCKQUOTE> 
            \see showKey for more information */ 
        inline virtual void set_showKey(bool __value)
        {
            if (this->showKey != __value) {
                this->showKey = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property showKey. 
            \details Description of the parameter showKey is: <BLOCKQUOTE>\copybrief showKey </BLOCKQUOTE> 
            \see showKey for more information */ 
        inline virtual bool get_showKey() const  
        {
            return this->showKey; 
        }
        /*! \brief sets the property showKeyFrame to the specified \a __value. 
            \details Description of the parameter showKeyFrame is: <BLOCKQUOTE>\copybrief showKeyFrame </BLOCKQUOTE> 
            \see showKeyFrame for more information */ 
        inline virtual void set_showKeyFrame(bool __value)
        {
            if (this->showKeyFrame != __value) {
                this->showKeyFrame = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property showKeyFrame. 
            \details Description of the parameter showKeyFrame is: <BLOCKQUOTE>\copybrief showKeyFrame </BLOCKQUOTE> 
            \see showKeyFrame for more information */ 
        inline virtual bool get_showKeyFrame() const  
        {
            return this->showKeyFrame; 
        }
        /*! \brief sets the property keyFrameColor to the specified \a __value. 
            \details Description of the parameter keyFrameColor is: <BLOCKQUOTE>\copybrief keyFrameColor </BLOCKQUOTE> 
            \see keyFrameColor for more information */ 
        inline virtual void set_keyFrameColor(const QColor & __value)  
        {
            if (this->keyFrameColor != __value) {
                this->keyFrameColor = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property keyFrameColor. 
            \details Description of the parameter keyFrameColor is: <BLOCKQUOTE>\copybrief keyFrameColor </BLOCKQUOTE> 
            \see keyFrameColor for more information */ 
        inline virtual QColor get_keyFrameColor() const  
        {
            return this->keyFrameColor; 
        }
        /*! \brief sets the property keyBackgroundColor to the specified \a __value. 
            \details Description of the parameter keyBackgroundColor is: <BLOCKQUOTE>\copybrief keyBackgroundColor </BLOCKQUOTE> 
            \see keyBackgroundColor for more information */ 
        inline virtual void set_keyBackgroundColor(const QColor & __value)  
        {
            if (this->keyBackgroundColor != __value) {
                this->keyBackgroundColor = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property keyBackgroundColor. 
            \details Description of the parameter keyBackgroundColor is: <BLOCKQUOTE>\copybrief keyBackgroundColor </BLOCKQUOTE> 
            \see keyBackgroundColor for more information */ 
        inline virtual QColor get_keyBackgroundColor() const  
        {
            return this->keyBackgroundColor; 
        }
        /*! \brief sets the property keyFrameWidth to the specified \a __value. 
            \details Description of the parameter keyFrameWidth is: <BLOCKQUOTE>\copybrief keyFrameWidth </BLOCKQUOTE> 
            \see keyFrameWidth for more information */ 
        inline virtual void set_keyFrameWidth(double __value)
        {
            if (this->keyFrameWidth != __value) {
                this->keyFrameWidth = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property keyFrameWidth. 
            \details Description of the parameter keyFrameWidth is: <BLOCKQUOTE>\copybrief keyFrameWidth </BLOCKQUOTE> 
            \see keyFrameWidth for more information */ 
        inline virtual double get_keyFrameWidth() const  
        {
            return this->keyFrameWidth; 
        }
        /*! \brief sets the property keyAutosize to the specified \a __value. 
            \details Description of the parameter keyAutosize is: <BLOCKQUOTE>\copybrief keyAutosize </BLOCKQUOTE> 
            \see keyAutosize for more information */ 
        inline virtual void set_keyAutosize(bool __value)
        {
            if (this->keyAutosize != __value) {
                this->keyAutosize = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property keyAutosize. 
            \details Description of the parameter keyAutosize is: <BLOCKQUOTE>\copybrief keyAutosize </BLOCKQUOTE> 
            \see keyAutosize for more information */ 
        inline virtual bool get_keyAutosize() const  
        {
            return this->keyAutosize; 
        }
        /*! \brief sets the property keyPosition to the specified \a __value. 
            \details Description of the parameter keyPosition is: <BLOCKQUOTE>\copybrief keyPosition </BLOCKQUOTE> 
            \see keyPosition for more information */ 
        inline virtual void set_keyPosition(const JKQTPkeyPosition & __value)  
        {
            if (this->keyPosition != __value) {
                this->keyPosition = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property keyPosition. 
            \details Description of the parameter keyPosition is: <BLOCKQUOTE>\copybrief keyPosition </BLOCKQUOTE> 
            \see keyPosition for more information */ 
        inline virtual JKQTPkeyPosition get_keyPosition() const  
        {
            return this->keyPosition; 
        }
        /*! \brief sets the property keyLayout to the specified \a __value. 
            \details Description of the parameter keyLayout is: <BLOCKQUOTE>\copybrief keyLayout </BLOCKQUOTE> 
            \see keyLayout for more information */ 
        inline virtual void set_keyLayout(const JKQTPkeyLayout & __value)  
        {
            if (this->keyLayout != __value) {
                this->keyLayout = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property keyLayout. 
            \details Description of the parameter keyLayout is: <BLOCKQUOTE>\copybrief keyLayout </BLOCKQUOTE> 
            \see keyLayout for more information */ 
        inline virtual JKQTPkeyLayout get_keyLayout() const  
        {
            return this->keyLayout; 
        }

        /*! \brief sets the property plotLabelFontSize to the specified \a __value. 
            \details Description of the parameter plotLabelFontSize is: <BLOCKQUOTE>\copybrief plotLabelFontSize </BLOCKQUOTE> 
            \see plotLabelFontSize for more information */ 
        inline virtual void set_plotLabelFontSize(double __value)
        {
            if (this->plotLabelFontSize != __value) {
                this->plotLabelFontSize = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property plotLabelFontSize. 
            \details Description of the parameter plotLabelFontSize is: <BLOCKQUOTE>\copybrief plotLabelFontSize </BLOCKQUOTE> 
            \see plotLabelFontSize for more information */ 
        inline virtual double get_plotLabelFontSize() const  
        {
            return this->plotLabelFontSize; 
        }
        /*! \brief sets the property plotLabelFontname to the specified \a __value. 
            \details Description of the parameter plotLabelFontname is: <BLOCKQUOTE>\copybrief plotLabelFontname </BLOCKQUOTE> 
            \see plotLabelFontname for more information */ 
        inline virtual void set_plotLabelFontname(const QString & __value)  
        {
            if (this->plotLabelFontname != __value) {
                this->plotLabelFontname = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property plotLabelFontname. 
            \details Description of the parameter plotLabelFontname is: <BLOCKQUOTE>\copybrief plotLabelFontname </BLOCKQUOTE> 
            \see plotLabelFontname for more information */ 
        inline virtual QString get_plotLabelFontname() const  
        {
            return this->plotLabelFontname; 
        }
        /*! \brief sets the property plotLabel to the specified \a __value. 
            \details Description of the parameter plotLabel is: <BLOCKQUOTE>\copybrief plotLabel </BLOCKQUOTE> 
            \see plotLabel for more information */ 
        inline virtual void set_plotLabel(const QString & __value)  
        {
            if (this->plotLabel != __value) {
                this->plotLabel = __value; 
                update_plot(); 
            } 
        } 
        /*! \brief returns the property plotLabel. 
            \details Description of the parameter plotLabel is: <BLOCKQUOTE>\copybrief plotLabel </BLOCKQUOTE> 
            \see plotLabel for more information */ 
        inline virtual QString get_plotLabel() const  
        {
            return this->plotLabel; 
        }

        /*! \brief sets the property gridPrinting to the specified \a __value. 
            \details Description of the parameter gridPrinting is: <BLOCKQUOTE>\copybrief gridPrinting </BLOCKQUOTE> 
            \see gridPrinting for more information */ 
        inline virtual void set_gridPrinting(bool __value)
        {
            this->gridPrinting = __value;
        } 
        /*! \brief returns the property gridPrinting. 
            \details Description of the parameter gridPrinting is: <BLOCKQUOTE>\copybrief gridPrinting </BLOCKQUOTE> 
            \see gridPrinting for more information */ 
        inline virtual bool get_gridPrinting() const  
        {
            return this->gridPrinting; 
        }
        /*! \brief sets the property gridPrintingCurrentX to the specified \a __value. 
            \details Description of the parameter gridPrintingCurrentX is: <BLOCKQUOTE>\copybrief gridPrintingCurrentX </BLOCKQUOTE> 
            \see gridPrintingCurrentX for more information */ 
        inline virtual void set_gridPrintingCurrentX(size_t __value)
        {
            this->gridPrintingCurrentX = __value;
        } 
        /*! \brief returns the property gridPrintingCurrentX. 
            \details Description of the parameter gridPrintingCurrentX is: <BLOCKQUOTE>\copybrief gridPrintingCurrentX </BLOCKQUOTE> 
            \see gridPrintingCurrentX for more information */ 
        inline virtual size_t get_gridPrintingCurrentX() const  
        {
            return this->gridPrintingCurrentX; 
        }
        /*! \brief sets the property gridPrintingCurrentY to the specified \a __value. 
            \details Description of the parameter gridPrintingCurrentY is: <BLOCKQUOTE>\copybrief gridPrintingCurrentY </BLOCKQUOTE> 
            \see gridPrintingCurrentY for more information */ 
        inline virtual void set_gridPrintingCurrentY(size_t __value)
        {
            this->gridPrintingCurrentY = __value;
        } 
        /*! \brief returns the property gridPrintingCurrentY. 
            \details Description of the parameter gridPrintingCurrentY is: <BLOCKQUOTE>\copybrief gridPrintingCurrentY </BLOCKQUOTE> 
            \see gridPrintingCurrentY for more information */ 
        inline virtual size_t get_gridPrintingCurrentY() const  
        {
            return this->gridPrintingCurrentY; 
        }
        /*! \brief sets the property currentSaveDirectory to the specified \a __value. 
            \details Description of the parameter currentSaveDirectory is: <BLOCKQUOTE>\copybrief currentSaveDirectory </BLOCKQUOTE> 
            \see currentSaveDirectory for more information */ 
        inline virtual void set_currentSaveDirectory(const QString & __value)  
        {
            this->currentSaveDirectory = __value;
        } 
        /*! \brief returns the property currentSaveDirectory. 
            \details Description of the parameter currentSaveDirectory is: <BLOCKQUOTE>\copybrief currentSaveDirectory </BLOCKQUOTE> 
            \see currentSaveDirectory for more information */ 
        inline virtual QString get_currentSaveDirectory() const  
        {
            return this->currentSaveDirectory; 
        }
        /*! \brief sets the property currentFileFormat to the specified \a __value. 
            \details Description of the parameter currentFileFormat is: <BLOCKQUOTE>\copybrief currentFileFormat </BLOCKQUOTE> 
            \see currentFileFormat for more information */ 
        inline virtual void set_currentFileFormat(const QString & __value)  
        {
            this->currentFileFormat = __value;
        } 
        /*! \brief returns the property currentFileFormat. 
            \details Description of the parameter currentFileFormat is: <BLOCKQUOTE>\copybrief currentFileFormat </BLOCKQUOTE> 
            \see currentFileFormat for more information */ 
        inline virtual QString get_currentFileFormat() const  
        {
            return this->currentFileFormat; 
        }
        /*! \brief sets the property CSVdecimalSeparator to the specified \a __value. 
            \details Description of the parameter CSVdecimalSeparator is: <BLOCKQUOTE>\copybrief CSVdecimalSeparator </BLOCKQUOTE> 
            \see CSVdecimalSeparator for more information */ 
        inline virtual void set_CSVdecimalSeparator(const QString & __value)  
        {
            this->CSVdecimalSeparator = __value;
        } 
        /*! \brief returns the property CSVdecimalSeparator. 
            \details Description of the parameter CSVdecimalSeparator is: <BLOCKQUOTE>\copybrief CSVdecimalSeparator </BLOCKQUOTE> 
            \see CSVdecimalSeparator for more information */ 
        inline virtual QString get_CSVdecimalSeparator() const  
        {
            return this->CSVdecimalSeparator; 
        }
        /*! \brief sets the property CSVcommentInitializer to the specified \a __value. 
            \details Description of the parameter CSVcommentInitializer is: <BLOCKQUOTE>\copybrief CSVcommentInitializer </BLOCKQUOTE> 
            \see CSVcommentInitializer for more information */ 
        inline virtual void set_CSVcommentInitializer(const QString & __value)  
        {
            this->CSVcommentInitializer = __value;
        } 
        /*! \brief returns the property CSVcommentInitializer. 
            \details Description of the parameter CSVcommentInitializer is: <BLOCKQUOTE>\copybrief CSVcommentInitializer </BLOCKQUOTE> 
            \see CSVcommentInitializer for more information */ 
        inline virtual QString get_CSVcommentInitializer() const  
        {
            return this->CSVcommentInitializer; 
        }

        /*! \brief returns the property iplotBorderTop. \details Description of the parameter iplotBorderTop is:  <BLOCKQUOTE>\copybrief iplotBorderTop </BLOCKQUOTE>. \see iplotBorderTop for more information */ 
        inline int get_iplotBorderTop() const { return this->iplotBorderTop; }
        /*! \brief returns the property iplotBorderLeft. \details Description of the parameter iplotBorderLeft is:  <BLOCKQUOTE>\copybrief iplotBorderLeft </BLOCKQUOTE>. \see iplotBorderLeft for more information */ 
        inline int get_iplotBorderLeft() const { return this->iplotBorderLeft; }
        /*! \brief returns the property iplotBorderBottom. \details Description of the parameter iplotBorderBottom is:  <BLOCKQUOTE>\copybrief iplotBorderBottom </BLOCKQUOTE>. \see iplotBorderBottom for more information */ 
        inline int get_iplotBorderBottom() const { return this->iplotBorderBottom; }
        /*! \brief returns the property iplotBorderRight. \details Description of the parameter iplotBorderRight is:  <BLOCKQUOTE>\copybrief iplotBorderRight </BLOCKQUOTE>. \see iplotBorderRight for more information */ 
        inline int get_iplotBorderRight() const { return this->iplotBorderRight; }
        /*! \brief returns the property plotWidth. \details Description of the parameter plotWidth is:  <BLOCKQUOTE>\copybrief plotWidth </BLOCKQUOTE>. \see plotWidth for more information */ 
        inline int get_plotWidth() const { return this->plotWidth; }
        /*! \brief returns the property plotHeight. \details Description of the parameter plotHeight is:  <BLOCKQUOTE>\copybrief plotHeight </BLOCKQUOTE>. \see plotHeight for more information */ 
        inline int get_plotHeight() const { return this->plotHeight; }
        /** \brief returns the internal JKQTmathText, used to render text with LaTeX markup */
        inline JKQTmathText* get_mathText() { return &mathText; }
        /** \brief returns the internal JKQTmathText, used to render text with LaTeX markup */
        inline const JKQTmathText* get_mathText() const { return &mathText; }
        /** \brief returns the x-axis objet of the plot */
        inline JKQTPhorizontalAxis* get_xAxis() { return xAxis; }
        /** \brief returns the y-axis objet of the plot */
        inline JKQTPverticalAxis* get_yAxis() { return yAxis; }
        /** \brief returns the x-axis objet of the plot */
        inline const JKQTPhorizontalAxis* get_xAxis() const { return xAxis; }
        /** \brief returns the y-axis objet of the plot */
        inline const JKQTPverticalAxis* get_yAxis() const { return yAxis; }


        /*! \brief returns the property actSavePlot. \details Description of the parameter actSavePlot is:  <BLOCKQUOTE>\copybrief actSavePlot </BLOCKQUOTE>. \see actSavePlot for more information */ 
        inline QAction* get_actSavePlot() const { return this->actSavePlot; }
        /*! \brief returns the property actSaveData. \details Description of the parameter actSaveData is:  <BLOCKQUOTE>\copybrief actSaveData </BLOCKQUOTE>. \see actSaveData for more information */ 
        inline QAction* get_actSaveData() const { return this->actSaveData; }
        /*! \brief returns the property actCopyData. \details Description of the parameter actCopyData is:  <BLOCKQUOTE>\copybrief actCopyData </BLOCKQUOTE>. \see actCopyData for more information */ 
        inline QAction* get_actCopyData() const { return this->actCopyData; }
        /*! \brief returns the property actCopyPixelImage. \details Description of the parameter actCopyPixelImage is:  <BLOCKQUOTE>\copybrief actCopyPixelImage </BLOCKQUOTE>. \see actCopyPixelImage for more information */ 
        inline QAction* get_actCopyPixelImage() const { return this->actCopyPixelImage; }
        /*! \brief returns the property actCopyMatlab. \details Description of the parameter actCopyMatlab is:  <BLOCKQUOTE>\copybrief actCopyMatlab </BLOCKQUOTE>. \see actCopyMatlab for more information */ 
        inline QAction* get_actCopyMatlab() const { return this->actCopyMatlab; }
        /*! \brief returns the property actSavePDF. \details Description of the parameter actSavePDF is:  <BLOCKQUOTE>\copybrief actSavePDF </BLOCKQUOTE>. \see actSavePDF for more information */ 
        inline QAction* get_actSavePDF() const { return this->actSavePDF; }
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        /*! \brief returns the property actSavePS. \details Description of the parameter actSavePS is:  <BLOCKQUOTE>\copybrief actSavePS </BLOCKQUOTE>. \see actSavePS for more information */ 
        inline QAction* get_actSavePS() const { return this->actSavePS; }
#endif
        /*! \brief returns the property actSavePix. \details Description of the parameter actSavePix is:  <BLOCKQUOTE>\copybrief actSavePix </BLOCKQUOTE>. \see actSavePix for more information */ 
        inline QAction* get_actSavePix() const { return this->actSavePix; }
        /*! \brief returns the property actSaveSVG. \details Description of the parameter actSaveSVG is:  <BLOCKQUOTE>\copybrief actSaveSVG </BLOCKQUOTE>. \see actSaveSVG for more information */ 
        inline QAction* get_actSaveSVG() const { return this->actSaveSVG; }
        /*! \brief returns the property actPrint. \details Description of the parameter actPrint is:  <BLOCKQUOTE>\copybrief actPrint </BLOCKQUOTE>. \see actPrint for more information */ 
        inline QAction* get_actPrint() const { return this->actPrint; }
        /*! \brief returns the property actSaveCSV. \details Description of the parameter actSaveCSV is:  <BLOCKQUOTE>\copybrief actSaveCSV </BLOCKQUOTE>. \see actSaveCSV for more information */ 
        inline QAction* get_actSaveCSV() const { return this->actSaveCSV; }
        /*! \brief returns the property actZoomAll. \details Description of the parameter actZoomAll is:  <BLOCKQUOTE>\copybrief actZoomAll </BLOCKQUOTE>. \see actZoomAll for more information */ 
        inline QAction* get_actZoomAll() const { return this->actZoomAll; }
        /*! \brief returns the property actZoomIn. \details Description of the parameter actZoomIn is:  <BLOCKQUOTE>\copybrief actZoomIn </BLOCKQUOTE>. \see actZoomIn for more information */ 
        inline QAction* get_actZoomIn() const { return this->actZoomIn; }
        /*! \brief returns the property actZoomOut. \details Description of the parameter actZoomOut is:  <BLOCKQUOTE>\copybrief actZoomOut </BLOCKQUOTE>. \see actZoomOut for more information */ 
        inline QAction* get_actZoomOut() const { return this->actZoomOut; }
        /*! \brief returns the property actShowPlotData. \details Description of the parameter actShowPlotData is:  <BLOCKQUOTE>\copybrief actShowPlotData </BLOCKQUOTE>. \see actShowPlotData for more information */ 
        inline QAction* get_actShowPlotData() const { return this->actShowPlotData; }
        /*! \brief returns the property lstAdditionalPlotterActions. \details Description of the parameter lstAdditionalPlotterActions is:  <BLOCKQUOTE>\copybrief lstAdditionalPlotterActions </BLOCKQUOTE>. \see lstAdditionalPlotterActions for more information */ 
        inline AdditionalActionsMap get_lstAdditionalPlotterActions() const { return this->lstAdditionalPlotterActions; }

        /** \brief this function registers additional actions to lstAdditionalPlotterActions, which are displayed in the context-menu */
        void registerAdditionalAction(const QString& key, QAction* act);
        void deregisterAdditionalAction(QAction* act);

        /*! \brief returns the property masterSynchronizeWidth. \details Description of the parameter masterSynchronizeWidth is:  <BLOCKQUOTE>\copybrief masterSynchronizeWidth </BLOCKQUOTE>. \see masterSynchronizeWidth for more information */ 
        inline bool get_masterSynchronizeWidth() const { return this->masterSynchronizeWidth; }
        /*! \brief returns the property masterSynchronizeHeight. \details Description of the parameter masterSynchronizeHeight is:  <BLOCKQUOTE>\copybrief masterSynchronizeHeight </BLOCKQUOTE>. \see masterSynchronizeHeight for more information */ 
        inline bool get_masterSynchronizeHeight() const { return this->masterSynchronizeHeight; }
        /*! \brief sets the property def_backgroundColor to the specified \a __value. 
            \details Description of the parameter def_backgroundColor is:  <BLOCKQUOTE>\copybrief def_backgroundColor </BLOCKQUOTE> 
        	\see def_backgroundColor for more information */ 
        inline virtual void set_def_backgroundColor(const QColor & __value)  
        { 
        	this->def_backgroundColor = __value; 
        }
        /*! \brief sets the property def_plotBackgroundColor to the specified \a __value. 
            \details Description of the parameter def_plotBackgroundColor is:  <BLOCKQUOTE>\copybrief def_plotBackgroundColor </BLOCKQUOTE> 
        	\see def_plotBackgroundColor for more information */ 
        inline virtual void set_def_plotBackgroundColor(const QColor & __value)  
        { 
        	this->def_plotBackgroundColor = __value; 
        }

        /*! \brief sets the property fontSizePrintMultiplier to the specified \a __value. 
            \details Description of the parameter fontSizePrintMultiplier is: <BLOCKQUOTE>\copybrief fontSizePrintMultiplier </BLOCKQUOTE> 
            \see fontSizePrintMultiplier for more information */ 
        inline virtual void set_fontSizePrintMultiplier(double __value)
        {
            this->fontSizePrintMultiplier = __value;
        } 
        /*! \brief returns the property fontSizePrintMultiplier. 
            \details Description of the parameter fontSizePrintMultiplier is: <BLOCKQUOTE>\copybrief fontSizePrintMultiplier </BLOCKQUOTE> 
            \see fontSizePrintMultiplier for more information */ 
        inline virtual double get_fontSizePrintMultiplier() const  
        {
            return this->fontSizePrintMultiplier; 
        }
        /*! \brief sets the property lineWidthPrintMultiplier to the specified \a __value. 
            \details Description of the parameter lineWidthPrintMultiplier is: <BLOCKQUOTE>\copybrief lineWidthPrintMultiplier </BLOCKQUOTE> 
            \see lineWidthPrintMultiplier for more information */ 
        inline virtual void set_lineWidthPrintMultiplier(double __value)
        {
            this->lineWidthPrintMultiplier = __value;
        } 
        /*! \brief returns the property lineWidthPrintMultiplier. 
            \details Description of the parameter lineWidthPrintMultiplier is: <BLOCKQUOTE>\copybrief lineWidthPrintMultiplier </BLOCKQUOTE> 
            \see lineWidthPrintMultiplier for more information */ 
        inline virtual double get_lineWidthPrintMultiplier() const  
        {
            return this->lineWidthPrintMultiplier; 
        }
        /*! \brief returns the property fontSizeMultiplier. \details Description of the parameter fontSizeMultiplier is:  <BLOCKQUOTE>\copybrief fontSizeMultiplier </BLOCKQUOTE>. \see fontSizeMultiplier for more information */ 
        inline double get_fontSizeMultiplier() const { return this->fontSizeMultiplier; }
        /*! \brief returns the property lineWidthMultiplier. \details Description of the parameter lineWidthMultiplier is:  <BLOCKQUOTE>\copybrief lineWidthMultiplier </BLOCKQUOTE>. \see lineWidthMultiplier for more information */ 
        inline double get_lineWidthMultiplier() const { return this->lineWidthMultiplier; }


        /** \brief returns description of i'th graph */
        JKQTPplotElement* getGraph(size_t i);

        /** \brief returns the number of graphs */
        size_t getGraphCount();

        /** \brief remove the i-th graph */
        void deleteGraph(size_t i, bool deletegraph=true);

        /** \brief remove the given graph, if it is contained */
        void deleteGraph(JKQTPplotElement* gr, bool deletegraph=true);

        /** \brief remove all plots
         *
         *  \param deleteGraphs if set \c true (default) the graph objects will also be deleted
         */
        void clearGraphs(bool deleteGraphs=true);

        void setAllGraphsInvisible();
        void setAllGraphsVisible();
        void setGraphVisible(int i, bool visible=true);

        /** \brief add a new graph, returns it's position in the graphs list, if the graph is already in the plot, this returns the index in the list */
        size_t addGraph(JKQTPplotElement* gr);

        /** \brief returns \c true, if the given graph is in this plot */
        bool containsGraph(JKQTPplotElement* gr) const;

        /** \brief move the given graph to the top, or add it, if it is not yet contained */
        size_t moveGraphTop(JKQTPplotElement* gr);
        /** \brief move the given graph to the top, or add it, if it is not yet contained */
        size_t moveGraphBottom(JKQTPplotElement* gr);

        /** \brief add a new graphs from a QVector */
        template <class TJKQTPgraphContainer>
        inline void addGraphs(const TJKQTPgraphContainer& gr) {
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
         * Both point to columns in the datastore explained in the JKQtPlotterBase class. The plotWidth, color, pen style ...
         * will be extracted from the automatic plot style creation mechanism implemented in JKQtPlotterBase::getNextStyle().
         * If you want to change them either use another overloaded version of addGraph(), or use getGraph() and setGraph():
         * \code
         * size_t i=addGraph(0,1,"graph1");
         * JKQTPplotElement gr=getGraph(i);
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
            \param synchronizeZoomingMasterToSlave if set, also zooming in the master leads to a modification of the linked axes in the slave
            \param synchronizeZoomingSlaveToMaster if set, also zooming in the slave leads to a modification of the linked axes in the master
        */
        void synchronizeToMaster(JKQtBasePlotter* master, bool synchronizeWidth, bool synchronizeHeight, bool synchronizeZoomingMasterToSlave=false, bool synchronizeZoomingSlaveToMaster=false);

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

        QVector<QLineF> getBoundingLinesX1Y1(QRectF *rect=nullptr) const;

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
         *
         *  These presets are application global and will be used ONLY on initialization of a JKQtBasePlotter. You can overwrite them
         *  on a local-basis for each JKQtBasePrinter separately. The changed values from this function call will only take effect for
         *  newly created plotters AFTER the function call!
         */
        static void setDefaultJKQtBasePrinterUserSettings(QString userSettigsFilename, QString userSettigsPrefix);

        /** \brief Service from this class to implement a special QPaintDevice as a plugin, that can be registered to JKQtBasePlotter/JKQtPlotter
         *         and then be used to export graphics, use registerPaintDeviceAdapter() to register such a plass */
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

        /** \brief register a user-defined QPaintDevice (with factory JKQTPPaintDeviceAdapter) as a plugin to JKQtBasePlotter/JKQtPlotter,
         *         which will use it to export graphics */
        static void registerPaintDeviceAdapter(JKQTPPaintDeviceAdapter* adapter);
        /** \brief de-register a  JKQTPPaintDeviceAdapter from JKQtBasePlotter/JKQtPlotter */
        static void deregisterPaintDeviceAdapter(JKQTPPaintDeviceAdapter* adapter);


        /** \brief virtual base-class for exporter classes that can be used to save data inot a file */
        class LIB_EXPORT JKQTPSaveDataAdapter {
            public:
                virtual ~JKQTPSaveDataAdapter() ;
                virtual QString getFilter() const=0;
                virtual void saveJKQTPData(const QString& filename, const QList<QVector<double> >& data, const QStringList& columnNames) const=0;
        };

        /** \brief register a JKQTPSaveDataAdapter with JKQtPlotter/JKQtBasePlotter that can be used to export data from the internal datastore into a file */
        static bool registerSaveDataAdapter(JKQTPSaveDataAdapter* adapter);
        /** \brief de-register a JKQTPSaveDataAdapter from JKQtPlotter/JKQtBasePlotter */
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
        static QHash<JKQtBasePlotter::textSizeKey, JKQtBasePlotter::textSizeData> tbrh;

};

inline uint qHash(const JKQtBasePlotter::textSizeKey& data) {
    return qHash(data.f.family())+qHash(data.text);
}

#endif // JKQTPBASEPLOTTER_H
