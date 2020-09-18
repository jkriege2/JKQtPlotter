/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

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


#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtpbaseelements.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpimagetools.h"

#ifndef JKQTPGRAPHSBASEERROR_H
#define JKQTPGRAPHSBASEERROR_H


/*! \brief This mix-in class assembles all styling properties applicable to error indicators
    \ingroup jkqtplotter_basegraphserrors
    \ingroup jkqtplotter_basegraphs_stylemixins

    \image html errorbarstyles.png "line-graphs with different types of error indicators"

    \see \ref jkqtplotter_graphsgroup_classstructure_mixins

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPGraphErrorStyleMixin {
        Q_GADGET
    public:
        /** \brief class contructor */
        JKQTPGraphErrorStyleMixin();
        virtual ~JKQTPGraphErrorStyleMixin()=default;

        /** \brief initiaize the error indicator style (from the parent plotter) */
        void initErrorStyle(JKQTBasePlotter* parent, int &parentPlotStyle);



        /*! \copydoc m_errorBarCapSize */
        void setErrorBarCapSize(double __value);
        /*! \copydoc m_errorBarCapSize */
        double getErrorBarCapSize() const;



        /** \brief derive the properties of the error indicators from \a pen */
        void setErrorStyleFromPen(const JKQTBasePlotter::JKQTPPen& pen);

        /** \brief derive the colors of the error indicators from a given \a color */
        void setErrorColorFromGraphColor(QColor graphColor);



        /** \brief set the color of the error indicator (out)lines */
        void setErrorLineColor(const QColor & __value);
        /** \brief get the color of the error indicator (out)lines */
        QColor getErrorLineColor() const;
        /** \brief set the style of the error indicator (out)lines */
        void setErrorLineStyle(Qt::PenStyle  __value);
        /** \brief get the style of the error indicator (out)lines */
        Qt::PenStyle getErrorLineStyle() const;
        /** \brief set the width [pt] of the error indicator (out)lines */
        void setErrorLineWidth(double __value);
        /** \brief get the width [pt] of the error indicator (out)lines */
        double getErrorLineWidth() const;
        /** \brief sets the dash offset for a custom dash style of the error indicator (out)lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashOffset
         */
        void setErrorLineDashOffset(qreal offset);
        /** \brief returns the dash offset for a custom dash style of the error indicator (out)lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashOffset
         */
        qreal getErrorLineDashOffset() const;
        /** \brief sets the dash pattern for a custom dash style of the error indicator (out)lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashPattern
         */
        void setErrorLineDashPattern(const QVector<qreal> &pattern);
        /** \brief gets the dash pattern for a custom dash style of the error indicator (out)lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setDashPattern
         */
        QVector<qreal> getErrorLineDashPattern() const;
        /** \brief sets the join style of the error indicator (out)lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setJoinStyle
         */
        void setErrorLineJoinStyle(Qt::PenJoinStyle style);
        /** \brief returns the join style of the error indicator (out)lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setJoinStyle
         */
        Qt::PenJoinStyle getErrorLineJoinStyle() const;
        /** \brief sets the cap style of the error indicator (out)lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setCapStyle
         */
        void setErrorLineCapStyle(Qt::PenCapStyle style);
        /** \brief gets the cap style of the error indicator (out)lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setCapStyle
         */
        Qt::PenCapStyle getErrorLineCapStyle() const;
        /** \brief sets the brush used to fill the line area of the error indicator (out)lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setBrush
         */
        void setErrorLineBrush(const QBrush& style);
        /** \brief gets the brush used to fill the line area of the error indicator (out)lines
         *  \see https://doc.qt.io/qt-5/qpen.html#setBrush
         */
        QBrush getErrorLineBrush() const;


        /** \brief set the fill color of the error indicators */
        void setErrorFillColor(const QColor & __value);
        /** \brief get the fill color of the error indicators */
        QColor getErrorFillColor() const;
        /** \brief set the fill style of the error indicators */
        void setErrorFillStyle(Qt::BrushStyle  __value);
        /** \brief get the fill style of the error indicators */
        Qt::BrushStyle getErrorFillStyle() const;
        /** \brief set the color of the error indicator filling and sets fill style to Qt::TexturePattern */
        void setErrorFillTexture(const QPixmap & __value);
        /** \brief set the color of the error indicator filling and sets fill style to Qt::TexturePattern */
        void setErrorFillTexture(const QImage & __value);
        /** \brief set the color of the error indicator filling */
        QPixmap getErrorFillTexture() const;
        /** \brief set the color of the error indicator filling */
        QImage getErrorFillTextureImage() const;

        /** \brief set the filling of the error indicators to a gradient and sets fill style to a gradient setting */
        void setErrorFillGradient(const QGradient & __value);
        /** \brief get the gradient object of the error indicator filling */
        const QGradient *getErrorFillGradient() const;

        /** \brief sets an error indicator fill brush (overwrites all internal properties!) */
        void setErrorFillBrush(const QBrush& b);
        /** \brief sets an error indicator fill transformation */
        void setErrorFillTransform(const QTransform& b);



    protected:
        /** \brief error indicator line pen */
        QPen m_errorLinePen;
        /** \brief width of the error indicators' lines, given in pt */
        double m_errorLineWidth;


        /** \brief fill style of the graph */
        QBrush m_errorFillBrush;
        /** \brief last fill color of the graph  */
        QColor m_errorFillColor;


        /** \brief size of the error bar end markers [pt] */
        double m_errorBarCapSize;


        /** \brief constructs a QPen from the error indicator line styling properties */
        QPen getErrorLinePen(JKQTPEnhancedPainter &painter, const JKQTBasePlotter* parent) const;
        /** \brief constructs a QPen from the error indicator line styling properties, suitable for drawing rectangles with sharp edges */
        QPen getErrorLinePenForRects(JKQTPEnhancedPainter &painter, const JKQTBasePlotter* parent) const;

        /** \brief constructs a QBrush from the error indicator fill styling properties */
        QBrush getErrorFillBrush(JKQTPEnhancedPainter &painter, const JKQTBasePlotter *parent) const;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        void intPlotXYErrorIndicators(JKQTPEnhancedPainter& painter, const JKQTBasePlotter *parent, const JKQTPGraph* parentGraph, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPErrorPlotstyle xErrorStyle, JKQTPErrorPlotstyle yErrorStyle, int xErrorColumnLower=-1, int yErrorColumnLower=-1, bool xErrorSymmetric=true, bool yErrorSymmetric=true, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr) const;
        /** \brief this function can be used to set the color of the error indicators automatically
         *
         * return \c true and the colors to use, if applicable, the default implementation returns false */
        virtual bool intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter& painter, const JKQTBasePlotter* parent, const JKQTPGraph *parentGraph, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPErrorPlotstyle xErrorStyle, JKQTPErrorPlotstyle yErrorStyle, int index, QColor& errorLineColor, QColor& errorFillColor) const;
};



/*! \brief This class adds data fields for error indicators in x direction
    \ingroup jkqtplotter_basegraphserrors
    \see JKQTPYGraphErrorData, \ref jkqtplotter_graphsgroup_classstructure_mixins
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXGraphErrorData  {
        Q_GADGET
    public:
        /** \brief class contructor */
        JKQTPXGraphErrorData();
        virtual ~JKQTPXGraphErrorData()=default;

        /*! \copydoc xErrorSymmetric*/
        void setXErrorSymmetric(bool __value);
        /*! \copydoc xErrorSymmetric */
        bool getXErrorSymmetric() const;
        /*! \copydoc xErrorColumnLower */
        int getXErrorColumnLower() const;
        /*! \copydoc xErrorColumn */
        int getXErrorColumn() const;
        /*! \copydoc xErrorStyle */
        void setXErrorStyle(JKQTPErrorPlotstyle  __value);
        /*! \copydoc xErrorStyle */
        JKQTPErrorPlotstyle getXErrorStyle() const;
        /** \copydoc xErrorColumn */
        void setXErrorColumn(int __value);
        /** \copydoc xErrorColumnLower */
        void setXErrorColumnLower(int __value);


    protected:
        /** \brief the column that contains the error of the x-component of the datapoints */
        int xErrorColumn;
        /** \brief the column that contains the error of the x-component of the datapoints. This is used as the lower error length, if xErrorSymmetric \c ==false. */
        int xErrorColumnLower;
        /** \brief indicates whether the x-errors are symmetric (from one column only) */
        bool  xErrorSymmetric;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPErrorPlotstyle xErrorStyle;

        /** \brief returns the upper x-error for the i-th datapoint, read from datastore \a ds */
        virtual double getXErrorU(int i, JKQTPDatastore* ds) const;
        /** \brief returns the lower x-error for the i-th datapoint, read from datastore \a ds */
        virtual double getXErrorL(int i, JKQTPDatastore* ds) const;

};





/*! \brief This class adds data fields for error indicators in y direction
    \ingroup jkqtplotter_basegraphserrors
    \see JKQTPXGraphErrorData \ref jkqtplotter_graphsgroup_classstructure_mixins
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPYGraphErrorData  {
        Q_GADGET
    public:
        /** \brief class contructor */
        JKQTPYGraphErrorData();
        virtual ~JKQTPYGraphErrorData()=default;

        /*! \copydoc yErrorSymmetric */
        void setYErrorSymmetric(bool __value);
        /*! \copydoc yErrorSymmetric */
        bool getYErrorSymmetric() const;
        /*! \copydoc yErrorColumnLower */
        int getYErrorColumnLower() const;
        /*! \copydoc yErrorColumn */
        int getYErrorColumn() const;
        /*! \copydoc yErrorStyle */
        void setYErrorStyle(JKQTPErrorPlotstyle  __value);
        /*! \copydoc yErrorStyle */
        JKQTPErrorPlotstyle getYErrorStyle() const;
        /*! \copydoc yErrorColumn */
        void setYErrorColumn(int __value);
        /*! \copydoc yErrorColumnLower */
        void setYErrorColumnLower(int __value);


    protected:
        /** \brief the column that contains the error of the x-component of the datapoints */
        int yErrorColumn;
        /** \brief the column that contains the error of the x-component of the datapoints. This is used as the lower error length, if yErrorSymmetric \c ==false. */
        int yErrorColumnLower;
        /** \brief indicates whether the x-errors are symmetric (from one column only) */
        bool  yErrorSymmetric;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPErrorPlotstyle yErrorStyle;

        /** \brief returns the upper y-error for the i-th datapoint, read from datastore \a ds */
        virtual double getYErrorU(int i, JKQTPDatastore* ds) const;
        /** \brief returns the lower y-error for the i-th datapoint, read from datastore \a ds */
        virtual double getYErrorL(int i, JKQTPDatastore* ds) const;

};



/*! \brief This class is the base class for graphs that only display error indicators in x-direction
    \ingroup jkqtplotter_basegraphserrors
    \see JKQTPXGraphErrorData, JKQTPGraphErrorStyleMixin, \ref jkqtplotter_graphsgroup_classstructure_mixins
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXGraphErrors: public JKQTPXGraphErrorData, public JKQTPGraphErrorStyleMixin {
        Q_GADGET
    public:
        /** \brief class contructor */
        JKQTPXGraphErrors();
        virtual ~JKQTPXGraphErrors()=default;

        /** \brief returns true, if the error plots use the given column */
        virtual bool errorUsesColumn(int c) const ;

    protected:
        /** \brief draws the error indicators */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr) const ;

};



/*! \brief This class is the base class for graphs that only display error indicators in y-direction
    \ingroup jkqtplotter_basegraphserrors
    \see JKQTPYGraphErrorData, JKQTPGraphErrorStyleMixin, \ref jkqtplotter_graphsgroup_classstructure_mixins
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPYGraphErrors: public JKQTPYGraphErrorData, public JKQTPGraphErrorStyleMixin {
        Q_GADGET
    public:
        /** \brief class contructor */
        JKQTPYGraphErrors();
        virtual ~JKQTPYGraphErrors()=default;

        /** \brief returns true, if the error plots use the given column */
        virtual bool errorUsesColumn(int c) const ;

    protected:
        /** \brief draws the error indicators */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr) const ;

};





/*! \brief This class is the base class for graphs that only display error indicators in x- and y-direction
    \ingroup jkqtplotter_basegraphserrors
    \see JKQTPXGraphErrorData, JKQTPYGraphErrorData, JKQTPGraphErrorStyleMixin, \ref jkqtplotter_graphsgroup_classstructure_mixins
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXYGraphErrors: public JKQTPXGraphErrorData, public JKQTPYGraphErrorData, public JKQTPGraphErrorStyleMixin {
        Q_GADGET
    public:
        /** \brief class contructor */
        JKQTPXYGraphErrors();
        virtual ~JKQTPXYGraphErrors()=default;

        /** \brief returns true, if the error plots use the given column */
        virtual bool errorUsesColumn(int c) const ;

    protected:
        /** \brief draws the error indicators */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr) const ;

};









#endif // JKQTPGRAPHSBASEERROR_H
