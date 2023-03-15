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

#ifndef JKQTPGRAPHSSTATISTICSADAPTORS_H_INCLUDED
#define JKQTPGRAPHSSTATISTICSADAPTORS_H_INCLUDED



#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtmath/jkqtpstatisticstools.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtplotter/jkqtpgraphsbaseerrors.h"
#include "jkqtplotter/graphs/jkqtpboxplot.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter/graphs/jkqtpsinglecolumnsymbols.h"
#include "jkqtplotter/graphs/jkqtpbarchart.h"
#include "jkqtplotter/graphs/jkqtpevaluatedfunction.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/graphs/jkqtpcontour.h"
#include "jkqtplotter/graphs/jkqtpimpulses.h"
#include "jkqtplotter/graphs/jkqtpfilledcurve.h"
#include "jkqtplotter/graphs/jkqtpviolinplot.h"

/*! \brief add a JKQTPBoxplotHorizontalElement to the given plotter, where the boxplot values are calculated from the data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param boxposY y-coordinate of the boxplot
    \param quantile1Spec specifies which quantile to calculate for \a qantile1 (range: 0..1)
    \param quantile2Spec specifies which quantile to calculate for \a qantile2 (range: 0..1)
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \param[out] statOutput optionally returns the internally calculated statistics as a JKQTPStat5NumberStatistics
    \return a boxplot element with its values initialized from the given data range

    Example:
    \code
        jkqtpstatAddHBoxplot(plot1->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -0.3);
    \endcode

    \image html datastore_statistics_boxplots_simple.png


    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstat5NumberStatistics()
*/
template <class InputIt>
inline JKQTPBoxplotHorizontalElement* jkqtpstatAddHBoxplot(JKQTBasePlotter* plotter, InputIt first, InputIt last, double boxposY, double quantile1Spec=0.25, double quantile2Spec=0.75, double minimumQuantile=0, double maximumQuantile=1.0, JKQTPStat5NumberStatistics* statOutput=nullptr) {
    JKQTPStat5NumberStatistics stat=jkqtpstat5NumberStatistics(first, last, quantile1Spec, quantile2Spec, minimumQuantile, maximumQuantile);
    if (statOutput) *statOutput=stat;
    JKQTPBoxplotHorizontalElement* res=new JKQTPBoxplotHorizontalElement(plotter);
    res->setMin(stat.minimum);
    res->setMax(stat.maximum);
    res->setMedian(stat.median);
    res->setMean(jkqtpstatAverage(first, last));
    res->setPercentile25(stat.quantile1);
    res->setPercentile75(stat.quantile2);
    res->setMedianConfidenceIntervalWidth(stat.IQRSignificanceEstimate());
    res->setDrawMean(true);
    res->setDrawNotch(true);
    res->setDrawMedian(true);
    res->setDrawMinMax(true);
    res->setDrawBox(true);
    res->setPos(boxposY);
    plotter->addGraph(res);
    return res;
}


/*! \brief add a JKQTPBoxplotVerticalElement to the given plotter, where the boxplot values are calculated from the data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param boxposX x-coordinate of the boxplot
    \param quantile1Spec specifies which quantile to calculate for \a qantile1 (range: 0..1)
    \param quantile2Spec specifies which quantile to calculate for \a qantile2 (range: 0..1)
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \param[out] statOutput optionally returns the internally calculated statistics as a JKQTPStat5NumberStatistics
    \return a boxplot element with its values initialized from the given data range

    Example:
    \code
        jkqtpstatAddVBoxplot(plot1->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -0.3);
    \endcode

    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstat5NumberStatistics()
*/
template <class InputIt>
inline JKQTPBoxplotVerticalElement* jkqtpstatAddVBoxplot(JKQTBasePlotter* plotter, InputIt first, InputIt last, double boxposX, double quantile1Spec=0.25, double quantile2Spec=0.75, double minimumQuantile=0, double maximumQuantile=1.0, JKQTPStat5NumberStatistics* statOutput=nullptr) {
    JKQTPStat5NumberStatistics stat=jkqtpstat5NumberStatistics(first, last, quantile1Spec, quantile2Spec, minimumQuantile, maximumQuantile);
    if (statOutput) *statOutput=stat;
    JKQTPBoxplotVerticalElement* res=new JKQTPBoxplotVerticalElement(plotter);
    res->setMin(stat.minimum);
    res->setMax(stat.maximum);
    res->setMedian(stat.median);
    res->setMean(jkqtpstatAverage(first, last));
    res->setPercentile25(stat.quantile1);
    res->setPercentile75(stat.quantile2);
    res->setMedianConfidenceIntervalWidth(stat.IQRSignificanceEstimate());
    res->setDrawMean(true);
    res->setDrawNotch(true);
    res->setDrawMedian(true);
    res->setDrawMinMax(true);
    res->setDrawBox(true);
    res->setPos(boxposX);
    plotter->addGraph(res);
    return res;
}



/*! \brief add a JKQTPBoxplotHorizontalElement and a JKQTPSingleColumnSymbolsGraph for outliers  to the given plotter, where the boxplot values are calculated from the data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param boxposY y-coordinate of the outliers (and the boxplot)
    \param quantile1Spec specifies which quantile to calculate for \a qantile1 (range: 0..1)
    \param quantile2Spec specifies which quantile to calculate for \a qantile2 (range: 0..1)
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \param outliercolumnBaseName this string is used in building the column names for the outlier columns
    \param[out] statOutput optionally returns the internally calculated statistics as a JKQTPStat5NumberStatistics
    \return a boxplot element with its values initialized from the given data range

    Example:
    \code
        jkqtpstatAddHBoxplotAndOutliers(plot1->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -0.3);
        jkqtpstatAddHBoxplotAndOutliers(plot1->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -0.3,
                                        0.25, 0.75, // 1. and 3. Quartile for the boxplot box
                                        0.05, 0.95 // Quantiles for the boxplot box whiskers' ends
    \endcode

    \image html datastore_statistics_boxplots_outliers.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstat5NumberStatistics()
*/
template <class InputIt>
inline std::pair<JKQTPBoxplotHorizontalElement*,JKQTPSingleColumnSymbolsGraph*> jkqtpstatAddHBoxplotAndOutliers(JKQTBasePlotter* plotter, InputIt first, InputIt last, double boxposY, double quantile1Spec=0.25, double quantile2Spec=0.75, double minimumQuantile=0.03, double maximumQuantile=0.97, const QString& outliercolumnBaseName=QString("boxplot"), JKQTPStat5NumberStatistics* statOutput=nullptr) {
    JKQTPStat5NumberStatistics stat=jkqtpstat5NumberStatistics(first, last, quantile1Spec, quantile2Spec, minimumQuantile, maximumQuantile);
    if (statOutput) *statOutput=stat;
    JKQTPBoxplotHorizontalElement* resB=new JKQTPBoxplotHorizontalElement(plotter);
    resB->setMin(stat.minimum);
    resB->setMax(stat.maximum);
    resB->setMedian(stat.median);
    resB->setMean(jkqtpstatAverage(first, last));
    resB->setPercentile25(stat.quantile1);
    resB->setPercentile75(stat.quantile2);
    resB->setMedianConfidenceIntervalWidth(stat.IQRSignificanceEstimate());
    resB->setDrawMean(true);
    resB->setDrawNotch(true);
    resB->setDrawMedian(true);
    resB->setDrawMinMax(true);
    resB->setDrawBox(true);
    resB->setPos(boxposY);
    JKQTPSingleColumnSymbolsGraph* resO=new JKQTPSingleColumnSymbolsGraph(plotter);
    resO->setDataColumn(plotter->getDatastore()->addCopiedColumn(stat.outliers, outliercolumnBaseName));
    resO->setPosition(boxposY);
    resO->setPositionScatterStyle(JKQTPSingleColumnSymbolsGraph::NoScatter);
    resO->setDataDirection(JKQTPSingleColumnSymbolsGraph::DataDirection::X);
    resO->setColor(resB->getKeyLabelColor());
    resO->setTitle("");

    plotter->addGraph(resB);
    plotter->addGraph(resO);
    return std::pair<JKQTPBoxplotHorizontalElement*,JKQTPSingleColumnSymbolsGraph*>(resB, resO);
}



/*! \brief add a JKQTPBoxplotVerticalElement and a JKQTPSingleColumnSymbolsGraph for outliers to the given plotter, where the boxplot values are calculated from the data range \a first ... \a last
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param boxposX x-coordinate of the outliers (and the boxplot)
    \param quantile1Spec specifies which quantile to calculate for \a qantile1 (range: 0..1)
    \param quantile2Spec specifies which quantile to calculate for \a qantile2 (range: 0..1)
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0.03, i.e. the 3% quantile!)
    \param maximumQuantile specifies a quantile for the return value maximum (default is 0.97, i.e. the 97% quantile!)
    \param outliercolumnBaseName this string is used in building the column names for the outlier columns
    \param[out] statOutput optionally returns the internally calculated statistics as a JKQTPStat5NumberStatistics
    \return a boxplot element with its values initialized from the given data range

    Example:
    \code
        jkqtpstatAddVBoxplotAndOutliers(plot1->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -0.3);
        jkqtpstatAddVBoxplotAndOutliers(plot1->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -0.3,
                                        0.25, 0.75, // 1. and 3. Quartile for the boxplot box
                                        0.05, 0.95 // Quantiles for the boxplot box whiskers' ends
    \endcode    

    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstat5NumberStatistics()
*/
template <class InputIt>
inline std::pair<JKQTPBoxplotVerticalElement*,JKQTPSingleColumnSymbolsGraph*> jkqtpstatAddVBoxplotAndOutliers(JKQTBasePlotter* plotter, InputIt first, InputIt last, double boxposX, double quantile1Spec=0.25, double quantile2Spec=0.75, double minimumQuantile=0.03, double maximumQuantile=0.97, const QString& outliercolumnBaseName=QString("boxplot"), JKQTPStat5NumberStatistics* statOutput=nullptr) {
    JKQTPStat5NumberStatistics stat=jkqtpstat5NumberStatistics(first, last, quantile1Spec, quantile2Spec, minimumQuantile, maximumQuantile);
    if (statOutput) *statOutput=stat;
    JKQTPBoxplotVerticalElement* resB=new JKQTPBoxplotVerticalElement(plotter);
    resB->setMin(stat.minimum);
    resB->setMax(stat.maximum);
    resB->setMedian(stat.median);
    resB->setMean(jkqtpstatAverage(first, last));
    resB->setPercentile25(stat.quantile1);
    resB->setPercentile75(stat.quantile2);
    resB->setMedianConfidenceIntervalWidth(stat.IQRSignificanceEstimate());
    resB->setDrawMean(true);
    resB->setDrawNotch(true);
    resB->setDrawMedian(true);
    resB->setDrawMinMax(true);
    resB->setDrawBox(true);
    resB->setPos(boxposX);
    JKQTPSingleColumnSymbolsGraph* resO=new JKQTPSingleColumnSymbolsGraph(plotter);
    resO->setDataColumn(plotter->getDatastore()->addCopiedColumn(stat.outliers, outliercolumnBaseName));
    resO->setPosition(boxposX);
    resO->setPositionScatterStyle(JKQTPSingleColumnSymbolsGraph::NoScatter);
    resO->setDataDirection(JKQTPSingleColumnSymbolsGraph::DataDirection::Y);
    resO->setColor(resB->getKeyLabelColor());


    plotter->addGraph(resB);
    plotter->addGraph(resO);
    return std::pair<JKQTPBoxplotVerticalElement*,JKQTPSingleColumnSymbolsGraph*>(resB, resO);
}








/*! \brief add a JKQTPViolinplotHorizontalElement to the given plotter, where the Violinplot values are calculated from the data range \a first ... \a last , uses a kernel density estimate as density distribution estimate
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param violinposY y-coordinate of the Violinplot
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE, if <0 then \c jkqtpstatEstimateKDEBandwidth(first,last) is called
    \param distBasename name basing for added columns
    \param violinDistSamples number of samples of the distribution (between min and max)
    \return a Violinplot element with its values initialized from the given data range

    Example:
    \code
        jkqtpstatAddHViolinplotKDE(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -20);
    \endcode

    \image html JKQTPGraphViolinplot_ViolinHBoth.png


    \see \ref JKQTPlotterViolinplotGraphs, JKQTPViolinplotHorizontalElement, jkqtpstatKDE1DAutoranged()
*/
template <class InputIt>
inline JKQTPViolinplotHorizontalElement* jkqtpstatAddHViolinplotKDE(JKQTBasePlotter* plotter, InputIt first, InputIt last, double violinposY, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=-1, const QString& distBasename=QString("violin plot distribution"), int violinDistSamples=100) {
    if (bandwidth<=0) bandwidth=jkqtpstatEstimateKDEBandwidth(first,last);
    JKQTPStat5NumberStatistics stat=jkqtpstat5NumberStatistics(first, last);
    size_t cViol1Cat=plotter->getDatastore()->addColumn(distBasename+", category");
    size_t cViol1Freq=plotter->getDatastore()->addColumn(distBasename+", KDE");
    jkqtpstatKDE1DAutoranged(first, last, plotter->getDatastore()->backInserter(cViol1Cat), plotter->getDatastore()->backInserter(cViol1Freq),
                             violinDistSamples, kernel, bandwidth);
    JKQTPViolinplotHorizontalElement* res=new JKQTPViolinplotHorizontalElement(plotter);
    res->setMin(stat.minimum);
    res->setMax(stat.maximum);
    res->setMedian(stat.median);
    res->setMean(jkqtpstatAverage(first, last));
    res->setDrawMean(true);
    res->setDrawMedian(true);
    res->setDrawMinMax(true);
    res->setPos(violinposY);
    res->setViolinPositionColumn(cViol1Cat);
    res->setViolinFrequencyColumn(cViol1Freq);
    plotter->addGraph(res);
    return res;
}




/*! \brief add a JKQTPViolinplotHorizontalElement to the given plotter, where the Violinplot values are calculated from the data range \a first ... \a last , uses a histogram as density distribution estimate
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param violinposY y-coordinate of the Violinplot
    \param distBasename name basing for added columns
    \param violinDistSamples number of bin of the distribution (between min and max)
    \return a Violinplot element with its values initialized from the given data range

    Example:
    \code
        jkqtpstatAddHViolinplotHistogram(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -10);
    \endcode

    \image html jkqtpstatAddHViolinplotHistogram.png


    \see \ref JKQTPlotterViolinplotGraphs, JKQTPViolinplotHorizontalElement, jkqtpstatHistogram1DAutoranged()
*/
template <class InputIt>
inline JKQTPViolinplotHorizontalElement* jkqtpstatAddHViolinplotHistogram(JKQTBasePlotter* plotter, InputIt first, InputIt last, double violinposY, const QString& distBasename=QString("violin plot distribution"), int violinDistSamples=21) {
    JKQTPStat5NumberStatistics stat=jkqtpstat5NumberStatistics(first, last);
    size_t cViol1Cat=plotter->getDatastore()->addColumn(distBasename+", category");
    size_t cViol1Freq=plotter->getDatastore()->addColumn(distBasename+", histogram");
    jkqtpstatHistogram1DAutoranged(first, last, plotter->getDatastore()->backInserter(cViol1Cat), plotter->getDatastore()->backInserter(cViol1Freq),
                             violinDistSamples);
    JKQTPViolinplotHorizontalElement* res=new JKQTPViolinplotHorizontalElement(plotter);
    res->setMin(stat.minimum);
    res->setMax(stat.maximum);
    res->setMedian(stat.median);
    res->setMean(jkqtpstatAverage(first, last));
    res->setDrawMean(true);
    res->setDrawMedian(true);
    res->setDrawMinMax(true);
    res->setPos(violinposY);
    res->setViolinPositionColumn(cViol1Cat);
    res->setViolinFrequencyColumn(cViol1Freq);
    res->setViolinStyle(JKQTPViolinplotHorizontalElement::StepViolin);
    plotter->addGraph(res);
    return res;
}




/*! \brief add a JKQTPViolinplotVerticalElement to the given plotter, where the Violinplot values are calculated from the data range \a first ... \a last , uses a kernel density estimate as density distribution estimate
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param violinposY y-coordinate of the Violinplot
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE, if <0 then \c jkqtpstatEstimateKDEBandwidth(first,last) is called
    \param distBasename name basing for added columns
    \param violinDistSamples number of samples of the distribution (between min and max)
    \return a Violinplot element with its values initialized from the given data range

    Example:
    \code
        jkqtpstatAddVViolinplotKDE(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -20);
    \endcode

    \image html jkqtpstatAddVViolinplotKDE.png


    \see \ref JKQTPlotterViolinplotGraphs, JKQTPViolinplotVerticalElement, jkqtpstatKDE1DAutoranged()
*/
template <class InputIt>
inline JKQTPViolinplotVerticalElement* jkqtpstatAddVViolinplotKDE(JKQTBasePlotter* plotter, InputIt first, InputIt last, double violinposY, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=-1, const QString& distBasename=QString("violin plot distribution"), int violinDistSamples=100) {
    if (bandwidth<=0) bandwidth=jkqtpstatEstimateKDEBandwidth(first,last);
    JKQTPStat5NumberStatistics stat=jkqtpstat5NumberStatistics(first, last);
    size_t cViol1Cat=plotter->getDatastore()->addColumn(distBasename+", category");
    size_t cViol1Freq=plotter->getDatastore()->addColumn(distBasename+", KDE");
    jkqtpstatKDE1DAutoranged(first, last, plotter->getDatastore()->backInserter(cViol1Cat), plotter->getDatastore()->backInserter(cViol1Freq),
                             violinDistSamples, kernel, bandwidth);
    JKQTPViolinplotVerticalElement* res=new JKQTPViolinplotVerticalElement(plotter);
    res->setMin(stat.minimum);
    res->setMax(stat.maximum);
    res->setMedian(stat.median);
    res->setMean(jkqtpstatAverage(first, last));
    res->setDrawMean(true);
    res->setDrawMedian(true);
    res->setDrawMinMax(true);
    res->setPos(violinposY);
    res->setViolinPositionColumn(cViol1Cat);
    res->setViolinFrequencyColumn(cViol1Freq);
    plotter->addGraph(res);
    return res;
}




/*! \brief add a JKQTPViolinplotVerticalElement to the given plotter, where the Violinplot values are calculated from the data range \a first ... \a last , uses a histogram as density distribution estimate
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param violinposY y-coordinate of the Violinplot
    \param distBasename name basing for added columns
    \param violinDistSamples number of bin of the distribution (between min and max)
    \return a Violinplot element with its values initialized from the given data range

    Example:
    \code
        jkqtpstatAddVViolinplotHistogram(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -10);
    \endcode

    \image html jkqtpstatAddVViolinplotHistogram.png


    \see \ref JKQTPlotterViolinplotGraphs, JKQTPViolinplotVerticalElement, jkqtpstatHistogram1DAutoranged()
*/
template <class InputIt>
inline JKQTPViolinplotVerticalElement* jkqtpstatAddVViolinplotHistogram(JKQTBasePlotter* plotter, InputIt first, InputIt last, double violinposY, const QString& distBasename=QString("violin plot distribution"), int violinDistSamples=21) {
    JKQTPStat5NumberStatistics stat=jkqtpstat5NumberStatistics(first, last);
    size_t cViol1Cat=plotter->getDatastore()->addColumn(distBasename+", category");
    size_t cViol1Freq=plotter->getDatastore()->addColumn(distBasename+", histogram");
    jkqtpstatHistogram1DAutoranged(first, last, plotter->getDatastore()->backInserter(cViol1Cat), plotter->getDatastore()->backInserter(cViol1Freq),
                             violinDistSamples);
    JKQTPViolinplotVerticalElement* res=new JKQTPViolinplotVerticalElement(plotter);
    res->setMin(stat.minimum);
    res->setMax(stat.maximum);
    res->setMedian(stat.median);
    res->setMean(jkqtpstatAverage(first, last));
    res->setDrawMean(true);
    res->setDrawMedian(true);
    res->setDrawMinMax(true);
    res->setPos(violinposY);
    res->setViolinPositionColumn(cViol1Cat);
    res->setViolinFrequencyColumn(cViol1Freq);
    res->setViolinStyle(JKQTPViolinplotVerticalElement::StepViolin);
    plotter->addGraph(res);
    return res;
}










/*! \brief add a JKQTPViolinplotHorizontalElement and an outliers graph to the given plotter, where the Violinplot values are calculated from the data range \a first ... \a last , uses a kernel density estimate as density distribution estimate
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param violinposY y-coordinate of the Violinplot
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE, if <0 then \c jkqtpstatEstimateKDEBandwidth(first,last) is called
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \param distBasename name basing for added columns
    \param violinDistSamples number of samples of the distribution (between min and max)
    \return a Violinplot element with its values initialized from the given data range

    Example:
    \code
        jkqtpstatAddHViolinplotKDEAndOutliers(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -15);
    \endcode

    \image html jkqtpstatAddHViolinplotKDEAndOutliers.png


    \see \ref JKQTPlotterViolinplotGraphs, JKQTPViolinplotHorizontalElement, jkqtpstatKDE1DAutoranged()
*/
template <class InputIt>
inline std::pair<JKQTPViolinplotHorizontalElement*,JKQTPSingleColumnSymbolsGraph*> jkqtpstatAddHViolinplotKDEAndOutliers(JKQTBasePlotter* plotter, InputIt first, InputIt last, double violinposY, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=-1, double minimumQuantile=0.03, double maximumQuantile=0.97, const QString& distBasename=QString("violin plot distribution"), int violinDistSamples=100) {
    size_t cOutliersY=plotter->getDatastore()->addColumn(distBasename+", outliers, value");
    std::vector<double> datain, datause;
    std::copy(first, last, std::back_inserter(datain));
    std::sort(datain.begin(), datain.end());
    datause.reserve(datain.size());
    size_t i0=jkqtp_boundedRoundTo<size_t>(0,minimumQuantile*static_cast<double>(datain.size()),datain.size()-1);
    size_t i1=jkqtp_boundedRoundTo<size_t>(0,maximumQuantile*static_cast<double>(datain.size()),datain.size()-1);
    for (size_t i=0; i<datain.size(); i++) {
        if (i<=i0 || i>=i1) {
            plotter->getDatastore()->appendToColumn(cOutliersY, datain[i]);
        } else {
            datause.push_back(datain[i]);
        }
    }



    if (datause.size()>0) {
        if (bandwidth<=0) bandwidth=jkqtpstatEstimateKDEBandwidth(datause.begin(), datause.end());
        size_t cViol1Cat=plotter->getDatastore()->addColumn(distBasename+", category");
        size_t cViol1Freq=plotter->getDatastore()->addColumn(distBasename+", KDE");
        jkqtpstatKDE1DAutoranged(datause.begin(), datause.end(), plotter->getDatastore()->backInserter(cViol1Cat), plotter->getDatastore()->backInserter(cViol1Freq),
                                 violinDistSamples, kernel, bandwidth);
        JKQTPViolinplotHorizontalElement* res=new JKQTPViolinplotHorizontalElement(plotter);
        res->setMin(datause[0]);
        res->setMax(datause[datause.size()-1]);
        res->setMedian(jkqtpstatMedianOfSortedVector(datause));
        res->setMean(jkqtpstatAverage(datause.begin(), datause.end()));
        res->setDrawMean(true);
        res->setDrawMedian(true);
        res->setDrawMinMax(true);
        res->setPos(violinposY);
        res->setViolinPositionColumn(cViol1Cat);
        res->setViolinFrequencyColumn(cViol1Freq);
        plotter->addGraph(res);

        JKQTPSingleColumnSymbolsGraph* resO=new JKQTPSingleColumnSymbolsGraph(plotter);
        resO->setDataColumn(cOutliersY);
        resO->setPosition(violinposY);
        resO->setPositionScatterStyle(JKQTPSingleColumnSymbolsGraph::NoScatter);
        resO->setDataDirection(JKQTPSingleColumnSymbolsGraph::DataDirection::X);
        resO->setColor(res->getKeyLabelColor());
        resO->setTitle("");

        plotter->addGraph(res);
        plotter->addGraph(resO);
        return std::pair<JKQTPViolinplotHorizontalElement*,JKQTPSingleColumnSymbolsGraph*>(res,resO);
    } else {
        return std::pair<JKQTPViolinplotHorizontalElement*,JKQTPSingleColumnSymbolsGraph*>(nullptr,nullptr);
    }

}





/*! \brief add a JKQTPViolinplotHorizontalElement and an outliers graph to the given plotter, where the Violinplot values are calculated from the data range \a first ... \a last , uses a histogram as density distribution estimate
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param violinposY y-coordinate of the Violinplot
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \param distBasename name basing for added columns
    \param violinDistSamples number of samples of the distribution (between min and max)
    \return a Violinplot element with its values initialized from the given data range

    Example:
    \code
        jkqtpstatAddHViolinplotHistogramAndOutliers(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -5);
    \endcode

    \image html jkqtpstatAddHViolinplotHistogramAndOutliers.png


    \see \ref JKQTPlotterViolinplotGraphs, JKQTPViolinplotHorizontalElement, jkqtpstatHistogram1DAutoranged()
*/
template <class InputIt>
inline std::pair<JKQTPViolinplotHorizontalElement*,JKQTPSingleColumnSymbolsGraph*> jkqtpstatAddHViolinplotHistogramAndOutliers(JKQTBasePlotter* plotter, InputIt first, InputIt last, double violinposY, double minimumQuantile=0.03, double maximumQuantile=0.97, const QString& distBasename=QString("violin plot distribution"), int violinDistSamples=21) {
    size_t cOutliersY=plotter->getDatastore()->addColumn(distBasename+", outliers, value");
    std::vector<double> datain, datause;
    std::copy(first, last, std::back_inserter(datain));
    std::sort(datain.begin(), datain.end());
    datause.reserve(datain.size());
    size_t i0=jkqtp_boundedRoundTo<size_t>(0,minimumQuantile*static_cast<double>(datain.size()),datain.size()-1);
    size_t i1=jkqtp_boundedRoundTo<size_t>(0,maximumQuantile*static_cast<double>(datain.size()),datain.size()-1);
    for (size_t i=0; i<datain.size(); i++) {
        if (i<=i0 || i>=i1) {
            plotter->getDatastore()->appendToColumn(cOutliersY, datain[i]);
        } else {
            datause.push_back(datain[i]);
        }
    }



    if (datause.size()>0) {
        size_t cViol1Cat=plotter->getDatastore()->addColumn(distBasename+", category");
        size_t cViol1Freq=plotter->getDatastore()->addColumn(distBasename+", Histogram");
        jkqtpstatHistogram1DAutoranged(datause.begin(), datause.end(), plotter->getDatastore()->backInserter(cViol1Cat), plotter->getDatastore()->backInserter(cViol1Freq), violinDistSamples);
        JKQTPViolinplotHorizontalElement* res=new JKQTPViolinplotHorizontalElement(plotter);
        res->setMin(datause[0]);
        res->setMax(datause[datause.size()-1]);
        res->setMedian(jkqtpstatMedianOfSortedVector(datause));
        res->setMean(jkqtpstatAverage(datause.begin(), datause.end()));
        res->setDrawMean(true);
        res->setDrawMedian(true);
        res->setDrawMinMax(true);
        res->setPos(violinposY);
        res->setViolinPositionColumn(cViol1Cat);
        res->setViolinFrequencyColumn(cViol1Freq);
        res->setViolinStyle(JKQTPViolinplotVerticalElement::StepViolin);
        plotter->addGraph(res);

        JKQTPSingleColumnSymbolsGraph* resO=new JKQTPSingleColumnSymbolsGraph(plotter);
        resO->setDataColumn(cOutliersY);
        resO->setPosition(violinposY);
        resO->setPositionScatterStyle(JKQTPSingleColumnSymbolsGraph::NoScatter);
        resO->setDataDirection(JKQTPSingleColumnSymbolsGraph::DataDirection::X);
        resO->setColor(res->getKeyLabelColor());
        resO->setTitle("");

        plotter->addGraph(res);
        plotter->addGraph(resO);
        return std::pair<JKQTPViolinplotHorizontalElement*,JKQTPSingleColumnSymbolsGraph*>(res,resO);
    } else {
        return std::pair<JKQTPViolinplotHorizontalElement*,JKQTPSingleColumnSymbolsGraph*>(nullptr,nullptr);
    }

}









/*! \brief add a JKQTPViolinplotVerticalElement and an outliers graph to the given plotter, where the Violinplot values are calculated from the data range \a first ... \a last , uses a kernel density estimate as density distribution estimate
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param violinposY y-coordinate of the Violinplot
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE, if <0 then \c jkqtpstatEstimateKDEBandwidth(first,last) is called
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \param distBasename name basing for added columns
    \param violinDistSamples number of samples of the distribution (between min and max)
    \return a Violinplot element with its values initialized from the given data range

    Example:
    \code
        jkqtpstatAddVViolinplotKDEAndOutliers(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -15);
    \endcode

    \image html jkqtpstatAddVViolinplotKDEAndOutliers.png


    \see \ref JKQTPlotterViolinplotGraphs, JKQTPViolinplotVerticalElement, jkqtpstatKDE1DAutoranged()
*/
template <class InputIt>
inline std::pair<JKQTPViolinplotVerticalElement*,JKQTPSingleColumnSymbolsGraph*> jkqtpstatAddVViolinplotKDEAndOutliers(JKQTBasePlotter* plotter, InputIt first, InputIt last, double violinposY, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=-1, double minimumQuantile=0.03, double maximumQuantile=0.97, const QString& distBasename=QString("violin plot distribution"), int violinDistSamples=100) {
    size_t cOutliersY=plotter->getDatastore()->addColumn(distBasename+", outliers, value");
    std::vector<double> datain, datause;
    std::copy(first, last, std::back_inserter(datain));
    std::sort(datain.begin(), datain.end());
    datause.reserve(datain.size());
    size_t i0=jkqtp_boundedRoundTo<size_t>(0,minimumQuantile*static_cast<double>(datain.size()),datain.size()-1);
    size_t i1=jkqtp_boundedRoundTo<size_t>(0,maximumQuantile*static_cast<double>(datain.size()),datain.size()-1);
    for (size_t i=0; i<datain.size(); i++) {
        if (i<=i0 || i>=i1) {
            plotter->getDatastore()->appendToColumn(cOutliersY, datain[i]);
        } else {
            datause.push_back(datain[i]);
        }
    }



    if (datause.size()>0) {
        if (bandwidth<=0) bandwidth=jkqtpstatEstimateKDEBandwidth(datause.begin(), datause.end());
        size_t cViol1Cat=plotter->getDatastore()->addColumn(distBasename+", category");
        size_t cViol1Freq=plotter->getDatastore()->addColumn(distBasename+", KDE");
        jkqtpstatKDE1DAutoranged(datause.begin(), datause.end(), plotter->getDatastore()->backInserter(cViol1Cat), plotter->getDatastore()->backInserter(cViol1Freq),
                                 violinDistSamples, kernel, bandwidth);
        JKQTPViolinplotVerticalElement* res=new JKQTPViolinplotVerticalElement(plotter);
        res->setMin(datause[0]);
        res->setMax(datause[datause.size()-1]);
        res->setMedian(jkqtpstatMedianOfSortedVector(datause));
        res->setMean(jkqtpstatAverage(datause.begin(), datause.end()));
        res->setDrawMean(true);
        res->setDrawMedian(true);
        res->setDrawMinMax(true);
        res->setPos(violinposY);
        res->setViolinPositionColumn(cViol1Cat);
        res->setViolinFrequencyColumn(cViol1Freq);
        plotter->addGraph(res);

        JKQTPSingleColumnSymbolsGraph* resO=new JKQTPSingleColumnSymbolsGraph(plotter);
        resO->setDataColumn(cOutliersY);
        resO->setPosition(violinposY);
        resO->setPositionScatterStyle(JKQTPSingleColumnSymbolsGraph::NoScatter);
        resO->setDataDirection(JKQTPSingleColumnSymbolsGraph::DataDirection::Y);
        resO->setColor(res->getKeyLabelColor());
        resO->setTitle("");

        plotter->addGraph(res);
        plotter->addGraph(resO);
        return std::pair<JKQTPViolinplotVerticalElement*,JKQTPSingleColumnSymbolsGraph*>(res,resO);
    } else {
        return std::pair<JKQTPViolinplotVerticalElement*,JKQTPSingleColumnSymbolsGraph*>(nullptr,nullptr);
    }

}





/*! \brief add a JKQTPViolinplotVerticalElement and an outliers graph to the given plotter, where the Violinplot values are calculated from the data range \a first ... \a last , uses a histogram as density distribution estimate
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param violinposY y-coordinate of the Violinplot
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \param distBasename name basing for added columns
    \param violinDistSamples number of samples of the distribution (between min and max)
    \return a Violinplot element with its values initialized from the given data range

    Example:
    \code
        jkqtpstatAddVViolinplotHistogramAndOutliers(plot->getPlotter(), datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), -5);
    \endcode

    \image html jkqtpstatAddVViolinplotHistogramAndOutliers.png


    \see \ref JKQTPlotterViolinplotGraphs, JKQTPViolinplotVerticalElement, jkqtpstatHistogram1DAutoranged()
*/
template <class InputIt>
inline std::pair<JKQTPViolinplotVerticalElement*,JKQTPSingleColumnSymbolsGraph*> jkqtpstatAddVViolinplotHistogramAndOutliers(JKQTBasePlotter* plotter, InputIt first, InputIt last, double violinposY, double minimumQuantile=0.03, double maximumQuantile=0.97, const QString& distBasename=QString("violin plot distribution"), int violinDistSamples=21) {
    size_t cOutliersY=plotter->getDatastore()->addColumn(distBasename+", outliers, value");
    std::vector<double> datain, datause;
    std::copy(first, last, std::back_inserter(datain));
    std::sort(datain.begin(), datain.end());
    datause.reserve(datain.size());
    size_t i0=jkqtp_boundedRoundTo<size_t>(0,minimumQuantile*static_cast<double>(datain.size()),datain.size()-1);
    size_t i1=jkqtp_boundedRoundTo<size_t>(0,maximumQuantile*static_cast<double>(datain.size()),datain.size()-1);
    for (size_t i=0; i<datain.size(); i++) {
        if (i<=i0 || i>=i1) {
            plotter->getDatastore()->appendToColumn(cOutliersY, datain[i]);
        } else {
            datause.push_back(datain[i]);
        }
    }



    if (datause.size()>0) {
        size_t cViol1Cat=plotter->getDatastore()->addColumn(distBasename+", category");
        size_t cViol1Freq=plotter->getDatastore()->addColumn(distBasename+", Histogram");
        jkqtpstatHistogram1DAutoranged(datause.begin(), datause.end(), plotter->getDatastore()->backInserter(cViol1Cat), plotter->getDatastore()->backInserter(cViol1Freq), violinDistSamples);
        JKQTPViolinplotVerticalElement* res=new JKQTPViolinplotVerticalElement(plotter);
        res->setMin(datause[0]);
        res->setMax(datause[datause.size()-1]);
        res->setMedian(jkqtpstatMedianOfSortedVector(datause));
        res->setMean(jkqtpstatAverage(datause.begin(), datause.end()));
        res->setDrawMean(true);
        res->setDrawMedian(true);
        res->setDrawMinMax(true);
        res->setPos(violinposY);
        res->setViolinPositionColumn(cViol1Cat);
        res->setViolinFrequencyColumn(cViol1Freq);
        res->setViolinStyle(JKQTPViolinplotVerticalElement::StepViolin);
        plotter->addGraph(res);

        JKQTPSingleColumnSymbolsGraph* resO=new JKQTPSingleColumnSymbolsGraph(plotter);
        resO->setDataColumn(cOutliersY);
        resO->setPosition(violinposY);
        resO->setPositionScatterStyle(JKQTPSingleColumnSymbolsGraph::NoScatter);
        resO->setDataDirection(JKQTPSingleColumnSymbolsGraph::DataDirection::Y);
        resO->setColor(res->getKeyLabelColor());
        resO->setTitle("");

        plotter->addGraph(res);
        plotter->addGraph(resO);
        return std::pair<JKQTPViolinplotVerticalElement*,JKQTPSingleColumnSymbolsGraph*>(res,resO);
    } else {
        return std::pair<JKQTPViolinplotVerticalElement*,JKQTPSingleColumnSymbolsGraph*>(nullptr,nullptr);
    }

}








/*! \brief calculate an autoranged histogram and add a JKQTPBarVerticalGraph to the given plotter, where the histogram is calculated from the data range \a first ... \a last, bins defined by their number
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param bins number of bins in the resulting histogram
    \param histogramcolumnBaseName this string is used in building the column names for the outlier columns
    \param normalized indicates whether the histogram has to be normalized
    \param cummulative if \c true, a cummulative histogram is calculated
    \return a graph class pointer (of type \a GraphClass ) displaying the histogram data

    Example:
    \code
        jkqtpstatAddHHistogram1DAutoranged(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), 11);
    \endcode

    \image html datastore_statistics_hist.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstatHistogram1DAutoranged(), JKQTPBarVerticalGraph
*/
template <class InputIt>
inline JKQTPBarVerticalGraph* jkqtpstatAddHHistogram1DAutoranged(JKQTBasePlotter* plotter, InputIt first, InputIt last, int bins=11, bool normalized=true, bool cummulative=false, const QString& histogramcolumnBaseName=QString("histogram")) {
    size_t histcolX=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", values");
    jkqtpstatHistogram1DAutoranged(first, last, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), bins, normalized, cummulative, JKQTPStatHistogramBinXMode::XIsMid);
    JKQTPBarVerticalGraph* resO=new JKQTPBarVerticalGraph(plotter);
    resO->setXColumn(histcolX);
    resO->setYColumn(histcolY);
    resO->setTitle(histogramcolumnBaseName);
    plotter->addGraph(resO);
    return resO;
}



/*! \brief calculate an autoranged histogram and add a JKQTPBarVerticalGraph to the given plotter, where the histogram is calculated from the data range \a first ... \a last, bins defined by their width
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binWidth width of the bins
    \param histogramcolumnBaseName this string is used in building the column names for the histogram columns
    \param normalized indicates whether the histogram has to be normalized
    \param cummulative if \c true, a cummulative histogram is calculated
    \return a graph class pointer (of type \a GraphClass ) displaying the histogram data

    Example:
    \code
        jkqtpstatAddHHistogram1DAutoranged(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), 0.5);
    \endcode

    \image html datastore_statistics_hist.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstatHistogram1DAutoranged(), JKQTPBarVerticalGraph
*/
template <class InputIt>
inline JKQTPBarVerticalGraph* jkqtpstatAddHHistogram1DAutoranged(JKQTBasePlotter* plotter, InputIt first, InputIt last, double binWidth, bool normalized=true, bool cummulative=false, const QString& histogramcolumnBaseName=QString("histogram")) {
    size_t histcolX=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", values");
    jkqtpstatHistogram1DAutoranged(first, last, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), binWidth, normalized, cummulative, JKQTPStatHistogramBinXMode::XIsMid);
    JKQTPBarVerticalGraph* resO=new JKQTPBarVerticalGraph(plotter);
    resO->setXColumn(histcolX);
    resO->setYColumn(histcolY);
    resO->setTitle(histogramcolumnBaseName);
    plotter->addGraph(resO);
    return resO;
}


/*! \brief calculate an autoranged histogram and add a JKQTPBarVerticalGraph to the given plotter, where the histogram is calculated from the data range \a first ... \a last, bins defined by their width
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam BinsInputIt standard iterator type of \a binsFirst and \a binsLast.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binsFirst iterator pointing to the first item in the set of histogram bins
    \param binsLast iterator pointing behind the last item in the set of histogram bins
    \param histogramcolumnBaseName this string is used in building the column names for the histogram columns
    \param normalized indicates whether the histogram has to be normalized
    \param cummulative if \c true, a cummulative histogram is calculated
    \return a graph class pointer (of type \a GraphClass ) displaying the histogram data

    Example:
    \code
        std::vector<double> bins{-2,-1.5,-1,-0.75,-0.5,-0.25,0,0.25,0.5,0.75,1,1.5,2,2.5,3,4,5,6,7,8,9,10};
        jkqtpstatAddHHistogram1D(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), bins.begin(), bins.end());
    \endcode

    \image html datastore_statistics_hist.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstatHistogram1D(), JKQTPBarVerticalGraph
*/
template <class InputIt, class BinsInputIt>
inline JKQTPBarVerticalGraph* jkqtpstatAddHHistogram1D(JKQTBasePlotter* plotter, InputIt first, InputIt last, BinsInputIt binsFirst, BinsInputIt binsLast, bool normalized=true, bool cummulative=false, const QString& histogramcolumnBaseName=QString("histogram")) {
    size_t histcolX=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", values");
    jkqtpstatHistogram1D(first, last, binsFirst, binsLast, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), normalized, cummulative, JKQTPStatHistogramBinXMode::XIsMid);
    JKQTPBarVerticalGraph* resO=new JKQTPBarVerticalGraph(plotter);
    resO->setXColumn(histcolX);
    resO->setYColumn(histcolY);
    resO->setTitle(histogramcolumnBaseName);
    plotter->addGraph(resO);
    return resO;
}


/*! \brief calculate an autoranged histogram and add a JKQTPBarHorizontalGraph to the given plotter, where the histogram is calculated from the data range \a first ... \a last, bins defined by their number
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param bins number of bins in the resulting histogram
    \param histogramcolumnBaseName this string is used in building the column names for the histogram columns
    \param normalized indicates whether the histogram has to be normalized
    \param cummulative if \c true, a cummulative histogram is calculated
    \return a graph class pointer (of type \a GraphClass ) displaying the histogram data

    Example:
    \code
        jkqtpstatAddVHistogram1DAutoranged(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), 11);
    \endcode

    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstatHistogram1DAutoranged(), JKQTPBarHorizontalGraph
*/
template <class InputIt>
inline JKQTPBarHorizontalGraph* jkqtpstatAddVHistogram1DAutoranged(JKQTBasePlotter* plotter, InputIt first, InputIt last, int bins=11, bool normalized=true, bool cummulative=false, const QString& histogramcolumnBaseName=QString("histogram")) {
    size_t histcolX=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", values");
    jkqtpstatHistogram1DAutoranged(first, last, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), bins, normalized, cummulative, JKQTPStatHistogramBinXMode::XIsMid);
    JKQTPBarHorizontalGraph* resO=new JKQTPBarHorizontalGraph(plotter);
    resO->setXColumn(histcolY);
    resO->setYColumn(histcolX);
    resO->setTitle(histogramcolumnBaseName);
    plotter->addGraph(resO);
    return resO;
}



/*! \brief calculate an autoranged histogram and add a JKQTPBarHorizontalGraph to the given plotter, where the histogram is calculated from the data range \a first ... \a last, bins defined by their width
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binWidth width of the bins
    \param histogramcolumnBaseName this string is used in building the column names for the histogram columns
    \param normalized indicates whether the histogram has to be normalized
    \param cummulative if \c true, a cummulative histogram is calculated
    \return a graph class pointer (of type \a GraphClass ) displaying the histogram data

    Example:
    \code
        jkqtpstatAddVHistogram1DAutoranged(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), 0.5);
    \endcode

    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstatHistogram1DAutoranged(), JKQTPBarHorizontalGraph
*/
template <class InputIt>
inline JKQTPBarHorizontalGraph* jkqtpstatAddVHistogram1DAutoranged(JKQTBasePlotter* plotter, InputIt first, InputIt last, double binWidth, bool normalized=true, bool cummulative=false, const QString& histogramcolumnBaseName=QString("histogram")) {
    size_t histcolX=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", values");
    jkqtpstatHistogram1DAutoranged(first, last, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), binWidth, normalized, cummulative, JKQTPStatHistogramBinXMode::XIsMid);
    JKQTPBarHorizontalGraph* resO=new JKQTPBarHorizontalGraph(plotter);
    resO->setXColumn(histcolY);
    resO->setYColumn(histcolX);
    resO->setTitle(histogramcolumnBaseName);
    plotter->addGraph(resO);
    return resO;
}


/*! \brief calculate an autoranged histogram and add a JKQTPBarHorizontalGraph to the given plotter, where the histogram is calculated from the data range \a first ... \a last, bins defined by their width
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam BinsInputIt standard iterator type of \a binsFirst and \a binsLast.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binsFirst iterator pointing to the first item in the set of histogram bins
    \param binsLast iterator pointing behind the last item in the set of histogram bins
    \param histogramcolumnBaseName this string is used in building the column names for the histogram columns
    \param normalized indicates whether the histogram has to be normalized
    \param cummulative if \c true, a cummulative histogram is calculated
    \return a graph class pointer (of type \a GraphClass ) displaying the histogram data

    Example:
    \code
        std::vector<double> bins{-2,-1.5,-1,-0.75,-0.5,-0.25,0,0.25,0.5,0.75,1,1.5,2,2.5,3,4,5,6,7,8,9,10};
        jkqtpstatAddVHistogram1D(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), bins.begin(), bins.end());
    \endcode

    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstatHistogram1D(), JKQTPBarHorizontalGraph
*/
template <class InputIt, class BinsInputIt>
inline JKQTPBarHorizontalGraph* jkqtpstatAddVHistogram1D(JKQTBasePlotter* plotter, InputIt first, InputIt last, BinsInputIt binsFirst, BinsInputIt binsLast, bool normalized=true, bool cummulative=false, const QString& histogramcolumnBaseName=QString("histogram")) {
    size_t histcolX=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", values");
    jkqtpstatHistogram1D(first, last, binsFirst, binsLast, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), normalized, cummulative, JKQTPStatHistogramBinXMode::XIsMid);
    JKQTPBarHorizontalGraph* resO=new JKQTPBarHorizontalGraph(plotter);
    resO->setXColumn(histcolY);
    resO->setYColumn(histcolX);
    resO->setTitle(histogramcolumnBaseName);
    plotter->addGraph(resO);
    return resO;
}








/*! \brief calculate calculate a 2-dimensional histogram and add a JKQTPColumnMathImage to the given plotter, where the histogram is calculated from the given data range \a firstX / \a firstY ... \a lastY / \a lastY
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param plotter the plotter to which to add the resulting graph
    \param firstX iterator pointing to the first x-position item in the dataset to use \f$ X_1 \f$
    \param lastX iterator pointing behind the last x-position item in the dataset to use \f$ X_N \f$
    \param firstY iterator pointing to the first y-position item in the dataset to use \f$ Y_1 \f$
    \param lastY iterator pointing behind the last y-position item in the dataset to use \f$ Y_N \f$
    \param xbins number of bins in x-direction
    \param ybins number of bins in y-direction
    \param normalized indicates whether the histogram has to be normalized
    \param histogramcolumnBaseName this string is used in building the column names for the histogram data columns
    \param[out] oxmin position of the first histogram bin in x-direction
    \param[out] oxmax position of the last histogram bin in x-direction
    \param[out] oymin position of the first histogram bin in y-direction
    \param[out] oymax position of the last histogram bin in y-direction
    \return a graph class pointer (of type \c JKQTPColumnMathImage ) displaying the histogram data

    \image html datastore_statistics_2d_hist.png

    \see jkqtpstatHistogram2D(), \ref JKQTPlotterBasicJKQTPDatastoreStatistics2D
*/
template <class InputItX, class InputItY>
inline JKQTPColumnMathImage* jkqtpstatAddHistogram2DImage(JKQTBasePlotter* plotter, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, size_t xbins=10, size_t ybins=10, bool normalized=true, const QString& histogramcolumnBaseName=QString("histogram"), double* oxmin=nullptr, double* oxmax=nullptr, double* oymin=nullptr, double* oymax=nullptr) {
    double xmin=0, xmax=0;
    double ymin=0, ymax=0;
    jkqtpstatMinMax(firstX,lastX, xmin,xmax);
    jkqtpstatMinMax(firstY,lastY, ymin,ymax);
    size_t histcol=plotter->getDatastore()->addImageColumn(xbins, ybins, histogramcolumnBaseName);
    jkqtpstatHistogram2D(firstX,lastX,firstY,lastY,
                         plotter->getDatastore()->begin(histcol),
                         xmin, xmax, ymin, ymax,
                         xbins, ybins, normalized);
    JKQTPColumnMathImage* gHist;
    plotter->addGraph(gHist=new JKQTPColumnMathImage(plotter));
    gHist->setImageColumn(static_cast<int>(histcol));
    gHist->setX(xmin);
    gHist->setY(ymin);
    gHist->setWidth(xmax-xmin);
    gHist->setHeight(ymax-ymin);
    gHist->setTitle(QObject::tr("2D Histogram"));
    if (oxmax) *oxmax=xmax;
    if (oxmin) *oxmin=xmin;
    if (oymax) *oymax=ymax;
    if (oymin) *oymin=ymin;
    return gHist;
}






/*! \brief calculate calculate a 2-dimensional histogram and add a JKQTPColumnContourPlot to the given plotter, where the histogram is calculated from the given data range \a firstX / \a firstY ... \a lastY / \a lastY
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param plotter the plotter to which to add the resulting graph
    \param firstX iterator pointing to the first x-position item in the dataset to use \f$ X_1 \f$
    \param lastX iterator pointing behind the last x-position item in the dataset to use \f$ X_N \f$
    \param firstY iterator pointing to the first y-position item in the dataset to use \f$ Y_1 \f$
    \param lastY iterator pointing behind the last y-position item in the dataset to use \f$ Y_N \f$
    \param xbins number of bins in x-direction
    \param ybins number of bins in y-direction
    \param normalized indicates whether the histogram has to be normalized
    \param histogramcolumnBaseName this string is used in building the column names for the histogram data columns
    \param[out] oxmin position of the first histogram bin in x-direction
    \param[out] oxmax position of the last histogram bin in x-direction
    \param[out] oymin position of the first histogram bin in y-direction
    \param[out] oymax position of the last histogram bin in y-direction
    \return a graph class pointer (of type \c JKQTPColumnContourPlot ) displaying the histogram data as a contour plot

    \image html datastore_statistics_2d_histcontour.png

    \see jkqtpstatHistogram2D(), \ref JKQTPlotterBasicJKQTPDatastoreStatistics2D
*/
template <class InputItX, class InputItY>
inline JKQTPColumnContourPlot* jkqtpstatAddHistogram2DContour(JKQTBasePlotter* plotter, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, size_t xbins=10, size_t ybins=10, bool normalized=true, const QString& histogramcolumnBaseName=QString("histogram"), double* oxmin=nullptr, double* oxmax=nullptr, double* oymin=nullptr, double* oymax=nullptr) {
    double xmin=0, xmax=0;
    double ymin=0, ymax=0;
    jkqtpstatMinMax(firstX,lastX, xmin,xmax);
    jkqtpstatMinMax(firstY,lastY, ymin,ymax);
    size_t histcol=plotter->getDatastore()->addImageColumn(xbins, ybins, histogramcolumnBaseName);
    jkqtpstatHistogram2D(firstX,lastX,firstY,lastY,
                         plotter->getDatastore()->begin(histcol),
                         xmin, xmax, ymin, ymax,
                         xbins, ybins, true);
    JKQTPColumnContourPlot* gHist;
    plotter->addGraph(gHist=new JKQTPColumnContourPlot(plotter));
    gHist->setImageColumn(static_cast<int>(histcol));
    gHist->setX(xmin);
    gHist->setY(ymin);
    gHist->setWidth(xmax-xmin);
    gHist->setHeight(ymax-ymin);
    gHist->setTitle(QObject::tr("2D Histogram"));
    gHist->createContourLevels(5);
    if (oxmax) *oxmax=xmax;
    if (oxmin) *oxmin=xmin;
    if (oymax) *oymax=ymax;
    if (oymin) *oymin=ymin;
    return gHist;
}
/*! \brief calculate calculate a 2-dimensional histogram and add a JKQTPColumnMathImage to the given plotter, where the histogram is calculated from the given data range \a firstX / \a firstY ... \a lastY / \a lastY
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param plotter the plotter to which to add the resulting graph
    \param firstX iterator pointing to the first x-position item in the dataset to use \f$ X_1 \f$
    \param lastX iterator pointing behind the last x-position item in the dataset to use \f$ X_N \f$
    \param firstY iterator pointing to the first y-position item in the dataset to use \f$ Y_1 \f$
    \param lastY iterator pointing behind the last y-position item in the dataset to use \f$ Y_N \f$
    \param xbinwidth width of bins in x-direction
    \param ybinwidth width of bins in y-direction
    \param normalized indicates whether the histogram has to be normalized
    \param histogramcolumnBaseName this string is used in building the column names for the histogram data columns
    \param[out] oxmin position of the first histogram bin in x-direction
    \param[out] oxmax position of the last histogram bin in x-direction
    \param[out] oymin position of the first histogram bin in y-direction
    \param[out] oymax position of the last histogram bin in y-direction
    \return a graph class pointer (of type \c JKQTPColumnMathImage ) displaying the histogram data

    \image html datastore_statistics_2d_hist.png

    \see jkqtpstatHistogram2D(), \ref JKQTPlotterBasicJKQTPDatastoreStatistics2D
*/
template <class InputItX, class InputItY>
inline JKQTPColumnMathImage* jkqtpstatAddHistogram2DImage(JKQTBasePlotter* plotter, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, double xbinwidth, double ybinwidth, bool normalized=true, const QString& histogramcolumnBaseName=QString("histogram"), double* oxmin=nullptr, double* oxmax=nullptr, double* oymin=nullptr, double* oymax=nullptr) {
    double xmin=0, xmax=0;
    double ymin=0, ymax=0;
    jkqtpstatMinMax(firstX,lastX, xmin,xmax);
    jkqtpstatMinMax(firstY,lastY, ymin,ymax);
    size_t Nx=jkqtp_ceilTo<size_t>((xmax-xmin)/xbinwidth);
    size_t Ny=jkqtp_ceilTo<size_t>((ymax-ymin)/ybinwidth);
    size_t histcol=plotter->getDatastore()->addImageColumn(Nx, Ny, histogramcolumnBaseName);
    jkqtpstatHistogram2D(firstX,lastX,firstY,lastY,
                         plotter->getDatastore()->begin(histcol),
                         xmin, xmax, ymin, ymax,
                         Nx, Ny, normalized);
    JKQTPColumnMathImage* gHist;
    plotter->addGraph(gHist=new JKQTPColumnMathImage(plotter));
    gHist->setImageColumn(static_cast<int>(histcol));
    gHist->setX(xmin);
    gHist->setY(ymin);
    gHist->setWidth(xmax-xmin);
    gHist->setHeight(ymax-ymin);
    gHist->setTitle(QObject::tr("2D Histogram"));
    if (oxmax) *oxmax=xmax;
    if (oxmin) *oxmin=xmin;
    if (oymax) *oymax=ymax;
    if (oymin) *oymin=ymin;
    return gHist;
}






/*! \brief calculate calculate a 2-dimensional histogram and add a JKQTPColumnContourPlot to the given plotter, where the histogram is calculated from the given data range \a firstX / \a firstY ... \a lastY / \a lastY
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param plotter the plotter to which to add the resulting graph
    \param firstX iterator pointing to the first x-position item in the dataset to use \f$ X_1 \f$
    \param lastX iterator pointing behind the last x-position item in the dataset to use \f$ X_N \f$
    \param firstY iterator pointing to the first y-position item in the dataset to use \f$ Y_1 \f$
    \param lastY iterator pointing behind the last y-position item in the dataset to use \f$ Y_N \f$
    \param xbinwidth width of bins in x-direction
    \param ybinwidth width of bins in y-direction
    \param normalized indicates whether the histogram has to be normalized
    \param histogramcolumnBaseName this string is used in building the column names for the histogram data columns
    \param[out] oxmin position of the first histogram bin in x-direction
    \param[out] oxmax position of the last histogram bin in x-direction
    \param[out] oymin position of the first histogram bin in y-direction
    \param[out] oymax position of the last histogram bin in y-direction
    \return a graph class pointer (of type \c JKQTPColumnContourPlot ) displaying the histogram data as a contour plot

    \image html datastore_statistics_2d_histcontour.png

    \see jkqtpstatHistogram2D(), \ref JKQTPlotterBasicJKQTPDatastoreStatistics2D

*/
template <class InputItX, class InputItY>
inline JKQTPColumnContourPlot* jkqtpstatAddHistogram2DContour(JKQTBasePlotter* plotter, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, double xbinwidth, double ybinwidth, bool normalized=true, const QString& histogramcolumnBaseName=QString("histogram"), double* oxmin=nullptr, double* oxmax=nullptr, double* oymin=nullptr, double* oymax=nullptr) {
    double xmin=0, xmax=0;
    double ymin=0, ymax=0;
    jkqtpstatMinMax(firstX,lastX, xmin,xmax);
    jkqtpstatMinMax(firstY,lastY, ymin,ymax);
    size_t Nx=jkqtp_ceilTo<size_t>((xmax-xmin)/xbinwidth);
    size_t Ny=jkqtp_ceilTo<size_t>((ymax-ymin)/ybinwidth);
    size_t histcol=plotter->getDatastore()->addImageColumn(Nx, Ny, histogramcolumnBaseName);
    jkqtpstatHistogram2D(firstX,lastX,firstY,lastY,
                         plotter->getDatastore()->begin(histcol),
                         xmin, xmax, ymin, ymax,
                         Nx, Ny, normalized);
    JKQTPColumnContourPlot* gHist;
    plotter->addGraph(gHist=new JKQTPColumnContourPlot(plotter));
    gHist->setImageColumn(static_cast<int>(histcol));
    gHist->setX(xmin);
    gHist->setY(ymin);
    gHist->setWidth(xmax-xmin);
    gHist->setHeight(ymax-ymin);
    gHist->setTitle(QObject::tr("2D Histogram"));
    gHist->createContourLevels(5);
    if (oxmax) *oxmax=xmax;
    if (oxmin) *oxmin=xmin;
    if (oymax) *oymax=ymax;
    if (oymin) *oymin=ymin;
    return gHist;
}




/*! \brief calculate an autoranged KDE and add a JKQTPXYLineGraph to the given plotter, where the KDE is calculated from the data range \a first ... \a last, bins defined by their number
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param Nout number of points in the resulting KDE
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param cummulative if \c true, a cummulative KDE is calculated
    \param KDEcolumnBaseName this string is used in building the column names for the KDE data columns
    \return a graph class pointer (of type \a GraphClass ) displaying the KDE data

    Example:
    \code
        jkqtpstatAddHKDE1DAutoranged(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), 200);
    \endcode

    \image html datastore_statistics_kde.png


    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstatKDE1DAutoranged(), JKQTPXYLineGraph
*/
template <class InputIt>
inline JKQTPXYLineGraph* jkqtpstatAddHKDE1DAutoranged(JKQTBasePlotter* plotter, InputIt first, InputIt last, int Nout=100, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false, const QString& KDEcolumnBaseName=QString("KDE")) {
    size_t histcolX=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", positions");
    size_t histcolY=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", values");
    jkqtpstatKDE1DAutoranged(first, last, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), Nout, kernel, bandwidth, cummulative);
    JKQTPXYLineGraph* resO=new JKQTPXYLineGraph(plotter);
    resO->setXColumn(histcolX);
    resO->setYColumn(histcolY);
    resO->setTitle(KDEcolumnBaseName);
    resO->setDrawLine(true);
    resO->setSymbolType(JKQTPNoSymbol);
    plotter->addGraph(resO);
    return resO;
}



/*! \brief calculate an autoranged KDE and add a JKQTPXYLineGraph to the given plotter, where the KDE is calculated from the data range \a first ... \a last, bins defined by their width
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binWidth width of the bins
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param cummulative if \c true, a cummulative KDE is calculated
    \param KDEcolumnBaseName this string is used in building the column names for the KDE data columns
    \return a graph class pointer (of type \a GraphClass ) displaying the KDE data

    Example:
    \code
        jkqtpstatAddHKDE1DAutoranged(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), 0.01);
    \endcode

    \image html datastore_statistics_kde.png


    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstatKDE1DAutoranged(), JKQTPXYLineGraph
*/
template <class InputIt>
inline JKQTPXYLineGraph* jkqtpstatAddHKDE1DAutoranged(JKQTBasePlotter* plotter, InputIt first, InputIt last, double binWidth, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false, const QString& KDEcolumnBaseName=QString("KDE")) {
    size_t histcolX=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", values");
    jkqtpstatKDE1DAutoranged(first, last, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), binWidth, kernel, bandwidth, cummulative);
    JKQTPXYLineGraph* resO=new JKQTPXYLineGraph(plotter);
    resO->setXColumn(histcolX);
    resO->setYColumn(histcolY);
    resO->setTitle(KDEcolumnBaseName);
    resO->setDrawLine(true);
    resO->setSymbolType(JKQTPNoSymbol);
    plotter->addGraph(resO);
    return resO;
}


/*! \brief calculate an autoranged KDE and add a JKQTPXYLineGraph to the given plotter, where the KDE is calculated from the data range \a first ... \a last, bins defined by their width
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam BinsInputIt standard iterator type of \a binsFirst and \a binsLast.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binsFirst iterator pointing to the first item in the set of KDE bins
    \param binsLast iterator pointing behind the last item in the set of KDE bins
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param cummulative if \c true, a cummulative KDE is calculated
    \param KDEcolumnBaseName this string is used in building the column names for the KDE data columns
    \return a graph class pointer (of type \a GraphClass ) displaying the KDE data

    Example:
    \code
        std::vector<double> bins{-2,-1.5,-1,-0.75,-0.5,-0.25,0,0.25,0.5,0.75,1,1.5,2,2.5,3,4,5,6,7,8,9,10};
        jkqtpstatAddHKDE1D(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), bins.begin(), bins.end());
    \endcode

    \image html datastore_statistics_kde.png


    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstatKDE1D(), JKQTPXYLineGraph
*/
template <class InputIt, class BinsInputIt>
inline JKQTPXYLineGraph* jkqtpstatAddHKDE1D(JKQTBasePlotter* plotter, InputIt first, InputIt last, BinsInputIt binsFirst, BinsInputIt binsLast, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false, const QString& KDEcolumnBaseName=QString("KDE")) {
    size_t histcolX=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", values");
    jkqtpstatKDE1D(first, last, binsFirst, binsLast, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), kernel, bandwidth, cummulative);
    JKQTPXYLineGraph* resO=new JKQTPXYLineGraph(plotter);
    resO->setXColumn(histcolX);
    resO->setYColumn(histcolY);
    resO->setTitle(KDEcolumnBaseName);
    resO->setDrawLine(true);
    resO->setSymbolType(JKQTPNoSymbol);
    plotter->addGraph(resO);
    return resO;
}

/*! \brief calculate an autoranged KDE and add a JKQTPXYLineGraph to the given plotter, where the KDE is calculated from the data range \a first ... \a last, evaluation positions are given by the range \a binXLeft ... \a binXRight (in steps of \a binxDelta )
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binXLeft first x-position, where to evaluate the KDE
    \param binXDelta distance between two x-positions at which the KDE is evaluated
    \param binXRight last x-position, where to evaluate the KDE
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param cummulative if \c true, a cummulative KDE is calculated
    \param KDEcolumnBaseName this string is used in building the column names for the KDE data columns
    \return a graph class pointer (of type \a GraphClass ) displaying the KDE data

    Example:
    \code
        std::vector<double> bins{-2,-1.5,-1,-0.75,-0.5,-0.25,0,0.25,0.5,0.75,1,1.5,2,2.5,3,4,5,6,7,8,9,10};
        jkqtpstatAddHKDE1D(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), bins.begin(), bins.end());
    \endcode

    \image html datastore_statistics_kde.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstatKDE1D(), JKQTPXYLineGraph
*/
template <class InputIt>
inline JKQTPXYLineGraph* jkqtpstatAddHKDE1D(JKQTBasePlotter* plotter, InputIt first, InputIt last, double binXLeft, double binXDelta, double binXRight, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false, const QString& KDEcolumnBaseName=QString("KDE")) {
    size_t histcolX=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", values");
    jkqtpstatKDE1D(first, last, binXLeft, binXDelta, binXRight, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), kernel, bandwidth, cummulative);
    JKQTPXYLineGraph* resO=new JKQTPXYLineGraph(plotter);
    resO->setXColumn(histcolX);
    resO->setYColumn(histcolY);
    resO->setTitle(KDEcolumnBaseName);
    resO->setDrawLine(true);
    resO->setSymbolType(JKQTPNoSymbol);
    plotter->addGraph(resO);
    return resO;
}











/*! \brief calculate an autoranged vertical KDE and add a JKQTPXYLineGraph to the given plotter, where the KDE is calculated from the data range \a first ... \a last, bins defined by their number
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param Nout number of points in the resulting KDE
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param cummulative if \c true, a cummulative KDE is calculated
    \param KDEcolumnBaseName this string is used in building the column names for the KDE data columns
    \return a graph class pointer (of type \a GraphClass ) displaying the KDE data

    Example:
    \code
        jkqtpstatAddVKDE1DAutoranged(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), 200);
    \endcode

    \image html datastore_statistics_kde.png


    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstatKDE1DAutoranged(), JKQTPXYLineGraph
*/
template <class InputIt>
inline JKQTPXYLineGraph* jkqtpstatAddVKDE1DAutoranged(JKQTBasePlotter* plotter, InputIt first, InputIt last, int Nout=100, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false, const QString& KDEcolumnBaseName=QString("KDE")) {
    size_t histcolX=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", positions");
    size_t histcolY=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", values");
    jkqtpstatKDE1DAutoranged(first, last, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), Nout, kernel, bandwidth, cummulative);
    JKQTPXYLineGraph* resO=new JKQTPXYLineGraph(plotter);
    resO->setXColumn(histcolY);
    resO->setYColumn(histcolX);
    resO->setTitle(KDEcolumnBaseName);
    resO->setDrawLine(true);
    resO->setSymbolType(JKQTPNoSymbol);
    plotter->addGraph(resO);
    return resO;
}



/*! \brief calculate an autoranged vertical KDE and add a JKQTPXYLineGraph to the given plotter, where the KDE is calculated from the data range \a first ... \a last, bins defined by their width
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binWidth width of the bins
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param cummulative if \c true, a cummulative KDE is calculated
    \param KDEcolumnBaseName this string is used in building the column names for the KDE data columns
    \return a graph class pointer (of type \a GraphClass ) displaying the KDE data

    Example:
    \code
        jkqtpstatAddVKDE1DAutoranged(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), 0.01);
    \endcode

    \image html datastore_statistics_kde.png


    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstatKDE1DAutoranged(), JKQTPXYLineGraph
*/
template <class InputIt>
inline JKQTPXYLineGraph* jkqtpstatAddVKDE1DAutoranged(JKQTBasePlotter* plotter, InputIt first, InputIt last, double binWidth, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false, const QString& KDEcolumnBaseName=QString("KDE")) {
    size_t histcolX=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", values");
    jkqtpstatKDE1DAutoranged(first, last, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), binWidth, kernel, bandwidth, cummulative);
    JKQTPXYLineGraph* resO=new JKQTPXYLineGraph(plotter);
    resO->setXColumn(histcolY);
    resO->setYColumn(histcolX);
    resO->setTitle(KDEcolumnBaseName);
    resO->setDrawLine(true);
    resO->setSymbolType(JKQTPNoSymbol);
    plotter->addGraph(resO);
    return resO;
}


/*! \brief calculate an autoranged vertical KDE and add a JKQTPXYLineGraph to the given plotter, where the KDE is calculated from the data range \a first ... \a last, bins defined by their width
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam BinsInputIt standard iterator type of \a binsFirst and \a binsLast.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binsFirst iterator pointing to the first item in the set of KDE bins
    \param binsLast iterator pointing behind the last item in the set of KDE bins
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param cummulative if \c true, a cummulative KDE is calculated
    \param KDEcolumnBaseName this string is used in building the column names for the KDE data columns
    \return a graph class pointer (of type \a GraphClass ) displaying the KDE data

    Example:
    \code
        std::vector<double> bins{-2,-1.5,-1,-0.75,-0.5,-0.25,0,0.25,0.5,0.75,1,1.5,2,2.5,3,4,5,6,7,8,9,10};
        jkqtpstatAddVKDE1D(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), bins.begin(), bins.end());
    \endcode

    \image html datastore_statistics_kde.png


    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstatKDE1D(), JKQTPXYLineGraph
*/
template <class InputIt, class BinsInputIt>
inline JKQTPXYLineGraph* jkqtpstatAddVKDE1D(JKQTBasePlotter* plotter, InputIt first, InputIt last, BinsInputIt binsFirst, BinsInputIt binsLast, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false, const QString& KDEcolumnBaseName=QString("KDE")) {
    size_t histcolX=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", values");
    jkqtpstatKDE1D(first, last, binsFirst, binsLast, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), kernel, bandwidth, cummulative);
    JKQTPXYLineGraph* resO=new JKQTPXYLineGraph(plotter);
    resO->setXColumn(histcolY);
    resO->setYColumn(histcolX);
    resO->setTitle(KDEcolumnBaseName);
    resO->setDrawLine(true);
    resO->setSymbolType(JKQTPNoSymbol);
    plotter->addGraph(resO);
    return resO;
}

/*! \brief calculate an autoranged vertical KDE and add a JKQTPXYLineGraph to the given plotter, where the KDE is calculated from the data range \a first ... \a last, evaluation positions are given by the range \a binXLeft ... \a binXRight (in steps of \a binxDelta )
    \ingroup jkqtptools_math_statistics_adaptors


    \tparam InputIt standard iterator type of \a first and \a last.
    \param plotter the plotter to which to add the resulting graph
    \param first iterator pointing to the first item in the dataset to use \f$ X_1 \f$
    \param last iterator pointing behind the last item in the dataset to use \f$ X_N \f$
    \param binXLeft first x-position, where to evaluate the KDE
    \param binXDelta distance between two x-positions at which the KDE is evaluated
    \param binXRight last x-position, where to evaluate the KDE
    \param kernel the kernel function to use (e.g. jkqtpstatKernel1DGaussian() )
    \param bandwidth bandwidth used for the KDE
    \param cummulative if \c true, a cummulative KDE is calculated
    \param KDEcolumnBaseName this string is used in building the column names for the KDE data columns
    \return a graph class pointer (of type \a GraphClass ) displaying the KDE data

    Example:
    \code
        std::vector<double> bins{-2,-1.5,-1,-0.75,-0.5,-0.25,0,0.25,0.5,0.75,1,1.5,2,2.5,3,4,5,6,7,8,9,10};
        jkqtpstatAddVKDE1D(plot1->getPlotter(),  datastore1->begin(randomdatacol1), datastore1->end(randomdatacol1), bins.begin(), bins.end());
    \endcode

    \image html datastore_statistics_kde.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreStatistics, jkqtpstatKDE1D(), JKQTPXYLineGraph
*/
template <class InputIt>
inline JKQTPXYLineGraph* jkqtpstatAddVKDE1D(JKQTBasePlotter* plotter, InputIt first, InputIt last, double binXLeft, double binXDelta, double binXRight, const std::function<double(double)>& kernel=std::function<double(double)>(&jkqtpstatKernel1DGaussian), double bandwidth=1.0, bool cummulative=false, const QString& KDEcolumnBaseName=QString("KDE")) {
    size_t histcolX=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(KDEcolumnBaseName+", values");
    jkqtpstatKDE1D(first, last, binXLeft, binXDelta, binXRight, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), kernel, bandwidth, cummulative);
    JKQTPXYLineGraph* resO=new JKQTPXYLineGraph(plotter);
    resO->setXColumn(histcolY);
    resO->setYColumn(histcolX);
    resO->setTitle(KDEcolumnBaseName);
    resO->setDrawLine(true);
    resO->setSymbolType(JKQTPNoSymbol);
    plotter->addGraph(resO);
    return resO;
}















/*! \brief calculate calculate a 2-dimensional kernel density estimate (KDE) and add a JKQTPColumnMathImage to the given plotter, where the KDE is calculated from the given data range \a firstX / \a firstY ... \a lastY / \a lastY
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param plotter the plotter to which to add the resulting graph
    \param firstX iterator pointing to the first x-position item in the dataset to use \f$ X_1 \f$
    \param lastX iterator pointing behind the last x-position item in the dataset to use \f$ X_N \f$
    \param firstY iterator pointing to the first y-position item in the dataset to use \f$ Y_1 \f$
    \param lastY iterator pointing behind the last y-position item in the dataset to use \f$ Y_N \f$
    \param xbins number of bins in x-direction (i.e. width of the output KDE)
    \param ybins number of bins in y-direction (i.e. height of the output KDE)
    \param kernel the kernel function to use (e.g. jkqtpstatKernel2DGaussian() )
    \param bandwidthX x-bandwidth used for the KDE
    \param bandwidthY y-bandwidth used for the KDE
    \param kdecolumnBaseName this string is used in building the column names for the KDE data columns
    \param[out] oxmin position of the first KDE bin in x-direction
    \param[out] oxmax position of the last KDE bin in x-direction
    \param[out] oymin position of the first KDE bin in y-direction
    \param[out] oymax position of the last KDE bin in y-direction
    \return a graph class pointer (of type \c JKQTPColumnMathImage ) displaying the KDE data

    \image html datastore_statistics_2d_kde.png

    \see jkqtpstatKDE2D(), \ref JKQTPlotterBasicJKQTPDatastoreStatistics2D
*/
template <class InputItX, class InputItY>
inline JKQTPColumnMathImage* jkqtpstatAddKDE2DImage(JKQTBasePlotter* plotter, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, size_t xbins=10, size_t ybins=10, const std::function<double(double,double)>& kernel=std::function<double(double,double)>(&jkqtpstatKernel2DGaussian), double bandwidthX=1.0, double bandwidthY=1.0, const QString& kdecolumnBaseName=QString("histogram"), double* oxmin=nullptr, double* oxmax=nullptr, double* oymin=nullptr, double* oymax=nullptr) {
    double xmin=0, xmax=0;
    double ymin=0, ymax=0;
    jkqtpstatMinMax(firstX,lastX, xmin,xmax);
    jkqtpstatMinMax(firstY,lastY, ymin,ymax);
    size_t histcol=plotter->getDatastore()->addImageColumn(xbins, ybins, kdecolumnBaseName);
    jkqtpstatKDE2D(firstX,lastX,firstY,lastY,
                         plotter->getDatastore()->begin(histcol),
                         xmin, xmax, ymin, ymax,
                         xbins, ybins, kernel, bandwidthX, bandwidthY);
    JKQTPColumnMathImage* gHist;
    plotter->addGraph(gHist=new JKQTPColumnMathImage(plotter));
    gHist->setImageColumn(static_cast<int>(histcol));
    gHist->setX(xmin);
    gHist->setY(ymin);
    gHist->setWidth(xmax-xmin);
    gHist->setHeight(ymax-ymin);
    gHist->setTitle(QObject::tr("2D KDE"));
    if (oxmax) *oxmax=xmax;
    if (oxmin) *oxmin=xmin;
    if (oymax) *oymax=ymax;
    if (oymin) *oymin=ymin;
    return gHist;
}






/*! \brief calculate calculate a 2-dimensional kernel density estimate (KDE) and add a JKQTPColumnContourPlot to the given plotter, where the KDE is calculated from the given data range \a firstX / \a firstY ... \a lastY / \a lastY
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param plotter the plotter to which to add the resulting graph
    \param firstX iterator pointing to the first x-position item in the dataset to use \f$ X_1 \f$
    \param lastX iterator pointing behind the last x-position item in the dataset to use \f$ X_N \f$
    \param firstY iterator pointing to the first y-position item in the dataset to use \f$ Y_1 \f$
    \param lastY iterator pointing behind the last y-position item in the dataset to use \f$ Y_N \f$
    \param xbins number of bins in x-direction (i.e. width of the output KDE)
    \param ybins number of bins in y-direction (i.e. height of the output KDE)
    \param kernel the kernel function to use (e.g. jkqtpstatKernel2DGaussian() )
    \param bandwidthX x-bandwidth used for the KDE
    \param bandwidthY y-bandwidth used for the KDE
    \param kdecolumnBaseName this string is used in building the column names for the KDE data columns
    \param[out] oxmin position of the first KDE bin in x-direction
    \param[out] oxmax position of the last KDE bin in x-direction
    \param[out] oymin position of the first KDE bin in y-direction
    \param[out] oymax position of the last KDE bin in y-direction
    \return a graph class pointer (of type \c JKQTPColumnContourPlot ) displaying the KDE data as a contour plot

    \image html datastore_statistics_2d_kdecontour.png

    \see jkqtpstatKDE2D(), \ref JKQTPlotterBasicJKQTPDatastoreStatistics2D
*/
template <class InputItX, class InputItY>
inline JKQTPColumnContourPlot* jkqtpstatAddKDE2DContour(JKQTBasePlotter* plotter, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, size_t xbins=10, size_t ybins=10, const std::function<double(double,double)>& kernel=std::function<double(double,double)>(&jkqtpstatKernel2DGaussian), double bandwidthX=1.0, double bandwidthY=1.0, const QString& kdecolumnBaseName=QString("histogram"), double* oxmin=nullptr, double* oxmax=nullptr, double* oymin=nullptr, double* oymax=nullptr) {
    double xmin=0, xmax=0;
    double ymin=0, ymax=0;
    jkqtpstatMinMax(firstX,lastX, xmin,xmax);
    jkqtpstatMinMax(firstY,lastY, ymin,ymax);
    size_t histcol=plotter->getDatastore()->addImageColumn(xbins, ybins, kdecolumnBaseName);
    jkqtpstatKDE2D(firstX,lastX,firstY,lastY,
                         plotter->getDatastore()->begin(histcol),
                         xmin, xmax, ymin, ymax,
                         xbins, ybins, kernel, bandwidthX, bandwidthY);
    JKQTPColumnContourPlot* gHist;
    plotter->addGraph(gHist=new JKQTPColumnContourPlot(plotter));
    gHist->setImageColumn(static_cast<int>(histcol));
    gHist->setX(xmin);
    gHist->setY(ymin);
    gHist->setWidth(xmax-xmin);
    gHist->setHeight(ymax-ymin);
    gHist->setTitle(QObject::tr("2D KDE"));
    gHist->createContourLevels(5);
    if (oxmax) *oxmax=xmax;
    if (oxmin) *oxmin=xmin;
    if (oymax) *oymax=ymax;
    if (oymin) *oymin=ymin;
    return gHist;
}







/*! \brief calculate the linear regression coefficients for a given data range \a firstX / \a firstY ... \a lastX / \a lastY where the model is \f$ f(x)=a+b\cdot x \f$
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param plotter the plotter to which to add the resulting graph
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used \note If \a fixA \c ==true, You need to provide a value for A in \a coeffA
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used \note If \a fixB \c ==true, You need to provide a value for B in \a coeffB

    Example:
    \code
        jkqtpstatAddLinearRegression(plot1->getPlotter(), datastore1->begin(colLinX), datastore1->end(colLinX), datastore1->begin(colLinY), datastore1->end(colLinY));
    \endcode

    \image html datastore_regression_lin.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatLinearRegression()
*/
template <class InputItX, class InputItY>
inline JKQTPXFunctionLineGraph* jkqtpstatAddLinearRegression(JKQTBasePlotter* plotter, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, double* coeffA=nullptr, double* coeffB=nullptr, bool fixA=false, bool fixB=false) {
    double cA=0;
    if (coeffA) cA=*coeffA;
    double cB=0;
    if (coeffB) cB=*coeffB;

    JKQTPASSERT_M(!fixA || (fixA && coeffA!=nullptr), "if fixA=true, coeffA needs to be provided");
    JKQTPASSERT_M(!fixB || (fixB && coeffB!=nullptr), "if fixB=true, coeffB needs to be provided");

    jkqtpstatLinearRegression(firstX, lastX, firstY, lastY, cA, cB, fixA, fixB);
    JKQTPXFunctionLineGraph* g=new JKQTPXFunctionLineGraph(plotter);
    g->setSpecialFunction(JKQTPXFunctionLineGraph::SpecialFunction::Line);
    g->setParamsV(cA, cB);
    g->setTitle(QString("regression: $f(x) = %1%2{\\cdot}x, \\chi^2=%4, R^2=%3$").arg(jkqtp_floattolatexqstr(cA, 2, true, 1e-16,1e-2, 1e4,false)).arg(jkqtp_floattolatexqstr(cB, 2, true, 1e-16,1e-2, 1e4,true)).arg(jkqtp_floattolatexqstr(jkqtpstatCoefficientOfDetermination(firstX,lastX,firstY,lastY,jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType::Linear, cA, cB)),3)).arg(jkqtp_floattolatexqstr(jkqtpstatSumOfDeviations(firstX,lastX,firstY,lastY,jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType::Linear, cA, cB)),3)));
    plotter->addGraph(g);
    if (coeffA) *coeffA=cA;
    if (coeffB) *coeffB=cB;
    return g;
}


/*! \brief calculate the linear regression coefficients for a given data data used to draw any `JKQTPXYGraph` \a datagraph  where the model is \f$ f(x)=a+b\cdot x \f$
    \ingroup jkqtptools_math_statistics_adaptors

    \param datagraph graph representing the (x,y) datapairs to which to fit the regression line
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used \note If \a fixA \c ==true, You need to provide a value for A in \a coeffA
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used \note If \a fixB \c ==true, You need to provide a value for B in \a coeffB

    Example:
    \code
        JKQTPXYLineGraph* graphD;
        plot1->addGraph(graphD=new JKQTPXYLineGraph(plot1));
        graphD->setXYColumns(colLinX, colLinY);
        jkqtpstatAddLinearRegression(graphD);
    \endcode

    \image html datastore_regression_lin.png

    \note The line graph is added to the same plotter that is the parent of \a datagraph !

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatLinearRegression()
*/
JKQTPLOTTER_LIB_EXPORT JKQTPXFunctionLineGraph* jkqtpstatAddLinearRegression(JKQTPXYGraph *datagraph, double* coeffA=nullptr, double* coeffB=nullptr, bool fixA=false, bool fixB=false);



/*! \brief  calculate the (robust) iteratively reweighted least-squares (IRLS) estimate for the parameters of the model \f$ f(x)=a+b\cdot x \f$
           for a given data range \a firstX / \a firstY ... \a lastX / \a lastY
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param plotter the plotter to which to add the resulting graph
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used \note If \a fixA \c ==true, You need to provide a value for A in \a coeffA
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used \note If \a fixB \c ==true, You need to provide a value for B in \a coeffB
    \param p regularization parameter, the optimization problem is formulated in the \f$ L_p \f$ norm, using this \a p (see image below for an example)
    \param iterations the number of iterations the IRLS algorithm performs

    Example:
    \code
        jkqtpstatAddRobustIRLSLinearRegression(plot1->getPlotter(), datastore1->begin(colLinX), datastore1->end(colLinX), datastore1->begin(colLinY), datastore1->end(colLinY));
    \endcode

    \image html datastore_regression_linrobust.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatRobustIRLSLinearRegression()
*/
template <class InputItX, class InputItY>
inline JKQTPXFunctionLineGraph* jkqtpstatAddRobustIRLSLinearRegression(JKQTBasePlotter* plotter, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, double* coeffA=nullptr, double* coeffB=nullptr, bool fixA=false, bool fixB=false, double p=1.1, int iterations=100) {
    double cA=0;
    if (coeffA) cA=*coeffA;
    double cB=0;
    if (coeffB) cB=*coeffB;

    JKQTPASSERT_M(!fixA || (fixA && coeffA!=nullptr), "if fixA=true, coeffA needs to be provided");
    JKQTPASSERT_M(!fixB || (fixB && coeffB!=nullptr), "if fixB=true, coeffB needs to be provided");

    jkqtpstatRobustIRLSLinearRegression(firstX, lastX, firstY, lastY, cA, cB, fixA, fixB, p, iterations);
    JKQTPXFunctionLineGraph* g=new JKQTPXFunctionLineGraph(plotter);
    g->setSpecialFunction(JKQTPXFunctionLineGraph::SpecialFunction::Line);
    g->setParamsV(cA, cB);
    g->setTitle(QString("robust regression: $f(x) = %1%2{\\cdot}x, \\chi^2=%4, R^2=%3$").arg(jkqtp_floattolatexqstr(cA, 2, true, 1e-16,1e-2, 1e4,false)).arg(jkqtp_floattolatexqstr(cB, 2, true, 1e-16,1e-2, 1e4,true)).arg(jkqtp_floattolatexqstr(jkqtpstatCoefficientOfDetermination(firstX,lastX,firstY,lastY,jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType::Linear, cA, cB)),3)).arg(jkqtp_floattolatexqstr(jkqtpstatSumOfDeviations(firstX,lastX,firstY,lastY,jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType::Linear, cA, cB)),3)));
    plotter->addGraph(g);
    if (coeffA) *coeffA=cA;
    if (coeffB) *coeffB=cB;
    return g;
}


/*! \brief  calculate the (robust) iteratively reweighted least-squares (IRLS) estimate for the parameters of the model \f$ f(x)=a+b\cdot x \f$
           for a given data range \a firstX / \a firstY ... \a lastX / \a lastY
    \ingroup jkqtptools_math_statistics_adaptors

    \param datagraph graph representing the (x,y) datapairs to which to fit the regression line
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used \note If \a fixA \c ==true, You need to provide a value for A in \a coeffA
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used \note If \a fixB \c ==true, You need to provide a value for B in \a coeffB
    \param p regularization parameter, the optimization problem is formulated in the \f$ L_p \f$ norm, using this \a p (see image below for an example)
    \param iterations the number of iterations the IRLS algorithm performs

    Example:
    \code
        JKQTPXYLineGraph* graphD;
        plot1->addGraph(graphD=new JKQTPXYLineGraph(plot1));
        graphD->setXYColumns(colLinX, colLinY);
        jkqtpstatAddRobustIRLSLinearRegression(graphD);
    \endcode

    \image html datastore_regression_linrobust.png

    \note The line graph is added to the same plotter that is the parent of \a datagraph !

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatRobustIRLSLinearRegression()
*/
JKQTPLOTTER_LIB_EXPORT JKQTPXFunctionLineGraph *jkqtpstatAddRobustIRLSLinearRegression(JKQTPXYGraph *datagraph, double* coeffA=nullptr, double* coeffB=nullptr, bool fixA=false, bool fixB=false, double p=1.1, int iterations=100);



/*! \brief calculate the weighted linear regression coefficients for a given for a given data range \a firstX / \a firstY / \a firstW ... \a lastX / \a lastY / \a lastW  where the model is \f$ f(x)=a+b\cdot x \f$
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \tparam InputItW standard iterator type of \a firstW and \a lastW.
    \param plotter the plotter to which to add the resulting graph
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param firstW iterator pointing to the first item in the weight-dataset to use \f$ w_1 \f$
    \param lastW iterator pointing behind the last item in the weight-dataset to use \f$ w_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used \note If \a fixA \c ==true, You need to provide a value for A in \a coeffA
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used \note If \a fixB \c ==true, You need to provide a value for B in \a coeffB
    \param fWeightDataToWi an optional function, which is applied to the data from \a firstW ... \a lastW to convert them to weight, i.e. \c wi=fWeightDataToWi(*itW)
                           e.g. if you use data used to draw error bars, you can use jkqtp_inversePropSaveDefault(). The default is jkqtp_identity(), which just returns the values.
                           In the case of jkqtp_inversePropSaveDefault(), a datapoint x,y, has a large weight, if it's error is small and in the case if jkqtp_identity() it's weight
                           is directly proportional to the given value.

    Example:
    \code
        double coeffA=0, coeffB=0;
        jkqtpstatLinearWeightedRegression(datastore1->begin(colWLinX), datastore1->end(colWLinX),
                                          datastore1->begin(colWLinY), datastore1->end(colWLinY),
                                          datastore1->begin(colWLinE), datastore1->end(colWLinE),
                                          coeffA, coeffB, false, false,
                                          &jkqtp_inversePropSaveDefault<double>);
    \endcode

    \image html datastore_regression_linweight.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatLinearRegression()
*/
template <class InputItX, class InputItY, class InputItW>
inline JKQTPXFunctionLineGraph* jkqtpstatAddLinearWeightedRegression(JKQTBasePlotter* plotter, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, InputItW firstW, InputItW lastW, double* coeffA=nullptr, double* coeffB=nullptr, bool fixA=false, bool fixB=false, std::function<double(double)> fWeightDataToWi=&jkqtp_identity<double>) {
    double cA=0;
    if (coeffA) cA=*coeffA;
    double cB=0;
    if (coeffB) cB=*coeffB;

    JKQTPASSERT_M(!fixA || (fixA && coeffA!=nullptr), "if fixA=true, coeffA needs to be provided");
    JKQTPASSERT_M(!fixB || (fixB && coeffB!=nullptr), "if fixB=true, coeffB needs to be provided");

    jkqtpstatLinearWeightedRegression(firstX, lastX, firstY, lastY, firstW, lastW, cA, cB, fixA, fixB, fWeightDataToWi);
    JKQTPXFunctionLineGraph* g=new JKQTPXFunctionLineGraph(plotter);
    g->setSpecialFunction(JKQTPXFunctionLineGraph::SpecialFunction::Line);
    g->setParamsV(cA, cB);
    g->setTitle(QString("weighted regression: $f(x) = %1%2{\\cdot}x, \\chi^2=%4, R^2=%3$").arg(jkqtp_floattolatexqstr(cA, 2, true, 1e-16,1e-2, 1e4,false)).arg(jkqtp_floattolatexqstr(cB, 2, true, 1e-16,1e-2, 1e4,true)).arg(jkqtp_floattolatexqstr(jkqtpstatWeightedCoefficientOfDetermination(firstX,lastX,firstY,lastY,firstW,lastW,jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType::Linear, cA, cB),fWeightDataToWi),3)).arg(jkqtp_floattolatexqstr(jkqtpstatWeightedSumOfDeviations(firstX,lastX,firstY,lastY,firstW,lastW,jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType::Linear, cA, cB),fWeightDataToWi),3)));
    plotter->addGraph(g);
    if (coeffA) *coeffA=cA;
    if (coeffB) *coeffB=cB;
    return g;
}

/*! \brief calculate the linear weighted regression coefficients for a given data data used to draw any `JKQTPXYGraph` \a datagraph , which also implements JKQTPYGraphErrorData and where the model is \f$ f(x)=a+b\cdot x \f$
    \ingroup jkqtptools_math_statistics_adaptors

    \param datagraph graph representing the (x,y,error) data triples to which to fit the regression line
                     The errors are used as iverse weights!
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used \note If \a fixA \c ==true, You need to provide a value for A in \a coeffA
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used \note If \a fixB \c ==true, You need to provide a value for B in \a coeffB

    Example:
    \code
        JKQTPXYLineErrorGraph* graphD;
        plot1->addGraph(graphD=new JKQTPXYLineErrorGraph(plot1));
        graphD->setXYColumns(colLinX, colLinY);
        graphD->setYErrorColumn(static_cast<int>(colWLinE));
        jkqtpstatAddLinearWeightedRegression(graphD);
    \endcode

    \image html datastore_regression_linweight.png

    \note The line graph is added to the same plotter that is the parent of \a datagraph !

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatLinearRegression()
*/
JKQTPLOTTER_LIB_EXPORT JKQTPXFunctionLineGraph* jkqtpstatAddLinearWeightedRegression(JKQTPXYGraph *datagraph, double* coeffA=nullptr, double* coeffB=nullptr, bool fixA=false, bool fixB=false);














/*! \brief calculate the linear regression coefficients for a given data range \a firstX / \a firstY ... \a lastX / \a lastY where the model is defined by \a type
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param plotter the plotter to which to add the resulting graph
    \param type model to be fitted
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used \note If \a fixA \c ==true, You need to provide a value for A in \a coeffA
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used \note If \a fixB \c ==true, You need to provide a value for B in \a coeffB

    Example:
    \code
        jkqtpstatRegression(plot1->getPlotter(), JKQTPStatRegressionModelType::Exponential, datastore1->begin(colLinX), datastore1->end(colLinX), datastore1->begin(colLinY), datastore1->end(colLinY));
    \endcode

    \image html datastore_regression_nonlinreg.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatRegression()
*/
template <class InputItX, class InputItY>
inline JKQTPXFunctionLineGraph* jkqtpstatAddRegression(JKQTBasePlotter* plotter, JKQTPStatRegressionModelType type, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, double* coeffA=nullptr, double* coeffB=nullptr, bool fixA=false, bool fixB=false) {
    double cA=0;
    if (coeffA) cA=*coeffA;
    double cB=0;
    if (coeffB) cB=*coeffB;

    JKQTPASSERT_M(!fixA || (fixA && coeffA!=nullptr), "if fixA=true, coeffA needs to be provided");
    JKQTPASSERT_M(!fixB || (fixB && coeffB!=nullptr), "if fixB=true, coeffB needs to be provided");

    jkqtpstatRegression(type, firstX, lastX, firstY, lastY, cA, cB, fixA, fixB);
    JKQTPXFunctionLineGraph* g=new JKQTPXFunctionLineGraph(plotter);
    g->setPlotFunctionFunctor(jkqtpStatGenerateRegressionModel(type, cA, cB));
    g->setTitle(QString("regression: $%1, \\chi^2=%3, R^2=%2$").arg(jkqtpstatRegressionModel2Latex(type, cA, cB)).arg(jkqtp_floattolatexqstr(jkqtpstatCoefficientOfDetermination(firstX,lastX,firstY,lastY,jkqtpStatGenerateRegressionModel(type, cA, cB)),3)).arg(jkqtp_floattolatexqstr(jkqtpstatSumOfDeviations(firstX,lastX,firstY,lastY,jkqtpStatGenerateRegressionModel(type, cA, cB)),3)));
    plotter->addGraph(g);
    if (coeffA) *coeffA=cA;
    if (coeffB) *coeffB=cB;
    return g;
}


/*! \brief calculate the linear regression coefficients for a given data data used to draw any `JKQTPXYGraph` \a datagraph  where the model is defined by \a type

    \param datagraph graph representing the (x,y) datapairs to which to fit the regression line
    \param type model to be fitted
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used \note If \a fixA \c ==true, You need to provide a value for A in \a coeffA
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used \note If \a fixB \c ==true, You need to provide a value for B in \a coeffB

    Example:
    \code
        JKQTPXYLineGraph* graphD;
        plot1->addGraph(graphD=new JKQTPXYLineGraph(plot1));
        graphD->setXYColumns(colLinX, colLinY);
        jkqtpstatRegression(graphD, JKQTPStatRegressionModelType::Exponential);
    \endcode

    \image html datastore_regression_nonlinreg.png

    \note The line graph is added to the same plotter that is the parent of \a datagraph !

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatRegression()
*/
JKQTPLOTTER_LIB_EXPORT JKQTPXFunctionLineGraph* jkqtpstatAddRegression(JKQTPXYGraph *datagraph, JKQTPStatRegressionModelType type, double* coeffA=nullptr, double* coeffB=nullptr, bool fixA=false, bool fixB=false);



/*! \brief  calculate the (robust) iteratively reweighted least-squares (IRLS) estimate for the parameters where the model is defined by \a type
           for a given data range \a firstX / \a firstY ... \a lastX / \a lastY
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param plotter the plotter to which to add the resulting graph
    \param type model to be fitted
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used \note If \a fixA \c ==true, You need to provide a value for A in \a coeffA
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used \note If \a fixB \c ==true, You need to provide a value for B in \a coeffB
    \param p regularization parameter, the optimization problem is formulated in the \f$ L_p \f$ norm, using this \a p (see image below for an example)
    \param iterations the number of iterations the IRLS algorithm performs

    Example:
    \code
        jkqtpstatAddRobustIRLSRegression(plot1->getPlotter(), JKQTPStatRegressionModelType::Exponential, datastore1->begin(colLinX), datastore1->end(colLinX), datastore1->begin(colLinY), datastore1->end(colLinY));
    \endcode

    \image html datastore_regression_linrobust.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatRobustIRLSRegression()
*/
template <class InputItX, class InputItY>
inline JKQTPXFunctionLineGraph* jkqtpstatAddRobustIRLSRegression(JKQTBasePlotter* plotter, JKQTPStatRegressionModelType type, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, double* coeffA=nullptr, double* coeffB=nullptr, bool fixA=false, bool fixB=false, double p=1.1, int iterations=100) {
    double cA=0;
    if (coeffA) cA=*coeffA;
    double cB=0;
    if (coeffB) cB=*coeffB;

    JKQTPASSERT_M(!fixA || (fixA && coeffA!=nullptr), "if fixA=true, coeffA needs to be provided");
    JKQTPASSERT_M(!fixB || (fixB && coeffB!=nullptr), "if fixB=true, coeffB needs to be provided");

    jkqtpstatRobustIRLSRegression(type, firstX, lastX, firstY, lastY, cA, cB, fixA, fixB, p, iterations);
    JKQTPXFunctionLineGraph* g=new JKQTPXFunctionLineGraph(plotter);
    g->setPlotFunctionFunctor(jkqtpStatGenerateRegressionModel(type, cA, cB));
    g->setTitle(QString("robust regression: $%1, \\chi^2=%3, R^2=%2$").arg(jkqtpstatRegressionModel2Latex(type, cA, cB)).arg(jkqtp_floattolatexqstr(jkqtpstatCoefficientOfDetermination(firstX,lastX,firstY,lastY,jkqtpStatGenerateRegressionModel(type, cA, cB)),3)).arg(jkqtp_floattolatexqstr(jkqtpstatSumOfDeviations(firstX,lastX,firstY,lastY,jkqtpStatGenerateRegressionModel(type, cA, cB)),3)));
    plotter->addGraph(g);
    if (coeffA) *coeffA=cA;
    if (coeffB) *coeffB=cB;
    return g;
}


/*! \brief  calculate the (robust) iteratively reweighted least-squares (IRLS) estimate for the parameters where the model is defined by \a type
           for a given data range \a firstX / \a firstY ... \a lastX / \a lastY
    \ingroup jkqtptools_math_statistics_adaptors

    \param datagraph graph representing the (x,y) datapairs to which to fit the regression line
    \param type model to be fitted
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used \note If \a fixA \c ==true, You need to provide a value for A in \a coeffA
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used \note If \a fixB \c ==true, You need to provide a value for B in \a coeffB
    \param p regularization parameter, the optimization problem is formulated in the \f$ L_p \f$ norm, using this \a p (see image below for an example)
    \param iterations the number of iterations the IRLS algorithm performs

    Example:
    \code
        JKQTPXYLineGraph* graphD;
        plot1->addGraph(graphD=new JKQTPXYLineGraph(plot1));
        graphD->setXYColumns(colLinX, colLinY);
        jkqtpstatAddRobustIRLSRegression(graphD, JKQTPStatRegressionModelType::Exponential);
    \endcode

    \image html datastore_regression_linrobust.png

    \note The line graph is added to the same plotter that is the parent of \a datagraph !

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatRobustIRLSRegression()
*/
JKQTPLOTTER_LIB_EXPORT JKQTPXFunctionLineGraph *jkqtpstatAddRobustIRLSRegression(JKQTPXYGraph *datagraph, JKQTPStatRegressionModelType type, double* coeffA=nullptr, double* coeffB=nullptr, bool fixA=false, bool fixB=false, double p=1.1, int iterations=100);



/*! \brief calculate the weighted linear regression coefficients for a given for a given data range \a firstX / \a firstY / \a firstW ... \a lastX / \a lastY / \a lastW  where the model is defined by \a type
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \tparam InputItW standard iterator type of \a firstW and \a lastW.
    \param plotter the plotter to which to add the resulting graph
    \param type model to be fitted
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param firstW iterator pointing to the first item in the weight-dataset to use \f$ w_1 \f$
    \param lastW iterator pointing behind the last item in the weight-dataset to use \f$ w_N \f$
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used \note If \a fixA \c ==true, You need to provide a value for A in \a coeffA
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used \note If \a fixB \c ==true, You need to provide a value for B in \a coeffB
    \param fWeightDataToWi an optional function, which is applied to the data from \a firstW ... \a lastW to convert them to weight, i.e. \c wi=fWeightDataToWi(*itW)
                           e.g. if you use data used to draw error bars, you can use jkqtp_inversePropSaveDefault(). The default is jkqtp_identity(), which just returns the values.
                           In the case of jkqtp_inversePropSaveDefault(), a datapoint x,y, has a large weight, if it's error is small and in the case if jkqtp_identity() it's weight
                           is directly proportional to the given value.

    Example:
    \code
        double coeffA=0, coeffB=0;
        jkqtpstatAddWeightedRegression(plotter, JKQTPStatRegressionModelType::Exponential,
                                          datastore1->begin(colWLinX), datastore1->end(colWLinX),
                                          datastore1->begin(colWLinY), datastore1->end(colWLinY),
                                          datastore1->begin(colWLinE), datastore1->end(colWLinE),
                                          coeffA, coeffB, false, false,
                                          &jkqtp_inversePropSaveDefault<double>);
    \endcode

    \image html datastore_regression_linweight.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatWeightedRegression()
*/
template <class InputItX, class InputItY, class InputItW>
inline JKQTPXFunctionLineGraph* jkqtpstatAddWeightedRegression(JKQTBasePlotter* plotter, JKQTPStatRegressionModelType type, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, InputItW firstW, InputItW lastW, double* coeffA=nullptr, double* coeffB=nullptr, bool fixA=false, bool fixB=false, std::function<double(double)> fWeightDataToWi=&jkqtp_identity<double>) {
    double cA=0;
    if (coeffA) cA=*coeffA;
    double cB=0;
    if (coeffB) cB=*coeffB;

    JKQTPASSERT_M(!fixA || (fixA && coeffA!=nullptr), "if fixA=true, coeffA needs to be provided");
    JKQTPASSERT_M(!fixB || (fixB && coeffB!=nullptr), "if fixB=true, coeffB needs to be provided");

    jkqtpstatWeightedRegression(type, firstX, lastX, firstY, lastY, firstW, lastW, cA, cB, fixA, fixB, fWeightDataToWi);
    JKQTPXFunctionLineGraph* g=new JKQTPXFunctionLineGraph(plotter);
    g->setPlotFunctionFunctor(jkqtpStatGenerateRegressionModel(type, cA, cB));
    g->setTitle(QString("weighted regression: $%1, \\chi^2=%3, R^2=%2$").arg(jkqtpstatRegressionModel2Latex(type, cA, cB)).arg(jkqtp_floattolatexqstr(jkqtpstatCoefficientOfDetermination(firstX,lastX,firstY,lastY,jkqtpStatGenerateRegressionModel(type, cA, cB)),3)).arg(jkqtp_floattolatexqstr(jkqtpstatSumOfDeviations(firstX,lastX,firstY,lastY,jkqtpStatGenerateRegressionModel(type, cA, cB)),3)));
    plotter->addGraph(g);
    if (coeffA) *coeffA=cA;
    if (coeffB) *coeffB=cB;
    return g;
}

/*! \brief calculate the linear weighted regression coefficients for a given data data used to draw any `JKQTPXYGraph` \a datagraph , which also implements JKQTPYGraphErrorData and where the model is defined by \a type
    \ingroup jkqtptools_math_statistics_adaptors

    \param datagraph graph representing the (x,y,error) data triples to which to fit the regression line
                     The errors are used as iverse weights!
    \param type model to be fitted
    \param[in,out] coeffA returns the offset of the linear model
    \param[in,out] coeffB returns the slope of the linear model
    \param fixA if \c true, the offset coefficient \f$ a \f$ is not determined by the fit, but the value provided in \a coeffA is used \note If \a fixA \c ==true, You need to provide a value for A in \a coeffA
    \param fixB if \c true, the slope coefficient \f$ b \f$ is not determined by the fit, but the value provided in \a coeffB is used \note If \a fixB \c ==true, You need to provide a value for B in \a coeffB

    Example:
    \code
        JKQTPXYLineErrorGraph* graphD;
        plot1->addGraph(graphD=new JKQTPXYLineErrorGraph(plot1));
        graphD->setXYColumns(colLinX, colLinY);
        graphD->setYErrorColumn(static_cast<int>(colWLinE));
        jkqtpstatAddWeightedRegression(graphD, JKQTPStatRegressionModelType::Exponential);
    \endcode

    \image html datastore_regression_linweight.png

    \note The line graph is added to the same plotter that is the parent of \a datagraph !

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatWeightedRegression()
*/
JKQTPLOTTER_LIB_EXPORT JKQTPXFunctionLineGraph* jkqtpstatAddWeightedRegression(JKQTPXYGraph *datagraph, JKQTPStatRegressionModelType type, double* coeffA=nullptr, double* coeffB=nullptr, bool fixA=false, bool fixB=false);







/*! \brief fits (in a least-squares sense) a polynomial \f$ f(x)=\sum\limits_{i=0}^Pp_ix^i \f$ of order P to a set of N data pairs \f$ (x_i,y_i) \f$ from a given data range \a firstX / \a firstY ... \a lastX / \a lastY
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \tparam OutputItP output iterator for the polynomial coefficients
    \param plotter the plotter to which to add the resulting graph
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param P degree of the polynomial (P>=N !!!)
    \param[out] firstRes Iterator (of type \a OutputItP ), which receives the (P+1)-entry vector with the polynomial coefficients \f$ p_i \f$

    Example:
    \code
        std::vector<double> pFit;
        jkqtpstatAddPolyFit(plot1->getPlotter(), JKQTPStatRegressionModelType::Exponential, datastore1->begin(colLinX), datastore1->end(colLinX), datastore1->begin(colLinY), datastore1->end(colLinY), 3, std::back_inserter(pFit));
    \endcode

    \image html datastore_regression_polynom.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatPolyFit()
*/
template <class InputItX, class InputItY, class OutputItP>
inline JKQTPXFunctionLineGraph* jkqtpstatAddPolyFit(JKQTBasePlotter* plotter, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, size_t P, OutputItP firstRes) {
    std::vector<double> pFit;
    JKQTPXFunctionLineGraph* gPoly=new JKQTPXFunctionLineGraph(plotter);
    jkqtpstatPolyFit(firstX,lastX,firstY,lastY,P,std::back_inserter(pFit));
    gPoly->setPlotFunctionFunctor(jkqtp_generatePolynomialModel(pFit.begin(), pFit.end()));
    gPoly->setTitle(QString("regression: $%1, \\chi^2=%3, R^2=%2$").arg(jkqtp_polynomialModel2Latex(pFit.begin(), pFit.end())).arg(jkqtp_floattolatexqstr(jkqtpstatCoefficientOfDetermination(firstX,lastX,firstY,lastY,jkqtp_generatePolynomialModel(pFit.begin(), pFit.end())),3)).arg(jkqtp_floattolatexqstr(jkqtpstatSumOfDeviations(firstX,lastX,firstY,lastY,jkqtp_generatePolynomialModel(pFit.begin(), pFit.end())),3)));
    std::copy(pFit.begin(), pFit.end(), firstRes);
    plotter->addGraph(gPoly);
    return gPoly;
}


/*! \brief fits (in a least-squares sense) a polynomial \f$ f(x)=\sum\limits_{i=0}^Pp_ix^i \f$ of order P to a set of N data pairs \f$ (x_i,y_i) \f$ from a given data range \a firstX / \a firstY ... \a lastX / \a lastY
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputItX standard iterator type of \a firstX and \a lastX.
    \tparam InputItY standard iterator type of \a firstY and \a lastY.
    \param plotter the plotter to which to add the resulting graph
    \param firstX iterator pointing to the first item in the x-dataset to use \f$ x_1 \f$
    \param lastX iterator pointing behind the last item in the x-dataset to use \f$ x_N \f$
    \param firstY iterator pointing to the first item in the y-dataset to use \f$ y_1 \f$
    \param lastY iterator pointing behind the last item in the y-dataset to use \f$ y_N \f$
    \param P degree of the polynomial (P>=N !!!)

    Example:
    \code
        jkqtpstatAddPolyFit(plot1->getPlotter(), JKQTPStatRegressionModelType::Exponential, datastore1->begin(colLinX), datastore1->end(colLinX), datastore1->begin(colLinY), datastore1->end(colLinY), 3);
    \endcode

    \image html datastore_regression_polynom.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatPolyFit()
*/
template <class InputItX, class InputItY>
inline JKQTPXFunctionLineGraph* jkqtpstatAddPolyFit(JKQTBasePlotter* plotter, InputItX firstX, InputItX lastX, InputItY firstY, InputItY lastY, size_t P) {
    std::vector<double> pFit;
    return jkqtpstatAddPolyFit(plotter, firstX,lastX,firstY,lastY,P,std::back_inserter(pFit));
}


/*! \brief fits (in a least-squares sense) a polynomial \f$ f(x)=\sum\limits_{i=0}^Pp_ix^i \f$ of order P to a set of N data pairs \f$ (x_i,y_i) \f$ from a given JKQTPXYGraph \a datagraph
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam OutputItP output iterator for the polynomial coefficients
    \param datagraph graph representing the (x,y) datapairs to which to fit the regression line
    \param P degree of the polynomial (P>=N !!!)
    \param[out] firstRes Iterator (of type \a OutputItP ), which receives the (P+1)-entry vector with the polynomial coefficients \f$ p_i \f$

    Example:
    \code
        JKQTPXYLineGraph* graphD;
        plot1->addGraph(graphD=new JKQTPXYLineGraph(plot1));
        graphD->setXYColumns(colLinX, colLinY);
        std::vector<double> pFit;
        jkqtpstatAddPolyFit(graphD, 3,std::back_inserter(pFit));
    \endcode

    \image html datastore_regression_polynom.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatPolyFit()
*/
template <class OutputItP>
inline JKQTPXFunctionLineGraph* jkqtpstatAddPolyFit(JKQTPXYGraph *datagraph, size_t P, OutputItP firstRes) {
    JKQTBasePlotter* plt=datagraph->getParent();
    JKQTPDatastore* ds=plt->getDatastore();
    return jkqtpstatAddPolyFit(plt, ds->begin(datagraph->getXColumn()), ds->end(datagraph->getXColumn()), ds->begin(datagraph->getYColumn()), ds->end(datagraph->getYColumn()),P,firstRes);
}



/*! \brief fits (in a least-squares sense) a polynomial \f$ f(x)=\sum\limits_{i=0}^Pp_ix^i \f$ of order P to a set of N data pairs \f$ (x_i,y_i) \f$ from a given JKQTPXYGraph \a datagraph
    \ingroup jkqtptools_math_statistics_adaptors

    \param datagraph graph representing the (x,y) datapairs to which to fit the regression line
    \param P degree of the polynomial (P>=N !!!)

    Example:
    \code
        JKQTPXYLineGraph* graphD;
        plot1->addGraph(graphD=new JKQTPXYLineGraph(plot1));
        graphD->setXYColumns(colLinX, colLinY);
        jkqtpstatAddPolyFit(graphD, 3);
    \endcode

    \image html datastore_regression_polynom.png

    \see \ref JKQTPlotterBasicJKQTPDatastoreRegression, jkqtpstatPolyFit()
*/
JKQTPLOTTER_LIB_EXPORT JKQTPXFunctionLineGraph* jkqtpstatAddPolyFit(JKQTPXYGraph *datagraph, size_t P);










/*! \brief create a plot with y-direction error bars, calculated from average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam TGraph type of graph that should be added to the plot
    \tparam InputCatIt standard iterator type of \a inFirstCat_X and \a inLastCat_X
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_X iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for x-coordinates)
    \param inLastCat_X iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for x-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for y-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for y-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt, class TGraph>
inline TGraph* jkqtpstatAddYErrorGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_X, InputCatIt inLastCat_X, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    std::map<double, std::vector<double> > groupeddataBar;
    jkqtpstatGroupData(inFirstCat_X, inLastCat_X, inFirstValue_Y, inLastValue_Y, groupeddataBar, groupDefFunc);

    size_t colGroup=plotter->getDatastore()->addColumn(columnBaseName+", group");
    size_t colAverage=plotter->getDatastore()->addColumn(columnBaseName+", average");
    size_t colStdDev=plotter->getDatastore()->addColumn(columnBaseName+", stddev");

    for (auto it=groupeddataBar.begin(); it!=groupeddataBar.end(); ++it) {
        plotter->getDatastore()->appendToColumn(colGroup, it->first);
        plotter->getDatastore()->appendToColumn(colAverage, jkqtpstatAverage(it->second.begin(), it->second.end()));
        plotter->getDatastore()->appendToColumn(colStdDev, jkqtpstatStdDev(it->second.begin(), it->second.end()));
    }

    // 2.4. Finally the calculated groups are drawn
    TGraph* graph;
    plotter->addGraph(graph=new TGraph(plotter));
    graph->setXColumn(colGroup);
    graph->setYColumn(colAverage);
    graph->setYErrorColumn(static_cast<int>(colStdDev));

    return graph;
}


/*! \brief create a \c JKQTPXYLineErrorGraph with y-direction error bars, calculated from average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputCatIt standard iterator type of \a inFirstCat_X and \a inLastCat_X
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_X iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for x-coordinates)
    \param inLastCat_X iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for x-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for y-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for y-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), jkqtpstatAddYErrorGraph(), JKQTPXYLineErrorGraph, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline JKQTPXYLineErrorGraph* jkqtpstatAddYErrorLineGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_X, InputCatIt inLastCat_X, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    return jkqtpstatAddYErrorGraph<InputCatIt,InputValueIt,JKQTPXYLineErrorGraph>(plotter,inFirstCat_X,  inLastCat_X,  inFirstValue_Y,  inLastValue_Y,  groupDefFunc, columnBaseName);
}


/*! \brief create a \c JKQTPBarVerticalErrorGraph with y-direction error bars, calculated from average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputCatIt standard iterator type of \a inFirstCat_X and \a inLastCat_X
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_X iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for x-coordinates)
    \param inLastCat_X iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for x-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for y-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for y-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), jkqtpstatAddYErrorGraph(), JKQTPBarVerticalErrorGraph, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline JKQTPBarVerticalErrorGraph* jkqtpstatAddYErrorBarGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_X, InputCatIt inLastCat_X, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    return jkqtpstatAddYErrorGraph<InputCatIt,InputValueIt,JKQTPBarVerticalErrorGraph>(plotter,inFirstCat_X,  inLastCat_X,  inFirstValue_Y,  inLastValue_Y,  groupDefFunc, columnBaseName);
}


/*! \brief create a \c JKQTPImpulsesVerticalErrorGraph with y-direction error bars, calculated from average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputCatIt standard iterator type of \a inFirstCat_X and \a inLastCat_X
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_X iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for x-coordinates)
    \param inLastCat_X iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for x-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for y-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for y-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), jkqtpstatAddYErrorGraph(), JKQTPImpulsesVerticalErrorGraph, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline JKQTPImpulsesVerticalErrorGraph* jkqtpstatAddYErrorImpulsesGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_X, InputCatIt inLastCat_X, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    return jkqtpstatAddYErrorGraph<InputCatIt,InputValueIt,JKQTPImpulsesVerticalErrorGraph>(plotter,inFirstCat_X,  inLastCat_X,  inFirstValue_Y,  inLastValue_Y,  groupDefFunc, columnBaseName);
}


/*! \brief create a \c JKQTPXYParametrizedErrorScatterGraph with y-direction error bars, calculated from average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputCatIt standard iterator type of \a inFirstCat_X and \a inLastCat_X
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_X iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for x-coordinates)
    \param inLastCat_X iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for x-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for y-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for y-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), jkqtpstatAddYErrorGraph(), JKQTPXYParametrizedErrorScatterGraph, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline JKQTPXYParametrizedErrorScatterGraph* jkqtpstatAddYErrorParametrizedScatterGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_X, InputCatIt inLastCat_X, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    return jkqtpstatAddYErrorGraph<InputCatIt,InputValueIt,JKQTPXYParametrizedErrorScatterGraph>(plotter,inFirstCat_X,  inLastCat_X,  inFirstValue_Y,  inLastValue_Y,  groupDefFunc, columnBaseName);
}


/*! \brief create a \c JKQTPFilledCurveYErrorGraph with y-direction error bars, calculated from average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputCatIt standard iterator type of \a inFirstCat_X and \a inLastCat_X
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_X iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for x-coordinates)
    \param inLastCat_X iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for x-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for y-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for y-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), jkqtpstatAddYErrorGraph(), JKQTPFilledCurveYErrorGraph, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline JKQTPFilledCurveYErrorGraph* jkqtpstatAddYErrorFilledCurveGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_X, InputCatIt inLastCat_X, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    return jkqtpstatAddYErrorGraph<InputCatIt,InputValueIt,JKQTPFilledCurveYErrorGraph>(plotter,inFirstCat_X,  inLastCat_X,  inFirstValue_Y,  inLastValue_Y,  groupDefFunc, columnBaseName);
}







/*! \brief create a plot with x-direction error bars, calculated from average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam TGraph type of graph that should be added to the plot
    \tparam InputCatIt standard iterator type of \a inFirstCat_Y and \a inLastCat_Y
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_Y iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for y-coordinates)
    \param inLastCat_Y iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for y-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for x-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for x-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt, class TGraph>
inline TGraph* jkqtpstatAddXErrorGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_Y, InputCatIt inLastCat_Y, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    std::map<double, std::vector<double> > groupeddataBar;
    jkqtpstatGroupData(inFirstCat_Y, inLastCat_Y, inFirstValue_Y, inLastValue_Y, groupeddataBar, groupDefFunc);

    size_t colGroup=plotter->getDatastore()->addColumn(columnBaseName+", group");
    size_t colAverage=plotter->getDatastore()->addColumn(columnBaseName+", average");
    size_t colStdDev=plotter->getDatastore()->addColumn(columnBaseName+", stddev");

    for (auto it=groupeddataBar.begin(); it!=groupeddataBar.end(); ++it) {
        plotter->getDatastore()->appendToColumn(colGroup, it->first);
        plotter->getDatastore()->appendToColumn(colAverage, jkqtpstatAverage(it->second.begin(), it->second.end()));
        plotter->getDatastore()->appendToColumn(colStdDev, jkqtpstatStdDev(it->second.begin(), it->second.end()));
    }

    // 2.4. Finally the calculated groups are drawn
    TGraph* graph;
    plotter->addGraph(graph=new TGraph(plotter));
    graph->setXColumn(colAverage);
    graph->setYColumn(colGroup);
    graph->setXErrorColumn(static_cast<int>(colStdDev));

    return graph;
}


/*! \brief create a \c JKQTPXYLineErrorGraph with x-direction error bars, calculated from average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputCatIt standard iterator type of \a inFirstCat_Y and \a inLastCat_Y
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_Y iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for y-coordinates)
    \param inLastCat_Y iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for y-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for x-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for x-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), jkqtpstatAddXErrorGraph(), JKQTPXYLineErrorGraph, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline JKQTPXYLineErrorGraph* jkqtpstatAddXErrorLineGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_Y, InputCatIt inLastCat_Y, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    return jkqtpstatAddXErrorGraph<InputCatIt,InputValueIt,JKQTPXYLineErrorGraph>(plotter,inFirstCat_Y,  inLastCat_Y,  inFirstValue_Y,  inLastValue_Y,  groupDefFunc, columnBaseName);
}


/*! \brief create a \c JKQTPBarHorizontalErrorGraph with x-direction error bars, calculated from average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputCatIt standard iterator type of \a inFirstCat_Y and \a inLastCat_Y
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_Y iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for y-coordinates)
    \param inLastCat_Y iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for y-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for x-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for x-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), jkqtpstatAddXErrorGraph(), JKQTPBarHorizontalErrorGraph, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline JKQTPBarHorizontalErrorGraph* jkqtpstatAddXErrorBarGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_Y, InputCatIt inLastCat_Y, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    return jkqtpstatAddXErrorGraph<InputCatIt,InputValueIt,JKQTPBarHorizontalErrorGraph>(plotter,inFirstCat_Y,  inLastCat_Y,  inFirstValue_Y,  inLastValue_Y,  groupDefFunc, columnBaseName);
}


/*! \brief create a \c JKQTPImpulsesHorizontalErrorGraph with x-direction error bars, calculated from average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputCatIt standard iterator type of \a inFirstCat_Y and \a inLastCat_Y
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_Y iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for y-coordinates)
    \param inLastCat_Y iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for y-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for x-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for x-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), jkqtpstatAddXErrorGraph(), JKQTPImpulsesHorizontalErrorGraph, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline JKQTPImpulsesHorizontalErrorGraph* jkqtpstatAddXErrorImpulsesGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_Y, InputCatIt inLastCat_Y, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    return jkqtpstatAddXErrorGraph<InputCatIt,InputValueIt,JKQTPImpulsesHorizontalErrorGraph>(plotter,inFirstCat_Y,  inLastCat_Y,  inFirstValue_Y,  inLastValue_Y,  groupDefFunc, columnBaseName);
}


/*! \brief create a \c JKQTPXYParametrizedErrorScatterGraph with x-direction error bars, calculated from average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputCatIt standard iterator type of \a inFirstCat_Y and \a inLastCat_Y
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_Y iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for y-coordinates)
    \param inLastCat_Y iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for y-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for x-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for x-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), jkqtpstatAddXErrorGraph(), JKQTPXYParametrizedErrorScatterGraph, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline JKQTPXYParametrizedErrorScatterGraph* jkqtpstatAddXErrorParametrizedScatterGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_Y, InputCatIt inLastCat_Y, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    return jkqtpstatAddXErrorGraph<InputCatIt,InputValueIt,JKQTPXYParametrizedErrorScatterGraph>(plotter,inFirstCat_Y,  inLastCat_Y,  inFirstValue_Y,  inLastValue_Y,  groupDefFunc, columnBaseName);
}


/*! \brief create a JKQTPFilledCurveXErrorGraph with x-direction error bars, calculated from average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputCatIt standard iterator type of \a inFirstCat_Y and \a inLastCat_Y
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_Y iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for y-coordinates)
    \param inLastCat_Y iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for y-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for x-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for x-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), jkqtpstatAddXErrorGraph(), JKQTPFilledCurveXErrorGraph, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline JKQTPFilledCurveXErrorGraph* jkqtpstatAddXErrorFilledCurveGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_Y, InputCatIt inLastCat_Y, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    return jkqtpstatAddXErrorGraph<InputCatIt,InputValueIt,JKQTPFilledCurveXErrorGraph>(plotter,inFirstCat_Y,  inLastCat_Y,  inFirstValue_Y,  inLastValue_Y,  groupDefFunc, columnBaseName);
}








/*! \brief create a plot with x- and y-direction error bars, calculated from directional average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam TGraph type of graph that should be added to the plot
    \tparam InputCatIt standard iterator type of \a inFirstCat_X and \a inLastCat_X
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_X iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for x-coordinates)
    \param inLastCat_X iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for x-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for y-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for y-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt, class TGraph>
inline TGraph* jkqtpstatAddXYErrorGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_X, InputCatIt inLastCat_X, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    std::map<double, std::pair< std::vector<double>, std::vector<double> > > groupeddataBar;
    jkqtpstatGroupData(inFirstCat_X, inLastCat_X, inFirstValue_Y, inLastValue_Y, groupeddataBar, groupDefFunc);

    size_t colGroup=plotter->getDatastore()->addColumn(columnBaseName+", group");
    size_t colAverageX=plotter->getDatastore()->addColumn(columnBaseName+", category-average");
    size_t colStdDevX=plotter->getDatastore()->addColumn(columnBaseName+", category-stddev");
    size_t colAverageY=plotter->getDatastore()->addColumn(columnBaseName+", value-average");
    size_t colStdDevY=plotter->getDatastore()->addColumn(columnBaseName+", value-stddev");

    for (auto it=groupeddataBar.begin(); it!=groupeddataBar.end(); ++it) {
        plotter->getDatastore()->appendToColumn(colGroup, it->first);
        plotter->getDatastore()->appendToColumn(colAverageX, jkqtpstatAverage(it->second.first.begin(), it->second.first.end()));
        plotter->getDatastore()->appendToColumn(colStdDevX, jkqtpstatStdDev(it->second.first.begin(), it->second.first.end()));
        plotter->getDatastore()->appendToColumn(colAverageY, jkqtpstatAverage(it->second.second.begin(), it->second.second.end()));
        plotter->getDatastore()->appendToColumn(colStdDevY, jkqtpstatStdDev(it->second.second.begin(), it->second.second.end()));
    }

    // 2.4. Finally the calculated groups are drawn
    TGraph* graph;
    plotter->addGraph(graph=new TGraph(plotter));
    graph->setXColumn(colAverageX);
    graph->setYColumn(colAverageY);
    graph->setXErrorColumn(static_cast<int>(colStdDevX));
    graph->setYErrorColumn(static_cast<int>(colStdDevY));

    return graph;
}


/*! \brief create a \c JKQTPXYLineErrorGraph with y-direction error bars, calculated from average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputCatIt standard iterator type of \a inFirstCat_X and \a inLastCat_X
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_X iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for x-coordinates)
    \param inLastCat_X iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for x-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for y-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for y-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), jkqtpstatAddXYErrorGraph(), JKQTPXYLineErrorGraph, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline JKQTPXYLineErrorGraph* jkqtpstatAddXYErrorLineGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_X, InputCatIt inLastCat_X, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    return jkqtpstatAddXYErrorGraph<InputCatIt,InputValueIt,JKQTPXYLineErrorGraph>(plotter,inFirstCat_X,  inLastCat_X,  inFirstValue_Y,  inLastValue_Y,  groupDefFunc, columnBaseName);
}



/*! \brief create a \c JKQTPXYParametrizedErrorScatterGraph with y-direction error bars, calculated from average +/- stddev of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputCatIt standard iterator type of \a inFirstCat_X and \a inLastCat_X
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_X iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for x-coordinates)
    \param inLastCat_X iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for x-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for y-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for y-coordinates)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the graph showing \f$ c_{\text{out},j} \f$ and average +/- stddev for each group \f$ j \f$



    \see jkqtpstatGroupData(), jkqtpstatAddXYErrorGraph(), JKQTPXYParametrizedErrorScatterGraph, \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline JKQTPXYParametrizedErrorScatterGraph* jkqtpstatAddXYErrorParametrizedScatterGraph(JKQTBasePlotter* plotter, InputCatIt inFirstCat_X, InputCatIt inLastCat_X, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped data")) {
    return jkqtpstatAddXYErrorGraph<InputCatIt,InputValueIt,JKQTPXYParametrizedErrorScatterGraph>(plotter,inFirstCat_X,  inLastCat_X,  inFirstValue_Y,  inLastValue_Y,  groupDefFunc, columnBaseName);
}





/*! \brief create horizontal boxplots of type \c TGraph, from the 5-value-summary of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors
    \internal

    \tparam TGraph type of graph that should be added to the plot, has to offer the same interface as JKQTPBoxplotVerticalGraph or JKQTPBoxplotHorizontalGraph
    \tparam InputCatIt standard iterator type of \a inFirstCat_Y and \a inLastCat_Y
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_Y iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for y-coordinates)
    \param inLastCat_Y iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for y-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for x-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for x-coordinates)
    \param quantile1Spec specifies which quantile to calculate for \a qantile1 (range: 0..1)
    \param quantile2Spec specifies which quantile to calculate for \a qantile2 (range: 0..1)
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the boxplot graph



    \see jkqtpstatGroupData(), \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt, class TGraph>
inline TGraph* jkqtpstatAddBoxplots(JKQTBasePlotter* plotter, InputCatIt inFirstCat_Y, InputCatIt inLastCat_Y, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, double quantile1Spec=0.25, double quantile2Spec=0.75, double minimumQuantile=0, double maximumQuantile=1.0, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped boxplot data")) {
    std::map<double, std::vector<double> > groupeddataBar;
    jkqtpstatGroupData(inFirstCat_Y, inLastCat_Y, inFirstValue_Y, inLastValue_Y, groupeddataBar, groupDefFunc);

    size_t colGroup=plotter->getDatastore()->addColumn(columnBaseName+", group");
    size_t colMin=plotter->getDatastore()->addColumn(columnBaseName+", minimum");
    size_t colQ25=plotter->getDatastore()->addColumn(columnBaseName+", quartile25");
    size_t colMedian=plotter->getDatastore()->addColumn(columnBaseName+", median");
    size_t colIQRSig=plotter->getDatastore()->addColumn(columnBaseName+", MedianSignificance");
    size_t colAverage=plotter->getDatastore()->addColumn(columnBaseName+", average");
    size_t colQ75=plotter->getDatastore()->addColumn(columnBaseName+", quartile75");
    size_t colMax=plotter->getDatastore()->addColumn(columnBaseName+", maximum");

    for (auto it=groupeddataBar.begin(); it!=groupeddataBar.end(); ++it) {

        auto stat5=jkqtpstat5NumberStatistics(it->second.begin(), it->second.end(), quantile1Spec,  quantile2Spec,  minimumQuantile,  maximumQuantile);

        plotter->getDatastore()->appendToColumn(colGroup, it->first);
        plotter->getDatastore()->appendToColumn(colAverage, jkqtpstatAverage(it->second.begin(), it->second.end()));
        plotter->getDatastore()->appendToColumn(colMin, stat5.minimum);
        plotter->getDatastore()->appendToColumn(colQ25, stat5.quantile1);
        plotter->getDatastore()->appendToColumn(colMedian, stat5.median);
        plotter->getDatastore()->appendToColumn(colQ75, stat5.quantile2);
        plotter->getDatastore()->appendToColumn(colMax, stat5.maximum);
        plotter->getDatastore()->appendToColumn(colIQRSig, stat5.IQRSignificanceEstimate());
    }

    // 2.4. Finally the calculated groups are drawn
    TGraph* graph;
    plotter->addGraph(graph=new TGraph(plotter));
    graph->setPositionColumn(colGroup);
    graph->setMinColumn(colMin);
    graph->setMaxColumn(colMax);
    graph->setMedianColumn(colMedian);
    graph->setMeanColumn(colAverage);
    graph->setPercentile25Column(colQ25);
    graph->setPercentile75Column(colQ75);
    graph->setMedianConfidenceColumn(colIQRSig);

    return graph;
}

/*! \brief create vertical boxplots of type \c JKQTPBoxplotVerticalGraph, from the 5-value-summary of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors
    \internal

    \tparam InputCatIt standard iterator type of \a inFirstCat_Y and \a inLastCat_Y
    \tparam InputValueIt standard iterator type of \a inFirstValue_X and \a inLastValue_X
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_Y iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for y-coordinates)
    \param inLastCat_Y iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for y-coordinates)
    \param inFirstValue_X iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for x-coordinates)
    \param inLastValue_X iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for x-coordinates)
    \param quantile1Spec specifies which quantile to calculate for \a qantile1 (range: 0..1)
    \param quantile2Spec specifies which quantile to calculate for \a qantile2 (range: 0..1)
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the boxplot graph



    \see jkqtpstatGroupData(), \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline JKQTPBoxplotVerticalGraph* jkqtpstatVAddBoxplots(JKQTBasePlotter* plotter, InputCatIt inFirstCat_Y, InputCatIt inLastCat_Y, InputValueIt inFirstValue_X, InputValueIt inLastValue_X, double quantile1Spec=0.25, double quantile2Spec=0.75, double minimumQuantile=0, double maximumQuantile=1.0, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped boxplot data")) {

    return jkqtpstatAddBoxplots<InputCatIt,InputValueIt,JKQTPBoxplotVerticalGraph>(plotter,  inFirstCat_Y,  inLastCat_Y,  inFirstValue_X,  inLastValue_X,  quantile1Spec,  quantile2Spec,  minimumQuantile,  maximumQuantile, groupDefFunc, columnBaseName);
}

/*! \brief create horizontal boxplots of type \c JKQTPBoxplotHorizontalGraph, from the 5-value-summary of groups in the input data
    \ingroup jkqtptools_math_statistics_adaptors
    \internal

    \tparam InputCatIt standard iterator type of \a inFirstCat_X and \a inLastCat_X
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_X iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for y-coordinates)
    \param inLastCat_X iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for y-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for x-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for x-coordinates)
    \param quantile1Spec specifies which quantile to calculate for \a qantile1 (range: 0..1)
    \param quantile2Spec specifies which quantile to calculate for \a qantile2 (range: 0..1)
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the boxplot graph



    \see jkqtpstatGroupData(), \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline JKQTPBoxplotHorizontalGraph* jkqtpstatVAddBoxplots(JKQTBasePlotter* plotter, InputCatIt inFirstCat_X, InputCatIt inLastCat_X, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, double quantile1Spec=0.25, double quantile2Spec=0.75, double minimumQuantile=0, double maximumQuantile=1.0, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped boxplot data")) {

    return jkqtpstatAddBoxplots<InputCatIt,InputValueIt,JKQTPBoxplotHorizontalGraph>(plotter,  inFirstCat_X,  inLastCat_X,  inFirstValue_Y,  inLastValue_Y,  quantile1Spec,  quantile2Spec,  minimumQuantile,  maximumQuantile, groupDefFunc, columnBaseName);
}

/*! \brief create vertical boxplots of type \c JKQTPBoxplotVerticalGraph, from the 5-value-summary of groups in the input data, also adds a graph showing the outliers
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputCatIt standard iterator type of \a inFirstCat_X and \a inLastCat_X
    \tparam InputValueIt standard iterator type of \a inFirstValue_Y and \a inLastValue_Y
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_X iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for y-coordinates)
    \param inLastCat_X iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for y-coordinates)
    \param inFirstValue_Y iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for x-coordinates)
    \param inLastValue_Y iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for x-coordinates)
    \param quantile1Spec specifies which quantile to calculate for \a qantile1 (range: 0..1)
    \param quantile2Spec specifies which quantile to calculate for \a qantile2 (range: 0..1)
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the boxplot graph (return.first) and the outliers graph (return.second)



    \see jkqtpstatGroupData(), \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline std::pair<JKQTPBoxplotVerticalGraph*, JKQTPXYLineGraph*> jkqtpstatAddVBoxplotsAndOutliers(JKQTBasePlotter* plotter, InputCatIt inFirstCat_X, InputCatIt inLastCat_X, InputValueIt inFirstValue_Y, InputValueIt inLastValue_Y, double quantile1Spec=0.25, double quantile2Spec=0.75, double minimumQuantile=0.03, double maximumQuantile=0.97, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped boxplot data")) {
    std::map<double, std::vector<double> > groupeddataBar;
    jkqtpstatGroupData(inFirstCat_X, inLastCat_X, inFirstValue_Y, inLastValue_Y, groupeddataBar, groupDefFunc);

    size_t colGroup=plotter->getDatastore()->addColumn(columnBaseName+", group");
    size_t colMin=plotter->getDatastore()->addColumn(columnBaseName+", minimum");
    size_t colQ25=plotter->getDatastore()->addColumn(columnBaseName+", quartile25");
    size_t colMedian=plotter->getDatastore()->addColumn(columnBaseName+", median");
    size_t colIQRSig=plotter->getDatastore()->addColumn(columnBaseName+", MedianSignificance");
    size_t colAverage=plotter->getDatastore()->addColumn(columnBaseName+", average");
    size_t colQ75=plotter->getDatastore()->addColumn(columnBaseName+", quartile75");
    size_t colMax=plotter->getDatastore()->addColumn(columnBaseName+", maximum");
    size_t colOutlierG=plotter->getDatastore()->addColumn(columnBaseName+", outlier-group");
    size_t colOutlierV=plotter->getDatastore()->addColumn(columnBaseName+", outlier-value");

    for (auto it=groupeddataBar.begin(); it!=groupeddataBar.end(); ++it) {

        auto stat5=jkqtpstat5NumberStatistics(it->second.begin(), it->second.end(), quantile1Spec,  quantile2Spec,  minimumQuantile,  maximumQuantile);

        plotter->getDatastore()->appendToColumn(colGroup, it->first);
        plotter->getDatastore()->appendToColumn(colAverage, jkqtpstatAverage(it->second.begin(), it->second.end()));
        plotter->getDatastore()->appendToColumn(colMin, stat5.minimum);
        plotter->getDatastore()->appendToColumn(colQ25, stat5.quantile1);
        plotter->getDatastore()->appendToColumn(colMedian, stat5.median);
        plotter->getDatastore()->appendToColumn(colQ75, stat5.quantile2);
        plotter->getDatastore()->appendToColumn(colMax, stat5.maximum);
        plotter->getDatastore()->appendToColumn(colIQRSig, stat5.IQRSignificanceEstimate());
        std::fill_n(plotter->getDatastore()->backInserter(colOutlierG), stat5.outliers.size(), jkqtp_todouble(it->first));
        plotter->getDatastore()->appendToColumn(colOutlierV, stat5.outliers.begin(), stat5.outliers.end());
    }

    JKQTPBoxplotVerticalGraph* graph;
    plotter->addGraph(graph=new JKQTPBoxplotVerticalGraph(plotter));
    graph->setPositionColumn(colGroup);
    graph->setMinColumn(colMin);
    graph->setMaxColumn(colMax);
    graph->setMedianColumn(colMedian);
    graph->setMeanColumn(colAverage);
    graph->setPercentile25Column(colQ25);
    graph->setPercentile75Column(colQ75);
    graph->setMedianConfidenceColumn(colIQRSig);

    JKQTPXYLineGraph* graphOL;
    plotter->addGraph(graphOL=new JKQTPXYLineGraph(plotter));
    graphOL->setXColumn(colOutlierG);
    graphOL->setYColumn(colOutlierV);
    graphOL->setDrawLine(false);
    graphOL->setSymbolSize(graphOL->getSymbolSize()/2.0);
    graphOL->setColor(graph->getLineColor());
    return std::pair<JKQTPBoxplotVerticalGraph*, JKQTPXYLineGraph*>(graph, graphOL);
}


/*! \brief create vertical boxplots of type \c JKQTPBoxplotHorizontalGraph, from the 5-value-summary of groups in the input data, also adds a graph showing the outliers
    \ingroup jkqtptools_math_statistics_adaptors

    \tparam InputCatIt standard iterator type of \a inFirstCat_Y and \a inLastCat_Y
    \tparam InputValueIt standard iterator type of \a inFirstValue_X and \a inLastValue_X
    \param plotter the plotter to which to add the resulting graph
    \param inFirstCat_Y iterator pointing to the first item in the category dataset to use \f$ c_1 \f$ (used for y-coordinates)
    \param inLastCat_Y iterator pointing behind the last item in the category dataset to use \f$ c_N \f$ (used for y-coordinates)
    \param inFirstValue_X iterator pointing to the first item in the category dataset to use \f$ v_1 \f$ (used for x-coordinates)
    \param inLastValue_X iterator pointing behind the last item in the category dataset to use \f$ v_N \f$ (used for x-coordinates)
    \param quantile1Spec specifies which quantile to calculate for \a qantile1 (range: 0..1)
    \param quantile2Spec specifies which quantile to calculate for \a qantile2 (range: 0..1)
    \param minimumQuantile specifies a quantile for the return value minimum (default is 0 for the real minimum, but you could e.g. use 0.05 for the 5% quantile!)
    \param maximumQuantile specifies a quantile for the return value maximum (default is 1 for the real maximum, but you could e.g. use 0.95 for the 95% quantile!)
    \param groupDefFunc assigns a group \f$ c_{\text{out},j} \f$ to each category value \f$ c_i \f$ .
    \param columnBaseName string component used to build the names of the columns generated by this function
    \return the boxplot graph (return.first) and the outliers graph (return.second)



    \see jkqtpstatGroupData(), \ref JKQTPlotterBasicJKQTPDatastoreStatisticsGroupedStat
*/
template <class InputCatIt, class InputValueIt>
inline std::pair<JKQTPBoxplotHorizontalGraph*, JKQTPXYLineGraph*> jkqtpstatAddHBoxplotsAndOutliers(JKQTBasePlotter* plotter, InputCatIt inFirstCat_Y, InputCatIt inLastCat_Y, InputValueIt inFirstValue_X, InputValueIt inLastValue_X, double quantile1Spec=0.25, double quantile2Spec=0.75, double minimumQuantile=0.03, double maximumQuantile=0.97, JKQTPStatGroupDefinitionFunctor1D groupDefFunc=&jkqtpstatGroupingIdentity1D, const QString& columnBaseName=QString("grouped boxplot data")) {
    std::map<double, std::vector<double> > groupeddataBar;
    jkqtpstatGroupData(inFirstCat_Y, inLastCat_Y, inFirstValue_X, inLastValue_X, groupeddataBar, groupDefFunc);

    size_t colGroup=plotter->getDatastore()->addColumn(columnBaseName+", group");
    size_t colMin=plotter->getDatastore()->addColumn(columnBaseName+", minimum");
    size_t colQ25=plotter->getDatastore()->addColumn(columnBaseName+", quartile25");
    size_t colMedian=plotter->getDatastore()->addColumn(columnBaseName+", median");
    size_t colIQRSig=plotter->getDatastore()->addColumn(columnBaseName+", MedianSignificance");
    size_t colAverage=plotter->getDatastore()->addColumn(columnBaseName+", average");
    size_t colQ75=plotter->getDatastore()->addColumn(columnBaseName+", quartile75");
    size_t colMax=plotter->getDatastore()->addColumn(columnBaseName+", maximum");
    size_t colOutlierG=plotter->getDatastore()->addColumn(columnBaseName+", outlier-group");
    size_t colOutlierV=plotter->getDatastore()->addColumn(columnBaseName+", outlier-value");

    for (auto it=groupeddataBar.begin(); it!=groupeddataBar.end(); ++it) {

        auto stat5=jkqtpstat5NumberStatistics(it->second.begin(), it->second.end(), quantile1Spec,  quantile2Spec,  minimumQuantile,  maximumQuantile);

        plotter->getDatastore()->appendToColumn(colGroup, it->first);
        plotter->getDatastore()->appendToColumn(colAverage, jkqtpstatAverage(it->second.begin(), it->second.end()));
        plotter->getDatastore()->appendToColumn(colMin, stat5.minimum);
        plotter->getDatastore()->appendToColumn(colQ25, stat5.quantile1);
        plotter->getDatastore()->appendToColumn(colMedian, stat5.median);
        plotter->getDatastore()->appendToColumn(colQ75, stat5.quantile2);
        plotter->getDatastore()->appendToColumn(colMax, stat5.maximum);
        plotter->getDatastore()->appendToColumn(colIQRSig, stat5.IQRSignificanceEstimate());
        std::fill_n(plotter->getDatastore()->backInserter(colOutlierG), stat5.outliers.size(), jkqtp_todouble(it->first));
        plotter->getDatastore()->appendToColumn(colOutlierV, stat5.outliers.begin(), stat5.outliers.end());
    }

    JKQTPBoxplotHorizontalGraph* graph;
    plotter->addGraph(graph=new JKQTPBoxplotHorizontalGraph(plotter));
    graph->setPositionColumn(colGroup);
    graph->setMinColumn(colMin);
    graph->setMaxColumn(colMax);
    graph->setMedianColumn(colMedian);
    graph->setMeanColumn(colAverage);
    graph->setPercentile25Column(colQ25);
    graph->setPercentile75Column(colQ75);
    graph->setMedianConfidenceColumn(colIQRSig);

    JKQTPXYLineGraph* graphOL;
    plotter->addGraph(graphOL=new JKQTPXYLineGraph(plotter));
    graphOL->setYColumn(colOutlierG);
    graphOL->setXColumn(colOutlierV);
    graphOL->setDrawLine(false);
    graphOL->setSymbolSize(graphOL->getSymbolSize()/2.0);
    graphOL->setColor(graph->getLineColor());
    return std::pair<JKQTPBoxplotHorizontalGraph*, JKQTPXYLineGraph*>(graph, graphOL);
}






#endif // JKQTPGRAPHSSTATISTICSADAPTORS_H_INCLUDED
