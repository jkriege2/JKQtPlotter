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


#include "jkqtcommon/jkqtp_imexport.h"
#include "jkqtcommon/jkqtpstatisticstools.h"
#include "jkqtplotter/jkqtpgraphsboxplot.h"
#include "jkqtplotter/jkqtpgraphsscatter.h"
#include "jkqtplotter/jkqtpgraphssinglecolumnsymbols.h"
#include "jkqtplotter/jkqtpgraphsbarchart.h"

#ifndef JKQTPGRAPHSSTATISTICSADAPTORS_H_INCLUDED
#define JKQTPGRAPHSSTATISTICSADAPTORS_H_INCLUDED

/*! \brief add a JKQTPBoxplotHorizontalElement to the given plotter, where the boxplot values are calculated from the data range \a first ... \a last
    \ingroup jkqtptools_math_statistics
    \ingroup jkqtplotter_statgraphs

    \tparam InputIt standard iterator type of \a first and \a last.
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

    \see \ref JKQTPlottersimpletest_datastore_statistics, jkqtpstat5NumberStatistics()
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
    \ingroup jkqtptools_math_statistics
    \ingroup jkqtplotter_statgraphs

    \tparam InputIt standard iterator type of \a first and \a last.
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

    \see \ref JKQTPlottersimpletest_datastore_statistics, jkqtpstat5NumberStatistics()
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
    \ingroup jkqtptools_math_statistics
    \ingroup jkqtplotter_statgraphs

    \tparam InputIt standard iterator type of \a first and \a last.
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

    \see \ref JKQTPlottersimpletest_datastore_statistics, jkqtpstat5NumberStatistics()
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
    \ingroup jkqtptools_math_statistics
    \ingroup jkqtplotter_statgraphs

    \tparam InputIt standard iterator type of \a first and \a last.
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

    \see \ref JKQTPlottersimpletest_datastore_statistics, jkqtpstat5NumberStatistics()
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



/*! \brief calculate an autoranged histogram and add a JKQTPBarVerticalGraph to the given plotter, where the histogram is calculated from the data range \a first ... \a last, bins defined by their number
    \ingroup jkqtptools_math_statistics_1dhist
    \ingroup jkqtplotter_statgraphs

    \tparam InputIt standard iterator type of \a first and \a last.
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

    \see \ref JKQTPlottersimpletest_datastore_statistics, jkqtpstatHistogram1DAutoranged(), JKQTPBarVerticalGraph
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
    \ingroup jkqtptools_math_statistics_1dhist
    \ingroup jkqtplotter_statgraphs

    \tparam InputIt standard iterator type of \a first and \a last.
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

    \see \ref JKQTPlottersimpletest_datastore_statistics, jkqtpstatHistogram1DAutoranged(), JKQTPBarVerticalGraph
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
    \ingroup jkqtptools_math_statistics_1dhist
    \ingroup jkqtplotter_statgraphs

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam BinsInputIt standard iterator type of \a binsFirst and \a binsLast.
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

    \see \ref JKQTPlottersimpletest_datastore_statistics, jkqtpstatHistogram1D(), JKQTPBarVerticalGraph
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
    \ingroup jkqtptools_math_statistics_1dhist
    \ingroup jkqtplotter_statgraphs

    \tparam InputIt standard iterator type of \a first and \a last.
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

    \see \ref JKQTPlottersimpletest_datastore_statistics, jkqtpstatHistogram1DAutoranged(), JKQTPBarHorizontalGraph
*/
template <class InputIt>
inline JKQTPBarHorizontalGraph* jkqtpstatAddVHistogram1DAutoranged(JKQTBasePlotter* plotter, InputIt first, InputIt last, int bins=11, bool normalized=true, bool cummulative=false, const QString& histogramcolumnBaseName=QString("histogram")) {
    size_t histcolX=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", values");
    jkqtpstatHistogram1DAutoranged(first, last, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), bins, normalized, cummulative, JKQTPStatHistogramBinXMode::XIsMid);
    JKQTPBarHorizontalGraph* resO=new JKQTPBarHorizontalGraph(plotter);
    resO->setXColumn(histcolX);
    resO->setYColumn(histcolY);
    resO->setTitle(histogramcolumnBaseName);
    plotter->addGraph(resO);
    return resO;
}



/*! \brief calculate an autoranged histogram and add a JKQTPBarHorizontalGraph to the given plotter, where the histogram is calculated from the data range \a first ... \a last, bins defined by their width
    \ingroup jkqtptools_math_statistics_1dhist
    \ingroup jkqtplotter_statgraphs

    \tparam InputIt standard iterator type of \a first and \a last.
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

    \see \ref JKQTPlottersimpletest_datastore_statistics, jkqtpstatHistogram1DAutoranged(), JKQTPBarHorizontalGraph
*/
template <class InputIt>
inline JKQTPBarHorizontalGraph* jkqtpstatAddVHistogram1DAutoranged(JKQTBasePlotter* plotter, InputIt first, InputIt last, double binWidth, bool normalized=true, bool cummulative=false, const QString& histogramcolumnBaseName=QString("histogram")) {
    size_t histcolX=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", values");
    jkqtpstatHistogram1DAutoranged(first, last, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), binWidth, normalized, cummulative, JKQTPStatHistogramBinXMode::XIsMid);
    JKQTPBarHorizontalGraph* resO=new JKQTPBarHorizontalGraph(plotter);
    resO->setXColumn(histcolX);
    resO->setYColumn(histcolY);
    resO->setTitle(histogramcolumnBaseName);
    plotter->addGraph(resO);
    return resO;
}


/*! \brief calculate an autoranged histogram and add a JKQTPBarHorizontalGraph to the given plotter, where the histogram is calculated from the data range \a first ... \a last, bins defined by their width
    \ingroup jkqtptools_math_statistics_1dhist
    \ingroup jkqtplotter_statgraphs

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam BinsInputIt standard iterator type of \a binsFirst and \a binsLast.
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

    \see \ref JKQTPlottersimpletest_datastore_statistics, jkqtpstatHistogram1D(), JKQTPBarHorizontalGraph
*/
template <class InputIt, class BinsInputIt>
inline JKQTPBarHorizontalGraph* jkqtpstatAddVHistogram1D(JKQTBasePlotter* plotter, InputIt first, InputIt last, BinsInputIt binsFirst, BinsInputIt binsLast, bool normalized=true, bool cummulative=false, const QString& histogramcolumnBaseName=QString("histogram")) {
    size_t histcolX=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", bins");
    size_t histcolY=plotter->getDatastore()->addColumn(histogramcolumnBaseName+", values");
    jkqtpstatHistogram1D(first, last, binsFirst, binsLast, plotter->getDatastore()->backInserter(histcolX), plotter->getDatastore()->backInserter(histcolY), normalized, cummulative, JKQTPStatHistogramBinXMode::XIsMid);
    JKQTPBarHorizontalGraph* resO=new JKQTPBarHorizontalGraph(plotter);
    resO->setXColumn(histcolX);
    resO->setYColumn(histcolY);
    resO->setTitle(histogramcolumnBaseName);
    plotter->addGraph(resO);
    return resO;
}












/*! \brief calculate an autoranged KDE and add a JKQTPXYLineGraph to the given plotter, where the KDE is calculated from the data range \a first ... \a last, bins defined by their number
    \ingroup jkqtptools_math_statistics_1dhist
    \ingroup jkqtplotter_statgraphs

    \tparam InputIt standard iterator type of \a first and \a last.
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

    \see \ref JKQTPlottersimpletest_datastore_statistics, jkqtpstatKDE1DAutoranged(), JKQTPXYLineGraph
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
    \ingroup jkqtptools_math_statistics_1dhist
    \ingroup jkqtplotter_statgraphs

    \tparam InputIt standard iterator type of \a first and \a last.
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

    \see \ref JKQTPlottersimpletest_datastore_statistics, jkqtpstatKDE1DAutoranged(), JKQTPXYLineGraph
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
    \ingroup jkqtptools_math_statistics_1dhist
    \ingroup jkqtplotter_statgraphs

    \tparam InputIt standard iterator type of \a first and \a last.
    \tparam BinsInputIt standard iterator type of \a binsFirst and \a binsLast.
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

    \see \ref JKQTPlottersimpletest_datastore_statistics, jkqtpstatKDE1D(), JKQTPXYLineGraph
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
    \ingroup jkqtptools_math_statistics_1dhist
    \ingroup jkqtplotter_statgraphs

    \tparam InputIt standard iterator type of \a first and \a last.
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

    \see \ref JKQTPlottersimpletest_datastore_statistics, jkqtpstatKDE1D(), JKQTPXYLineGraph
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





#endif // JKQTPGRAPHSSTATISTICSADAPTORS_H_INCLUDED
