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

#ifndef jkqtpgraphlabels_H
#define jkqtpgraphlabels_H


#include <QString>
#include <QPainter>
#include <QPair>
#include <functional>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/graphs/jkqtpgraphlabelstylemixin.h"

// forward declarations
class JKQTBasePlotter;
class JKQTPlotter;
class JKQTPCoordinateAxis;
class JKQTPDatastore;



/*! \brief This graph plots a series of data labels. This can be used to add number-labels to e.g. a barchart.
    \ingroup jkqtplotter_graphlabels

    Sometimes it is necessary to display the numeric value of a datapoint right in the plot.
    To achieve this, JKQTPXYGraphLabels is provided, which draws a text-label next to each datapoint in a
    series of x- and y-values (thus it is derived from JKQTPXYGraph). Each label is drawn with a little offset
    from the actual datapoint. Under each label, a small (stylable) box is drawn, which is by default simply
    the same color as the background color of complete plot, but with transparency, thus providing a certain
    level of contrast to the text, even if drawn above other plot elements. Use the properties of the parent
    mix-in class JKQTPGraphValueLabelStyleMixin to style the text and the box.

    Here is an example of labels together with a bars and lines chart:

    \image html JKQTPGLabelAwayFromXAxis.png

    To achieve this, use code like this:
    \code
        // create barchart and line-chart to display the data:
        JKQTPBarVerticalGraph* graph1=new JKQTPBarVerticalGraph(&plot);
        graph1->setBarPositionColumn(columnX);
        graph1->setBarHeightColumn(columnY);

        JKQTPXYLineGraph* graph2=new JKQTPXYLineGraph(&plot);
        graph2->setXColumn(columnX);
        graph2->setYColumn(columnY);

        // create the data labels:
        JKQTPXYGraphLabels* graphLabels=new JKQTPXYGraphLabels(JKQTPXYGraphLabels::YValueLabel, &plot);
        graphLabels->setXColumn(graph1->getXColumn());
        graphLabels->setYColumn(graph1->getYColumn());
        graphLabels->setLabelPosition(JKQTPGLabelAwayFromXAxis),

        // add the graphs to the plot, so it is actually displayed
        plot.addGraph(graph1);
        plot.addGraph(graph2);
        plot.addGraph(graphLabels);
    \endcode

    The text shown in the label is determined by a functor of type JKQTPXYGraphLabels::LabelGenerator that calculates
    it from the labels position and data-index.
    This functor can be custimized (set setCustomLabelGenerator() or the matching constructor), or it may be one of
    three default immplementations, that display:
      - the current x-value (JKQTPXYGraphLabels::XValueLabel constructor arguent or setDefaultXLabelGenerator() )
      - the current y-value (JKQTPXYGraphLabels::YValueLabel constructor arguent or setDefaultYLabelGenerator() )
      - the current x- and y-value (JKQTPXYGraphLabels::XYValueLabel constructor arguent or setDefaultXYLabelGenerator() )
    .

    The default generators can be further customized with a series of properties on how to format their output:
      - setXValueLabelFormat(), setYValueLabelFormat(), setXYValueLabelFormat() ... these set the basic label and are
        typically only \c "$%1$" or \c "$%1/%2$" , but you can set them to any string (\c %1 / \c %2 is replaced by
        the string-converted x-/y-values) and e.g. add a prefix, like \c setXValueLabelFormat("$x=%1$").
        You can use any LaTeX-markup that is allowed by JKQTMathText here!
      - setXDefaultConverter(), setXDefaultPrecision(), setXBelowIsZero(), ... (all these exist in X and Y variants!)
    .

    \see \ref JKQTPlotterGraphLabels

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPXYGraphLabels: public JKQTPXYGraph, public JKQTPGraphValueLabelStyleMixin, public JKQTPXYLabelsGeneratorMixin {
        Q_OBJECT
    public:
        enum LabelContentsDefaultType {
            XValueLabel, //!< \brief generates a label with the x-coordinate only, calls setDefaultXLabelGenerator()
            YValueLabel, //!< \brief generates a label with the y-coordinate only, calls setDefaultYLabelGenerator()
            XYValueLabel //!< \brief generates a label with the x- and y-coordinate, calls setDefaultXYLabelGenerator()
        };
        Q_ENUM(LabelContentsDefaultType)
        /** \brief type of a functor that generates a label */
        typedef std::function<QString(double,double,int)> LabelGenerator;

        /** \brief class constructor, generates a JKQTPXYGraphLabels with one of the default label generator functors (x-value, y-value or x+y-values) */
        explicit JKQTPXYGraphLabels(LabelContentsDefaultType lt, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor, generates a JKQTPXYGraphLabels with one of the default label generator functors (x-value, y-value or x+y-values) */
        JKQTPXYGraphLabels(LabelContentsDefaultType lt, JKQTPlotter* parent);
        /** \brief class constructor, generates a JKQTPXYGraphLabels with a custom LabelGenerator functor */
        JKQTPXYGraphLabels(const LabelGenerator& lgen, JKQTBasePlotter* parent=nullptr);
        /** \brief class constructor, generates a JKQTPXYGraphLabels with a custom LabelGenerator functor */
        JKQTPXYGraphLabels(const LabelGenerator& lgen, JKQTPlotter* parent);

        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, const QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;
        /** \copydoc JKQTPPlotElement::getXMinMax() */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \copydoc JKQTPPlotElement::getYMinMax() */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;


        /** \brief set color of line and symbol */
        void setColor(QColor c, bool setTextColor_=true, bool setFrameColor_=true);
        /** \brief sets a custom label geneator function of type LabelGenerator to use
         *
         *  \see m_labelGenerator, setDefaultXLabelGenerator(), setDefaultYLabelGenerator(), setDefaultXYLabelGenerator()
         */
        void setCustomLabelGenerator(const LabelGenerator& labgen);
        /** \brief sets the label geneator function to a default implementation, that prints the x-value only
         *
         *  The generator uses m_xValueLabelFormat, m_xDefaultConverter, ...
         *  \see m_labelGenerator, setCustomLabelGenerator(), setDefaultXLabelGenerator(), setDefaultYLabelGenerator(), setDefaultXYLabelGenerator()
         */
        void setDefaultXLabelGenerator();
        /** \brief sets the label geneator function to a default implementation, that prints the y-value only
         *
         *  The generator uses m_yValueLabelFormat, m_yDefaultConverter, ...
         *  \see m_labelGenerator, setCustomLabelGenerator(), setDefaultXLabelGenerator(), setDefaultYLabelGenerator(), setDefaultXYLabelGenerator()
         */
        void setDefaultYLabelGenerator();
        /** \brief sets the label geneator function to a default implementation, that prints the x- and y-value
         *
         *  The generator uses m_xyValueLabelFormat, m_xDefaultConverter, m_yDefaultConverter, ...
         *  \see m_labelGenerator, setCustomLabelGenerator(), setDefaultXLabelGenerator(), setDefaultYLabelGenerator(), setDefaultXYLabelGenerator()
         */
        void setDefaultXYLabelGenerator();
    protected:
        /** \brief generates the label at position (\a x , \a y ) and at the given data \a index */
        QString generateLabel(double x, double y, int index) const;
    private:
        /** \brief generate the label for the datapoint at location (\a x , \a y ) and being the \a index -th value in the columns */
        LabelGenerator m_labelGenerator;
};




#endif // jkqtpgraphlabels_H
