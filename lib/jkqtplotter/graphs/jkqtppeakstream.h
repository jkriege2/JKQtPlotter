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

#ifndef jkqtpgraphspeakstream_H_INCLUDED
#define jkqtpgraphspeakstream_H_INCLUDED



#include <QString>
#include <QPainter>
#include <QPair>
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpgraphsbasestylingmixins.h"

// forward declarations
class JKQTBasePlotter;
class JKQTPlotter;
class JKQTPCoordinateAxis;
class JKQTPDatastore;


/*! \brief interprets data as a stream of x- or y-positions \f$ p_i \f$ (depending in \a yPeaks ) ans plots a line on the height
           \a baseline and upright lines from baseline to baseline+peakHeight at each position \f$ p_i \f$. This can be used to
           display a stream of photons with given arrivaltimes \f$ p_i \f$.
    \ingroup jkqtplotter_diverse

    \image html JKQTPPeakStreamGraphY.png "yPeaks=true"
    \image html JKQTPPeakStreamGraphX.png "yPeaks=false"

    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPPeakStreamGraph: public JKQTPSingleColumnGraph, public JKQTPGraphLineStyleMixin {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTPPeakStreamGraph(JKQTBasePlotter* parent=nullptr);
        JKQTPPeakStreamGraph(JKQTPlotter* parent);

        /** \brief get the maximum and minimum x-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) override;
        /** \brief get the maximum and minimum y-value of the graph
         *
         * The result is given in the two parameters which are call-by-reference parameters!
         */
        virtual bool getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) override;


        /** \brief plots the graph to the plotter object specified as parent */
        virtual void draw(JKQTPEnhancedPainter& painter) override;
        /** \brief plots a key marker inside the specified rectangle \a rect */
        virtual void drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) override;
        /** \brief returns the color to be used for the key label */
        virtual QColor getKeyLabelColor() const override;
        /** \brief set symbol color and fill color at the same time */
        void setColor(QColor col);
        /** \copydoc baseline */ 
        void setBaseline(double __value);
        /** \copydoc baseline */ 
        double getBaseline() const;
        /** \copydoc peakHeight */ 
        void setPeakHeight(double __value);
        /** \copydoc peakHeight */ 
        double getPeakHeight() const;
        /** \copydoc yPeaks */ 
        void setYPeaks(bool __value);
        /** \copydoc yPeaks */ 
        bool getYPeaks() const;
        /** \copydoc drawBaseline */ 
        void setDrawBaseline(bool __value);
        /** \copydoc drawBaseline */ 
        bool getDrawBaseline() const;

    protected:

        /** \brief position of the baseline */
        double baseline;
        /** \brief height of each peak */
        double peakHeight;
        /** \brief if set \c true the peaks are in Y direction (default: true)*/
        bool yPeaks;
        /** \brief indicates whether to draw the basleine (default: \c true ) */
        bool drawBaseline;

};




#endif // jkqtpgraphspeakstream_H_INCLUDED
