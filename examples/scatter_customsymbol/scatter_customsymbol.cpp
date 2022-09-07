/** \example scatter_customsymbol.cpp
 * Usage of JKQTPScatterGraph with custom symbols
 *
 * \ref JKQTPlotterscatterCustomSymbol
 */
 
#include "jkqtpexampleapplication.h"
#include <QApplication>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtpexampleapplication.h"
#include <random>

int main(int argc, char* argv[])
{

    JKQTPAppSettingController highDPIController(argc,argv);
    JKQTPExampleApplication app(argc, argv);


    // 1. create a plotter window and get a pointer to the internal datastore (for convenience)
    JKQTPlotter plot;
    JKQTPDatastore* ds=plot.getDatastore();

    // 2. add two columns to the JKQTPDatastore and obtain back-inserter iterators for these
    size_t columnX=ds->addColumn("x");
    auto colXInserter=ds->backInserter(columnX);
    size_t columnY1=ds->addColumn("y1");
    auto colY1Inserter=ds->backInserter(columnY1);
    size_t columnY2=ds->addColumn("y2");
    auto colY2Inserter=ds->backInserter(columnY2);
    size_t columnY3=ds->addColumn("y3");
    auto colY3Inserter=ds->backInserter(columnY3);
    size_t columnY4=ds->addColumn("y4");
    auto colY4Inserter=ds->backInserter(columnY4);


    // 3. now we create data for a simple plot (a sine curve with random noise)
    const int Ndata=5;
    for (int i=0; i<Ndata; i++) {
        // put data
        const double x=double(i)/double(Ndata-1);
        *(colXInserter++)=x;
        *(colY1Inserter++)=3.0+pow(x*1.3, 2.0)*1.3;
        *(colY2Inserter++)=2.0+pow(x*1.3, 2.0)*1.2;
        *(colY3Inserter++)=1.0+pow(x*1.3, 2.0)*1.1;
        *(colY4Inserter++)=pow(x*1.3, 2.0);
    }

    // 4. define custom symbol
    JKQTPCustomGraphSymbolFunctor f=[](QPainter& p) {
        p.setBrush(Qt::NoBrush); // ensure that circles are not drawn filled
        p.drawEllipse(QPointF(-0.33, -0.33/4.0), 0.33/2.0, 0.33/2.0);
        p.drawEllipse(QPointF(0, -0.33/4.0), 0.33/2.0, 0.33/2.0);
        p.drawEllipse(QPointF(0.33, -0.33/4.0), 0.33/2.0, 0.33/2.0);
        p.drawEllipse(QPointF(-0.33/2.0, 0.33/4.0), 0.33/2.0, 0.33/2.0);
        p.drawEllipse(QPointF(0.33/2.0, 0.33/4.0), 0.33/2.0, 0.33/2.0);
    };
    JKQTPGraphSymbols customsymbol_olympicrings=JKQTPRegisterCustomGraphSymbol(f);

    JKQTPGraphSymbols customsymbol_coloredolympicrings=JKQTPRegisterCustomGraphSymbol(
                [](QPainter& p) {
                        p.setBrush(Qt::NoBrush); // ensure that circles are not drawn filled
                        const double w=p.pen().widthF();
                        p.setPen(QPen(QColor("red"), w));
                        p.drawEllipse(QPointF(-0.33, -0.33/4.0), 0.33/2.0, 0.33/2.0);
                        p.setPen(QPen(QColor("black"), w));
                        p.drawEllipse(QPointF(0, -0.33/4.0), 0.33/2.0, 0.33/2.0);
                        p.setPen(QPen(QColor("gold"), w));
                        p.drawEllipse(QPointF(0.33, -0.33/4.0), 0.33/2.0, 0.33/2.0);
                        p.setPen(QPen(QColor("darkgreen"), w));
                        p.drawEllipse(QPointF(-0.33/2.0, 0.33/4.0), 0.33/2.0, 0.33/2.0);
                        p.setPen(QPen(QColor("darkblue"), w));
                        p.drawEllipse(QPointF(0.33/2.0, 0.33/4.0), 0.33/2.0, 0.33/2.0);
                    });

    // 4. create graphs in the plot, which plots the datasets:
    JKQTPXYScatterGraph* graph1=new JKQTPXYScatterGraph(&plot);
    graph1->setXColumn(columnX);
    graph1->setYColumn(columnY1);
    graph1->setSymbolType(customsymbol_olympicrings);
    graph1->setSymbolSize(30);
    graph1->setTitle(QObject::tr("olympics"));
    plot.addGraph(graph1);

    JKQTPXYScatterGraph* graph2=new JKQTPXYScatterGraph(&plot);
    graph2->setXColumn(columnX);
    graph2->setYColumn(columnY2);
    graph2->setSymbolType(customsymbol_coloredolympicrings);
    graph2->setSymbolSize(30);
    graph2->setTitle(QObject::tr("colored olympics"));
    plot.addGraph(graph2);

    JKQTPXYScatterGraph* graph3=new JKQTPXYScatterGraph(&plot);
    graph3->setXColumn(columnX);
    graph3->setYColumn(columnY3);
    graph3->setSymbolType(JKQTPCharacterSymbol+QChar(0x2660).unicode());
    graph3->setSymbolColor(QColor("darkblue"));
    graph3->setSymbolSize(15);
    graph3->setTitle(QObject::tr("spades"));
    plot.addGraph(graph3);

    JKQTPXYScatterGraph* graph4=new JKQTPXYScatterGraph(&plot);
    graph4->setXColumn(columnX);
    graph4->setYColumn(columnY4);
    graph4->setSymbolType(JKQTPFilledCharacterSymbol+QChar(0x2665).unicode());
    graph4->setSymbolSize(20);
    graph4->setSymbolColor(QColor("black"));
    graph4->setSymbolFillColor(QColor("red"));
    graph4->setSymbolLineWidth(0.5);
    graph4->setTitle(QObject::tr("hearts"));
    plot.addGraph(graph4);

    // 6. autoscale the plot so the graph is contained
    plot.setXY(-0.1,1.1,-1, 6);

    // show plotter and make it a decent size
    plot.getXAxis()->setShowZeroAxis(false);
    plot.getYAxis()->setShowZeroAxis(false);
    plot.setGrid(false);
    plot.getPlotter()->setKeyPosition(JKQTPKeyInsideTopLeft);
    plot.getPlotter()->setPlotLabel(QObject::tr("Custom Scatter Symbols Example"));
    plot.show();
    plot.resize(500/plot.devicePixelRatioF(),500/plot.devicePixelRatioF());

    return app.exec();
}
