/*
    Copyright (c) 2008-2023 Jan W. Krieger (<jan@jkrieger.de>)

    

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
#include <QDialog>
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/gui/jkqtpgraphsmodel.h"
#include "jkqtplotter/gui/jkqtpenhancedtableview.h"
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
#  include <QPrintDialog>
#  include <QPrinter>
#  include <QPrinterInfo>
#  include <QtPrintSupport/QPrintPreviewWidget>
#endif
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
#include "jkqtplotter/graphs/jkqtpimage.h"
#include "jkqtplotter/graphs/jkqtpimagergb.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtplotter/jkqtpkey.h"
#include <algorithm>
#include <mutex>

QString JKQTBasePlotter::globalUserSettigsFilename="";
QString JKQTBasePlotter::globalUserSettigsPrefix="";
std::mutex JKQTBasePlotter::globalUserSettingsMutex;
JKQTPSynchronized<QList<JKQTPPaintDeviceAdapter*>> JKQTBasePlotter::jkqtpPaintDeviceAdapters;
JKQTPSynchronized<QList<JKQTPSaveDataAdapter*>> JKQTBasePlotter::jkqtpSaveDataAdapters;



void initJKQTBasePlotterResources()
{
    static std::once_flag flag;
    std::call_once(flag, []() {
        Q_INIT_RESOURCE(jkqtpbaseplotter);
        initJKQTMathTextResources();
    });
}


void JKQTBasePlotter::setDefaultJKQTBasePrinterUserSettings(QString userSettigsFilename, const QString& userSettigsPrefix)
{
    std::lock_guard<std::mutex> lock(globalUserSettingsMutex);
    globalUserSettigsFilename=userSettigsFilename;
    globalUserSettigsPrefix=userSettigsPrefix;
}

void JKQTBasePlotter::registerPaintDeviceAdapter(JKQTPPaintDeviceAdapter *adapter)
{
    JKQTPSynchronized<QList<JKQTPPaintDeviceAdapter*>>::WriteLocker lock(jkqtpPaintDeviceAdapters);
    jkqtpPaintDeviceAdapters.get().append(adapter);
}

void JKQTBasePlotter::deregisterPaintDeviceAdapter(JKQTPPaintDeviceAdapter *adapter)
{
    JKQTPSynchronized<QList<JKQTPPaintDeviceAdapter*>>::WriteLocker lock(jkqtpPaintDeviceAdapters);
    if (jkqtpPaintDeviceAdapters.get().contains(adapter)) jkqtpPaintDeviceAdapters.get().removeAll(adapter);
}

bool JKQTBasePlotter::registerSaveDataAdapter(JKQTPSaveDataAdapter *adapter)
{
    if (adapter){
        JKQTPSynchronized<QList<JKQTPSaveDataAdapter*>>::WriteLocker lock(jkqtpSaveDataAdapters);
        QString format=adapter->getFilter();
        for (int i=0; i<jkqtpSaveDataAdapters.get().size(); i++) {
            if (jkqtpSaveDataAdapters.get()[i] && jkqtpSaveDataAdapters.get()[i]->getFilter()==format) {
                return false;
            }
        }
        jkqtpSaveDataAdapters.get().append(adapter);
        return true;
    }
    return false;
}

bool JKQTBasePlotter::deregisterSaveDataAdapter(JKQTPSaveDataAdapter *adapter)
{
    JKQTPSynchronized<QList<JKQTPSaveDataAdapter*>>::WriteLocker lock(jkqtpSaveDataAdapters);
    if (jkqtpSaveDataAdapters.get().contains(adapter)) jkqtpSaveDataAdapters.get().removeAll(adapter);
    return true;
}






JKQTMathTextNodeSize JKQTBasePlotter::getTextSizeDetail(const QFont &fm, const QString &text, QPainter& painter)
{
    return getTextSizeDetail(fm.family(), fm.pointSizeF(), text, painter);
}

JKQTMathTextNodeSize JKQTBasePlotter::getTextSizeDetail(const QString &fontName, double fontSize, const QString &text, QPainter& painter)
{
    JKQTBasePlotter::textSizeKey  dh(fontName, fontSize, text, painter.device());
    static QReadWriteLock s_TextSizeLock;
    thread_local QHash<JKQTBasePlotter::textSizeKey, JKQTMathTextNodeSize> s_TextSizeDataCache;
    QReadLocker lockR(&s_TextSizeLock);
    if (s_TextSizeDataCache.contains(dh)) return s_TextSizeDataCache[dh];
    lockR.unlock();
    QWriteLocker lockW(&s_TextSizeLock);
    if (s_TextSizeDataCache.contains(dh)) return s_TextSizeDataCache[dh];
    mathText.setFontSpecial(fontName);
    mathText.setFontSize(fontSize);
    mathText.parse(text);
    const JKQTMathTextNodeSize d=mathText.getSizeDetail(painter);
    s_TextSizeDataCache[dh]=d;
    //qDebug()<<"+++ textsize hash size: "<<tbrh.size();
    return d;
}

void JKQTBasePlotter::getTextSizeDetail(const QString &fontName, double fontSize, const QString &text, QPainter &painter, double &width, double &ascent, double &descent, double &strikeoutPos)
{
    if (!text.isEmpty()) {
        const JKQTMathTextNodeSize d=getTextSizeDetail(fontName, fontSize, text, painter);
        width=d.width;
        ascent=d.baselineHeight;
        descent=d.getDescent();
        strikeoutPos=d.strikeoutPos;
    } else {
        width=0;
        ascent=0;
        descent=0;
        strikeoutPos=0;
    }
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
    if (text.isEmpty()) return QSizeF(0,0);
    const JKQTMathTextNodeSize d=getTextSizeDetail(fontName, fontSize, text, painter);
    return d.getSize();
}


// define this to get timing information about painting on the debug output
//#define JKQTBP_DEBUGTIMING
#undef JKQTBP_DEBUGTIMING

/**************************************************************************************************************************
 * JKQTBasePlotter
 **************************************************************************************************************************/
JKQTBasePlotter::JKQTBasePlotter(bool datastore_internal, QObject* parent, JKQTPDatastore* datast):
    QObject(parent), plotterStyle(JKQTPGetSystemDefaultBaseStyle()), m_plotsModel(nullptr), xAxis(nullptr), yAxis(nullptr), mainKey(nullptr)
{
    initJKQTBasePlotterResources();
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
    {
        std::lock_guard<std::mutex> lock(globalUserSettingsMutex);
        userSettigsFilename=globalUserSettigsFilename;
        userSettigsPrefix=globalUserSettigsPrefix;
    }
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    currentPrinter=QPrinterInfo::defaultPrinter().printerName();
#endif

    if (datastore_internal) {
      datastore=new JKQTPDatastore();
      datastoreInternal=true;
    } else {
      datastore=datast;
      datastoreInternal=false;
    }

    // some fonts that are know to deliver bad rendering quality
    mathText.addReplacementFont("MS Shell Dlg 2", "Arial");
    mathText.addReplacementFont("MS Shell Dlg", "Arial");
    mathText.addReplacementFont("MS Sans Serif", "Arial");
    mathText.addReplacementFont("MS Serif", "Times New Roman");
    mathText.addReplacementFont("MS Sans Serif Standard", "Arial");
    mathText.addReplacementFont("MS Serif Standard", "Times New Roman");

    xAxis=new JKQTPHorizontalAxis(this);
    yAxis=new JKQTPVerticalAxis(this);
    m_plotsModel=new JKQTPGraphsModel(this);
    connect(this, SIGNAL(plotUpdated()), m_plotsModel, SLOT(plotUpdated()));

    mainKey=new JKQTPMainKey(this);

    emitSignals=false;


    setWidgetSize(400,300);
    internalPlotWidth=400;
    internalPlotHeight=300;
    initSettings();

    emitSignals=true;

    actSavePlot=new QAction(QIcon(":/JKQTPlotter/jkqtp_saveplot.png"), tr("Save Plot"), this);
    actSavePlot->setToolTip(tr("Save plot as image file (PDF, PS; PNG, ...)."));
    actSaveData=new QAction(QIcon(":/JKQTPlotter/jkqtp_savedata.png"), tr("Save Data"), this);
    actSaveData->setToolTip(tr("Save Data of the plot as file (CSV, ...)."));
    actCopyData=new QAction(QIcon(":/JKQTPlotter/jkqtp_copydata.png"), tr("Copy Data"), this);
    actCopyData->setToolTip(tr("Copy Data of the plot to the clipboard to be pasted into Excel etc."));
    actCopyMatlab=new QAction(QIcon(":/JKQTPlotter/jkqtp_copymatlab.png"), tr("Copy Data to Matlab"), this);
    actCopyMatlab->setToolTip(tr("Copy Data of the plot to the clipboard in Matlab script format."));
    actCopyPixelImage=new QAction(QIcon(":/JKQTPlotter/jkqtp_copyimg.png"), tr("Copy Image"), this);
    actCopyPixelImage->setToolTip(tr("Copy the plot as a pixel image to the clipboard"));


#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    actSavePDF=new QAction(QIcon(":/JKQTPlotter/jkqtp_savepdf.png"), tr("Save P&DF"), this);
    actSavePDF->setToolTip(tr("Save as PDF"));
    //toolbar->addAction(actSavePDF);
    actSaveSVG=new QAction(QIcon(":/JKQTPlotter/jkqtp_savesvg.png"), tr("Save S&VG"), this);
    actSaveSVG->setToolTip(tr("Save as Scalable Vector Graphics (SVG)"));
    //toolbar->addAction(actSaveSVG);
#endif
    actSavePix=new QAction(QIcon(":/JKQTPlotter/jkqtp_savepix.png"), tr("Save &Image"), this);
    actSavePix->setToolTip(tr("Save as Pixel Image (PNG, JPEG, TIFF ...)"));
    //toolbar->addAction(actSavePix);

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    actPrint=new QAction(QIcon(":/JKQTPlotter/jkqtp_print.png"), tr("&Print"), this);
    actPrint->setToolTip("Print");
#endif
    //toolbar->addSeparator();
    actSaveCSV=new QAction(QIcon(":/JKQTPlotter/jkqtp_savecsv.png"), tr("Save &CSV"), this);
    actSaveCSV->setToolTip(tr("Save the data which is used for the plot as Comma Separated Values (CSV)"));
    //toolbar->addAction(actSaveCSV);
    //toolbar->addSeparator();
    actZoomAll=new QAction(QIcon(":/JKQTPlotter/jkqtp_zoomall.png"), tr("Zoom &All"), this);
    actZoomAll->setToolTip(tr("Zoom to view all data"));
    actZoomIn=new QAction(QIcon(":/JKQTPlotter/jkqtp_zoomin.png"), tr("Zoom &In"), this);
    actZoomIn->setToolTip(tr("Zoom in around the center of the plot"));
    actZoomOut=new QAction(QIcon(":/JKQTPlotter/jkqtp_zoomout.png"), tr("Zoom &Out"), this);
    actZoomOut->setToolTip(tr("Zoom out"));

    actShowPlotData=new QAction(QIcon(":/JKQTPlotter/jkqtp_showplotdata.png"), tr("&Show Plot Data"), this);
    actShowPlotData->setToolTip(tr("Opens a dialog that contains all data used for the plot in a table."));



    connect(actSavePlot,   SIGNAL(triggered()), this, SLOT(saveImage()));
    connect(actSaveData,   SIGNAL(triggered()), this, SLOT(saveData()));
    connect(actCopyData,   SIGNAL(triggered()), this, SLOT(copyData()));
    connect(actCopyPixelImage,   SIGNAL(triggered()), this, SLOT(copyPixelImage()));
    connect(actCopyMatlab,   SIGNAL(triggered()), this, SLOT(copyDataMatlab()));
    connect(actShowPlotData,   SIGNAL(triggered()), this, SLOT(showPlotData()));

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    connect(actPrint,      SIGNAL(triggered()), this, SLOT(print()));
    connect(actSaveSVG,    SIGNAL(triggered()), this, SLOT(saveAsSVG()));
    connect(actSavePDF,    SIGNAL(triggered()), this, SLOT(saveAsPDF()));
#endif
    connect(actSavePix,    SIGNAL(triggered()), this, SLOT(saveAsPixelImage()));

    connect(actSaveCSV,    SIGNAL(triggered()), this, SLOT(saveAsCSV()));
    connect(actZoomAll,    SIGNAL(triggered()), this, SLOT(zoomToFit()));
    connect(actZoomIn,     SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(actZoomOut,    SIGNAL(triggered()), this, SLOT(zoomOut()));

    setWidgetSize(400,300);

}

JKQTBasePlotter::~JKQTBasePlotter(){
    clearGraphs(false);
    if (datastoreInternal && datastore!=nullptr) delete datastore;
    delete mainKey;
    delete xAxis;
    delete yAxis;
    for (const auto& ax: qAsConst(secondaryYAxis)) {
        delete ax;
    }
    secondaryYAxis.clear();
    for (const auto& ax: qAsConst(secondaryXAxis)) {
        delete ax;
    }
    secondaryXAxis.clear();

}

void JKQTBasePlotter::setGrid(bool val) {
    xAxis->setDrawGrid(val);
    yAxis->setDrawGrid(val);
}

void JKQTBasePlotter::setGridColor(QColor color)
{
    xAxis->setGridColor(color);
    yAxis->setGridColor(color);
}

void JKQTBasePlotter::setMinorGridColor(QColor color)
{
    xAxis->setMinorGridColor(color);
    yAxis->setMinorGridColor(color);
}

void JKQTBasePlotter::setGridWidth(double __value)
{
    xAxis->setGridWidth(__value);
    yAxis->setGridWidth(__value);
}

void JKQTBasePlotter::setMinorGridWidth(double __value)
{
    xAxis->setMinorGridWidth(__value);
    yAxis->setMinorGridWidth(__value);
}
void JKQTBasePlotter::setGridStyle(Qt::PenStyle __value)
{
    xAxis->setGridStyle(__value);
    yAxis->setGridStyle(__value);
}

void JKQTBasePlotter::setMinorGridStyle(Qt::PenStyle __value)
{
    xAxis->setMinorGridStyle(__value);
    yAxis->setMinorGridStyle(__value);
}

void JKQTBasePlotter::setShowZeroAxes(bool showX, bool showY) {
    xAxis->setShowZeroAxis(showX);
    yAxis->setShowZeroAxis(showY);
}

void JKQTBasePlotter::setShowZeroAxes(bool showXY) {
    setShowZeroAxes(showXY,showXY);
}

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
    redrawPlot();
}

void JKQTBasePlotter::useInternalDatastore(){
    if (!datastoreInternal) {
        datastore=new JKQTPDatastore();
        datastoreInternal=true;
    }
    redrawPlot();
}

void JKQTBasePlotter::forceInternalDatastore(){
    if (datastoreInternal && datastore!=nullptr) {
        delete datastore;
        datastore=nullptr;
    }
    datastore=new JKQTPDatastore();
    datastoreInternal=true;
    redrawPlot();
}

bool JKQTBasePlotter::isEmittingSignalsEnabled() const {
    return this->emitSignals;
}

void JKQTBasePlotter::initSettings() {
    useClipping=true;
    //doDrawing=true;

    maintainAspectRatio=false;
    aspectRatio=1;

    maintainAxisAspectRatio=false;
    axisAspectRatio=1;

    gridPrinting=false;
    gridPrintingList.clear();
    gridPrintingCurrentX=0;
    gridPrintingCurrentY=0;


    internalPlotBorderLeft=plotterStyle.plotBorderLeft;
    internalPlotBorderRight=plotterStyle.plotBorderRight;
    internalPlotBorderTop=plotterStyle.plotBorderTop;
    internalPlotBorderBottom=plotterStyle.plotBorderBottom;

    //plotWidth=700;
    //plotHeight=150;

    xAxis->setRange(-10, 10);
    yAxis->setRange(-10, 10);


    plotLabel="";

    redrawPlot();
}






void JKQTBasePlotter::zoomIn(double factor) {
    //std::cout<<(double)event->delta()/120.0<<":   "<<factor<<std::endl;



    for (auto ax: getXAxes(true)) {
        const double old_mi=ax->x2p(ax->getMin());
        const double old_ma=ax->x2p(ax->getMax());
        const double range=fabs(old_ma-old_mi);
        const double new_range=range/factor;
        const double new_mi=qMin(old_mi,old_ma)-(new_range-range)/2.0;
        const double new_ma=qMax(old_mi,old_ma)+(new_range-range)/2.0;
        const double xmin=ax->p2x(new_mi);
        const double xmax=ax->p2x(new_ma);
        ax->setRange(qMin(xmin, xmax), qMax(xmin, xmax));
    }
    for (auto ax: getYAxes(true)) {
        const double old_mi=ax->x2p(ax->getMin());
        const double old_ma=ax->x2p(ax->getMax());
        const double range=fabs(old_ma-old_mi);
        const double new_range=range/factor;
        const double new_mi=qMin(old_mi,old_ma)-(new_range-range)/2.0;
        const double new_ma=qMax(old_mi,old_ma)+(new_range-range)/2.0;
        const double xmin=ax->p2x(new_mi);
        const double xmax=ax->p2x(new_ma);
        ax->setRange(qMin(xmin, xmax), qMax(xmin, xmax));
    }

    redrawPlot();
    if (emitSignals) emit zoomChangedLocally(xAxis->getMin(), xAxis->getMax(), yAxis->getMin(), yAxis->getMax(), this);
}

void JKQTBasePlotter::zoomOut(double factor) {
    zoomIn(1.0/factor);
}

void JKQTBasePlotter::setMaintainAspectRatio(bool value) {
    maintainAspectRatio=value;
    setAspectRatio(aspectRatio);
    redrawPlot();
}

void JKQTBasePlotter::setMaintainAxisAspectRatio(bool value) {
    maintainAxisAspectRatio=value;
    setAspectRatio(aspectRatio);
    redrawPlot();
}


void JKQTBasePlotter::setWidgetSize(int wid, int heigh) {
    widgetWidth=wid;
    widgetHeight=heigh;

    redrawPlot();
}

void JKQTBasePlotter::setWidgetSize(QSize size)
{
    setWidgetSize(size.width(), size.height());
}

int JKQTBasePlotter::getWidth() {
    return widgetWidth;
}

int JKQTBasePlotter::getHeight() {
    return widgetHeight;
}

double JKQTBasePlotter::getXMin() const {
    return xAxis->getMin();
}

double JKQTBasePlotter::getXMax() const {
    return xAxis->getMax();
}

double JKQTBasePlotter::getYMin() const {
    return yAxis->getMin();
}

double JKQTBasePlotter::getYMax() const {
    return yAxis->getMax();
}

double JKQTBasePlotter::getAbsoluteXMin() const {
    return xAxis->getAbsoluteMin();
}

double JKQTBasePlotter::getAbsoluteXMax() const {
    return xAxis->getAbsoluteMax();
}

double JKQTBasePlotter::getAbsoluteYMin() const {
    return yAxis->getAbsoluteMin();
}

double JKQTBasePlotter::getAbsoluteYMax() const {
    return yAxis->getAbsoluteMax();
}

void JKQTBasePlotter::addGridPrintingPlotter(size_t x, size_t y, JKQTBasePlotter *plotter) {
    if (plotter==this) {
        setGridPrintingCurrentPos(x,y);
    } else {
        JKQTPGridPrintingItem i;
        i.x=x;
        i.y=y;
        i.plotter=plotter;
        gridPrintingList.push_back(i);
    }
}

void JKQTBasePlotter::clearGridPrintingPlotters() {
    gridPrintingList.clear();
}


void JKQTBasePlotter::setWidgetWidth(int wid) {
    widgetWidth=wid;
    redrawPlot();
}

void JKQTBasePlotter::setWidgetHeight(int heigh) {
    widgetHeight=heigh;
    redrawPlot();
}

void JKQTBasePlotter::saveSettings(QSettings& settings, const QString& group) const{

    plotterStyle.saveSettings(settings, group);

    settings.setValue(group+"apect_ratio", aspectRatio);
    settings.setValue(group+"maintain_apect_ratio", maintainAspectRatio);
    settings.setValue(group+"axis_apect_ratio", axisAspectRatio);
    settings.setValue(group+"maintain_axis_apect_ratio", maintainAxisAspectRatio);


    mathText.saveSettings(settings, group+"math_text.");

    xAxis->saveSettings(settings, group+"xaxis_");
    yAxis->saveSettings(settings, group+"yaxis_");

    saveUserSettings();

}

const JKQTBasePlotterStyle &JKQTBasePlotter::getCurrentPlotterStyle() const
{
    return plotterStyle;
}

void JKQTBasePlotter::setCurrentPlotterStyle(const JKQTBasePlotterStyle &style)
{
    plotterStyle=style;
    propagateStyle();
}

void JKQTBasePlotter::propagateStyle() {
    bool old=emitPlotSignals;
    emitPlotSignals=false;
    mainKey->setCurrentKeyStyle(plotterStyle.keyStyle);
    xAxis->setCurrentAxisStyle(plotterStyle.xAxisStyle);
    yAxis->setCurrentAxisStyle(plotterStyle.yAxisStyle);
    for (int i=0; i<graphs.size(); i++) {
        JKQTPColorPaletteStyleAndToolsMixin* palTools=dynamic_cast<JKQTPColorPaletteStyleAndToolsMixin*>(graphs[i]);
        if (palTools) {
            palTools->getColorBarRightAxis()->setCurrentAxisStyle(plotterStyle.rightColorbarAxisStyle);
            palTools->getColorBarTopAxis()->setCurrentAxisStyle(plotterStyle.topColorbarAxisStyle);
        }
        JKQTPMathImage* matImg=dynamic_cast<JKQTPMathImage*>(graphs[i]);
        if (matImg) {
            matImg->getColorBarRightAxis()->setCurrentAxisStyle(plotterStyle.rightColorbarAxisStyle);
            matImg->getColorBarTopAxis()->setCurrentAxisStyle(plotterStyle.topColorbarAxisStyle);
            matImg->getModifierColorBarRightAxis()->setCurrentAxisStyle(plotterStyle.rightColorbarAxisStyle);
            matImg->getModifierColorBarTopAxis()->setCurrentAxisStyle(plotterStyle.topColorbarAxisStyle);
        }
        JKQTPRGBMathImage* matImgRGB=dynamic_cast<JKQTPRGBMathImage*>(graphs[i]);
        if (matImgRGB) {
            matImgRGB->getColorBarRightAxis()->setCurrentAxisStyle(plotterStyle.rightColorbarAxisStyle);
            matImgRGB->getColorBarTopAxis()->setCurrentAxisStyle(plotterStyle.topColorbarAxisStyle);
            matImgRGB->getColorBarRightAxisG()->setCurrentAxisStyle(plotterStyle.rightColorbarAxisStyle);
            matImgRGB->getColorBarTopAxisG()->setCurrentAxisStyle(plotterStyle.topColorbarAxisStyle);
            matImgRGB->getColorBarRightAxisB()->setCurrentAxisStyle(plotterStyle.rightColorbarAxisStyle);
            matImgRGB->getColorBarTopAxisB()->setCurrentAxisStyle(plotterStyle.topColorbarAxisStyle);
        }
    }
    emitPlotSignals=old;
    redrawPlot();
}

QRectF JKQTBasePlotter::calcPlotMarginRect(PlotMarginUse use, PlotMarginSide side) const
{
    QRectF r;
    if (internalPlotMargins.contains(use)) {
        const double marginSize=internalPlotMargins[use].getMargin(side);
        const double allTop=internalPlotMargins.calcTop();
        //const double allBottom=internalPlotMargins.calcBottom();
        const double allLeft=internalPlotMargins.calcLeft();
        //const double allRight=internalPlotMargins.calcRight();
        const double allHeight=internalPlotBorderTop+internalPlotHeight+internalPlotBorderBottom;
        const double allWidth=internalPlotBorderLeft+internalPlotWidth+internalPlotBorderRight;
        if (marginSize>0.0) {
            switch (side) {
            case sideLeft:
                return QRectF(internalPlotMargins.calcLeft(muOutermost, use)-marginSize, allTop, marginSize, internalPlotHeight);
            case sideRight:
                return QRectF(allWidth-internalPlotMargins.calcRight(muOutermost, use), allTop, marginSize, internalPlotHeight);
            case sideTop:
                return QRectF(allLeft, internalPlotMargins.calcTop(muOutermost, use)-marginSize, internalPlotWidth, marginSize);
            case sideBottom:
                return QRectF(allLeft, allHeight-internalPlotMargins.calcBottom(muOutermost, use), internalPlotWidth, marginSize);
            }
        }
    }
    return r;
}

QRectF JKQTBasePlotter::calcPlotRect() const
{
    return QRectF(internalPlotBorderLeft, internalPlotBorderTop, internalPlotWidth, internalPlotHeight);
}

void JKQTBasePlotter::loadCurrentPlotterStyle(const QSettings &settings, const QString &group)
{
    plotterStyle.loadSettings(settings, group);
    propagateStyle();
}

void JKQTBasePlotter::saveCurrentPlotterStyle(QSettings &settings, const QString &group) const
{
    plotterStyle.saveSettings(settings, group);
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
    int count=settings.value(group+"selections/count", 0).toInt();
    getDataColumnsByUserSaved.clear();
    for (int i=0; i<count; i++) {
        const QString itg=QString("item%1").arg(i);
        QString n=settings.value(group+"selections/"+itg+"/name", "").toString();
        QStringList item=settings.value(group+"selections/"+itg+"/items", QStringList()).toStringList();
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

    plotterStyle.loadSettings(settings, group);
    propagateStyle();

    aspectRatio=settings.value(g+"apect_ratio", aspectRatio).toDouble();
    maintainAspectRatio=settings.value(g+"maintain_apect_ratio", maintainAspectRatio).toBool();
    axisAspectRatio=settings.value(g+"axis_apect_ratio", axisAspectRatio).toDouble();
    maintainAxisAspectRatio=settings.value(g+"maintain_axis_apect_ratio", maintainAxisAspectRatio).toBool();



    mathText.loadSettings(settings, g+"math_text/");

    xAxis->loadSettings(settings, g+"xaxis_");
    yAxis->loadSettings(settings, g+"yaxis_");

    loadUserSettings();

    redrawPlot();
}



void JKQTBasePlotter::correctXYRangeForAspectRatio(double& xminn, double& xmaxx, double& yminn, double& ymaxx) const {
    if (xminn>xmaxx) std::swap(xminn,xmaxx);
    if (yminn>ymaxx) std::swap(yminn,ymaxx);
    if (maintainAspectRatio) {
        if (xAxis->isLinearAxis() && yAxis->isLinearAxis()) {
            const double mid=(yminn+ymaxx)/2.0;
            const double w=fabs(xmaxx-xminn)/aspectRatio;
            //qDebug()<<"mod y from "<<yminn<<"..."<<ymaxx<<" to "<<mid-w/2.0<<"..."<<mid+w/2.0;
            yminn=mid-w/2.0;
            ymaxx=mid+w/2.0;
        } else if (xAxis->isLogAxis() && yAxis->isLogAxis()) {
            const double mid=(log(yminn)+log(ymaxx))/2.0;
            const double w=fabs(log(xmaxx)-log(xminn))/aspectRatio;
            yminn=exp(mid-w/2.0);
            ymaxx=exp(mid+w/2.0);
        }
    }
}

JKQTPBaseKey *JKQTBasePlotter::getMainKey()
{
    return mainKey;
}

const JKQTPBaseKey *JKQTBasePlotter::getMainKey() const
{
    return mainKey;
}


void JKQTBasePlotter::setXY(double xminn, double xmaxx, double yminn, double ymaxx, bool affectsSecondaryAxes) {

    correctXYRangeForAspectRatio(xminn, xmaxx, yminn, ymaxx);

    const double xminpix=xAxis->x2p(xminn);
    const double xmaxpix=xAxis->x2p(xmaxx);
    const double yminpix=yAxis->x2p(yminn);
    const double ymaxpix=yAxis->x2p(ymaxx);

    xAxis->setRange(xminn, xmaxx);
    yAxis->setRange(yminn, ymaxx);

    if (affectsSecondaryAxes) {
        for (auto ax: getXAxes(false)) {
            ax->setRange(ax->p2x(xminpix), ax->p2x(xmaxpix));
        }
        for (auto ax: getYAxes(false)) {
            ax->setRange(ax->p2x(yminpix), ax->p2x(ymaxpix));
        }
    }

    if (emitSignals) emit zoomChangedLocally(xAxis->getMin(), xAxis->getMax(), yAxis->getMin(), yAxis->getMax(), this);
}

void JKQTBasePlotter::setX(double xminn, double xmaxx, bool affectsSecondaryAxes){
    setXY(xminn, xmaxx, yAxis->getMin(), yAxis->getMax(), affectsSecondaryAxes);
}

void JKQTBasePlotter::setY(double yminn, double ymaxx, bool affectsSecondaryAxes) {
    setXY(xAxis->getMin(), xAxis->getMax(), yminn, ymaxx, affectsSecondaryAxes);
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
    internalPlotMargins.clear();
    internalPlotMargins[PlotMarginUse::muUserBorder]=PlotMargin(plotterStyle.plotBorderLeft, plotterStyle.plotBorderRight, plotterStyle.plotBorderTop, plotterStyle.plotBorderBottom);

    // calculate plot label size (if required)
    internalPlotMargins[PlotMarginUse::muPlotTitle]=PlotMargin();
    if (!plotLabel.isEmpty()) {
        QSizeF s=getTextSizeSize(plotterStyle.plotLabelFontName, plotterStyle.plotLabelFontSize*fontSizeMultiplier, plotLabel, painter);
        internalPlotMargins[PlotMarginUse::muPlotTitle].top=s.height()+pt2px(painter, plotterStyle.plotLabelOffset)+pt2px(painter, plotterStyle.plotLabelTopBorder);
    }

    // calculate key/legend size (if required)
    internalPlotKeyDescription = mainKey->getSize(painter);

    // this needs to be done twice, as the kay size calculation needs the internalPlotWidth and internalPlotHeight
    // for a second step
    for (int i=0; i<2; i++) {
    #ifdef JKQTBP_AUTOTIMER
        JKQTPAutoOutputTimer jkaat(QString("JKQTBasePlotter[%1]::calcPlotScaling()::iteration%2").arg(objectName()).arg(i+1));
    #endif

        internalPlotMargins[PlotMarginUse::muKey]=PlotMargin();
        switch(internalPlotKeyDescription.keyLocation) {
        case JKQTPBaseKey::KeySizeDescription::keyInside:
            break;
        case JKQTPBaseKey::KeySizeDescription::keyOutsideLeft:
            internalPlotMargins[PlotMarginUse::muKey].left=internalPlotKeyDescription.requiredSize.width();
            break;
        case JKQTPBaseKey::KeySizeDescription::keyOutsideRight:
            internalPlotMargins[PlotMarginUse::muKey].right=internalPlotKeyDescription.requiredSize.width();
            break;
        case JKQTPBaseKey::KeySizeDescription::keyOutsideTop:
            internalPlotMargins[PlotMarginUse::muKey].top=internalPlotKeyDescription.requiredSize.height();
            break;
        case JKQTPBaseKey::KeySizeDescription::keyOutsideBottom:
            internalPlotMargins[PlotMarginUse::muKey].bottom=internalPlotKeyDescription.requiredSize.height();
            break;
        }



        // read additional size required for coordinate axes
        internalPlotMargins[PlotMarginUse::muAxesOutside]=PlotMargin();
        internalPlotMargins[PlotMarginUse::muAxesOutsideExtend]=PlotMargin();
        internalPlotMargins[PlotMarginUse::muGraphsOutside]=PlotMargin();
        double elongateLeft=0,elongateRight=0;
        auto s=xAxis->getSize1(painter);
        internalPlotMargins[PlotMarginUse::muAxesOutside].bottom+=s.requiredSize;
        if (s.elongateMin>0) elongateLeft=qMax(elongateLeft,s.elongateMin);
        if (s.elongateMax>0) elongateRight=qMax(elongateRight,s.elongateMax);
        s=xAxis->getSize2(painter);
        if (s.elongateMin>0) elongateLeft=qMax(elongateLeft,s.elongateMin);
        if (s.elongateMax>0) elongateRight=qMax(elongateRight,s.elongateMax);
        internalPlotMargins[PlotMarginUse::muAxesOutside].top+=s.requiredSize;

        double elongateBottom=0,elongateTop=0;
        s=yAxis->getSize1(painter);
        if (s.elongateMin>0) elongateBottom=qMax(elongateBottom,s.elongateMin);
        if (s.elongateMax>0) elongateTop=qMax(elongateTop,s.elongateMax);
        internalPlotMargins[PlotMarginUse::muAxesOutside].left+=s.requiredSize;
        s=yAxis->getSize2(painter);
        if (s.elongateMin>0) elongateBottom=qMax(elongateBottom,s.elongateMin);
        if (s.elongateMax>0) elongateTop=qMax(elongateTop,s.elongateMax);
        internalPlotMargins[PlotMarginUse::muAxesOutside].right+=s.requiredSize;


        // read size required by secondary axes
        for (const auto& ax: qAsConst(secondaryYAxis)) {
            const auto s1=ax->getSize1(painter);
            const auto s2=ax->getSize2(painter);
            internalPlotMargins[PlotMarginUse::muAxesOutside].left+=s1.requiredSize+((fabs(s1.requiredSize)>0.1)?pt2px(painter, plotterStyle.secondaryAxisSeparation):0.0);
            internalPlotMargins[PlotMarginUse::muAxesOutside].right+=s2.requiredSize+((fabs(s2.requiredSize)>0.1)?pt2px(painter, plotterStyle.secondaryAxisSeparation):0.0);
        }
        for (const auto& ax: qAsConst(secondaryXAxis)) {
            const auto s1=ax->getSize1(painter);
            const auto s2=ax->getSize2(painter);
            internalPlotMargins[PlotMarginUse::muAxesOutside].bottom+=s1.requiredSize+((fabs(s1.requiredSize)>0.1)?pt2px(painter, plotterStyle.secondaryAxisSeparation):0.0);
            internalPlotMargins[PlotMarginUse::muAxesOutside].top+=s2.requiredSize+((fabs(s2.requiredSize)>0.1)?pt2px(painter, plotterStyle.secondaryAxisSeparation):0.0);
        }

        if (internalPlotMargins.calcRight()<elongateRight) internalPlotMargins[PlotMarginUse::muAxesOutsideExtend].right=elongateRight-internalPlotMargins.calcRight();
        if (internalPlotMargins.calcLeft()<elongateLeft) internalPlotMargins[PlotMarginUse::muAxesOutsideExtend].left=elongateLeft-internalPlotMargins.calcLeft();
        if (internalPlotMargins.calcTop()<elongateTop) internalPlotMargins[PlotMarginUse::muAxesOutsideExtend].top=elongateTop-internalPlotMargins.calcTop();
        if (internalPlotMargins.calcBottom()<elongateBottom) internalPlotMargins[PlotMarginUse::muAxesOutsideExtend].bottom=elongateBottom-internalPlotMargins.calcBottom();


        // read additional space required by graphs
        for (int i=0; i<graphs.size(); i++) {
            if (graphs[i]->isVisible()) {
                int leftSpace, rightSpace, topSpace, bottomSpace;
                graphs[i]->getOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
                internalPlotMargins[PlotMarginUse::muGraphsOutside].bottom+=bottomSpace;
                internalPlotMargins[PlotMarginUse::muGraphsOutside].top+=topSpace;
                internalPlotMargins[PlotMarginUse::muGraphsOutside].left+=leftSpace;
                internalPlotMargins[PlotMarginUse::muGraphsOutside].right+=rightSpace;
            }
        }

    //qDebug()<<"  end JKQTBasePlotter::calcPlotScaling";
        // sum up all borders and store the sums
        internalPlotBorderLeft=internalPlotMargins.calcLeft();
        internalPlotBorderRight=internalPlotMargins.calcRight();
        internalPlotBorderTop=internalPlotMargins.calcTop();
        internalPlotBorderBottom=internalPlotMargins.calcBottom();


        // synchronize to a master-plotter
        if (masterPlotterX) {
            if (masterSynchronizeWidth) {
                internalPlotBorderLeft=masterPlotterX->internalPlotBorderLeft;
                internalPlotBorderRight=masterPlotterX->internalPlotBorderRight;
            }
        }
        if (masterPlotterY) {
            if (masterSynchronizeHeight) {
                internalPlotBorderTop=masterPlotterY->internalPlotBorderTop;
                internalPlotBorderBottom=masterPlotterY->internalPlotBorderBottom;
            }
        }

        // first we calculate the width and height of the plot from the widget dimensions and
        // the supplied border sizes
        internalPlotWidth=jkqtp_roundTo<int>(static_cast<double>(widgetWidth)/paintMagnification-internalPlotBorderLeft-internalPlotBorderRight);
        internalPlotHeight=jkqtp_roundTo<int>(static_cast<double>(widgetHeight)/paintMagnification-internalPlotBorderTop-internalPlotBorderBottom);

        // recalculate key size
        if (i==0) mainKey->modifySize(painter, internalPlotKeyDescription, QSizeF(internalPlotWidth, internalPlotHeight));

    }


    ////////////////////////////////////////////////////////////////////
    // ENSURE ASPECT RATIO (if activated)
    ////////////////////////////////////////////////////////////////////
    if (maintainAspectRatio && (!xAxis->isLogAxis()) && (!xAxis->isLogAxis())) {
        double currRatio=static_cast<double>(internalPlotWidth)/static_cast<double>(internalPlotHeight);
        double newPlotWidth=internalPlotWidth;
        double newPlotHeight=internalPlotHeight;
        double dx=0;
        double dy=0;
        if (jkqtp_approximatelyUnequal(currRatio,aspectRatio)) {
            if (aspectRatio>=currRatio) {
                newPlotWidth=aspectRatio*static_cast<double>(internalPlotHeight);
            } else {
                newPlotHeight=static_cast<double>(internalPlotWidth)/aspectRatio;
            }
            dx=internalPlotWidth-newPlotWidth;
            dy=internalPlotHeight-newPlotHeight;
            if (dx<0) {
                newPlotWidth=internalPlotWidth;
                newPlotHeight=static_cast<double>(internalPlotWidth)/aspectRatio;
            } else if (dy<0) {
                newPlotWidth=aspectRatio*static_cast<double>(internalPlotHeight);
                newPlotHeight=internalPlotHeight;
            }
            dx=internalPlotWidth-newPlotWidth;
            dy=internalPlotHeight-newPlotHeight;
            if ((dx<0)||(dy<0)) {
                newPlotWidth=internalPlotWidth;
                newPlotHeight=internalPlotHeight;
            }
        }
        //if (newPlotWidth>widgetWidth-internalPlotBorderLeft-internalPlotBorderRight) newPlotWidth=widgetWidth-internalPlotBorderLeft-internalPlotBorderRight;
        //if (newPlotHeight>widgetHeight-internalPlotBorderTop-internalPlotBorderBottom) newPlotHeight=widgetHeight-internalPlotBorderTop-internalPlotBorderBottom;
        dx=internalPlotWidth-newPlotWidth;
        dy=internalPlotHeight-newPlotHeight;
        internalPlotBorderBottom+=dy/2.0;
        internalPlotBorderTop+=dy/2.0;
        internalPlotBorderLeft+=dx/2.0;
        internalPlotBorderRight+=dx/2.0;
        internalPlotWidth=jkqtp_roundTo<int>(static_cast<double>(widgetWidth)/paintMagnification-internalPlotBorderLeft-internalPlotBorderRight);
        internalPlotHeight=jkqtp_roundTo<int>(static_cast<double>(widgetHeight)/paintMagnification-internalPlotBorderTop-internalPlotBorderBottom);
    }


    for (auto ax: getAxes(true)) {
        ax->calcPlotScaling(true);
    }

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
        if (jkqtp_approximatelyUnequal(currRatio,axisAspectRatio)) {
            //if (axisAspectRatio<=currRatio) {
                newPlotWidth=axisAspectRatio*cplotHeight;
            //} else {
            //    newPlotHeight=cplotWidth/axisAspectRatio;
            //}
        }
        xAxis->setRange(xmid-newPlotWidth/2.0, xmid+newPlotWidth/2.0);
        yAxis->setRange(ymid-newPlotHeight/2.0, ymid+newPlotHeight/2.0);
        for (auto ax: getAxes(true)) {
            ax->calcPlotScaling(true);
        }
    }


    //if (emitPlotSignals) emit plotScalingRecalculated();
    emit plotScalingRecalculated();
}


void JKQTBasePlotter::drawSystemGrid(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTBasePlotter::drawSystemGrid");
#endif
    //qDebug()<<"start JKQTBasePlotter::drawSystemGrid";
    for (auto ax: getAxes(true)) {
        ax->drawGrids(painter);
    }

    //qDebug()<<"  end JKQTBasePlotter::drawSystemGrid";
}

void JKQTBasePlotter::drawSystemXAxis(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTBasePlotter::drawSystemXAxis");
#endif
    //qDebug()<<"start JKQTBasePlotter::drawSystemXAxis";    
    xAxis->drawAxes(painter);
    //qDebug()<<"  end JKQTBasePlotter::drawSystemXAxis";

    if (secondaryXAxis.size()>0) {
        double ibMove1=xAxis->getSize1(painter).requiredSize;
        //if (ibMove1>0) ibMove1+=pt2px(painter, plotterStyle.secondaryAxisSeparation);
        double ibMove2=xAxis->getSize2(painter).requiredSize;
        //if (ibMove2>0) ibMove2+=pt2px(painter, plotterStyle.secondaryAxisSeparation);

        for (auto& ax: secondaryXAxis) {
            const double add1=ax->getSize1(painter).requiredSize;
            const double add2=ax->getSize2(painter).requiredSize;
            if (add1>0) ibMove1+=pt2px(painter, plotterStyle.secondaryAxisSeparation);
            if (add2>0) ibMove2+=pt2px(painter, plotterStyle.secondaryAxisSeparation);
            ax->drawAxes(painter, ibMove1, ibMove2);
            ibMove1+=add1;
            ibMove2+=add2;
        }
    }
}

void JKQTBasePlotter::drawSystemYAxis(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTBasePlotter::drawSystemYAxis");
#endif
    //qDebug()<<"start JKQTBasePlotter::drawSystemYAxis";
    yAxis->drawAxes(painter);
    //qDebug()<<"  end JKQTBasePlotter::drawSystemYAxis";

    if (secondaryYAxis.size()>0) {
        double ibMove1=yAxis->getSize1(painter).requiredSize;
        //if (ibMove1>0) ibMove1+=pt2px(painter, plotterStyle.secondaryAxisSeparation);
        double ibMove2=yAxis->getSize2(painter).requiredSize;
        //if (ibMove2>0) ibMove2+=pt2px(painter, plotterStyle.secondaryAxisSeparation);

        for (auto& ax: secondaryYAxis) {
            const double add1=ax->getSize1(painter).requiredSize;
            const double add2=ax->getSize2(painter).requiredSize;
            if (add1>0) ibMove1+=pt2px(painter, plotterStyle.secondaryAxisSeparation);
            if (add2>0) ibMove2+=pt2px(painter, plotterStyle.secondaryAxisSeparation);
            ax->drawAxes(painter, ibMove1, ibMove2);
            ibMove1+=add1;
            ibMove2+=add2;
        }
    }
}


JKQTBasePlotter::JKQTPPen JKQTBasePlotter::getPlotStyle(int i, JKQTPPlotStyleType type) const{
    int colorI=-1;
    int styleI=0;
    int symbolI=-1;
    int brushI=0;
    for (int k=0; k<=i; k++) {
        colorI++;
        if (colorI>=plotterStyle.graphsStyle.defaultGraphColors.size()) {
            styleI++;
            brushI++;
            colorI=0;
            if (styleI>=plotterStyle.graphsStyle.defaultGraphPenStyles.size()) styleI=0;
            if (brushI>=plotterStyle.graphsStyle.defaultGraphFillStyles.size()) brushI=0;
        }
        symbolI++;
        if (symbolI>=plotterStyle.graphsStyle.defaultGraphSymbols.size()) {
            symbolI=0;
        }
    }
    JKQTPPen p;
    //std::cout<<"plotstyle "<<i<<std::endl;
    //std::cout<<"color "<<colorI<<std::endl;
    //std::cout<<"style "<<styleI<<std::endl;
    const JKQTGraphsSpecificStyleProperties& baseProps=plotterStyle.graphsStyle.getGraphStyleByType(type);
    Qt::PenStyle basePenStyle=plotterStyle.graphsStyle.defaultGraphPenStyles[styleI];
    JKQTFillStyleSummmary basebrushStyle=plotterStyle.graphsStyle.defaultGraphFillStyles[brushI];
    JKQTPGraphSymbols baseSymbol=plotterStyle.graphsStyle.defaultGraphSymbols[symbolI];
    QColor baseColor=plotterStyle.graphsStyle.defaultGraphColors[colorI];
    double baseWidth=baseProps.defaultLineWidth;
    if (type==JKQTPPlotStyleType::Annotation) {
        baseColor=plotterStyle.graphsStyle.annotationStyle.defaultColor;
        basePenStyle=plotterStyle.graphsStyle.annotationStyle.defaultLineStyle;
        basebrushStyle=plotterStyle.graphsStyle.annotationStyle.defaultFillStyle;
        baseSymbol=plotterStyle.graphsStyle.annotationStyle.defaultSymbol;
    }
    if (type==JKQTPPlotStyleType::Geometric) {
        baseColor=plotterStyle.graphsStyle.geometricStyle.defaultColor;
        basePenStyle=plotterStyle.graphsStyle.geometricStyle.defaultLineStyle;
        basebrushStyle=plotterStyle.graphsStyle.geometricStyle.defaultFillStyle;
        baseSymbol=plotterStyle.graphsStyle.geometricStyle.defaultSymbol;
    }
    if (type==JKQTPPlotStyleType::Barchart || type==JKQTPPlotStyleType::Boxplot || type==JKQTPPlotStyleType::Impulses) {
        basePenStyle=Qt::SolidLine;
    }
    if (type==JKQTPPlotStyleType::Boxplot) {
        basebrushStyle=JKQTFillStyleSummmary(Qt::SolidPattern);
    }
    if (type==JKQTPPlotStyleType::Impulses) {
    }
    const QColor lineColor=JKQTPGetDerivedColor(baseProps.graphColorDerivationMode, baseColor);
    const QColor errorColor=JKQTPGetDerivedColor(baseProps.errorColorDerivationMode, baseColor);

    p.setColor(lineColor);
    p.setStyle(basePenStyle);
    p.setSymbolType(baseSymbol);
    p.setFillStyle(basebrushStyle);
    p.setErrorFillStyle(Qt::SolidPattern);
    p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, baseWidth));
    p.setFillColor(JKQTPGetDerivedColor(baseProps.fillColorDerivationMode, baseColor));
    p.setErrorLineColor(errorColor);
    p.setErrorFillColor(JKQTPGetDerivedColor(baseProps.errorFillColorDerivationMode, baseColor));
    //qDebug()<<"baseColor="<<baseColor<<"/"<<baseColor.alphaF()*100.0<<"% --> ErrorFillColor="<<p.errorFillColor()<<"/"<<p.errorFillColor().alphaF()*100.0<<"%";
    p.setErrorLineWidth(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, baseProps.defaultErrorIndicatorWidth));
    p.setSymbolSize(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, baseProps.defaultSymbolSize));
    p.setSymbolFillColor(JKQTPGetDerivedColor(baseProps.symbolFillColorDerivationMode, baseColor));
    p.setSymbolLineWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, baseProps.defaultSymbolLineWidth));
    return p;
}

int JKQTBasePlotter::getNextStyle() {
    int res=0;
    while (usedStyles.contains(res)) res++;
    //std::cout<<res<<std::endl;
    usedStyles.push_back(res);
    return res;
};

void JKQTBasePlotter::drawKey(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTBasePlotter::drawKey");
#endif
    // calculate rect of the actual plot
    QRectF rPlotRect;

    switch(internalPlotKeyDescription.keyLocation) {
        case JKQTPBaseKey::KeySizeDescription::keyInside:
            rPlotRect=calcPlotRect();
            break;
        case JKQTPBaseKey::KeySizeDescription::keyOutsideLeft:
            rPlotRect=calcPlotMarginRect(muKey, sideLeft);
            break;
        case JKQTPBaseKey::KeySizeDescription::keyOutsideRight:
            rPlotRect=calcPlotMarginRect(muKey, sideRight);
            break;
        case JKQTPBaseKey::KeySizeDescription::keyOutsideTop:
            rPlotRect=calcPlotMarginRect(muKey, sideTop);
            break;
        case JKQTPBaseKey::KeySizeDescription::keyOutsideBottom:
            rPlotRect=calcPlotMarginRect(muKey, sideBottom);
            break;
    }
    if (!rPlotRect.isEmpty())  {
        mainKey->drawKey(painter, rPlotRect, internalPlotKeyDescription);
        if (plotterStyle.debugShowRegionBoxes) {
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            QPen p("darkblue");
            QColor col=p.color(); col.setAlphaF(0.8f); p.setColor(col);
            p.setWidthF(plotterStyle.debugRegionLineWidth);
            painter.setBrush(QBrush(QColor(Qt::transparent)));
            painter.setPen(p);
            painter.drawRect(rPlotRect);

        }
    }
}

void JKQTBasePlotter::drawPlotLabel(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTBasePlotter::drawPlotLabel");
#endif
    if (plotLabel.isEmpty()) return;

    mathText.setFontSize(plotterStyle.plotLabelFontSize*fontSizeMultiplier);
    mathText.setFontSpecial(plotterStyle.plotLabelFontName);
    mathText.setFontColor(plotterStyle.plotLabelColor);

    mathText.parse(plotLabel);
    QRectF rPlotLabel=calcPlotMarginRect(muPlotTitle, sideTop);
    rPlotLabel.setTop(rPlotLabel.top()+pt2px(painter, plotterStyle.plotLabelTopBorder));
    mathText.draw(painter, Qt::AlignHCenter | Qt::AlignTop, rPlotLabel, plotterStyle.debugShowTextBoxes);
    if (plotterStyle.debugShowRegionBoxes) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        QPen p("blue");
        QColor col=p.color(); col.setAlphaF(0.8f); p.setColor(col);
        p.setWidthF(plotterStyle.debugRegionLineWidth);
        painter.setBrush(QBrush(QColor(Qt::transparent)));
        painter.setPen(p);
        painter.drawRect(rPlotLabel);
        rPlotLabel.setTop(rPlotLabel.top()-pt2px(painter, plotterStyle.plotLabelTopBorder));
        painter.drawRect(rPlotLabel);

    }
}



void JKQTBasePlotter::drawPlot(JKQTPEnhancedPainter& painter) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot("JKQTBasePlotter::drawPlot()");
#endif

#if QT_VERSION<QT_VERSION_CHECK(6,0,0)
    painter.setRenderHint(JKQTPEnhancedPainter::NonCosmeticDefaultPen, true);
#endif
    painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing, plotterStyle.useAntiAliasingForSystem);
    painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing, plotterStyle.useAntiAliasingForText);


    // draw background
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setPen(Qt::NoPen);
        if (plotterStyle.widgetBackgroundBrush!=QBrush(Qt::transparent)) painter.fillRect(QRectF(0,0,widgetWidth/paintMagnification, widgetHeight/paintMagnification), plotterStyle.widgetBackgroundBrush);
    }

    // calculate rect of the actual plot
    const QRectF rPlotRect=calcPlotRect();


    // draw plot frame
    {
        QPen penPlotFrame(Qt::NoPen);
        if (plotterStyle.plotFrameVisible) {
            penPlotFrame=QPen(plotterStyle.plotFrameColor);
            penPlotFrame.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, pt2px(painter, plotterStyle.plotFrameWidth*lineWidthMultiplier)));
        }
        painter.setPen(penPlotFrame);
        painter.setBrush(plotterStyle.plotBackgroundBrush);
        painter.drawRoundedRectOrRect(rPlotRect, plotterStyle.plotFrameRounding);
    }


    // overlay plot frrame with the grid(s)
    drawSystemGrid(painter);


    // draw the plot label (typically above the graph)
    drawPlotLabel(painter);

    // draw the graphs
    {
        painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing, plotterStyle.graphsStyle.useAntiAliasingForGraphs);
        painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing, plotterStyle.useAntiAliasingForText);
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        drawGraphs(painter);
    }

    // draw coordinate systems (first and secondary)
    painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing, plotterStyle.useAntiAliasingForSystem);
    painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing, plotterStyle.useAntiAliasingForText);
    drawSystemXAxis(painter);
    drawSystemYAxis(painter);

    // draw plot key/legend
    painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing, plotterStyle.graphsStyle.useAntiAliasingForGraphs);
    painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing, plotterStyle.useAntiAliasingForText);
    if (plotterStyle.keyStyle.visible) drawKey(painter);

    if (plotterStyle.debugShowRegionBoxes) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        QPen p("red");
        QColor col=p.color(); col.setAlphaF(0.8f); p.setColor(col);
        p.setWidthF(plotterStyle.debugRegionLineWidth);
        p.setStyle(Qt::DashLine);
        painter.setPen(p);
        painter.setBrush(QBrush(QColor(Qt::transparent)));
        painter.drawRect(rPlotRect);
        col=p.color(); col.setAlphaF(0.8f); p.setColor(col);
        p.setWidthF(plotterStyle.debugRegionLineWidth);
        p.setStyle(Qt::SolidLine);
        painter.setPen(p);
        painter.drawRect(0, 0, widgetWidth, widgetHeight);

    }

    //qDebug()<<"  end JKQTBasePlotter::paintPlot";
}


void JKQTBasePlotter::gridPrintingCalc() {
    gridPrintingRows.clear();
    gridPrintingColumns.clear();
    if (!gridPrinting) {
        gridPrintingSize=QSize(static_cast<int>(widgetWidth/paintMagnification), static_cast<int>(widgetHeight/paintMagnification));
        gridPrintingRows.push_back(static_cast<size_t>(widgetHeight/paintMagnification));
        gridPrintingColumns.push_back(static_cast<size_t>(widgetWidth/paintMagnification));
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
        gridPrintingColumns[static_cast<int>(gridPrintingCurrentX)]=static_cast<size_t>(widgetWidth/paintMagnification);
        gridPrintingRows[static_cast<int>(gridPrintingCurrentY)]=static_cast<size_t>(widgetHeight/paintMagnification);
        for (int i=0; i< gridPrintingList.size(); i++) {
            size_t cw=static_cast<size_t>(gridPrintingList[i].plotter->widgetWidth/paintMagnification);
            size_t ch=static_cast<size_t>(gridPrintingList[i].plotter->widgetHeight/paintMagnification);
            size_t c=gridPrintingList[i].x;
            size_t r=gridPrintingList[i].y;
            if (cw>gridPrintingColumns[static_cast<int>(c)]) gridPrintingColumns[static_cast<int>(c)]=cw;
            if (ch>gridPrintingRows[static_cast<int>(r)]) gridPrintingRows[static_cast<int>(r)]=ch;
        }
        int w=0;
        int h=0;
        for (int i=0; i<gridPrintingColumns.size(); i++) { w+=static_cast<int>(gridPrintingColumns[i]); };
        for (int i=0; i<gridPrintingRows.size(); i++) { h+= static_cast<int>(gridPrintingRows[i]); };
        gridPrintingSize=QSize(w, h);
    }
}

void JKQTBasePlotter::gridPaint(JKQTPEnhancedPainter& painter, QSizeF pageRect, bool scaleIfTooLarge, bool scaleIfTooSmall) {
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
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        // scale the plot so it fits on the page
        painter.scale(scale, scale);
        drawPlot(painter);

    } else {

        QList<double> fsm, lwm, pm;
        QList<QBrush> backg;
        for (int i=0; i< gridPrintingList.size(); i++) {
            fsm.append(gridPrintingList[i].plotter->getFontSizeMultiplier());
            lwm.append(gridPrintingList[i].plotter->getLineWidthMultiplier());
            pm.append(gridPrintingList[i].plotter->getPaintMagnification());
            backg.append(gridPrintingList[i].plotter->getExportBackgroundBrush());
            gridPrintingList[i].plotter->setFontSizeMultiplier(fontSizeMultiplier);
            gridPrintingList[i].plotter->setLineWidthMultiplier(lineWidthMultiplier);
            gridPrintingList[i].plotter->setPaintMagnification(paintMagnification);
            gridPrintingList[i].plotter->setBackgroundBrush(gridPrintingList[i].plotter->getExportBackgroundBrush());
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
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.scale(scale, scale);

        {
            // plot this plotter
            painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
            int t_x=0;
            int t_y=0;
            //std::cout<<"printing this ...\n";
            for (size_t i=0; i<gridPrintingCurrentX; i++) { t_x+= static_cast<int>(gridPrintingColumns[static_cast<int>(i)]); }
            for (size_t i=0; i<gridPrintingCurrentY; i++) { t_y+= static_cast<int>(gridPrintingRows[static_cast<int>(i)]); }
            //std::cout<<"printing this @ "<<t_x<<", "<<t_y<<" ...\n";
            painter.translate(t_x, t_y);
            drawPlot(painter);

            //std::cout<<"this printed ...\n";

            // plot all the other plotters
            for (int i=0; i< gridPrintingList.size(); i++) {
                //std::cout<<"printing "<<i<<" ...\n";
                painter.save(); auto __finalpaintinnerloop=JKQTPFinally([&painter]() {painter.restore();});
                int gt_x=0;
                int gt_y=0;
                //std::cout<<"printing "<<i<<" @g "<<gridPrintingList[i].x<<", "<<gridPrintingList[i].y<<" ...\n";
                //std::cout<<"colrowlistsizes     "<<gridPrintingColumns.size()<<", "<<gridPrintingRows.size()<<" ...\n";
                for (size_t j=0; j<gridPrintingList[i].x; j++) {  gt_x+= static_cast<int>(gridPrintingColumns[static_cast<int>(j)]);  }
                for (size_t j=0; j<gridPrintingList[i].y; j++) {  gt_y+= static_cast<int>(gridPrintingRows[static_cast<int>(j)]); }
                //std::cout<<"printing "<<i<<" @ "<<t_x<<", "<<t_y<<" ...\n";
                painter.translate(gt_x, gt_y);
                gridPrintingList[i].plotter->drawPlot(painter);

            }



            for (int i=0; i< gridPrintingList.size(); i++) {
                gridPrintingList[i].plotter->setFontSizeMultiplier(fsm[i]);
                gridPrintingList[i].plotter->setLineWidthMultiplier(lwm[i]);
                gridPrintingList[i].plotter->setPaintMagnification(pm[i]);
                gridPrintingList[i].plotter->setBackgroundBrush(backg[i]);
                gridPrintingList[i].plotter->redrawPlot();
            }
        }

    }
}


#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
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

    p->setPageMargins(QMarginsF(10,10,10,10),QPageLayout::Millimeter);

    if (widgetWidth>widgetHeight) {
        p->setPageOrientation(QPageLayout::Landscape);
    } else {
        p->setPageOrientation(QPageLayout::Portrait);
    }

    emit beforeExporting();; auto __finalpaint=JKQTPFinally([&]() { emit afterExporting();});
    printpreviewNew(p, false, -1.0, -1.0, displayPreview);

    if (delP) delete p;
    saveUserSettings();
}
#endif


bool JKQTBasePlotter::printpreviewNew(QPaintDevice* paintDevice, bool setAbsolutePaperSize, double printsizeX_inMM, double printsizeY_inMM, bool displayPreview) {

    JKQTPASSERT_M(paintDevice, "INTERNAL ERROR: no QPaintDevice given to JKQTBasePlotter::printpreviewNew()!");


#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    QPrinter *printer=dynamic_cast<QPrinter*>(paintDevice);
    QSvgGenerator* svg=dynamic_cast<QSvgGenerator*>(paintDevice);
#endif
    double lw=lineWidthMultiplier;
    double fs=fontSizeMultiplier;
    double oldP=paintMagnification;
    QBrush bc=plotterStyle.widgetBackgroundBrush;
    plotterStyle.widgetBackgroundBrush=plotterStyle.exportBackgroundBrush;
    lineWidthMultiplier=lineWidthPrintMultiplier;
    fontSizeMultiplier=fontSizePrintMultiplier;
    exportPreviewLabel=nullptr;
    printMagnification=1.0;
    paintMagnification=1.0;
    gridPrintingCalc();


    printAspect=gridPrintingSize.height()/gridPrintingSize.width();
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    if (printer) printPageSizeMM=printer->pageLayout().pageSize().size(QPageSize::Millimeter);
    else if (paintDevice) printPageSizeMM=QSizeF(paintDevice->widthMM(), paintDevice->heightMM());
#else
    if (paintDevice) printPageSizeMM=QSizeF(paintDevice->widthMM(), paintDevice->heightMM());
#endif
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

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    bool delPrinter=false;
    if (svg) {
        printer=new QPrinter();
        printer->setColorMode(QPrinter::Color);
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setResolution(svg->logicalDpiX());
        printer->setPageMargins(QMarginsF(0,0,0,0),QPageLayout::Millimeter);
        printer->setColorMode(QPrinter::Color);
        delPrinter=true;
    } else if (!printer) {
        printer=new QPrinter();
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setResolution(paintDevice->logicalDpiX());
        printer->setPageMargins(QMarginsF(0,0,0,0),QPageLayout::Millimeter);
        printer->setColorMode(QPrinter::Color);
        delPrinter=true;
    }

    printPreview=new QPrintPreviewWidget(printer, dlg);
    connect(printPreview, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printpreviewPaintRequestedNewPrinter(QPrinter*)));
#endif

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
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    } else if (printer && !svg) {
        int y=layout->columnCount();
        layout->addWidget(new QLabel(tr("plot size: ")), 0,y);
#endif
    } else {
        int y=layout->columnCount();
        layout->addWidget(new QLabel(tr("paper size: ")), 1,y);
        layout->addWidget(new QLabel(tr("plot size: ")), 0,y);
    }
    layout->addWidget(spinSizeX, 0,layout->columnCount());
    layout->addWidget(new QLabel(tr(" x ")), 0,layout->columnCount());
    layout->addWidget(spinSizeY, 0,layout->columnCount());
    layout->addWidget(chkAspect, 0,layout->columnCount());
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    if (!setAbsolutePaperSize && printer && !svg) {
        layout->addWidget(new QLabel(tr("%1x%2 mm^2").arg(printer->pageLayout().pageSize().size(QPageSize::Millimeter).width()).arg(printer->pageLayout().pageSize().size(QPageSize::Millimeter).height())), 1,layout->columnCount()-4, 1, 4);

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
#endif


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
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    layout->addWidget(printPreview, layout->rowCount(),0, 1, layout->columnCount());
#endif
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), dlg, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dlg, SLOT(reject()));
    layout->addWidget(buttonBox, layout->rowCount(),0, 1, layout->columnCount());
    dlg->resize(800,500);

    bool res=false;
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    if (printer) {
        if (!displayPreview || dlg->exec()==QDialog::Accepted) {
            //qDebug()<<svg<<printer<<delPrinter;
            if (svg) {
                printpreviewPaintRequestedNewPaintDevice(svg);
            } else if (!delPrinter) {
                printpreviewPaintRequestedNewPrinter(printer);
            } else {
                printpreviewPaintRequestedNewPaintDevice(paintDevice);
            }
            res=true;
        }
    }
    if ((svg||delPrinter) && printer) {
        delete printer;
    }
#endif
    delete dlg;
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    printPreview=nullptr;
#endif
    lineWidthMultiplier=lw;
    fontSizeMultiplier=fs;
    plotterStyle.widgetBackgroundBrush=bc;
    paintMagnification=oldP;


    return res;

}

bool JKQTBasePlotter::exportpreview(QSizeF pageSize, bool unitIsMM) {
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    printPreview=nullptr;
#endif
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
    QScrollArea* scroll=new QScrollArea(dlg);
    {
        QPalette p(scroll->palette());
        // Set background colour to black
        p.setColor(QPalette::Window, Qt::darkGray);
        scroll->setPalette(p);
    }
    exportPreviewLabel=new QLabel(scroll);
    exportPreviewLabel->setMargin(10);
    scroll->setWidget(exportPreviewLabel);
    {
        QPalette p(exportPreviewLabel->palette());
        // Set background colour to black
        p.setColor(QPalette::Window, Qt::darkGray);
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

void JKQTBasePlotter::setFontSizeMultiplier(double __value)
{
    this->fontSizeMultiplier = __value;
}

void JKQTBasePlotter::setLineWidthMultiplier(double __value)
{
    this->lineWidthMultiplier = __value;
}

void JKQTBasePlotter::setPrintMagnification(double __value)
{
    this->printMagnification = __value;
}

double JKQTBasePlotter::getPrintMagnification() const
{
    return this->printMagnification;
}

void JKQTBasePlotter::setPaintMagnification(double __value)
{
    this->paintMagnification = __value;
}

double JKQTBasePlotter::getPaintMagnification() const
{
    return this->paintMagnification;
}

void JKQTBasePlotter::updatePreviewLabel() {
    double factor=1;
    if (exportUnitInMM) factor=600.0/double(printSizeX_Millimeter);
    QImage pix(jkqtp_roundTo<int>(double(printSizeX_Millimeter)*factor*1.1), jkqtp_roundTo<int>(double(printSizeY_Millimeter)*factor*1.1), QImage::Format_ARGB32);
    pix.fill(Qt::transparent);
    JKQTPEnhancedPainter painter;
    painter.begin(&pix);
    exportpreviewPaintRequested(painter, QSize(jkqtp_roundTo<int>(double(printSizeX_Millimeter)*factor), jkqtp_roundTo<int>(double(printSizeY_Millimeter)*factor)));
    painter.end();
    if (exportPreviewLabel) {
        QPixmap pm;
        pm.convertFromImage(pix);
        exportPreviewLabel->setPixmap(pm);
        exportPreviewLabel->resize(pm.size());
    }
}

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
void JKQTBasePlotter::printpreviewPaintRequested(QPrinter* printer) {
    JKQTPASSERT_M(printer, "INTERNAL ERROR: no QPrinter given to JKQTBasePlotter::printpreviewPaintRequested()!");

    double lw=lineWidthMultiplier;
    double fs=fontSizeMultiplier;
    QBrush bc=plotterStyle.widgetBackgroundBrush;
    plotterStyle.widgetBackgroundBrush=plotterStyle.exportBackgroundBrush;
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
        //printer->setPageSize(QPrinter::Custom);
        printer->setPageOrientation(QPageLayout::Portrait);
        printer->setPageSize(QPageSize(QSizeF(printSizeX_Millimeter, printSizeY_Millimeter), QPageSize::Millimeter));
        if (!gridPrinting) widgetHeight=jkqtp_roundTo<int>(widgetWidth*printSizeY_Millimeter/printSizeX_Millimeter);

    }

    JKQTPEnhancedPainter painter;
    painter.setPainterFlag(JKQTPEnhancedPainter::VectorPainting);
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
    qDebug()<<"print with paper size "<<printer->pageLayout().paintRectPixels(printer->resolution()).size()<<" ";
    QSizeF sl=QSizeF(QSizeF(printer->pageLayout().paintRectPixels(printer->resolution()).size()).width()/printer->logicalDpiX()*25.4, QSizeF(printer->pageLayout().paintRectPixels(printer->resolution()).size()).height()/printer->logicalDpiY()*25.4);
    qDebug()<<"print with paper size "<<sl<<" mm^2";
    qDebug()<<"paintMagnification = "<<paintMagnification;
    qDebug()<<"lineWidthMultiplier = "<<lineWidthMultiplier;
    qDebug()<<"fontSizeMultiplier = "<<fontSizeMultiplier;
    qDebug()<<"printer resolution = "<<printer->resolution()<<" dpi";
    qDebug()<<"\nplotLabelFontSize = "<<plotterStyle.plotLabelFontSize<<" pt";
    qDebug()<<"plotterStyle.keyStyle.fontSize = "<<plotterStyle.keyStyle.fontSize<<" pt";
    qDebug()<<"x-axis label fontsize = "<<xAxis->getLabelFontSize()<<" pt";
    qDebug()<<"y-axis label fontsize = "<<yAxis->getLabelFontSize()<<" pt";
#endif
    gridPaint(painter, printer->pageLayout().paintRectPixels(printer->resolution()).size(), printScaleToPagesize, printScaleToPagesize);
    painter.end();
    widgetWidth=oldWidgetWidth;
    widgetHeight=oldWidgetHeight;
    QApplication::restoreOverrideCursor();
    QApplication::processEvents();
    emitPlotSignals=oldEmitPlotSignals;
    lineWidthMultiplier=lw;
    fontSizeMultiplier=fs;
    paintMagnification=oldpm;
    plotterStyle.widgetBackgroundBrush=bc;

}


void JKQTBasePlotter::printpreviewPaintRequestedNewPrinter(QPrinter* printer) {
    JKQTPASSERT_M(printer, "INTERNAL ERROR: no QPrinter given to JKQTBasePlotter::printpreviewPaintRequestedNewPrinter()!");
    QPaintDevice* paintDevice=dynamic_cast<QPaintDevice*>(printer);
    printpreviewPaintRequestedNewPaintDevice(paintDevice);

}
#endif

void JKQTBasePlotter::printpreviewPaintRequestedNewPaintDevice(QPaintDevice *paintDevice)
{
    JKQTPASSERT_M(paintDevice, "INTERNAL ERROR: no QPaintDevice given to JKQTBasePlotter::printpreviewPaintRequestedNewPaintDevice()!");

    //QPaintDevice* paintDevice=dynamic_cast<QPaintDevice*>(printer);
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    QPrinter* printer=dynamic_cast<QPrinter*>(paintDevice);
    QSvgGenerator* svg=dynamic_cast<QSvgGenerator*>(paintDevice);
#endif

    double lw=lineWidthMultiplier;
    double fs=fontSizeMultiplier;
    QBrush bc=plotterStyle.widgetBackgroundBrush;
    plotterStyle.widgetBackgroundBrush=plotterStyle.exportBackgroundBrush;
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
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        if (printer) {
            printer->setPageOrientation(QPageLayout::Portrait);
            printer->setPageSize(QPageSize(QSizeF(printSizeX_Millimeter, printSizeY_Millimeter), QPageSize::Millimeter));
        } else if (svg) {
            QRectF siz=QRectF(0,0,printSizeX_Millimeter,printSizeY_Millimeter);
            svg->setSize(QSizeF(ceil(siz.width()*svg->resolution()/25.4), ceil(siz.height()*svg->resolution()/25.4)).toSize());
            svg->setViewBox(QRect(0,0,-1,-1));//*25.4/double(svg->resolution()), printSizeY_Millimeter*25.4/double(svg->resolution())));
        }
#endif
    }
    paintMagnification=gridPrintingSize.width()/(printSizeX_Millimeter/25.4*double(paintDevice->logicalDpiX()));
    if (!gridPrinting) widgetHeight=jkqtp_roundTo<int>(widgetWidth*printSizeY_Millimeter/printSizeX_Millimeter);

    JKQTPEnhancedPainter painter;
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    if (printer) painter.begin(printer);
    else if (svg) painter.begin(svg);
    else painter.begin(paintDevice);
    if (printer||svg) painter.setPainterFlag(JKQTPEnhancedPainter::VectorPainting);
#else
    painter.begin(paintDevice);
#endif
    if (printKeepAbsoluteFontSizes) {
        fontSizeMultiplier=1.0;//1.0/paintMagnification;
    }

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    if (svg) {
        lineWidthMultiplier=lineWidthMultiplier*72.0/96.0;
        fontSizeMultiplier=fontSizeMultiplier*72.0/96.0;
    }
#endif
#ifdef SHOW_JKQTPLOTTER_DEBUG
    qDebug()<<"\n\n\n==========================================================================";
    qDebug()<<"printAspect = "<<printAspect;
    qDebug()<<"printKeepAspect = "<<printKeepAspect;
    qDebug()<<"printSetAbsoluteSize = "<<printSetAbsolutePageSize;
    qDebug()<<"printScaleToPagesize = "<<printScaleToPagesize;
    qDebug()<<"printKeepAbsoluteFontSizes = "<<printKeepAbsoluteFontSizes;
    qDebug()<<"print with widget size "<<widgetWidth<<widgetHeight;
    if (printer) {
        qDebug()<<"print with paper size "<<printer->pageLayout().paintRectPixels(printer->resolution()).size()<<" ";
        QSizeF sl=QSizeF(QSizeF(printer->pageLayout().paintRectPixels(printer->resolution()).size()).width()/printer->logicalDpiX()*25.4, QSizeF(printer->pageLayout().paintRectPixels(printer->resolution()).size()).height()/printer->logicalDpiY()*25.4);
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
    qDebug()<<"\nplotLabelFontSize = "<<plotterStyle.plotLabelFontSize<<" pt";
    qDebug()<<"plotterStyle.keyStyle.fontSize = "<<plotterStyle.keyStyle.fontSize<<" pt";
    qDebug()<<"x-axis label fontsize = "<<xAxis->getLabelFontSize()<<" pt";
    qDebug()<<"y-axis label fontsize = "<<yAxis->getLabelFontSize()<<" pt";
#endif
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    if (printer) gridPaint(painter, printer->pageLayout().paintRectPixels(printer->resolution()).size(), printScaleToPagesize, printScaleToPagesize);
    else if (svg) gridPaint(painter, svg->size(), printScaleToPagesize, printScaleToPagesize);
    else gridPaint(painter, QSizeF(paintDevice->width(), paintDevice->height()), printScaleToPagesize, printScaleToPagesize);
#else
    gridPaint(painter, QSizeF(paintDevice->width(), paintDevice->height()), printScaleToPagesize, printScaleToPagesize);
#endif
    painter.end();
    widgetWidth=oldWidgetWidth;
    widgetHeight=oldWidgetHeight;
    QApplication::restoreOverrideCursor();
    QApplication::processEvents();
    emitPlotSignals=oldEmitPlotSignals;
    lineWidthMultiplier=lw;
    fontSizeMultiplier=fs;
    paintMagnification=oldpm;
    plotterStyle.widgetBackgroundBrush=bc;
}

void JKQTBasePlotter::exportpreviewPaintRequested(JKQTPEnhancedPainter &painter, QSize size) {
    double lw=lineWidthMultiplier;
    double fs=fontSizeMultiplier;
    QBrush bc=plotterStyle.widgetBackgroundBrush;
    plotterStyle.widgetBackgroundBrush=plotterStyle.exportBackgroundBrush;
    lineWidthMultiplier=lineWidthPrintMultiplier;
    fontSizeMultiplier=fontSizePrintMultiplier;
    bool oldEmitPlotSignals=emitPlotSignals;
    emitPlotSignals=false;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QApplication::processEvents();
    int oldWidgetWidth=widgetWidth;
    int oldWidgetHeight=widgetHeight;
    //widgetWidth=size.width();
    //widgetHeight=size.height();

    gridPaint(painter, size);
    widgetWidth=oldWidgetWidth;
    widgetHeight=oldWidgetHeight;
    QApplication::restoreOverrideCursor();
    QApplication::processEvents();
    emitPlotSignals=oldEmitPlotSignals;
    lineWidthMultiplier=lw;
    fontSizeMultiplier=fs;
    plotterStyle.widgetBackgroundBrush=bc;
}

void JKQTBasePlotter::printpreviewSetZoom(double value) {
    printZoomFactor=value/100.0;
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    if (printPreview) printPreview->updatePreview();
#endif
    if (exportPreviewLabel) updatePreviewLabel();
}

void JKQTBasePlotter::printpreviewSetSizeX(double value) {
    printSizeX_Millimeter=value;
    if (printKeepAspect) {
        printSizeY_Millimeter=printSizeX_Millimeter*printAspect;
        spinSizeY->setValue(printSizeY_Millimeter);
    }
    if (printDoUpdate) {
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        if (printPreview) printPreview->updatePreview();
#endif
        if (exportPreviewLabel) updatePreviewLabel();
    }
}

void JKQTBasePlotter::printpreviewSetSizeY(double value) {
    if (printKeepAspect) return;
    printSizeY_Millimeter=value;
    if (printDoUpdate) {
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        if (printPreview) printPreview->updatePreview();
#endif
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
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    if (printPreview) printPreview->updatePreview();
#endif
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
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    if (printPreview) printPreview->updatePreview();
#endif
    if (exportPreviewLabel) updatePreviewLabel();
}

void JKQTBasePlotter::printpreviewSetKeepAbsFontsize(bool checked)
{
    printKeepAbsoluteFontSizes=checked;
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    if (printPreview) printPreview->updatePreview();
#endif
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
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    if (printPreview) printPreview->updatePreview();
#endif
    if (exportPreviewLabel) updatePreviewLabel();
}

void JKQTBasePlotter::printpreviewSetFontSizeMultiplier(double value) {
    fontSizePrintMultiplier=value/100.0;
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    if (printPreview) printPreview->updatePreview();
#endif
    if (exportPreviewLabel) updatePreviewLabel();
}

void JKQTBasePlotter::printpreviewUpdate()
{
    if (printDoUpdate) {
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        if (printPreview) printPreview->updatePreview();
#endif
        if (exportPreviewLabel) updatePreviewLabel();
    }
}

void JKQTBasePlotter::draw(JKQTPEnhancedPainter& painter, const QRect& rect) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTBasePlotter::draw(rect, %1)"));
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
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(rect.topLeft());
    #ifdef JKQTBP_DEBUGTIMING
        QElapsedTimer time;
        time.start();
    #endif
        gridPaint(painter, rect.size());
    #ifdef JKQTBP_DEBUGTIMING
        qDebug()<<on<<"::draw ... gridPaint       = " <<time.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(time.nsecsElapsed())/1000000.0<<" msecs";
    #endif
    }
    emitPlotSignals=oldEmitPlotSignals;
}


void JKQTBasePlotter::draw(JKQTPEnhancedPainter& painter, const QPoint& pos) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTBasePlotter::draw(pos, %1)"));
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
    QRectF rect(pos, QSizeF(widgetWidth/paintMagnification, widgetHeight/paintMagnification));
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(rect.topLeft());
    #ifdef JKQTBP_DEBUGTIMING
        QElapsedTimer time;
        time.start();
    #endif
        gridPaint(painter, rect.size());
    #ifdef JKQTBP_DEBUGTIMING
        qDebug()<<on<<"::draw ... gridPaint       = " <<time.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(time.nsecsElapsed())/1000000.0<<" msecs";
    #endif
    }
    emitPlotSignals=oldEmitPlotSignals;
}

void JKQTBasePlotter::drawNonGrid(JKQTPEnhancedPainter& painter, const QRect& rect) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTBasePlotter::drawNonGrid(rect, %1)"));
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
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.translate(rect.topLeft());
#ifdef JKQTBP_DEBUGTIMING
    QElapsedTimer time;
    time.start();
#endif
    calcPlotScaling(painter);
#ifdef JKQTBP_DEBUGTIMING
    qDebug()<<on<<"::drawNonGrid ... calcPlotScaling = " <<time.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(time.nsecsElapsed())/1000000.0<<" msecs";
#endif
    {
        double scale=static_cast<double>(rect.width())/static_cast<double>(widgetWidth)*paintMagnification;
        if ((scale*static_cast<double>(widgetWidth)/paintMagnification>static_cast<double>(rect.width())) || (scale*static_cast<double>(widgetHeight)/paintMagnification>static_cast<double>(rect.height()))) {
            scale=static_cast<double>(rect.height())/static_cast<double>(widgetHeight)*paintMagnification;
        }
        painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
        // scale the plot so it fits on the page
        painter.scale(scale, scale);
    #ifdef JKQTBP_DEBUGTIMING
        time.start();
    #endif
        drawPlot(painter);
    #ifdef JKQTBP_DEBUGTIMING
        qDebug()<<on<<"::drawNonGrid ... paintPlot       = " <<time.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(time.nsecsElapsed())/1000000.0<<" msecs";
    #endif


    #ifdef JKQTBP_DEBUGTIMING
        qDebug()<<on<<"::drawNonGrid ... DONE            = "<<timeAll.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(timeAll.nsecsElapsed())/1000000.0<<" msecs";
    #endif
    }
    emitPlotSignals=oldEmitPlotSignals;
}


void JKQTBasePlotter::drawNonGrid(JKQTPEnhancedPainter& painter, const QPoint& pos) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTBasePlotter::drawNonGrid(pos)"));
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
    QRectF rect(pos, QSizeF(widgetWidth/paintMagnification, widgetHeight/paintMagnification));
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.translate(rect.topLeft());
#ifdef JKQTBP_DEBUGTIMING
    QElapsedTimer time;
    time.start();
#endif
    calcPlotScaling(painter);
#ifdef JKQTBP_DEBUGTIMING
    qDebug()<<on<<"::drawNonGrid ... calcPlotScaling = " <<time.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(time.nsecsElapsed())/1000000.0<<" msecs";
#endif
    {
        double scale=static_cast<double>(rect.width())/static_cast<double>(widgetWidth)*paintMagnification;
        if ((scale*static_cast<double>(widgetWidth)/paintMagnification>static_cast<double>(rect.width())) || (scale*static_cast<double>(widgetHeight)/paintMagnification>static_cast<double>(rect.height()))) {
            scale=static_cast<double>(rect.height())/static_cast<double>(widgetHeight)*paintMagnification;
        }
        painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
        // scale the plot so it fits on the page
        painter.scale(scale, scale);
    #ifdef JKQTBP_DEBUGTIMING
        time.start();
    #endif
        drawPlot(painter);
    #ifdef JKQTBP_DEBUGTIMING
        qDebug()<<on<<"::drawNonGrid ... paintPlot       = " <<time.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(time.nsecsElapsed())/1000000.0<<" msecs";
    #endif
    }


#ifdef JKQTBP_DEBUGTIMING
    qDebug()<<on<<"::drawNonGrid ... DONE            = "<<timeAll.nsecsElapsed()/1000<<" usecs = "<<static_cast<double>(timeAll.nsecsElapsed())/1000000.0<<" msecs";
#endif
    emitPlotSignals=oldEmitPlotSignals;
}

void JKQTBasePlotter::updateSecondaryAxes()
{

}

void JKQTBasePlotter::setEmittingPlotSignalsEnabled(bool __value)
{
    this->emitPlotSignals = __value;
}

bool JKQTBasePlotter::isEmittingPlotSignalsEnabled() const
{
    return this->emitPlotSignals;
}

int JKQTBasePlotter::getPlotBorderTop() const {
    return this->plotterStyle.plotBorderTop;
}

int JKQTBasePlotter::getPlotBorderLeft() const {
    return this->plotterStyle.plotBorderLeft;
}

int JKQTBasePlotter::getPlotBorderBottom() const {
    return this->plotterStyle.plotBorderBottom;
}

int JKQTBasePlotter::getPlotBorderRight() const {
    return this->plotterStyle.plotBorderRight;
}

bool JKQTBasePlotter::doesMaintainAspectRatio() const {
    return this->maintainAspectRatio;
}

void JKQTBasePlotter::setAspectRatio(double __value)
{
    if (jkqtp_approximatelyUnequal(this->aspectRatio , __value)) {
        this->aspectRatio = __value;
        setXY(getXMin(), getXMax(), getYMin(), getYMax());
        redrawPlot();
    }
}

double JKQTBasePlotter::getAspectRatio() const
{
    return this->aspectRatio;
}

bool JKQTBasePlotter::doesMaintainAxisAspectRatio() const {
    return this->maintainAxisAspectRatio;
}

double JKQTBasePlotter::getAxisAspectRatio() const
{
    return this->axisAspectRatio;
}

bool JKQTBasePlotter::isUsingAntiAliasingForSystem() const
{
    return this->plotterStyle.useAntiAliasingForSystem;
}

bool JKQTBasePlotter::isUsingAntiAliasingForGraphs() const
{
    return this->plotterStyle.graphsStyle.useAntiAliasingForGraphs;
}

bool JKQTBasePlotter::isUsingAntiAliasingForText() const
{
    return this->plotterStyle.useAntiAliasingForText;
}


void JKQTBasePlotter::setBackgroundColor(const QColor &__value)
{
    if (this->plotterStyle.widgetBackgroundBrush != QBrush(__value)) {
        this->plotterStyle.widgetBackgroundBrush=QBrush(__value);
        redrawPlot();
    }
}

QColor JKQTBasePlotter::getBackgroundColor() const
{
    return this->plotterStyle.widgetBackgroundBrush.color();
}

void JKQTBasePlotter::setExportBackgroundColor(const QColor &__value)
{
    if (this->plotterStyle.exportBackgroundBrush != QBrush(__value)) {
        this->plotterStyle.exportBackgroundBrush=QBrush(__value);
        redrawPlot();
    }
}

QColor JKQTBasePlotter::getExportBackgroundColor() const
{
    return this->plotterStyle.exportBackgroundBrush.color();
}

QColor JKQTBasePlotter::getPlotBackgroundColor() const
{
    return this->plotterStyle.plotBackgroundBrush.color();
}

QBrush JKQTBasePlotter::getBackgroundBrush() const
{
    return this->plotterStyle.widgetBackgroundBrush;
}

QBrush JKQTBasePlotter::getExportBackgroundBrush() const
{
    return this->plotterStyle.exportBackgroundBrush;
}

QBrush JKQTBasePlotter::getPlotBackgroundBrush() const
{
    return this->plotterStyle.plotBackgroundBrush;
}



void JKQTBasePlotter::setShowKey(bool __value)
{
    mainKey->setVisible(__value);
}

bool JKQTBasePlotter::getShowKey() const
{
    return mainKey->getVisible();
}

void JKQTBasePlotter::setKeyPosition(const JKQTPKeyPosition &__value)
{
    mainKey->setPosition(__value);
}

JKQTPKeyPosition JKQTBasePlotter::getKeyPosition() const
{
    return mainKey->getPosition();
}

QColor JKQTBasePlotter::getDefaultTextColor() const
{
    return this->plotterStyle.defaultTextColor;
}

double JKQTBasePlotter::getDefaultTextSize() const
{
    return this->plotterStyle.defaultFontSize;
}

QString JKQTBasePlotter::getDefaultTextFontName() const
{
    return this->plotterStyle.defaultFontName;
}

void JKQTBasePlotter::setDefaultTextColor(QColor __value)
{
    if (this->plotterStyle.defaultTextColor != __value) {
        this->plotterStyle.defaultTextColor = __value;
        redrawPlot();
    }
}

void JKQTBasePlotter::setDefaultTextSize(double __value)
{
    if (jkqtp_approximatelyUnequal(this->plotterStyle.defaultFontSize , __value)) {
        this->plotterStyle.defaultFontSize = __value;
        redrawPlot();
    }
}

void JKQTBasePlotter::setDefaultTextFontName(const QString &__value)
{
    if (this->plotterStyle.defaultFontName != __value) {
        this->plotterStyle.defaultFontName = __value;
        redrawPlot();
    }
}

void JKQTBasePlotter::setPlotLabelFontSize(double __value)
{
    if (jkqtp_approximatelyUnequal(this->plotterStyle.plotLabelFontSize , __value)) {
        this->plotterStyle.plotLabelFontSize = __value;
        redrawPlot();
    }
}

double JKQTBasePlotter::getPlotLabelFontSize() const
{
    return this->plotterStyle.plotLabelFontSize;
}

void JKQTBasePlotter::setPlotLabelOffset(double __value)
{
    if (jkqtp_approximatelyUnequal(this->plotterStyle.plotLabelOffset , __value)) {
        this->plotterStyle.plotLabelOffset = __value;
        redrawPlot();
    }
}

double JKQTBasePlotter::getPlotLabelOffset() const
{
    return this->plotterStyle.plotLabelOffset;
}

void JKQTBasePlotter::setPlotLabelTopBorder(double __value)
{
    if (jkqtp_approximatelyUnequal(this->plotterStyle.plotLabelTopBorder , __value)) {
        this->plotterStyle.plotLabelTopBorder = __value;
        redrawPlot();
    }
}

double JKQTBasePlotter::getPlotLabelTopBorder() const
{
    return this->plotterStyle.plotLabelTopBorder;
}

void JKQTBasePlotter::setPlotLabelColor(QColor __value)
{
    if (this->plotterStyle.plotLabelColor != __value) {
        this->plotterStyle.plotLabelColor = __value;
        redrawPlot();
    }
}

QColor JKQTBasePlotter::getPlotLabelColor() const
{
    return this->plotterStyle.plotLabelColor;
}

void JKQTBasePlotter::setplotLabelFontName(const QString &__value)
{
    if (this->plotterStyle.plotLabelFontName != __value) {
        this->plotterStyle.plotLabelFontName = __value;
        redrawPlot();
    }
}

QString JKQTBasePlotter::getplotLabelFontName() const
{
    return this->plotterStyle.plotLabelFontName;
}

void JKQTBasePlotter::setPlotLabel(const QString &__value)
{
    if (this->plotLabel != __value) {
        this->plotLabel = __value;
        redrawPlot();
    }
}

QString JKQTBasePlotter::getPlotLabel() const
{
    return this->plotLabel;
}


void JKQTBasePlotter::setPlotBackgroundColor(const QColor &__value)
{
    if (this->plotterStyle.plotBackgroundBrush != QBrush(__value)) {
        this->plotterStyle.plotBackgroundBrush=QBrush(__value);
        redrawPlot();
    }
}

void JKQTBasePlotter::setBackgroundBrush(const QBrush &__value)
{
    if (this->plotterStyle.widgetBackgroundBrush != (__value)) {
        this->plotterStyle.widgetBackgroundBrush=(__value);
        redrawPlot();
    }
}

void JKQTBasePlotter::setExportBackgroundBrush(const QBrush &__value)
{
    if (this->plotterStyle.exportBackgroundBrush != (__value)) {
        this->plotterStyle.exportBackgroundBrush=(__value);
        redrawPlot();
    }
}

void JKQTBasePlotter::setPlotBackgroundBrush(const QBrush &__value)
{
    if (this->plotterStyle.plotBackgroundBrush != (__value)) {
        this->plotterStyle.plotBackgroundBrush=(__value);
        redrawPlot();
    }
}

void JKQTBasePlotter::setBackgroundGradient(const QGradient &__value)
{
    if (this->plotterStyle.widgetBackgroundBrush != QBrush(__value)) {
        this->plotterStyle.widgetBackgroundBrush=QBrush(__value);
        redrawPlot();
    }
}

void JKQTBasePlotter::setExportBackgroundGradient(const QGradient &__value)
{
    if (this->plotterStyle.exportBackgroundBrush != QBrush(__value)) {
        this->plotterStyle.exportBackgroundBrush=QBrush(__value);
        redrawPlot();
    }
}

void JKQTBasePlotter::setPlotBackgroundGradient(const QGradient &__value)
{
    if (this->plotterStyle.plotBackgroundBrush != QBrush(__value)) {
        this->plotterStyle.plotBackgroundBrush=QBrush(__value);
        redrawPlot();
    }

}

void JKQTBasePlotter::setBackgroundTexture(const QPixmap &__value)
{
    if (this->plotterStyle.widgetBackgroundBrush != QBrush(__value)) {
        this->plotterStyle.widgetBackgroundBrush=QBrush(__value);
        redrawPlot();
    }
}

void JKQTBasePlotter::setExportBackgroundTexture(const QPixmap &__value)
{
    if (this->plotterStyle.exportBackgroundBrush != QBrush(__value)) {
        this->plotterStyle.exportBackgroundBrush=QBrush(__value);
        redrawPlot();
    }
}

void JKQTBasePlotter::setPlotBackgroundTexture(const QPixmap &__value)
{
    if (this->plotterStyle.plotBackgroundBrush != QBrush(__value)) {
        this->plotterStyle.plotBackgroundBrush=QBrush(__value);
        redrawPlot();
    }
}

void JKQTBasePlotter::setBackgroundTexture(const QImage &__value)
{
    if (this->plotterStyle.widgetBackgroundBrush != QBrush(__value)) {
        this->plotterStyle.widgetBackgroundBrush=QBrush(__value);
        redrawPlot();
    }
}

void JKQTBasePlotter::setExportBackgroundTexture(const QImage &__value)
{
    if (this->plotterStyle.exportBackgroundBrush != QBrush(__value)) {
        this->plotterStyle.exportBackgroundBrush=QBrush(__value);
        redrawPlot();
    }
}

void JKQTBasePlotter::setPlotBackgroundTexture(const QImage &__value)
{
    if (this->plotterStyle.plotBackgroundBrush != QBrush(__value)) {
        this->plotterStyle.plotBackgroundBrush=QBrush(__value);
        redrawPlot();
    }
}

bool JKQTBasePlotter::isPlotFrameVisible() const
{
    return plotterStyle.plotFrameVisible;
}

QColor JKQTBasePlotter::getPlotFrameColor() const
{
    return plotterStyle.plotFrameColor;
}

double JKQTBasePlotter::getPlotFrameWidth() const
{
    return plotterStyle.plotFrameWidth;
}

double JKQTBasePlotter::getPlotFrameRounding() const
{
    return plotterStyle.plotFrameRounding;
}

void JKQTBasePlotter::setPlotFrameWidth(double __value)
{
    if (jkqtp_approximatelyUnequal(this->plotterStyle.plotFrameWidth , __value)) {
        this->plotterStyle.plotFrameWidth = __value;
        redrawPlot();
    }
}

void JKQTBasePlotter::setPlotFrameRounding(double __value)
{
    if (jkqtp_approximatelyUnequal(this->plotterStyle.plotFrameRounding , __value)) {
        this->plotterStyle.plotFrameRounding = __value;
        redrawPlot();
    }
}

void JKQTBasePlotter::setPlotFrameColor(QColor __value)
{
    if (this->plotterStyle.plotFrameColor != __value) {
        this->plotterStyle.plotFrameColor = __value;
        redrawPlot();
    }
}

void JKQTBasePlotter::setPlotFrameVisible(bool __value)
{
    if (this->plotterStyle.plotFrameVisible != __value) {
        this->plotterStyle.plotFrameVisible = __value;
        redrawPlot();
    }
}

void JKQTBasePlotter::setUseAntiAliasingForText(bool __value)
{
    if (this->plotterStyle.useAntiAliasingForText != __value) {
        this->plotterStyle.useAntiAliasingForText = __value;
        redrawPlot();
    }
}

void JKQTBasePlotter::setUseAntiAliasingForGraphs(bool __value)
{
    if (this->plotterStyle.graphsStyle.useAntiAliasingForGraphs != __value) {
        this->plotterStyle.graphsStyle.useAntiAliasingForGraphs = __value;
        redrawPlot();
    }
}

void JKQTBasePlotter::setUseAntiAliasingForSystem(bool __value)
{
    if (this->plotterStyle.useAntiAliasingForSystem != __value) {
        this->plotterStyle.useAntiAliasingForSystem = __value;
        redrawPlot();
    }
}

void JKQTBasePlotter::setAxisAspectRatio(double __value)
{
    if (jkqtp_approximatelyUnequal(this->axisAspectRatio , __value)) {
        this->axisAspectRatio = __value;
        redrawPlot();
    }
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

void JKQTBasePlotter::setCurrentSaveDirectory(const QString &__value)
{
    this->currentSaveDirectory = __value;
}

QString JKQTBasePlotter::getCurrentSaveDirectory() const
{
    return this->currentSaveDirectory;
}

void JKQTBasePlotter::setCurrentFileFormat(const QString &__value)
{
    this->currentFileFormat = __value;
}

QString JKQTBasePlotter::getCurrentFileFormat() const
{
    return this->currentFileFormat;
}

void JKQTBasePlotter::setCSVdecimalSeparator(const QString &__value)
{
    plotterStyle.CSVdecimalSeparator = __value;
}

void JKQTBasePlotter::setCSVcommentInitializer(const QString &__value)
{
    plotterStyle.CSVcommentInitializer = __value;
}

void JKQTBasePlotter::enableDebugShowRegionBoxes(bool enabled)
{
    if (plotterStyle.debugShowRegionBoxes != enabled) {
        plotterStyle.debugShowRegionBoxes = enabled;
        redrawPlot();
    }
}

void JKQTBasePlotter::enableDebugShowTextBoxes(bool enabled)
{
    if (plotterStyle.debugShowTextBoxes != enabled) {
        plotterStyle.debugShowTextBoxes = enabled;
        redrawPlot();
    }
}

bool JKQTBasePlotter::isDebugShowRegionBoxesEnabled() const
{
    return plotterStyle.debugShowRegionBoxes;
}

bool JKQTBasePlotter::isDebugShowTextBoxesEnabled() const
{
    return plotterStyle.debugShowTextBoxes;
}

QString JKQTBasePlotter::getCSVcommentInitializer() const
{
    return plotterStyle.CSVcommentInitializer;
}

JKQTMathText *JKQTBasePlotter::getMathText() {
    return &mathText;
}

const JKQTMathText *JKQTBasePlotter::getMathText() const {
    return &mathText;
}

JKQTPHorizontalAxisBase *JKQTBasePlotter::getXAxis(JKQTPCoordinateAxisRef axis) {
    if (axis==JKQTPPrimaryAxis) return xAxis;
    else return secondaryXAxis[axis];
}

JKQTPVerticalAxisBase *JKQTBasePlotter::getYAxis(JKQTPCoordinateAxisRef axis) {
    if (axis==JKQTPPrimaryAxis) return yAxis;
    else return secondaryYAxis[axis];
}

const JKQTPHorizontalAxisBase *JKQTBasePlotter::getXAxis(JKQTPCoordinateAxisRef axis) const {
    if (axis==JKQTPPrimaryAxis) return xAxis;
    else return secondaryXAxis[axis];
}

const JKQTPVerticalAxisBase *JKQTBasePlotter::getYAxis(JKQTPCoordinateAxisRef axis) const {
    if (axis==JKQTPPrimaryAxis) return yAxis;
    else return secondaryYAxis[axis];
}

bool JKQTBasePlotter::hasXAxis(JKQTPCoordinateAxisRef axis) const
{
    return secondaryXAxis.contains(axis);
}

bool JKQTBasePlotter::hasYAxis(JKQTPCoordinateAxisRef axis) const
{
    return secondaryYAxis.contains(axis);
}

QSet<JKQTPCoordinateAxisRef> JKQTBasePlotter::getAvailableXAxisRefs(bool includePrimary) const
{
    QSet<JKQTPCoordinateAxisRef> res;
    for (auto it=secondaryXAxis.begin(); it!=secondaryXAxis.end(); ++it) {
        res.insert(it.key());
    }
    if (includePrimary) res.insert(JKQTPPrimaryAxis);
        return res;
}

QSet<JKQTPCoordinateAxisRef> JKQTBasePlotter::getAvailableYAxisRefs(bool includePrimary) const
{
    QSet<JKQTPCoordinateAxisRef> res;
    for (auto it=secondaryYAxis.begin(); it!=secondaryYAxis.end(); ++it) {
        res.insert(it.key());
    }
    if (includePrimary) res.insert(JKQTPPrimaryAxis);
    return res;
}

QMap<JKQTPCoordinateAxisRef, JKQTPHorizontalAxisBase *> JKQTBasePlotter::getXAxes(bool includePrimary)
{
    QMap<JKQTPCoordinateAxisRef, JKQTPHorizontalAxisBase *> res=secondaryXAxis;
    if (includePrimary) res[JKQTPPrimaryAxis]=xAxis;
    return res;
}

QMap<JKQTPCoordinateAxisRef, JKQTPVerticalAxisBase *> JKQTBasePlotter::getYAxes(bool includePrimary)
{
    QMap<JKQTPCoordinateAxisRef, JKQTPVerticalAxisBase *> res=secondaryYAxis;
    if (includePrimary) res[JKQTPPrimaryAxis]=yAxis;
    return res;
}

QList<JKQTPCoordinateAxis *> JKQTBasePlotter::getAxes(bool includePrimaries)
{
    QList<JKQTPCoordinateAxis *> res;
    if (includePrimaries) res<<xAxis<<yAxis;
    for (auto& ax: secondaryXAxis) res<<ax;
    for (auto& ax: secondaryYAxis) res<<ax;
    return res;
}

QList<const JKQTPCoordinateAxis *> JKQTBasePlotter::getAxes(bool includePrimaries) const
{
    QList<const JKQTPCoordinateAxis *> res;
    if (includePrimaries) res<<xAxis<<yAxis;
    for (auto& ax: secondaryXAxis) res<<ax;
    for (auto& ax: secondaryYAxis) res<<ax;
    return res;
}

JKQTPCoordinateAxisRef JKQTBasePlotter::addSecondaryXAxis(JKQTPHorizontalAxisBase *axis)
{
    const auto keys=secondaryXAxis.keys();
    JKQTPCoordinateAxisRef ref=JKQTPSecondaryAxis;
    if (keys.size()>0) {
        ref=static_cast<JKQTPCoordinateAxisRef>(static_cast<int>(*std::max_element(keys.begin(), keys.end()))+1);
    }
    secondaryXAxis[ref]=axis;
    axis->setParent(this);
    return ref;
}

JKQTPCoordinateAxisRef JKQTBasePlotter::addSecondaryYAxis(JKQTPVerticalAxisBase *axis)
{
    const auto keys=secondaryYAxis.keys();
    JKQTPCoordinateAxisRef ref=JKQTPSecondaryAxis;
    if (keys.size()>0) {
        ref=static_cast<JKQTPCoordinateAxisRef>(static_cast<int>(*std::max_element(keys.begin(), keys.end()))+1);
    }
    secondaryYAxis[ref]=axis;
    axis->setParent(this);
    return ref;
}

QAction *JKQTBasePlotter::getActionSavePlot() const {
    return this->actSavePlot;
}

QAction *JKQTBasePlotter::getActionSaveData() const {
    return this->actSaveData;
}

QAction *JKQTBasePlotter::getActionCopyData() const {
    return this->actCopyData;
}

QAction *JKQTBasePlotter::getActionCopyPixelImage() const {
    return this->actCopyPixelImage;
}

QAction *JKQTBasePlotter::getActionCopyMatlab() const {
    return this->actCopyMatlab;
}

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
QAction *JKQTBasePlotter::getActionSaveSVG() const {
    return this->actSaveSVG;
}

QAction *JKQTBasePlotter::getActionPrint() const {
    return this->actPrint;
}

QAction *JKQTBasePlotter::getActionSavePDF() const {
    return this->actSavePDF;
}
#endif

QAction *JKQTBasePlotter::getActionSavePix() const {
    return this->actSavePix;
}

QAction *JKQTBasePlotter::getActionSaveCSV() const {
    return this->actSaveCSV;
}

QAction *JKQTBasePlotter::getActionZoomAll() const {
    return this->actZoomAll;
}

QAction *JKQTBasePlotter::getActionZoomIn() const {
    return this->actZoomIn;
}

QAction *JKQTBasePlotter::getActionZoomOut() const {
    return this->actZoomOut;
}

QAction *JKQTBasePlotter::getActionShowPlotData() const {
    return this->actShowPlotData;
}

JKQTBasePlotter::AdditionalActionsMap JKQTBasePlotter::getLstAdditionalPlotterActions() const {
    return this->lstAdditionalPlotterActions;
}

QString JKQTBasePlotter::getCSVdecimalSeparator() const
{
    return plotterStyle.CSVdecimalSeparator;
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

bool JKQTBasePlotter::getMasterSynchronizeWidth() const {
    return this->masterSynchronizeWidth;
}

bool JKQTBasePlotter::getMasterSynchronizeHeight() const {
    return this->masterSynchronizeHeight;
}


void JKQTBasePlotter::setFontSizePrintMultiplier(double __value)
{
    this->fontSizePrintMultiplier = __value;
}

double JKQTBasePlotter::getFontSizePrintMultiplier() const
{
    return this->fontSizePrintMultiplier;
}

void JKQTBasePlotter::setLineWidthPrintMultiplier(double __value)
{
    this->lineWidthPrintMultiplier = __value;
}

double JKQTBasePlotter::getLineWidthPrintMultiplier() const
{
    return this->lineWidthPrintMultiplier;
}

double JKQTBasePlotter::getFontSizeMultiplier() const {
    return this->fontSizeMultiplier;
}

double JKQTBasePlotter::getLineWidthMultiplier() const {
    return this->lineWidthMultiplier;
}

const JKQTPKeyStyle &JKQTBasePlotter::getMainKeyStyle() const
{
    return plotterStyle.keyStyle;
}

JKQTPKeyStyle &JKQTBasePlotter::getMainKeyStyle()
{
    return plotterStyle.keyStyle;
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
        const auto dp=loc.decimalPoint();
        const QString sep="\t";
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

bool JKQTBasePlotter::saveData(const QString& filename, const QString &format) {
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

    QMap<QString, QStringList> saveAdapterFileExtensions;
    {
        JKQTPSynchronized<QList<JKQTPSaveDataAdapter*>>::ReadLocker lock(jkqtpSaveDataAdapters);
        for (int i=0; i<jkqtpSaveDataAdapters.get().size(); i++) {
            const QString fid=jkqtpSaveDataAdapters.get()[i]->getFormatID();
            fileformats<<jkqtpSaveDataAdapters.get()[i]->getFilter();
            fileformatIDs<<fid;
            saveAdapterFileExtensions[fid]=jkqtpSaveDataAdapters.get()[i]->getFileExtension();
        }
    }



    QString fn=filename;
    QString fmt=format.toLower().trimmed();
    if (fmt=="sylk") fmt="slk";
    else if (fmt=="ssv") fmt="sem";

    if (fmt.isEmpty()) {
        const QString e=QFileInfo(filename).suffix().toLower();
        fmt=e;
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
        } else if (e=="ssv") {
            fmt="sem";
        }
        if (!fileformatIDs.contains(fmt)) {
            for (auto it= saveAdapterFileExtensions.begin(); it!=saveAdapterFileExtensions.end(); ++it) {
                if (it.value().contains(e)) {
                    fmt=it.key();
                    break;
                }
            }
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
        const int filtIdx=fileformats.indexOf(selectedFilter);
        if (filtIdx>=0) fmt=fileformatIDs[filtIdx] ;
    }

    saveUserSettings();
    if (!fn.isEmpty()) {

        if (fmt=="csv") {
            saveAsCSV(fn);
            return true;
        } else if (fmt=="tab") {
            saveAsTabSV(fn);
            return true;
        } else if (fmt=="gex") {
            saveAsGerExcelCSV(fn);
            return true;
        } else if (fmt=="sem") {
            saveAsSemicolonSV(fn);
            return true;
        } else if (fmt=="slk") {
            saveAsSYLK(fn);
            return true;
        } else if (fmt=="dif") {
            saveAsDIF(fn);
            return true;
        } else if (fmt=="m") {
            saveAsMatlab(fn);
            return true;
        } else if (fmt.startsWith("custom")) { // for backward compatibility!
            QString fidx=fmt;
            fidx=fidx.remove(0,6);
            int idx=fidx.toInt();
            JKQTPSynchronized<QList<JKQTPSaveDataAdapter*>>::ReadLocker lock(jkqtpSaveDataAdapters);
            if (idx>=0 && idx<jkqtpSaveDataAdapters.get().size() && jkqtpSaveDataAdapters.get()[idx]) {
                QStringList columnNames;
                const QList<QVector<double> > dataset=datastore->getData(&columnNames);
                jkqtpSaveDataAdapters.get()[idx]->saveJKQTPData(fn, dataset, columnNames);
                return true;
            }
        } else {
            JKQTPSynchronized<QList<JKQTPSaveDataAdapter*>>::ReadLocker lock(jkqtpSaveDataAdapters);
            for (int i=0; i<jkqtpSaveDataAdapters.get().size(); i++) {
                if (fmt == jkqtpSaveDataAdapters.get()[i]->getFormatID()) {
                    QStringList columnNames;
                    const QList<QVector<double> > dataset=datastore->getData(&columnNames);
                    jkqtpSaveDataAdapters.get()[i]->saveJKQTPData(fn, dataset, columnNames);
                    return true;
                }
            }
        }
    }
    return false;
}

void JKQTBasePlotter::saveAsCSV(const QString& filename) {
    saveAsCSV(filename, plotterStyle.CSVdecimalSeparator, plotterStyle.CSVcommentInitializer);
}

void JKQTBasePlotter::saveAsCSV(const QString &filename, const QString &decimalSeparator, const QString &commentInitializer)
{
    loadUserSettings();
    QString fn=filename;
    if (fn.isEmpty()) {
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot Data"),
                                    currentSaveDirectory,
                                    tr("Comma Separated Values (*.csv *.dat)"));
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    if (!fn.isEmpty()) {
        datastore->saveCSV(fn, getDataColumnsByUser(), ", ", decimalSeparator, commentInitializer);
    }
    saveUserSettings();

}
void JKQTBasePlotter::saveAsSemicolonSV(const QString& filename) {
    saveAsSemicolonSV(filename, plotterStyle.CSVdecimalSeparator, plotterStyle.CSVcommentInitializer);
}

void JKQTBasePlotter::saveAsSemicolonSV(const QString& filename, const QString &decimalSeparator, const QString &commentInitializer) {
    loadUserSettings();
    QString fn=filename;
    if (fn.isEmpty()) {
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot Data"),
                                    currentSaveDirectory,
                                    tr("Comma Separated Values (*.csv *.dat)"));
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    if (!fn.isEmpty()) {
        datastore->saveCSV(fn, getDataColumnsByUser(), ";", decimalSeparator, commentInitializer);
    }
    saveUserSettings();
}

void JKQTBasePlotter::saveAsTabSV(const QString& filename) {
    saveAsTabSV(filename, plotterStyle.CSVdecimalSeparator, plotterStyle.CSVcommentInitializer);
}

void JKQTBasePlotter::saveAsTabSV(const QString& filename, const QString &decimalSeparator, const QString &commentInitializer) {
    loadUserSettings();
    QString fn=filename;
    if (fn.isEmpty()) {
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot Data"),
                                    currentSaveDirectory,
                                    tr("Tabulator Separated Values (*.txt)"));
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    if (!fn.isEmpty()) {
        datastore->saveCSV(fn, getDataColumnsByUser(), "\t", decimalSeparator, commentInitializer);
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

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
bool JKQTBasePlotter::saveAsPDF(const QString& filename, bool displayPreview) {
    loadUserSettings();
    QString fn=filename;
    if (fn.isEmpty()) {
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot"),
                                    currentSaveDirectory,
                                    tr("PDF File (*.pdf)"));
        if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
    }

    if (!fn.isEmpty()) {
        emit beforeExporting();; auto __finalpaint=JKQTPFinally([&]() { emit afterExporting();});
        std::shared_ptr<QPrinter> printer=std::make_shared<QPrinter>();
        bool doLandscape=widgetWidth>widgetHeight;
        if (gridPrinting) {
            gridPrintingCalc();
            doLandscape=gridPrintingSize.width()>gridPrintingSize.height();
        }
        if (doLandscape) {
            printer->setPageOrientation(QPageLayout::Landscape);
        } else {
            printer->setPageOrientation(QPageLayout::Portrait);
        }
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setColorMode(QPrinter::Color);
        printer->setOutputFileName(fn);
        printer->setPageMargins(QMarginsF(0,0,0,0),QPageLayout::Millimeter);
        printer->setColorMode(QPrinter::Color);
        return printpreviewNew(printer.get(), true, -1.0, -1.0, displayPreview);
    }
    saveUserSettings();
    return false;
}
#endif

bool JKQTBasePlotter::saveImage(const QString& filename, bool displayPreview) {
    loadUserSettings();
    QString fn=filename;
    QStringList filterstrings;
    QList<QStringList> filterextensions;

    const auto findExporterByExtension=[&filterextensions](const QString& ext) {
        const QString extl=ext.toLower();
        for (int i=0; i<filterextensions.size(); i++) {
            if (filterextensions[i].contains(extl)) return i;
        }
        return -1;
    };

    // add default exporters
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    filterstrings<<tr("Portable Document Format PDF [Qt] (*.pdf)");    filterextensions<<(QStringList()<<"pdf");
    const int idxDefaultPDF=filterstrings.size()-1;
    filterstrings<<tr("Scalable Vector Graphics [Qt] (*.svg)");    filterextensions<<(QStringList()<<"svg");
    const int idxDefaultSVG=filterstrings.size()-1;
#endif
    filterstrings<<tr("PNG Image [Qt] (*.png)");    filterextensions<<(QStringList()<<"png");
    filterstrings<<tr("BMP Image [Qt] (*.bmp)");    filterextensions<<(QStringList()<<"bmp");
    filterstrings<<tr("TIFF Image [Qt] (*.tif *.tiff)");    filterextensions<<(QStringList()<<"tif"<<"tiff");
    filterstrings<<tr("JPEG Image [Qt] (*.jpg *.jpeg)");    filterextensions<<(QStringList()<<"jpg"<<"jpeg");
    //  add JKQTPPaintDeviceAdapter exporters
    const int filtersIndexFirstExporterPLugin=filterstrings.size();
    {
        JKQTPSynchronized<QList<JKQTPPaintDeviceAdapter*>>::ReadLocker lock(jkqtpPaintDeviceAdapters);
        for (int i=0; i<jkqtpPaintDeviceAdapters.get().size(); i++) {
            filterstrings<<jkqtpPaintDeviceAdapters.get()[i]->getFilter();
            filterextensions<<QStringList();
            for (const auto& ext: jkqtpPaintDeviceAdapters.get()[i]->getFileExtension()) filterextensions.last()<<ext.toLower();
        }
    }
    //const bool isWithSpecialDeviceAdapter=(filterstrings.size()>filtersIndexFirstExporterPLugin);
    // add remaining QImageWriter exporters
    const int filtersIndexFirstQtWriter=filterstrings.size();
    const QList<QByteArray> writerformats=QImageWriter::supportedImageFormats();
    for (int i=0; i<writerformats.size(); i++) {
        const QString ext=writerformats[i].toLower();
        const int extIdx=findExporterByExtension(ext);
        const QString name=writerformats[i].toUpper();
        // only add QtWriters that are not yt contained in the default filters options
        if (extIdx<0 || extIdx>=filtersIndexFirstExporterPLugin) {
            filterstrings<<QString("%1 Image (*.%2)").arg(name).arg(ext);
            filterextensions<<(QStringList()<<ext);
        }
    }

    QString selFormat;
    if (fn.isEmpty()) {
        selFormat=currentFileFormat;
        fn = QFileDialog::getSaveFileName(nullptr, tr("Save Plot"),
                                    currentSaveDirectory,
                                          filterstrings.join(";;"), &selFormat);
        if (!fn.isEmpty()) {
            currentSaveDirectory=QFileInfo(fn).absolutePath();
            currentFileFormat=selFormat;
        }
    } else {
        const QString fnExt=QFileInfo(filename).suffix().toLower();
        const int filtidx=findExporterByExtension(fnExt);
        if (filtidx>=0) selFormat=filterstrings[filtidx];

    }

    //qDebug()<<"fn="<<fn<<"  selFormat="<<selFormat;

    saveUserSettings();
    if (!fn.isEmpty()) {
        const QString fnExt=QFileInfo(filename).suffix().toLower();

        const int filtID=[&](){
            // 1. look for the selected format
            int idx=filterstrings.indexOf(selFormat);
            // 2. if not found try to match up the file extension
            if (idx<0) idx=findExporterByExtension(fnExt);
            return idx;
        }();
        JKQTPSynchronized<QList<JKQTPPaintDeviceAdapter*>>::ReadLocker lock(jkqtpPaintDeviceAdapters);
        // now we determine whether we selected a jkqtpPaintDeviceAdapters, if not adapterID will be <0
        const int adapterID=[&](){
            int idx=filtID-filtersIndexFirstExporterPLugin;
            if (idx<0) idx=-1;
            if (idx>=jkqtpPaintDeviceAdapters.get().size()) idx=-1;
            return idx;
        }();


        if (filtID<0) {
            qWarning()<<"You tried to save an image to to '"<<fn<<"', but JKQTPlottter did not recognize the file format!";
            return false;
        }


#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        // SVG and PDF need to be treated separately!
        if (filtID==idxDefaultPDF) {
            return saveAsPDF(fn, displayPreview);
        }
        if (filtID==idxDefaultSVG) {
            return saveAsSVG(fn, displayPreview);
        }
#endif

        // we need to use a jkqtpPaintDeviceAdapters
        if (adapterID>=0) {
            QString tempFM="";
            if (QFile::exists(fn)) {
                // if the file fn already exists, we make a temporary copy, so it is not destroyed by the export process!
#ifdef QFWIDLIB_LIBRARY
                QSharedPointer<QFTemporaryFile> tf=QSharedPointer<QTemporaryFile>(new QFTemporaryFile());
#else
                QSharedPointer<QTemporaryFile> tf=QSharedPointer<QTemporaryFile>(new QTemporaryFile());
#endif
                tf->open();
                tempFM=tf->fileName();
                tf->close();
                tf.reset();
                QFile::copy(fn, tempFM);
            }


            emit beforeExporting();; auto __finalpaint=JKQTPFinally([&]() { emit afterExporting();});

            gridPrintingCalc();
            QSharedPointer<QPaintDevice> paintDevice=QSharedPointer<QPaintDevice>(jkqtpPaintDeviceAdapters.get()[adapterID]->createPaintdevice(fn, jkqtp_roundTo<int>(gridPrintingSize.width()), jkqtp_roundTo<int>(gridPrintingSize.height())));

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
            if (!printpreviewNew(paintDevice.get(), jkqtpPaintDeviceAdapters.get()[adapterID]->getSetAbsolutePaperSize(), jkqtpPaintDeviceAdapters.get()[adapterID]->getPrintSizeXInMM(), jkqtpPaintDeviceAdapters.get()[adapterID]->getPrintSizeYInMM(), displayPreview)) {
                if (QFile::exists(tempFM)) {
                    QFile::copy(tempFM, fn);
                    QFile::remove(tempFM);
                }
                return false;
            } else
#endif
            {
                paintDevice.reset(jkqtpPaintDeviceAdapters.get()[adapterID]->createPaintdeviceMM(fn,printSizeX_Millimeter,printSizeY_Millimeter));
                printpreviewPaintRequestedNewPaintDevice(paintDevice.get());
                return true;
            }

        } else {
            // here we can let Qt figure out the correct exporter
            return saveAsPixelImage(fn, displayPreview, writerformats.value(filtID-filtersIndexFirstQtWriter, QByteArray()));
        }
    }
    return false;
}


bool JKQTBasePlotter::saveAsPixelImage(const QString& filename, bool displayPreview, const QByteArray& outputFormat, const QSize &outputSizeIncrease) {
    loadUserSettings();
    QString fn=filename;
    QStringList filt;
    QList<QByteArray> writerformats=QImageWriter::supportedImageFormats();
    for (int i=0; i<writerformats.size(); i++) {
        filt<<QString("%1 Image (*.%2)").arg(QString(writerformats[i]).toUpper()).arg(QString(writerformats[i].toLower()));
    }
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
        const int filtID=filt.indexOf(selFormat);
        //QString ext=tolower(extract_file_ext(fn.toStdString()));
        const QString form=[&]()->QString{
            if (filtID>=0 && filtID<writerformats.size()) {
                return writerformats[filtID];
            }
            if (outputFormat.size()>0) {
                return outputFormat;
            }
            return "NONE";
        }();


        emit beforeExporting();; auto __finalpaint=JKQTPFinally([&]() { emit afterExporting();});
        gridPrintingCalc();
        //std::cout<<gridPrintingSize.width()<<", "<<gridPrintingSize.height()<<std::endl;

        if (!displayPreview) {
            printSizeX_Millimeter=widgetWidth;
            printSizeY_Millimeter=widgetHeight;
        }

        if (!displayPreview || exportpreview(gridPrintingSize, false)) {
            QElapsedTimer timer;
            timer.start();
            QImage png(QSizeF(double(printSizeX_Millimeter)+outputSizeIncrease.width(), double(printSizeY_Millimeter)+outputSizeIncrease.height()).toSize(), QImage::Format_ARGB32);
            png.fill(Qt::transparent);
            const double durPrepImage=timer.nsecsElapsed();
            {
                JKQTPEnhancedPainter painter;
                painter.begin(&png);
                painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
                painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
                painter.setRenderHint(JKQTPEnhancedPainter::SmoothPixmapTransform);
    #if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
                painter.setRenderHint(JKQTPEnhancedPainter::NonCosmeticDefaultPen, true);
                painter.setRenderHint(JKQTPEnhancedPainter::HighQualityAntialiasing);
    #endif

                /*calcPlotScaling(painter);
                gridPaint(painter, png.rect().size());*/\
                //qDebug()<<QSize(printSizeX_Millimeter, printSizeY_Millimeter);
                exportpreviewPaintRequested(painter, QSize(jkqtp_roundTo<int>(printSizeX_Millimeter), jkqtp_roundTo<int>(printSizeY_Millimeter)));
                painter.end();
            }
            const double durPlot=timer.nsecsElapsed()-durPrepImage;
            bool res=false;
            if (form=="NONE") res= png.save(fn);
            else res= png.save(fn, form.toLatin1().data());
            const double durAll=timer.nsecsElapsed();
            const double durSave=durAll-durPlot;
            qDebug()<<"durPrepImage="<<durPrepImage/1e6<<"ms, durPlot="<<durPlot/1e6<<"ms, durSave="<<durSave/1e6<<"ms, durAll="<<durAll/1e6<<"ms";
            return res;
        }

    }
    return false;
}

QImage JKQTBasePlotter::grabPixelImage(QSize size, bool showPreview)
{
    gridPrintingCalc();
    //std::cout<<gridPrintingSize.width()<<", "<<gridPrintingSize.height()<<std::endl;
    printSizeX_Millimeter=gridPrintingSize.width();
    printSizeY_Millimeter=gridPrintingSize.height();
    if (!showPreview && !gridPrinting) {
        printSizeX_Millimeter=widgetWidth;
        printSizeY_Millimeter=widgetHeight;
    }
    emit beforeExporting();; auto __finalpaint=JKQTPFinally([&]() { emit afterExporting();});
    if (!showPreview||exportpreview(gridPrintingSize, false)) {

        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        if (size.isEmpty() || size.isNull() || !size.isValid() || size.width()*size.height()==0) size=QSizeF(double(printSizeX_Millimeter), double(printSizeY_Millimeter)).toSize();
        QImage png(size, QImage::Format_ARGB32);
        {
            png.fill(Qt::transparent);
            JKQTPEnhancedPainter painter;
            painter.begin(&png);
            painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
            painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
            painter.setRenderHint(JKQTPEnhancedPainter::SmoothPixmapTransform);
    #if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
            painter.setRenderHint(JKQTPEnhancedPainter::NonCosmeticDefaultPen, true);
            painter.setRenderHint(JKQTPEnhancedPainter::HighQualityAntialiasing);
    #endif

            /*calcPlotScaling(painter);
            gridPaint(painter, png.rect().size());*/
            exportpreviewPaintRequested(painter, QSizeF(printSizeX_Millimeter, printSizeY_Millimeter).toSize());
            painter.end();
        }
        QApplication::restoreOverrideCursor();

        return png;
    }
    return QImage();

}


void JKQTBasePlotter::copyPixelImage(bool showPreview) {

    gridPrintingCalc();
    //std::cout<<gridPrintingSize.width()<<", "<<gridPrintingSize.height()<<std::endl;
    printSizeX_Millimeter=gridPrintingSize.width();
    printSizeY_Millimeter=gridPrintingSize.height();

    emit beforeExporting();; auto __finalpaint=JKQTPFinally([&]() { emit afterExporting();});
    if (!showPreview||exportpreview(gridPrintingSize, false)) {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        QByteArray svgdata;
            {

                QBuffer buffer(&svgdata);
                QSvgGenerator* svg=new QSvgGenerator;
                svg->setResolution(96);
                //svg->setResolution(300);
                QSize size=QSizeF(printSizeX_Millimeter, printSizeX_Millimeter).toSize();
                double factor=double(size.width())/double(widgetWidth)*paintMagnification;
                // TODO: CORRECT THIS
                //qDebug()<<size;
                svg->setSize(size);
                svg->setOutputDevice(&buffer);
                JKQTPEnhancedPainter painter;
                painter.setPainterFlag(JKQTPEnhancedPainter::VectorPainting);
                painter.begin(svg);
                painter.scale(factor,factor);
                printAspect=printSizeY_Millimeter/printSizeX_Millimeter;
                exportpreviewPaintRequested(painter, QSizeF(widgetWidth/paintMagnification, widgetWidth/paintMagnification*printAspect).toSize());
                painter.end();
                delete svg;

            }

        if (!showPreview) {
            printSizeX_Millimeter=widgetWidth;
            printSizeY_Millimeter=widgetHeight;
        }
        QImage png(QSizeF(double(printSizeX_Millimeter), double(printSizeY_Millimeter)).toSize(), QImage::Format_ARGB32);
            {
                png.fill(Qt::transparent);
                JKQTPEnhancedPainter painter;
                painter.begin(&png);
                painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
                painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
                painter.setRenderHint(JKQTPEnhancedPainter::SmoothPixmapTransform);
#if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
                painter.setRenderHint(JKQTPEnhancedPainter::NonCosmeticDefaultPen, true);
                painter.setRenderHint(JKQTPEnhancedPainter::HighQualityAntialiasing);
#endif

                /*calcPlotScaling(painter);
                gridPaint(painter, png.rect().size());*/
                exportpreviewPaintRequested(painter, QSizeF(printSizeX_Millimeter, printSizeY_Millimeter).toSize());
                painter.end();
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
    }


}

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
bool JKQTBasePlotter::saveAsSVG(const QString& filename, bool displayPreview) {
    bool printed=false;
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
                QSharedPointer<QFTemporaryFile> tf=QSharedPointer<QFTemporaryFile>(new QFTemporaryFile());
#else
                QSharedPointer<QTemporaryFile> tf=QSharedPointer<QTemporaryFile>(new QTemporaryFile());
#endif
            tf->open();
            tempFM=tf->fileName();
            tf->close();
            tf.reset();
            QFile::copy(fn, tempFM);
        }

        emit beforeExporting();; auto __finalpaint=JKQTPFinally([&]() { emit afterExporting();});
        gridPrintingCalc();
        std::shared_ptr<QSvgGenerator> svg=std::make_shared<QSvgGenerator>();
        svg->setResolution(96);
        QSize size=QSizeF(gridPrintingSize.width()*25.4/svg->resolution(), gridPrintingSize.height()*25.4/svg->resolution()).toSize();
        svg->setSize(size);
        svg->setFileName(fn);
        printed=printpreviewNew(svg.get(), true, -1.0, -1.0, displayPreview);
        if (!printed) {
            if (QFile::exists(tempFM)) {
                QFile::copy(tempFM, fn);
                QFile::remove(tempFM);
            }
        }


    }
    saveUserSettings();
    return printed;
}
#endif

void JKQTBasePlotter::setPlotBorder(int left, int right, int top, int bottom){
    plotterStyle.plotBorderTop=top;
    plotterStyle.plotBorderLeft=left;
    plotterStyle.plotBorderBottom=bottom;
    plotterStyle.plotBorderRight=right;
    //updateGeometry();
    redrawPlot();
}

void JKQTBasePlotter::setPlotBorderLeft(int left)
{
    plotterStyle.plotBorderLeft=left;
    redrawPlot();
}

void JKQTBasePlotter::setPlotBorderRight(int right)
{
    plotterStyle.plotBorderRight=right;
    redrawPlot();
}

void JKQTBasePlotter::setPlotBorderTop(int top)
{
    plotterStyle.plotBorderTop=top;
    redrawPlot();
}

void JKQTBasePlotter::setPlotBorderBottom(int bottom)
{
    plotterStyle.plotBorderBottom=bottom;
    redrawPlot();
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




void JKQTBasePlotter::drawGraphs(JKQTPEnhancedPainter& painter){
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTBasePlotter::plotGraphs"));
#endif
    if (datastore==nullptr || graphs.isEmpty()) return;
    //qDebug()<<"start JKQTBasePlotter::plotGraphs()";
    if (useClipping) {
        QRegion cregion(jkqtp_roundTo<int>(internalPlotBorderLeft), jkqtp_roundTo<int>(internalPlotBorderTop), jkqtp_roundTo<int>(internalPlotWidth), jkqtp_roundTo<int>(internalPlotHeight));
        painter.setClipping(true);
        painter.setClipRegion(cregion);
    }

    const QRectF rGOTop=calcPlotMarginRect(muGraphsOutside, sideTop);
    const QRectF rGOBottom=calcPlotMarginRect(muGraphsOutside, sideBottom);
    const QRectF rGOLeft=calcPlotMarginRect(muGraphsOutside, sideLeft);
    const QRectF rGORight=calcPlotMarginRect(muGraphsOutside, sideRight);
    double ibTop=rGOTop.bottom();
    double ibLeft=rGOLeft.right();
    double ibBottom=rGOBottom.top();
    double ibRight=rGORight.left();


    for (int j=0; j<graphs.size(); j++) {
        JKQTPPlotElement* g=graphs[j];
        if (g->isVisible()) g->draw(painter);
    }

    if (useClipping) {
        painter.setClipping(false);
    }

    for (int j=0; j<graphs.size(); j++) {
        JKQTPPlotElement* g=graphs[j];
        if (g->isVisible()) {
            int leftSpace=0, rightSpace=0, topSpace=0, bottomSpace=0;
            g->getOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
            ibTop-=topSpace;
            ibLeft-=leftSpace;
            const QRectF rleft(ibLeft, internalPlotBorderTop, leftSpace, internalPlotHeight);
            const QRectF rright(ibRight, internalPlotBorderTop, rightSpace, internalPlotHeight);
            const QRectF rtop(internalPlotBorderLeft, ibTop, internalPlotWidth, topSpace);
            const QRectF rbottom(internalPlotBorderLeft, ibBottom, internalPlotWidth, bottomSpace);
            g->drawOutside(painter, rleft.toRect(), rright.toRect(), rtop.toRect(), rbottom.toRect() );

            if (plotterStyle.debugShowRegionBoxes) {
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
                QPen p("green");
                p.setWidthF(plotterStyle.debugRegionLineWidth);
                QColor col=p.color(); col.setAlphaF(0.8f); p.setColor(col);
                painter.setPen(p);
                painter.setBrush(QBrush(QColor(Qt::transparent)));
                if (rleft.width()>0 && rleft.height()>0) painter.drawRect(rleft);
                if (rright.width()>0 && rright.height()>0) painter.drawRect(rright);
                if (rtop.width()>0 && rtop.height()>0) painter.drawRect(rtop);
                if (rbottom.width()>0 && rbottom.height()>0) painter.drawRect(rbottom);

            }

            ibRight+=rightSpace;
            ibBottom+=bottomSpace;
        }
    }

    //qDebug()<<"  end JKQTBasePlotter::plotGraphs()";
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



bool JKQTBasePlotter::getGraphsXMinMax(double& minx, double& maxx, double& smallestGreaterZero, const PlotElementPreciate& predicate) {
    bool start=true;
    minx=0;
    maxx=0;
    smallestGreaterZero=0;
    size_t count=0;
    for (int i=0; i<graphs.size(); i++) {
        if (graphs[i]->isVisible() && predicate(graphs[i])) {

            double gminx=0;
            double gmaxx=0;
            double gsmallestGreaterZero=0;
            if (graphs[i]->getXMinMax(gminx, gmaxx, gsmallestGreaterZero)) {
               // std::cout<<"      -> "<<i<<": "<<graphs[i]->getTitle().toStdString()<<": "<<gminx<<"..."<<gmaxx<<std::endl;
                if ((gminx<minx) || start) minx=gminx;
                if ((gmaxx>maxx) || start) maxx=gmaxx;

                double xvsgz;
                xvsgz=gsmallestGreaterZero; if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;
                xvsgz=gmaxx; if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;
                xvsgz=gminx; if ((xvsgz>10.0*DBL_MIN)&&((smallestGreaterZero<10.0*DBL_MIN) || (xvsgz<smallestGreaterZero))) smallestGreaterZero=xvsgz;

                //std::cout<<"         "<<i<<": gminx="<<gminx<<"  gmaxx="<<gmaxx<<"   minx="<<minx<<" maxx="<<maxx<<std::endl;

                start=false;
                count++;
            } else {
                //std::cout<<"      -> "<<i<<": "<<graphs[i]->getTitle().toStdString()<<": FALSE"<<std::endl;
            }
        } else {
            //std::cout<<"      -> "<<i<<": "<<graphs[i]->getTitle().toStdString()<<": INVISIBLE"<<std::endl;
        }
    }
    return count>0;
}

bool JKQTBasePlotter::getGraphsYMinMax(double& miny, double& maxy, double& smallestGreaterZero, const PlotElementPreciate &predicate) {
    bool start=true;
    miny=0;
    maxy=0;
    smallestGreaterZero=0;
    size_t count=0;
    for (int i=0; i<graphs.size(); i++) {
        if (graphs[i]->isVisible() && predicate(graphs[i])) {
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
                count++;
            }
        }
    }
    return count>0;
}


void JKQTBasePlotter::zoomToFit(bool zoomX, bool zoomY, bool includeX0, bool includeY0, double scaleX, double scaleY) {
   // std::cout<<"JKQTBasePlotter::zoomToFit():\n";
    if (graphs.size()<=0) return;
    auto calcLocScaling=[](JKQTPCoordinateAxis* axis, double &xxmin, double&xxmax, double&xsmallestGreaterZero, bool include0, double scale) -> bool {
        if (JKQTPIsOKFloat(xxmin) && JKQTPIsOKFloat(xxmax)) {
            bool doScale=true;
            if (fabs(xxmin-xxmax)<1e-305) {
                xxmin=axis->getMin();
                xxmax=axis->getMax();
                doScale=false;
            }
            if (axis->isLogAxis()) {
                if ((xxmin<=1e-305)&&(xxmax<=1e-305)) {xxmin=0.1; xxmax=1.0; }
                else if ((xxmin<=1e-305)&&(xxmax>0)) {
                    if (xsmallestGreaterZero>10.0*DBL_MIN) xxmin=xsmallestGreaterZero;
                    else xxmin=xxmax/axis->getLogAxisBase();
                }
                if (doScale) {
                    double d=scale*(log(xxmax)-log(xxmin));  // new width
                    double c=(log(xxmax)+log(xxmin))/2.0;     // center of interval
                    xxmin=exp(c-d/2.0);
                    xxmax=exp(c+d/2.0);
                }
            } else if (doScale) {
                double d=scale*(xxmax-xxmin);  // new width
                double c=(xxmax+xxmin)/2.0;     // center of interval
                xxmin=c-d/2.0;
                xxmax=c+d/2.0;
            }
            if (include0 && !axis->isLogAxis()) {
                if (xxmin>0) xxmin=0;
                else if (xxmax<0) xxmax=0;
            }

            if (JKQTPIsOKFloat(xxmin) && JKQTPIsOKFloat(xxmax)) {
                axis->setRange(xxmin, xxmax);
            } else if (!JKQTPIsOKFloat(xxmin) && JKQTPIsOKFloat(xxmax)) {
                axis->setRange(axis->getMin(), xxmax);
            } else if (JKQTPIsOKFloat(xxmin) && !JKQTPIsOKFloat(xxmax)) {
                axis->setRange(xxmin, axis->getMax());
            }
            return doScale;
        }
        return false;
    };

    if (zoomX) {
        const auto axes=getXAxes();
        for (auto it=axes.begin(); it!=axes.end(); ++it) {
            double xxmin=0;
            double xxmax=0;
            double xsmallestGreaterZero=0;
            if (getGraphsXMinMax(xxmin, xxmax, xsmallestGreaterZero, filterPlotElementByXAxis(it.key()))) {
                calcLocScaling(it.value(), xxmin, xxmax, xsmallestGreaterZero, includeX0, scaleX);
            }
        }
    }

    if (zoomY) {
        const auto axes=getYAxes();
        for (auto it=axes.begin(); it!=axes.end(); ++it) {
            double xxmin=0;
            double xxmax=0;
            double xsmallestGreaterZero=0;
            if (getGraphsYMinMax(xxmin, xxmax, xsmallestGreaterZero, filterPlotElementByYAxis(it.key()))) {
                calcLocScaling(it.value(), xxmin, xxmax, xsmallestGreaterZero, includeY0, scaleY);
            }
        }
    }
    //std::cout<<"end of zoomToFit\n";
    //setXY(xxmin, xxmax, yymin, yymax);
    if (emitSignals) emit zoomChangedLocally(xAxis->getMin(), xAxis->getMax(), yAxis->getMin(), yAxis->getMax(), this);
}








JKQTPPlotElement* JKQTBasePlotter::getGraph(size_t i) {
    return graphs[static_cast<int>(i)];
};

const JKQTPPlotElement* JKQTBasePlotter::getGraph(size_t i) const {
    return graphs[static_cast<int>(i)];
};

size_t JKQTBasePlotter::getGraphCount() const {
    return static_cast<size_t>(graphs.size());
};

void JKQTBasePlotter::deleteGraph(size_t i, bool deletegraph) {
    if (long(i)<0 || long(i)>=graphs.size()) return;
    JKQTPPlotElement* g=graphs[static_cast<int>(i)];
    graphs.removeAt(static_cast<int>(i));
    if (deletegraph && g) delete g;
    redrawPlot();
};

void JKQTBasePlotter::deleteGraph(JKQTPPlotElement* gr, bool deletegraph) {
    int i=indexOfGraph(gr);
    while (i>=0) {
        graphs.removeAt(i);
        i=indexOfGraph(gr);
    }

    if (deletegraph && gr) delete gr;
    redrawPlot();
}

void JKQTBasePlotter::clearGraphs(bool deleteGraphs) {
    for (int i=0; i<graphs.size(); i++) {
        JKQTPPlotElement* g=graphs[i];
        if (g && deleteGraphs) delete g;
    }
    graphs.clear();
    usedStyles.clear();
    redrawPlot();
}

void JKQTBasePlotter::setAllGraphsInvisible()
{
    for (int i=0; i<graphs.size(); i++) {
        JKQTPPlotElement* g=graphs[i];
        g->setVisible(false);
    }
    redrawPlot();
}

void JKQTBasePlotter::setAllGraphsVisible()
{
    for (int i=0; i<graphs.size(); i++) {
        JKQTPPlotElement* g=graphs[i];
        g->setVisible(true);
    }
    redrawPlot();
}

void JKQTBasePlotter::setGraphVisible(int i, bool visible)
{
    JKQTPPlotElement* g=graphs.value(i, nullptr);
    if (g) g->setVisible(visible);
    redrawPlot();
}

void JKQTBasePlotter::setGraphInvisible(int i)
{
    setGraphVisible(i, false);
}

void JKQTBasePlotter::setOnlyGraphVisible(int gr)
{
    for (int i=0; i<graphs.size(); i++) {
        JKQTPPlotElement* g=graphs[i];
        g->setVisible(false);
    }
    JKQTPPlotElement* g=graphs.value(gr, nullptr);
    if (g) g->setVisible(true);
    redrawPlot();
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
    redrawPlot();
}

size_t JKQTBasePlotter::addGraphOnTop(JKQTPPlotElement* gr) {
    gr->setParent(this);
    const int indexGR=indexOfGraph(gr);
    if (indexGR>=0) {
        graphs.removeAt(indexGR);
    }
    graphs.push_back(gr);
    redrawPlot();
    return static_cast<size_t>(graphs.size()-1);
}

size_t JKQTBasePlotter::addGraphAtBottom(JKQTPPlotElement* gr) {
    gr->setParent(this);
    const int indexGR=indexOfGraph(gr);
    if (indexGR>=0) {
        graphs.removeAt(indexGR);
    }
    graphs.push_front(gr);
    redrawPlot();
    return static_cast<size_t>(0);

}

void JKQTBasePlotter::moveGraphTop(int idx) {
    if (idx>=0 && idx<graphs.size()) {
        JKQTPPlotElement* g=graphs[idx];
        graphs.removeAt(idx);
        graphs.push_back(g);
        redrawPlot();
    } else {
        throw std::out_of_range("index out of range in JKQTBasePlotter::moveGraphTop()");
    }
}

void JKQTBasePlotter::moveGraphBottom(int idx)
{
    if (idx>=0 && idx<graphs.size()) {
        JKQTPPlotElement* g=graphs[idx];
        graphs.removeAt(idx);
        graphs.push_front(g);
        redrawPlot();
    } else {
        throw std::out_of_range("index out of range in JKQTBasePlotter::moveGraphBottom()");
    }
}

int JKQTBasePlotter::indexOfGraph(const JKQTPPlotElement *gr) const
{
    for (int i=0; i<graphs.size(); i++) {
        if (graphs[i]==gr) return i;
    }
    return -1;
}

void JKQTBasePlotter::moveGraphUp(int idx)
{
    if (idx>=0 && idx<graphs.size()) {
        if (idx+1<graphs.size()) {
            graphs.move(idx, idx+1);
            redrawPlot();
        }
    } else {
        throw std::out_of_range("index out of range in JKQTBasePlotter::moveGraphUp()");
    }
}

void JKQTBasePlotter::moveGraphDown(int idx)
{
    if (idx>=0 && idx<graphs.size()) {
        if (idx-1>=0) {
            graphs.move(idx-1, idx);
            redrawPlot();
        }
    } else {
        throw std::out_of_range("index out of range in JKQTBasePlotter::moveGraphDown()");
    }
}

void JKQTBasePlotter::moveGraphUp(const JKQTPPlotElement *gr) {
    moveGraphUp(indexOfGraph(gr));
}

void JKQTBasePlotter::moveGraphDown(const JKQTPPlotElement *gr)
{
    moveGraphDown(indexOfGraph(gr));
}

void JKQTBasePlotter::moveGraphTop(const JKQTPPlotElement *gr) {
    moveGraphTop(indexOfGraph(gr));
}

void JKQTBasePlotter::moveGraphBottom(const JKQTPPlotElement *gr)
{
    moveGraphBottom(indexOfGraph(gr));
}

void JKQTBasePlotter::modifyGraphs(const std::function<void (JKQTPPlotElement *)> &func)
{
    if (!func) return;
    for (int i=0; i<graphs.size(); i++) {
        func(graphs[i]);
    }
    redrawPlot();
}

void JKQTBasePlotter::sortGraphs(const std::function<bool (const JKQTPPlotElement *, const JKQTPPlotElement *)> &compareLess)
{
    if (!compareLess) return;
    std::sort(graphs.begin(), graphs.end(), compareLess);
    redrawPlot();
};

bool JKQTBasePlotter::containsGraph(const JKQTPPlotElement* gr) const {
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
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    tb->addAction(tv->getActionPrint());
#endif

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
    for (auto ax: getAxes(true)) {
        ax->setDoUpdateScaling(enabled);
    }
}



JKQTBasePlotter::textSizeKey::textSizeKey(const QFont &f_, const QString &text_, QPaintDevice *pd):
    text(text_), f(f_), ldpiX(0), ldpiY(0), pdpiX(0), pdpiY(0)
{
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

JKQTBasePlotter::textSizeKey::textSizeKey(const QString &fontName, double fontSize, const QString &text_, QPaintDevice *pd):
    text(text_), f([&](){
        QFont fnew;
        fnew.setFamily(JKQTMathTextFontSpecifier::fromFontSpec(fontName).fontName());
        fnew.setPointSizeF(fontSize);
        return fnew;
    }()), ldpiX(0), ldpiY(0), pdpiX(0), pdpiY(0)
{
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







QPaintDevice *JKQTPPaintDeviceAdapter::createPaintdeviceMM(const QString &filename, double widthMM, double heightMM) const
{
#if QT_VERSION>=QT_VERSION_CHECK(6,0,0)
    const qreal dpix=qGuiApp->primaryScreen()->logicalDotsPerInchX();
    const qreal dpiy=qGuiApp->primaryScreen()->logicalDotsPerInchY();
#else
    const qreal dpix=QApplication::desktop()->logicalDpiX();
    const qreal dpiy=QApplication::desktop()->logicalDpiY();
#endif
    return createPaintdevice(filename, jkqtp_roundTo<int>(widthMM/25.4*dpix), jkqtp_roundTo<int>(heightMM/25.4*dpiy));
}

JKQTPSaveDataAdapter::~JKQTPSaveDataAdapter() = default;

JKQTBasePlotter::JKQTPPen::JKQTPPen():
    m_color(QColor("red")),
    m_fillColor(QColor("red").lighter()),
    m_symbolFillColor(QColor("red").lighter()),
    m_errorColor(m_color.darker()),
    m_errorFillColor(m_color.lighter()),
    m_width(2),
    m_errorWidth(2),
    m_symbolLineWidth(1),
    m_style(Qt::SolidLine),
    m_symbolSize(12),
    m_fillStyle(Qt::SolidPattern),
    m_errorFillStyle(Qt::SolidPattern),
    m_symbol(JKQTPCross)
{

}

void JKQTBasePlotter::JKQTPPen::setSymbolSize(double w)
{
    m_symbolSize=w;
}

void JKQTBasePlotter::JKQTPPen::setSymbolLineWidthF(double w)
{
    m_symbolLineWidth=w;
}

void JKQTBasePlotter::JKQTPPen::setWidthF(double w) {
    m_width=w;
}

void JKQTBasePlotter::JKQTPPen::setWidth(double w) {
    m_width=w;
}

void JKQTBasePlotter::JKQTPPen::setErrorLineWidth(double w)
{
    m_errorWidth=w;
}

double JKQTBasePlotter::JKQTPPen::width() const {
    return m_width;
}

double JKQTBasePlotter::JKQTPPen::widthF() const {
    return m_width;
}

double JKQTBasePlotter::JKQTPPen::errorWidthF() const {
    return m_errorWidth;
}

double JKQTBasePlotter::JKQTPPen::symbolLineWidthF() const
{
    return m_symbolLineWidth;
}

double JKQTBasePlotter::JKQTPPen::symbolSize() const
{
    return m_symbolSize;
}

QColor JKQTBasePlotter::JKQTPPen::color() const {
    return m_color;
}

QColor JKQTBasePlotter::JKQTPPen::symbolFillColor() const {
    return m_symbolFillColor;
}

QColor JKQTBasePlotter::JKQTPPen::errorColor() const {
    return m_errorColor;
}

void JKQTBasePlotter::JKQTPPen::setColor(const QColor &col) {
    m_color=col;
}


Qt::PenStyle JKQTBasePlotter::JKQTPPen::style() const {
    return m_style;
}

Qt::PenStyle JKQTBasePlotter::JKQTPPen::errorStyle() const {
    return Qt::SolidLine;
}
void JKQTBasePlotter::JKQTPPen::setStyle(Qt::PenStyle s) {
    m_style=s;
}

QColor JKQTBasePlotter::JKQTPPen::fillColor() const {
    return m_fillColor;
}

QColor JKQTBasePlotter::JKQTPPen::errorFillColor() const {
    return m_errorFillColor;
}

void JKQTBasePlotter::JKQTPPen::setFillColor(const QColor &col) {
    m_fillColor=col;
}

void JKQTBasePlotter::JKQTPPen::setErrorFillColor(const QColor &col)
{
    m_errorFillColor=col;
}

void JKQTBasePlotter::JKQTPPen::setErrorLineColor(const QColor &col)
{
    m_errorColor=col;
}

void JKQTBasePlotter::JKQTPPen::setFillStyle(const JKQTFillStyleSummmary &s) {
    m_fillStyle=s;
}

void JKQTBasePlotter::JKQTPPen::setErrorFillStyle(Qt::BrushStyle s) {
    m_errorFillStyle=s;
}

void JKQTBasePlotter::JKQTPPen::setSymbolFillColor(QColor c) {
    m_symbolFillColor=c;
}

JKQTFillStyleSummmary JKQTBasePlotter::JKQTPPen::fillStyle() const {
    return m_fillStyle;
}

Qt::BrushStyle JKQTBasePlotter::JKQTPPen::errorFillStyle() const
{
    return m_errorFillStyle;
}

JKQTPGraphSymbols JKQTBasePlotter::JKQTPPen::symbol() const
{
    return m_symbol;
}

void JKQTBasePlotter::JKQTPPen::setSymbolType(JKQTPGraphSymbols symbol)
{
    m_symbol=symbol;
}

bool JKQTBasePlotter::filterPlotElementByXAxis::operator()(const JKQTPPlotElement *el) const {
    return el->getXAxisRef()==ref;
}

bool JKQTBasePlotter::filterPlotElementByYAxis::operator()(const JKQTPPlotElement *el) const {
    return el->getYAxisRef()==ref;
}

double JKQTBasePlotter::PlotMargins::calcLeft() const
{
    double margin=0;
    for (auto it=begin(); it!=end(); ++it) {
        margin+=it.value().left;
    }
    return margin;
}

double JKQTBasePlotter::PlotMargins::calcLeft(JKQTBasePlotter::PlotMarginUse start, JKQTBasePlotter::PlotMarginUse stop) const
{
    if (start==stop) return 0.0;
    if (stop<start) qSwap(start,stop);
    double margin=0;
    for (auto it=begin(); it!=end(); ++it) {
        if (it.key()>=start && it.key()<=stop) margin+=it.value().left;
    }
    return margin;
}

double JKQTBasePlotter::PlotMargins::calcRight() const
{
    double margin=0;
    for (auto it=begin(); it!=end(); ++it) {
        margin+=it.value().right;
    }
    return margin;
}

double JKQTBasePlotter::PlotMargins::calcRight(JKQTBasePlotter::PlotMarginUse start, JKQTBasePlotter::PlotMarginUse stop) const
{
    if (start==stop) return 0.0;
    if (start>stop) qSwap(start, stop);
    double margin=0;
    for (auto it=begin(); it!=end(); ++it) {
        if (it.key()>=start && it.key()<=stop) margin+=it.value().right;
    }
    return margin;
}

double JKQTBasePlotter::PlotMargins::calcTop() const
{
    double margin=0;
    for (auto it=begin(); it!=end(); ++it) {
        margin+=it.value().top;
    }
    return margin;
}

double JKQTBasePlotter::PlotMargins::calcTop(JKQTBasePlotter::PlotMarginUse start, JKQTBasePlotter::PlotMarginUse stop) const
{
    if (start==stop) return 0.0;
    if (start>stop) qSwap(start, stop);
    double margin=0;
    for (auto it=begin(); it!=end(); ++it) {
        if (it.key()>=start && it.key()<=stop) margin+=it.value().top;
    }
    return margin;
}

double JKQTBasePlotter::PlotMargins::calcBottom() const
{
    double margin=0;
    for (auto it=begin(); it!=end(); ++it) {
        margin+=it.value().bottom;
    }
    return margin;
}

double JKQTBasePlotter::PlotMargins::calcBottom(JKQTBasePlotter::PlotMarginUse start, JKQTBasePlotter::PlotMarginUse stop) const
{
    if (start==stop) return 0.0;
    if (start>stop) qSwap(start, stop);
    double margin=0;
    for (auto it=begin(); it!=end(); ++it) {
        if (it.key()>=start && it.key()<=stop) margin+=it.value().bottom;
    }
    return margin;
}


