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

#ifndef jkqtpgraphsviolinplotstylingmixins_H
#define jkqtpgraphsviolinplotstylingmixins_H



#include <QString>
#include <QPainter>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtplotter_configmacros.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"




/*! \brief Styling Mix-In for Violinplots
    \ingroup jkqtplotter_basegraphs_stylemixins


    \image html JKQTPViolinplotHorizontalElement_elements.png

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGraphViolinplotStyleMixin: public JKQTPGraphLineStyleMixin, public JKQTPGraphFillStyleMixin {
#ifndef JKQTPLOTTER_WORKAROUND_QGADGET_BUG
      Q_GADGET
#endif
    public:


        /** \brief class constructor */
        JKQTPGraphViolinplotStyleMixin();

        void initViolinplotStyle(JKQTBasePlotter* parent, int &parentPlotStyle);
        /** \copydoc violinWidthAbsolute */
        void setViolinWidthAbsolute(double __value);
        /** \copydoc violinWidthAbsolute */
        double getViolinWidthAbsolute() const;

        /** \copydoc relativeWhiskerWidth */
        void setRelativeWhiskerWidth(double __value);
        /** \copydoc relativeWhiskerWidth */
        double getRelativeWhiskerWidth() const;


        /** \brief set the line style of whisker lines  */
        void setWhiskerLineStyle(Qt::PenStyle  __value);
        /** \brief get the line style of whisker lines  */
        Qt::PenStyle getWhiskerLineStyle() const;

        /** \brief set the width [pt] of whisker lines  */
        void setWhiskerLineWidth(double __value);
        /** \brief get the  width [pt] of whisker lines  */
        double getWhiskerLineWidth() const;

        /** \brief set the color of whisker lines  */
        void setWhiskerLineColor(QColor __value);
        /** \brief get the color of whisker lines  */
        QColor getWhiskerLineColor() const;


        /** \brief sets the dash offset for a custom dash style of whisker lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashOffset
         */
        void setWhiskerLineDashOffset(qreal offset);
        /** \brief returns the dash offset for a custom dash style of whisker lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashOffset
         */
        qreal getWhiskerLineDashOffset() const;
        /** \brief sets the dash pattern for a custom dash style of whisker lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashPattern
         */
        void setWhiskerLineDashPattern(const QVector<qreal> &pattern);
        /** \brief gets the dash pattern for a custom dash style of whisker lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashPattern
         */
        QVector<qreal> getWhiskerLineDashPattern() const;
        /** \brief sets the join style of whisker lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setJoinStyle
         */
        void setWhiskerLineJoinStyle(Qt::PenJoinStyle style);
        /** \brief returns the join style of whisker lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setJoinStyle
         */
        Qt::PenJoinStyle getWhiskerLineJoinStyle() const;
        /** \brief sets the cap style of whisker lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setCapStyle
         */
        void setWhiskerLineCapStyle(Qt::PenCapStyle style);
        /** \brief gets the cap style of whisker lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setCapStyle
         */
        Qt::PenCapStyle getWhiskerLineCapStyle() const;
        /** \brief sets the brush used to fill the line area of whisker lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setBrush
         */
        void setWhiskerLineBrush(const QBrush& style);
        /** \brief gets the brush used to fill the line area of whisker lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setBrush
         */
        QBrush getWhiskerLineBrush() const;

        /** \brief build a pen to be used for drawing whiskers */
        QPen getWhiskerPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const;



        /** \brief set the line style of whisker cap lines  */
        void setWhiskerCapLineStyle(Qt::PenStyle  __value);
        /** \brief get the line style of whisker cap lines  */
        Qt::PenStyle getWhiskerCapLineStyle() const;

        /** \brief set the width [pt] of whisker cap lines  */
        void setWhiskerCapLineWidth(double __value);
        /** \brief get the  width [pt] of whisker cap lines  */
        double getWhiskerCapLineWidth() const;

        /** \brief set the color of whisker cap lines  */
        void setWhiskerCapLineColor(QColor __value);
        /** \brief get the color of whisker cap lines  */
        QColor getWhiskerCapLineColor() const;


        /** \brief sets the dash offset for a custom dash style of whisker cap lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashOffset
         */
        void setWhiskerCapLineDashOffset(qreal offset);
        /** \brief returns the dash offset for a custom dash style of whisker cap lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashOffset
         */
        qreal getWhiskerCapLineDashOffset() const;
        /** \brief sets the dash pattern for a custom dash style of whisker cap lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashPattern
         */
        void setWhiskerCapLineDashPattern(const QVector<qreal> &pattern);
        /** \brief gets the dash pattern for a custom dash style of whisker cap lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashPattern
         */
        QVector<qreal> getWhiskerCapLineDashPattern() const;
        /** \brief sets the join style of whisker cap lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setJoinStyle
         */
        void setWhiskerCapLineJoinStyle(Qt::PenJoinStyle style);
        /** \brief returns the join style of whisker cap lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setJoinStyle
         */
        Qt::PenJoinStyle getWhiskerCapLineJoinStyle() const;
        /** \brief sets the cap style of whisker cap lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setCapStyle
         */
        void setWhiskerCapLineCapStyle(Qt::PenCapStyle style);
        /** \brief gets the cap style of whisker cap lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setCapStyle
         */
        Qt::PenCapStyle getWhiskerCapLineCapStyle() const;
        /** \brief sets the brush used to fill the line area of whisker cap lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setBrush
         */
        void setWhiskerCapLineBrush(const QBrush& style);
        /** \brief gets the brush used to fill the line area of whisker cap lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setBrush
         */
        QBrush getWhiskerCapLineBrush() const;

        /** \brief build a pen to be used for drawing whisker caps */
        QPen getWhiskerCapPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const;



        /** \brief set the line style of median lines  */
        void setMedianLineStyle(Qt::PenStyle  __value);
        /** \brief get the line style of median lines  */
        Qt::PenStyle getMedianLineStyle() const;

        /** \brief set the width [pt] of median lines  */
        void setMedianLineWidth(double __value);
        /** \brief get the  width [pt] of median lines  */
        double getMedianLineWidth() const;

        /** \brief set the color of median lines  */
        void setMedianLineColor(QColor __value);
        /** \brief get the color of median lines  */
        QColor getMedianLineColor() const;


        /** \brief sets the dash offset for a custom dash style of median lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashOffset
         */
        void setMedianLineDashOffset(qreal offset);
        /** \brief returns the dash offset for a custom dash style of median lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashOffset
         */
        qreal getMedianLineDashOffset() const;
        /** \brief sets the dash pattern for a custom dash style of median lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashPattern
         */
        void setMedianLineDashPattern(const QVector<qreal> &pattern);
        /** \brief gets the dash pattern for a custom dash style of median lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashPattern
         */
        QVector<qreal> getMedianLineDashPattern() const;
        /** \brief sets the join style of median lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setJoinStyle
         */
        void setMedianLineJoinStyle(Qt::PenJoinStyle style);
        /** \brief returns the join style of median lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setJoinStyle
         */
        Qt::PenJoinStyle getMedianLineJoinStyle() const;
        /** \brief sets the cap style of median lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setCapStyle
         */
        void setMedianLineCapStyle(Qt::PenCapStyle style);
        /** \brief gets the cap style of median lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setCapStyle
         */
        Qt::PenCapStyle getMedianLineCapStyle() const;
        /** \brief sets the brush used to fill the line area of median lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setBrush
         */
        void setMedianLineBrush(const QBrush& style);
        /** \brief gets the brush used to fill the line area of median lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setBrush
         */
        QBrush getMedianLineBrush() const;

        /** \brief build a pen to be used for drawing medians */
        QPen getMedianPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter *parent) const;





        /** \brief set the line style of Mean lines  */
        void setMeanLineStyle(Qt::PenStyle  __value);
        /** \brief get the line style of Mean lines  */
        Qt::PenStyle getMeanLineStyle() const;
        /** \brief sets the dash offset for a custom dash style of Mean lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashOffset
         */
        void setMeanLineDashOffset(qreal offset);
        /** \brief returns the dash offset for a custom dash style of Mean lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashOffset
         */
        qreal getMeanLineDashOffset() const;
        /** \brief sets the dash pattern for a custom dash style of Mean lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashPattern
         */
        void setMeanLineDashPattern(const QVector<qreal> &pattern);
        /** \brief gets the dash pattern for a custom dash style of Mean lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashPattern
         */
        QVector<qreal> getMeanLineDashPattern() const;
        /** \brief set the type of the symbol for the mean */
        void setMeanSymbolType(JKQTPGraphSymbols __value);
        /** \brief get the type of the symbol for the mean */
        JKQTPGraphSymbols getMeanSymbolType() const;

        /** \brief set the size (=diameter in pt) of the symbol for the mean (in pt) */
        void setMeanSize(double __value);
        /** \brief get the size (=diameter in pt) of the symbol for the mean (in pt) */
        double getMeanSize() const;

        /** \brief set the color of the symbol for the mean, or mean line */
        void setMeanColor(const QColor & __value);
        /** \brief set the color of the symbol for the mean, or mean line */
        QColor getMeanColor() const;

        /** \brief set the color of filling of the symbol for the mean */
        void setMeanFillColor(const QColor & __value);
        /** \brief set the color of filling of the symbol for the mean */
        QColor getMeanFillColor() const;

        /** \brief set the line width of the symbol for the mean outline, or mean line (in pt) */
        void setMeanLineWidth(double __value);
        /** \brief get the line width of the symbol for the mean outline, or mean line (in pt) */
        double getMeanLineWidth() const;

        /** \brief defines how to draw the violin graph */
        enum ViolinStyle {
            SmoothViolin,  /*!< \brief connect all violin points by lines, resulting in a (nearly) smooth curve \image html JKQTPGraphViolinplot_SmoothViolin.png */
            StepViolin,    /*!< \brief connect violin points by a steped line, but fully filled \image html JKQTPGraphViolinplot_StepViolin.png  */
            BoxViolin,     /*!< \brief violin datapoints are drawn like a boxplot \image html JKQTPGraphViolinplot_BoxViolin.png  */
        };

        /** \brief returns the style of the violin plot */
        ViolinStyle getViolinStyle() const;
        /** \brief set the style of the violin plot */
        void setViolinStyle(ViolinStyle style);

        /** \brief defines where to draw the violin graph with respect to the violin center position */
        enum ViolinPositionMode {
            ViolinLeft,                 /*!< \brief draw violin on the left hand side only (for vertical violins) \image html JKQTPGraphViolinplot_ViolinLeft.png */
            ViolinBottom=ViolinLeft,    /*!< \brief draw violin on the bottom side only (for horizontal violins) \image html JKQTPGraphViolinplot_ViolinBottom.png */
            ViolinRight,                /*!< \brief draw violin on the right hand side only (for vertical violins) \image html JKQTPGraphViolinplot_ViolinRight.png */
            ViolinTop=ViolinRight,      /*!< \brief draw violin on the top side only (for horizontal violins) \image html JKQTPGraphViolinplot_ViolinTop.png */
            ViolinBoth                  /*!< \brief draw violin on the left+right or top+bottom side
                                                    \image html JKQTPGraphViolinplot_ViolinBoth.png
                                                    \image html JKQTPGraphViolinplot_ViolinHBoth.png */
        };

        /** \brief returns the position mode of the violin plot */
        ViolinPositionMode getViolinPositionMode() const;
        /** \brief set the position mode of the violin plot */
        void setViolinPositionMode(ViolinPositionMode positionMode);


        /** \brief constructs a QPen from the line styling properties to draw the mean line */
        QPen getMeanLinePen(JKQTPEnhancedPainter &painter, JKQTBasePlotter* parent) const;
        /** \brief constructs a QPen from the line styling properties to draw the mean symbol */
        QPen getMeanSymbolPen(JKQTPEnhancedPainter &painter, JKQTBasePlotter* parent) const;
        /** \brief constructs a QPen from the line styling properties */
        QBrush getMeanSymbolBrush(JKQTPEnhancedPainter &painter, JKQTBasePlotter* parent) const;

        /** \brief set the color of the graph (colors all elements, based on the given color \a c ) */
        void setViolinplotColor(QColor c, JKQTBasePlotter *parent);

        /** \brief set the color of the graph (colors all elements, based on the given color \a c , sets background colors from \a bc ) */
        void setViolinplotColor(QColor c, QColor bc, JKQTBasePlotter *parent);
    protected:
        /*! \brief plot a symbol at location x,y (in painter coordinates), using the current style

            \param parent parent JKQTBasePlotter of the graph that uses this mix-in (used e.g. for line-width transformation)
            \param painter the <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a> to draw to
            \param x x-coordinate of the symbol center
            \param y y-coordinate of the symbol center
         */
        void plotStyledMeanSymbol(JKQTBasePlotter* parent, JKQTPEnhancedPainter &painter, double x, double y) const;
        /** \brief draws a vertical boxplot, with all coordinates/sizes given in coordinates of the given painter,
         *         using the style properties declared in this class. Provide a parameter with \c JKQTP_NAN of you
         *         don't want it to be drawn, or don't know its value
         *
         *  \param parent parent JKQTBasePlotter of the graph that uses this mix-in (used e.g. for line-width transformation)
         *  \param painter the <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a> to draw to
         *  \param xp x-coordinate of the boxplot center
         *  \param xpleft x-coordinate of the boxplot box left edge
         *  \param xpright x-coordinate of the boxplot box right edge
         *  \param violin_cat category-dataset for the violin (y-values of KDE/histogram/...)
         *  \param violin_freq frequency/count-dataset for the violin (y-values/frequencies of KDE/histogram/...)
         *  \param minp y-coordinate of the minimum (lower whisker)
         *  \param medianp y-coordinate the median
         *  \param maxp y-coordinate of the maximum (upper whisker)
         *  \param meanp y-coordinate of the mean (symbol or line)
         */
        void plotVerticalViolinplot(JKQTBasePlotter* parent, JKQTPEnhancedPainter &painter, double xp, double xpleft, double xpright, const QVector<double> &violin_cat, const QVector<double>& violin_freq, double minp, double medianp, double maxp, double meanp=JKQTP_NAN) const;

        /** \brief draws a horizontal boxplot, with all coordinates/sizes given in coordinates of the given painter,
         *         using the style properties declared in this class. Provide a parameter with \c JKQTP_NAN of you
         *         don't want it to be drawn, or don't know its value
         *
         *  \param parent parent JKQTBasePlotter of the graph that uses this mix-in (used e.g. for line-width transformation)
         *  \param painter the <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a> to draw to
         *  \param yp y-coordinate of the boxplot center
         *  \param ypbottom y-coordinate of the boxplot box bottom edge
         *  \param yptop y-coordinate of the boxplot box top edge
         *  \param violin_cat category-dataset for the violin (x-values of KDE/histogram/...)
         *  \param violin_freq frequency/count-dataset for the violin (y-values/frequencies of KDE/histogram/...)
         *  \param minp x-coordinate of the minimum (lower whisker)
         *  \param medianp x-coordinate the median
         *  \param maxp x-coordinate of the maximum (upper whisker)
         *  \param meanp x-coordinate of the mean (symbol or line)
         */
        void plotHorizontalViolinplot(JKQTBasePlotter* parent, JKQTPEnhancedPainter &painter, double yp, double ypbottom, double yptop, const QVector<double>& violin_cat, const QVector<double>& violin_freq, double minp, double medianp, double maxp, double meanp=JKQTP_NAN) const;

        /** \brief draw a small, stylized, vertical symbol into \a rect that symbolizes a boxplot, e.g. in a plot legend */
        void plotVerticalKeyMarker(JKQTBasePlotter* parent, JKQTPEnhancedPainter &painter, const QRectF& rect) const;

        /** \brief draw a small, stylized, horizontal symbol into \a rect that symbolizes a boxplot, e.g. in a plot legend */
        void plotHorizontalKeyMarker(JKQTBasePlotter* parent, JKQTPEnhancedPainter &painter, const QRectF& rect) const;
    private:
        /** \brief which symbol to use for the datapoints */
        JKQTPGraphSymbols m_meanSymbolType;
        /** \brief size (diameter in pt) of the symbol for the data points, given in pt */
        double m_meanSymbolSize;
        /** \brief outline color of the symbol or line pen of the mean-line */
        QPen m_meanSymbolLinePen;
        /** \brief color of the symbol filling */
        QColor m_meanSymbolFillColor;
        /** \brief width (in pt) of the lines used to plot the symbol for the data points, given in pt */
        double m_meanSymbolLineWidth;
        /** \brief line style of the whisker lines */
        QPen m_whiskerLinePen;
        /** \brief line width (in pt) of the whisker lines */
        double whiskerLineWidth;
        /** \brief line style of the whisker cap lines */
        QPen m_whiskerCapLinePen;
        /** \brief line width (in pt) of the whisker cap lines */
        double whiskerCapLineWidth;
        /** \brief line style of the median lines */
        QPen m_medianLinePen;
        /** \brief line width (in pt) of the median lines */
        double medianLineWidth;
        /** \brief width of box in pt.
         *
         *  \note If several boxplots are drawn, the width is typically calculated,
         *        based on a relative width and the position distances, see JKQTPBoxplotVerticalGraph::boxWidthRelative  */
        double violinWidthAbsolute;
        /** \brief width of the whiskers, relative to the box width (default: 0.5) */
        double relativeWhiskerWidth;

        /** \brief the style of the violin plot */
        ViolinStyle m_violinStyle;

        /** \brief the position mode of the violin plot */
        ViolinPositionMode m_violinPositionMode;

};





#endif // jkqtpgraphsviolinplotstylingmixins_H
