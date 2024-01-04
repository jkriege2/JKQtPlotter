/** \example multithreaded.cpp
 * Using JKQTBasePlotter in multiple threads in parallel.
 *
 * \ref JKQTPlotterMultiThreaded
 */
 
#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QBoxLayout>
#include <QVector>
#include <QElapsedTimer>
#include <QFile>
#include <thread>
#include "multithreaded_thread.h"
#include "jkqtmath/jkqtpstatbasics.h"
#include "jkqtpexampleapplication.h"

#define NUM_SHOWN_PLOTS 3
#define NUM_PLOTS 8
#define NUM_GRAPHS 6
#define NUM_DATAPOINTS 1000


int main(int argc, char* argv[])
{
    JKQTPAppSettingController highDPIController(argc,argv);
    JKQTPExampleApplication app(argc, argv);
    QMainWindow* mainWin=new QMainWindow();
    mainWin->setWindowTitle("Multi-Threaded Plotting");
    QWidget* main;
    mainWin->setCentralWidget(main=new QWidget(mainWin));

    QString markdownFile="";
    for (int i=1; i<argc; i++) {
        if (QString(argv[i]).startsWith("--mdfile=")) {
            markdownFile=QString::fromLatin1(argv[i]).right(QString::fromLatin1(argv[i]).size()-9);
            if (markdownFile.startsWith('"')) markdownFile.remove('"');
        }
    }


    int result=0;
    QStringList filenamesSerial, filenamesParallel;
    {

        QHBoxLayout* mainLayout=new QHBoxLayout(main);
        main->setLayout(mainLayout);
        QVBoxLayout* lay_serial=new QVBoxLayout();
        QVBoxLayout* lay_parallel=new QVBoxLayout();
        mainLayout->addLayout(lay_serial);
        QLabel* l;
        lay_serial->addWidget(l=new QLabel("Serialized Plotting"));
        QFont f=l->font();
        f.setBold(true);
        f.setPointSize(16);
        l->setFont(f);
        lay_parallel->addWidget(l=new QLabel("Parallel Plotting"));
        l->setFont(f);

        QLabel* labSerialResult=new QLabel(main);
        lay_serial->addWidget(labSerialResult);
        QLabel* labParallelResult=new QLabel(main);
        lay_parallel->addWidget(labParallelResult);

        mainLayout->addLayout(lay_parallel);
        QVector<QLabel*> pic_parallel, pic_serial;
        for (int i=0; i<NUM_SHOWN_PLOTS; i++) {
            pic_serial.push_back(new QLabel(main));
            pic_parallel.push_back(new QLabel(main));
            lay_serial->addWidget(pic_serial.last(), 1);
            lay_parallel->addWidget(pic_parallel.last(), 1);
        }

        QElapsedTimer timer;
        QList<double> runtimesSerial;
        timer.start();
        for (int i=0; i<NUM_PLOTS; i++) {
            double dur=0;
            filenamesSerial<<PlottingThread::plotAndSave("serial", i, NUM_GRAPHS, NUM_DATAPOINTS, &dur);
            runtimesSerial<<dur/1e6;
        }
        const double durSerialNano=timer.nsecsElapsed();
        qDebug()<<"durSerial = "<<durSerialNano/1e6<<"ms";

        QList<double> runtimesParallel;
        QList<QSharedPointer<PlottingThread>> threads;
        for (int i=0; i<NUM_PLOTS; i++) {
            qDebug()<<"  creating thread "<<i;
            threads.append(QSharedPointer<PlottingThread>::create("parallel",i, NUM_GRAPHS, NUM_DATAPOINTS, nullptr));
        }
        timer.start();
        for (int i=0; i<NUM_PLOTS; i++) {
            qDebug()<<"  staring thread "<<i;
            threads[i]->start();
        }
        for (int i=0; i<NUM_PLOTS; i++) {
            qDebug()<<"  waiting for thread "<<i;
            if (threads[i]->wait()) {
                filenamesParallel<<threads[i]->getFilename();
                runtimesParallel<<threads[i]->getRuntimeNanosends()/1e6;
            }
        }
        const double durParallelNano=timer.nsecsElapsed();
        qDebug()<<"durParallel = "<<durParallelNano/1e6<<"ms";

        threads.clear();

        for (int ii=0; ii<NUM_SHOWN_PLOTS; ii++) {
            int i=ii;
            if (ii>NUM_SHOWN_PLOTS/2) i=NUM_PLOTS-1-NUM_SHOWN_PLOTS+ii;
            pic_serial[ii]->setPixmap(QPixmap(filenamesSerial[i], "PNG"));
            pic_parallel[ii]->setPixmap(QPixmap(filenamesParallel[i], "PNG"));
        }
        QString ser_result, par_result;
        labSerialResult->setText(ser_result=QString("runtime, overall = %1ms<br/>single runtimes = (%2 +/- %3) ms<br/>speedup = %4x<br/>threads / available = %5 / %6<br/><br/>  ").arg(durSerialNano/1e6,0,'f',1).arg(jkqtpstatAverage(runtimesSerial.begin(), runtimesSerial.end()),0,'f',1).arg(jkqtpstatStdDev(runtimesSerial.begin(), runtimesSerial.end()),0,'f',1).arg(jkqtpstatSum(runtimesSerial.begin(), runtimesSerial.end())/(durSerialNano/1e6),0,'f',2).arg(1).arg(std::thread::hardware_concurrency()));
        labParallelResult->setText(par_result=QString("runtime, overall = %1ms<br/>single runtimes = (%2 +/- %3) ms<br/>speedup = %4x<br/>threads / available = %5 / %6<br/><br/><b>speedup vs. serial = %7x</b>").arg(durParallelNano/1e6,0,'f',1).arg(jkqtpstatAverage(runtimesParallel.begin(), runtimesParallel.end()),0,'f',1).arg(jkqtpstatStdDev(runtimesParallel.begin(), runtimesParallel.end()),0,'f',1).arg(jkqtpstatSum(runtimesParallel.begin(), runtimesParallel.end())/(durParallelNano/1e6),0,'f',2).arg(NUM_PLOTS).arg(std::thread::hardware_concurrency()).arg(durSerialNano/durParallelNano,0,'f',1));
        mainWin->show();

        if (!markdownFile.isEmpty()) {
            qDebug()<<"modifying MD-file "<<markdownFile;
            QFile f(markdownFile);
            QByteArray md;
            if (f.open(QFile::ReadOnly|QFile::Text)) {
                md=f.readAll();
                qDebug()<<"  read "<<md.size()<<" bytes";
                f.close();
                const auto istart=md.indexOf("[comment]:RESULTS");
                const auto iend=md.indexOf("[comment]:RESULTS_END");
                qDebug()<<"  istart="<<istart<<",  iend="<<iend;
                if (istart>=0 && iend>istart) {
                    const QByteArray newResults="[comment]:RESULTS\n\n<u><b>SERIAL RESULTS:</b></u><br/>"+ser_result.toUtf8()
                                               +"\n\n<u><b>PARALLEL RESULTS:</b></u><br/>\n"+par_result.toUtf8()+"\n\n";
                    md.replace(istart,iend-istart,newResults);
                    if (f.open(QFile::WriteOnly)) {
                        qDebug()<<"  writing "<<md.size() <<"bytes";
                        f.write(md);
                        f.close();
                    }
                }
            }
        } else {
            qDebug()<<"no MD-file given";
        }

        result = app.exec();
    }
    for (const auto& fn: (filenamesSerial+filenamesParallel)) {
        QFile::remove(fn);
    }
    return result;
}
