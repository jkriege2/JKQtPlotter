/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)

    

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
 * \defgroup jkqtplotter_elements Plot Elements
 * \ingroup jkqtplotter
 * \defgroup jkqtplotter_plots Graphs
 * \ingroup jkqtplotter_elements
 */

/** \file jkqtpgraphs.h
  * \ingroup jkqtplotter
  */

#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include <functional>

#ifndef jkqtpgraphsevaluatedfunction_H
#define jkqtpgraphsevaluatedfunction_H




/*! \brief type of functions that may be plottet
    \ingroup jkqtplotter_plots

    This is the type of functions \f$ y=f(x, \vec{p}) \f$ that may be plottet by JKQTPxFunctionLineGraph
    and JKQTPyFunctionLineGraph. It is possible to supply parameters \f$ \vec{p} \f$ to the function that
    influence its result. Parameters are given as a pointer to some memory location. The function has to
    know on its own how to interpret these.
*/
typedef std::function<double(double, void*)> jkqtpPlotFunctionType;

/*! \brief simplified type of functions (without parameters) that may be plottet
    \ingroup jkqtplotter_plots

    This is the type of functions \f$ y=f(x) \f$ that may be plottet by JKQTPxFunctionLineGraph
    and JKQTPyFunctionLineGraph.
*/
typedef std::function<double(double)> jkqtpSimplePlotFunctionType;


/*! \brief This implements line plots where the data is taken from a user supplied function \f$ y=f(x) \f$
    \ingroup jkqtplotter_plots

    This class implements an intelligent plotting algorithm for functions. It starts by sampling
    the function at minSamples positions. Then each function interval is bisected recursively if
    necessary. To do so the function is evaluated at the mid point and the slopes \f$ \alpha_{\mbox{left}} \f$
    and \f$ \alpha_{\mbox{right}} \f$ of the two linear segments are compared. the midpoint is added
    to the graph if \f[ \left|\alpha_{\mbox{right}}-\alpha_{\mbox{left}}\right|>\mbox{slopeTolerance} \f]
    In addition all sampling points except minimum and maximum are beeing shifted by a random fraction their
    distance to the other points. This helps to prevent beats when sampling periodic functions.

    the following image
    \image html plot_functionplots.png
 */
class LIB_EXPORT JKQTPxFunctionLineGraph: public JKQTPgraph {
        Q_OBJECT
    public:

        enum SpecialFunction {
            Polynomial, /*!< \brief a polynomial \f$ f(x)=p_0+p_1x+p_2x^2+p_3x^3+... \f$ The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */
            Line=Polynomial, /*!< \brief a polynomial \f$ f(x)=p_0+p_1x \f$ The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */
            Exponential,  /*!< \brief an exponential function \f$ f(x)=p_0+p_1\cdot\exp(x/p_2) \f$ or \f$ f(x)=p_0\cdot\exp(x/p_1) \f$ (depending on the number of parameters). The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */
            PowerLaw,  /*!< \brief an exponential function \f$ f(x)=p_0+p_1\cdot x^{p_3} \f$ or \f$ f(x)=p_0\cdot x^{p_1} \f$ or \f$ f(x)= x^{p_0} \f$ (depending on the number of parameters) The parameters \a params have to be point to a QVector<double> and contain the parameters \f$ p_0, p_1, ... \f$ */

            UserFunction,  /*!< \brief no special function but the function is provided by the user */
        };

        /** \brief class constructor */
        JKQTPxFunctionLineGraph(JKQtBasePlotter* parent=nullptr);

        /** \brief class constructor */
        JKQTPxFunctionLineGraph(JKQtPlotter* parent);

        /** \brief class destructor */
        virtual ~JKQTPxFunctionLineGraph();

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() override;

        /** \brief get the maximum and minimum x-value of the graph
         *
         * This functions returns 0 for both parameters, so that the plotter uses the predefined
         * min and max values.
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \brief get the maximum and minimum y-value of the graph
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;

        /** \brief clear the data sampled from the function. */
        void clearData();

        /*! \brief sets the property color to the specified \a __value. 
            \details Description of the parameter color is: <CENTER>\copybrief color.</CENTER> 
            \see color for more information */ 
        inline virtual void set_color(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \brief returns the property color. 
            \details Description of the parameter color is: <CENTER>\copybrief color.</CENTER> 
            \see color for more information */ 
        inline virtual QColor get_color() const  
        {
            return this->color; 
        }
        /*! \brief sets the property fillColor to the specified \a __value. 
            \details Description of the parameter fillColor is: <CENTER>\copybrief fillColor.</CENTER> 
            \see fillColor for more information */ 
        inline virtual void set_fillColor(const QColor & __value)  
        {
            this->fillColor = __value;
        } 
        /*! \brief returns the property fillColor. 
            \details Description of the parameter fillColor is: <CENTER>\copybrief fillColor.</CENTER> 
            \see fillColor for more information */ 
        inline virtual QColor get_fillColor() const  
        {
            return this->fillColor; 
        }
        /*! \brief sets the property fillStyle to the specified \a __value. 
            \details Description of the parameter fillStyle is: <CENTER>\copybrief fillStyle.</CENTER> 
            \see fillStyle for more information */ 
        inline virtual void set_fillStyle(const Qt::BrushStyle & __value)  
        {
            this->fillStyle = __value;
        } 
        /*! \brief returns the property fillStyle. 
            \details Description of the parameter fillStyle is: <CENTER>\copybrief fillStyle.</CENTER> 
            \see fillStyle for more information */ 
        inline virtual Qt::BrushStyle get_fillStyle() const  
        {
            return this->fillStyle; 
        }
        /*! \brief sets the property style to the specified \a __value. 
            \details Description of the parameter style is: <CENTER>\copybrief style.</CENTER> 
            \see style for more information */ 
        inline virtual void set_style(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \brief returns the property style. 
            \details Description of the parameter style is: <CENTER>\copybrief style.</CENTER> 
            \see style for more information */ 
        inline virtual Qt::PenStyle get_style() const  
        {
            return this->style; 
        }
        /*! \brief sets the property lineWidth to the specified \a __value. 
            \details Description of the parameter lineWidth is: <CENTER>\copybrief lineWidth.</CENTER> 
            \see lineWidth for more information */ 
        inline virtual void set_lineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \brief returns the property lineWidth. 
            \details Description of the parameter lineWidth is: <CENTER>\copybrief lineWidth.</CENTER> 
            \see lineWidth for more information */ 
        inline virtual double get_lineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \brief sets the property drawLine to the specified \a __value. 
            \details Description of the parameter drawLine is: <CENTER>\copybrief drawLine.</CENTER> 
            \see drawLine for more information */ 
        inline virtual void set_drawLine(bool __value)
        {
            this->drawLine = __value;
        } 
        /*! \brief returns the property drawLine. 
            \details Description of the parameter drawLine is: <CENTER>\copybrief drawLine.</CENTER> 
            \see drawLine for more information */ 
        inline virtual bool get_drawLine() const  
        {
            return this->drawLine; 
        }

        /** \brief sets the property plotFunction to the specified \a __value.
         *
         *  \details Description of the parameter varname is: <CENTER>\copybrief plotFunction.</CENTER>
         * \see plotFunction for more information */
        virtual void set_plotFunction (jkqtpPlotFunctionType && __value);
        /** \brief sets the property plotFunction to the specified \a __value.
         *
         *  \details Description of the parameter varname is: <CENTER>\copybrief plotFunction.</CENTER>
         * \see plotFunction for more information */
        virtual void set_plotFunction (const jkqtpPlotFunctionType & __value);
        /** \brief sets the property plotFunction to the specified \a __value.
         *
         *  \details Description of the parameter plotFunction is: <CENTER>\copybrief plotFunction.</CENTER>
         * \see plotFunction for more information */
        virtual void set_plotFunction (jkqtpSimplePlotFunctionType && __value);
        /** \brief sets the property plotFunction to the specified \a __value.
         *
         *  \details Description of the parameter plotFunction is: <CENTER>\copybrief plotFunction.</CENTER>
         * \see plotFunction for more information */
        virtual void set_plotFunction (const jkqtpSimplePlotFunctionType & __value);
        /*! \brief returns the property plotFunction. \see plotFunction for more information */ \
        virtual jkqtpPlotFunctionType get_plotFunction () const;
        /*! \brief returns the property simplePlotFunction. \see simplePlotFunction for more information */ \
        virtual jkqtpSimplePlotFunctionType get_simplePlotFunction () const;

        /*! \brief sets the property params to the specified \a __value. 
            \details Description of the parameter params is: <CENTER>\copybrief params.</CENTER> 
            \see params for more information */ 
        inline virtual void set_params(void* __value)
        {
            if (this->params != __value) { 
                this->params = __value; 
                clearData(); 
            } 
        } 
        /*! \brief returns the property params. 
            \details Description of the parameter params is: <CENTER>\copybrief params.</CENTER> 
            \see params for more information */ 
        inline virtual void* get_params() const  
        {
            return this->params; 
        }
        /** \brief sets the params as a pointer to an internal COPY of the given vector (not the data of the vector, as then the size would be unknown!!!) */
        void set_params(const QVector<double>& params);
        /** \brief sets the params from a copy of the given array of length \a N */
        void set_copiedParams(const double* params, int N);
        /** \brief set an internal parameter vector as function parameters, initialized with {p1} */
        void set_paramsV(double p1);
        /** \brief set an internal parameter vector as function parameters, initialized with {p1,p2} */
        void set_paramsV(double p1, double p2);
        /** \brief set an internal parameter vector as function parameters, initialized with {p1,p2,p3} */
        void set_paramsV(double p1, double p2, double p3);
        /** \brief set an internal parameter vector as function parameters, initialized with {p1,p2,p3,p4} */
        void set_paramsV(double p1, double p2, double p3, double p4);
        /** \brief set an internal parameter vector as function parameters, initialized with {p1,p2,p3,p4,p5} */
        void set_paramsV(double p1, double p2, double p3, double p4, double p5);

        /** \brief returns the currently set internal parameter vector */
        QVector<double> get_internalParams() const;
        /** \brief returns the currently set internal parameter vector */
        QVector<double> get_internalErrorParams() const;
        /*! \brief sets the property minSamples to the specified \a __value. 
            \details Description of the parameter minSamples is: <CENTER>\copybrief minSamples.</CENTER> 
            \see minSamples for more information */ 
        inline virtual void set_minSamples(const unsigned int & __value)  
        {
            this->minSamples = __value;
        } 
        /*! \brief returns the property minSamples. 
            \details Description of the parameter minSamples is: <CENTER>\copybrief minSamples.</CENTER> 
            \see minSamples for more information */ 
        inline virtual unsigned int get_minSamples() const  
        {
            return this->minSamples; 
        }
        /*! \brief sets the property maxRefinementDegree to the specified \a __value. 
            \details Description of the parameter maxRefinementDegree is: <CENTER>\copybrief maxRefinementDegree.</CENTER> 
            \see maxRefinementDegree for more information */ 
        inline virtual void set_maxRefinementDegree(const unsigned int & __value)  
        {
            this->maxRefinementDegree = __value;
        } 
        /*! \brief returns the property maxRefinementDegree. 
            \details Description of the parameter maxRefinementDegree is: <CENTER>\copybrief maxRefinementDegree.</CENTER> 
            \see maxRefinementDegree for more information */ 
        inline virtual unsigned int get_maxRefinementDegree() const  
        {
            return this->maxRefinementDegree; 
        }
        /*! \brief sets the property slopeTolerance to the specified \a __value. 
            \details Description of the parameter slopeTolerance is: <CENTER>\copybrief slopeTolerance.</CENTER> 
            \see slopeTolerance for more information */ 
        inline virtual void set_slopeTolerance(double __value)
        {
            this->slopeTolerance = __value;
        } 
        /*! \brief returns the property slopeTolerance. 
            \details Description of the parameter slopeTolerance is: <CENTER>\copybrief slopeTolerance.</CENTER> 
            \see slopeTolerance for more information */ 
        inline virtual double get_slopeTolerance() const  
        {
            return this->slopeTolerance; 
        }
        /*! \brief sets the property minPixelPerSample to the specified \a __value. 
            \details Description of the parameter minPixelPerSample is: <CENTER>\copybrief minPixelPerSample.</CENTER> 
            \see minPixelPerSample for more information */ 
        inline virtual void set_minPixelPerSample(double __value)
        {
            this->minPixelPerSample = __value;
        } 
        /*! \brief returns the property minPixelPerSample. 
            \details Description of the parameter minPixelPerSample is: <CENTER>\copybrief minPixelPerSample.</CENTER> 
            \see minPixelPerSample for more information */ 
        inline virtual double get_minPixelPerSample() const  
        {
            return this->minPixelPerSample; 
        }
        /*! \brief sets the property plotRefinement to the specified \a __value. 
            \details Description of the parameter plotRefinement is: <CENTER>\copybrief plotRefinement.</CENTER> 
            \see plotRefinement for more information */ 
        inline virtual void set_plotRefinement(bool __value)
        {
            this->plotRefinement = __value;
        } 
        /*! \brief returns the property plotRefinement. 
            \details Description of the parameter plotRefinement is: <CENTER>\copybrief plotRefinement.</CENTER> 
            \see plotRefinement for more information */ 
        inline virtual bool get_plotRefinement() const  
        {
            return this->plotRefinement; 
        }
        /*! \brief sets the property displaySamplePoints to the specified \a __value. 
            \details Description of the parameter displaySamplePoints is: <CENTER>\copybrief displaySamplePoints.</CENTER> 
            \see displaySamplePoints for more information */ 
        inline virtual void set_displaySamplePoints(bool __value)
        {
            this->displaySamplePoints = __value;
        } 
        /*! \brief returns the property displaySamplePoints. 
            \details Description of the parameter displaySamplePoints is: <CENTER>\copybrief displaySamplePoints.</CENTER> 
            \see displaySamplePoints for more information */ 
        inline virtual bool get_displaySamplePoints() const  
        {
            return this->displaySamplePoints; 
        }
        /*! \brief sets the property drawErrorPolygons to the specified \a __value. 
            \details Description of the parameter drawErrorPolygons is: <CENTER>\copybrief drawErrorPolygons.</CENTER> 
            \see drawErrorPolygons for more information */ 
        inline virtual void set_drawErrorPolygons(bool __value)
        {
            this->drawErrorPolygons = __value;
        } 
        /*! \brief returns the property drawErrorPolygons. 
            \details Description of the parameter drawErrorPolygons is: <CENTER>\copybrief drawErrorPolygons.</CENTER> 
            \see drawErrorPolygons for more information */ 
        inline virtual bool get_drawErrorPolygons() const  
        {
            return this->drawErrorPolygons; 
        }
        /*! \brief sets the property drawErrorLines to the specified \a __value. 
            \details Description of the parameter drawErrorLines is: <CENTER>\copybrief drawErrorLines.</CENTER> 
            \see drawErrorLines for more information */ 
        inline virtual void set_drawErrorLines(bool __value)
        {
            this->drawErrorLines = __value;
        } 
        /*! \brief returns the property drawErrorLines. 
            \details Description of the parameter drawErrorLines is: <CENTER>\copybrief drawErrorLines.</CENTER> 
            \see drawErrorLines for more information */ 
        inline virtual bool get_drawErrorLines() const  
        {
            return this->drawErrorLines; 
        }
        /** \brief sets the property errorPlotFunction to the specified \a __value.
         *
         *  \details Description of the parameter varname is: <CENTER>\copybrief errorPlotFunction.</CENTER>
         * \see errorPlotFunction for more information */
        virtual void set_errorPlotFunction (jkqtpPlotFunctionType && __value);
        /** \brief sets the property errorPlotFunction to the specified \a __value.
         *
         *  \details Description of the parameter varname is: <CENTER>\copybrief errorPlotFunction.</CENTER>
         * \see errorPlotFunction for more information */
        virtual void set_errorPlotFunction (const jkqtpPlotFunctionType & __value);
        /*! \brief returns the property varname. \see varname for more information */ \
        virtual jkqtpPlotFunctionType get_errorPlotFunction () const;
        /** \brief sets the property errorPlotFunction to the specified \a __value.
         *
         *  \details Description of the parameter varname is: <CENTER>\copybrief errorPlotFunction.</CENTER>
         * \see errorPlotFunction for more information */
        virtual void set_errorPlotFunction (jkqtpSimplePlotFunctionType && __value);
        /** \brief sets the property errorPlotFunction to the specified \a __value.
         *
         *  \details Description of the parameter varname is: <CENTER>\copybrief errorPlotFunction.</CENTER>
         * \see errorPlotFunction for more information */
        virtual void set_errorPlotFunction (const jkqtpSimplePlotFunctionType & __value);
        /*! \brief returns the property varname. \see varname for more information */ \
        virtual jkqtpSimplePlotFunctionType get_errorSimplePlotFunction () const;
        /*! \brief sets the property errorParams to the specified \a __value. 
            \details Description of the parameter errorParams is: <CENTER>\copybrief errorParams.</CENTER> 
            \see errorParams for more information */ 
        inline virtual void set_errorParams(void* __value)
        {
            this->errorParams = __value;
        } 
        /*! \brief returns the property errorParams. 
            \details Description of the parameter errorParams is: <CENTER>\copybrief errorParams.</CENTER> 
            \see errorParams for more information */ 
        inline virtual void* get_errorParams() const  
        {
            return this->errorParams; 
        }
        /** \brief sets the error params as a pointer to an internal COPY of the given vector (not the data of the vector, as then the size would be unknown!!!) */
        void set_errorParams(const QVector<double>& errorParams);

        /*! \brief sets the property parameterColumn to the specified \a __value. 
            \details Description of the parameter parameterColumn is: <CENTER>\copybrief parameterColumn.</CENTER> 
            \see parameterColumn for more information */ 
        inline virtual void set_parameterColumn(int __value)
        {
            this->parameterColumn = __value;
        } 
        /*! \brief returns the property parameterColumn. 
            \details Description of the parameter parameterColumn is: <CENTER>\copybrief parameterColumn.</CENTER> 
            \see parameterColumn for more information */ 
        inline virtual int get_parameterColumn() const  
        {
            return this->parameterColumn; 
        }
        /*! \brief sets the property parameterColumn to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter parameterColumn is:  <CENTER>\copybrief parameterColumn.</CENTER> 
        	\see parameterColumn for more information */ 
        inline virtual void set_parameterColumn (size_t __value) { this->parameterColumn = static_cast<int>(__value); }
        /*! \brief sets the property errorParameterColumn to the specified \a __value. 
            \details Description of the parameter errorParameterColumn is: <CENTER>\copybrief errorParameterColumn.</CENTER> 
            \see errorParameterColumn for more information */ 
        inline virtual void set_errorParameterColumn(int __value)
        {
            this->errorParameterColumn = __value;
        } 
        /*! \brief returns the property errorParameterColumn. 
            \details Description of the parameter errorParameterColumn is: <CENTER>\copybrief errorParameterColumn.</CENTER> 
            \see errorParameterColumn for more information */ 
        inline virtual int get_errorParameterColumn() const  
        {
            return this->errorParameterColumn; 
        }
        /*! \brief sets the property errorParameterColumn to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter errorParameterColumn is:  <CENTER>\copybrief errorParameterColumn.</CENTER> 
        	\see errorParameterColumn for more information */ 
        inline virtual void set_errorParameterColumn (size_t __value) { this->errorParameterColumn = static_cast<int>(__value); }

        /*! \brief sets the property errorColor to the specified \a __value. 
            \details Description of the parameter errorColor is: <CENTER>\copybrief errorColor.</CENTER> 
            \see errorColor for more information */ 
        inline virtual void set_errorColor(const QColor & __value)  
        {
            this->errorColor = __value;
        } 
        /*! \brief returns the property errorColor. 
            \details Description of the parameter errorColor is: <CENTER>\copybrief errorColor.</CENTER> 
            \see errorColor for more information */ 
        inline virtual QColor get_errorColor() const  
        {
            return this->errorColor; 
        }
        /*! \brief sets the property errorFillColor to the specified \a __value. 
            \details Description of the parameter errorFillColor is: <CENTER>\copybrief errorFillColor.</CENTER> 
            \see errorFillColor for more information */ 
        inline virtual void set_errorFillColor(const QColor & __value)  
        {
            this->errorFillColor = __value;
        } 
        /*! \brief returns the property errorFillColor. 
            \details Description of the parameter errorFillColor is: <CENTER>\copybrief errorFillColor.</CENTER> 
            \see errorFillColor for more information */ 
        inline virtual QColor get_errorFillColor() const  
        {
            return this->errorFillColor; 
        }
        /*! \brief sets the property errorFillStyle to the specified \a __value. 
            \details Description of the parameter errorFillStyle is: <CENTER>\copybrief errorFillStyle.</CENTER> 
            \see errorFillStyle for more information */ 
        inline virtual void set_errorFillStyle(const Qt::BrushStyle & __value)  
        {
            this->errorFillStyle = __value;
        } 
        /*! \brief returns the property errorFillStyle. 
            \details Description of the parameter errorFillStyle is: <CENTER>\copybrief errorFillStyle.</CENTER> 
            \see errorFillStyle for more information */ 
        inline virtual Qt::BrushStyle get_errorFillStyle() const  
        {
            return this->errorFillStyle; 
        }
        /*! \brief sets the property errorStyle to the specified \a __value. 
            \details Description of the parameter errorStyle is: <CENTER>\copybrief errorStyle.</CENTER> 
            \see errorStyle for more information */ 
        inline virtual void set_errorStyle(const Qt::PenStyle & __value)  
        {
            this->errorStyle = __value;
        } 
        /*! \brief returns the property errorStyle. 
            \details Description of the parameter errorStyle is: <CENTER>\copybrief errorStyle.</CENTER> 
            \see errorStyle for more information */ 
        inline virtual Qt::PenStyle get_errorStyle() const  
        {
            return this->errorStyle; 
        }
        /*! \brief sets the property errorLineWidth to the specified \a __value. 
            \details Description of the parameter errorLineWidth is: <CENTER>\copybrief errorLineWidth.</CENTER> 
            \see errorLineWidth for more information */ 
        inline virtual void set_errorLineWidth(double __value)
        {
            this->errorLineWidth = __value;
        } 
        /*! \brief returns the property errorLineWidth. 
            \details Description of the parameter errorLineWidth is: <CENTER>\copybrief errorLineWidth.</CENTER> 
            \see errorLineWidth for more information */ 
        inline virtual double get_errorLineWidth() const  
        {
            return this->errorLineWidth; 
        }

        /** \copydoc JKQTPgraph::usesColumn() */
        virtual bool usesColumn(int c) const override;


        /** \brief sets function to the given special function */
        void setSpecialFunction(SpecialFunction function);
        /** \brief returns, which special function is set (or if any is set) */
        SpecialFunction getFunctionType() const;
    protected:
        /** \brief which plot style to use from the parent plotter (via JKQtPlotterBase::getPlotStyle() and JKQtPlotterBase::getNextStyle() ) */
        int parentPlotStyle;

        struct doublePair {
            double x;
            double f;
            doublePair* next;
        };
        /** \brief a linked list holding the datapoints \f$ \left(x, y=f(x, \vec{p})\right) \f$ to be plotted */
        doublePair* data;

        /** \brief fill the data array with data from the function plotFunction */
        virtual void createPlotData( bool collectParams=true);

        virtual void collectParameters();
        /** \brief refine datapoints on the function graph between two evaluations \a a and \a b */
        void refine(doublePair* a, doublePair* b, unsigned int degree=0);

        /** \brief if set, the values from this datatsore column are used for the parameters \c p1 , \c p2 , \c p3 , ...  of the plot function */
        int parameterColumn;
        /** \brief if set, the values from this datatsore column are used for the parameters \c p1 , \c p2 , \c p3 , ...  of the error plot function */
        int errorParameterColumn;

        /** \brief color of the graph */
        QColor color;
        /** \brief color of the graph fill */
        QColor fillColor;
        /** \brief linestyle of the graph lines */
        Qt::PenStyle style;
        /** \brief width (pixels) of the graph */
        double lineWidth;
        /** \brief fill style, if the curve should be filled */
        Qt::BrushStyle fillStyle;
        /** \brief indicates whether to draw a line or not */
        bool drawLine;
        /** \brief indicates whether to fill the space between the curve and the x-axis */
        bool fillCurve;
        /** \brief the function to be plotted */
        jkqtpPlotFunctionType plotFunction;
        /** \brief a simple function to be plotted, simplified form without parameters */
        jkqtpSimplePlotFunctionType simplePlotFunction;
        /** \brief indicates whether a special function is set (and if so, which one), or a user-supplied function */
        SpecialFunction functionType;
        /** \brief pointer to the parameters supplied to the plotting funtion */
        void* params;
        /** \brief the minimum number of points to evaluate the function at */
        unsigned int minSamples;
        /** \brief the maximum number of recursive refinement steps
         *
         * each step bisects the interval \f$ [a, b] \f$ into two halfes. So the maximum number
         * of points plotted at all are thus:
         *  \f[ \mbox{minSamples} \cdot 2^{\mbox{maxRefinementDegree}} \f]
         */
        unsigned int maxRefinementDegree;
        /** \brief the tolerance for the difference of two subsequent slopes */
        double slopeTolerance;
        /** \brief create one sample at least every \a minPixelPerSample pixels */
        double minPixelPerSample;
        /** \brief switch on or off [default: on] the plot refinement algorithm */
        bool plotRefinement;
        /** \brief if true [default: off] display the points where the function has been sampled */
        bool displaySamplePoints;
        /** \brief indicates whether an error polygon should be drawn */
        bool drawErrorPolygons;
        /** \brief indicates whether error lines should be drawn */
        bool drawErrorLines;
        /** \brief this function calculates the error at a given position */
        jkqtpPlotFunctionType errorPlotFunction;
        /** \brief this function calculates the error at a given position, simplified form without parameters */
        jkqtpSimplePlotFunctionType errorSimplePlotFunction;
        /** \brief parameters for errorFunction */
        void* errorParams;
        /** \brief color of the error graph */
        QColor errorColor;
        /** \brief color of the error graph fill */
        QColor errorFillColor;
        /** \brief linestyle of the error graph lines */
        Qt::PenStyle errorStyle;
        /** \brief width (pixels) of the error graph */
        double errorLineWidth;
        /** \brief fill style, if the error curve should be filled */
        Qt::BrushStyle errorFillStyle;


        QBrush getBrush(JKQTPEnhancedPainter& painter) const;
        QPen getLinePen(JKQTPEnhancedPainter& painter) const;

        QBrush getErrorBrush(JKQTPEnhancedPainter& painter) const;
        QPen getErrorLinePen(JKQTPEnhancedPainter &painter) const;

        QVector<double> iparams, ierrorparams;
};

/*! \brief This implements line plots where the data is taken from a user supplied function \f$ x=f(y) \f$
    \ingroup jkqtplotter_plots

 */
class LIB_EXPORT JKQTPyFunctionLineGraph: public JKQTPxFunctionLineGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPyFunctionLineGraph(JKQtBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPyFunctionLineGraph(JKQtPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
    protected:

        /** \brief fill the data array with data from the function plotFunction */
        virtual void createPlotData( bool collectParams=true) override;

};




#endif // jkqtpgraphsevaluatedfunction_H
