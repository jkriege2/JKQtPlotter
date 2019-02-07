/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

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
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplottertools/jkqtpimagetools.h"

#ifndef JKQTPGRAPHSBASEERROR_H
#define JKQTPGRAPHSBASEERROR_H


/*! \brief Descendents of this class add data fields for error indicators and methods to plot them to a class.
    \ingroup jkqtplotter_basegraphserrors

    This class is meant to be used with multiple inheritance. I.e. if you implemented some kind of plot
    you may derive an error plot class in which you will have to overwrite the JKQTPGraph::drawErrorsBefor()
    or drawErrorsAfter() so it calls the plotErrorIndicators() method from this class. In addition this class
    will add some public datamemebers and methods to your class that allow to specify the properties of the
    error indicators (plot properties: color, width, ... and columns for the data).

    \image html jkqtplotter_simpletest_errorbarstyles.png "line-graphs with different types of error indicators"


    \see \ref jkqtplotter_graphsgroup_classstructure_mixins

 */
class JKQTP_LIB_EXPORT JKQTPGraphErrors {
    public:
        /** \brief class contructor */
        JKQTPGraphErrors(QColor graphColor, JKQTBasePlotter* basePlotter=nullptr);
        virtual ~JKQTPGraphErrors()=default;

        /** \brief derive the properties of the error indicators from \a pen */
        void setErrorStyleFromPen(const JKQTBasePlotter::JKQTPPen& pen);

        /*! \copydoc errorColor
            \see see errorColor for details */ 
        inline virtual void setErrorColor(const QColor & __value)  
        {
            this->errorColor = __value;
        } 
        /*! \copydoc errorColor
            \see see errorColor for details */ 
        inline virtual QColor getErrorColor() const  
        {
            return this->errorColor; 
        }
        /*! \copydoc errorStyle
            \see see errorStyle for details */ 
        inline virtual void setErrorStyle(const Qt::PenStyle & __value)  
        {
            this->errorStyle = __value;
        } 
        /*! \copydoc errorStyle
            \see see errorStyle for details */ 
        inline virtual Qt::PenStyle getErrorStyle() const  
        {
            return this->errorStyle; 
        }
        /*! \copydoc errorWidth
            \see see errorWidth for details */ 
        inline virtual void setErrorWidth(double __value)
        {
            this->errorWidth = __value;
        } 
        /*! \copydoc errorWidth
            \see see errorWidth for details */ 
        inline virtual double getErrorWidth() const  
        {
            return this->errorWidth; 
        }
        /*! \copydoc errorFillColor
            \see see errorFillColor for details */ 
        inline virtual void setErrorFillColor(const QColor & __value)  
        {
            this->errorFillColor = __value;
        } 
        /*! \copydoc errorFillColor
            \see see errorFillColor for details */ 
        inline virtual QColor getErrorFillColor() const  
        {
            return this->errorFillColor; 
        }
        /*! \copydoc errorFillStyle
            \see see errorFillStyle for details */ 
        inline virtual void setErrorFillStyle(const Qt::BrushStyle & __value)  
        {
            this->errorFillStyle = __value;
        } 
        /*! \copydoc errorFillStyle
            \see see errorFillStyle for details */ 
        inline virtual Qt::BrushStyle getErrorFillStyle() const  
        {
            return this->errorFillStyle; 
        }
        /*! \copydoc errorbarSize
            \see see errorbarSize for details */ 
        inline virtual void setErrorbarSize(double __value)
        {
            this->errorbarSize = __value;
        } 
        /*! \copydoc errorbarSize
            \see see errorbarSize for details */ 
        inline virtual double getErrorbarSize() const  
        {
            return this->errorbarSize; 
        }
        /** \brief returns true, if the error plots use the given column */
        virtual bool errorUsesColumn(int c) const;

        void setErrorColorFromGraphColor(QColor graphColor);

    protected:
        /** \brief color of the error lines/bars */
        QColor errorColor;
        /** \brief linestyle of the error lines/bars */
        Qt::PenStyle errorStyle;
        /** \brief width (pixels) of the error lines/bars */
        double errorWidth;
        /** \brief fill color for error polygons */
        QColor errorFillColor;
        /** \brief fill style for error polygons */
        Qt::BrushStyle errorFillStyle;
        /** \brief size of the error bar end markers [pixels] */
        double errorbarSize;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. This method is called by
         *         the JKQTPGraph descendents */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph,  int xColumn, int yColumn, double xrelshift=0, double yrelshift=0.0, const  QVector<int>* dataorder=nullptr)=0;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        void intPlotXYErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPErrorPlotstyle xErrorStyle, JKQTPErrorPlotstyle yErrorStyle, int xErrorColumnLower=-1, int yErrorColumnLower=-1, bool xErrorSymmetric=true, bool yErrorSymmetric=true, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr);

        /** \brief this function can be used to set the color of the error indicators automatically
         *
         * return \c true and the colors to use, if applicable, the default implementation returns false */
        virtual bool intPlotXYErrorIndicatorsGetColor(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, int xErrorColumn, int yErrorColumn, JKQTPErrorPlotstyle xErrorStyle, JKQTPErrorPlotstyle yErrorStyle, int index, QColor& errorColor, QColor& errorFillColor);


        virtual double getXErrorU(int i, JKQTPDatastore* ds) const;
        virtual double getXErrorL(int i, JKQTPDatastore* ds) const;
        virtual double getYErrorU(int i, JKQTPDatastore* ds) const;
        virtual double getYErrorL(int i, JKQTPDatastore* ds) const;

        //** \brief plot a single error indicator */
        //void intPlotXYErrorIndicator(JKQTPEnhancedPainter& painter, double x, double xperror, double xmerror, double y, double yperror, double ymerror, JKQTPErrorPlotstyle xErrorStyle=JKQTPNoError, JKQTPErrorPlotstyle yErrorStyle=JKQTPNoError, double lastx=0, double lastxperror=0, double lastxmerror=0, double lasty=0, double lastyperror=0, double lastymerror=0);
};


/*! \brief This class adds data fields for error indicators in x direction to a JKQTPGraph descendent.
    \ingroup jkqtplotter_basegraphserrors
    \see JKQTPGraphErrors,  \ref jkqtplotter_graphsgroup_classstructure_mixins
 */
class JKQTP_LIB_EXPORT JKQTPXGraphErrors: public JKQTPGraphErrors {
    public:
        /** \brief class contructor */
        JKQTPXGraphErrors(QColor graphColor, JKQTBasePlotter* basePlotter=nullptr);

        /*! \copydoc xErrorSymmetric
            \see see xErrorSymmetric for details */ 
        inline virtual void setXErrorSymmetric(bool __value)
        {
            this->xErrorSymmetric = __value;
        } 
        /*! \copydoc xErrorSymmetric
            \see see xErrorSymmetric for details */ 
        inline virtual bool getXErrorSymmetric() const  
        {
            return this->xErrorSymmetric; 
        }
        /*! \brief returns the property xErrorColumnLower ( \copybrief xErrorColumnLower ). \details Description of the parameter xErrorColumnLower is:  <BLOCKQUOTE>\copydoc xErrorColumnLower </BLOCKQUOTE>. \see xErrorColumnLower for more information */ 
        inline int getXErrorColumnLower() const { return this->xErrorColumnLower; }
        /*! \brief returns the property xErrorColumn ( \copybrief xErrorColumn ). \details Description of the parameter xErrorColumn is:  <BLOCKQUOTE>\copydoc xErrorColumn </BLOCKQUOTE>. \see xErrorColumn for more information */ 
        inline int getXErrorColumn() const { return this->xErrorColumn; }
        /*! \copydoc xErrorStyle
            \see see xErrorStyle for details */ 
        inline virtual void setXErrorStyle(const JKQTPErrorPlotstyle & __value)  
        {
            this->xErrorStyle = __value;
        } 
        /*! \copydoc xErrorStyle
            \see see xErrorStyle for details */ 
        inline virtual JKQTPErrorPlotstyle getXErrorStyle() const  
        {
            return this->xErrorStyle; 
        }
        /** \brief set the column from which to read the error values for x-error indicators */
        void setXErrorColumn(int __value);
        /** \brief set the column from which to read the error values for lower x-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void setXErrorColumnLower(int __value);

        /** \brief returns true, if the error plots use the given column */
        virtual bool errorUsesColumn(int c) const override;

    protected:
        /** \brief the column that contains the error of the x-component of the datapoints */
        int xErrorColumn;
        /** \brief the column that contains the error of the x-component of the datapoints. This is used as the lower error length, if xErrorSymmetric \c ==false. */
        int xErrorColumnLower;
        /** \brief indicates whether the x-errors are symmetric (from one column only) */
        bool  xErrorSymmetric;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPErrorPlotstyle xErrorStyle;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr) override;

        virtual double getXErrorU(int i, JKQTPDatastore* ds) const override;
        virtual double getXErrorL(int i, JKQTPDatastore* ds) const override;

};


/*! \brief This class adds data fields for error indicators in y direction to a class.
    \ingroup jkqtplotter_basegraphserrors
    \see JKQTPGraphErrors,  \ref jkqtplotter_graphsgroup_classstructure_mixins

 */
class JKQTP_LIB_EXPORT JKQTPYGraphErrors: public JKQTPGraphErrors {
    public:
        /** \brief class contructor */
        JKQTPYGraphErrors(QColor graphColor, JKQTBasePlotter* basePlotter=nullptr);

        /*! \copydoc yErrorSymmetric
            \see see yErrorSymmetric for details */ 
        inline virtual void setYErrorSymmetric(bool __value)
        {
            this->yErrorSymmetric = __value;
        } 
        /*! \copydoc yErrorSymmetric
            \see see yErrorSymmetric for details */ 
        inline virtual bool getYErrorSymmetric() const  
        {
            return this->yErrorSymmetric; 
        }
        /*! \brief returns the property yErrorColumnLower ( \copybrief yErrorColumnLower ). \details Description of the parameter yErrorColumnLower is:  <BLOCKQUOTE>\copydoc yErrorColumnLower </BLOCKQUOTE>. \see yErrorColumnLower for more information */ 
        inline int getYErrorColumnLower() const { return this->yErrorColumnLower; }
        /*! \brief returns the property yErrorColumn ( \copybrief yErrorColumn ). \details Description of the parameter yErrorColumn is:  <BLOCKQUOTE>\copydoc yErrorColumn </BLOCKQUOTE>. \see yErrorColumn for more information */ 
        inline int getYErrorColumn() const { return this->yErrorColumn; }
        /*! \copydoc yErrorStyle
            \see see yErrorStyle for details */ 
        inline virtual void setYErrorStyle(const JKQTPErrorPlotstyle & __value)  
        {
            this->yErrorStyle = __value;
        } 
        /*! \copydoc yErrorStyle
            \see see yErrorStyle for details */ 
        inline virtual JKQTPErrorPlotstyle getYErrorStyle() const  
        {
            return this->yErrorStyle; 
        }
        /** \copydoc JKQTPGraphErrors::errorUsesColumn() */
        virtual bool errorUsesColumn(int c) const override;

        /** \brief set the column from which to read the error values for y-error indicators */
        void setYErrorColumn(int __value);
        /** \brief set the column from which to read the error values for lower y-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void setYErrorColumnLower(int __value);
    protected:
        /** \brief the column that contains the error of the x-component of the datapoints */
        int yErrorColumn;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPErrorPlotstyle yErrorStyle;
        /** \brief indicates whether the y-errors are symmetric (from one column only) */
        bool  yErrorSymmetric;
        /** \brief the column that contains the error of the y-component of the datapoints. This is used as the lower error length, if yErrorSymmetric \c ==false. */
        int yErrorColumnLower;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr) override;

        virtual double getYErrorU(int i, JKQTPDatastore* ds) const override;
        virtual double getYErrorL(int i, JKQTPDatastore* ds) const override;
};


/*! \brief This class adds data fields for error indicators in x and y direction to a class.
    \ingroup jkqtplotter_basegraphserrors
    \see JKQTPGraphErrors,  \ref jkqtplotter_graphsgroup_classstructure_mixins

 */
class JKQTP_LIB_EXPORT JKQTPXYGraphErrors: public JKQTPGraphErrors {
    public:
        /** \brief class contructor */
        JKQTPXYGraphErrors(QColor graphColor, JKQTBasePlotter* basePlotter=nullptr);
        /*! \copydoc xErrorSymmetric
            \see see xErrorSymmetric for details */ 
        inline virtual void setXErrorSymmetric(bool __value)
        {
            this->xErrorSymmetric = __value;
        } 
        /*! \copydoc xErrorSymmetric
            \see see xErrorSymmetric for details */ 
        inline virtual bool getXErrorSymmetric() const  
        {
            return this->xErrorSymmetric; 
        }
        /*! \copydoc yErrorSymmetric
            \see see yErrorSymmetric for details */ 
        inline virtual void setYErrorSymmetric(bool __value)
        {
            this->yErrorSymmetric = __value;
        } 
        /*! \copydoc yErrorSymmetric
            \see see yErrorSymmetric for details */ 
        inline virtual bool getYErrorSymmetric() const  
        {
            return this->yErrorSymmetric; 
        }
        /*! \brief returns the property xErrorColumnLower ( \copybrief xErrorColumnLower ). \details Description of the parameter xErrorColumnLower is:  <BLOCKQUOTE>\copydoc xErrorColumnLower </BLOCKQUOTE>. \see xErrorColumnLower for more information */ 
        inline int getXErrorColumnLower() const { return this->xErrorColumnLower; }
        /*! \brief returns the property xErrorColumn ( \copybrief xErrorColumn ). \details Description of the parameter xErrorColumn is:  <BLOCKQUOTE>\copydoc xErrorColumn </BLOCKQUOTE>. \see xErrorColumn for more information */ 
        inline int getXErrorColumn() const { return this->xErrorColumn; }
        /*! \brief returns the property yErrorColumnLower ( \copybrief yErrorColumnLower ). \details Description of the parameter yErrorColumnLower is:  <BLOCKQUOTE>\copydoc yErrorColumnLower </BLOCKQUOTE>. \see yErrorColumnLower for more information */ 
        inline int getYErrorColumnLower() const { return this->yErrorColumnLower; }
        /*! \brief returns the property yErrorColumn ( \copybrief yErrorColumn ). \details Description of the parameter yErrorColumn is:  <BLOCKQUOTE>\copydoc yErrorColumn </BLOCKQUOTE>. \see yErrorColumn for more information */ 
        inline int getYErrorColumn() const { return this->yErrorColumn; }
        /*! \copydoc yErrorStyle
            \see see yErrorStyle for details */ 
        inline virtual void setYErrorStyle(const JKQTPErrorPlotstyle & __value)  
        {
            this->yErrorStyle = __value;
        } 
        /*! \copydoc yErrorStyle
            \see see yErrorStyle for details */ 
        inline virtual JKQTPErrorPlotstyle getYErrorStyle() const  
        {
            return this->yErrorStyle; 
        }
        /*! \copydoc xErrorStyle
            \see see xErrorStyle for details */ 
        inline virtual void setXErrorStyle(const JKQTPErrorPlotstyle & __value)  
        {
            this->xErrorStyle = __value;
        } 
        /*! \copydoc xErrorStyle
            \see see xErrorStyle for details */ 
        inline virtual JKQTPErrorPlotstyle getXErrorStyle() const  
        {
            return this->xErrorStyle; 
        }
        /** \copydoc JKQTPGraphErrors::errorUsesColumn() */
        virtual bool errorUsesColumn(int c) const override;

        /** \brief set the column from which to read the error values for x-error indicators */
        void setXErrorColumn(int __value);
        /** \brief set the column from which to read the error values for lower x-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void setXErrorColumnLower(int __value);
        /** \brief set the column from which to read the error values for x-error indicators */
        void setYErrorColumn(int __value);
        /** \brief set the column from which to read the error values for lower x-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void setYErrorColumnLower(int __value);
        /** \brief set the column from which to read the error values for y-error indicators */
        void setXErrorColumn(size_t __value);
        /** \brief set the column from which to read the error values for lower y-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void setXErrorColumnLower(size_t __value);
        /** \brief set the column from which to read the error values for y-error indicators */
        void setYErrorColumn(size_t __value);
        /** \brief set the column from which to read the error values for lower y-error indicators (if not set [=-1], the xErrorColumn-values will be used for upper and lower error indicators) */
        void setYErrorColumnLower(size_t __value);

    protected:
        /** \brief the column that contains the error of the x-component of the datapoints */
        int yErrorColumn;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPErrorPlotstyle yErrorStyle;
        /** \brief the column that contains the error of the x-component of the datapoints */
        int xErrorColumn;
        /** \brief how to draw the errors (if available) of the x-value */
        JKQTPErrorPlotstyle xErrorStyle;
        /** \brief indicates whether the x-errors are symmetric (from one column only) */
        bool  xErrorSymmetric;
        /** \brief indicates whether the y-errors are symmetric (from one column only) */
        bool  yErrorSymmetric;
        /** \brief the column that contains the error of the x-component of the datapoints. This is used as the lower error length, if xErrorSymmetric \c ==false. */
        int xErrorColumnLower;
        /** \brief the column that contains the error of the y-component of the datapoints. This is used as the lower error length, if yErrorSymmetric \c ==false. */
        int yErrorColumnLower;

        /** \brief draw error indicators with the parameters defined in this class. The position of the datapoints is
         *         given by the \a xColumn and \a yColumn. It is also possible to specify a datarange. */
        virtual void plotErrorIndicators(JKQTPEnhancedPainter& painter, JKQTBasePlotter* parent, JKQTPGraph* parentGraph, int xColumn, int yColumn, double xrelshift=0, double yrelshift=0.0, const QVector<int> *dataorder=nullptr) override;

        virtual double getXErrorU(int i, JKQTPDatastore* ds) const override;
        virtual double getXErrorL(int i, JKQTPDatastore* ds) const override;
        virtual double getYErrorU(int i, JKQTPDatastore* ds) const override;
        virtual double getYErrorL(int i, JKQTPDatastore* ds) const override;
};







#endif // JKQTPGRAPHSBASEERROR_H
