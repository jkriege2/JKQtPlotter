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



/** \file jkqtpgraphsevaluatedfunction.h
  * \ingroup jkqtplotter_functiongraphs
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
    \ingroup jkqtplotter_functiongraphs

    This is the type of functions \f$ y=f(x, \vec{p}) \f$ that may be plottet by JKQTPXFunctionLineGraph
    and JKQTPYFunctionLineGraph. It is possible to supply parameters \f$ \vec{p} \f$ to the function that
    influence its result. Parameters are given as a pointer to some memory location. The function has to
    know on its own how to interpret these.
*/
typedef std::function<double(double, void*)> jkqtpPlotFunctionType;

/*! \brief simplified type of functions (without parameters) that may be plottet
    \ingroup jkqtplotter_functiongraphs

    This is the type of functions \f$ y=f(x) \f$ that may be plottet by JKQTPXFunctionLineGraph
    and JKQTPYFunctionLineGraph.
*/
typedef std::function<double(double)> jkqtpSimplePlotFunctionType;


/*! \brief This implements line plots where the data is taken from a user supplied function \f$ y=f(x) \f$
    \ingroup jkqtplotter_functiongraphs

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
class LIB_EXPORT JKQTPXFunctionLineGraph: public JKQTPGraph {
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
        JKQTPXFunctionLineGraph(JKQTBasePlotter* parent=nullptr);

        /** \brief class constructor */
        JKQTPXFunctionLineGraph(JKQTPlotter* parent);

        /** \brief class destructor */
        virtual ~JKQTPXFunctionLineGraph();

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

        /*! \brief sets the property color ( \copybrief color ) to the specified \a __value. 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual void setColor(const QColor & __value)  
        {
            this->color = __value;
        } 
        /*! \brief returns the property color ( \copybrief color ). 
            \details Description of the parameter color is: <BLOCKQUOTE>\copydoc color </BLOCKQUOTE> 
            \see color for more information */ 
        inline virtual QColor getColor() const  
        {
            return this->color; 
        }
        /*! \brief sets the property fillColor ( \copybrief fillColor ) to the specified \a __value. 
            \details Description of the parameter fillColor is: <BLOCKQUOTE>\copydoc fillColor </BLOCKQUOTE> 
            \see fillColor for more information */ 
        inline virtual void setFillColor(const QColor & __value)  
        {
            this->fillColor = __value;
        } 
        /*! \brief returns the property fillColor ( \copybrief fillColor ). 
            \details Description of the parameter fillColor is: <BLOCKQUOTE>\copydoc fillColor </BLOCKQUOTE> 
            \see fillColor for more information */ 
        inline virtual QColor getFillColor() const  
        {
            return this->fillColor; 
        }
        /*! \brief sets the property fillStyle ( \copybrief fillStyle ) to the specified \a __value. 
            \details Description of the parameter fillStyle is: <BLOCKQUOTE>\copydoc fillStyle </BLOCKQUOTE> 
            \see fillStyle for more information */ 
        inline virtual void setFillStyle(const Qt::BrushStyle & __value)  
        {
            this->fillStyle = __value;
        } 
        /*! \brief returns the property fillStyle ( \copybrief fillStyle ). 
            \details Description of the parameter fillStyle is: <BLOCKQUOTE>\copydoc fillStyle </BLOCKQUOTE> 
            \see fillStyle for more information */ 
        inline virtual Qt::BrushStyle getFillStyle() const  
        {
            return this->fillStyle; 
        }
        /*! \brief sets the property style ( \copybrief style ) to the specified \a __value. 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc style </BLOCKQUOTE> 
            \see style for more information */ 
        inline virtual void setStyle(const Qt::PenStyle & __value)  
        {
            this->style = __value;
        } 
        /*! \brief returns the property style ( \copybrief style ). 
            \details Description of the parameter style is: <BLOCKQUOTE>\copydoc style </BLOCKQUOTE> 
            \see style for more information */ 
        inline virtual Qt::PenStyle getStyle() const  
        {
            return this->style; 
        }
        /*! \brief sets the property lineWidth ( \copybrief lineWidth ) to the specified \a __value. 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual void setLineWidth(double __value)
        {
            this->lineWidth = __value;
        } 
        /*! \brief returns the property lineWidth ( \copybrief lineWidth ). 
            \details Description of the parameter lineWidth is: <BLOCKQUOTE>\copydoc lineWidth </BLOCKQUOTE> 
            \see lineWidth for more information */ 
        inline virtual double getLineWidth() const  
        {
            return this->lineWidth; 
        }
        /*! \brief sets the property drawLine ( \copybrief drawLine ) to the specified \a __value. 
            \details Description of the parameter drawLine is: <BLOCKQUOTE>\copydoc drawLine </BLOCKQUOTE> 
            \see drawLine for more information */ 
        inline virtual void setDrawLine(bool __value)
        {
            this->drawLine = __value;
        } 
        /*! \brief returns the property drawLine ( \copybrief drawLine ). 
            \details Description of the parameter drawLine is: <BLOCKQUOTE>\copydoc drawLine </BLOCKQUOTE> 
            \see drawLine for more information */ 
        inline virtual bool getDrawLine() const  
        {
            return this->drawLine; 
        }

        /** \brief sets the property plotFunction ( \copybrief plotFunction ) to the specified \a __value.
         *
         *  \details Description of the parameter plotFunction is: <BLOCKQUOTE>\copydoc plotFunction </BLOCKQUOTE>
         * \see plotFunction for more information */
        virtual void setPlotFunction (jkqtpPlotFunctionType && __value);
        /** \brief sets the property plotFunction ( \copybrief plotFunction ) to the specified \a __value.
         *
         *  \details Description of the parameter plotFunction is: <BLOCKQUOTE>\copydoc plotFunction </BLOCKQUOTE>
         * \see plotFunction for more information */
        virtual void setPlotFunction (const jkqtpPlotFunctionType & __value);
        /** \brief sets the property plotFunction ( \copybrief plotFunction ) to the specified \a __value.
         *
         *  \details Description of the parameter plotFunction is: <BLOCKQUOTE>\copydoc plotFunction </BLOCKQUOTE>
         * \see plotFunction for more information */
        virtual void setPlotFunction (jkqtpSimplePlotFunctionType && __value);
        /** \brief sets the property plotFunction ( \copybrief plotFunction ) to the specified \a __value.
         *
         *  \details Description of the parameter plotFunction is: <BLOCKQUOTE>\copydoc plotFunction </BLOCKQUOTE>
         * \see plotFunction for more information */
        virtual void setPlotFunction (const jkqtpSimplePlotFunctionType & __value);
        /*! \brief returns the property plotFunction ( \copybrief plotFunction ). \see plotFunction for more information */ \
        virtual jkqtpPlotFunctionType getPlotFunction () const;
        /*! \brief returns the property simplePlotFunction ( \copybrief simplePlotFunction ). \see simplePlotFunction for more information */ \
        virtual jkqtpSimplePlotFunctionType getSimplePlotFunction () const;

        /*! \brief sets the property params ( \copybrief params ) to the specified \a __value. 
            \details Description of the parameter params is: <BLOCKQUOTE>\copydoc params </BLOCKQUOTE> 
            \see params for more information */ 
        inline virtual void setParams(void* __value)
        {
            if (this->params != __value) { 
                this->params = __value; 
                clearData(); 
            } 
        } 
        /*! \brief returns the property params ( \copybrief params ). 
            \details Description of the parameter params is: <BLOCKQUOTE>\copydoc params </BLOCKQUOTE> 
            \see params for more information */ 
        inline virtual void* getParams() const  
        {
            return this->params; 
        }
        /** \brief sets the params as a pointer to an internal COPY of the given vector (not the data of the vector, as then the size would be unknown!!!) */
        void setParams(const QVector<double>& params);
        /** \brief sets the params from a copy of the given array of length \a N */
        void setCopiedParams(const double* params, int N);
        /** \brief set an internal parameter vector as function parameters, initialized with {p1} */
        void setParamsV(double p1);
        /** \brief set an internal parameter vector as function parameters, initialized with {p1,p2} */
        void setParamsV(double p1, double p2);
        /** \brief set an internal parameter vector as function parameters, initialized with {p1,p2,p3} */
        void setParamsV(double p1, double p2, double p3);
        /** \brief set an internal parameter vector as function parameters, initialized with {p1,p2,p3,p4} */
        void setParamsV(double p1, double p2, double p3, double p4);
        /** \brief set an internal parameter vector as function parameters, initialized with {p1,p2,p3,p4,p5} */
        void setParamsV(double p1, double p2, double p3, double p4, double p5);

        /** \brief returns the currently set internal parameter vector */
        QVector<double> getInternalParams() const;
        /** \brief returns the currently set internal parameter vector */
        QVector<double> getInternalErrorParams() const;
        /*! \brief sets the property minSamples ( \copybrief minSamples ) to the specified \a __value. 
            \details Description of the parameter minSamples is: <BLOCKQUOTE>\copydoc minSamples </BLOCKQUOTE> 
            \see minSamples for more information */ 
        inline virtual void setMinSamples(const unsigned int & __value)  
        {
            this->minSamples = __value;
        } 
        /*! \brief returns the property minSamples ( \copybrief minSamples ). 
            \details Description of the parameter minSamples is: <BLOCKQUOTE>\copydoc minSamples </BLOCKQUOTE> 
            \see minSamples for more information */ 
        inline virtual unsigned int getMinSamples() const  
        {
            return this->minSamples; 
        }
        /*! \brief sets the property maxRefinementDegree ( \copybrief maxRefinementDegree ) to the specified \a __value. 
            \details Description of the parameter maxRefinementDegree is: <BLOCKQUOTE>\copydoc maxRefinementDegree </BLOCKQUOTE> 
            \see maxRefinementDegree for more information */ 
        inline virtual void setMaxRefinementDegree(const unsigned int & __value)  
        {
            this->maxRefinementDegree = __value;
        } 
        /*! \brief returns the property maxRefinementDegree ( \copybrief maxRefinementDegree ). 
            \details Description of the parameter maxRefinementDegree is: <BLOCKQUOTE>\copydoc maxRefinementDegree </BLOCKQUOTE> 
            \see maxRefinementDegree for more information */ 
        inline virtual unsigned int getMaxRefinementDegree() const  
        {
            return this->maxRefinementDegree; 
        }
        /*! \brief sets the property slopeTolerance ( \copybrief slopeTolerance ) to the specified \a __value. 
            \details Description of the parameter slopeTolerance is: <BLOCKQUOTE>\copydoc slopeTolerance </BLOCKQUOTE> 
            \see slopeTolerance for more information */ 
        inline virtual void setSlopeTolerance(double __value)
        {
            this->slopeTolerance = __value;
        } 
        /*! \brief returns the property slopeTolerance ( \copybrief slopeTolerance ). 
            \details Description of the parameter slopeTolerance is: <BLOCKQUOTE>\copydoc slopeTolerance </BLOCKQUOTE> 
            \see slopeTolerance for more information */ 
        inline virtual double getSlopeTolerance() const  
        {
            return this->slopeTolerance; 
        }
        /*! \brief sets the property minPixelPerSample ( \copybrief minPixelPerSample ) to the specified \a __value. 
            \details Description of the parameter minPixelPerSample is: <BLOCKQUOTE>\copydoc minPixelPerSample </BLOCKQUOTE> 
            \see minPixelPerSample for more information */ 
        inline virtual void setMinPixelPerSample(double __value)
        {
            this->minPixelPerSample = __value;
        } 
        /*! \brief returns the property minPixelPerSample ( \copybrief minPixelPerSample ). 
            \details Description of the parameter minPixelPerSample is: <BLOCKQUOTE>\copydoc minPixelPerSample </BLOCKQUOTE> 
            \see minPixelPerSample for more information */ 
        inline virtual double getMinPixelPerSample() const  
        {
            return this->minPixelPerSample; 
        }
        /*! \brief sets the property plotRefinement ( \copybrief plotRefinement ) to the specified \a __value. 
            \details Description of the parameter plotRefinement is: <BLOCKQUOTE>\copydoc plotRefinement </BLOCKQUOTE> 
            \see plotRefinement for more information */ 
        inline virtual void setPlotRefinement(bool __value)
        {
            this->plotRefinement = __value;
        } 
        /*! \brief returns the property plotRefinement ( \copybrief plotRefinement ). 
            \details Description of the parameter plotRefinement is: <BLOCKQUOTE>\copydoc plotRefinement </BLOCKQUOTE> 
            \see plotRefinement for more information */ 
        inline virtual bool getPlotRefinement() const  
        {
            return this->plotRefinement; 
        }
        /*! \brief sets the property displaySamplePoints ( \copybrief displaySamplePoints ) to the specified \a __value. 
            \details Description of the parameter displaySamplePoints is: <BLOCKQUOTE>\copydoc displaySamplePoints </BLOCKQUOTE> 
            \see displaySamplePoints for more information */ 
        inline virtual void setDisplaySamplePoints(bool __value)
        {
            this->displaySamplePoints = __value;
        } 
        /*! \brief returns the property displaySamplePoints ( \copybrief displaySamplePoints ). 
            \details Description of the parameter displaySamplePoints is: <BLOCKQUOTE>\copydoc displaySamplePoints </BLOCKQUOTE> 
            \see displaySamplePoints for more information */ 
        inline virtual bool getDisplaySamplePoints() const  
        {
            return this->displaySamplePoints; 
        }
        /*! \brief sets the property drawErrorPolygons ( \copybrief drawErrorPolygons ) to the specified \a __value. 
            \details Description of the parameter drawErrorPolygons is: <BLOCKQUOTE>\copydoc drawErrorPolygons </BLOCKQUOTE> 
            \see drawErrorPolygons for more information */ 
        inline virtual void setDrawErrorPolygons(bool __value)
        {
            this->drawErrorPolygons = __value;
        } 
        /*! \brief returns the property drawErrorPolygons ( \copybrief drawErrorPolygons ). 
            \details Description of the parameter drawErrorPolygons is: <BLOCKQUOTE>\copydoc drawErrorPolygons </BLOCKQUOTE> 
            \see drawErrorPolygons for more information */ 
        inline virtual bool getDrawErrorPolygons() const  
        {
            return this->drawErrorPolygons; 
        }
        /*! \brief sets the property drawErrorLines ( \copybrief drawErrorLines ) to the specified \a __value. 
            \details Description of the parameter drawErrorLines is: <BLOCKQUOTE>\copydoc drawErrorLines </BLOCKQUOTE> 
            \see drawErrorLines for more information */ 
        inline virtual void setDrawErrorLines(bool __value)
        {
            this->drawErrorLines = __value;
        } 
        /*! \brief returns the property drawErrorLines ( \copybrief drawErrorLines ). 
            \details Description of the parameter drawErrorLines is: <BLOCKQUOTE>\copydoc drawErrorLines </BLOCKQUOTE> 
            \see drawErrorLines for more information */ 
        inline virtual bool getDrawErrorLines() const  
        {
            return this->drawErrorLines; 
        }
        /** \brief sets the property errorPlotFunction ( \copybrief errorPlotFunction ) to the specified \a __value.
         *
         *  \details Description of the parameter errorPlotFunction is: <BLOCKQUOTE>\copydoc errorPlotFunction </BLOCKQUOTE>
         * \see errorPlotFunction for more information */
        virtual void setErrorPlotFunction (jkqtpPlotFunctionType && __value);
        /** \brief sets the property errorPlotFunction ( \copybrief errorPlotFunction ) to the specified \a __value.
         *
         *  \details Description of the parameter errorPlotFunction is: <BLOCKQUOTE>\copydoc errorPlotFunction </BLOCKQUOTE>
         * \see errorPlotFunction for more information */
        virtual void setErrorPlotFunction (const jkqtpPlotFunctionType & __value);
        /*! \brief returns the property errorPlotFunction ( \copybrief errorPlotFunction ). \see errorPlotFunction for more information */ \
        virtual jkqtpPlotFunctionType getErrorPlotFunction () const;
        /** \brief sets the property errorPlotFunction ( \copybrief errorPlotFunction ) to the specified \a __value.
         *
         *  \details Description of the parameter errorPlotFunction is: <BLOCKQUOTE>\copydoc errorPlotFunction </BLOCKQUOTE>
         * \see errorPlotFunction for more information */
        virtual void setErrorPlotFunction (jkqtpSimplePlotFunctionType && __value);
        /** \brief sets the property errorPlotFunction ( \copybrief errorPlotFunction ) to the specified \a __value.
         *
         *  \details Description of the parameter errorPlotFunction is: <BLOCKQUOTE>\copydoc errorPlotFunction </BLOCKQUOTE>
         * \see errorPlotFunction for more information */
        virtual void setErrorPlotFunction (const jkqtpSimplePlotFunctionType & __value);
        /*! \brief returns the property errorSimplePlotFunction ( \copybrief errorSimplePlotFunction ). \see errorSimplePlotFunction for more information */ \
        virtual jkqtpSimplePlotFunctionType getErrorSimplePlotFunction () const;
        /*! \brief sets the property errorParams ( \copybrief errorParams ) to the specified \a __value. 
            \details Description of the parameter errorParams is: <BLOCKQUOTE>\copydoc errorParams </BLOCKQUOTE> 
            \see errorParams for more information */ 
        inline virtual void setErrorParams(void* __value)
        {
            this->errorParams = __value;
        } 
        /*! \brief returns the property errorParams ( \copybrief errorParams ). 
            \details Description of the parameter errorParams is: <BLOCKQUOTE>\copydoc errorParams </BLOCKQUOTE> 
            \see errorParams for more information */ 
        inline virtual void* getErrorParams() const  
        {
            return this->errorParams; 
        }
        /** \brief sets the error params as a pointer to an internal COPY of the given vector (not the data of the vector, as then the size would be unknown!!!) */
        void setErrorParams(const QVector<double>& errorParams);

        /*! \brief sets the property parameterColumn ( \copybrief parameterColumn ) to the specified \a __value. 
            \details Description of the parameter parameterColumn is: <BLOCKQUOTE>\copydoc parameterColumn </BLOCKQUOTE> 
            \see parameterColumn for more information */ 
        inline virtual void setParameterColumn(int __value)
        {
            this->parameterColumn = __value;
        } 
        /*! \brief returns the property parameterColumn ( \copybrief parameterColumn ). 
            \details Description of the parameter parameterColumn is: <BLOCKQUOTE>\copydoc parameterColumn </BLOCKQUOTE> 
            \see parameterColumn for more information */ 
        inline virtual int getParameterColumn() const  
        {
            return this->parameterColumn; 
        }
        /*! \brief sets the property parameterColumn ( \copybrief parameterColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter parameterColumn is:  <BLOCKQUOTE>\copydoc parameterColumn </BLOCKQUOTE> 
        	\see parameterColumn for more information */ 
        inline virtual void setParameterColumn (size_t __value) { this->parameterColumn = static_cast<int>(__value); }
        /*! \brief sets the property errorParameterColumn ( \copybrief errorParameterColumn ) to the specified \a __value. 
            \details Description of the parameter errorParameterColumn is: <BLOCKQUOTE>\copydoc errorParameterColumn </BLOCKQUOTE> 
            \see errorParameterColumn for more information */ 
        inline virtual void setErrorParameterColumn(int __value)
        {
            this->errorParameterColumn = __value;
        } 
        /*! \brief returns the property errorParameterColumn ( \copybrief errorParameterColumn ). 
            \details Description of the parameter errorParameterColumn is: <BLOCKQUOTE>\copydoc errorParameterColumn </BLOCKQUOTE> 
            \see errorParameterColumn for more information */ 
        inline virtual int getErrorParameterColumn() const  
        {
            return this->errorParameterColumn; 
        }
        /*! \brief sets the property errorParameterColumn ( \copybrief errorParameterColumn ) to the specified \a __value, where __value is static_cast'ed from size_t to int. 
            \details Description of the parameter errorParameterColumn is:  <BLOCKQUOTE>\copydoc errorParameterColumn </BLOCKQUOTE> 
        	\see errorParameterColumn for more information */ 
        inline virtual void setErrorParameterColumn (size_t __value) { this->errorParameterColumn = static_cast<int>(__value); }

        /*! \brief sets the property errorColor ( \copybrief errorColor ) to the specified \a __value. 
            \details Description of the parameter errorColor is: <BLOCKQUOTE>\copydoc errorColor </BLOCKQUOTE> 
            \see errorColor for more information */ 
        inline virtual void setErrorColor(const QColor & __value)  
        {
            this->errorColor = __value;
        } 
        /*! \brief returns the property errorColor ( \copybrief errorColor ). 
            \details Description of the parameter errorColor is: <BLOCKQUOTE>\copydoc errorColor </BLOCKQUOTE> 
            \see errorColor for more information */ 
        inline virtual QColor getErrorColor() const  
        {
            return this->errorColor; 
        }
        /*! \brief sets the property errorFillColor ( \copybrief errorFillColor ) to the specified \a __value. 
            \details Description of the parameter errorFillColor is: <BLOCKQUOTE>\copydoc errorFillColor </BLOCKQUOTE> 
            \see errorFillColor for more information */ 
        inline virtual void setErrorFillColor(const QColor & __value)  
        {
            this->errorFillColor = __value;
        } 
        /*! \brief returns the property errorFillColor ( \copybrief errorFillColor ). 
            \details Description of the parameter errorFillColor is: <BLOCKQUOTE>\copydoc errorFillColor </BLOCKQUOTE> 
            \see errorFillColor for more information */ 
        inline virtual QColor getErrorFillColor() const  
        {
            return this->errorFillColor; 
        }
        /*! \brief sets the property errorFillStyle ( \copybrief errorFillStyle ) to the specified \a __value. 
            \details Description of the parameter errorFillStyle is: <BLOCKQUOTE>\copydoc errorFillStyle </BLOCKQUOTE> 
            \see errorFillStyle for more information */ 
        inline virtual void setErrorFillStyle(const Qt::BrushStyle & __value)  
        {
            this->errorFillStyle = __value;
        } 
        /*! \brief returns the property errorFillStyle ( \copybrief errorFillStyle ). 
            \details Description of the parameter errorFillStyle is: <BLOCKQUOTE>\copydoc errorFillStyle </BLOCKQUOTE> 
            \see errorFillStyle for more information */ 
        inline virtual Qt::BrushStyle getErrorFillStyle() const  
        {
            return this->errorFillStyle; 
        }
        /*! \brief sets the property errorStyle ( \copybrief errorStyle ) to the specified \a __value. 
            \details Description of the parameter errorStyle is: <BLOCKQUOTE>\copydoc errorStyle </BLOCKQUOTE> 
            \see errorStyle for more information */ 
        inline virtual void setErrorStyle(const Qt::PenStyle & __value)  
        {
            this->errorStyle = __value;
        } 
        /*! \brief returns the property errorStyle ( \copybrief errorStyle ). 
            \details Description of the parameter errorStyle is: <BLOCKQUOTE>\copydoc errorStyle </BLOCKQUOTE> 
            \see errorStyle for more information */ 
        inline virtual Qt::PenStyle getErrorStyle() const  
        {
            return this->errorStyle; 
        }
        /*! \brief sets the property errorLineWidth ( \copybrief errorLineWidth ) to the specified \a __value. 
            \details Description of the parameter errorLineWidth is: <BLOCKQUOTE>\copydoc errorLineWidth </BLOCKQUOTE> 
            \see errorLineWidth for more information */ 
        inline virtual void setErrorLineWidth(double __value)
        {
            this->errorLineWidth = __value;
        } 
        /*! \brief returns the property errorLineWidth ( \copybrief errorLineWidth ). 
            \details Description of the parameter errorLineWidth is: <BLOCKQUOTE>\copydoc errorLineWidth </BLOCKQUOTE> 
            \see errorLineWidth for more information */ 
        inline virtual double getErrorLineWidth() const  
        {
            return this->errorLineWidth; 
        }

        /** \copydoc JKQTPGraph::usesColumn() */
        virtual bool usesColumn(int c) const override;


        /** \brief sets function to the given special function */
        void setSpecialFunction(SpecialFunction function);
        /** \brief returns, which special function is set (or if any is set) */
        SpecialFunction getFunctionType() const;
    protected:
        /** \brief which plot style to use from the parent plotter (via JKQTBasePlotter::getPlotStyle() and JKQTBasePlotter::getNextStyle() ) */
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
    \ingroup jkqtplotter_functiongraphs

 */
class LIB_EXPORT JKQTPYFunctionLineGraph: public JKQTPXFunctionLineGraph {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPYFunctionLineGraph(JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor */
        JKQTPYFunctionLineGraph(JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
    protected:

        /** \brief fill the data array with data from the function plotFunction */
        virtual void createPlotData( bool collectParams=true) override;

};




#endif // jkqtpgraphsevaluatedfunction_H
