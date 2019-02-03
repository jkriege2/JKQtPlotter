/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

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


#include <QFileInfo>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtGlobal>
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include <QSvgGenerator>
#include <QDebug>
#include <QElapsedTimer>
#include <QPrintPreviewWidget>
#include <QDialog>
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplottergui/jkqtpgraphsmodel.h"
#include "jkqtplottergui/jkqtpenhancedtableview.h"
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintDialog>
#include <QGridLayout>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>
#include <QVBoxLayout>
#ifdef QFWIDLIB_LIBRARY
#  include "qftools.h"
#endif
#include "jkqtplotter/jkqtpgraphsboxplot.h"
#include "jkqtplotter/jkqtpgraphsbarchart.h"
#include "jkqtplotter/jkqtpgraphsfilledcurve.h"
#include "jkqtplotter/jkqtpgraphsimpulses.h"
#include "jkqtplotter/jkqtpgraphs.h"
#include "jkqtplotter/jkqtpgraphsgeometric.h"
#include "jkqtplotter/jkqtpgraphsimage.h"

static QString globalUserSettigsFilename="";
static QString globalUserSettigsPrefix="";
static QList<JKQTPPaintDeviceAdapter*> jkqtpPaintDeviceAdapters;
static QList<JKQTPSaveDataAdapter*> jkqtpSaveDataAdapters;


void initJKQTBasePlotterResources()
{
    Q_INIT_RESOURCE(jkqtpbaseplotter);
    initJKQTMathTextResources();
}

JKQTBasePlotter::JKQTBasePlotter()
{
    initJKQTBasePlotterResources();
}

void JKQTBasePlotter::setDefaultJKQTBasePrinterUserSettings(QString userSettigsFilename, const QString& userSettigsPrefix)
{
    globalUserSettigsFilename=userSettigsFilename;
    globalUserSettigsPrefix=userSettigsPrefix;
}

void JKQTBasePlotter::registerPaintDeviceAdapter(JKQTPPaintDeviceAdapter *adapter)
{
    jkqtpPaintDeviceAdapters.append(adapter);
}

void JKQTBasePlotter::deregisterPaintDeviceAdapter(JKQTPPaintDeviceAdapter *adapter)
{
    if (jkqtpPaintDeviceAdapters.contains(adapter)) jkqtpPaintDeviceAdapters.removeAll(adapter);
}

bool JKQTBasePlotter::registerSaveDataAdapter(JKQTPSaveDataAdapter *adapter)
{
    if (adapter){
        QString format=adapter->getFilter();
        for (int i=0; i<jkqtpSaveDataAdapters.size(); i++) {
            if (jkqtpSaveDataAdapters[i] && jkqtpSaveDataAdapters[i]->getFilter()==format) {
                return false;
            }
        }
        jkqtpSaveDataAdapters.append(adapter);
        return true;
    }
    return false;
}

bool JKQTBasePlotter::deregisterSaveDataAdapter(JKQTPSaveDataAdapter *adapter)
{
    if (jkqtpSaveDataAdapters.contains(adapter)) jkqtpSaveDataAdapters.removeAll(adapter);
    return true;
}

JKQTBasePlotter::textSizeData JKQTBasePlotter::getTextSizeDetail(const QFont &fm, const QString &text, QPainter& painter)
{
    return getTextSizeDetail(fm.family(), fm.pointSizeF(), text, painter);
}

JKQTBasePlotter::textSizeData JKQTBasePlotter::getTextSizeDetail(const QString &fontName, double fontSize, const QString &text, QPainter& painter)
{
    JKQTBasePlotter::textSizeKey  dh(fontName, fontSize, text, painter.device());
    if (tbrh.contains(dh)) return tbrh[dh];
    JKQTBasePlotter::textSizeData d;
    mathText.setFontRoman(fontName);
    mathText.setFontSize(fontSize);
    mathText.parse(text);
    mathText.getSizeDetail(painter, d.width, d.ascent, d.descent, d.strikeoutPos);
    tbrh[dh]=d;
    //qDebug()<<"+++ textsize hash size: "<<tbrh.size();
    return d;
}

void JKQTBasePlotter::getTextSizeDetail(const QString &fontName, double fontSize, const QString &text, QPainter &painter, double &width, double &ascent, double &descent, double &strikeoutPos)
{
    JKQTBasePlotter::textSizeData d=getTextSizeDetail(fontName, fontSize, text, painter);
    width=d.width;
    ascent=d.ascent;
    descent=d.descent;
    strikeoutPos=d.strikeoutPos;
}

void JKQTBasePlotter::getTextSizeDetail(const QFont &fm, const QString &text, QPainter &painter, double &width, double &ascent, double &descent, double &strikeoutPos)
{
    getTextSizeDetail(fm.family(), fm.pointSizeF(), text, painter, width, ascent, descent, strikeoutPos);
}

QSizeF JKQTBasePlotter::getTextSizeSize(const QFont &fm, const QString &text, QPainter &painter)
{
    return getTextSizeSize(fm.family(), fm.pointSizeF(), text, painter);
}

QSizeF JKQTBasePlotter::getTextSizeSize(const QString &fontName, double fontSize, const QString &text, QPainter &painter)
{
    JKQTBasePlotter::textSizeData d=getTextSizeDetail(fontName, fontSize, text, painter);
    return QSizeF(d.width, d.ascent+d.descent);
}


// define this to get timing information about painting on the debug output
//#define JKQTBP_DEBUGTIMING
#undef JKQTBP_DEBUGTIMING

/**************************************************************************************************************************
 * JKQTBasePlotter
 **************************************************************************************************************************/
JKQTBasePlotter::JKQTBasePlotter(bool datastore_internal, QObject* parent, JKQTPDatastore* datast):
    QObject(parent), m_plotsModel(nullptr), xAxis(nullptr), yAxis(nullptr)
{

    dataColumnsListWidget=nullptr;
    printMagnification=1.0;
    printZoomFactor=1.0;
    printSizeX_Millimeter=1.0;
    printSizeY_Millimeter=1.0;
    paintMagnification=1.0;
    printDoUpdate=true;

    emitPlotSignals=true;
    masterPlotterX=nullptr;
    masterPlotterY=nullptr;
    masterSynchronizeWidth=false;
    masterSynchronizeHeight=false;
    fontSizePrintMultiplier=1;
    lineWidthPrintMultiplier=1;
    fontSizeMultiplier=1;
    lineWidthMultiplier=1;
    userSettigsFilename=globalUserSettigsFilename;
    userSettigsPrefix=globalUserSettigsPrefix;
    currentPrinter=QPrinterInfo::defaultPrinter().printerName();

    if (datastore_internal) {
      datastore=new JKQTPDatastore();
      datastoreInternal=true;
    } else {
      datastore=datast;
      datastoreInternal=false;
    }

    xAxis=new JKQTPHorizontalAxis(this);
    yAxis=new JKQTPVerticalAxis(this);
    m_plotsModel=new JKQTPGraphsModel(this);
    connect(this, SIGNAL(plotUpdated()), m_plotsModel, SLOT(plotUpdated()));


    emitSignals=false;

    plot_minmaxcoorinate=1e10;


    resize(400,300);
    plotWidth=400;
    plotHeight=300;
    initSettings();

    emitSignals=true;

    actSavePlot=new QAction(QIcon(":/JKQTPlotter/jkqtp_saveplot.png"), "Save Plot", this);
    actSavePlot->setToolTip("Save plot as image file (PDF, PS; PNG, ...).");
    actSaveData=new QAction(QIcon(":/JKQTPlotter/jkqtp_savedata.png"), "Save Data", this);
    actSaveData->setToolTip("Save Data of the plot as file (CSV, ...).");
    actCopyData=new QAction(QIcon(":/JKQTPlotter/jkqtp_copydata.png"), "Copy Data", this);
    actCopyData->setToolTip("Copy Data of the plot to the clipboard to be pasted into Excel etc.");
    actCopyMatlab=new QAction(QIcon(":/JKQTPlotter/jkqtp_copymatlab.png"), "Copy Data to Matlab", this);
    actCopyMatlab->setToolTip("Copy Data of the plot to the clipboard in Matlab script format.");
    actCopyPixelImage=new QAction(QIcon(":/JKQTPlotter/jkqtp_copyimg.png"), "Copy Image", this);
    actCopyPixelImage->setToolTip("Copy the plot as a pixel image to the clipboard");


    actSavePDF=new QAction(QIcon(":/JKQTPlotter/jkqtp_savepdf.png"), "Save P&DF", this);
    actSavePDF->setToolTip("Save as PDF");
    //toolbar->addAction(actSavePDF);
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    actSavePS=new QAction(QIcon(":/JKQTPlotter/jkqtp_saveps.png"), "Save P&S", this);
    actSavePS->setToolTip("Save as PostScript");
    //toolbar->addAction(actSavePS);
#endif
    actSaveSVG=new QAction(QIcon(":/JKQTPlotter/jkqtp_savesvg.png"), "Save S&VG", this);
    actSaveSVG->setToolTip("Save as Scalable Vector Graphics (SVG)");
    //toolbar->addAction(actSaveSVG);
    actSavePix=new QAction(QIcon(":/JKQTPlotter/jkqtp_savepix.png"), "Save &Image", this);
    actSavePix->setToolTip("Save as Pixel Image (PNG, JPEG, TIFF ...)");
    //toolbar->addAction(actSavePix);

    actPrint=new QAction(QIcon(":/JKQTPlotter/jkqtp_print.png"), "&Print", this);
    actPrint->setToolTip("Print");
    //toolbar->addSeparator();
    actSaveCSV=new QAction(QIcon(":/JKQTPlotter/jkqtp_savecsv.png"), "Save &CSV", this);
    actSaveCSV->setToolTip("Save the data which is used for the plot as Comma Separated Values (CSV)");
    //toolbar->addAction(actSaveCSV);
    //toolbar->addSeparator();
    actZoomAll=new QAction(QIcon(":/JKQTPlotter/jkqtp_zoomall.png"), "Zoom &All", this);
    actZoomAll->setToolTip("Zoom to view all data");
    actZoomIn=new QAction(QIcon(":/JKQTPlotter/jkqtp_zoomin.png"), "Zoom &In", this);
    actZoomIn->setToolTip("Zoom in around the center of the plot");
    actZoomOut=new QAction(QIcon(":/JKQTPlotter/jkqtp_zoomout.png"), "Zoom &Out", this);
    actZoomOut->setToolTip("Zoom out");

    actShowPlotData=new QAction(QIcon(":/JKQTPlotter/jkqtp_showplotdata.png"), "&Show Plot Data", this);
    actShowPlotData->setToolTip("opens a dialog that contains all data used for the plot in a table.");



    connect(actSavePlot,   SIGNAL(triggered()), this, SLOT(saveImage()));
    connect(actSaveData,   SIGNAL(triggered()), this, SLOT(saveData()));
    connect(actCopyData,   SIGNAL(triggered()), this, SLOT(copyData()));
    connect(actCopyPixelImage,   SIGNAL(triggered()), this, SLOT(copyPixelImage()));
    connect(actCopyMatlab,   SIGNAL(triggered()), this, SLOT(copyDataMatlab()));
    connect(actShowPlotData,   SIGNAL(triggered()), this, SLOT(showPlotData()));

    connect(actSavePDF,    SIGNAL(triggered()), this, SLOT(saveAsPDF()));
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    connect(actSavePS,     SIGNAL(triggered()), this, SLOT(saveAsPS()));
#endif
    connect(actSaveSVG,    SIGNAL(triggered()), this, SLOT(saveAsSVG()));
    connect(actSavePix,    SIGNAL(triggered()), this, SLOT(saveAsPixelImage()));

    connect(actPrint,      SIGNAL(triggered()), this, SLOT(print()));
    connect(actSaveCSV,    SIGNAL(triggered()), this, SLOT(saveAsCSV()));
    connect(actZoomAll,    SIGNAL(triggered()), this, SLOT(zoomToFit()));
    connect(actZoomIn,     SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(actZoomOut,    SIGNAL(triggered()), this, SLOT(zoomOut()));

    resize(400,300);
#ifdef USE_XITS_FONTS
    mathText.useXITS();
#endif
}

JKQTBasePlotter::~JKQTBasePlotter(){
    clearGraphs(false);
    if (datastoreInternal && datastore!=nullptr) delete datastore;
    delete xAxis;
    delete yAxis;
}

void JKQTBasePlotter::setGrid(bool val) {
    xAxis->setDrawGrid(val);
    yAxis->setDrawGrid(val);
};

void JKQTBasePlotter::useExternalDatastore(JKQTPDatastore* newStore){
    if (datastoreInternal && datastore!=nullptr) {
        delete datastore;
        datastore=nullptr;
    }
    datastore=newStore;
    datastoreInternal=false;
};

void JKQTBasePlotter::useAsInternalDatastore(JKQTPDatastore* newStore){
    if (datastoreInternal && datastore!=nullptr) {
        delete datastore;
        datastore=nullptr;
    }
    datastore=newStore;
    datastoreInternal=true;
    if (emitPlotSignals) emit plotUpdated();
}

void JKQTBasePlotter::useInternalDatastore(){
    if (!datastoreInternal) {
        datastore=new JKQTPDatastore();
        datastoreInternal=true;
    }
    if (emitPlotSignals) emit plotUpdated();
}

void JKQTBasePlotter::forceInternalDatastore(){
    if (datastoreInternal && datastore!=nullptr) {
        delete datastore;
        datastore=nullptr;
    }
    datastore=new JKQTPDatastore();
    datastoreInternal=true;
    if (emitPlotSignals) emit plotUpdated();
}

bool JKQTBasePlotter::isEmittingSignalsEnabled() const {
    return this->emitSignals;
}

void JKQTBasePlotter::initSettings() {
    useClipping=true;
    //doDrawing=true;

    maintainAspectRatio=default_maintainAspectRatio=false;
    aspectRatio=default_aspectRatio=1;

    maintainAxisAspectRatio=default_maintainAxisAspectRatio=false;
    axisAspectRatio=default_axisAspectRatio=1;

    default_useAntiAliasingForSystem=false; useAntiAliasingForSystem=default_useAntiAliasingForSystem;
    default_useAntiAliasingForGraphs=true; useAntiAliasingForGraphs=default_useAntiAliasingForGraphs;
    default_useAntiAliasingForText=true; useAntiAliasingForText=default_useAntiAliasingForText;

    gridPrinting=false;
    gridPrintingList.clear();
    gridPrintingCurrentX=0;
    gridPrintingCurrentY=0;


    default_plotBorderLeft=5; iplotBorderLeft_nographs=iplotBorderLeft=plotBorderLeft=default_plotBorderLeft;
    default_plotBorderRight=5; iplotBorderRight_nographs=iplotBorderRight=plotBorderRight=default_plotBorderRight;
    default_plotBorderTop=5; iplotBorderTop_nographs=iplotBorderTop=plotBorderTop=default_plotBorderTop;
    default_plotBorderBottom=5; iplotBorderBottom_nographs=iplotBorderBottom=plotBorderBottom=default_plotBorderBottom;

    //plotWidth=700;
    //plotHeight=150;

    xAxis->setRange(-10, 10);
    yAxis->setRange(-10, 10);

    default_keyFont="Arial"; keyFont=default_keyFont;
    default_keyFontSize=9; keyFontSize=default_keyFontSize;


    default_keyXOffset=0.5; keyXOffset=default_keyXOffset;
    default_keyYOffset=0.5; keyYOffset=default_keyYOffset;
    default_keyXSeparation=0.75; keyXSeparation=default_keyXSeparation;
    default_keyYSeparation=0.75; keyYSeparation=default_keyYSeparation;
    default_keyFrameColor=QColor("black"); keyFrameColor=default_keyFrameColor;
    default_keyFrameWidth=1; keyFrameWidth=default_keyFrameWidth;
    default_showKeyFrame=true; showKeyFrame=default_showKeyFrame;
    default_keyBackgroundColor=QColor("white"); keyBackgroundColor=default_keyBackgroundColor;
    default_showKey=true; showKey=default_showKey;
    default_key_item_width=20; key_item_width=default_key_item_width;
    default_key_item_height=2.2; key_item_height=default_key_item_height;
    default_key_line_length=3; key_line_length=default_key_line_length;
    default_keyXMargin=0.5; keyXMargin=default_keyXMargin;
    default_keyYMargin=0.5; keyYMargin=default_keyYMargin;
    default_keyPosition=JKQTPKeyInsideTopRight; keyPosition=default_keyPosition;
    default_keyLayout=JKQTPKeyLayoutOneColumn; keyLayout=default_keyLayout;
    default_keyAutosize=true; keyAutosize=default_keyAutosize;

    default_exportBackgroundColor=QColor("white"); exportBackgroundColor=default_exportBackgroundColor;
    default_backgroundColor=QColor("white"); backgroundColor=default_backgroundColor;
    default_plotBackgroundColor=QColor("white"); plotBackgroundColor=default_plotBackgroundColor;
    default_graphColor=QColor("red"); graphColor=default_graphColor;
    manyGraphsColorCount=15;
    default_manyGraphsColor[0]=graphColor;
    default_manyGraphsColor[1]=QColor("green");
    default_manyGraphsColor[2]=QColor("blue");
    default_manyGraphsColor[3]=QColor("fuchsia");
    default_manyGraphsColor[4]=QColor("darkorange");
    default_manyGraphsColor[5]=QColor("navy");
    default_manyGraphsColor[6]=QColor("firebrick");
    default_manyGraphsColor[7]=QColor("darkgreen");
    default_manyGraphsColor[8]=QColor("darkmagenta");
    default_manyGraphsColor[9]=QColor("darkgreen");
    default_manyGraphsColor[10]=QColor("darkslateblue");
    default_manyGraphsColor[11]=QColor("maroon");
    default_manyGraphsColor[12]=QColor("indianred");
    default_manyGraphsColor[13]=QColor("darkolivegreen");
    default_manyGraphsColor[14]=QColor("mediumpurple");
    default_manyGraphsColor[15]=QColor("darkcyan");
    for (int i=0; i<manyGraphsColorCount; i++) {
        manyGraphsColor[i]=default_manyGraphsColor[i];
        //std::cout<<manyGraphsColor[i].red()<<", "<<manyGraphsColor[i].green()<<", "<<manyGraphsColor[i].blue()<<"    ";
        //std::cout<<"init: manyGraphsColor["<<i<<"] = "<<jkqtp_QColor2String(manyGraphsColor[i]).toStdString()<<" = "<<manyGraphsColor[i].name().toStdString()<<std::endl;
        //std::cout<<"init: default_manyGraphsColor["<<i<<"] = "<<jkqtp_QColor2String(default_manyGraphsColor[i]).toStdString()<<" = "<<default_manyGraphsColor[i].name().toStdString()<<std::endl;
    }
    default_manyGraphsStyle[0]=Qt::SolidLine;
    default_manyGraphsStyle[1]=Qt::DashLine;
    default_manyGraphsStyle[2]=Qt::DotLine;
    default_manyGraphsStyle[3]=Qt::DashDotLine;
    default_manyGraphsStyle[4]=Qt::DashDotDotLine;
    for (int i=0; i<=4; i++) { manyGraphsStyle[i]=default_manyGraphsStyle[i]; }
    default_graphWidth=2; graphWidth=default_graphWidth;
    default_CSVdecimalSeparator="."; CSVdecimalSeparator=default_CSVdecimalSeparator;
    default_CSVcommentInitializer="# "; CSVcommentInitializer=default_CSVcommentInitializer;

    plotLabel="";
    default_plotLabelFontname=plotLabelFontname=default_keyFont;
    default_plotLabelFontSize=plotLabelFontSize=12;

    if (emitPlotSignals) emit plotUpdated();
}






void JKQTBasePlotter::zoomIn(double factor) {
    //std::cout<<(double)event->delta()/120.0<<":   "<<factor<<std::endl;
    double xmin=p2x(static_cast<long>(round(static_cast<double>(plotWidth)/2.0-static_cast<double>(plotWidth)/(2.0*factor))));
    double xmax=p2x(static_cast<long>(round(static_cast<double>(plotWidth)/2.0+static_cast<double>(plotWidth)/(2.0*factor))));
    double ymin=p2y(static_cast<long>(round(static_cast<double>(plotHeight)/2.0+static_cast<double>(plotHeight)/(2.0*factor))));
    double ymax=p2y(static_cast<long>(round(static_cast<double>(plotHeight)/2.0-static_cast<double>(plotHeight)/(2.0*factor))));


    xAxis->setRange(xmin, xmax);
    yAxis->setRange(ymin, ymax);
    if (emitPlotSignals) emit plotUpdated();
    if (emitSignals) emit zoomChangedLocally(xAxis->getMin(), xAxis->getMax(), yAxis->getMin(), yAxis->getMax(), this);
}

void JKQTBasePlotter::zoom(double nxmin, double nxmax, double nymin, double nymax){
    // only react on double clicks inside event
    double xmin=nxmin;
    double xmax=nxmax;
    double ymin=nymin;
    double ymax=nymax;

    xAxis->setRange(xmin, xmax);
    yAxis->setRange(ymin, ymax);
    if (emitPlotSignals) emit plotUpdated();
    if (emitSignals) emit zoomChangedLocally(xAxis->getMin(), xAxis->getMax(), yAxis->getMin(), yAxis->getMax(), this);
}

void JKQTBasePlotter::resize(int wid, int heigh) {
    widgetWidth=wid;
    widgetHeight=heigh;

    if (emitPlotSignals) emit plotUpdated();
}


void JKQTBasePlotter::setWidth(int wid) {
    widgetWidth=wid;
    if (emitPlotSignals) emit plotUpdated();
}

void JKQTBasePlotter::setHeight(int heigh) {
    widgetHeight=heigh;
    if (emitPlotSignals) emit plotUpdated();
}

void JKQTBasePlotter::saveSettings(QSettings& settings, const QString& group) const{
    QString g=group+"/";
    if (group.isEmpty()) g="";

    if (aspectRatio!=default_aspectRatio) settings.setValue(g+"apect_ratio", aspectRatio);
    if (maintainAspectRatio!=default_maintainAspectRatio) settings.setValue(g+"maintain_apect_ratio", maintainAspectRatio);
    if (axisAspectRatio!=default_axisAspectRatio) settings.setValue(g+"axis_apect_ratio", axisAspectRatio);
    if (maintainAxisAspectRatio!=default_maintainAxisAspectRatio) settings.setValue(g+"maintain_axis_apect_ratio", maintainAxisAspectRatio);
    if (CSVdecimalSeparator!=default_CSVdecimalSeparator) settings.setValue(g+"csv_decimal_separator", CSVdecimalSeparator);
    if (CSVcommentInitializer!=default_CSVcommentInitializer) settings.setValue(g+"csv_comment_initializer", CSVcommentInitializer);

    if (plotBorderLeft!=default_plotBorderLeft) settings.setValue(g+"plot_border_left", plotBorderLeft);
    if (plotBorderRight!=default_plotBorderRight) settings.setValue(g+"plot_border_right", plotBorderRight);
    if (plotBorderTop!=default_plotBorderTop) settings.setValue(g+"plot_border_top", plotBorderTop);
    if (plotBorderBottom!=default_plotBorderBottom) settings.setValue(g+"plot_border_bottom", plotBorderBottom);

    if (keyXOffset!=default_keyXOffset) settings.setValue(g+"key_xoffset", keyXOffset);
    if (keyYOffset!=default_keyYOffset) settings.setValue(g+"key_yoffset", keyYOffset);
    if (keyXMargin!=default_keyXMargin) settings.setValue(g+"key_xmargin", keyXMargin);
    if (keyYMargin!=default_keyYMargin) settings.setValue(g+"key_ymargin", keyYMargin);
    if (keyXSeparation!=default_keyXSeparation) settings.setValue(g+"key_xseparation", keyXSeparation);
    if (keyYSeparation!=default_keyYSeparation) settings.setValue(g+"key_yseparation", keyYSeparation);
    if (keyFrameColor!=default_keyFrameColor) settings.setValue(g+"key_frame_color", jkqtp_QColor2String(keyFrameColor));
    if (keyFrameWidth!=default_keyFrameWidth) settings.setValue(g+"key_frame_width", keyFrameWidth);
    if (showKeyFrame!=default_showKeyFrame) settings.setValue(g+"show_key_frame", showKeyFrame);
    if (keyBackgroundColor!=default_keyBackgroundColor) settings.setValue(g+"key_background_color", jkqtp_QColor2String(keyBackgroundColor));
    if (showKey!=default_showKey) settings.setValue(g+"show_key", showKey);
    if (keyPosition!=default_keyPosition) settings.setValue(g+"key_position", JKQTPKeyPosition2String(keyPosition));
    if (keyLayout!=default_keyLayout) settings.setValue(g+"key_layout", JKQTPKeyLayout2String(keyLayout));

    if (useAntiAliasingForSystem!=default_useAntiAliasingForSystem) settings.setValue(g+"use_antialiasing_for_system", useAntiAliasingForSystem);
    if (useAntiAliasingForGraphs!=default_useAntiAliasingForGraphs) settings.setValue(g+"use_antialiasing_for_graphs", useAntiAliasingForGraphs);
    if (useAntiAliasingForText!=default_useAntiAliasingForText) settings.setValue(g+"use_antialiasing_for_text", useAntiAliasingForText);

    if (backgroundColor!=default_backgroundColor) settings.setValue(g+"background_color", jkqtp_QColor2String(backgroundColor));
    if (exportBackgroundColor!=default_exportBackgroundColor) settings.setValue(g+"exportBackgroundColor", jkqtp_QColor2String(exportBackgroundColor));
    if (plotBackgroundColor!=default_plotBackgroundColor) settings.setValue(g+"plot_background_color", jkqtp_QColor2String(plotBackgroundColor));
    if (graphColor!=default_graphColor) settings.setValue(g+"graph_color", jkqtp_QColor2String(graphColor));
    if (graphWidth!=default_graphWidth) settings.setValue(g+"graph_linewidth", graphWidth);


    if (keyFont!=default_keyFont) settings.setValue(g+"key_fontname", keyFont);
    if (keyFontSize!=default_keyFontSize) settings.setValue(g+"key_fontsize", keyFontSize);
    if (key_item_width!=default_key_item_width) settings.setValue(g+"key_item_width", key_item_width);
    if (key_item_height!=default_key_item_height) settings.setValue(g+"key_item_height", key_item_height);
    if (key_line_length!=default_key_line_length) settings.setValue(g+"key_line_width", key_line_length);
    if (keyAutosize!=default_keyAutosize) settings.setValue(g+"key_autosize", keyAutosize);

    if (plotLabelFontname!=default_plotLabelFontname) settings.setValue(g+"plotLabelFontname", plotLabelFontname);
    if (plotLabelFontSize!=default_plotLabelFontSize) settings.setValue(g+"plotLabelFontSize", plotLabelFontSize);

    bool palette_changed=false;
    for (int cnt=0; cnt<manyGraphsColorCount; cnt++) {
        if (manyGraphsColor[cnt]!=default_manyGraphsColor[cnt]) {
            palette_changed=true;
            break;
        }
    }
    if (palette_changed) {
        for (int cnt=0; cnt<manyGraphsColorCount; cnt++) {
            settings.setValue(QString(g+"manygraphscolor%1").arg(cnt), jkqtp_QColor2String(manyGraphsColor[cnt]));
        }
    }
    mathText.saveSettings(settings, g+"math_text.");

    xAxis->saveSettings(settings, g+"xaxis_");
    yAxis->saveSettings(settings, g+"yaxis_");

    saveUserSettings();

}

void JKQTBasePlotter::loadUserSettings(const QSettings &settings, const QString& group) {
    currentSaveDirectory=settings.value(group+"currentSaveDirectory", currentSaveDirectory).toString();
    currentFileFormat=settings.value(group+"currentFileFormat", currentFileFormat).toString();
    currentDataFileFormat=settings.value(group+"currentDataFileFormat", currentFileFormat).toString();
    printZoomFactor=settings.value(group+"printZoomFactor", printZoomFactor).toDouble();
    printSizeX_Millimeter=settings.value(group+"printSizeX", printSizeX_Millimeter).toDouble();
    printSizeY_Millimeter=settings.value(group+"printSizeY", printSizeY_Millimeter).toDouble();
    printMagnification=settings.value(group+"printMagnification", printMagnification).toDouble();
    printSetAbsolutePageSize=settings.value(group+"printSetAbsoluteSize", printSetAbsolutePageSize).toBool();
    printAspect=settings.value(group+"printAspect", printAspect).toDouble();
    fontSizePrintMultiplier=settings.value(group+"fontSizePrintMultiplier", fontSizePrintMultiplier).toDouble();
    lineWidthPrintMultiplier=settings.value(group+"lineWidthPrintMultiplier", lineWidthPrintMultiplier).toDouble();
    printKeepAspect=settings.value(group+"printKeepAspect", printKeepAspect).toBool();
    exportUnitInMM=settings.value(group+"exportUnitInMM", exportUnitInMM).toBool();
    currentPrinter=settings.value(group+"printer", currentPrinter).toString();
    int count=settings.value(group+"selections.count", 0).toInt();
    getDataColumnsByUserSaved.clear();
    for (int i=0; i<count; i++) {
        const QString itg=QString("item%1").arg(i);
        QString n=settings.value(group+"selections."+itg+".name", "").toString();
        QStringList item=settings.value(group+"selections."+itg+".items", QStringList()).toStringList();
        if (!n.isEmpty()) {
            getDataColumnsByUserSaved[n]=item;
        }
    }
}

void JKQTBasePlotter::saveUserSettings(QSettings &settings, const QString& group) const {
    settings.setValue(group+"printer", currentPrinter);
    //qDebug()<<settings.fileName()<<group<<currentSaveDirectory<<QDir(currentSaveDirectory).absolutePath();
    settings.setValue(group+"currentSaveDirectory", QDir(currentSaveDirectory).absolutePath());
    settings.setValue(group+"currentFileFormat",currentFileFormat);
    settings.setValue(group+"currentDataFileFormat",currentDataFileFormat);
    settings.setValue(group+"printZoomFactor", printZoomFactor);
    settings.setValue(group+"printSizeX", printSizeX_Millimeter);
    settings.setValue(group+"printSizeY", printSizeY_Millimeter);
    settings.setValue(group+"printMagnification", printMagnification);
    settings.setValue(group+"printSetAbsoluteSize", printSetAbsolutePageSize);
    settings.setValue(group+"printAspect", printAspect);
    settings.setValue(group+"printKeepAspect", printKeepAspect);
    settings.setValue(group+"fontSizePrintMultiplier", fontSizePrintMultiplier);
    settings.setValue(group+"lineWidthPrintMultiplier", lineWidthPrintMultiplier);
    settings.setValue(group+"exportUnitInMM", exportUnitInMM);
    //settings.setValue(group+"", );

    settings.beginGroup(group+"selections");
    QStringList keys=getDataColumnsByUserSaved.keys();
    settings.setValue("count", keys.size());
    for (int i=0; i<keys.size(); i++) {
        settings.beginGroup(QString("item%1").arg(i));
        settings.setValue("name", keys[i]);
        settings.setValue("items", getDataColumnsByUserSaved[keys[i]]);
        settings.endGroup();
    }
    settings.endGroup();
}

void JKQTBasePlotter::loadSettings(const QSettings &settings, const QString& group){
    QString g=group+"/";
    if (group.isEmpty()) g="";

    aspectRatio=settings.value(g+"apect_ratio", aspectRatio).toDouble();
    maintainAspectRatio=settings.value(g+"maintain_apect_ratio", maintainAspectRatio).toBool();
    axisAspectRatio=settings.value(g+"axis_apect_ratio", axisAspectRatio).toDouble();
    maintainAxisAspectRatio=settings.value(g+"maintain_axis_apect_ratio", maintainAxisAspectRatio).toBool();


    CSVdecimalSeparator=settings.value(g+"csv_decimal_separator", CSVdecimalSeparator).toString();
    CSVcommentInitializer=settings.value(g+"csv_comment_initializer", CSVcommentInitializer).toString();

    plotBorderLeft=settings.value(g+"plot_border_left", plotBorderLeft).toInt();
    plotBorderRight=settings.value(g+"plot_border_right", plotBorderRight).toInt();
    plotBorderTop=settings.value(g+"plot_border_top", plotBorderTop).toInt();
    plotBorderBottom=settings.value(g+"plot_border_bottom", plotBorderBottom).toInt();


    keyXOffset=settings.value(g+"key_xoffset", keyXOffset).toInt();
    keyYOffset=settings.value(g+"key_yoffset", keyYOffset).toInt();
    keyXMargin=settings.value(g+"key_xmargin", keyXMargin).toInt();
    keyYMargin=settings.value(g+"key_ymargin", keyYMargin).toInt();
    keyXSeparation=settings.value(g+"key_xseparation", keyXSeparation).toInt();
    keyYSeparation=settings.value(g+"key_yseparation", keyYSeparation).toInt();
    keyFrameColor=QColor(settings.value(g+"key_frame_color", jkqtp_QColor2String(keyFrameColor)).toString());
    keyFrameWidth=settings.value(g+"key_frame_width", keyFrameWidth).toDouble();
    showKeyFrame=settings.value(g+"show_key_frame", showKeyFrame).toBool();
    keyBackgroundColor=QColor(settings.value(g+"key_background_color", jkqtp_QColor2String(keyBackgroundColor)).toString());
    showKey=settings.value(g+"show_key", showKey).toBool();
    keyPosition=String2JKQTPKeyPosition(settings.value(g+"key_position", JKQTPKeyPosition2String(keyPosition)).toString());
    keyLayout=JKQTPKeyLayoutOneColumn;
    keyLayout=String2JKQTPKeyLayout(settings.value(g+"key_layout", JKQTPKeyLayout2String(keyLayout)).toString());

    useAntiAliasingForSystem=settings.value(g+"use_antialiasing_for_system", useAntiAliasingForSystem).toBool();
    useAntiAliasingForGraphs=settings.value(g+"use_antialiasing_for_graphs", useAntiAliasingForGraphs).toBool();
    useAntiAliasingForText=settings.value(g+"use_antialiasing_for_text", useAntiAliasingForText).toBool();

    backgroundColor=QColor(settings.value(g+"background_color", default_backgroundColor).toString());
    exportBackgroundColor=QColor(settings.value(g+"exportBackgroundColor", default_exportBackgroundColor).toString());
    plotBackgroundColor=QColor(settings.value(g+"plot_background_color", jkqtp_QColor2String(plotBackgroundColor)).toString());
    graphColor=QColor(settings.value(g+"graph_color", jkqtp_QColor2String(graphColor)).toString());
    graphWidth=settings.value(g+"graph_linewidth", graphWidth).toDouble();

    keyFont=settings.value(g+"key_fontname", keyFont).toString();
    keyFontSize=settings.value(g+"key_fontsize", keyFontSize).toInt();
    key_item_width=settings.value(g+"key_item_width", key_item_width).toInt();
    key_item_height=settings.value(g+"key_item_height", key_item_height).toInt();
    key_line_length=settings.value(g+"key_line_width", key_line_length).toInt();
    keyAutosize=settings.value(g+"key_autosize", keyAutosize).toBool();

    plotLabelFontname=settings.value(g+"plotLabelFontname", plotLabelFontname).toString();
    plotLabelFontSize=settings.value(g+"plotLabelFontSize", plotLabelFontSize).toDouble();

    int cnt=0;

    while (settings.contains(QString(g+"manygraphscolor%1").arg(cnt)) && cnt<50) {
        QString col=settings.value(QString(g+"manygraphscolor%1").arg(cnt), "").toString();
        manyGraphsColor[cnt]=QColor(col);
        cnt++;
        //std::cout<<"cnt="<<cnt<<std::endl;
    }
    if (cnt>0) {
        manyGraphsColorCount=cnt;
    }

    mathText.loadSettings(settings, g+"math_text.");
#ifdef USE_XITS_FONTS
    mathText.useXITS();
#endif

    /*for (int i=0; i<manyGraphsColorCount; i++) {
        std::cout<<"manyGraphsColor["<<i<<"] = "<<jkqtp_QColor2String(manyGraphsColor[i]).toStdString()<<std::endl;
        std::cout<<"default_manyGraphsColor["<<i<<"] = "<<jkqtp_QColor2String(default_manyGraphsColor[i]).toStdString()<<std::endl;
    }*/
    xAxis->loadSettings(settings, g+"xaxis_");
    yAxis->loadSettings(settings, g+"yaxis_");

    loadUserSettings();

    if (emitPlotSignals) emit plotUpdated();
}



void JKQTBasePlotter::setXY(double xminn, double xmaxx, double yminn, double ymaxx){
    xAxis->setRange(xminn, xmaxx);
    yAxis->setRange(yminn, ymaxx);
    if (maintainAxisAspectRatio) {
        double mid=(yAxis->getMax()+yAxis->getMin())/2.0;
        double w=fabs(xmaxx-xminn)/axisAspectRatio;
        yAxis->setRange(mid-w/2.0, mid+w/2.0);
    }
    if (emitSignals) emit zoomChangedLocally(xAxis->getMin(), xAxis->getMax(), yAxis->getMin(), yAxis->getMax(), this);
}

void JKQTBasePlotter::setX(double xminn, double xmaxx){
    xAxis->setRange(xminn, xmaxx);
    if (maintainAxisAspectRatio) {
        double mid=(yAxis->getMax()+yAxis->getMin())/2.0;
        double w=fabs(xmaxx-xminn)/axisAspectRatio;
        yAxis->setRange(mid-w/2.0, mid+w/2.0);
    }
    if (emitSignals) emit zoomChangedLocally(xAxis->getMin(), xAxis->getMax(), yAxis->getMin(), yAxis->getMax(), this);
}

void JKQTBasePlotter::setY(double yminn, double ymaxx){
    yAxis->setRange(yminn, ymaxx);
    if (maintainAxisAspectRatio) {
        double mid=(xAxis->getMax()+xAxis->getMin())/2.0;
        double w=fabs(ymaxx-yminn)*axisAspectRatio;
        xAxis->setRange(mid-w/2.0, mid+w/2.0);
    }
    if (emitSignals) emit zoomChangedLocally(xAxis->getMin(), xAxis->getMax(), yAxis->getMin(), yAxis->getMax(), this);
}

void JKQTBasePlotter::setAbsoluteX(double xminn, double xmaxx) {
    xAxis->setAbsoluteRange(xminn, xmaxx);
    if (emitSignals) emit zoomChangedLocally(xAxis->getMin(), xAxis->getMax(), yAxis->getMin(), yAxis->getMax(), this);
}

void JKQTBasePlotter::setAbsoluteY(double yminn, double ymaxx) {
    yAxis->setAbsoluteRange(yminn, ymaxx);
    if (emitSignals) emit zoomChangedLocally(xAxis->getMin(), xAxis->getMax(), yAxis->getMin(), yAxis->getMax(), this);
}

void JKQTBasePlotter::setAbsoluteXY(double xminn, double xmaxx, double yminn, double ymaxx) {
    xAxis->setAbsoluteRange(xminn, xmaxx);
    yAxis->setAbsoluteRange(yminn, ymaxx);
    if (emitSignals) emit zoomChangedLocally(xAxis->getMin(), xAxis->getMax(), yAxis->getMin(), yAxis->getMax(), this);
}

void JKQTBasePlotter::calcPlotScaling(JKQTPEnhancedPainter& painter){
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTBasePlotter[%1]::calcPlotScaling()").arg(objectName()));
#endif

    //qDebug()<<"this="<<this<<"  --> calcPlotScaling()";

    if (emitSignals) emit beforePlotScalingRecalculate();
    //qDebug()<<"start JKQTBasePlotter::calcPlotScaling";
    // if the key is plotted outside , then we have to add place for it (i.e. change the plotBorders
    iplotBorderBottom=plotBorderBottom;
    iplotBorderLeft=plotBorderLeft;
    iplotBorderRight=plotBorderRight;
    iplotBorderTop=plotBorderTop;
    iTitleHeight=0;


    if (!plotLabel.isEmpty()) {
        /*JKQTMathText mt;
        mt.setFontSize(plotLabelFontSize*fontSizeMultiplier);
        mt.setFontRoman(plotLabelFontname);
#ifdef USE_XITS_FONTS
        mt.useXITS();
#endif
        mt.parse(plotLabel);
        QSizeF s=mt.getSize(painter);*/
        QSizeF s=getTextSizeSize(plotLabelFontname, plotLabelFontSize*fontSizeMultiplier, plotLabel, painter);
        iplotBorderTop+=s.height()*1.2;
        iTitleHeight=s.height()*1.2;

    }

    double keyWidth, keyHeight;
    QFont f=painter.font();
    f.setFamily(keyFont);
    f.setPointSizeF(keyFontSize*fontSizeMultiplier);
    QFontMetricsF kfm(f);
    getKeyExtent(painter, &keyWidth, &keyHeight);
    iplotKeyBorderTop=0;
    iplotKeyBorderBottom=0;
    iplotKeyBorderLeft=0;
    iplotKeyBorderRight=0;
    if (keyPosition==JKQTPKeyOutsideTopRight) {
        iplotKeyBorderTop=keyHeight+2*keyYMargin*kfm.width('X')+ceil(2*keyFrameWidth)+keyYOffset*kfm.width('X')+2;
        iplotBorderTop = iplotBorderTop + iplotKeyBorderTop;
    } else if (keyPosition==JKQTPKeyOutsideTopLeft) {
        iplotKeyBorderTop=keyHeight+2*keyYMargin*kfm.width('X')+ceil(2*keyFrameWidth)+keyYOffset*kfm.width('X')+2;
        iplotBorderTop = iplotBorderTop + iplotKeyBorderTop;
    } else if (keyPosition==JKQTPKeyOutsideLeftTop) {
        iplotKeyBorderLeft=keyWidth+2*keyXMargin*kfm.width('X')+ceil(2*keyFrameWidth)+keyXOffset*kfm.width('X')+2;
        iplotBorderLeft = iplotBorderLeft + iplotKeyBorderLeft;
    } else if (keyPosition==JKQTPKeyOutsideLeftBottom) {
        iplotKeyBorderLeft=keyWidth+2*keyXMargin*kfm.width('X')+ceil(2*keyFrameWidth)+keyXOffset*kfm.width('X')+2;
        iplotBorderLeft = iplotBorderLeft + iplotKeyBorderLeft;
    } else if (keyPosition==JKQTPKeyOutsideBottomRight) {
        iplotKeyBorderBottom=keyHeight+2*keyYMargin*kfm.width('X')+ceil(2*keyFrameWidth)+keyYOffset*kfm.width('X')+2;
        iplotBorderBottom = iplotBorderBottom + iplotKeyBorderBottom;
    } else if (keyPosition==JKQTPKeyOutsideBottomLeft) {
        iplotKeyBorderBottom=keyHeight+2*keyYMargin*kfm.width('X')+ceil(2*keyFrameWidth)+keyYOffset*kfm.width('X')+2;
        iplotBorderBottom = iplotBorderBottom + iplotKeyBorderBottom;
    } else if (keyPosition==JKQTPKeyOutsideRightTop) {
        iplotKeyBorderRight = keyWidth+2*keyXMargin*kfm.width('X')+ceil(2*keyFrameWidth)+keyXOffset*kfm.width('X')+2;
        iplotBorderRight = iplotBorderRight + iplotKeyBorderRight;
    } else if (keyPosition==JKQTPKeyOutsideRightBottom) {
        iplotKeyBorderRight = keyWidth+2*keyXMargin*kfm.width('X')+ceil(2*keyFrameWidth)+keyXOffset*kfm.width('X')+2;
        iplotBorderRight = iplotBorderRight + iplotKeyBorderRight;
    }



    /*if (displayMousePosition) {
        QFontMetrics fm=fontMetrics();
        QString test="�Aquator";
        int labelHeight=fm.size(Qt::TextSingleLine, test).height()*1.5;
        //if (mousePosLabel!=nullptr) labelHeight=mousePosLabel->height();
        iplotBorderTop=iplotBorderTop+(labelHeight-plotBorderTop)*1.1;
    }*/

    // read additional size required for coordinate axes
    QSizeF s=xAxis->getSize1(painter);
    iplotBorderBottom+=s.height();
    s=xAxis->getSize2(painter);
    iplotBorderTop+=s.height();

    s=yAxis->getSize1(painter);
    iplotBorderLeft+=s.width();
    s=yAxis->getSize2(painter);
    iplotBorderRight+=s.width();

    iplotBorderTop_nographs=iplotBorderTop;
    iplotBorderLeft_nographs=iplotBorderLeft;
    iplotBorderBottom_nographs=iplotBorderBottom;
    iplotBorderRight_nographs=iplotBorderRight;





    // read additional space required by graphs
    for (int i=0; i<graphs.size(); i++) {
        if (graphs[i]->isVisible()) {
            int leftSpace, rightSpace, topSpace, bottomSpace;
            graphs[i]->getOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
            iplotBorderBottom+=bottomSpace;
            iplotBorderTop+=topSpace;
            iplotBorderLeft+=leftSpace;
            iplotBorderRight+=rightSpace;
        }
    }

    //qDebug()<<"  end JKQTBasePlotter::calcPlotScaling";


    // synchronize to a master-plotter
    if (masterPlotterX) {
        if (masterSynchronizeWidth) {
            iplotBorderLeft=masterPlotterX->iplotBorderLeft;
            iplotBorderRight=masterPlotterX->iplotBorderRight;
        }
    }
    if (masterPlotterY) {
        if (masterSynchronizeHeight) {
            iplotBorderTop=masterPlotterY->iplotBorderTop;
            iplotBorderBottom=masterPlotterY->iplotBorderBottom;
        }
    }

    // first we calculate the width and height of the plot from the widget dimensions and
    // the supplied border sizes
    plotWidth=widgetWidth/paintMagnification-iplotBorderLeft-iplotBorderRight;
    plotHeight=widgetHeight/paintMagnification-iplotBorderTop-iplotBorderBottom;


    ////////////////////////////////////////////////////////////////////
    // ENSURE ASPECT RATIO (if activated)
    ////////////////////////////////////////////////////////////////////
    if (maintainAspectRatio && (!xAxis->isLogAxis()) && (!xAxis->isLogAxis())) {
        double currRatio=static_cast<double>(plotWidth)/static_cast<double>(plotHeight);
        double newPlotWidth=plotWidth;
        double newPlotHeight=plotHeight;
        double dx=0;
        double dy=0;
        if (currRatio!=aspectRatio) {
            if (aspectRatio>=currRatio) {
                newPlotWidth=aspectRatio*static_cast<double>(plotHeight);
            } else {
                newPlotHeight=static_cast<double>(plotWidth)/aspectRatio;
            }
            dx=plotWidth-newPlotWidth;
            dy=plotHeight-newPlotHeight;
            if (dx<0) {
                newPlotWidth=plotWidth;
                newPlotHeight=static_cast<double>(plotWidth)/aspectRatio;
            } else if (dy<0) {
                newPlotWidth=aspectRatio*static_cast<double>(plotHeight);
                newPlotHeight=plotHeight;
            }
            dx=plotWidth-newPlotWidth;
            dy=plotHeight-newPlotHeight;
            if ((dx<0)||(dy<0)) {
                newPlotWidth=plotWidth;
                newPlotHeight=plotHeight;
            }
        }
        //if (newPlotWidth>widgetWidth-iplotBorderLeft-iplotBorderRight) newPlotWidth=widgetWidth-iplotBorderLeft-iplotBorderRight;
        //if (newPlotHeight>widgetHeight-iplotBorderTop-iplotBorderBottom) newPlotHeight=widgetHeight-iplotBorderTop-iplotBorderBottom;
        dx=plotWidth-newPlotWidth;
        dy=plotHeight-newPlotHeight;
        iplotBorderBottom+=dy/2.0;
        iplotBorderTop+=dy/2.0;
        iplotBorderLeft+=dx/2.0;
        iplotBorderRight+=dx/2.0;
        plotWidth=widgetWidth/paintMagnification-iplotBorderLeft-iplotBorderRight;
        plotHeight=widgetHeight/paintMagnification-iplotBorderTop-iplotBorderBottom;
    }


    xAxis->calcPlotScaling(true);
    yAxis->calcPlotScaling(true);

    ////////////////////////////////////////////////////////////////////
    // ENSURE ASPECT RATIO OF PLOT 1x1 PIXELS (if activated)
    ////////////////////////////////////////////////////////////////////
    if (maintainAxisAspectRatio && (!xAxis->isLogAxis()) && (!xAxis->isLogAxis())) {
        double cplotWidth=fabs(xAxis->getMax()-xAxis->getMin());
        double newPlotWidth=cplotWidth;
        double xmid=(xAxis->getMax()+xAxis->getMin())/2.0;
        double cplotHeight=fabs(yAxis->getMax()-yAxis->getMin());
        double newPlotHeight=cplotHeight;
        double ymid=(yAxis->getMax()+yAxis->getMin())/2.0;
        double currRatio=fabs(cplotWidth/cplotHeight);
        if (currRatio!=axisAspectRatio) {
            //if (axisAspectRatio<=currRatio) {
                newPlotWidth=axisAspectRatio*cplotHeight;
            //} else {
            //    newPlotHeight=cplotWidth/axisAspectRatio;
            //}
        }
        xAxis->setRange(xmid-newPlotWidth/2.0, xmid+newPlotWidth/2.0);
        yAxis->setRange(ymid-newPlotHeight/2.0, ymid+newPlotHeight/2.0);
        xAxis->calcPlotScaling(true);
        yAxis->calcPlotScaling(true);
    }


    //if (emitPlotSignals) emit plotScalingRecalculated();
    emit plotScalingRecalculated();
}


void JKQTBasePlotter::plotSystemGrid(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTBasePlotter::plotSystemGrid");
#endif
    //qDebug()<<"start JKQTBasePlotter::plotSystemGrid";
    xAxis->drawGrids(painter);
    yAxis->drawGrids(painter);
    //qDebug()<<"  end JKQTBasePlotter::plotSystemGrid";
}

void JKQTBasePlotter::plotSystemXAxis(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTBasePlotter::plotSystemXAxis");
#endif
    //qDebug()<<"start JKQTBasePlotter::plotSystemXAxis";
    xAxis->drawAxes(painter);
    //qDebug()<<"  end JKQTBasePlotter::plotSystemXAxis";
}

void JKQTBasePlotter::plotSystemYAxis(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTBasePlotter::plotSystemYAxis");
#endif
    //qDebug()<<"start JKQTBasePlotter::plotSystemYAxis";
    yAxis->drawAxes(painter);
    //qDebug()<<"  end JKQTBasePlotter::plotSystemYAxis";
}





JKQTBasePlotter::JKQTPPen JKQTBasePlotter::getPlotStyle(int i) const{
    int colorI=-1;
    int styleI=0;
    for (int k=0; k<=i; k++) {
        colorI++;
        if (colorI>=manyGraphsColorCount) {
            styleI++;
            colorI=0;
            if (styleI>=5) styleI=0;
        }
    }
    JKQTPPen p;
    //std::cout<<"plotstyle "<<i<<std::endl;
    //std::cout<<"color "<<colorI<<std::endl;
    //std::cout<<"style "<<styleI<<std::endl;
    p.setColor(manyGraphsColor[colorI]);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, graphWidth));
    p.setStyle(manyGraphsStyle[styleI]);
    return p;
}

int JKQTBasePlotter::getNextStyle() {
    int res=0;
    while (usedStyles.contains(res)) res++;
    //std::cout<<res<<std::endl;
    usedStyles.push_back(res);
    return res;
};

void JKQTBasePlotter::plotKey(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTBasePlotter::plotKey");
#endif
    //qDebug()<<"start JKQTBasePlotter::plotKey";

    QFont kf(keyFont, keyFontSize*fontSizeMultiplier);
    QFontMetricsF kfm(kf);

    // get the size of the key and if keyWidth>0 && keyHeight>0 draw the frame and the contents
    double keyWidth=0;
    double keyHeight=0;
    getKeyExtent(painter, &keyWidth, &keyHeight);
    double keyrWidth=keyWidth+2.0*keyXMargin*kfm.width('X')+2.0*keyFrameWidth*lineWidthPrintMultiplier;
    double keyrHeight=keyHeight+2.0*keyYMargin*kfm.width('X')+2.0*keyFrameWidth*lineWidthPrintMultiplier;

    if ((keyWidth>0) && (keyHeight>0)) {
        // key position
        double x,y;
        switch(keyPosition) {
            case JKQTPKeyOutsideTopRight:
                x=iplotBorderLeft+plotWidth+iplotBorderRight-keyrWidth-keyXOffset*kfm.width('X');
                y=keyYOffset*kfm.width('X')+iTitleHeight;
                break;

            case JKQTPKeyOutsideTopLeft:
                x=iplotBorderLeft+keyXOffset*kfm.width('X');
                y=keyYOffset*kfm.width('X')+iTitleHeight;
                break;

            case JKQTPKeyOutsideBottomRight:
                x=iplotBorderLeft+plotWidth-keyrWidth-keyXOffset*kfm.width('X');
                y=iplotBorderTop+plotHeight+iplotBorderBottom-keyrHeight-keyYOffset*kfm.width('X');
                break;
            case JKQTPKeyOutsideBottomLeft:
                x=iplotBorderLeft+keyXOffset*kfm.width('X');
                y=iplotBorderTop+plotHeight+iplotBorderBottom-keyrHeight-keyYOffset*kfm.width('X');
                break;



            case JKQTPKeyOutsideRightTop:
                x=iplotBorderLeft+plotWidth+iplotBorderRight-keyrWidth-keyXOffset*kfm.width('X');
                y=iplotBorderTop+keyYOffset*kfm.width('X');
                break;
            case JKQTPKeyOutsideRightBottom:
                x=iplotBorderLeft+plotWidth+iplotBorderRight-keyrWidth-keyXOffset*kfm.width('X');
                y=iplotBorderTop+plotHeight-keyrHeight-keyYOffset*kfm.width('X');
                break;

            case JKQTPKeyOutsideLeftTop:
                x=keyXOffset*kfm.width('X');
                y=iplotBorderTop+keyYOffset*kfm.width('X');
                break;
            case JKQTPKeyOutsideLeftBottom:
                x=keyXOffset*kfm.width('X');
                y=iplotBorderTop+plotHeight-keyrHeight-keyYOffset*kfm.width('X');
                break;




            case JKQTPKeyInsideBottomRight:
                x=iplotBorderLeft+plotWidth-keyrWidth-keyXOffset*kfm.width('X');
                y=iplotBorderTop+plotHeight-keyrHeight-keyYOffset;
                break;
            case JKQTPKeyInsideBottomLeft:
                x=iplotBorderLeft+keyXOffset*kfm.width('X');
                y=iplotBorderTop+plotHeight-keyrHeight-keyYOffset*kfm.width('X');
                break;
            case JKQTPKeyInsideTopLeft:
                x=iplotBorderLeft+keyXOffset*kfm.width('X');
                y=iplotBorderTop+keyYOffset*kfm.width('X');
                break;
            case JKQTPKeyInsideTopRight:
            default:
                x=iplotBorderLeft+plotWidth-keyrWidth-keyXOffset*kfm.width('X');
                y=iplotBorderTop+keyYOffset*kfm.width('X');
                break;
        }

        // save old brushes and pens
        painter.save();
        QPen pf=painter.pen();
        QBrush bf=painter.brush();
        pf.setColor(keyFrameColor);
        pf.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, pt2px(painter, keyFrameWidth*lineWidthMultiplier)));
        pf.setStyle(Qt::SolidLine);
        bf.setColor(keyBackgroundColor);
        bf.setStyle(Qt::SolidPattern);
        painter.setBrush(bf);
        if (!showKeyFrame) {
            QPen pff=pf;
            pff.setColor(keyBackgroundColor);
            pff.setWidthF(JKQTPLOTTER_ABS_MIN_LINEWIDTH);
            painter.setPen(pff);
        } else {
            painter.setPen(pf);
        }


        painter.drawRect(QRectF(x,y,keyrWidth, keyrHeight));
        y=y+keyYMargin*kfm.width('X')+keyFrameWidth*lineWidthMultiplier/2.0;
        x=x+keyXMargin*kfm.width('X')+keyFrameWidth*lineWidthMultiplier/2.0;

        painter.setPen(pf);

        if (useClipping) {
            QRegion cregion(x,y,keyrWidth, keyrHeight);
            painter.setClipping(true);
            painter.setClipRegion(cregion);
            painter.setClipping(true);
        }

        plotKeyContents(painter, x, y, keyWidth, keyHeight);

        if (useClipping) painter.setClipping(false);
        painter.restore();

    }
    //qDebug()<<"  end JKQTBasePlotter::plotKey";
}

void JKQTBasePlotter::plotOverlays(JKQTPEnhancedPainter &painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTBasePlotter::plotOverlays");
#endif
    if (overlays.isEmpty()) return;
    if (useClipping) {
        QRegion cregion(iplotBorderLeft, iplotBorderTop, plotWidth, plotHeight);
        painter.setClipping(true);
        painter.setClipRegion(cregion);
    }

    for (int j=0; j<overlays.size(); j++) {
        JKQTPOverlayElement* g=overlays[j];
        if (g->isVisible()) g->draw(painter);
    }

    if (useClipping) {
        painter.setClipping(false);
    }
}



void JKQTBasePlotter::paintPlot(JKQTPEnhancedPainter& painter, bool drawOverlays) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTBasePlotter::paintPlot");
#endif
    //qDebug()<<"start JKQTBasePlotter::paintPlot";
    // draw background
    painter.save();
    painter.setPen(backgroundColor);
    if (backgroundColor!=Qt::transparent) painter.fillRect(QRectF(0,0,widgetWidth/paintMagnification, widgetHeight/paintMagnification), QBrush(backgroundColor));
    painter.fillRect(QRectF(iplotBorderLeft, iplotBorderTop, plotWidth, plotHeight), QBrush(plotBackgroundColor));
    painter.restore();
    painter.setRenderHint(JKQTPEnhancedPainter::NonCosmeticDefaultPen, true);
    painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing, useAntiAliasingForSystem);
    painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing, useAntiAliasingForText);
    plotSystemGrid(painter);

    if (!plotLabel.isEmpty()) {
        mathText.setFontSize(plotLabelFontSize*fontSizeMultiplier);
        mathText.setFontRoman(plotLabelFontname);

        mathText.parse(plotLabel);
        double a=0,d=0,so=0,w=0;
        getTextSizeDetail(plotLabelFontname,plotLabelFontSize*fontSizeMultiplier,plotLabel, painter, w, a, d, so);
        QSizeF s=QSizeF(w, a+d);
        mathText.draw(painter, iplotBorderLeft+(plotWidth-s.width())/2.0,plotBorderTop+a*1.2);
    }

    painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing, useAntiAliasingForGraphs);
    painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing, useAntiAliasingForText);
    painter.save();
    plotGraphs(painter);
    painter.restore();
    painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing, useAntiAliasingForSystem);
    painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing, useAntiAliasingForText);

    plotSystemXAxis(painter);
    plotSystemYAxis(painter);
    painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing, useAntiAliasingForGraphs);
    painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing, useAntiAliasingForText);
    if (showKey) plotKey(painter);
    painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing, useAntiAliasingForText);
    if (drawOverlays) plotOverlays(painter);
    //qDebug()<<"  end JKQTBasePlotter::paintPlot";
}

void JKQTBasePlotter::paintOverlays(JKQTPEnhancedPainter &painter) {
    painter.setRenderHint(JKQTPEnhancedPainter::NonCosmeticDefaultPen, true);
    painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing, useAntiAliasingForGraphs);
    painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing, useAntiAliasingForText);
    painter.save();
    plotOverlays(painter);
    painter.restore();
}

void JKQTBasePlotter::gridPrintingCalc() {
    gridPrintingRows.clear();
    gridPrintingColumns.clear();
    if (!gridPrinting) {
        gridPrintingSize=QSize(widgetWidth/paintMagnification, widgetHeight/paintMagnification);
        gridPrintingRows.push_back(widgetHeight/paintMagnification);
        gridPrintingColumns.push_back(widgetWidth/paintMagnification);
    } else {
        // first find min and max columns/rows
        size_t cmin=gridPrintingCurrentX;
        size_t cmax=gridPrintingCurrentX;
        size_t rmin=gridPrintingCurrentY;
        size_t rmax=gridPrintingCurrentY;
        for (int i=0; i< gridPrintingList.size(); i++) {
            size_t c=gridPrintingList[i].x;
            size_t r=gridPrintingList[i].y;
            if (c<cmin) cmin=c;
            if (c>cmax) cmax=c;
            if (r<rmin) rmin=r;
            if (r>rmax) rmax=r;
        }
        // fill gridPrintingRows and gridPrintingColumns
        for (size_t i=0; i<=cmax; i++) { gridPrintingColumns.push_back(0); }
        for (size_t i=0; i<=rmax; i++) { gridPrintingRows.push_back(0); }
        gridPrintingColumns[gridPrintingCurrentX]=widgetWidth/paintMagnification;
        gridPrintingRows[gridPrintingCurrentY]=widgetHeight/paintMagnification;
        for (int i=0; i< gridPrintingList.size(); i++) {
            int cw=gridPrintingList[i].plotter->widgetWidth/paintMagnification;
            int ch=gridPrintingList[i].plotter->widgetHeight/paintMagnification;
            size_t c=gridPrintingList[i].x;
            size_t r=gridPrintingList[i].y;
            if (cw>gridPrintingColumns[c]) gridPrintingColumns[c]=cw;
            if (ch>gridPrintingRows[r]) gridPrintingRows[r]=ch;
        }
        int w=0;
        int h=0;
        for (int i=0; i<gridPrintingColumns.size(); i++) { w+=gridPrintingColumns[i]; };
        for (int i=0; i<gridPrintingRows.size(); i++) { h+=gridPrintingRows[i]; };
        gridPrintingSize=QSize(w, h);
    }
}

void JKQTBasePlotter::gridPaint(JKQTPEnhancedPainter& painter, QSizeF pageRect, bool drawOverlays, bool scaleIfTooLarge, bool scaleIfTooSmall) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTBasePlotter::gridPaint");
#endif
    calcPlotScaling(painter);

    if (!gridPrinting) {
        double scale=static_cast<double>(pageRect.width())/static_cast<double>(widgetWidth)*paintMagnification;
        if (/*(scale*static_cast<double>(widgetWidth)>static_cast<double>(pageRect.width())) ||*/ (scale*static_cast<double>(widgetHeight)/paintMagnification>static_cast<double>(pageRect.height())) && scaleIfTooLarge) {
            scale=static_cast<double>(pageRect.height())/static_cast<double>(widgetHeight)*paintMagnification;
        }
        if (!scaleIfTooSmall && scale>1.0) scale=1.0;
        if (!scaleIfTooLarge && !scaleIfTooSmall) scale=1.0;
#ifdef SHOW_JKQTPLOTTER_DEBUG
        qDebug()<<"gridPaint: scale="<<scale;
#endif
        painter.save();
        // scale the plot so it fits on the page
        painter.scale(scale, scale);
        paintPlot(painter, drawOverlays);
        painter.restore();
    } else {

        QList<double> fsm, lwm, pm;
        QList<QColor> backg;
        for (int i=0; i< gridPrintingList.size(); i++) {
            fsm.append(gridPrintingList[i].plotter->getFontSizeMultiplier());
            lwm.append(gridPrintingList[i].plotter->getLineWidthMultiplier());
            pm.append(gridPrintingList[i].plotter->getPaintMagnification());
            backg.append(gridPrintingList[i].plotter->getExportBackgroundColor());
            gridPrintingList[i].plotter->setFontSizeMultiplier(fontSizeMultiplier);
            gridPrintingList[i].plotter->setLineWidthMultiplier(lineWidthMultiplier);
            gridPrintingList[i].plotter->setPaintMagnification(paintMagnification);
            gridPrintingList[i].plotter->setBackgroundColor(gridPrintingList[i].plotter->getExportBackgroundColor());
            gridPrintingList[i].plotter->calcPlotScaling(painter);
        }
        gridPrintingCalc(); // ensure the grid plot has been calculated
        // scale the plot so it fits on the page
        double scale=static_cast<double>(pageRect.width())/static_cast<double>(gridPrintingSize.width());
        if (/*(scale*static_cast<double>(gridPrintingSize.width())>static_cast<double>(pageRect.width())) ||*/ (scale*static_cast<double>(gridPrintingSize.height())>static_cast<double>(pageRect.height())) && scaleIfTooLarge) {
            scale=static_cast<double>(pageRect.height())/static_cast<double>(gridPrintingSize.height());
        }
        if (!scaleIfTooSmall && scale>1.0) scale=1.0;
        if (!scaleIfTooLarge && !scaleIfTooSmall) scale=1.0;
#ifdef SHOW_JKQTPLOTTER_DEBUG
        qDebug()<<"gridPaint: scale="<<scale;
#endif
        painter.save();
        painter.scale(scale, scale);


        // plot this plotter
        painter.save();
        int t_x=0;
        int t_y=0;
        //std::cout<<"printing this ...\n";
        for (size_t i=0; i<gridPrintingCurrentX; i++) { t_x+=gridPrintingColumns[i]; }
        for (size_t i=0; i<gridPrintingCurrentY; i++) { t_y+=gridPrintingRows[i]; }
        //std::cout<<"printing this @ "<<t_x<<", "<<t_y<<" ...\n";
        painter.translate(t_x, t_y);
        paintPlot(painter, drawOverlays);
        painter.restore();
        //std::cout<<"this printed ...\n";

        // plot all the other plotters
        for (int i=0; i< gridPrintingList.size(); i++) {
            //std::cout<<"printing "<<i<<" ...\n";
            painter.save();
            int t_x=0;
            int t_y=0;
            //std::cout<<"printing "<<i<<" @g "<<gridPrintingList[i].x<<", "<<gridPrintingList[i].y<<" ...\n";
            //std::cout<<"colrowlistsizes     "<<gridPrintingColumns.size()<<", "<<gridPrintingRows.size()<<" ...\n";
            for (int j=0; j<gridPrintingList[i].x; j++) {  t_x+=gridPrintingColumns[j];  }
            for (int j=0; j<gridPrintingList[i].y; j++) {  t_y+=gridPrintingRows[j]; }
            //std::cout<<"printing "<<i<<" @ "<<t_x<<", "<<t_y<<" ...\n";
            painter.translate(t_x, t_y);
            gridPrintingList[i].plotter->paintPlot(painter, drawOverlays);
            painter.restore();
        }


        painter.restore();
        for (int i=0; i< gridPrintingList.size(); i++) {
            gridPrintingList[i].plotter->setFontSizeMultiplier(fsm[i]);
            gridPrintingList[i].plotter->setLineWidthMultiplier(lwm[i]);
            gridPrintingList[i].plotter->setPaintMagnification(pm[i]);
            gridPrintingList[i].plotter->setBackgroundColor(backg[i]);
            gridPrintingList[i].plotter->redrawPlot();
        }

    }
}


void JKQTBasePlotter::gridPaintOverlays(JKQTPEnhancedPainter &painter, QSizeF pageRect)
{
    if (!gridPrinting) {
        double scale=static_cast<double>(pageRect.width())/static_cast<double>(widgetWidth)*paintMagnification;
        if (/*(scale*static_cast<double>(widgetWidth)/paintMagnification>static_cast<double>(pageRect.width())) ||*/ (scale*static_cast<double>(widgetHeight)>static_cast<double>(pageRect.height()))) {
            scale=static_cast<double>(pageRect.height())/static_cast<double>(widgetHeight);
        }
        painter.save();
        // scale the plot so it fits on the page
        painter.scale(scale, scale);
        paintOverlays(painter);
        painter.restore();
    } else {
        gridPrintingCalc(); // ensure the grid plot has been calculated
        // scale the plot so it fits on the page
        double scale=static_cast<double>(pageRect.width())/static_cast<double>(gridPrintingSize.width());
        if (/*(scale*static_cast<double>(gridPrintingSize.width())>static_cast<double>(pageRect.width())) ||*/ (scale*static_cast<double>(gridPrintingSize.height())>static_cast<double>(pageRect.height()))) {
            scale=static_cast<double>(pageRect.height())/static_cast<double>(gridPrintingSize.height());
        }
        painter.save();
        painter.scale(scale, scale);


        // plot this plotter
        painter.save();
        int t_x=0;
        int t_y=0;
        //std::cout<<"printing this ...\n";
        for (size_t i=0; i<gridPrintingCurrentX; i++) { t_x+=gridPrintingColumns[i]; }
        for (size_t i=0; i<gridPrintingCurrentY; i++) { t_y+=gridPrintingRows[i]; }
        //std::cout<<"printing this @ "<<t_x<<", "<<t_y<<" ...\n";
        painter.translate(t_x, t_y);
        paintOverlays(painter);
        painter.restore();
        //std::cout<<"this printed ...\n";

        // plot all the other plotters
        for (int i=0; i< gridPrintingList.size(); i++) {
            //std::cout<<"printing "<<i<<" ...\n";
            painter.save();
            int t_x=0;
            int t_y=0;
            //std::cout<<"printing "<<i<<" @g "<<gridPrintingList[i].x<<", "<<gridPrintingList[i].y<<" ...\n";
            //std::cout<<"colrowlistsizes     "<<gridPrintingColumns.size()<<", "<<gridPrintingRows.size()<<" ...\n";
            for (int j=0; j<gridPrintingList[i].x; j++) {  t_x+=gridPrintingColumns[j];  }
            for (int j=0; j<gridPrintingList[i].y; j++) {  t_y+=gridPrintingRows[j]; }
            //std::cout<<"printing "<<i<<" @ "<<t_x<<", "<<t_y<<" ...\n";
            painter.translate(t_x, t_y);
            gridPrintingList[i].plotter->paintOverlays(painter);
            painter.restore();
        }


        painter.restore();

    }
}


void JKQTBasePlotter::print(QPrinter* printer, bool displayPreview) {
    loadUserSettings();
    QPrinter* p=printer;
    bool delP=false;


    // select a printer
    if (p==nullptr) {
        p=new QPrinter();
        p->setPrinterName(currentPrinter);
        delP=true;

        QPrintDialog *dialog = new QPrintDialog(p, nullptr);
        dialog->setWindowTitle(tr("Print Plot"));
        if (dialog->exec() != QDialog::Accepted) {
            delete p;
			delete dialog;
            return;
        }
        currentPrinter=p->printerName();
		delete dialog;
    }

    p->setPageMargins(10,10,10,10,QPrinter::Millimeter);

    if (widgetWidth>widgetHeight) {
        p->setOrientation(QPrinter::Landscape);
    } else {
        p->setOrientation(QPrinter::Portrait);
    }

    printpreviewNew(p, false, -1.0, -1.0, displayPreview);

    if (delP) delete p;
    saveUserSettings();
}


void JKQTBasePlotter::printpreview(QPrinter *p, bool setabsolutesize) {
    double lw=lineWidthMultiplier;
    double fs=fontSizeMultiplier;
    QColor bc=backgroundColor;
    backgroundColor=exportBackgroundColor;
    lineWidthMultiplier=lineWidthPrintMultiplier;
    fontSizeMultiplier=fontSizePrintMultiplier;
    exportPreviewLabel=nullptr;
    printSizeX_Millimeter=widgetWidth;
    printSizeY_Millimeter=widgetHeight;
    printAspect=1;
    printKeepAspect=false;

    printSetAbsolutePageSize=setabsolutesize;
    printKeepAbsoluteFontSizes=false;
    printScaleToPagesize=true;
    QDialog* dlg=new QDialog(nullptr, Qt::WindowMinMaxButtonsHint);
    dlg->setSizeGripEnabled(true);
    //printZoomFactor=0.95;
    //printMagnification=1.5;
    QGridLayout* layout=new QGridLayout();
    dlg->setLayout(layout);
    dlg->setWindowTitle(tr("Graph print/export preview ..."));
    dlg->setWindowIcon(QIcon(":/JKQTPlotter/jkqtp_exportprintpreview.png"));
    printPreview=new QPrintPreviewWidget(p, dlg);
    connect(printPreview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printpreviewPaintRequested(QPrinter*)));

    if (!setabsolutesize) {
        JKQTPEnhancedDoubleSpinBox* spinZoom=new JKQTPEnhancedDoubleSpinBox(dlg);
        spinZoom->setRange(0.0001,10000);
        spinZoom->setValue(printZoomFactor*100.0);
        spinZoom->setSingleStep(5);
        spinZoom->setDecimals(0);
        spinZoom->setSuffix(tr(" %"));
        spinZoom->setToolTip(tr("use this setting to scale the plot)"));
        connect(spinZoom, SIGNAL(editingFinished(double)), this, SLOT(printpreviewSetZoom(double)));
        layout->addWidget(new QLabel(tr("size on paper: ")), 0,layout->columnCount());
        layout->addWidget(spinZoom, 0,layout->columnCount());
    } else {
        printSizeX_Millimeter=100;
        printAspect=double(widgetHeight)/double(widgetWidth);

        if (gridPrinting) {
            gridPrintingCalc();
            printAspect=double(gridPrintingSize.height())/double(gridPrintingSize.width());
        }

        printSizeY_Millimeter=printSizeX_Millimeter*printAspect;
        printKeepAspect=true;

        JKQTPEnhancedDoubleSpinBox* spinSizeX=new JKQTPEnhancedDoubleSpinBox(dlg);
        spinSizeX->setRange(1,10000);
        spinSizeX->setValue(printSizeX_Millimeter);
        spinSizeX->setSingleStep(10);
        spinSizeX->setDecimals(1);
        spinSizeX->setSuffix(tr(" mm"));
        spinSizeX->setToolTip(tr("set the size of the output page"));
        connect(spinSizeX, SIGNAL(editingFinished(double)), this, SLOT(printpreviewSetSizeX(double)));
        spinSizeY=new JKQTPEnhancedDoubleSpinBox(dlg);
        spinSizeY->setRange(1,10000);
        spinSizeY->setValue(printSizeY_Millimeter);
        spinSizeY->setSingleStep(10);
        spinSizeY->setDecimals(1);
        spinSizeY->setSuffix(tr(" mm"));
        spinSizeY->setToolTip(tr("set the size of the output page"));
        spinSizeY->setEnabled(false);
        connect(spinSizeY, SIGNAL(valueChanged(double)), this, SLOT(printpreviewSetSizeY(double)));
        QCheckBox* chkAspect=new QCheckBox(tr("keep aspect ratio"), dlg);
        chkAspect->setChecked(true);
        chkAspect->setToolTip(tr("choose whether to keep the aspect ratio for the print/export, as on the screen."));
        connect(chkAspect, SIGNAL(toggled(bool)), spinSizeY, SLOT(setDisabled(bool)));
        connect(chkAspect, SIGNAL(toggled(bool)), this, SLOT(printpreviewSetAspectRatio(bool)));

        layout->addWidget(new QLabel(tr("page size: ")), 0,layout->columnCount());
        layout->addWidget(spinSizeX, 0,layout->columnCount());
        layout->addWidget(new QLabel(tr(" x ")), 0,layout->columnCount());
        layout->addWidget(spinSizeY, 0,layout->columnCount());
        layout->addWidget(chkAspect, 0,layout->columnCount());
    }

    JKQTPEnhancedDoubleSpinBox* spinMagnification=new JKQTPEnhancedDoubleSpinBox(dlg);
    spinMagnification->setRange(1,1000);
    spinMagnification->setValue(printMagnification*100.0);
    spinMagnification->setSingleStep(10);
    spinMagnification->setDecimals(0);
    spinMagnification->setSuffix(tr(" %"));
    spinMagnification->setToolTip(tr("use this to change the base size of the plot<br>"
                                     "This will not change the size of the plot on<br>"
                                     "the page, only it's appearance. A magn. of 100%<br>"
                                     "will print the plot in the same proportions as<br>"
                                     "on the screen, whereas 50% will use twice as much<br>"
                                     "space for the plot, as on the screen. <i>This mainly<br>"
                                     "influences the relative font size!</i>"));
    connect(spinMagnification, SIGNAL(editingFinished(double)), this, SLOT(printpreviewSetMagnification(double)));

    JKQTPEnhancedDoubleSpinBox* spinLineWidthMult=new JKQTPEnhancedDoubleSpinBox(dlg);
    spinLineWidthMult->setRange(1,1000);
    spinLineWidthMult->setValue(lineWidthPrintMultiplier*100.0);
    spinLineWidthMult->setSingleStep(10);
    spinLineWidthMult->setDecimals(0);
    spinLineWidthMult->setSuffix(tr(" %"));
    spinLineWidthMult->setToolTip(tr("use this to change the relative width of the lines\n"
                                     "in the exported/printed plot."));
    connect(spinLineWidthMult, SIGNAL(editingFinished(double)), this, SLOT(printpreviewSetLineWidthMultiplier(double)));

    JKQTPEnhancedDoubleSpinBox* spinFontSizeMult=new JKQTPEnhancedDoubleSpinBox(dlg);
    spinFontSizeMult->setRange(1,1000);
    spinFontSizeMult->setValue(fontSizePrintMultiplier*100.0);
    spinFontSizeMult->setSingleStep(10);
    spinFontSizeMult->setDecimals(0);
    spinFontSizeMult->setSuffix(tr(" %"));
    spinFontSizeMult->setToolTip(tr("use this to change the relative size of the text fonts\n"
                                     "in the exported/printed plot."));
    connect(spinFontSizeMult, SIGNAL(editingFinished(double)), this, SLOT(printpreviewSetFontSizeMultiplier(double)));


    //layout->addWidget(new QLabel(tr("magnification: ")), 0,layout->columnCount());
    //layout->addWidget(spinMagnification, 0,layout->columnCount());
    layout->addWidget(new QLabel(tr("linewidth mult.: ")), 0,layout->columnCount());
    layout->addWidget(spinLineWidthMult, 0,layout->columnCount());
    layout->addWidget(new QLabel(tr("font size mult.: ")), 0,layout->columnCount());
    layout->addWidget(spinFontSizeMult, 0,layout->columnCount());
    layout->addWidget(new QWidget(), 0,layout->columnCount());
    layout->setColumnStretch(layout->columnCount()-1, 1);
    layout->addWidget(printPreview, layout->rowCount(),0, 1, layout->columnCount());
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), dlg, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dlg, SLOT(reject()));
    layout->addWidget(buttonBox, layout->rowCount(),0, 1, layout->columnCount());
    dlg->resize(800,500);

    if (dlg->exec()==QDialog::Accepted) {
        printpreviewPaintRequested(p);
    }
    delete dlg;
    printPreview=nullptr;
    lineWidthMultiplier=lw;
    fontSizeMultiplier=fs;
    backgroundColor=bc;

}

bool JKQTBasePlotter::printpreviewNew(QPaintDevice* paintDevice, bool setAbsolutePaperSize, double printsizeX_inMM, double printsizeY_inMM, bool displayPreview) {
    QPrinter *printer=dynamic_cast<QPrinter*>(paintDevice);
    QSvgGenerator* svg=dynamic_cast<QSvgGenerator*>(paintDevice);
    double lw=lineWidthMultiplier;
    double fs=fontSizeMultiplier;
    double oldP=paintMagnification;
    QColor bc=backgroundColor;
    backgroundColor=exportBackgroundColor;
    lineWidthMultiplier=lineWidthPrintMultiplier;
    fontSizeMultiplier=fontSizePrintMultiplier;
    exportPreviewLabel=nullptr;
    printMagnification=1.0;
    paintMagnification=1.0;
    gridPrintingCalc();

    //double resolution=paintDevice->logicalDpiX();
    //if (printer) resolution=printer->resolution();

    printAspect=gridPrintingSize.height()/gridPrintingSize.width();
    if (printer) printPageSizeMM=printer->pageRect(QPrinter::Millimeter).size();
    else printPageSizeMM=QSizeF(paintDevice->widthMM(), paintDevice->heightMM());
    printSizeX_Millimeter=double(gridPrintingSize.width())/96.0*25.4;//double(resolution)*25.4; // convert current widget size in pt to millimeters, assuming 96dpi (default screen resolution)
    printSizeY_Millimeter=double(gridPrintingSize.height())/96.0*25.4;//double(resolution)*25.4;
    if (printsizeX_inMM>0) printSizeX_Millimeter=printsizeX_inMM;
    if (printsizeY_inMM>0) printSizeY_Millimeter=printsizeY_inMM;

    bool startWithMagnification=false;
    if (!setAbsolutePaperSize) {
        if (printSizeX_Millimeter>printPageSizeMM.width() || printSizeY_Millimeter>printPageSizeMM.height()) {
            startWithMagnification=true;
            printMagnification=qMin(printPageSizeMM.width()/printSizeX_Millimeter, printPageSizeMM.height()/printSizeY_Millimeter);
        }
    }
    printKeepAspect=true;

    printSetAbsolutePageSize=setAbsolutePaperSize;
    printKeepAbsoluteFontSizes=true;
    printScaleToPagesize=false;
    QDialog* dlg=new QDialog(nullptr, Qt::WindowMinMaxButtonsHint);
    dlg->setSizeGripEnabled(true);
    //printZoomFactor=0.95;
    //printMagnification=1.5;
    QGridLayout* layout=new QGridLayout();
    dlg->setLayout(layout);
    dlg->setWindowTitle(tr("Graph print/export preview ..."));
    dlg->setWindowIcon(QIcon(":/JKQTPlotter/jkqtp_exportprintpreview.png"));

    bool delPrinter=false;
    if (svg) {
        printer=new QPrinter();
        printer->setColorMode(QPrinter::Color);
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setResolution(svg->logicalDpiX());
        printer->setPageMargins(0,0,0,0,QPrinter::Millimeter);
        printer->setColorMode(QPrinter::Color);
        delPrinter=true;
    } else if (!printer) {
        printer=new QPrinter();
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setResolution(paintDevice->logicalDpiX());
        printer->setPageMargins(0,0,0,0,QPrinter::Millimeter);
        printer->setColorMode(QPrinter::Color);
        delPrinter=true;
    }


    printPreview=new QPrintPreviewWidget(printer, dlg);
    connect(printPreview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printpreviewPaintRequestedNew(QPrinter*)));



    spinSizeX=new JKQTPEnhancedDoubleSpinBox(dlg);
    spinSizeX->setRange(10,100000);
    spinSizeX->setValue(printSizeX_Millimeter);
    spinSizeX->setSingleStep(10);
    spinSizeX->setDecimals(1);
    spinSizeX->setSuffix(tr(" mm"));
    spinSizeX->setToolTip(tr("set the size of the output in millimeters"));
    connect(spinSizeX, SIGNAL(valueChanged(double)), this, SLOT(printpreviewSetSizeXNew(double)));
    connect(spinSizeX, SIGNAL(editingFinished()), this, SLOT(printpreviewUpdate()));
    spinSizeY=new JKQTPEnhancedDoubleSpinBox(dlg);
    spinSizeY->setRange(10,100000);
    spinSizeY->setValue(printSizeY_Millimeter);
    spinSizeY->setSingleStep(10);
    spinSizeY->setDecimals(1);
    spinSizeY->setSuffix(tr(" mm"));
    spinSizeY->setToolTip(tr("set the size of the output in millimeters"));
    spinSizeY->setEnabled(false);
    connect(spinSizeY, SIGNAL(valueChanged(double)), this, SLOT(printpreviewSetSizeYNew(double)));
    connect(spinSizeY, SIGNAL(editingFinished()), this, SLOT(printpreviewUpdate()));
    QCheckBox* chkAspect=new QCheckBox(tr("keep aspect ratio"), dlg);
    chkAspect->setChecked(true);
    chkAspect->setToolTip(tr("choose whether to keep the aspect ratio for the print/export, as on the screen."));
    connect(chkAspect, SIGNAL(toggled(bool)), spinSizeY, SLOT(setDisabled(bool)));
    connect(chkAspect, SIGNAL(toggled(bool)), this, SLOT(printpreviewSetAspectRatio(bool)));

    if (setAbsolutePaperSize) {
        layout->addWidget(new QLabel(tr("paper/plot size: ")), 0,layout->columnCount());
    } else if (printer && !svg) {
        int y=layout->columnCount();
        layout->addWidget(new QLabel(tr("plot size: ")), 0,y);
    } else {
        int y=layout->columnCount();
        layout->addWidget(new QLabel(tr("paper size: ")), 1,y);
        layout->addWidget(new QLabel(tr("plot size: ")), 0,y);
    }
    layout->addWidget(spinSizeX, 0,layout->columnCount());
    layout->addWidget(new QLabel(tr(" x ")), 0,layout->columnCount());
    layout->addWidget(spinSizeY, 0,layout->columnCount());
    layout->addWidget(chkAspect, 0,layout->columnCount());
    if (!setAbsolutePaperSize && printer && !svg) {
        if (printer) layout->addWidget(new QLabel(tr("%1x%2 mm^2").arg(printer->pageRect(QPrinter::Millimeter).width()).arg(printer->pageRect(QPrinter::Millimeter).height())), 1,layout->columnCount()-4, 1, 4);

        QCheckBox* chkSetMagnification=new QCheckBox(tr("set by magnification: "), dlg);
        chkSetMagnification->setChecked(false);
        spinMagnification=new JKQTPEnhancedDoubleSpinBox(dlg);
        spinMagnification->setEnabled(false);
        spinMagnification->setRange(1,1000);
        spinMagnification->setValue(printMagnification*100.0);
        spinMagnification->setSingleStep(10);
        spinMagnification->setDecimals(0);
        spinMagnification->setSuffix(tr(" %"));
        spinMagnification->setToolTip(tr("use this to change the base size of the plot<br>"
                                         "This will not change the size of the plot on<br>"
                                         "the page, only it's appearance. A magn. of 100%<br>"
                                         "will print the plot in the same proportions as<br>"
                                         "on the screen, whereas 50% will use twice as much<br>"
                                         "space for the plot, as on the screen. <i>This mainly<br>"
                                         "influences the relative font size!</i>"));
        connect(spinMagnification, SIGNAL(valueChanged(double)), this, SLOT(printpreviewSetMagnificationNew(double)));
        connect(chkSetMagnification, SIGNAL(toggled(bool)), spinMagnification, SLOT(setEnabled(bool)));
        connect(chkSetMagnification, SIGNAL(toggled(bool)), spinSizeX, SLOT(setDisabled(bool)));
        connect(chkSetMagnification, SIGNAL(toggled(bool)), spinSizeY, SLOT(setDisabled(bool)));
        connect(chkSetMagnification, SIGNAL(toggled(bool)), chkAspect, SLOT(setDisabled(bool)));
        connect(spinMagnification, SIGNAL(editingFinished()), this, SLOT(printpreviewUpdate()));
        connect(chkSetMagnification, SIGNAL(toggled(bool)), this, SLOT(printpreviewToggleMagnification(bool)));
        spinMagnification->setEnabled(startWithMagnification);
        chkSetMagnification->setChecked(startWithMagnification);
        layout->addWidget(chkSetMagnification, 0,layout->columnCount());
        layout->addWidget(spinMagnification, 0,layout->columnCount());
    }



    JKQTPEnhancedDoubleSpinBox* spinLineWidthMult=new JKQTPEnhancedDoubleSpinBox(dlg);
    spinLineWidthMult->setRange(1,1000);
    spinLineWidthMult->setValue(lineWidthPrintMultiplier*100.0);
    spinLineWidthMult->setSingleStep(10);
    spinLineWidthMult->setDecimals(0);
    spinLineWidthMult->setSuffix(tr(" %"));
    spinLineWidthMult->setToolTip(tr("use this to change the relative width of the lines\n"
                                     "in the exported/printed plot."));
    connect(spinLineWidthMult, SIGNAL(editingFinished(double)), this, SLOT(printpreviewSetLineWidthMultiplier(double)));

    QCheckBox* chkKeepAbsoluteFontSize=new QCheckBox(tr("keep abs. fontsize"), dlg);
    chkKeepAbsoluteFontSize->setChecked(true);
    connect(chkKeepAbsoluteFontSize, SIGNAL(toggled(bool)), this, SLOT(printpreviewSetKeepAbsFontsize(bool)));

    JKQTPEnhancedDoubleSpinBox* spinFontSizeMult=new JKQTPEnhancedDoubleSpinBox(dlg);
    spinFontSizeMult->setEnabled(false);
    spinFontSizeMult->setRange(1,1000);
    spinFontSizeMult->setValue(fontSizePrintMultiplier*100.0);
    spinFontSizeMult->setSingleStep(10);
    spinFontSizeMult->setDecimals(0);
    spinFontSizeMult->setSuffix(tr(" %"));
    spinFontSizeMult->setToolTip(tr("use this to change the relative size of the text fonts\n"
                                     "in the exported/printed plot."));
    connect(spinFontSizeMult, SIGNAL(editingFinished(double)), this, SLOT(printpreviewSetFontSizeMultiplier(double)));
    connect(chkKeepAbsoluteFontSize, SIGNAL(toggled(bool)), spinFontSizeMult, SLOT(setDisabled(bool)));


    layout->addWidget(new QLabel(tr("linewidth mult.: ")), 0,layout->columnCount());
    layout->addWidget(spinLineWidthMult, 0,layout->columnCount());
    int gpos=layout->columnCount();
    layout->addWidget(chkKeepAbsoluteFontSize, 0,gpos,1,2);
    layout->addWidget(new QLabel(tr("font size mult.: ")), 1,gpos);
    layout->addWidget(spinFontSizeMult, 1,gpos+1);
    layout->addWidget(new QWidget(), 0,layout->columnCount());
    layout->setColumnStretch(layout->columnCount()-1, 1);
    layout->addWidget(printPreview, layout->rowCount(),0, 1, layout->columnCount());
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), dlg, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dlg, SLOT(reject()));
    layout->addWidget(buttonBox, layout->rowCount(),0, 1, layout->columnCount());
    dlg->resize(800,500);

    bool res=false;
    if (printer) {
        if (!displayPreview || dlg->exec()==QDialog::Accepted) {
            qDebug()<<svg<<printer<<delPrinter;
            if (svg) {
                printpreviewPaintRequestedNew(svg);
            } else if (printer && !delPrinter) {
                if (delPrinter) {
                    printpreviewPaintRequestedNew(paintDevice);
                } else {
                    printpreviewPaintRequestedNew(printer);
                }
            } else {
                printpreviewPaintRequestedNew(paintDevice);
            }
            res=true;
        }
    }
    if ((svg||delPrinter) && printer) {
        delete printer;
    }
    delete dlg;
    printPreview=nullptr;
    lineWidthMultiplier=lw;
    fontSizeMultiplier=fs;
    backgroundColor=bc;
    paintMagnification=oldP;

    mathText.setUseUnparsed(false);

    return res;

}

bool JKQTBasePlotter::exportpreview(QSizeF pageSize, bool unitIsMM) {
    printPreview=nullptr;
    printSizeX_Millimeter=pageSize.width();
    printSizeY_Millimeter=pageSize.height();
    printAspect=1;
    printKeepAspect=false;
    exportUnitInMM=unitIsMM;

    printSetAbsolutePageSize=true;
    printScaleToPagesize=false;
    printKeepAbsoluteFontSizes=true;
    QDialog* dlg=new QDialog(nullptr, Qt::WindowMinMaxButtonsHint);
    dlg->setSizeGripEnabled(true);
    //printZoomFactor=0.95;
    //printMagnification=1.5;
    QGridLayout* layout=new QGridLayout();
    dlg->setLayout(layout);
    dlg->setWindowTitle(tr("Graph export preview ..."));
    dlg->setWindowIcon(QIcon(":/JKQTPlotter/jkqtp_exportprintpreview.png"));
    /*printPreview=new QPrintPreviewWidget(p, dlg);
    connect(printPreview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printpreviewPaintRequested(QPrinter*)));*/
    QScrollArea* scroll=new QScrollArea(dlg);
    {
        QPalette p(scroll->palette());
        // Set background colour to black
        p.setColor(QPalette::Background, Qt::darkGray);
        scroll->setPalette(p);
    }
    exportPreviewLabel=new QLabel(scroll);
    exportPreviewLabel->setMargin(10);
    scroll->setWidget(exportPreviewLabel);
    {
        QPalette p(exportPreviewLabel->palette());
        // Set background colour to black
        p.setColor(QPalette::Background, Qt::darkGray);
        exportPreviewLabel->setPalette(p);
    }



    printAspect=double(widgetHeight)/double(widgetWidth);
    if (gridPrinting) {
        gridPrintingCalc();
        printAspect=double(gridPrintingSize.height())/double(gridPrintingSize.width());
    }
    printSizeY_Millimeter=printSizeX_Millimeter*printAspect;
    printKeepAspect=true;

    JKQTPEnhancedDoubleSpinBox* spinSizeX=new JKQTPEnhancedDoubleSpinBox(dlg);
    spinSizeX->setRange(1,10000);
    spinSizeX->setValue(printSizeX_Millimeter);
    spinSizeX->setSingleStep(10);
    if (unitIsMM) spinSizeX->setDecimals(1);
    else spinSizeX->setDecimals(0);
    if (unitIsMM) spinSizeX->setSuffix(tr(" mm"));
    spinSizeX->setToolTip(tr("set the size of the output page"));
    connect(spinSizeX, SIGNAL(editingFinished(double)), this, SLOT(printpreviewSetSizeX(double)));
    spinSizeY=new JKQTPEnhancedDoubleSpinBox(dlg);
    spinSizeY->setRange(1,10000);
    spinSizeY->setValue(printSizeY_Millimeter);
    spinSizeY->setSingleStep(10);
    if (unitIsMM) spinSizeY->setDecimals(1);
    else spinSizeY->setDecimals(0);
    if (unitIsMM) spinSizeY->setSuffix(tr(" mm"));
    spinSizeY->setToolTip(tr("set the size of the output page"));
    spinSizeY->setEnabled(false);
    connect(spinSizeY, SIGNAL(editingFinished(double)), this, SLOT(printpreviewSetSizeY(double)));
    QCheckBox* chkAspect=new QCheckBox(tr("keep aspect ratio"), dlg);
    chkAspect->setChecked(true);
    chkAspect->setToolTip(tr("choose whether to keep the aspect ratio for the print/export, as on the screen."));
    connect(chkAspect, SIGNAL(toggled(bool)), spinSizeY, SLOT(setDisabled(bool)));
    connect(chkAspect, SIGNAL(toggled(bool)), this, SLOT(printpreviewSetAspectRatio(bool)));

    layout->addWidget(new QLabel(tr("page size: ")), 0,layout->columnCount());
    layout->addWidget(spinSizeX, 0,layout->columnCount());
    layout->addWidget(new QLabel(tr(" x ")), 0,layout->columnCount());
    layout->addWidget(spinSizeY, 0,layout->columnCount());
    layout->addWidget(chkAspect, 0,layout->columnCount());


    JKQTPEnhancedDoubleSpinBox* spinMagnification=new JKQTPEnhancedDoubleSpinBox(dlg);
    spinMagnification->setRange(1,1000);
    spinMagnification->setValue(printMagnification*100.0);
    spinMagnification->setSingleStep(10);
    spinMagnification->setDecimals(0);
    spinMagnification->setSuffix(tr(" %"));
    spinMagnification->setToolTip(tr("use this to change the base size of the plot<br>"
                                     "This will not change the size of the plot on<br>"
                                     "the page, only it's appearance. A magn. of 100%<br>"
                                     "will print the plot in the same proportions as<br>"
                                     "on the screen, whereas 50% will use twice as much<br>"
                                     "space for the plot, as on the screen. <i>This mainly<br>"
                                     "influences the relative font size!</i>"));
    connect(spinMagnification, SIGNAL(editingFinished(double)), this, SLOT(printpreviewSetMagnification(double)));

    layout->addWidget(new QLabel(tr("magnification: ")), 0,layout->columnCount());
    layout->addWidget(spinMagnification, 0,layout->columnCount());


    JKQTPEnhancedDoubleSpinBox* spinLineWidthMult=new JKQTPEnhancedDoubleSpinBox(dlg);
    spinLineWidthMult->setRange(1,1000);
    spinLineWidthMult->setValue(lineWidthPrintMultiplier*100.0);
    spinLineWidthMult->setSingleStep(10);
    spinLineWidthMult->setDecimals(0);
    spinLineWidthMult->setSuffix(tr(" %"));
    spinLineWidthMult->setToolTip(tr("use this to change the relative width of the lines\n"
                                     "in the exported/printed plot."));
    connect(spinLineWidthMult, SIGNAL(editingFinished(double)), this, SLOT(printpreviewSetLineWidthMultiplier(double)));

    JKQTPEnhancedDoubleSpinBox* spinFontSizeMult=new JKQTPEnhancedDoubleSpinBox(dlg);
    spinFontSizeMult->setRange(1,1000);
    spinFontSizeMult->setValue(fontSizePrintMultiplier*100.0);
    spinFontSizeMult->setSingleStep(10);
    spinFontSizeMult->setDecimals(0);
    spinFontSizeMult->setSuffix(tr(" %"));
    spinFontSizeMult->setToolTip(tr("use this to change the relative size of the text fonts\n"
                                     "in the exported/printed plot."));
    connect(spinFontSizeMult, SIGNAL(editingFinished(double)), this, SLOT(printpreviewSetFontSizeMultiplier(double)));

    layout->addWidget(new QLabel(tr("magnification: ")), 0,layout->columnCount());
    layout->addWidget(spinMagnification, 0,layout->columnCount());
    layout->addWidget(new QLabel(tr("linewidth mult.: ")), 0,layout->columnCount());
    layout->addWidget(spinLineWidthMult, 0,layout->columnCount());
    layout->addWidget(new QLabel(tr("font size mult.: ")), 0,layout->columnCount());
    layout->addWidget(spinFontSizeMult, 0,layout->columnCount());


    layout->addWidget(new QWidget(), 0,layout->columnCount());
    layout->setColumnStretch(layout->columnCount()-1, 1);
    layout->addWidget(scroll, layout->rowCount(),0, 1, layout->columnCount());
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), dlg, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dlg, SLOT(reject()));
    layout->addWidget(buttonBox, layout->rowCount(),0, 1, layout->columnCount());
    dlg->resize(800,500);
    updatePreviewLabel();

    bool result=false;
    if (dlg->exec()==QDialog::Accepted) {
        result=true;
    }
    return result;
}

void JKQTBasePlotter::updatePreviewLabel() {
    double factor=1;
    if (exportUnitInMM) factor=600.0/double(printSizeX_Millimeter);
    QImage pix(double(printSizeX_Millimeter)*factor*1.1, double(printSizeY_Millimeter)*factor*1.1, QImage::Format_ARGB32);
    pix.fill(Qt::transparent);
    JKQTPEnhancedPainter painter;
    painter.begin(&pix);
    exportpreviewPaintRequested(painter, QSize(double(printSizeX_Millimeter)*factor, double(printSizeY_Millimeter)*factor));
    painter.end();
    if (exportPreviewLabel) {
        QPixmap pm;
        pm.convertFromImage(pix);
        exportPreviewLabel->setPixmap(pm);
        exportPreviewLabel->resize(pm.size());
    }
}

void JKQTBasePlotter::printpreviewPaintRequested(QPrinter* printer) {
    double lw=lineWidthMultiplier;
    double fs=fontSizeMultiplier;
    QColor bc=backgroundColor;
    backgroundColor=exportBackgroundColor;
    lineWidthMultiplier=lineWidthPrintMultiplier;
    fontSizeMultiplier=fontSizePrintMultiplier;

    bool oldEmitPlotSignals=emitPlotSignals;
    emitPlotSignals=false;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QApplication::processEvents();
    int oldWidgetWidth=widgetWidth;
    int oldWidgetHeight=widgetHeight;
    //widgetWidth=widgetWidth/printMagnification;
    //widgetHeight=widgetHeight/printMagnification;
    double oldpm=paintMagnification;
    paintMagnification=printMagnification;
    if (printSetAbsolutePageSize) {
#ifdef SHOW_JKQTPLOTTER_DEBUG
        qDebug()<<"set printing abs. size to "<<QSizeF(printSizeX_Millimeter, printSizeY_Millimeter)<<" mm^2";
#endif
        //printer->setPaperSize(QPrinter::Custom);
        printer->setOrientation(QPrinter::Portrait);
        printer->setPaperSize(QSizeF(printSizeX_Millimeter, printSizeY_Millimeter), QPrinter::Millimeter);
        if (!gridPrinting) widgetHeight=widgetWidth*printSizeY_Millimeter/printSizeX_Millimeter;

    }

    JKQTPEnhancedPainter painter;
    painter.begin(printer);
    if (!printSetAbsolutePageSize) {
#ifdef SHOW_JKQTPLOTTER_DEBUG
        qDebug()<<"scale printing to "<<printZoomFactor;
#endif
        painter.scale(printZoomFactor, printZoomFactor);
    }

    if (printKeepAbsoluteFontSizes) {
        if (printZoomFactor!=1.0) {
            fontSizeMultiplier=1.0/printZoomFactor;
        }
    }
#ifdef SHOW_JKQTPLOTTER_DEBUG

    qDebug()<<"printSetAbsoluteSize = "<<printSetAbsolutePageSize;
    qDebug()<<"printScaleToPagesize = "<<printScaleToPagesize;
    qDebug()<<"printKeepAbsoluteFontSizes = "<<printKeepAbsoluteFontSizes;
    qDebug()<<"print with widget size "<<widgetWidth<<widgetHeight;
    qDebug()<<"print with paper size "<<printer->pageRect().size()<<" ";
    QSizeF sl=QSizeF(QSizeF(printer->pageRect().size()).width()/printer->logicalDpiX()*25.4, QSizeF(printer->pageRect().size()).height()/printer->logicalDpiY()*25.4);
    qDebug()<<"print with paper size "<<sl<<" mm^2";
    qDebug()<<"paintMagnification = "<<paintMagnification;
    qDebug()<<"lineWidthMultiplier = "<<lineWidthMultiplier;
    qDebug()<<"fontSizeMultiplier = "<<fontSizeMultiplier;
    qDebug()<<"printer resolution = "<<printer->resolution()<<" dpi";
    qDebug()<<"\nplotLabelFontSize = "<<plotLabelFontSize<<" pt";
    qDebug()<<"keyFontSize = "<<keyFontSize<<" pt";
    qDebug()<<"x-axis label fontsize = "<<xAxis->getLabelFontSize()<<" pt";
    qDebug()<<"y-axis label fontsize = "<<yAxis->getLabelFontSize()<<" pt";
#endif
    gridPaint(painter, printer->pageRect().size(), true, printScaleToPagesize, printScaleToPagesize);
    painter.end();
    widgetWidth=oldWidgetWidth;
    widgetHeight=oldWidgetHeight;
    QApplication::restoreOverrideCursor();
    QApplication::processEvents();
    emitPlotSignals=oldEmitPlotSignals;
    lineWidthMultiplier=lw;
    fontSizeMultiplier=fs;
    paintMagnification=oldpm;
    backgroundColor=bc;

}


void JKQTBasePlotter::printpreviewPaintRequestedNew(QPrinter* printer) {
    QPaintDevice* paintDevice=dynamic_cast<QPaintDevice*>(printer);
    printpreviewPaintRequestedNew(paintDevice);

}

void JKQTBasePlotter::printpreviewPaintRequestedNew(QPaintDevice *paintDevice)
{
    //QPaintDevice* paintDevice=dynamic_cast<QPaintDevice*>(printer);
    QPrinter* printer=dynamic_cast<QPrinter*>(paintDevice);
    QSvgGenerator* svg=dynamic_cast<QSvgGenerator*>(paintDevice);

    double lw=lineWidthMultiplier;
    double fs=fontSizeMultiplier;
    QColor bc=backgroundColor;
    backgroundColor=exportBackgroundColor;
    lineWidthMultiplier=lineWidthPrintMultiplier;
    fontSizeMultiplier=fontSizePrintMultiplier;

    bool oldEmitPlotSignals=emitPlotSignals;
    emitPlotSignals=false;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QApplication::processEvents();
    int oldWidgetWidth=widgetWidth;
    int oldWidgetHeight=widgetHeight;
    double oldpm=paintMagnification;
    paintMagnification=1.0;
    gridPrintingCalc();
    if (printSetAbsolutePageSize) {
#ifdef SHOW_JKQTPLOTTER_DEBUG
        qDebug()<<"set printing abs. size to "<<QSizeF(printSizeX_Millimeter, printSizeY_Millimeter)<<" mm^2";
#endif
        if (printer) {
            printer->setOrientation(QPrinter::Portrait);
            printer->setPaperSize(QSizeF(printSizeX_Millimeter, printSizeY_Millimeter), QPrinter::Millimeter);
        } else if (svg) {
            QRectF siz=QRectF(0,0,printSizeX_Millimeter,printSizeY_Millimeter);
            svg->setSize(QSize(ceil(siz.width()*svg->resolution()/25.4), ceil(siz.height()*svg->resolution()/25.4)));
            svg->setViewBox(QRect(0,0,-1,-1));//*25.4/double(svg->resolution()), printSizeY_Millimeter*25.4/double(svg->resolution())));
        }

    }
    paintMagnification=gridPrintingSize.width()/(printSizeX_Millimeter/25.4*double(paintDevice->logicalDpiX()));
    if (!gridPrinting) widgetHeight=widgetWidth*printSizeY_Millimeter/printSizeX_Millimeter;

    JKQTPEnhancedPainter painter;
    if (printer) painter.begin(printer);
    else if (svg) painter.begin(svg);
    else painter.begin(paintDevice);

    if (printKeepAbsoluteFontSizes) {
        fontSizeMultiplier=1.0;//1.0/paintMagnification;
    }

    if (svg) {
        lineWidthMultiplier=lineWidthMultiplier*72.0/96.0;
        fontSizeMultiplier=fontSizeMultiplier*72.0/96.0;
    }
#ifdef SHOW_JKQTPLOTTER_DEBUG
    qDebug()<<"\n\n\n==========================================================================";
    qDebug()<<"printAspect = "<<printAspect;
    qDebug()<<"printKeepAspect = "<<printKeepAspect;
    qDebug()<<"printSetAbsoluteSize = "<<printSetAbsolutePageSize;
    qDebug()<<"printScaleToPagesize = "<<printScaleToPagesize;
    qDebug()<<"printKeepAbsoluteFontSizes = "<<printKeepAbsoluteFontSizes;
    qDebug()<<"print with widget size "<<widgetWidth<<widgetHeight;
    if (printer) {
        qDebug()<<"print with paper size "<<printer->pageRect().size()<<" ";
        QSizeF sl=QSizeF(QSizeF(printer->pageRect().size()).width()/printer->logicalDpiX()*25.4, QSizeF(printer->pageRect().size()).height()/printer->logicalDpiY()*25.4);
        qDebug()<<"print with paper size "<<sl<<" mm^2";
    } else if (svg) {
        qDebug()<<"print with paper size "<<svg->size()<<" ";
        QSizeF sl=QSizeF(QSizeF(svg->viewBoxF().size()).width()/svg->resolution()*25.4, QSizeF(svg->size()).height()/svg->resolution()*25.4);
        qDebug()<<"print with paper size "<<sl<<" mm^2";
    }
    qDebug()<<"plot size             "<<printSizeX_Millimeter<<printSizeY_Millimeter<<" mm^2";
    qDebug()<<"paintMagnification = "<<paintMagnification;
    qDebug()<<"lineWidthMultiplier = "<<lineWidthMultiplier;
    qDebug()<<"fontSizeMultiplier = "<<fontSizeMultiplier;
    qDebug()<<"printer resolution = "<<paintDevice->logicalDpiX()<<" dpi";
    qDebug()<<"\nplotLabelFontSize = "<<plotLabelFontSize<<" pt";
    qDebug()<<"keyFontSize = "<<keyFontSize<<" pt";
    qDebug()<<"x-axis label fontsize = "<<xAxis->getLabelFontSize()<<" pt";
    qDebug()<<"y-axis label fontsize = "<<yAxis->getLabelFontSize()<<" pt";
#endif
    if (printer) gridPaint(painter, printer->pageRect().size(), true, printScaleToPagesize, printScaleToPagesize);
    else if (svg) gridPaint(painter, svg->size(), true, printScaleToPagesize, printScaleToPagesize);
    else gridPaint(painter, QSizeF(paintDevice->width(), paintDevice->height()), true, printScaleToPagesize, printScaleToPagesize);
    painter.end();
    widgetWidth=oldWidgetWidth;
    widgetHeight=oldWidgetHeight;
    QApplication::restoreOverrideCursor();
    QApplication::processEvents();
    emitPlotSignals=oldEmitPlotSignals;
    lineWidthMultiplier=lw;
    fontSizeMultiplier=fs;
    paintMagnification=oldpm;
    backgroundColor=bc;
}

void JKQTBasePlotter::exportpreviewPaintRequested(JKQTPEnhancedPainter &painter, QSize size) {
    double lw=lineWidthMultiplier;
    double fs=fontSizeMultiplier;
    QColor bc=backgroundColor;
    backgroundColor=exportBackgroundColor;
    lineWidthMultiplier=lineWidthPrintMultiplier;
    fontSizeMultiplier=fontSizePrintMultiplier;
    bool oldEmitPlotSignals=emitPlotSignals;
    emitPlotSignals=false;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QApplication::processEvents();
    int oldWidgetWidth=widgetWidth;
    int oldWidgetHeight=widgetHeight;
    widgetWidth=size.width();
    widgetHeight=size.height();

    gridPaint(painter, size);
    widgetWidth=oldWidgetWidth;
    widgetHeight=oldWidgetHeight;
    QApplication::restoreOverrideCursor();
    QApplication::processEvents();
    emitPlotSignals=oldEmitPlotSignals;
    lineWidthMultiplier=lw;
    fontSizeMultiplier=fs;
    backgroundColor=bc;
}

void JKQTBasePlotter::printpreviewSetZoom(double value) {
    printZoomFactor=value/100.0;
    if (printPreview) printPreview->updatePreview();
    if (exportPreviewLabel) updatePreviewLabel();
}

void JKQTBasePlotter::printpreviewSetSizeX(double value) {
    printSizeX_Millimeter=value;
    if (printKeepAspect) {
        printSizeY_Millimeter=printSizeX_Millimeter*printAspect;
        spinSizeY->setValue(printSizeY_Millimeter);
    }
    if (printDoUpdate) {
        if (printPreview) printPreview->updatePreview();
        if (exportPreviewLabel) updatePreviewLabel();
    }
}
void JKQTBasePlotter::printpreviewSetSizeY(double value) {
    if (printKeepAspect) return;
    printSizeY_Millimeter=value;
    if (printDoUpdate) {
        if (printPreview) printPreview->updatePreview();
        if (exportPreviewLabel) updatePreviewLabel();
    }
}

void JKQTBasePlotter::printpreviewSetSizeXNew(double value) {
    printSizeX_Millimeter=value;
    if (printKeepAspect) {
        printSizeY_Millimeter=printSizeX_Millimeter*printAspect;
        spinSizeY->setValue(printSizeY_Millimeter);
    }

}
void JKQTBasePlotter::printpreviewSetSizeYNew(double value) {
    printSizeY_Millimeter=value;

}

void JKQTBasePlotter::printpreviewSetMagnification(double value) {
    printMagnification=value/100.0;
    if (printPreview) printPreview->updatePreview();
    if (exportPreviewLabel) updatePreviewLabel();
}


void JKQTBasePlotter::printpreviewSetMagnificationNew(double value)
{
    printMagnification=value/100.0;

    printDoUpdate=false;
    if (spinSizeY) {
        spinSizeY->setValue(printMagnification*printPageSizeMM.height());
    }
    if (spinSizeX) {
        spinSizeX->setValue(printMagnification*printPageSizeMM.width());
    }
    printDoUpdate=true;

}

void JKQTBasePlotter::printpreviewSetAspectRatio(bool checked) {
    printKeepAspect=checked;
    if (printKeepAspect) {
        printSizeY_Millimeter=printSizeX_Millimeter*printAspect;
        spinSizeY->setValue(printSizeY_Millimeter);
    }
    if (printPreview) printPreview->updatePreview();
    if (exportPreviewLabel) updatePreviewLabel();
}

void JKQTBasePlotter::printpreviewSetKeepAbsFontsize(bool checked)
{
    printKeepAbsoluteFontSizes=checked;
    if (printPreview) printPreview->updatePreview();
    if (exportPreviewLabel) updatePreviewLabel();
}

void JKQTBasePlotter::printpreviewToggleMagnification(bool checked)
{
    if (checked) {
        if (spinMagnification) printpreviewSetMagnificationNew(spinMagnification->value());
    }
    printpreviewUpdate();
}

void JKQTBasePlotter::printpreviewSetLineWidthMultiplier(double value) {
    lineWidthPrintMultiplier=value/100.0;
    if (printPreview) printPreview->updatePreview();
    if (exportPreviewLabel) updatePreviewLabel();
}

void JKQTBasePlotter::printpreviewSetFontSizeMultiplier(double value) {
    fontSizePrintMultiplier=value/100.0;
    if (printPreview) printPreview->updatePreview();
    if (exportPreviewLabel) updatePreviewLabel();
}

void JKQTBasePlotter::printpreviewUpdate()
{
    if (printDoUpdate) {
        if (printPreview) printPreview->updatePreview();
        if (exportPreviewLabel) updatePreviewLabel();
    }
}

void JKQTBasePlotter::draw(JKQTPEnhancedPainter& painter, const QRect& rect, bool drawOverlays) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTBasePlotter::draw(rect, %1)").arg(drawOverlays));
#endif
    bool oldEmitPlotSignals=emitPlotSignals;
    emitPlotSignals=false;
#ifdef JKQTBP_DEBUGTIMING
    QString on=objectName();
    if (on.isEmpty() && parent()) on=parent()->objectName();
    qDebug()<<on<<"::draw ...";
    QElapsedTimer timeAll;
    timeAll.start();
#endif
    //resize(rect.width(), rect.height());
    painter.save();
    painter.translate(rect.topLeft());
#ifdef JKQTBP_DEBUGTIMING
    QElapsedTimer time;
    time.start();
#endif
    gridPaint(painter, rect.size(), drawOverlays);
#ifdef JKQTBP_DEBUGTIMING
    qDebug()<<on<<"::draw ... gridPaint       = " <<time.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(time.nsecsElapsed())/1000000.0<<" msecs";
#endif
    painter.restore();
    emitPlotSignals=oldEmitPlotSignals;
}

void JKQTBasePlotter::drawOverlays(JKQTPEnhancedPainter &painter, const QRect& rect)
{

    //resize(rect.width(), rect.height());
    painter.save();
    painter.translate(rect.topLeft());

    gridPaintOverlays(painter, rect.size());

    painter.restore();
}

void JKQTBasePlotter::draw(JKQTPEnhancedPainter& painter, const QPoint& pos, bool drawOverlays) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTBasePlotter::draw(pos, %1)").arg(drawOverlays));
#endif
    bool oldEmitPlotSignals=emitPlotSignals;
    emitPlotSignals=false;
#ifdef JKQTBP_DEBUGTIMING
    QString on=objectName();
    if (on.isEmpty() && parent()) on=parent()->objectName();
    qDebug()<<on<<"::draw ...";
    QElapsedTimer timeAll;
    timeAll.start();
#endif
    QRect rect=QRect(pos, QSize(widgetWidth/paintMagnification, widgetHeight/paintMagnification));
    painter.save();
    painter.translate(rect.topLeft());
#ifdef JKQTBP_DEBUGTIMING
    QElapsedTimer time;
    time.start();
#endif
    gridPaint(painter, rect.size(), drawOverlays);
#ifdef JKQTBP_DEBUGTIMING
    qDebug()<<on<<"::draw ... gridPaint       = " <<time.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(time.nsecsElapsed())/1000000.0<<" msecs";
#endif
    painter.restore();
    emitPlotSignals=oldEmitPlotSignals;
}

void JKQTBasePlotter::drawNonGrid(JKQTPEnhancedPainter& painter, const QRect& rect, bool drawOverlays) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTBasePlotter::drawNonGrid(rect, %1)").arg(drawOverlays));
#endif
    bool oldEmitPlotSignals=emitPlotSignals;
    emitPlotSignals=false;
#ifdef JKQTBP_DEBUGTIMING
    QString on=objectName();
    if (on.isEmpty() && parent()) on=parent()->objectName();
    qDebug()<<on<<"::drawNonGrid ...";
    QElapsedTimer timeAll;
    timeAll.start();
#endif
    painter.save();
    painter.translate(rect.topLeft());
#ifdef JKQTBP_DEBUGTIMING
    QElapsedTimer time;
    time.start();
#endif
    calcPlotScaling(painter);
#ifdef JKQTBP_DEBUGTIMING
    qDebug()<<on<<"::drawNonGrid ... calcPlotScaling = " <<time.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(time.nsecsElapsed())/1000000.0<<" msecs";
#endif
    double scale=static_cast<double>(rect.width())/static_cast<double>(widgetWidth)*paintMagnification;
    if ((scale*static_cast<double>(widgetWidth)/paintMagnification>static_cast<double>(rect.width())) || (scale*static_cast<double>(widgetHeight)/paintMagnification>static_cast<double>(rect.height()))) {
        scale=static_cast<double>(rect.height())/static_cast<double>(widgetHeight)*paintMagnification;
    }
    painter.save();
    // scale the plot so it fits on the page
    painter.scale(scale, scale);
#ifdef JKQTBP_DEBUGTIMING
    time.start();
#endif
    paintPlot(painter, drawOverlays);
#ifdef JKQTBP_DEBUGTIMING
    qDebug()<<on<<"::drawNonGrid ... paintPlot       = " <<time.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(time.nsecsElapsed())/1000000.0<<" msecs";
#endif
    painter.restore();
    painter.restore();
#ifdef JKQTBP_DEBUGTIMING
    qDebug()<<on<<"::drawNonGrid ... DONE            = "<<timeAll.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(timeAll.nsecsElapsed())/1000000.0<<" msecs";
#endif
    emitPlotSignals=oldEmitPlotSignals;
}


void JKQTBasePlotter::drawNonGrid(JKQTPEnhancedPainter& painter, const QPoint& pos, bool drawOverlays) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTBasePlotter::drawNonGrid(pos, %1)").arg(drawOverlays));
#endif
    bool oldEmitPlotSignals=emitPlotSignals;
    emitPlotSignals=false;
#ifdef JKQTBP_DEBUGTIMING
    QString on=objectName();
    if (on.isEmpty() && parent()) on=parent()->objectName();
    qDebug()<<on<<"::drawNonGrid ...";
    QElapsedTimer timeAll;
    timeAll.start();
#endif
    QRect rect=QRect(pos, QSize(widgetWidth/paintMagnification, widgetHeight/paintMagnification));
    painter.save();
    painter.translate(rect.topLeft());
#ifdef JKQTBP_DEBUGTIMING
    QElapsedTimer time;
    time.start();
#endif
    calcPlotScaling(painter);
#ifdef JKQTBP_DEBUGTIMING
    qDebug()<<on<<"::drawNonGrid ... calcPlotScaling = " <<time.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(time.nsecsElapsed())/1000000.0<<" msecs";
#endif
    double scale=static_cast<double>(rect.width())/static_cast<double>(widgetWidth)*paintMagnification;
    if ((scale*static_cast<double>(widgetWidth)/paintMagnification>static_cast<double>(rect.width())) || (scale*static_cast<double>(widgetHeight)/paintMagnification>static_cast<double>(rect.height()))) {
        scale=static_cast<double>(rect.height())/static_cast<double>(widgetHeight)*paintMagnification;
    }
    painter.save();
    // scale the plot so it fits on the page
    painter.scale(scale, scale);
#ifdef JKQTBP_DEBUGTIMING
    time.start();
#endif
    paintPlot(painter, drawOverlays);
#ifdef JKQTBP_DEBUGTIMING
    qDebug()<<on<<"::drawNonGrid ... paintPlot       = " <<time.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(time.nsecsElapsed())/1000000.0<<" msecs";
#endif
    painter.restore();
    painter.restore();
#ifdef JKQTBP_DEBUGTIMING
    qDebug()<<on<<"::drawNonGrid ... DONE            = "<<timeAll.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(timeAll.nsecsElapsed())/1000000.0<<" msecs";
#endif
    emitPlotSignals=oldEmitPlotSignals;
}

void JKQTBasePlotter::drawNonGridOverlays(JKQTPEnhancedPainter& painter, const QPoint& pos) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTBasePlotter::drawNonGridOverlays(point)"));
#endif
    QRect rect=QRect(pos, QSize(widgetWidth/paintMagnification, widgetHeight/paintMagnification));
    painter.save();
    painter.translate(rect.topLeft());
    double scale=static_cast<double>(rect.width())/static_cast<double>(widgetWidth)*paintMagnification;
    if ((scale*static_cast<double>(widgetWidth)/paintMagnification>static_cast<double>(rect.width())) || (scale*static_cast<double>(widgetHeight)/paintMagnification>static_cast<double>(rect.height()))) {
        scale=static_cast<double>(rect.height())/static_cast<double>(widgetHeight)*paintMagnification;
    }
    painter.save();
    // scale the plot so it fits on the page
    painter.scale(scale, scale);
    paintOverlays(painter);
    painter.restore();
    painter.restore();
}

void JKQTBasePlotter::setEmittingPlotSignalsEnabled(bool __value)
{
    this->emitPlotSignals = __value;
}

bool JKQTBasePlotter::isEmittingPlotSignalsEnabled() const
{
    return this->emitPlotSignals;
}

void JKQTBasePlotter::setGridPrinting(bool __value)
{
    this->gridPrinting = __value;
}

bool JKQTBasePlotter::getGridPrinting() const
{
    return this->gridPrinting;
}

void JKQTBasePlotter::setGridPrintingCurrentX(size_t __value)
{
    this->gridPrintingCurrentX = __value;
}

size_t JKQTBasePlotter::getGridPrintingCurrentX() const
{
    return this->gridPrintingCurrentX;
}

void JKQTBasePlotter::setGridPrintingCurrentY(size_t __value)
{
    this->gridPrintingCurrentY = __value;
}

size_t JKQTBasePlotter::getGridPrintingCurrentY() const
{
    return this->gridPrintingCurrentY;
}

void JKQTBasePlotter::setGridPrintingCurrentPos(size_t x, size_t y)
{
    gridPrintingCurrentX=x;
    gridPrintingCurrentY=y;
}

void JKQTBasePlotter::registerAdditionalAction(const QString &key, QAction *act)
{
    if (!lstAdditionalPlotterActions.contains(key)) {
        QList<QPointer<QAction> > l;
        lstAdditionalPlotterActions.insert(key, l);
    }
    lstAdditionalPlotterActions[key].append(act);

    AdditionalActionsMapIterator it(lstAdditionalPlotterActions);
    while (it.hasNext()) {
        it.next();
        for (int i=it.value().size()-1; i>=0; i--) {
            if (!it.value().at(i)) {
                lstAdditionalPlotterActions[it.key()].removeAt(i);
            }
        }
    }
}

void JKQTBasePlotter::deregisterAdditionalAction(QAction *act)
{
    AdditionalActionsMapIterator it(lstAdditionalPlotterActions);
    while (it.hasNext()) {
        it.next();
        for (int i=it.value().size()-1; i>=0; i--) {
            if (it.value().at(i)==act) {
                lstAdditionalPlotterActions[it.key()].removeAt(i);
            }
        }
    }
}

void JKQTBasePlotter::copyData() {
    loadUserSettings();
    QString result="";
    QString qfresult;
    QSet<int> cols=getDataColumnsByUser();
    {
        QTextStream txt(&result);
        QLocale loc=QLocale::system();
        loc.setNumberOptions(QLocale::OmitGroupSeparator);
        QChar dp=loc.decimalPoint();
        QString sep="\t";
        datastore->saveCSV(txt, cols, sep, QString(dp), " ", "\"");
        txt.flush();
    }
    {
        QTextStream txt(&qfresult);
        datastore->saveCSV(txt, cols, ",", ".", "#!", "\"");
        txt.flush();
    }
    QClipboard *clipboard = QApplication::clipboard();
    QMimeData* mime=new QMimeData();
    mime->setText(result);
    mime->setData("jkqtplotter/csv", qfresult.toUtf8());
    clipboard->setMimeData(mime);
    //clipboard->setText(result);
    saveUserSettings();
}


void JKQTBasePlotter::copyDataMatlab() {
    loadUserSettings();
    QString result="";
    {
        QTextStream txt(&result);
        datastore->saveMatlab(txt, getDataColumnsByUser());
        txt.flush();
    }
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(result);
    saveUserSettings();
}

void JKQTBasePlotter::saveData(const QString& filename, const QString &format) {
    loadUserSettings();
    QStringList fileformats;
    QStringList fileformatIDs;
    fileformats<<tr("Comma Separated Values (*.csv *.dat)");
    fileformatIDs<<"csv";
    fileformats<<tr("Tab Separated Values (*.txt)");
    fileformatIDs<<"tab";
    fileformats<<tr("Semicolon Separated Values [German Excel] (*.csv *.dat *.txt)");
    fileformatIDs<<"gex";
    fileformats<<tr("Semicolon Separated Values (*.csv *.dat *.txt)");
    fileformatIDs<<"sem";
    fileformats<<tr("SYLK spreadsheet (*.slk)");
    fileformatIDs<<"slk";
    fileformats<<tr("DIF: Data Interchange Format (*.dif)");
    fileformatIDs<<"dif";
    fileformats<<tr("Matlab Script (*.m)");
    fileformatIDs<<"m";

    for (int i=0; i<jkqtpSaveDataAdapters.size(); i++) {
        fileformats<<jkqtpSaveDataAdapters[i]->getFilter();
        fileformatIDs<<QString("custom%1").arg(i);
    }



    QString fn=filename;
    QString fmt=format.toLower();
    if (fmt.isEmpty()) {
        QString e=QFileInfo(filename).suffix().toLower();//  jkqtp_tolower(extract_file_ext(fn.toStdString()));
        if (e=="csv" || e=="dat") {
            fmt="csv";
        } else if (e=="txt") {
            fmt="tab";
        } else if ((e=="slk")||(e=="sylk")) {
            fmt="slk";
        } else if (e=="dif") {
            fmt="dif";
        } else if (e=="m") {
            fmt="m";
        }
    }
    if (fn.isEmpty()) {
        QString selectedFilter=currentDataFileFormat;
        //qDebug()<<"before: currentSaveDirectory="<<currentSaveDirectory;
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot Data"),
                                    currentSaveDirectory,
                                    fileformats.join(";;"),
                                    &selectedFilter);
        if (!fn.isEmpty()) {
            currentSaveDirectory=QFileInfo(fn).absolutePath();
            currentDataFileFormat=selectedFilter;
        }
        //qDebug()<<"after: currentSaveDirectory="<<currentSaveDirectory;
        fmt="csv";
        for (int i=0; i<fileformats.size(); i++) {
            if (selectedFilter.contains(fileformats[i])) {
                fmt=fileformatIDs[i] ;
            }
        }


    }

    saveUserSettings();
    if (!fn.isEmpty()) {

        if (fmt=="csv") {
            saveAsCSV(fn);
        } else if (fmt=="tab") {
            saveAsTabSV(fn);
        } else if (fmt=="gex") {
            saveAsGerExcelCSV(fn);
        } else if (fmt=="sem") {
            saveAsSemicolonSV(fn);
        } else if (fmt=="slk") {
            saveAsSYLK(fn);
        } else if (fmt=="dif") {
            saveAsDIF(fn);
        } else if (fmt=="m") {
            saveAsMatlab(fn);
        } else if (fmt.startsWith("custom")) {
            QString fidx=fmt;
            fidx=fidx.remove(0,6);
            int idx=fidx.toInt();
            if (idx>=0 && idx<jkqtpSaveDataAdapters.size() && jkqtpSaveDataAdapters[idx]) {
                QStringList cn;
                QList<QVector<double> > dataset=datastore->getData(&cn);
                jkqtpSaveDataAdapters[idx]->saveJKQTPData(fn, dataset, cn);
            }
        }
    }
}

void JKQTBasePlotter::saveAsCSV(const QString& filename) {
    loadUserSettings();
    QString fn=filename;
    if (fn.isEmpty()) {
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot Data"),
                                    currentSaveDirectory,
                                    tr("Comma Separated Values (*.csv *.dat)"));
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    if (!fn.isEmpty()) {
        datastore->saveCSV(fn, getDataColumnsByUser(), ", ", CSVdecimalSeparator, CSVcommentInitializer);
    }
    saveUserSettings();
}

void JKQTBasePlotter::saveAsSYLK(const QString& filename) {
    loadUserSettings();
    QString fn=filename;
    if (fn.isEmpty()) {
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot Data"),
                                    currentSaveDirectory,
                                    tr("SYLK spreadsheet (*.slk)"));
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    if (!fn.isEmpty()) {
        datastore->saveSYLK(fn, getDataColumnsByUser());
    }
    saveUserSettings();
}

void JKQTBasePlotter::saveAsMatlab(const QString& filename) {
    loadUserSettings();
    QString fn=filename;
    if (fn.isEmpty()) {
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot Data"),
                                    currentSaveDirectory,
                                    tr("Matlab Script (*.m)"));
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    if (!fn.isEmpty()) {
        datastore->saveMatlab(fn, getDataColumnsByUser());
    }
    saveUserSettings();
}

void JKQTBasePlotter::saveAsDIF(const QString& filename) {
    loadUserSettings();
    QString fn=filename;
    if (fn.isEmpty()) {
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot Data"),
                                    currentSaveDirectory,
                                    tr("DIF: Data Interchange Format (*.dif)"));
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    if (!fn.isEmpty()) {
        datastore->saveDIF(fn, getDataColumnsByUser());
    }
    saveUserSettings();
}

void JKQTBasePlotter::saveAsSemicolonSV(const QString& filename) {
    loadUserSettings();
    QString fn=filename;
    if (fn.isEmpty()) {
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot Data"),
                                    currentSaveDirectory,
                                    tr("Comma Separated Values (*.csv *.dat)"));
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    if (!fn.isEmpty()) {
        datastore->saveCSV(fn, getDataColumnsByUser(), ";", CSVdecimalSeparator, CSVcommentInitializer);
    }
    saveUserSettings();
}

void JKQTBasePlotter::saveAsTabSV(const QString& filename) {
    loadUserSettings();
    QString fn=filename;
    if (fn.isEmpty()) {
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot Data"),
                                    currentSaveDirectory,
                                    tr("Tabulator Separated Values (*.txt)"));
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    if (!fn.isEmpty()) {
        datastore->saveCSV(fn, getDataColumnsByUser(), "\t", CSVdecimalSeparator, CSVcommentInitializer);
    }
    saveUserSettings();
}

void JKQTBasePlotter::saveAsGerExcelCSV(const QString& filename) {
    loadUserSettings();
    QString fn=filename;
    if (fn.isEmpty()) {
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot Data"),
                                    currentSaveDirectory,
                                    tr("Tabulator Separated Values (*.txt)"));
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    if (!fn.isEmpty()) {
        datastore->saveCSV(fn, getDataColumnsByUser(), ";", ",", " ", "\"");
    }
    saveUserSettings();
}

void JKQTBasePlotter::saveAsPDF(const QString& filename, bool displayPreview) {
    loadUserSettings();
    QString fn=filename;
    if (fn.isEmpty()) {
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot"),
                                    currentSaveDirectory,
                                    tr("PDF File (*.pdf)"));
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    if (!fn.isEmpty()) {
        QPrinter* printer=new QPrinter;
        bool doLandscape=widgetWidth>widgetHeight;
        if (gridPrinting) {
            gridPrintingCalc();
            doLandscape=gridPrintingSize.width()>gridPrintingSize.height();
        }
        if (doLandscape) {
            printer->setOrientation(QPrinter::Landscape);
        } else {
            printer->setOrientation(QPrinter::Portrait);
        }
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setColorMode(QPrinter::Color);
        printer->setOutputFileName(fn);
        printer->setPageMargins(0,0,0,0,QPrinter::Millimeter);
        printer->setColorMode(QPrinter::Color);
        printpreviewNew(printer, true, -1.0, -1.0, displayPreview);
        delete printer;
    }
    saveUserSettings();
}

void JKQTBasePlotter::saveAsPS(const QString& filename, bool displayPreview) {
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    loadUserSettings();
    QString fn=filename;
    if (fn.isEmpty()) {
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot"),
                                    currentSaveDirectory,
                                    tr("PostScript File (*.ps)"));
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    if (!fn.isEmpty()) {
        QPrinter* printer=new QPrinter;
        bool doLandscape=widgetWidth>widgetHeight;
        if (gridPrinting) {
            gridPrintingCalc();
            doLandscape=gridPrintingSize.width()>gridPrintingSize.height();
        }
        if (doLandscape) {
            printer->setOrientation(QPrinter::Landscape);
        } else {
            printer->setOrientation(QPrinter::Portrait);
        }
        printer->setOutputFormat(QPrinter::PostScriptFormat);
        printer->setColorMode(QPrinter::Color);
        printer->setOutputFileName(fn);
        printer->setPageMargins(0,0,0,0,QPrinter::Millimeter);
        printpreviewNew(printer, true, -1.0, -1.0, displayPreview);
        delete printer;
    }
    saveUserSettings();
#else
    saveAsPDF(filename+".pdf", displayPreview);
#endif
}


void JKQTBasePlotter::saveImage(const QString& filename, bool displayPreview) {
    loadUserSettings();
    QString fn=filename;
    QStringList filt;
    filt<<tr("Portable Document Format PDF [Qt] (*.pdf)");
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    filt<<tr("PostScript [Qt] (*.ps)");
#endif
    filt<<tr("Scalable Vector Graphics [Qt] (*.svg)");
    filt<<tr("PNG Image [Qt] (*.png)");
    filt<<tr("BMP Image [Qt] (*.bmp)");
    filt<<tr("TIFF Image [Qt] (*.tif *.tiff)");
    filt<<tr("JPEG Image [Qt] (*.jpg *.jpeg)");
    const int filtStartSize=filt.size();
    for (int i=0; i<jkqtpPaintDeviceAdapters.size(); i++) {
        filt<<jkqtpPaintDeviceAdapters[i]->getFilter();
    }
    int qtwritersidx=filt.size();
    QList<QByteArray> writerformats=QImageWriter::supportedImageFormats();
    for (int i=0; i<writerformats.size(); i++) {
        filt<<QString("%1 Image (*.%2)").arg(QString(writerformats[i]).toUpper()).arg(QString(writerformats[i].toLower()));
    }
    /*filt<<tr("X11 Bitmap [Qt] (*.xbm)");
    filt<<tr("X11 Pixmap [Qt] (*.xpm)");*/
    QString selFormat="";
    if (fn.isEmpty()) {
        selFormat=currentFileFormat;
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot"),
                                    currentSaveDirectory,
                                          filt.join(";;"), &selFormat);
        if (!fn.isEmpty()) {
            currentSaveDirectory=QFileInfo(fn).absolutePath();
            currentFileFormat=selFormat;
        }
    }

    //qDebug()<<"fn="<<fn<<"  selFormat="<<selFormat;

    saveUserSettings();
    if (!fn.isEmpty()) {
        int filtID=filt.indexOf(selFormat);
        bool isWithSpecialDeviceAdapter=filtID>=filtStartSize && filtID<filtStartSize+jkqtpPaintDeviceAdapters.size();
        int adapterID=filtID-filtStartSize;
        QString e=QFileInfo(filename).suffix().toLower();
        if (!isWithSpecialDeviceAdapter) {
            for (int i=0; i<jkqtpPaintDeviceAdapters.size(); i++) {
                if (jkqtpPaintDeviceAdapters[i]->getFileExtension().contains(e)) {
                    adapterID=i;
                    isWithSpecialDeviceAdapter=true;
                    break;
                }
            }
        }
        //qDebug()<<"filtID="<<filtID<<"   isWithSpecialDeviceAdapter="<<isWithSpecialDeviceAdapter<<"   adapterID="<<adapterID;
        int ids=0;
        if (filtID==ids++) {
            saveAsPDF(fn, displayPreview);
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        } else if (filtID==ids++) {
            saveAsPS(fn, displayPreview);
#endif
        } else if (filtID==ids++) {
            saveAsSVG(fn, displayPreview);
        } else if (isWithSpecialDeviceAdapter && adapterID>=0 && adapterID<jkqtpPaintDeviceAdapters.size()) {
            QString tempFM="";
            if (QFile::exists(fn)) {
#ifdef QFWIDLIB_LIBRARY
                QFTemporaryFile* tf=new QFTemporaryFile();
#else
                QTemporaryFile* tf=new QTemporaryFile();
#endif
                tf->open();
                tempFM=tf->fileName();
                tf->close();
                delete tf;
                QFile::copy(fn, tempFM);
            }

            mathText.setUseUnparsed(!jkqtpPaintDeviceAdapters[adapterID]->useLatexParser());

            gridPrintingCalc();
            QPaintDevice* svg=jkqtpPaintDeviceAdapters[adapterID]->createPaintdevice(fn, gridPrintingSize.width(), gridPrintingSize.height());

            if (!printpreviewNew(svg, jkqtpPaintDeviceAdapters[adapterID]->getSetAbsolutePaperSize(), jkqtpPaintDeviceAdapters[adapterID]->getPrintSizeXInMM(), jkqtpPaintDeviceAdapters[adapterID]->getPrintSizeYInMM(), displayPreview)) {
                delete svg;


                if (QFile::exists(tempFM)) {
                    QFile::copy(tempFM, fn);
                    QFile::remove(tempFM);
                }
            } else {
                delete svg;
                svg=jkqtpPaintDeviceAdapters[adapterID]->createPaintdeviceMM(fn,printSizeX_Millimeter,printSizeY_Millimeter);
                printpreviewPaintRequestedNew(svg);
                delete svg;
            }

        } else {
            saveAsPixelImage(fn, displayPreview, writerformats.value(filtID-qtwritersidx, QByteArray()));
        }
    }
}


void JKQTBasePlotter::saveAsPixelImage(const QString& filename, bool displayPreview, const QByteArray& outputFormat) {
    loadUserSettings();
    QString fn=filename;
    QStringList filt;
    QList<QByteArray> writerformats=QImageWriter::supportedImageFormats();
    for (int i=0; i<writerformats.size(); i++) {
        filt<<QString("%1 Image (*.%2)").arg(QString(writerformats[i]).toUpper()).arg(QString(writerformats[i].toLower()));
    }
    /*filt<<tr("PNG Image (*.png)");
    filt<<tr("BMP Image (*.bmp)");
    filt<<tr("TIFF Image (*.tif *.tiff)");
    filt<<tr("JPEG Image (*.jpg *.jpeg)");
    filt<<tr("X11 Bitmap (*.xbm)");
    filt<<tr("X11 Pixmap (*.xpm)");*/
    QString selFormat;
    if (fn.isEmpty()) {
        selFormat=currentFileFormat;
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot"),
                                    currentSaveDirectory,
                                          filt.join(";;"), &selFormat);
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    saveUserSettings();
    if (!fn.isEmpty()) {
        int filtID=filt.indexOf(selFormat);
        //QString ext=tolower(extract_file_ext(fn.toStdString()));
        QString form="NONE";
        if (filtID>=0 && filtID<writerformats.size()) {
            form=writerformats[filtID];
        }
        if (outputFormat.size()>0) {
            form =outputFormat;
        }
        /*if (filtID==2 || ext=="tif" || ext=="tiff") { // currentFileFormat
            form="TIFF";
        } else if (filtID==0 || ext=="png") {
            form="PNG";
        } else if (filtID==1 || ext=="bmp") {
            form="BMP";
        } else if (filtID==3 || ext=="jpg" || ext=="jpeg") {
            form="JPEG";
        } else if (ext=="ppm") {
            form="PPM";
        } else if (filtID==4 || ext=="xbm") {
            form="XBM";
        } else if (filtID==5 || ext=="xpm") {
            form="XPM";
        }*/


        gridPrintingCalc();
        //std::cout<<gridPrintingSize.width()<<", "<<gridPrintingSize.height()<<std::endl;

        if (!displayPreview) {
            printSizeX_Millimeter=widgetWidth;
            printSizeY_Millimeter=widgetHeight;
        }

        if (!displayPreview || exportpreview(gridPrintingSize, false)) {

            QImage png(QSize(double(printSizeX_Millimeter)*1.1, double(printSizeY_Millimeter)*1.1), QImage::Format_ARGB32);
            png.fill(Qt::transparent);
            JKQTPEnhancedPainter painter;
            painter.begin(&png);
            painter.setRenderHint(JKQTPEnhancedPainter::NonCosmeticDefaultPen, true);
            painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
            painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
            painter.setRenderHint(JKQTPEnhancedPainter::SmoothPixmapTransform);
            painter.setRenderHint(JKQTPEnhancedPainter::HighQualityAntialiasing);

            /*calcPlotScaling(painter);
            gridPaint(painter, png.rect().size());*/\
            //qDebug()<<QSize(printSizeX_Millimeter, printSizeY_Millimeter);
            exportpreviewPaintRequested(painter, QSize(printSizeX_Millimeter, printSizeY_Millimeter));
            painter.end();
            if (form=="NONE") png.save(fn);
            else png.save(fn, form.toLatin1().data());
        }
    }
}


void JKQTBasePlotter::copyPixelImage() {
    /*qDebug()<<gridPrintingSize.width()<<", "<<gridPrintingSize.height();
    qDebug()<<widgetWidth<<", "<<widgetHeight;
    qDebug()<<paintMagnification;*/
   /* QImage png(gridPrintingSize, QImage::Format_ARGB32);
    {
        JKQTPEnhancedPainter painter;
        painter.begin(&png);
        painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
        painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
        painter.setRenderHint(JKQTPEnhancedPainter::SmoothPixmapTransform);
        painter.setRenderHint(JKQTPEnhancedPainter::HighQualityAntialiasing);
        //calcPlotScaling(painter);
        gridPaint(painter, gridPrintingSize);
        painter.end();
    }

    QClipboard *clipboard = QApplication::clipboard();
    qDebug()<<"clipboard before adding content:\n"<<clipboard->mimeData()->formats();
    clipboard->setImage(png);
    qDebug()<<"clipboard after adding content:\n"<<clipboard->mimeData()->formats();

*/

    gridPrintingCalc();
    //std::cout<<gridPrintingSize.width()<<", "<<gridPrintingSize.height()<<std::endl;
    printSizeX_Millimeter=gridPrintingSize.width();
    printSizeY_Millimeter=gridPrintingSize.height();

    if (exportpreview(gridPrintingSize, false)) {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        //if (exportpreview(gridPrintingSize, false)) {
        QImage png(QSize(double(printSizeX_Millimeter), double(printSizeY_Millimeter)), QImage::Format_ARGB32);
            {
                png.fill(Qt::transparent);
                JKQTPEnhancedPainter painter;
                painter.begin(&png);
                painter.setRenderHint(JKQTPEnhancedPainter::NonCosmeticDefaultPen, true);
                painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
                painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
                painter.setRenderHint(JKQTPEnhancedPainter::SmoothPixmapTransform);
                painter.setRenderHint(JKQTPEnhancedPainter::HighQualityAntialiasing);

                /*calcPlotScaling(painter);
                gridPaint(painter, png.rect().size());*/
                exportpreviewPaintRequested(painter, QSize(printSizeX_Millimeter, printSizeY_Millimeter));
                painter.end();
            }
        QByteArray svgdata;
            {

                QBuffer buffer(&svgdata);
                QSvgGenerator* svg=new QSvgGenerator;
                svg->setResolution(96);
                //svg->setResolution(300);
                QSize size=QSize(printSizeX_Millimeter, printSizeX_Millimeter);
                double factor=double(size.width())/double(widgetWidth)*paintMagnification;
                // TODO: CORRECT THIS
                //qDebug()<<size;
                svg->setSize(size);
                svg->setOutputDevice(&buffer);
                JKQTPEnhancedPainter painter;
                painter.begin(svg);
                painter.scale(factor,factor);
                printAspect=printSizeY_Millimeter/printSizeX_Millimeter;
                exportpreviewPaintRequested(painter, QSize(widgetWidth/paintMagnification, widgetWidth/paintMagnification*printAspect));
                painter.end();
                delete svg;

            }





            QClipboard *clipboard = QApplication::clipboard();
            //qDebug()<<"clipboard before adding content:\n"<<clipboard->mimeData()->formats();
            //clipboard->setImage(png);
            clipboard->clear();
            clipboard->setPixmap(QPixmap::fromImage(png));
            QMimeData* mime=new QMimeData();
            mime->setImageData(QPixmap::fromImage(png));
            QBuffer pngbuf;
            png.save(&pngbuf, "png");
            mime->setData("image/x-png", pngbuf.data());
            png.save(&pngbuf, "bmp");
            mime->setData("image/bmp", pngbuf.data());
            mime->setData("image/svg+xml", svgdata);
            clipboard->setMimeData(mime);
            //qDebug()<<"clipboard after adding content:\n"<<clipboard->mimeData()->formats();




            QApplication::restoreOverrideCursor();

        //}
    }


}

void JKQTBasePlotter::saveAsSVG(const QString& filename, bool displayPreview) {
    loadUserSettings();
    QString fn=filename;
    if (fn.isEmpty()) {
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot"),
                                    currentSaveDirectory,
                                    tr("SVG Image (*.svg)"));
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    if (!fn.isEmpty()) {
        QString tempFM="";
        if (QFile::exists(fn)) {
#ifdef QFWIDLIB_LIBRARY
                QFTemporaryFile* tf=new QFTemporaryFile();
#else
                QTemporaryFile* tf=new QTemporaryFile();
#endif
            tf->open();
            tempFM=tf->fileName();
            tf->close();
            delete tf;
            QFile::copy(fn, tempFM);
        }

        gridPrintingCalc();
        QSvgGenerator* svg=new QSvgGenerator;
        svg->setResolution(96);
        QSize size=QSize(gridPrintingSize.width()*25.4/svg->resolution(), gridPrintingSize.height()*25.4/svg->resolution());
        svg->setSize(size);
        svg->setFileName(fn);

        if (!printpreviewNew(svg, true, -1.0, -1.0, displayPreview)) {
            if (QFile::exists(tempFM)) {
                QFile::copy(tempFM, fn);
                QFile::remove(tempFM);
            }
        }

        delete svg;

    }
    saveUserSettings();
}


void JKQTBasePlotter::setBorder(int left, int right, int top, int bottom){
    plotBorderTop=top;
    plotBorderLeft=left;
    plotBorderBottom=bottom;
    plotBorderRight=right;
    //updateGeometry();
    if (emitPlotSignals) emit plotUpdated();
}

void JKQTBasePlotter::synchronizeToMaster(JKQTBasePlotter* master, SynchronizationDirection synchronizeDirection, bool synchronizeAxisLength, bool synchronizeZoomingMasterToSlave, bool synchronizeZoomingSlaveToMaster) {
    // remove old connections
    if (masterPlotterX && (synchronizeDirection==sdXAxis ||  synchronizeDirection==sdXYAxes)) {
        disconnect(masterPlotterX, SIGNAL(zoomChangedLocally(double,double,double,double,JKQTBasePlotter*)),
                       this, SLOT(synchronizeXAxis(double,double,double,double,JKQTBasePlotter*)));
        disconnect(this, SIGNAL(zoomChangedLocally(double,double,double,double,JKQTBasePlotter*)),
                       masterPlotterX, SLOT(synchronizeXAxis(double,double,double,double,JKQTBasePlotter*)));
    }
    if (masterPlotterY && (synchronizeDirection==sdYAxis ||  synchronizeDirection==sdXYAxes)) {
        disconnect(masterPlotterY, SIGNAL(zoomChangedLocally(double,double,double,double,JKQTBasePlotter*)),
                       this, SLOT(synchronizeYAxis(double,double,double,double,JKQTBasePlotter*)));
        disconnect(this, SIGNAL(zoomChangedLocally(double,double,double,double,JKQTBasePlotter*)),
                       masterPlotterY, SLOT(synchronizeYAxis(double,double,double,double,JKQTBasePlotter*)));
    }

    // store new connection settings
    if (synchronizeDirection==sdXAxis ||  synchronizeDirection==sdXYAxes) {
        masterPlotterX=master;
        masterSynchronizeWidth=synchronizeAxisLength;
    }
    if (synchronizeDirection==sdYAxis ||  synchronizeDirection==sdXYAxes) {
        masterPlotterY=master;
        masterSynchronizeHeight=synchronizeAxisLength;
    }

    // connect widgets (if required)
    if (master) {
        if (synchronizeDirection==sdXAxis ||  synchronizeDirection==sdXYAxes) {
            if (synchronizeZoomingMasterToSlave) {
                connect(masterPlotterX, SIGNAL(zoomChangedLocally(double,double,double,double,JKQTBasePlotter*)),
                                   this, SLOT(synchronizeXAxis(double,double,double,double,JKQTBasePlotter*)));
            }
            if (synchronizeZoomingSlaveToMaster) {
                connect(this, SIGNAL(zoomChangedLocally(double,double,double,double,JKQTBasePlotter*)),
                                   masterPlotterX, SLOT(synchronizeXAxis(double,double,double,double,JKQTBasePlotter*)));

            }
        }
        if (synchronizeDirection==sdYAxis ||  synchronizeDirection==sdXYAxes) {
            if (synchronizeZoomingMasterToSlave) {
                connect(masterPlotterY, SIGNAL(zoomChangedLocally(double,double,double,double,JKQTBasePlotter*)),
                                   this, SLOT(synchronizeYAxis(double,double,double,double,JKQTBasePlotter*)));
            }
            if (synchronizeZoomingSlaveToMaster) {
                connect(this, SIGNAL(zoomChangedLocally(double,double,double,double,JKQTBasePlotter*)),
                                   masterPlotterY, SLOT(synchronizeYAxis(double,double,double,double,JKQTBasePlotter*)));

            }
        }
    }

}

void JKQTBasePlotter::synchronizeXToMaster(JKQTBasePlotter *master, bool synchronizeAxisLength, bool synchronizeZoomingMasterToSlave, bool synchronizeZoomingSlaveToMaster)
{
    synchronizeToMaster(master, sdXAxis, synchronizeAxisLength, synchronizeZoomingMasterToSlave, synchronizeZoomingSlaveToMaster);
}

void JKQTBasePlotter::synchronizeYToMaster(JKQTBasePlotter *master, bool synchronizeAxisLength, bool synchronizeZoomingMasterToSlave, bool synchronizeZoomingSlaveToMaster)
{
    synchronizeToMaster(master, sdYAxis, synchronizeAxisLength, synchronizeZoomingMasterToSlave, synchronizeZoomingSlaveToMaster);
}

void JKQTBasePlotter::resetMasterSynchronization(JKQTBasePlotter::SynchronizationDirection synchronizeDirection) {
    synchronizeToMaster(nullptr, synchronizeDirection, false, false, false);
}


void JKQTBasePlotter::synchronizeXAxis(double newxmin, double newxmax, double /*newymin*/, double /*newymax*/, JKQTBasePlotter * /*sender*/) {
    bool oldemitSignals=emitSignals;
    emitSignals=false;
    setX(newxmin, newxmax);
    emitSignals=oldemitSignals;
}

void JKQTBasePlotter::synchronizeYAxis(double /*newxmin*/, double /*newxmax*/, double newymin, double newymax, JKQTBasePlotter * /*sender*/) {
    bool oldemitSignals=emitSignals;
    emitSignals=false;
    setY(newymin, newymax);
    emitSignals=oldemitSignals;
}

void JKQTBasePlotter::synchronizeXYAxis(double newxmin, double newxmax, double newymin, double newymax, JKQTBasePlotter * /*sender*/) {
    bool oldemitSignals=emitSignals;
    emitSignals=false;
    setXY(newxmin, newxmax, newymin, newymax);
    emitSignals=oldemitSignals;
}

size_t JKQTBasePlotter::addGraph(size_t xColumn, size_t yColumn, const QString& title, JKQTPGraphPlotstyle graphStyle) {
    if (graphStyle==JKQTPImpulsesHorizontal) {
        JKQTPImpulsesHorizontalGraph* gr=new JKQTPImpulsesHorizontalGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        return addGraph(gr);
    } else if (graphStyle==JKQTPImpulsesVertical) {
        JKQTPImpulsesVerticalGraph* gr=new JKQTPImpulsesVerticalGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        return addGraph(gr);
    } else if (graphStyle==JKQTPFilledCurveX) {
        JKQTPFilledCurveXGraph* gr=new JKQTPFilledCurveXGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        return addGraph(gr);
    } else if (graphStyle==JKQTPFilledCurveY) {
        JKQTPFilledCurveYGraph* gr=new JKQTPFilledCurveYGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        return addGraph(gr);
    } else if (graphStyle==JKQTPStepsX) {
        JKQTPStepHorizontalGraph* gr=new JKQTPStepHorizontalGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        return addGraph(gr);
    } else if (graphStyle==JKQTPStepsY) {
        JKQTPStepVerticalGraph* gr=new JKQTPStepVerticalGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        return addGraph(gr);
    } else {
        JKQTPXYLineGraph* gr=new JKQTPXYLineGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        if (graphStyle==JKQTPPoints) { gr->setSymbol(JKQTPPlus); gr->setDrawLine(false); }
        else if (graphStyle==JKQTPLinesPoints) gr->setSymbol(JKQTPPlus);
        return addGraph(gr);
    }
    return -1;
}

size_t JKQTBasePlotter::addGraph(size_t xColumn, size_t yColumn, const QString& title, JKQTPGraphPlotstyle graphStyle, QColor color, JKQTPGraphSymbols symbol, Qt::PenStyle penstyle, double width) {
    if (graphStyle==JKQTPImpulsesHorizontal) {
        JKQTPImpulsesHorizontalGraph* gr=new JKQTPImpulsesHorizontalGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        gr->setColor(color);
        gr->setLineWidth(width);
        return addGraph(gr);
    } else if (graphStyle==JKQTPImpulsesVertical) {
        JKQTPImpulsesVerticalGraph* gr=new JKQTPImpulsesVerticalGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        gr->setColor(color);
        gr->setLineWidth(width);
        return addGraph(gr);
    } else if (graphStyle==JKQTPFilledCurveX) {
        JKQTPFilledCurveXGraph* gr=new JKQTPFilledCurveXGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        gr->setColor(QColor("black"));
        gr->setLineWidth(width);
        gr->setStyle(penstyle);
        gr->setFillColor(color);
        return addGraph(gr);
    } else if (graphStyle==JKQTPFilledCurveY) {
        JKQTPFilledCurveYGraph* gr=new JKQTPFilledCurveYGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        gr->setColor(QColor("black"));
        gr->setLineWidth(width);
        gr->setStyle(penstyle);
        gr->setFillColor(color);
        return addGraph(gr);
    } else if (graphStyle==JKQTPStepsX) {
        JKQTPStepHorizontalGraph* gr=new JKQTPStepHorizontalGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        gr->setColor(QColor("black"));
        gr->setLineWidth(width);
        gr->setStyle(penstyle);
        gr->setFillColor(color);
        return addGraph(gr);
    } else if (graphStyle==JKQTPStepsY) {
        JKQTPStepVerticalGraph* gr=new JKQTPStepVerticalGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        gr->setColor(QColor("black"));
        gr->setLineWidth(width);
        gr->setStyle(penstyle);
        gr->setFillColor(color);
        return addGraph(gr);
    } else {
        JKQTPXYLineGraph* gr=new JKQTPXYLineGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        gr->setLineWidth(width);
        gr->setStyle(penstyle);
        gr->setColor(color);
        gr->setSymbol(symbol);
        if (graphStyle==JKQTPPoints) { gr->setSymbol(JKQTPPlus); gr->setDrawLine(false); }
        else if (graphStyle==JKQTPLinesPoints) gr->setSymbol(JKQTPPlus);
        return addGraph(gr);
    }
    return -1;
}

size_t JKQTBasePlotter::addHorizontalBoxplot(QString title, int posColumn, int medianColumn, int minColumn, int maxColumn, int percentile25Column, int percentile75Column, int meanColumn) {
    JKQTPBoxplotHorizontalGraph* box=new JKQTPBoxplotHorizontalGraph(this);
    box->setPosColumn(posColumn);
    box->setMedianColumn(medianColumn);
    box->setMeanColumn(meanColumn);
    box->setMinColumn(minColumn);
    box->setMaxColumn(maxColumn);
    box->setPercentile25Column(percentile25Column);
    box->setPercentile75Column(percentile75Column);
    box->setTitle(title);
    return addGraph(box);
}

size_t JKQTBasePlotter::addVerticalBoxplot(QString title, int posColumn, int medianColumn, int minColumn, int maxColumn, int percentile25Column, int percentile75Column, int meanColumn) {
    JKQTPBoxplotVerticalGraph* box=new JKQTPBoxplotVerticalGraph(this);
    box->setPosColumn(posColumn);
    box->setMedianColumn(medianColumn);
    box->setMeanColumn(meanColumn);
    box->setMinColumn(minColumn);
    box->setMaxColumn(maxColumn);
    box->setPercentile25Column(percentile25Column);
    box->setPercentile75Column(percentile75Column);
    box->setTitle(title);
    return addGraph(box);
}

void JKQTBasePlotter::addHorizontalBargraph(size_t xColumn, QVector<size_t> yColumns, QStringList titles) {
    double w=0.9;
    double width=w/static_cast<double>(yColumns.size());
    double s=-1.0*w/2.0+width/2.0;
    for (int i=0; i<yColumns.size(); i++) {
        JKQTPBarVerticalGraph* g=new JKQTPBarVerticalGraph(this);
        g->setTitle(titles[i]);
        g->setXColumn(xColumn);
        g->setYColumn(yColumns[i]);
        g->setShift(s);
        g->setWidth(width);
        //std::cout<<"shift="<<s<<"   width="<<width<<std::endl;
        s=s+width;
        addGraph(g);
    }
}

void JKQTBasePlotter::addVerticalBargraph(QVector<size_t> xColumns, size_t yColumn, QStringList titles) {
    double w=0.9;
    double width=w/static_cast<double>(xColumns.size());
    double s=-1.0*w/2.0+width/2.0;
    for (int i=0; i<xColumns.size(); i++) {
        JKQTPBarHorizontalGraph* g=new JKQTPBarHorizontalGraph(this);
        g->setTitle(titles[i]);
        g->setXColumn(xColumns[i]);
        g->setYColumn(yColumn);
        g->setShift(s);
        g->setWidth(width);
        //std::cout<<"shift="<<s<<"   width="<<width<<std::endl;
        s=s+width;
        addGraph(g);
    }
}

size_t JKQTBasePlotter::addGraphWithXError(size_t xColumn, size_t yColumn, size_t xErrorColumn, const QString& title, JKQTPGraphPlotstyle graphStyle, JKQTPErrorPlotstyle errorStyle){
    if (graphStyle==JKQTPImpulsesHorizontal) {
        JKQTPImpulsesHorizontalErrorGraph* gr=new JKQTPImpulsesHorizontalErrorGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        gr->setXErrorStyle(errorStyle);
        gr->setXErrorColumn(xErrorColumn);
        gr->setErrorColor(gr->getColor().darker());
        QColor fc=gr->getColor();
        fc.setAlphaF(0.5);
        gr->setErrorFillColor(fc);
        return addGraph(gr);
    } else if (graphStyle==JKQTPFilledCurveY) {
        JKQTPFilledCurveYErrorGraph* gr=new JKQTPFilledCurveYErrorGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        gr->setXErrorStyle(errorStyle);
        gr->setXErrorColumn(xErrorColumn);
        gr->setErrorColor(gr->getColor());
        QColor fc=gr->getColor();
        fc.setAlphaF(0.5);
        gr->setErrorFillColor(fc);
        return addGraph(gr);
    } else {
        JKQTPXYLineErrorGraph* gr=new JKQTPXYLineErrorGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        gr->setYErrorColumn(xErrorColumn);
        gr->setYErrorStyle(errorStyle);
        gr->setXErrorStyle(JKQTPNoError);
        gr->setErrorColor(gr->getColor());
        QColor fc=gr->getColor();
        fc.setAlphaF(0.5);
        gr->setErrorFillColor(fc);
        if (graphStyle==JKQTPPoints) { gr->setSymbol(JKQTPPlus); gr->setDrawLine(false); }
        else if (graphStyle==JKQTPLinesPoints) gr->setSymbol(JKQTPPlus);
        return addGraph(gr);
    }

}

size_t JKQTBasePlotter::addGraphWithYError(size_t xColumn, size_t yColumn, size_t yErrorColumn, const QString& title, JKQTPGraphPlotstyle graphStyle, JKQTPErrorPlotstyle errorStyle){
    if (graphStyle==JKQTPImpulsesVertical) {
        JKQTPImpulsesVerticalErrorGraph* gr=new JKQTPImpulsesVerticalErrorGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        gr->setYErrorStyle(errorStyle);
        gr->setYErrorColumn(yErrorColumn);
        gr->setErrorColor(gr->getColor().darker());
        gr->setErrorWidth(gr->getLineWidth()/3.0);
        QColor fc=gr->getColor();
        fc.setAlphaF(0.5);
        gr->setErrorFillColor(fc);
        return addGraph(gr);
    } else if (graphStyle==JKQTPFilledCurveX) {
        JKQTPFilledCurveXErrorGraph* gr=new JKQTPFilledCurveXErrorGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        gr->setYErrorStyle(errorStyle);
        gr->setYErrorColumn(yErrorColumn);
        gr->setErrorColor(gr->getColor());
        QColor fc=gr->getColor();
        fc.setAlphaF(0.5);
        gr->setErrorFillColor(fc);
        return addGraph(gr);
    } else {
        JKQTPXYLineErrorGraph* gr=new JKQTPXYLineErrorGraph(this);
        gr->setTitle(title);
        gr->setXColumn(xColumn);
        gr->setYColumn(yColumn);
        gr->setYErrorColumn(yErrorColumn);
        gr->setYErrorStyle(errorStyle);
        gr->setXErrorStyle(JKQTPNoError);
        gr->setErrorColor(gr->getColor());
        QColor fc=gr->getColor();
        fc.setAlphaF(0.5);
        gr->setErrorFillColor(fc);
        if (graphStyle==JKQTPPoints) { gr->setSymbol(JKQTPPlus); gr->setDrawLine(false); }
        else if (graphStyle==JKQTPLinesPoints) gr->setSymbol(JKQTPPlus);
        return addGraph(gr);
    }
}

size_t JKQTBasePlotter::addGraphWithXYError(size_t xColumn, size_t yColumn, size_t xErrorColumn, size_t yErrorColumn, const QString& title, JKQTPGraphPlotstyle graphStyle){
    JKQTPXYLineErrorGraph* gr=new JKQTPXYLineErrorGraph(this);
    gr->setTitle(title);
    gr->setXColumn(xColumn);
    gr->setYColumn(yColumn);
    gr->setYErrorColumn(yErrorColumn);
    gr->setYErrorStyle(JKQTPErrorBars);
    gr->setXErrorColumn(xErrorColumn);
    gr->setXErrorStyle(JKQTPErrorBars);
    gr->setErrorColor(gr->getColor());
    QColor fc=gr->getColor();
    fc.setAlphaF(0.5);
    gr->setErrorFillColor(fc);
    if (graphStyle==JKQTPPoints) { gr->setSymbol(JKQTPPlus); gr->setDrawLine(false); }
    else if (graphStyle==JKQTPLinesPoints) gr->setSymbol(JKQTPPlus);
    return addGraph(gr);

}



void JKQTBasePlotter::plotGraphs(JKQTPEnhancedPainter& painter){
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTBasePlotter::plotGraphs"));
#endif
    if (datastore==nullptr || graphs.isEmpty()) return;
    //qDebug()<<"start JKQTBasePlotter::plotGraphs()";
    if (useClipping) {
        QRegion cregion(iplotBorderLeft, iplotBorderTop, plotWidth, plotHeight);
        painter.setClipping(true);
        painter.setClipRegion(cregion);
    }

    int ibTop=iplotBorderTop_nographs-plotBorderTop;
    int ibLeft=iplotBorderLeft_nographs-plotBorderLeft;
    int ibBottom=iplotBorderBottom_nographs-plotBorderBottom;
    int ibRight=iplotBorderRight_nographs-plotBorderRight;


    for (int j=0; j<graphs.size(); j++) {
        //int leftSpace, rightSpace, topSpace, bottomSpace;
        JKQTPPlotElement* g=graphs[j];
        //qDebug()<<"  drawing JKQTPPlotElement"<<j<<g->getTitle()<<g->metaObject()->className();
        if (g->isVisible()) g->draw(painter);
    }

    if (useClipping) {
        painter.setClipping(false);
    }

    for (int j=0; j<graphs.size(); j++) {
        int leftSpace, rightSpace, topSpace, bottomSpace;
        JKQTPPlotElement* g=graphs[j];
        if (g->isVisible()) {
            g->getOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
            ibTop+=topSpace;
            ibLeft+=leftSpace;
            g->drawOutside(painter, QRect(iplotBorderLeft+iplotKeyBorderLeft-ibLeft, iplotBorderTop, leftSpace, plotHeight),
                                    QRect(iplotBorderLeft+plotWidth+ibRight-iplotKeyBorderRight, iplotBorderTop, rightSpace, plotHeight),
                                    QRect(iplotBorderLeft, iplotBorderTop-ibTop+iplotKeyBorderTop, plotWidth, topSpace),
                                    QRect(iplotBorderLeft, iplotBorderTop+plotHeight+ibBottom, plotWidth, bottomSpace)
                           );
            ibRight+=rightSpace;
            ibBottom+=bottomSpace;
        }
    }

    //qDebug()<<"  end JKQTBasePlotter::plotGraphs()";
}


void JKQTBasePlotter::plotKeyContents(JKQTPEnhancedPainter& painter, double x, double y, double /*width*/, double /*height*/){
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTBasePlotter::plotKeyContents(%1, %2, %3, %4)").arg(x).arg(y).arg(width).arg(height));
#endif
    painter.save();

    double key_text_width=0;
    double key_text_height=0;
    double dw, dh;
    int columns=1;
    int lines=1;
    getKeyExtent(painter, &dw, &dh, &key_text_width, &key_text_height, &columns, &lines);

#ifdef SHOW_JKQTPLOTTER_DEBUG
    qDebug()<<"key_item_height="<<key_item_height;
    qDebug()<<"plotKey(): dw="<<dw<<"  dh="<<dh<<"  key_text_width="<<key_text_width<<"   key_text_height="<<key_text_height<<"  columns="<<columns<<"   lines="<<lines<<"   keyLayout="<<keyLayout;
#endif
    QFont kf(keyFont, keyFontSize*fontSizeMultiplier);
    QFontMetricsF kfm(kf);

    if (keyLayout==JKQTPKeyLayoutOneColumn) {

        for (int i=0; i<graphs.size(); i++) {
            JKQTPPlotElement* g=graphs[i];
#ifdef JKQTBP_AUTOTIMER
            jkaaot.write(QString("one-col: graph %1: %2").arg(i).arg(g->getTitle()));
#endif
            if (!g->getTitle().isEmpty() && g->isVisible()) {
                QSizeF fs=getTextSizeSize(keyFont,keyFontSize*fontSizeMultiplier,g->getTitle(),painter);//  mt.getSize(painter);
                double itheight=qMax(key_item_height*kfm.width('X'), fs.height());
                QRectF rect(x, y+1.5*lineWidthMultiplier, key_line_length*kfm.width('X'), itheight-3.0*lineWidthMultiplier);
                g->drawKeyMarker(painter, rect);
                mathText.setFontColor(QColor("black"));
                mathText.setFontSize(keyFontSize*fontSizeMultiplier);
                mathText.setFontRoman(keyFont);

                mathText.parse(g->getTitle());
                mathText.draw(painter, Qt::AlignLeft|Qt::AlignVCenter, QRectF(x+(key_line_length+keyXSeparation)*kfm.width('X'),y, key_text_width, itheight));
                //if (itheight<key_item_height*kfm.height()) itheight=key_item_height*kfm.height();
                //y=y+itheight+(keyYSeparation)*kfm.height();
                y=y+key_text_height+(keyYSeparation)*kfm.width('X');
            }
        }
    } else if (keyLayout==JKQTPKeyLayoutOneRow) {

        for (int i=0; i<graphs.size(); i++) {
            JKQTPPlotElement* g=graphs[i];
#ifdef JKQTBP_AUTOTIMER
            jkaaot.write(QString("one-row: graph %1: %2").arg(i).arg(g->getTitle()));
#endif
            if (!g->getTitle().isEmpty() && g->isVisible()) {
                QSizeF fs=getTextSizeSize(keyFont,keyFontSize*fontSizeMultiplier,g->getTitle(),painter);//  mt.getSize(painter);
                double itheight=qMax(key_item_height*kfm.width('X'), fs.height());
                QRectF rect(x, y+1.5*lineWidthMultiplier, key_line_length*kfm.width('X'), itheight-3.0*lineWidthMultiplier);
                g->drawKeyMarker(painter, rect);
                mathText.setFontColor(QColor("black"));
                mathText.setFontSize(keyFontSize*fontSizeMultiplier);
                mathText.setFontRoman(keyFont);

                mathText.parse(g->getTitle());
                mathText.draw(painter, Qt::AlignLeft|Qt::AlignVCenter, QRectF(x+(key_line_length+keyXSeparation)*kfm.width('X'),y, fs.width(), itheight));
                //if (itheight<key_item_height*kfm.height()) itheight=key_item_height*kfm.height();
                //y=y+itheight+(keyYSeparation)*kfm.height();
                x=x+fs.width()+(2.0*keyXSeparation+key_line_length)*kfm.width('X');
            }
        }
    } else if (keyLayout==JKQTPKeyLayoutMultiColumn) {
        //int columns=floor(static_cast<double>(plotWidth)/static_cast<double>(key_item_width*kfm.width('X')));
        bool colfirst=true;

        if (keyPosition==JKQTPKeyInsideTopLeft || keyPosition==JKQTPKeyInsideTopRight
            || keyPosition==JKQTPKeyOutsideTopLeft || keyPosition==JKQTPKeyOutsideTopRight) {
            colfirst=false;
        }

        int l=1;
        int c=1;
        double xx=x;
        double yy=y;
        for (int i=0; i<graphs.size(); i++) {
            JKQTPPlotElement* g=graphs[i];
#ifdef JKQTBP_AUTOTIMER
            jkaaot.write(QString("multi-col: graph %1: %2").arg(i).arg(g->getTitle()));
#endif
            if (!g->getTitle().isEmpty() && g->isVisible()) {
                //QSizeF fs=getTextSizeSize(keyFont,keyFontSize*fontSizeMultiplier,g->getTitle(),painter);//  mt.getSize(painter);
                double itheight=qMax(key_item_height*kfm.width('X'), key_text_height);
                QRectF rect(xx, yy+1.5*lineWidthMultiplier, key_line_length*kfm.width('X'), itheight-3.0*lineWidthMultiplier);
                g->drawKeyMarker(painter, rect);
                mathText.setFontColor(QColor("black"));
                mathText.setFontSize(keyFontSize*fontSizeMultiplier);
                mathText.setFontRoman(keyFont);
                mathText.parse(g->getTitle());
                //QSizeF fs=mt.getSize(painter);
                mathText.draw(painter, Qt::AlignLeft|Qt::AlignVCenter, QRectF(xx+(key_line_length+keyXSeparation)*kfm.width('X'),yy, key_text_width, key_text_height));

                if (colfirst) {
                    yy=yy+key_text_height+(keyYSeparation)*kfm.width('X');
                    l++;
                    if (l>lines) {
                        l=1;
                        c++;
                        xx=xx+key_text_width+(key_line_length+2.0*keyXSeparation)*kfm.width('X');
                        /*if (keyAutosize) xx=xx+key_text_width+(key_line_length+3.0*keyXSeparation)*kfm.width('X');
                        else xx=xx+(key_item_width+2.0*keyXSeparation)*kfm.width('X');*/
                        yy=y;
                    }
                } else {
                    /*if (keyAutosize) xx=xx+key_text_width+(key_line_length+3.0*keyXSeparation)*kfm.width('X');
                    else xx=xx+(key_item_width+2.0*keyXSeparation)*kfm.width('X');*/
                    xx=xx+key_text_width+(key_line_length+2.0*keyXSeparation)*kfm.width('X');
                    c++;
                    if (c>columns) {
                        c=1;
                        l++;
                        //yy=yy+(key_item_height+keyYSeparation)*kfm.height();
                        yy=yy+itheight+(keyYSeparation)*kfm.width('X');
                        xx=x;
                    }
                }
            }
        }
    }

    /*painter.setPen(pold);
    painter.setBrush(bold);*/

    painter.restore();
}

void JKQTBasePlotter::getKeyExtent(JKQTPEnhancedPainter& painter, double* width, double* height, double* text_width, double* text_height, int* columns_count, int *lines_count) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTBasePlotter::getKeyExtent");
#endif
    QFont f=painter.font();
    f.setFamily(keyFont);
    f.setPointSizeF(keyFontSize*fontSizeMultiplier);
    QFontMetricsF kfm(f);
    if (text_height!=nullptr) *text_height=key_item_height*kfm.width('X');
    if (keyLayout==JKQTPKeyLayoutOneColumn) {
        int keyHeight=graphs.size();
        double w=0;
        double h=0;
        painter.setFont(f);
        if (text_height!=nullptr) *text_height=0;


        for (int i=0; i<graphs.size(); i++) {
            if (graphs[i]->getTitle().isEmpty() || !graphs[i]->isVisible()) {
                keyHeight--;
            } else {
                //mt.parse(graphs[i]->getTitle());
                //QSizeF fs=mt.getSize(painter);
                QSizeF fs=getTextSizeSize(keyFont, keyFontSize*fontSizeMultiplier, graphs[i]->getTitle(), painter);
                if (fs.width()>w) w=fs.width();
                if (text_height && fs.height()>*text_height) *text_height=fs.height();
                h=h+qMax(key_item_height*kfm.width('X'), fs.height())+keyYSeparation*kfm.width('X');
            }
        }
        if (keyAutosize) {
            if (width) *width=w+(key_line_length+2.0*keyXSeparation)*kfm.width('X');
            if (text_width!=nullptr) *text_width=w+2.0*kfm.width('X');
        } else {
            if (width) *width=key_item_width*kfm.width('X');
            if (text_width!=nullptr) *text_width=(key_item_width-(key_line_length+keyXSeparation))*kfm.width('X');
        }
        if (h>keyYSeparation*kfm.width('X')) h=h-keyYSeparation*kfm.width('X');
        if (height) *height=h;//keyHeight*key_item_height*kfm.width('X');
        if (columns_count) *columns_count=1;
        if (lines_count) *lines_count=keyHeight;
    } else if (keyLayout==JKQTPKeyLayoutOneRow) {
        int keyWidth=graphs.size();
        double w=0;
        double h=0;
        painter.setFont(f);
        if (text_width!=nullptr) *text_width=0;


        for (int i=0; i<graphs.size(); i++) {
            if (graphs[i]->getTitle().isEmpty() || !graphs[i]->isVisible()) {
                keyWidth--;
            } else {
                //mt.parse(graphs[i]->getTitle());
                //QSizeF fs=mt.getSize(painter);
                QSizeF fs=getTextSizeSize(keyFont, keyFontSize*fontSizeMultiplier, graphs[i]->getTitle(), painter);
                if (fs.height()>h) h=fs.height();
                if (text_width && fs.width()>*text_width) *text_width=fs.width();
                w=w+fs.width()+(key_line_length+2.0*keyXSeparation)*kfm.width('X');
            }
        }
        if (h<key_item_height*kfm.width('X')) h=key_item_height*kfm.width('X');
        if (keyAutosize) {
            if (height) *height=h;
            if (text_height!=nullptr) *text_height=h;
        } else {
            if (height) *height=h;
            if (text_height!=nullptr) *text_height=(key_item_height-(keyYSeparation))*kfm.width('X');
        }
        if (w>(keyXSeparation)*kfm.width('X')) w=w-(keyXSeparation)*kfm.width('X');
        if (width) *width=w;//keyHeight*key_item_height*kfm.width('X');
        if (columns_count) *columns_count=keyWidth;
        if (lines_count) *lines_count=1;
    } else if (keyLayout==JKQTPKeyLayoutMultiColumn) {

        // copied code in plotKeyContents()!!!
        double keyHeight=graphs.size();
        double w=0;
        double txtH=0;
        QFont f=painter.font();
        f.setFamily(keyFont);
        f.setPointSizeF(keyFontSize*fontSizeMultiplier);
        painter.setFont(f);


        for (int i=0; i<graphs.size(); i++) {
            if (graphs[i]->getTitle().isEmpty() || !graphs[i]->isVisible()) {
                keyHeight--;
            } else {
                //mt.parse(graphs[i]->getTitle());
                //QSizeF fs=mt.getSize(painter);
                QSizeF fs=getTextSizeSize(keyFont, keyFontSize*fontSizeMultiplier, graphs[i]->getTitle(), painter);
                if (fs.width()>w) w=fs.width();
                if ( fs.height()>txtH) txtH=fs.height();
            }
        }

        if (text_height) {
            if (keyAutosize) *text_height=txtH;
            else *text_height=key_item_height*kfm.width('X');
        }


        double columns=floor(double(plotWidth)/(w+(2.0*keyXSeparation+key_line_length)*kfm.width('X')));
        if (!keyAutosize) columns=floor(double(plotWidth)/((key_item_width+2.0*keyXSeparation+key_line_length)*kfm.width('X')));
        columns=qMin(columns, keyHeight);
        int lines=static_cast<int>(ceil(static_cast<double>(keyHeight)/static_cast<double>(columns)));
        lines=qMin(static_cast<double>(lines), keyHeight);

        if (keyPosition==JKQTPKeyInsideTopLeft || keyPosition==JKQTPKeyInsideTopRight
            || keyPosition==JKQTPKeyOutsideTopLeft || keyPosition==JKQTPKeyOutsideTopRight) {
            if (keyAutosize) {
                lines=static_cast<int>(floor(static_cast<double>(plotHeight)/static_cast<double>(txtH+(keyYSeparation)*kfm.width('X'))));
            } else {
                lines=static_cast<int>(floor(static_cast<double>(plotHeight)/static_cast<double>((key_item_height+keyYSeparation)*kfm.width('X'))));
            }
            columns=static_cast<int>(ceil(static_cast<double>(keyHeight)/static_cast<double>(lines)));
            lines=qMin(static_cast<double>(lines), keyHeight);

        }

        if (columns_count) *columns_count=columns;
        if (lines_count) *lines_count=lines;

        if (keyAutosize) {
            if (width) *width=(w+(key_line_length+3.0*keyXSeparation)*kfm.width('X'))*columns;
            if (height) *height=lines*(txtH+keyYSeparation*kfm.width('X'));
            if (lines>0) *height=*height-keyYSeparation*kfm.width('X');
            if (text_width!=nullptr) *text_width=w;
        } else {
            if (width) *width=(key_item_width+2.0*keyXSeparation)*kfm.width('X')*columns;
            if (height) *height=lines*(key_item_height+keyYSeparation)*kfm.width('X');
            if (lines>0) *height=*height-keyYSeparation*kfm.width('X');
            if (text_width!=nullptr) *text_width=(key_item_width-(key_line_length+keyXSeparation))*kfm.width('X');
        }
#ifdef SHOW_JKQTPLOTTER_DEBUG
        qDebug()<<"getKeyExtent(): mult-column: columns="<<columns<<"  lines="<<lines;
#endif

    }
}

void JKQTBasePlotter::loadUserSettings()
{
    if (!userSettigsFilename.isEmpty()) {
        QSettings set(userSettigsFilename, QSettings::IniFormat);
        set.sync();
        loadUserSettings(set, userSettigsPrefix);
    }
}

void JKQTBasePlotter::saveUserSettings() const
{
    if (!userSettigsFilename.isEmpty()) {
        QSettings set(userSettigsFilename, QSettings::IniFormat);
        saveUserSettings(set, userSettigsPrefix);
        set.sync();
    }
}



void JKQTBasePlotter::getGraphsXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;
    for (int i=0; i<graphs.size(); i++) {
        if (graphs[i]->isVisible()) {
            double gminx=0;
            double gmaxx=0;
            double gsmallestGreaterZero=0;
            if (graphs[i]->getXMinMax(gminx, gmaxx, gsmallestGreaterZero)) {
                if ((gminx<minx) || start) minx=gminx;
                if ((gmaxx>maxx) || start) maxx=gmaxx;

                double xvsgz;
                xvsgz=gsmallestGreaterZero; if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;
                xvsgz=gmaxx; if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;
                xvsgz=gminx; if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;

                //std::cout<<"      i="<<i<<": gminx="<<gminx<<"  gmaxx="<<gmaxx<<"   minx="<<minx<<" maxx="<<maxx<<std::endl;

                start=false;
            }
        }
    }
}

void JKQTBasePlotter::getGraphsYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;
    for (int i=0; i<graphs.size(); i++) {
        if (graphs[i]->isVisible()) {
            double gminy=0;
            double gmaxy=0;
            double gsmallestGreaterZero=0;
            if (graphs[i]->getYMinMax(gminy, gmaxy, gsmallestGreaterZero)) {
                if ((gminy<miny) || start) miny=gminy;
                if ((gmaxy>maxy) || start) maxy=gmaxy;

                double xvsgz;
                xvsgz=gsmallestGreaterZero; if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;
                xvsgz=gmaxy; if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;
                xvsgz=gminy; if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;

                start=false;
            }
        }
    }
}


void JKQTBasePlotter::zoomToFit(bool zoomX, bool zoomY, bool includeX0, bool includeY0, double scaleX, double scaleY) {
    //std::cout<<"JKQTBasePlotter::zoomToFit():\n";
    double xxmin;
    double xxmax;
    double xsmallestGreaterZero;
    if (graphs.size()<=0) return;
    if (zoomX) {
        getGraphsXMinMax(xxmin, xxmax, xsmallestGreaterZero);
        //std::cout<<"   xxmin="<<xxmin<<"   xxmax="<<xxmax<<std::endl;
        if (JKQTPIsOKFloat(xxmin) && JKQTPIsOKFloat(xxmax)) {
            bool doScale=true;
            if (fabs(xxmin-xxmax)<1e-305) {
                xxmin=xAxis->getMin();
                xxmax=xAxis->getMax();
                doScale=false;
            }
            if (xAxis->isLogAxis()) {
                if ((xxmin<=1e-305)&&(xxmax<=1e-305)) {xxmin=0.1; xxmax=1.0; }
                else if ((xxmin<=1e-305)&&(xxmax>0)) {
                    if (xsmallestGreaterZero>10.0*DBL_MIN) xxmin=xsmallestGreaterZero;
                    else xxmin=xxmax/xAxis->getLogAxisBase();
                }
                if (doScale) {
                    double d=scaleX*(log(xxmax)-log(xxmin));  // new width
                    double c=(log(xxmax)+log(xxmin))/2.0;     // center of interval
                    xxmin=exp(c-d/2.0);
                    xxmax=exp(c+d/2.0);
                }
            } else if (doScale) {
                double d=scaleX*(xxmax-xxmin);  // new width
                double c=(xxmax+xxmin)/2.0;     // center of interval
                xxmin=c-d/2.0;
                xxmax=c+d/2.0;
            }
            if (includeX0 && !xAxis->isLogAxis()) {
                if (xxmin>0) xxmin=0;
                else if (xxmax<0) xxmax=0;
            }
            //std::cout<<"   => xxmin="<<xxmin<<"   xxmax="<<xxmax<<std::endl;
            if (JKQTPIsOKFloat(xxmin) && JKQTPIsOKFloat(xxmax)) {
                setX(xxmin, xxmax);
            } else if (!JKQTPIsOKFloat(xxmin) && JKQTPIsOKFloat(xxmax)) {
                setX(getXMin(), xxmax);
            } else if (JKQTPIsOKFloat(xxmin) && !JKQTPIsOKFloat(xxmax)) {
                setX(xxmin, getXMax());
            }
        }
    }
    double yymin;
    double yymax;
    double ysmallestGreaterZero;

    if (zoomY) {
        getGraphsYMinMax(yymin, yymax, ysmallestGreaterZero);
        //std::cout<<"   yymin="<<yymin<<"   yymax="<<yymax<<std::endl;
        bool doScale=true;
        if (fabs(yymin-yymax)<1e-305) {
            yymin=yAxis->getMin();
            yymax=yAxis->getMax();
            doScale=false;
        }
        if (yAxis->getLogAxis()) {
            if ((yymin<=1e-305)&&(yymax<=1e-305)) {yymin=0.1; yymax=1.0; }
            else if ((yymin<=1e-305)&&(yymax>0)) {
                if (ysmallestGreaterZero>10.0*DBL_MIN) yymin=ysmallestGreaterZero;
                else yymin=yymax/yAxis->getLogAxisBase();
            }
            if (doScale) {
                double d=scaleY*(log(yymax)-log(yymin));  // new width
                double c=(log(yymax)+log(yymin))/2.0;     // center of interval
                yymin=exp(c-d/2.0);
                yymax=exp(c+d/2.0);
            }
        } else if (doScale) {
            double d=scaleY*(yymax-yymin);  // new width
            double c=(yymax+yymin)/2.0;     // center of interval
            yymin=c-d/2.0;
            yymax=c+d/2.0;
        }
        if (includeY0 && !yAxis->getLogAxis()) {
            if (yymin>0) yymin=0;
            else if (yymax<0) yymax=0;
        }
        //std::cout<<"   => yymin="<<yymin<<"   yymax="<<yymax<<std::endl;

        if (JKQTPIsOKFloat(yymin) && JKQTPIsOKFloat(yymax)) {
            setY(yymin, yymax);
        } else if (!JKQTPIsOKFloat(yymin) && JKQTPIsOKFloat(yymax)) {
            setY(getYMin(), yymax);
        } else if (JKQTPIsOKFloat(yymin) && !JKQTPIsOKFloat(yymax)) {
            setY(yymin, getYMax());
        }

    }
    //std::cout<<"end of zoomToFit\n";
    //setXY(xxmin, xxmax, yymin, yymax);
    if (emitSignals) emit zoomChangedLocally(xAxis->getMin(), xAxis->getMax(), yAxis->getMin(), yAxis->getMax(), this);
}





JKQTPOverlayElement *JKQTBasePlotter::getOverlayElement(size_t i) {
    return overlays[i];
}

size_t JKQTBasePlotter::getOverlayElementCount() {
    return overlays.size();
}

void JKQTBasePlotter::deleteOverlayElement(size_t i, bool deletegraph) {
    if (long(i)<0 || long(i)>=overlays.size()) return;
    JKQTPOverlayElement* g=overlays[i];
    overlays.removeAt(i);
    if (deletegraph && g) delete g;
    if (emitPlotSignals) emit overlaysUpdated();
}

void JKQTBasePlotter::deleteOverlayElement(JKQTPOverlayElement *gr, bool deletegraph) {
    int i=overlays.indexOf(gr);
    while (i>=0) {
        overlays.removeAt(i);
        i=overlays.indexOf(gr);
    }

    if (deletegraph && gr) delete gr;
    if (emitPlotSignals) emit overlaysUpdated();
}

void JKQTBasePlotter::clearOverlayElement(bool deleteGraphs) {
    for (int i=0; i<overlays.size(); i++) {
        JKQTPOverlayElement* g=overlays[i];
        if (g && deleteGraphs) delete g;
    }
    overlays.clear();
    if (emitPlotSignals) emit overlaysUpdated();
}

size_t JKQTBasePlotter::addOverlayElement(JKQTPOverlayElement *gr) {
    gr->setParent(this);
    for (int i=0; i<overlays.size(); i++) {
        if (overlays[i]==gr) return i;
    }
    overlays.push_back(gr);
    if (emitPlotSignals) emit overlaysUpdated();
    return overlays.size()-1;
}

bool JKQTBasePlotter::containsOverlayElement(JKQTPOverlayElement *gr) const {
    for (int i=0; i<overlays.size(); i++) {
        if (overlays[i]==gr) {
            return true;
        }
    }
    return false;
}

size_t JKQTBasePlotter::moveOverlayElementTop(JKQTPOverlayElement *gr) {
    gr->setParent(this);
    for (int i=0; i<overlays.size(); i++) {
        if (overlays[i]==gr) {
            if (i<overlays.size()-1) {
                overlays.removeAt(i);
                overlays.push_back(gr);
            }
            return overlays.size()-1;
        }
    }
    overlays.push_back(gr);
    if (emitPlotSignals) emit overlaysUpdated();
    return overlays.size()-1;
}

void JKQTBasePlotter::addOverlayElements(const QList<JKQTPOverlayElement *> &gr) {
    for (int i=0; i< gr.size(); i++) {
        addOverlayElement(gr[i]);
    }
}

QVector<QLineF> JKQTBasePlotter::getBoundingLinesX1Y1(QRectF* rect) const
{
    QVector<QLineF> l;
    const QPointF pmimi=QPointF(x2p(xAxis->getMin()), y2p(yAxis->getMin()));
    const QPointF pmima=QPointF(x2p(xAxis->getMin()), y2p(yAxis->getMax()));
    const QPointF pmami=QPointF(x2p(xAxis->getMax()), y2p(yAxis->getMin()));
    const QPointF pmama=QPointF(x2p(xAxis->getMax()), y2p(yAxis->getMax()));
    l.append(QLineF(pmimi, pmami));
    l.append(QLineF(pmami, pmama));
    l.append(QLineF(pmama, pmima));
    l.append(QLineF(pmima, pmimi));
    if (rect) *rect=QRectF(pmimi,pmama);
    return l;
}





JKQTPPlotElement* JKQTBasePlotter::getGraph(size_t i) {
    return graphs[i];
};

size_t JKQTBasePlotter::getGraphCount() {
    return graphs.size();
};

void JKQTBasePlotter::deleteGraph(size_t i, bool deletegraph) {
    if (long(i)<0 || long(i)>=graphs.size()) return;
    JKQTPPlotElement* g=graphs[i];
    graphs.removeAt(i);
    if (deletegraph && g) delete g;
    if (emitPlotSignals) emit plotUpdated();
};

void JKQTBasePlotter::deleteGraph(JKQTPPlotElement* gr, bool deletegraph) {
    int i=graphs.indexOf(gr);
    while (i>=0) {
        graphs.removeAt(i);
        i=graphs.indexOf(gr);
    }

    if (deletegraph && gr) delete gr;
    if (emitPlotSignals) emit plotUpdated();
}

void JKQTBasePlotter::clearGraphs(bool deleteGraphs) {
    for (int i=0; i<graphs.size(); i++) {
        JKQTPPlotElement* g=graphs[i];
        if (g && deleteGraphs) delete g;
    }
    graphs.clear();
    usedStyles.clear();
    if (emitPlotSignals) emit plotUpdated();
}

void JKQTBasePlotter::setAllGraphsInvisible()
{
    for (int i=0; i<graphs.size(); i++) {
        JKQTPPlotElement* g=graphs[i];
        g->setVisible(false);
    }
    if (emitPlotSignals) emit plotUpdated();
}

void JKQTBasePlotter::setAllGraphsVisible()
{
    for (int i=0; i<graphs.size(); i++) {
        JKQTPPlotElement* g=graphs[i];
        g->setVisible(true);
    }
    if (emitPlotSignals) emit plotUpdated();
}

void JKQTBasePlotter::setGraphVisible(int i, bool visible)
{
    JKQTPPlotElement* g=graphs.value(i, nullptr);
    if (g) g->setVisible(visible);
    if (emitPlotSignals) emit plotUpdated();
}

void JKQTBasePlotter::setOnlyGraphVisible(int gr)
{
    for (int i=0; i<graphs.size(); i++) {
        JKQTPPlotElement* g=graphs[i];
        g->setVisible(false);
    }
    JKQTPPlotElement* g=graphs.value(gr, nullptr);
    if (g) g->setVisible(true);
    if (emitPlotSignals) emit plotUpdated();
}

void JKQTBasePlotter::setOnlyNthGraphsVisible(int start, int n)
{
    for (int i=0; i<graphs.size(); i++) {
        JKQTPPlotElement* g=graphs[i];
        g->setVisible(false);
    }
    for (int i=start; i<graphs.size(); i+=n) {
        JKQTPPlotElement* g=graphs.value(i, nullptr);
        if (g) g->setVisible(true);
    }
    if (emitPlotSignals) emit plotUpdated();
}

size_t JKQTBasePlotter::addGraph(JKQTPPlotElement* gr) {
    gr->setParent(this);
    for (int i=0; i<graphs.size(); i++) {
        if (graphs[i]==gr) return i;
    }
    graphs.push_back(gr);
    if (emitPlotSignals) emit plotUpdated();
    return graphs.size()-1;
};

size_t JKQTBasePlotter::moveGraphTop(JKQTPPlotElement* gr) {
    gr->setParent(this);
    for (int i=0; i<graphs.size(); i++) {
        if (graphs[i]==gr) {
            if (i<graphs.size()-1) {
                graphs.removeAt(i);
                graphs.push_back(gr);
            }
            return graphs.size()-1;
        }
    }
    graphs.push_back(gr);
    if (emitPlotSignals) emit plotUpdated();
    return graphs.size()-1;
}

size_t JKQTBasePlotter::moveGraphBottom(JKQTPPlotElement *gr)
{
    gr->setParent(this);
    for (int i=0; i<graphs.size(); i++) {
        if (graphs[i]==gr) {
            if (i<graphs.size()-1) {
                graphs.removeAt(i);
                graphs.push_front(gr);
            }
            return 0;
        }
    }
    graphs.push_front(gr);
    if (emitPlotSignals) emit plotUpdated();
    return graphs.size()-1;
};

bool JKQTBasePlotter::containsGraph(JKQTPPlotElement* gr) const {
    for (int i=0; i<graphs.size(); i++) {
        if (graphs[i]==gr) {
            return true;
        }
    }
    return false;
};

void JKQTBasePlotter::setUserSettigsFilename(const QString &filename, const QString &prefix)
{
    userSettigsFilename=filename;
    userSettigsPrefix=prefix;
}

void JKQTBasePlotter::setUserSettigsFilename(const QString &filename)
{
    userSettigsFilename=filename;
}

void JKQTBasePlotter::setUserSettigsPrefix(const QString &prefix)
{
    userSettigsPrefix=prefix;
}

QString JKQTBasePlotter::getUserSettigsFilename() const
{
    return userSettigsFilename;
}

QString JKQTBasePlotter::getUserSettigsPrefix() const
{
    return userSettigsPrefix;
}

QSet<int> JKQTBasePlotter::getDataColumnsByUser() {
    loadUserSettings();
    QSet<int> set;

    QStringList cols=getDatastore()->getColumnNames();

    QDialog* dlg=new QDialog(nullptr, Qt::WindowMinMaxButtonsHint);
    dlg->setSizeGripEnabled(true);
    //printZoomFactor=0.95;
    //printMagnification=1.5;
    QGridLayout* layout=new QGridLayout();
    dlg->setLayout(layout);
    dlg->setWindowTitle(tr("Select columns to export ..."));
    dlg->setWindowIcon(QIcon());

    QLabel* lab=new QLabel(tr("<center><b>Please check the columns that should be exported in the list!</b><br>"
                                         "You may also save a selection to reuse it in future, by clicking \"Save\". "
                                         "A stored selection can be used by selecting its name in the dropdown field "
                                         "above the list widget.</center>"), dlg);
    lab->setWordWrap(true);
    layout->addWidget(lab,0,0,1,2);

    dataColumnsCombobox=new QComboBox(dlg);
    dataColumnsCombobox->addItems(getDataColumnsByUserSaved.keys());
    connect(dataColumnsCombobox, SIGNAL(currentIndexChanged(QString)), this, SLOT(getDataColumnsByUserComboBoxSelected(QString)));
    layout->addWidget(dataColumnsCombobox, 1,0);
    QPushButton* btn=new QPushButton(tr("&save"), dlg);
    connect(btn, SIGNAL(clicked()), this, SLOT(getDataColumnsByUserSave()));
    layout->addWidget(btn, 1,1);

    dataColumnsListWidget=new QListWidget(dlg);
    for (int i=0; i<cols.size(); i++) {
        QListWidgetItem* item=new QListWidgetItem(cols[i], dataColumnsListWidget);
        item->setCheckState(Qt::Checked);
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        dataColumnsListWidget->addItem(item);
    }
    connect(dataColumnsListWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(getDataColumnsByUserItemChanged(QListWidgetItem*)));
    layout->addWidget(dataColumnsListWidget, 2,0,5,1);
    btn=new QPushButton(tr("select &all"), dlg);
    connect(btn, SIGNAL(clicked()), this, SLOT(getDataColumnsByUserCheckAll()));
    layout->addWidget(btn, 2,1);
    btn=new QPushButton(tr("select &none"), dlg);
    connect(btn, SIGNAL(clicked()), this, SLOT(getDataColumnsByUserCheckNone()));
    layout->addWidget(btn, 3,1);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), dlg, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dlg, SLOT(reject()));
    layout->addWidget(buttonBox, layout->rowCount(),0, 1, layout->columnCount());
    layout->setRowStretch(layout->rowCount()-2,1);
    layout->setColumnStretch(0,1);
    dlg->resize(350,500);
    dataColumnsCombobox->setCurrentIndex(-1);

    if (dlg->exec()==QDialog::Accepted) {
        for (int i=0; i<dataColumnsListWidget->count(); i++) {
            if (dataColumnsListWidget->item(i)->checkState()==Qt::Checked) {
                set.insert(i);
            }
        }

    }
    delete dlg;
    dataColumnsListWidget=nullptr;

    saveUserSettings();
    return set;
}


void JKQTBasePlotter::getDataColumnsByUserCheckAll() {
    if (!dataColumnsListWidget) return;
    for (int i=0; i<dataColumnsListWidget->count(); i++) {
        dataColumnsListWidget->item(i)->setCheckState(Qt::Checked);
    }
}

void JKQTBasePlotter::getDataColumnsByUserCheckNone() {
    if (!dataColumnsListWidget) return;
    for (int i=0; i<dataColumnsListWidget->count(); i++) {
        dataColumnsListWidget->item(i)->setCheckState(Qt::Unchecked);
    }
}

void JKQTBasePlotter::getDataColumnsByUserSave() {
    if (!dataColumnsListWidget) return;
    QString name=tr("my selection name");
    QStringList items=getDataColumnsByUserSaved.keys();
    items<<name;
    bool ok=false;
    name=QInputDialog::getItem(nullptr, tr("save columns selection"), tr("name for new selection:"), items, items.size()-1, true, &ok);
    if (ok) {
        QStringList data;
        for (int i=0; i<dataColumnsListWidget->count(); i++) {
            if (dataColumnsListWidget->item(i)->checkState()==Qt::Checked) data.append(dataColumnsListWidget->item(i)->text());
        }
        data.sort();
        getDataColumnsByUserSaved[name]=data;
        disconnect(dataColumnsCombobox, SIGNAL(currentIndexChanged(QString)), this, SLOT(getDataColumnsByUserComboBoxSelected(QString)));
        dataColumnsCombobox->clear();
        dataColumnsCombobox->addItems(getDataColumnsByUserSaved.keys());
        dataColumnsCombobox->setCurrentIndex(dataColumnsCombobox->findText(name));
        connect(dataColumnsCombobox, SIGNAL(currentIndexChanged(QString)), this, SLOT(getDataColumnsByUserComboBoxSelected(QString)));
    }
}

void JKQTBasePlotter::getDataColumnsByUserComboBoxSelected(const QString &name) {
    if (!dataColumnsListWidget) return;
    QStringList newItems=getDataColumnsByUserSaved.value(name, QStringList());
    if (getDataColumnsByUserSaved.contains(name)) {
        for (int i=0; i<dataColumnsListWidget->count(); i++) {
            if (newItems.contains(dataColumnsListWidget->item(i)->text()) && (!dataColumnsListWidget->item(i)->text().isEmpty())) {
                dataColumnsListWidget->item(i)->setCheckState(Qt::Checked);
            } else {
                dataColumnsListWidget->item(i)->setCheckState(Qt::Unchecked);
            }
        }
    }
}

void JKQTBasePlotter::getDataColumnsByUserItemChanged(QListWidgetItem * /*widgetitem*/) {
    if (!dataColumnsListWidget) return;
    QStringList data;
    for (int i=0; i<dataColumnsListWidget->count(); i++) {
        if (dataColumnsListWidget->item(i)->checkState()==Qt::Checked) data.append(dataColumnsListWidget->item(i)->text());
    }
    data.sort();

    QMapIterator<QString, QStringList> it(getDataColumnsByUserSaved);
    QString item="";
    while (it.hasNext()) {
        it.next();
        QStringList ld=it.value();
        ld.sort();
        if (data==ld) {
            item=it.key();
        }
    }

    disconnect(dataColumnsCombobox, SIGNAL(currentIndexChanged(QString)), this, SLOT(getDataColumnsByUserComboBoxSelected(QString)));
    dataColumnsCombobox->setCurrentIndex(dataColumnsCombobox->findText(item));
    connect(dataColumnsCombobox, SIGNAL(currentIndexChanged(QString)), this, SLOT(getDataColumnsByUserComboBoxSelected(QString)));
}

void JKQTBasePlotter::showPlotData() {
    QDialog* dlg=new QDialog(nullptr,Qt::Dialog|Qt::WindowCloseButtonHint|Qt::WindowMinMaxButtonsHint);
    dlg->setWindowTitle(tr("Plot data table"));
    dlg->setSizeGripEnabled(true);

    QVBoxLayout* layout=new QVBoxLayout();
    dlg->setLayout(layout);
    QToolBar* tb=new QToolBar("toolbar", dlg);
    QAction* actClose=new QAction(QIcon(":/JKQTPlotter/jkqtp_exit.png"), tr("&Close Window"), dlg);
    connect(actClose, SIGNAL(triggered()), dlg, SLOT(accept()));
    tb->addAction(actClose);
    tb->addSeparator();
    tb->addAction(actSaveData);
    tb->addAction(actCopyData);
    tb->addAction(actCopyMatlab);
    layout->addWidget(tb);

    JKQTPEnhancedTableView* tv=new JKQTPEnhancedTableView(dlg);
    layout->addWidget(tv);
    tb->addAction(tv->getActionPrint());

    JKQTPDatastoreModel* model=new JKQTPDatastoreModel(getDatastore(), this);
    tv->setModel(model);
    tv->resizeColumnsToContents();
    tv->resizeRowsToContents();

    dlg->exec();
    delete dlg;
}


void JKQTBasePlotter::setEmittingSignalsEnabled(bool enabled)
{
    emitSignals=enabled;
    xAxis->setDoUpdateScaling(enabled);
    yAxis->setDoUpdateScaling(enabled);
}


QHash<JKQTBasePlotter::textSizeKey, JKQTBasePlotter::textSizeData> JKQTBasePlotter::tbrh=QHash<JKQTBasePlotter::textSizeKey, JKQTBasePlotter::textSizeData>();

JKQTBasePlotter::textSizeKey::textSizeKey(const QFont &f, const QString &text, QPaintDevice *pd)
{
    this->text=text;
    this->f=f;
    if (pd) {
        ldpiX=pd->logicalDpiX();
        ldpiY=pd->logicalDpiY();
        pdpiX=pd->physicalDpiX();
        pdpiY=pd->physicalDpiY();
    } else {
        ldpiX=0;
        ldpiY=0;
        pdpiX=0;
        pdpiY=0;
    }
}

JKQTBasePlotter::textSizeKey::textSizeKey(const QString &fontName, double fontSize, const QString &text, QPaintDevice *pd)
{
    QFont f;
    f.setFamily(fontName);
    f.setPointSizeF(fontSize);
    this->text=text;
    this->f=f;
    if (pd) {
        ldpiX=pd->logicalDpiX();
        ldpiY=pd->logicalDpiY();
        pdpiX=pd->physicalDpiX();
        pdpiY=pd->physicalDpiY();
    } else {
        ldpiX=0;
        ldpiY=0;
        pdpiX=0;
        pdpiY=0;
    }
}

bool JKQTBasePlotter::textSizeKey::operator==(const JKQTBasePlotter::textSizeKey &other) const
{
    return ldpiX==other.ldpiX &&  ldpiY==other.ldpiY && text==other.text && f==other.f;
}


JKQTBasePlotter::textSizeData::textSizeData()
{
    ascent=0;
    descent=0;
    width=0;
    strikeoutPos=0;
}






bool JKQTPPaintDeviceAdapter::useLatexParser() const
{
    return true;
}

QPaintDevice *JKQTPPaintDeviceAdapter::createPaintdeviceMM(const QString &filename, double widthMM, double heightMM) const
{
    return createPaintdevice(filename, widthMM/25.4*QApplication::desktop()->logicalDpiX(), heightMM/25.4*QApplication::desktop()->logicalDpiY());
}

JKQTPSaveDataAdapter::~JKQTPSaveDataAdapter() = default;
