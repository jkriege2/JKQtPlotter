/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)



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

#ifndef JKQTPGRAPHSBASESTYLE_H
#define JKQTPGRAPHSBASESTYLE_H

#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtpimagetools.h"
#include "jkqtplotter/jkqtpcoordinateaxesstyle.h"
#include "jkqtplotter/jkqtpkeystyle.h"
#include "jkqtplotter/jkqtpimagetools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtcommon/jkqtpbasicimagetools.h"
#include "jkqtplotter/jkqtplotter_configmacros.h"
#include <QColor>
#include <QPainter>
#include <QString>
#include <QHash>
#include <QPair>
#include <QVector>
#include <QSettings>

class JKQTBasePlotterStyle; // forward

/** \brief Support Class for JKQTBasePlotter, which summarizes a fill style
 *  \ingroup jkqtpplotter_styling_classes
 *
 *  \see JKQTBasePlotter, JKQTGraphsBaseStyle, \ref jkqtpplotter_styling
 */
class JKQTPLOTTER_LIB_EXPORT JKQTFillStyleSummmary {
#ifndef JKQTPLOTTER_WORKAROUND_QGADGET_BUG
    Q_GADGET
#endif
public:
    JKQTFillStyleSummmary(Qt::BrushStyle style=Qt::SolidPattern, const QGradient& grad=QGradient(), double rotAngleDeg=0.0);

    Qt::BrushStyle brushStyle;
    QGradient gradient;
    QPixmap texture;
    double rotationAngleDeg;

    QBrush brush(const QColor& color) const;

    /** \brief reads object contents from a string representation, e.g. as created by JKQTFillStyleSummmary::toCSSString() */
    static JKQTFillStyleSummmary fromString(const QString& style);
    /** \brief converst the contents to a string representation */
    QString toCSSString() const;
};

/** \brief Support Class for JKQTBasePlotter, which summarizes all properties that define the visual styling of graphs
 *  \ingroup jkqtpplotter_styling_classes
 *
 *  \see JKQTBasePlotter, \ref jkqtpplotter_styling
 */
class JKQTPLOTTER_LIB_EXPORT JKQTGraphsSpecificStyleProperties {
#ifndef JKQTPLOTTER_WORKAROUND_QGADGET_BUG
      Q_GADGET
#endif
    public:
        /** \brief initializes the object for the given \a type and takes some properties from the \a parent */
        JKQTGraphsSpecificStyleProperties(JKQTPPlotStyleType type, const JKQTBasePlotterStyle& parent);
        /** \brief initializes the object as a copy of \a other, but modified some properties for the given \a type */
        JKQTGraphsSpecificStyleProperties(JKQTPPlotStyleType type, const JKQTGraphsSpecificStyleProperties& other);
        JKQTGraphsSpecificStyleProperties(const JKQTGraphsSpecificStyleProperties& other)=default;
		JKQTGraphsSpecificStyleProperties(JKQTGraphsSpecificStyleProperties&& other)=default;
		JKQTGraphsSpecificStyleProperties& operator=(const JKQTGraphsSpecificStyleProperties& other)=default;
		JKQTGraphsSpecificStyleProperties& operator=(JKQTGraphsSpecificStyleProperties&& other)=default;


        /** \brief loads the plot properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object
         *
         *  \param settings QSettings-object to read from
         *  \param group Group in the QSettings-object to read from
         *  \param defaultStyle If a setting cannot be found in \a settings, default values are taken from this object
         *                      By default, this is a default-constructed object
         */
        void loadSettings(const QSettings &settings, const QString& group, const JKQTGraphsSpecificStyleProperties &defaultStyle);

        /** \brief saves the plot properties into a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object.
         *
         *  \param settings QSettings-object to save to
         *  \param group Group in the QSettings-object to save to
         */
        void saveSettings(QSettings& settings, const QString& group) const;
		
        /** \brief width (in pt) of lines used for newly added graphs */
        double defaultLineWidth;
        /** \brief size (in pt) of symbols used for newly added graphs */
        double defaultSymbolSize;
        /** \brief width (in pt) of the outline of symbols used for newly added graphs */
        double defaultSymbolLineWidth;
        /** \brief width (in pt) of lines used for the error indicators of newly added graphs */
        double defaultErrorIndicatorWidth;
        /** \brief head decorator style */
        JKQTPLineDecoratorStyle defaultHeadDecoratorStyle;
        /** \brief head decorator size-factor, used to calculate the size of the arrow from the line width */
        double defaultHeadDecoratorSizeFactor;
        /** \↓brief fill style for error indicators */
        Qt::BrushStyle errorFillStyle;
        /** \brief defines how to derive the basic graph color for a new graph from the color selected from JKQTGraphsBaseStyle::defaultGraphColors
         *
         *  This property is usually JKQTPFFCMSameColor, but can be changed to allow to e.g. fill
         *  barcharts always with a lighter color than the full color ...
         */
        JKQTPColorDerivationMode graphColorDerivationMode;
        /** \brief defines how to derive a fill color for a new graph */
        JKQTPColorDerivationMode fillColorDerivationMode;
        /** \brief defines how to derive an error color for a new graph */
        JKQTPColorDerivationMode errorColorDerivationMode;
        /** \brief defines how to derive an error fill color from the error color for a new graph */
        JKQTPColorDerivationMode errorFillColorDerivationMode;
        /** \brief defines how to derive a symbol fill color for a new graph */
        JKQTPColorDerivationMode symbolFillColorDerivationMode;
    private:
        /** \brief modifies some of the settings to match the defaults for the given JKQTPPlotStyleType (e.g. sets line-width for impulses ...) */
        void modifyForDefaultStyle(JKQTPPlotStyleType type);
};



 /** \brief Support Class for JKQTBasePlotter, which summarizes all properties that define the visual styling of geometric elements
 *  \ingroup jkqtpplotter_styling_classes
 *
 *  \see JKQTBasePlotter, \ref jkqtpplotter_styling
 */
class JKQTPLOTTER_LIB_EXPORT JKQTGeometricSpecificStyleProperties: public JKQTGraphsSpecificStyleProperties {
#ifndef JKQTPLOTTER_WORKAROUND_QGADGET_BUG
      Q_GADGET
#endif
public:
     explicit JKQTGeometricSpecificStyleProperties(const JKQTBasePlotterStyle& parent);
     JKQTGeometricSpecificStyleProperties(const JKQTBasePlotterStyle& parent, const JKQTGraphsSpecificStyleProperties& other);
     JKQTGeometricSpecificStyleProperties(JKQTPPlotStyleType type, const JKQTBasePlotterStyle& parent);
     JKQTGeometricSpecificStyleProperties(JKQTPPlotStyleType type, const JKQTGraphsSpecificStyleProperties& other, const JKQTBasePlotterStyle &parent);
     JKQTGeometricSpecificStyleProperties(const JKQTGeometricSpecificStyleProperties& other)=default;
     JKQTGeometricSpecificStyleProperties(JKQTGeometricSpecificStyleProperties&& other)=default;
     JKQTGeometricSpecificStyleProperties& operator=(const JKQTGeometricSpecificStyleProperties& other)=default;
     JKQTGeometricSpecificStyleProperties& operator=(JKQTGeometricSpecificStyleProperties&& other)=default;


     /** \brief loads the plot properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object
         *
         *  \param settings QSettings-object to read from
         *  \param group Group in the QSettings-object to read from
         *  \param defaultStyle If a setting cannot be found in \a settings, default values are taken from this object
         *                      By default, this is a default-constructed object
         */
     void loadSettings(const QSettings &settings, const QString& group, const JKQTGeometricSpecificStyleProperties &defaultStyle);

     /** \brief saves the plot properties into a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object.
         *
         *  \param settings QSettings-object to save to
         *  \param group Group in the QSettings-object to save to
         */
     void saveSettings(QSettings& settings, const QString& group) const;


     /** \brief graph color used */
     QColor defaultColor;
     /** \brief graph line style used */
     Qt::PenStyle defaultLineStyle;
     /** \brief graph symbol used */
     JKQTPGraphSymbols defaultSymbol;
     /** \brief graph fill style used */
     JKQTFillStyleSummmary defaultFillStyle;


};


 /** \brief Support Class for JKQTBasePlotter, which summarizes all properties that define the visual styling of annotation elements
 *  \ingroup jkqtpplotter_styling_classes
 *
 *  \see JKQTBasePlotter, \ref jkqtpplotter_styling
 */
class JKQTPLOTTER_LIB_EXPORT JKQTAnnotationsSpecificStyleProperties: public JKQTGeometricSpecificStyleProperties {
#ifndef JKQTPLOTTER_WORKAROUND_QGADGET_BUG
      Q_GADGET
#endif
public:
     explicit JKQTAnnotationsSpecificStyleProperties(const JKQTBasePlotterStyle& parent);
     JKQTAnnotationsSpecificStyleProperties(const JKQTBasePlotterStyle& parent, const JKQTGraphsSpecificStyleProperties& other);
     JKQTAnnotationsSpecificStyleProperties(const JKQTAnnotationsSpecificStyleProperties& other)=default;
     JKQTAnnotationsSpecificStyleProperties(JKQTAnnotationsSpecificStyleProperties&& other)=default;
     JKQTAnnotationsSpecificStyleProperties& operator=(const JKQTAnnotationsSpecificStyleProperties& other)=default;
     JKQTAnnotationsSpecificStyleProperties& operator=(JKQTAnnotationsSpecificStyleProperties&& other)=default;


     /** \brief loads the plot properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object
         *
         *  \param settings QSettings-object to read from
         *  \param group Group in the QSettings-object to read from
         *  \param defaultStyle If a setting cannot be found in \a settings, default values are taken from this object
         *                      By default, this is a default-constructed object
         */
     void loadSettings(const QSettings &settings, const QString& group, const JKQTAnnotationsSpecificStyleProperties &defaultStyle);

     /** \brief saves the plot properties into a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object.
         *
         *  \param settings QSettings-object to save to
         *  \param group Group in the QSettings-object to save to
         */
     void saveSettings(QSettings& settings, const QString& group) const;

     /** \brief default text color in the plot */
     QColor defaultTextColor;
     /** \brief default font size in the plot [pt] */
     double defaultFontSize;
     /** \brief default font name in the plot */
     QString defaultFontName;


};


/** \brief Support Class for JKQTBasePlotter, which summarizes all properties that define the visual styling of barchart elements
*  \ingroup jkqtpplotter_styling_classes
*
*  \see JKQTBasePlotter, \ref jkqtpplotter_styling
*/
class JKQTPLOTTER_LIB_EXPORT JKQTBarchartSpecificStyleProperties: public JKQTGraphsSpecificStyleProperties {
#ifndef JKQTPLOTTER_WORKAROUND_QGADGET_BUG
     Q_GADGET
#endif
public:
    explicit JKQTBarchartSpecificStyleProperties(const JKQTBasePlotterStyle& parent);
    JKQTBarchartSpecificStyleProperties(const JKQTBasePlotterStyle& parent, const JKQTGraphsSpecificStyleProperties& other);
    JKQTBarchartSpecificStyleProperties(const JKQTBarchartSpecificStyleProperties& other)=default;
    JKQTBarchartSpecificStyleProperties(JKQTBarchartSpecificStyleProperties&& other)=default;
    JKQTBarchartSpecificStyleProperties& operator=(const JKQTBarchartSpecificStyleProperties& other)=default;
    JKQTBarchartSpecificStyleProperties& operator=(JKQTBarchartSpecificStyleProperties&& other)=default;


    /** \brief loads the plot properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object
        *
        *  \param settings QSettings-object to read from
        *  \param group Group in the QSettings-object to read from
        *  \param defaultStyle If a setting cannot be found in \a settings, default values are taken from this object
        *                      By default, this is a default-constructed object
        */
    void loadSettings(const QSettings &settings, const QString& group, const JKQTBarchartSpecificStyleProperties &defaultStyle);

    /** \brief saves the plot properties into a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object.
        *
        *  \param settings QSettings-object to save to
        *  \param group Group in the QSettings-object to save to
        */
    void saveSettings(QSettings& settings, const QString& group) const;

    /** \brief corner radius (in pt) for bars at the "value" end */
    double defaultRectRadiusAtValue;
    /** \brief corner radius (in pt) for bars at the "baseline" end */
    double defaultRectRadiusAtBaseline;
    /** \brief indicates whether to draw a baseline (style is derived from axis style) */
    bool drawBaseline;


};



/** \brief Support Class for JKQTBasePlotter, which summarizes all properties that define the visual styling of impulse/stick graph elements
*  \ingroup jkqtpplotter_styling_classes
*
*  \see JKQTBasePlotter, \ref jkqtpplotter_styling
*/
class JKQTPLOTTER_LIB_EXPORT JKQTImpulseSpecificStyleProperties: public JKQTGraphsSpecificStyleProperties {
#ifndef JKQTPLOTTER_WORKAROUND_QGADGET_BUG
     Q_GADGET
#endif
public:
    explicit JKQTImpulseSpecificStyleProperties(const JKQTBasePlotterStyle& parent);
    JKQTImpulseSpecificStyleProperties(const JKQTBasePlotterStyle& parent, const JKQTGraphsSpecificStyleProperties& other);
    JKQTImpulseSpecificStyleProperties(const JKQTImpulseSpecificStyleProperties& other)=default;
    JKQTImpulseSpecificStyleProperties(JKQTImpulseSpecificStyleProperties&& other)=default;
    JKQTImpulseSpecificStyleProperties& operator=(const JKQTImpulseSpecificStyleProperties& other)=default;
    JKQTImpulseSpecificStyleProperties& operator=(JKQTImpulseSpecificStyleProperties&& other)=default;


    /** \brief loads the plot properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object
        *
        *  \param settings QSettings-object to read from
        *  \param group Group in the QSettings-object to read from
        *  \param defaultStyle If a setting cannot be found in \a settings, default values are taken from this object
        *                      By default, this is a default-constructed object
        */
    void loadSettings(const QSettings &settings, const QString& group, const JKQTImpulseSpecificStyleProperties &defaultStyle);

    /** \brief saves the plot properties into a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object.
        *
        *  \param settings QSettings-object to save to
        *  \param group Group in the QSettings-object to save to
        */
    void saveSettings(QSettings& settings, const QString& group) const;

    /** \brief indicates whether to draw a baseline (style is derived from axis style) */
    bool drawBaseline;


};



/** \brief Support Class for JKQTBasePlotter, which summarizes all properties that define the visual styling of a JKQTBasePlotter
 *  \ingroup jkqtpplotter_styling_classes
 *
 *  \see JKQTBasePlotter, \ref jkqtpplotter_styling
 */
class JKQTPLOTTER_LIB_EXPORT JKQTGraphsBaseStyle {
#ifndef JKQTPLOTTER_WORKAROUND_QGADGET_BUG
      Q_GADGET
#endif
    public:
        explicit JKQTGraphsBaseStyle(const JKQTBasePlotterStyle& parent);



        /** \brief loads the plot properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object
         *
         *  \param settings QSettings-object to read from
         *  \param group Group in the QSettings-object to read from
         *  \param defaultStyle If a setting cannot be found in \a settings, default values are taken from this object
         *                      By default, this is a default-constructed object
         *  \param parent parent JKQTBasePlotterStyle instance for inital values
         */
        void loadSettings(const QSettings &settings, const QString& group, const JKQTGraphsBaseStyle &defaultStyle, const JKQTBasePlotterStyle& parent);

        /** \brief saves the plot properties into a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object.
         *
         *  \param settings QSettings-object to save to
         *  \param group Group in the QSettings-object to save to
         */
        void saveSettings(QSettings& settings, const QString& group=QString("graphs_styling/")) const;

        /** \brief specifies whether to use antialiasing for plotting the graphs
         *
         * \note You can set this property \c false to increase plotting speed of complex plots (with many graphs inside). You can reach a
         *       roughly three-fold speed improvement!
         */
        bool useAntiAliasingForGraphs;

		
		/** \brief styling options for standard graphs */
        JKQTGraphsSpecificStyleProperties defaultGraphStyle;
        /** \brief styling options for bargraph graphs */
        JKQTBarchartSpecificStyleProperties barchartStyle;
        /** \brief styling options for boxplots graphs */
        JKQTGraphsSpecificStyleProperties boxplotStyle;
        /** \brief styling options for filled graphs */
        JKQTGraphsSpecificStyleProperties filledStyle;
        /** \brief styling options for impulses graphs */
        JKQTImpulseSpecificStyleProperties impulseStyle;
        /** \brief styling options for geometric elements */
        JKQTGeometricSpecificStyleProperties geometricStyle;
		/** \brief styling options for annotation elements */
		JKQTAnnotationsSpecificStyleProperties annotationStyle;

        /** \brief returns defaultGraphStyle, barchartStyle, ..., depending on the value of \a type */
        const JKQTGraphsSpecificStyleProperties& getGraphStyleByType(JKQTPPlotStyleType type) const;

        /** \brief color palette used by default for new graphs */
        JKQTPMathImageColorPalette defaultPalette;

        /** \brief colors used to automatically collor different graphs differently */
        QVector<QColor> defaultGraphColors;
        /** \brief Qt::PenStyle used to automatically style different graphs differently */
        QVector<Qt::PenStyle> defaultGraphPenStyles;
        /** \brief JKQTPGraphSymbols used to automatically assign to different graphs */
        QVector<JKQTPGraphSymbols> defaultGraphSymbols;
        /** \brief Qt::BrushStyle used to automatically style different graphs differently */
        QVector<JKQTFillStyleSummmary> defaultGraphFillStyles;

        /** \brief standard color palette for the default style */
        static QVector<QColor> getDefaultGraphColors();
        /** \brief a list of Qt::PenStyles used to automatically style different graphs differently in the default style */
        static QVector<Qt::PenStyle> getDefaultGraphPenStyles();
        /** \brief list of JKQTPGraphSymbols used to automatically assign to different graphs in the default style */
        static QVector<JKQTPGraphSymbols> getDefaultGraphSymbols();
        /** \brief list of Qt::BrushStyle used to automatically style different graphs differently in the default style */
        static QVector<JKQTFillStyleSummmary> getDefaultGraphFillStyles();

};

#endif // JKQTPGRAPHSBASESTYLE_H
