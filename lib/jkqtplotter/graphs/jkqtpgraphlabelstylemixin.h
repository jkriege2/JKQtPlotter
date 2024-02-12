/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)

    

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


#ifndef jkqtpgraphlabelstylemixin_H
#define jkqtpgraphlabelstylemixin_H


#include <QString>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtplotter/jkqtplotter_configmacros.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"


class JKQTPlotter; // forward



/** \brief position of a label near a data-point, used by JKQTPGraphValueLabelStyleMixin
 *  \ingroup jkqtplotter_specialgraphs_stylemixins
 *
 *  \see JKQTPGraphValueLabelStyleMixin
 */
enum JKQTPGraphLabelPosition {
    JKQTPGLabelAboveData=0, /*!< \brief all labels always above the datapoint \image html JKQTPGLabelAboveData.png */
    JKQTPGLabelBelowData, /*!< \brief all labels always below the datapoint \image html JKQTPGLabelBelowData.png */
    JKQTPGLabelTowardsXAxis, /*!< \brief all labels between the datapoint and the x-axis the datapoint \image html JKQTPGLabelTowardsXAxis.png */
    JKQTPGLabelAwayFromXAxis, /*!< \brief all labels pointing away from the x-axis \image html JKQTPGLabelAwayFromXAxis.png */
    JKQTPGLabelRightHandSide, /*!< \brief all labels always on the right-hand side of the datapoint \image html JKQTPGLabelRightHandSide.png */
    JKQTPGLabelLeftHandSide, /*!< \brief all labels always on the left-hand side of the datapoint \image html JKQTPGLabelLeftHandSide.png */
    JKQTPGLabelTowardsYAxis, /*!< \brief all labels between the datapoint and the y-axis the datapoint \image html JKQTPGLabelTowardsYAxis.png */
    JKQTPGLabelAwayFromYAxis, /*!< \brief all labels pointing away from the y-axis \image html JKQTPGLabelAwayFromYAxis.png */
    JKQTPGLabelCenteredOnData, /*!< \brief graph label is drawn centered arond the data point \c (x,y) \image html JKQTPGLabelCenteredOnData.png */


    JKQTPGraphLabelDefault=JKQTPGLabelAwayFromXAxis,
};


/** \brief converts a JKQTPGraphLabelPosition variable into a human-readable string
 * \ingroup jkqtplotter_specialgraphs_stylemixins
 */
JKQTPLOTTER_LIB_EXPORT QString JKQTPGraphLabelPosition2String(JKQTPGraphLabelPosition pos);

/** \brief converts a String into a JKQTPGraphLabelPosition
 * \ingroup jkqtplotter_specialgraphs_stylemixins
 */
JKQTPLOTTER_LIB_EXPORT JKQTPGraphLabelPosition String2JKQTPGraphLabelPosition(const QString& pos);

/** \brief type of box surrounding a label near a data-point, used by JKQTPGraphValueLabelStyleMixin
 *  \ingroup jkqtplotter_specialgraphs_stylemixins
 *
 *  \see JKQTPGraphValueLabelStyleMixin
 */
enum JKQTPGraphLabelBoxType {
    JKQTPGLSimpleBox=0, /*!< \brief a simple (possibly rounded and outlined) box \image html JKQTPGLSimpleBox.png */
    JKQTPGLSimpleBoxAndLine=1, /*!< \brief a simple (possibly rounded and outlined) box, connected to the datapoint by a line \image html JKQTPGLSimpleBoxAndLine.png */

    JKQTPGraphLabelBoxDefault=JKQTPGLSimpleBox,
};


/** \brief converts a JKQTPGraphLabelBoxType variable into a human-readable string
 * \ingroup jkqtplotter_specialgraphs_stylemixins
 */
JKQTPLOTTER_LIB_EXPORT QString JKQTPGraphLabelBoxType2String(JKQTPGraphLabelBoxType pos);

/** \brief converts a String into a JKQTPGraphLabelBoxType
 * \ingroup jkqtplotter_specialgraphs_stylemixins
 */
JKQTPLOTTER_LIB_EXPORT JKQTPGraphLabelBoxType String2JKQTPGraphLabelBoxType(const QString& pos);




/*! \brief This Mix-In class provides setter/getter methods, storage and other facilities for value labels in graphs
    \ingroup jkqtplotter_specialgraphs_stylemixins

    in addition to all properties of JKQTPGraphTextStyleMixin for the actual label text,
    JKQTPGraphLineStyleMixin & JKQTPGraphFillStyleMixin for a box around the label,
    it also supports properties for positioning and further sryling the labels.

    supported properties:
      - padding from actual label to border
      - distance of label to actual datapoint
      - positioning of the label
      - styling for the box around the label
    .

    \image html jkqtpxygraphlabelsmixin.png

    \see JKQTPGraphTextStyleMixin, JKQTPGraphLineStyleMixin, JKQTPGraphFillStyleMixin, JKQTPXYGraphLabels
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGraphValueLabelStyleMixin: public JKQTPGraphTextStyleMixin, public JKQTPGraphLineStyleMixin, public JKQTPGraphFillStyleMixin {
#ifndef JKQTPLOTTER_WORKAROUND_QGADGET_BUG
    Q_GADGET
#endif
public:


    /** \brief class constructor */
    JKQTPGraphValueLabelStyleMixin(JKQTBasePlotter *parent);

    /** \brief initiaize the fill style (from the parent plotter) */
    void initValueLabelStyle(JKQTBasePlotter* parent, int &parentPlotStyle, JKQTPPlotStyleType styletype=JKQTPPlotStyleType::Default);

    virtual ~JKQTPGraphValueLabelStyleMixin();

    /** \copydoc m_labelOffset */
    void setLabelOffset(double r);
    /** \copydoc m_labelOffset */
    double getLabelOffset() const;
    /** \brief determines, whether to use getLabelOffset() or getLabelOffsetWithConnector() (or something else) and returns it */
    double getUsedLabelOffset() const;

    /** \copydoc m_labelOffsetWithConnector */
    void setLabelOffsetWithConnector(double r);
    /** \copydoc m_labelOffsetWithConnector */
    double getLabelOffsetWithConnector() const;

    /** \copydoc m_labelBoxRounding */
    void setLabelBoxRounding(double r);
    /** \copydoc m_labelBoxRounding */
    double getLabelBoxRounding() const;

    /** \copydoc m_labelXPadding */
    void setLabelXPadding(double r);
    /** \copydoc m_labelXPadding */
    double getLabelXPadding() const;


    /** \copydoc m_labelYPadding */
    void setLabelYPadding(double r);
    /** \copydoc m_labelYPadding */
    double getLabelYPadding() const;

    /** \copydoc m_labelPosition */
    void setLabelPosition(JKQTPGraphLabelPosition r);
    /** \copydoc m_labelPosition */
    JKQTPGraphLabelPosition getLabelPosition() const;
    /** \brief determines whether a label is positioned left of the mininmum, or right of the maxinmum datapoint (\c true )  */
    bool isLabelPositioningGrowingInX() const;
    /** \brief determines whether a label is positioned below the mininmum, or above the maxinmum datapoint (\c true )  */
    bool isLabelPositioningGrowingInY() const;

    /** \copydoc m_drawLabelBoxFrame */
    void setDrawLabelBoxFrame(bool r);
    /** \copydoc m_drawLabelBoxFrame */
    bool drawsLabelBoxFrame() const;

    /** \copydoc m_labelBoxType */
    void setLabelBoxType(JKQTPGraphLabelBoxType r);
    /** \copydoc m_labelBoxType */
    JKQTPGraphLabelBoxType getLabelBoxType() const;

    /** \brief draws a label, including its box
     *
     *  \brief painter the JKQTPEnhancedPainter to use  for drawing
     *  \brief xDataPixel position of the labeled datapoint in (screen) pixels
     *  \brief xData x- and y-coordinate of the datapoint (needed for some JKQTPGraphLabelPosition)
     *  \brief contents the text to be rendered
     *  \brief parent the JKQTPBasePlotter in whos context we are drawing (e.g. needed to render \a contents )
     *
     */
    void drawLabel(JKQTPEnhancedPainter& painter, const QPointF& xDataPixel, const QPointF& xData, const QString& contents, JKQTBasePlotter *parent) const;



#ifndef JKQTPLOTTER_WORKAROUND_QGADGET_BUG
    Q_PROPERTY(double labelOffset MEMBER m_labelOffset READ getLabelOffset WRITE setLabelOffset)
    Q_PROPERTY(double labelOffsetWithConnector MEMBER m_labelOffsetWithConnector READ getLabelOffsetWithConnector WRITE setLabelOffsetWithConnector)
    Q_PROPERTY(double labelBoxRounding MEMBER m_labelBoxRounding READ getLabelBoxRounding WRITE setLabelBoxRounding)
    Q_PROPERTY(double labelXPadding MEMBER m_labelXPadding READ getLabelXPadding WRITE setLabelXPadding)
    Q_PROPERTY(double labelYPadding MEMBER m_labelYPadding READ getLabelYPadding WRITE setLabelYPadding)
    Q_PROPERTY(JKQTPGraphLabelPosition labelPosition MEMBER m_labelPosition READ getLabelPosition WRITE setLabelPosition)
    Q_PROPERTY(bool drawLabelBoxFrame MEMBER m_drawLabelBoxFrame READ drawsLabelBoxFrame WRITE setDrawLabelBoxFrame)
    Q_PROPERTY(JKQTPGraphLabelBoxType labelBoxType MEMBER m_labelBoxType READ getLabelBoxType WRITE setLabelBoxType)
#endif

protected:
    /** \brief describes details about the geometry of a label, used by calcLabelGeometry() */
    struct LabelGeometry {
        inline LabelGeometry():
            labelOffsetPx(0.0),
            padX(0.0),
            padY(0.0),
            lw(0.0),
            label(""),
            textSize(),
            textRect(),
            boxRect(),
            boxpos(BoxTop)
            { };
        double labelOffsetPx;
        double padX;
        double padY;
        double lw;
        QString label;
        JKQTMathTextNodeSize textSize;
        QRectF textRect;
        QRectF boxRect;
        enum BoxPos {
            BoxLeft,
            BoxRight,
            BoxTop,
            BoxBottom,
            BoxCentered
        };
        BoxPos boxpos;
    };

    /** \brief calculate everything that is necessary to draw a label, including its box
     *
     *  \brief painter the JKQTPEnhancedPainter to use  for drawing
     *  \brief xDataPixel position of the labeled datapoint in (screen) pixels
     *  \brief xData x- and y-coordinate of the datapoint (needed for some JKQTPGraphLabelPosition)
     *  \brief contents the text to be rendered
     *  \brief parent the JKQTPBasePlotter in whos context we are drawing (e.g. needed to render \a contents )
     *
     */
    LabelGeometry calcLabelGeometry(JKQTPEnhancedPainter& painter, const QPointF& xDataPixel, const QPointF& xData, const QString& contents, JKQTBasePlotter *parent) const;

private:
    /** \brief offset of the box rectangle to the actual data point location [pt], this is used for simple boxes and is a rather close distance (e.g. JKQTPGLSimpleBox)
     *
     *  \image html jkqtpxygraphlabelsmixin.png
     *
     *  \see setLabelOffset(), getLabelOffset(), setLabelOffsetWithConnector(), getLabelOffsetWithConnector()
     */
    double m_labelOffset;
    /** \brief offset of the box rectangle to the actual data point location [pt], this variant is used when a visible connector is shown (e.g. JKQTPGLSimpleBoxAndLine)
     *
     *  \image html jkqtpxygraphlabelsmixin.png
     *
     *  \see setLabelOffsetWithConnector(), getLabelOffsetWithConnector(), setLabelOffset(), getLabelOffset()
     */
    double m_labelOffsetWithConnector;
    /** \brief rounding radius of the box rectangle (<=0 -> no rounded rectangle) [pt] */
    double m_labelBoxRounding;
    /** \brief padding in x-direction between label and surrounding box [pt]
     *
     *  \image html jkqtpxygraphlabelsmixin.png
     */
    double m_labelXPadding;
    /** \brief padding in y-direction between label and surrounding box [pt] *
     *
     *  \image html jkqtpxygraphlabelsmixin.png
     */
    double m_labelYPadding;
    /** \brief indicates whether to draw a frame around the box *
     *
     *  \image html jkqtpxygraphlabelsmixin.png
     */
    bool m_drawLabelBoxFrame;
    /** \brief position of the label
     *
     *  \see JKQTPGraphLabelPosition for details on available options
     */
    JKQTPGraphLabelPosition m_labelPosition;
    /** \brief type or style of the box surrounding the label text
     *
     *  \see JKQTPGraphLabelBoxType for details on available options
     */
    JKQTPGraphLabelBoxType m_labelBoxType;
};




/** \brief display mode for the (axis) labels
 * \ingroup jkqtplotter_specialgraphs_stylemixins */
enum JKQTPGraphLabelConverterType {
    JKQTPGLCdecimal, /*!< \brief print the numbers in decimal notation, without using exponent characters e.g. \c "0.00123"  */
    JKQTPGLCscientific, /*!< \brief print the numbers in scientific notation, e.g. \c "1.23e-4"  */
    JKQTPGLCexponentCharacter, /*!< \brief print the numbers and show a unit character, i.e. 5&mu; for \f$ 5\cdot 10^{-6} \f$ , \c 3k for \f$ 3\cdot 10^3 \f$ ...  */
    JKQTPGLCexponent, /*!< \brief show numbers in exponential for, e.g. \f$ 3\cdot 10^5 \f$ ...  */
    JKQTPGLCdatetime, /*!< \brief show numbers as times  */

    JKQTPGLDefaultConverter=JKQTPGLCexponent,
    };


/*! \brief This Mix-In class provides setter/getter methods, and tools for x- and y-value label formatting (i.e. double to string conversion)
    \ingroup jkqtplotter_specialgraphs_stylemixins

    \see JKQTPXYGraphLabels
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXYLabelsGeneratorMixin {
#ifndef JKQTPLOTTER_WORKAROUND_QGADGET_BUG
    Q_GADGET
#endif
public:

    /** \brief class constructor */
    explicit JKQTPXYLabelsGeneratorMixin();

    virtual ~JKQTPXYLabelsGeneratorMixin();



    /** \copydoc m_xDefaultPrecision */
    int getXDefaultPrecision() const;
    /** \copydoc m_xDefaultPrecision */
    void setXDefaultPrecision(int v);
    /** \copydoc m_yDefaultPrecision */
    int getYDefaultPrecision() const;
    /** \copydoc m_yDefaultPrecision */
    void setYDefaultPrecision(int v);

    /** \copydoc m_xDefaultConverter */
    JKQTPGraphLabelConverterType getXDefaultConverter() const;
    /** \copydoc m_xDefaultConverter */
    void setXDefaultConverter(JKQTPGraphLabelConverterType v);
    /** \copydoc m_yDefaultConverter */
    JKQTPGraphLabelConverterType getYDefaultConverter() const;
    /** \copydoc m_yDefaultConverter */
    void setYDefaultConverter(JKQTPGraphLabelConverterType v);


    /** \copydoc m_xValueLabelFormat */
    QString getXValueLabelFormat() const;
    /** \copydoc m_xValueLabelFormat */
    void setXValueLabelFormat(const QString& v);
    /** \copydoc m_yValueLabelFormat */
    QString getYValueLabelFormat() const;
    /** \copydoc m_yValueLabelFormat */
    void setYValueLabelFormat(const QString& v);
    /** \copydoc m_xyValueLabelFormat */
    QString getXYValueLabelFormat() const;
    /** \copydoc m_xyValueLabelFormat */
    void setXYValueLabelFormat(const QString& v);


    /** \copydoc m_xBelowIsZero */
    double getXBelowIsZero() const;
    /** \copydoc m_xBelowIsZero */
    void setXBelowIsZero(double v);
    /** \copydoc m_yBelowIsZero */
    double getYBelowIsZero() const;
    /** \copydoc m_yBelowIsZero */
    void setYBelowIsZero(double v);


    /** \copydoc m_xMinNoExponent */
    double getXMinNoExponent() const;
    /** \copydoc m_xMinNoExponent */
    void setXMinNoExponent(double v);
    /** \copydoc m_yMinNoExponent */
    double getYMinNoExponent() const;
    /** \copydoc m_yMinNoExponent */
    void setYMinNoExponent(double v);


    /** \copydoc m_xMaxNoExponent */
    double getXMaxNoExponent() const;
    /** \copydoc m_xMaxNoExponent */
    void setXMaxNoExponent(double v);
    /** \copydoc m_yMaxNoExponent */
    double getYMaxNoExponent() const;
    /** \copydoc m_yMaxNoExponent */
    void setYMaxNoExponent(double v);



    /** \copydoc m_xDateTimeFormat */
    QString getXDateTimeFormat() const;
    /** \copydoc m_xDateTimeFormat */
    void setXDateTimeFormat(const QString& v);
    /** \copydoc m_yDateTimeFormat */
    QString getYDateTimeFormat() const;
    /** \copydoc m_yDateTimeFormat */
    void setYDateTimeFormat(const QString& v);


protected:
    /** \brief default label generator for x-value only */
    QString generateDefaultXLabel(double x, double y, int index) const;
    /** \brief default label generator for y-value only */
    QString generateDefaultYLabel(double x, double y, int index) const;
    /** \brief default label generator for x- and y-value */
    QString generateDefaultXYLabel(double x, double y, int index) const;
    /** \brief converts \a x to a string, using several of the formatting properties set in this class for x-values */
    QString xValToString(double x) const;
    /** \brief converts \a y to a string, using several of the formatting properties set in this class for y-values */
    QString yValToString(double y) const;
    /** \brief converts \a x to a QString, using the provided options */
    static QString valToString(double x, JKQTPGraphLabelConverterType m_xDefaultConverter, int m_xDefaultPrecision, double m_xBelowIsZero, double m_xMinNoExponent, double m_xMaxNoExponent, const QString& m_xDateTimeFormat);
private:
    /** \brief format string for the x-label, use \c %1 as placeholder for the numeric value */
    QString m_xValueLabelFormat;
    /** \brief format string for the y-label, use \c %1 as placeholder for the numeric value */
    QString m_yValueLabelFormat;
    /** \brief format string for the x/y-label, use \c %1 as placeholder for the numeric value of x and \c %2 for y */
    QString m_xyValueLabelFormat;
    /** \brief type of number to string conversion for x-values */
    JKQTPGraphLabelConverterType m_xDefaultConverter;
    /** \brief type of number to string conversion for y-values */
    JKQTPGraphLabelConverterType m_yDefaultConverter;
    /** \brief default precision for x-labels */
    int m_xDefaultPrecision;
    /** \brief default precision for x-labels */
    int m_yDefaultPrecision;
    /** \brief x-values below this value are treated as exactly 0 */
    double m_xBelowIsZero;
    /** \brief y-values below this value are treated as exactly 0 */
    double m_yBelowIsZero;
    /** \brief x-values below this value will be shown in exponent notation, above in decimal notation */
    double m_xMinNoExponent;
    /** \brief y-values below this value will be shown in exponent notation, above in decimal notation */
    double m_yMinNoExponent;
    /** \brief x-values above this value will be shown in exponent notation, below in decimal notation */
    double m_xMaxNoExponent;
    /** \brief y-values above this value will be shown in exponent notation, below in decimal notation */
    double m_yMaxNoExponent;
    /** \brief format string for datetime -> string conversion of x-values */
    QString m_xDateTimeFormat;
    /** \brief format string for datetime -> string conversion of y-value */
    QString m_yDateTimeFormat;
};



#endif // jkqtpgraphlabelstylemixin_H
