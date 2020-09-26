/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)



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
#include <QColor>
#include <QPainter>
#include <QString>
#include <QHash>
#include <QPair>
#include <QVector>
#include <QSettings>

class JKQTBasePlotterStyle; // forward


/** \brief Support Class for JKQTBasePlotter, which summarizes all properties that define the visual styling of graphs
 *  \ingroup jkqtpplotter_styling
 *
 *  \see JKQTBasePlotter, \ref jkqtpplotter_styling
 */
class JKQTPLOTTER_LIB_EXPORT JKQTGraphsSpecificStyleProperties {
        Q_GADGET
    public:
        JKQTGraphsSpecificStyleProperties(JKQTPPlotStyleType type, const JKQTBasePlotterStyle& parent);
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
        double defaultSymbolLineSize;
        /** \brief width (in pt) of lines used for the error indicators of newly added graphs */
        double defaultErrorIndicatorWidth;
        /** \brief head decorator style */
        JKQTPLineDecoratorStyle defaultHeadDecoratorStyle;
        /** \brief head decorator size-factor, used to calculate the size of the arrow from the line width */
        double defaultHeadDecoratorSizeFactor;
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
 };



 /** \brief Support Class for JKQTBasePlotter, which summarizes all properties that define the visual styling of geometric elements
 *  \ingroup jkqtpplotter_styling
 *
 *  \see JKQTBasePlotter, \ref jkqtpplotter_styling
 */
 class JKQTPLOTTER_LIB_EXPORT JKQTGeometricSpecificStyleProperties: public JKQTGraphsSpecificStyleProperties {
     Q_GADGET
 public:
     JKQTGeometricSpecificStyleProperties(const JKQTBasePlotterStyle& parent);
     JKQTGeometricSpecificStyleProperties(const JKQTBasePlotterStyle& parent, const JKQTGraphsSpecificStyleProperties& other);
     JKQTGeometricSpecificStyleProperties(JKQTPPlotStyleType type, const JKQTBasePlotterStyle& parent);
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
     void loadSettings(const QSettings &settings, const QString& group, const JKQTGraphsSpecificStyleProperties &defaultStyle);

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
     Qt::BrushStyle defaultFillStyle;


 };


 /** \brief Support Class for JKQTBasePlotter, which summarizes all properties that define the visual styling of annotation elements
 *  \ingroup jkqtpplotter_styling
 *
 *  \see JKQTBasePlotter, \ref jkqtpplotter_styling
 */
 class JKQTPLOTTER_LIB_EXPORT JKQTAnnotationsSpecificStyleProperties: public JKQTGeometricSpecificStyleProperties {
     Q_GADGET
 public:
     JKQTAnnotationsSpecificStyleProperties(const JKQTBasePlotterStyle& parent);
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

     /** \brief defines how to derive a text color for a new graph */
     JKQTPColorDerivationMode textColorDerivationMode;
     /** \brief default text color in the plot */
     QColor defaultTextColor;
     /** \brief default font size in the plot [pt] */
     double defaultFontSize;
     /** \brief default font name in the plot */
     QString defaultFontName;


 };



/** \brief Support Class for JKQTBasePlotter, which summarizes all properties that define the visual styling of a JKQTBasePlotter
 *  \ingroup jkqtpplotter_styling
 *
 *  \see JKQTBasePlotter, \ref jkqtpplotter_styling
 */
class JKQTPLOTTER_LIB_EXPORT JKQTGraphsBaseStyle {
        Q_GADGET
    public:
        JKQTGraphsBaseStyle(const JKQTBasePlotterStyle& parent);



        /** \brief loads the plot properties from a <a href="http://doc.qt.io/qt-5/qsettings.html")">QSettings</a> object
         *
         *  \param settings QSettings-object to read from
         *  \param group Group in the QSettings-object to read from
         *  \param defaultStyle If a setting cannot be found in \a settings, default values are taken from this object
         *                      By default, this is a default-constructed object
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
        JKQTGraphsSpecificStyleProperties barchartStyle;
        /** \brief styling options for boxplots graphs */
        JKQTGraphsSpecificStyleProperties boxplotStyle;
        /** \brief styling options for filled graphs */
        JKQTGraphsSpecificStyleProperties filledStyle;
        /** \brief styling options for impulses graphs */
        JKQTGraphsSpecificStyleProperties impulseStyle;
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
        QVector<Qt::BrushStyle> defaultGraphFillStyles;

};

#endif // JKQTPGRAPHSBASESTYLE_H
