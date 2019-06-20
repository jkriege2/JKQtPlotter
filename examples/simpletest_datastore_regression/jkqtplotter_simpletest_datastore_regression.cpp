/** \example jkqtplotter_simpletest_datastore_regression.cpp
 * Explains how to use the internal statistics library (see \ref jkqtptools_statistics ) together with JKQTPDatastore to perform different types of regression and polynomial fitting.
 *
 * \ref JKQTPlotterBasicJKQTPDatastoreRegression
 */
 
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtppeakstream.h"
#include "jkqtplotter/graphs/jkqtpboxplot.h"
#include "jkqtplotter/graphs/jkqtpstatisticsadaptors.h"
#include "jkqtplotter/graphs/jkqtpevaluatedfunction.h"
#include "jkqtcommon/jkqtpstatisticstools.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include <random>
#include <cmath>


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);


    // 1. create a window with several plotters and get a pointer to the internal datastores (for convenience)
    QWidget mainWidget;
    QGridLayout* lay;
    mainWidget.setLayout(lay=new QGridLayout);
    JKQTPlotter* plot1=new JKQTPlotter(&mainWidget);
    plot1->getPlotter()->setPlotLabel("Simple Linear Regression");
    JKQTPDatastore* datastore1=plot1->getDatastore();
    lay->addWidget(plot1,0,0);
    JKQTPlotter *plot2=new JKQTPlotter(datastore1, &mainWidget);
    plot2->getPlotter()->setPlotLabel("Weighted Linear Regression");
    lay->addWidget(plot2,1,0);
    JKQTPlotter* plot3=new JKQTPlotter(datastore1, &mainWidget);
    plot3->getPlotter()->setPlotLabel("Robust Linear Regression");
    lay->addWidget(plot3,0,1);
    JKQTPlotter *plot6=new JKQTPlotter(datastore1, &mainWidget);
    plot6->getPlotter()->setPlotLabel("Polynomial Fitting");
    lay->addWidget(plot6,1,1);
    JKQTPlotter* plot4=new JKQTPlotter(datastore1, &mainWidget);
    plot4->getPlotter()->setPlotLabel("Exponential Regression");
    lay->addWidget(plot4,0,2);
    JKQTPlotter* plot5=new JKQTPlotter(datastore1, &mainWidget);
    plot5->getPlotter()->setPlotLabel("Power-Law Regression");
    lay->addWidget(plot5,1,2);


    // 2.1. To demonstrate linear regression, we create a dataset with a linear dependence between two
    //      columns and added gaussian noise
    std::random_device rd; // random number generators:
    std::mt19937 gen{rd()};
    std::normal_distribution<> d1{0,1};
    double a0=-5;
    double b0=2;
    size_t colLinX=datastore1->addColumn("lin data, x");
    size_t colLinY=datastore1->addColumn("lin data, y");
    for (double x=-5; x<=10; x++) {
        datastore1->appendToColumn(colLinX, x);
        datastore1->appendToColumn(colLinY, a0+b0*x+d1(gen));
    }
    //     we visualize this data with a simple scatter graph:
    JKQTPXYLineGraph* graphD;
    plot1->addGraph(graphD=new JKQTPXYLineGraph(plot1));
    graphD->setXYColumns(colLinX, colLinY);
    graphD->setDrawLine(false);
    graphD->setTitle(QString("data $f(x)=%1+%2\\cdot x+\\mathcal{N}(0,1)$").arg(jkqtp_floattolatexqstr(a0,1)).arg(jkqtp_floattolatexqstr(b0,1)));
    // 2.2. Now we calculate the regression line and add a plot to the graph:
    /*double coeffA=0, coeffB=0;
    jkqtpstatLinearRegression(datastore1->begin(colLinX), datastore1->end(colLinX), datastore1->begin(colLinY), datastore1->end(colLinY), coeffA, coeffB);
    JKQTPXFunctionLineGraph *graphRegLine=new JKQTPXFunctionLineGraph(plot1);
    graphRegLine->setSpecialFunction(JKQTPXFunctionLineGraph::SpecialFunction::Line);
    graphRegLine->setParamsV(coeffA, coeffB);
    graphRegLine->setTitle(QString("regression: $f(x) = %1 + %2 \\cdot x$").arg(jkqtp_floattolatexqstr(coeffA)).arg(jkqtp_floattolatexqstr(coeffB)));
    plot1->addGraph(graphRegLine);*/
    //      this code can also be written with one function call, using the "adaptor" jkqtpstatAddLinearRegression():
    //jkqtpstatAddLinearRegression(plot1->getPlotter(), datastore1->begin(colLinX), datastore1->end(colLinX), datastore1->begin(colLinY), datastore1->end(colLinY));
    // or even shorter:
    jkqtpstatAddLinearRegression(graphD);



    // 3.1. We extend the example above by
    //std::random_device rd; // random number generators:
    //std::mt19937 gen{rd()};
    std::uniform_real_distribution<> de{0.5,1.5};
    std::uniform_int_distribution<> ddecide{0,4};
    //double a0=-5;
    //double b0=2;
    size_t colWLinX=datastore1->addColumn("wlin data, x");
    size_t colWLinY=datastore1->addColumn("wlin data, y");
    size_t colWLinE=datastore1->addColumn("wlin data, errors");
    for (double x=-5; x<=10; x++) {
        double factor=1;
        if (ddecide(gen)==4) {
            factor=4;
        }
        const double err=de(gen)*factor;
        datastore1->appendToColumn(colWLinX, x);
        datastore1->appendToColumn(colWLinY, a0+b0*x+err);
        datastore1->appendToColumn(colWLinE, err);
    }
    //     we visualize this data with a simple scatter graph:
    JKQTPXYLineErrorGraph* graphE;
    plot2->addGraph(graphE=new JKQTPXYLineErrorGraph(plot2));
    graphE->setXYColumns(colWLinX, colWLinY);
    graphE->setYErrorColumn(static_cast<int>(colWLinE));
    graphE->setDrawLine(false);
    graphE->setTitle(QString("data $f(x)=%1+%2\\cdot x+\\mbox{Noise}$").arg(jkqtp_floattolatexqstr(a0,1)).arg(jkqtp_floattolatexqstr(b0,1)));
    // 2.2. Now we calculate the regression line and add a plot to the graph:
    /*double coeffA=0, coeffB=0;
    jkqtpstatLinearWeightedRegression(datastore1->begin(colWLinX), datastore1->end(colWLinX),
                                      datastore1->begin(colWLinY), datastore1->end(colWLinY),
                                      datastore1->begin(colWLinE), datastore1->end(colWLinE),
                                      coeffA, coeffB, false, false,
                                      &jkqtp_inversePropSaveDefault<double>);
    //      note that in addition to the three data-columns we also provided a C++ functor
    //      jkqtp_inversePropSaveDefault(), which calculates 1/error. This is done, because the function
    //      jkqtpstatLinearWeightedRegression() uses the data from the range datastore1->begin(colWLinE) ... datastore1->end(colWLinE)
    //      directly as weights, but we calculated errors, which are inversely proportional to the
    //      weight of each data point when solving the least squares problem, as data points with
    //      larger errors should be weighted less than thos with smaller errors
    //
    //      Now we can plot the resulting linear function:
    JKQTPXFunctionLineGraph *graphRegLine=new JKQTPXFunctionLineGraph(plot2);
    graphRegLine->setSpecialFunction(JKQTPXFunctionLineGraph::SpecialFunction::Line);
    graphRegLine->setParamsV(coeffA, coeffB);
    graphRegLine->setTitle(QString("weighted regression: $f(x) = %1 + %2 \\cdot x$").arg(jkqtp_floattolatexqstr(coeffA)).arg(jkqtp_floattolatexqstr(coeffB)));
    plot2->addGraph(graphRegLine);*/
    //      this code can also be written with one function call, using the "adaptor" jkqtpstatAddLinearRegression():
    //jkqtpstatAddLinearWeightedRegression(plot2->getPlotter(),
    //                                        datastore1->begin(colLinX), datastore1->end(colLinX),
    //                                        datastore1->begin(colLinY), datastore1->end(colLinY),
    //                                        datastore1->begin(colWLinE), datastore1->end(colWLinE),
    //                                        &coeffA, &coeffB, false, false,
    //                                        &jkqtp_inversePropSaveDefault<double>);

    // or even shorter:
    jkqtpstatAddLinearWeightedRegression(graphE);
    //      to demonstrate the effect of the weighting, we also add a simple linear regression that
    //      does not take into account the errors:
    jkqtpstatAddLinearRegression(graphE);



    // 4.1. To demonstrate IRLS linear regression, we create a dataset with a linear dependence between two
    //      columns and added gaussian noise and some outliers
    //std::random_device rd; // random number generators:
    //std::mt19937 gen{rd()};
    //std::normal_distribution<> d1{0,1};
    //double a0=-5;
    //double b0=2;
    size_t colRLinX=datastore1->addColumn("lin data, x");
    size_t colRLinY=datastore1->addColumn("lin data, y");
    for (double x=-5; x<=10; x++) {
        datastore1->appendToColumn(colRLinX, x);
        if (jkqtp_approximatelyEqual(x, -5)||jkqtp_approximatelyEqual(x, -3)) datastore1->appendToColumn(colRLinY, a0+b0*x+d1(gen)+12);
        else datastore1->appendToColumn(colRLinY, a0+b0*x+d1(gen));
    }
    //     we visualize this data with a simple scatter graph:
    //JKQTPXYLineGraph* graphD;
    plot3->addGraph(graphD=new JKQTPXYLineGraph(plot3));
    graphD->setXYColumns(colRLinX, colRLinY);
    graphD->setDrawLine(false);
    graphD->setTitle(QString("data $f(x)=%1+%2\\cdot x+\\mathcal{N}(0,1)$").arg(jkqtp_floattolatexqstr(a0,1)).arg(jkqtp_floattolatexqstr(b0,1)));
    // 4.2. Now we calculate the regression line and add a plot to the graph:
    double coeffA=0, coeffB=0;
    jkqtpstatRobustIRLSLinearRegression(datastore1->begin(colRLinX), datastore1->end(colRLinX), datastore1->begin(colRLinY), datastore1->end(colRLinY), coeffA, coeffB);
    JKQTPXFunctionLineGraph *graphRegLine=new JKQTPXFunctionLineGraph(plot3);
    graphRegLine->setSpecialFunction(JKQTPXFunctionLineGraph::SpecialFunction::Line);
    graphRegLine->setParamsV(coeffA, coeffB);
    graphRegLine->setTitle(QString("robust regression: $f(x) = %1 + %2 \\cdot x$, $p=1.1$").arg(jkqtp_floattolatexqstr(coeffA)).arg(jkqtp_floattolatexqstr(coeffB)));
    plot3->addGraph(graphRegLine);
    //      this code can also be written with one function call, using the "adaptor" jkqtpstatAddLinearRegression():
    //jkqtpstatAddRobustIRLSLinearRegression(plot3->getPlotter(), datastore1->begin(colRLinX), datastore1->end(colRLinX), datastore1->begin(colRLinY), datastore1->end(colRLinY));
    // or even shorter:
    //jkqtpstatAddRobustIRLSLinearRegression(graphD);
    //      as a comparison, we also add the result of the normal/non-robust linear regression:
    jkqtpstatAddLinearRegression(graphD);

    // the following code demonstrates the influence of the rgularization parameter p:
    //   - the closer it is to 1, the more robust the fit is (it is closer to the L1-norm)
    //   - the closer it is to 2, the closer the fit is to the least squares solution (i.e. the normal regression)
    double p;
    p=1.1;
    auto g=jkqtpstatAddRobustIRLSLinearRegression(graphD, nullptr, nullptr, false, false, p);
    g->setTitle(g->getTitle()+", $p="+jkqtp_floattolatexqstr(p)+"$");
    p=1.5;
    g=jkqtpstatAddRobustIRLSLinearRegression(graphD, nullptr, nullptr, false, false, p);
    g->setTitle(g->getTitle()+", $p="+jkqtp_floattolatexqstr(p)+"$");
    p=1.7;
    g=jkqtpstatAddRobustIRLSLinearRegression(graphD, nullptr, nullptr, false, false, p);
    g->setTitle(g->getTitle()+", $p="+jkqtp_floattolatexqstr(p)+"$");
    p=2;
    g=jkqtpstatAddRobustIRLSLinearRegression(graphD, nullptr, nullptr, false, false, p);
    g->setTitle(g->getTitle()+", $p="+jkqtp_floattolatexqstr(p)+"$");



    // 5.1. The functions for linear regression can also be used to calculate some non-linear models by transforming the input data.
    //      This is also supported by the statistics library. the supported models are defined in JKQTPStatRegressionModelType
    //std::random_device rd; // random number generators:
    //std::mt19937 gen{rd()};
    //std::normal_distribution<> d1{0,1};
    double a0_powerlaw=20;
    double b0_powerlaw=0.25;
    double a0_exp=5;
    double b0_exp=0.5;
    double a0_log=0;
    double b0_log=1;
    size_t colNLLinX=datastore1->addColumn("non-lin data, x");
    size_t colNLLinYExp=datastore1->addColumn("non-lin data, y, exponential model");
    size_t colNLLinYPow=datastore1->addColumn("non-lin data, y, power-law model");
    size_t colNLLinYLog=datastore1->addColumn("non-lin data, y, log-law model");
    auto model_powerlaw=jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType::PowerLaw);
    auto model_exp=jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType::Exponential);
    auto model_log=jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType::Logarithm);
    for (double x=0.1; x<=10; x+=0.5) {
        datastore1->appendToColumn(colNLLinX, x);
        double ypow=model_powerlaw(x, a0_powerlaw, b0_powerlaw)+d1(gen);
        while (ypow<0) {
            ypow=model_powerlaw(x, a0_powerlaw, b0_powerlaw)+d1(gen);
        }
        datastore1->appendToColumn(colNLLinYPow, ypow);
        double yexp=model_exp(x, a0_exp, b0_exp)+d1(gen);
        while (yexp<0) {
            yexp=model_exp(x, a0_exp, b0_exp)+d1(gen);
        }
        datastore1->appendToColumn(colNLLinYExp, yexp);
        datastore1->appendToColumn(colNLLinYLog, model_log(x, a0_log, b0_log));
    }
    //     we visualize this data with a simple scatter graphs:
    JKQTPXYLineGraph* graphD_powerlaw;
    plot5->addGraph(graphD_powerlaw=new JKQTPXYLineGraph(plot5));
    graphD_powerlaw->setXYColumns(colNLLinX, colNLLinYPow);
    graphD_powerlaw->setDrawLine(false);
    graphD_powerlaw->setTitle(QString("data $%1+\\mathcal{N}(0,1)$").arg(jkqtpstatRegressionModel2Latex(JKQTPStatRegressionModelType::PowerLaw, a0_powerlaw, b0_powerlaw)));
    JKQTPXYLineGraph* graphD_exp;
    plot4->addGraph(graphD_exp=new JKQTPXYLineGraph(plot4));
    graphD_exp->setXYColumns(colNLLinX, colNLLinYExp);
    graphD_exp->setDrawLine(false);
    graphD_exp->setTitle(QString("data $%1+\\mathcal{N}(0,1)$").arg(jkqtpstatRegressionModel2Latex(JKQTPStatRegressionModelType::Exponential, a0_exp, b0_exp)));
    JKQTPXYLineGraph* graphD_log;
    plot5->addGraph(graphD_log=new JKQTPXYLineGraph(plot5));
    graphD_log->setXYColumns(colNLLinX, colNLLinYLog);
    graphD_log->setDrawLine(false);
    graphD_log->setTitle(QString("data $%1+\\mathcal{N}(0,1)$").arg(jkqtpstatRegressionModel2Latex(JKQTPStatRegressionModelType::Logarithm, a0_log, b0_log)));
    // 5.2. Now we calculate the regression models and add a plot to the graph:
    double cA=0, cB=0;
    JKQTPXFunctionLineGraph* gFunc;
    jkqtpstatRegression(JKQTPStatRegressionModelType::Exponential, datastore1->begin(colNLLinX), datastore1->end(colNLLinX), datastore1->begin(colNLLinYExp), datastore1->end(colNLLinYExp), cA, cB);
    plot4->addGraph(gFunc=new JKQTPXFunctionLineGraph(plot4));
    gFunc->setPlotFunctionFunctor(jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType::Exponential, cA, cB));
    gFunc->setTitle(QString("regression: $%1$").arg(jkqtpstatRegressionModel2Latex(JKQTPStatRegressionModelType::Exponential, cA, cB)));
    cA=0; cB=0;
    jkqtpstatRegression(JKQTPStatRegressionModelType::PowerLaw, datastore1->begin(colNLLinX), datastore1->end(colNLLinX), datastore1->begin(colNLLinYPow), datastore1->end(colNLLinYPow), cA, cB);
    plot5->addGraph(gFunc=new JKQTPXFunctionLineGraph(plot5));
    gFunc->setPlotFunctionFunctor(jkqtpStatGenerateRegressionModel(JKQTPStatRegressionModelType::PowerLaw, cA, cB));
    gFunc->setTitle(QString("regression: $%1$").arg(jkqtpstatRegressionModel2Latex(JKQTPStatRegressionModelType::PowerLaw, cA, cB)));
    //      Note: Here we used the normal linear regression functions, but variants for IRLS and weighted regression are also available!
    // 5.3. Of course also adaptors exist:
    //jkqtpstatAddRegression(plot4->getPlotter(), JKQTPStatRegressionModelType::Exponential, datastore1->begin(colNLLinX), datastore1->end(colNLLinX), datastore1->begin(colNLLinYExp), datastore1->end(colNLLinYExp));
    //jkqtpstatAddRegression(plot5->getPlotter(), JKQTPStatRegressionModelType::PowerLaw, datastore1->begin(colNLLinX), datastore1->end(colNLLinX), datastore1->begin(colNLLinYPow), datastore1->end(colNLLinYPow));
    //jkqtpstatAddRegression(graphD_exp, JKQTPStatRegressionModelType::Exponential);
    //jkqtpstatAddRegression(graphD_powerlaw, JKQTPStatRegressionModelType::PowerLaw);
    jkqtpstatAddRegression(graphD_log, JKQTPStatRegressionModelType::Logarithm);




    // 6.1. To demonstrate polynomial fitting, we generate data for a polynomial model
    std::vector<double> pPoly {1,2,-2,0.5};
    size_t colPolyX=datastore1->addColumn("polynomial data, x");
    size_t colPolyY=datastore1->addColumn("polynomial data, y");
    for (double x=-10; x<=10; x++) {
        datastore1->appendToColumn(colPolyX, x);
        datastore1->appendToColumn(colPolyY, jkqtp_polyEval(x, pPoly.begin(), pPoly.end())+d1(gen)*50.0);
    }
    //     we visualize this data with a simple scatter graph:
    JKQTPXYLineGraph* graphP;
    plot6->addGraph(graphP=new JKQTPXYLineGraph(plot6));
    graphP->setXYColumns(colPolyX, colPolyY);
    graphP->setDrawLine(false);
    graphP->setTitle(QString("data $%1+\\mathcal{N}(0,50)$").arg(jkqtp_polynomialModel2Latex(pPoly.begin(), pPoly.end())));
    // 6.2. now we can fit polynomials with different number of coefficients:
    for (size_t p=0; p<=5; p++) {
        std::vector<double> pFit;
        JKQTPXFunctionLineGraph* gPoly;
        jkqtpstatPolyFit(datastore1->begin(colPolyX), datastore1->end(colPolyX), datastore1->begin(colPolyY), datastore1->end(colPolyY), p, std::back_inserter(pFit));
        plot6->addGraph(gPoly=new JKQTPXFunctionLineGraph(plot6));
        gPoly->setPlotFunctionFunctor(jkqtp_generatePolynomialModel(pFit.begin(), pFit.end()));
        gPoly->setTitle(QString("regression: $%1$").arg(jkqtp_polynomialModel2Latex(pFit.begin(), pFit.end())));
    }
    // 6.3. of course also the "adaptor" shortcuts are available:
    //for (size_t p=0; p<=5; p++) {
    //    jkqtpstatAddPolyFit(plot6->getPlotter(), datastore1->begin(colPolyX), datastore1->end(colPolyX), datastore1->begin(colPolyY), datastore1->end(colPolyY), p);
    //    jkqtpstatAddPolyFit(graphP, p);
    //}





    // autoscale the plot so the graph is contained
    plot1->zoomToFit();
    plot1->getXAxis()->setShowZeroAxis(false);
    plot1->getYAxis()->setShowZeroAxis(false);
    plot1->getPlotter()->setKeyPosition(JKQTPKeyPosition::JKQTPKeyInsideTopLeft);
    plot2->zoomToFit();
    plot2->getXAxis()->setShowZeroAxis(false);
    plot2->getYAxis()->setShowZeroAxis(false);
    plot2->getPlotter()->setKeyPosition(JKQTPKeyPosition::JKQTPKeyInsideTopLeft);
    plot3->zoomToFit();
    plot3->getXAxis()->setShowZeroAxis(false);
    plot3->getYAxis()->setShowZeroAxis(false);
    plot3->getPlotter()->setKeyPosition(JKQTPKeyPosition::JKQTPKeyInsideTopLeft);
    plot4->zoomToFit();
    plot4->getXAxis()->setShowZeroAxis(false);
    plot4->getYAxis()->setShowZeroAxis(false);
    plot4->getPlotter()->setKeyPosition(JKQTPKeyPosition::JKQTPKeyInsideTopLeft);
    plot4->setAbsoluteX(0.05, plot4->getXMax());
    plot4->zoomToFit();
    plot5->getXAxis()->setShowZeroAxis(false);
    plot5->getYAxis()->setShowZeroAxis(false);
    plot5->getPlotter()->setKeyPosition(JKQTPKeyPosition::JKQTPKeyInsideTopLeft);
    plot5->setAbsoluteX(0.05, plot5->getXMax());
    plot5->zoomToFit();
    plot6->getXAxis()->setShowZeroAxis(false);
    plot6->getYAxis()->setShowZeroAxis(false);
    plot6->getPlotter()->setKeyPosition(JKQTPKeyPosition::JKQTPKeyInsideBottomRight);
    plot6->zoomToFit();

    // show plotter and make it a decent size
    mainWidget.show();
    mainWidget.resize(1600,800);

    return app.exec();
}
