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




#include <QFileInfo>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtGlobal>
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include "jkqtplotter.h"


#define jkqtp_RESIZE_DELAY 100






/**************************************************************************************************************************
 * JKQTPlotter
 **************************************************************************************************************************/
JKQTPlotter::JKQTPlotter(bool datastore_internal, QWidget* parent, JKQTPDatastore* datast):
    QWidget(parent, Qt::Widget),
    currentMouseDragAction(),
    doDrawing(false),
    plotter(nullptr),
    mouseDragingRectangle(false),
    mouseDragRectXStart(0), mouseDragRectXStartPixel(0), mouseDragRectXEndPixel(0),
    mouseDragRectYEndPixel(0),  mouseDragRectXEnd(0), mouseDragRectYStart(0),
    mouseDragRectYStartPixel(0), mouseDragRectYEnd(0),
    mouseDragMarkers(),
    image(), oldImage(),
    menuSpecialContextMenu(nullptr),toolbar(nullptr), masterPlotterX(nullptr), masterPlotterY(nullptr),
    mousePosX(0), mousePosY(0),
    magnification(1),
    minSize(),
    contextMenu(nullptr),
    contextMenuMode(JKQTPContextMenuModes::jkqtpcmmStandardContextMenu),
    mouseContextX(0), mouseContextY(0), mouseLastClickX(0),  mouseLastClickY(0),
    contextSubMenus(),
    plotterStyle(JKQTPGetSystemDefaultStyle()),
    resizeTimer(), registeredOverrideMouseDragActionModes(),
    actgrpMouseLeft(nullptr), actMouseLeftAsDefault(nullptr), actMouseLeftAsRuler(nullptr), actMouseMoveToolTip(nullptr), actMouseLeftAsZoomRect(nullptr), actMouseLeftAsPanView(nullptr)
{    
    initJKQTPlotterResources();

    setParent(parent);
    connect(&resizeTimer, SIGNAL(timeout()), this, SLOT(delayedResizeEvent()));

    plotter=new JKQTBasePlotter(datastore_internal, this, datast);
    plotter->setEmittingSignalsEnabled(false);
    fixBasePlotterSettings();


    connect(plotter, SIGNAL(plotUpdated()), this, SLOT(redrawPlot()));
    connect(plotter, SIGNAL(beforePlotScalingRecalculate()), this, SLOT(intBeforePlotScalingRecalculate()));
    connect(plotter, SIGNAL(zoomChangedLocally(double, double, double, double, JKQTBasePlotter*)), this, SLOT(pzoomChangedLocally(double, double, double, double, JKQTBasePlotter*)));

    image=QImage(width(), height(), QImage::Format_ARGB32);
    oldImage=image;

    // enable mouse-tracking, so mouseMoved-Events can be caught
    setMouseTracking(true);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    actgrpMouseLeft=new QActionGroup(this);
    actMouseLeftAsDefault=actgrpMouseLeft->addAction(QIcon(":/JKQTPlotter/jkqtp_mouseact_default.png"), tr("Default Tool"));
    actMouseLeftAsDefault->setToolTip(tr("switch back to the default mouse action/tool (left button, no modifiers)"));
    actMouseLeftAsDefault->setCheckable(true);
    actMouseLeftAsPanView=actgrpMouseLeft->addAction(QIcon(":/JKQTPlotter/jkqtp_mouseact_drag.png"), tr("Pan View Tool"));
    actMouseLeftAsPanView->setToolTip(tr("switch on the pan view tool (left button, no modifiers)"));
    actMouseLeftAsPanView->setCheckable(true);
    actMouseLeftAsZoomRect=actgrpMouseLeft->addAction(QIcon(":/JKQTPlotter/jkqtp_mouseact_zoomrect.png"), tr("Zoom Rectangle Tool"));
    actMouseLeftAsZoomRect->setToolTip(tr("switch on the zoom rectangle tool (left button, no modifiers)"));
    actMouseLeftAsZoomRect->setCheckable(true);
    actMouseLeftAsRuler=actgrpMouseLeft->addAction(QIcon(":/JKQTPlotter/jkqtp_mouseact_ruler.png"), tr("Ruler Tool"));
    actMouseLeftAsRuler->setToolTip(tr("switch on the ruler tool (left button, no modifiers)"));
    actMouseLeftAsRuler->setCheckable(true);
    //actMouseLeftAsToolTip=actgrpMouseLeft->addAction(QIcon(":/JKQTPlotter/jkqtp_mouseact_tooltip.png"), tr("Data Tooltip Tool"));
    //actMouseLeftAsToolTip->setToolTip(tr("switch on the data tooltip tool when moving the mouse (no modifiers)"));
    //actMouseLeftAsToolTip->setCheckable(true);
    actgrpMouseLeft->setExclusive(true);
    actMouseLeftAsDefault->setChecked(true);

    actMouseMoveToolTip=new QAction(QIcon(":/JKQTPlotter/jkqtp_mouseact_tooltip.png"), tr("Data Tooltip Tool"));
    actMouseMoveToolTip->setToolTip(tr("switch on the data tooltip tool when moving the mouse (no modifiers)"));
    actMouseMoveToolTip->setCheckable(true);
    actMouseMoveToolTip->setChecked(false);

    connect(actMouseLeftAsDefault, SIGNAL(triggered()), this, SLOT(resetMouseLeftAction()));
    connect(actMouseLeftAsRuler, SIGNAL(triggered()), this, SLOT(setMouseLeftActionAsRuler()));
    connect(actMouseLeftAsPanView, SIGNAL(triggered()), this, SLOT(setMouseLeftActionAsPanView()));
    connect(actMouseLeftAsZoomRect, SIGNAL(triggered()), this, SLOT(setMouseLeftActionAsZoomRect()));
    connect(actMouseMoveToolTip, SIGNAL(toggled(bool)), this, SLOT(setMouseMoveActionAsToolTip(bool)));


    toolbar=new JKVanishQToolBar(this);
    toolbar->clear();
    toolbar->move(1,1);
    toolbar->hide();
    toolbar->setAutoFillBackground(true);
    toolbar->addSeparator();
    toolbar->addSeparator();
    populateToolbar(toolbar);
    QSize s=QSize(plotterStyle.toolbarIconSize, plotterStyle.toolbarIconSize);
    toolbar->setIconSize(s);

    contextMenu=nullptr;//new QMenu(this);

    //move(32,32);
    resize(400,300);
    doDrawing=true;
    plotter->setEmittingSignalsEnabled(true);
    redrawPlot();
}

JKQTPlotter::JKQTPlotter(JKQTPDatastore *datast, QWidget *parent):
    JKQTPlotter(false, parent, datast)
{

}

JKQTPlotter::JKQTPlotter(QWidget *parent):
    JKQTPlotter(true, parent, nullptr)
{

}

JKQTPlotter::~JKQTPlotter() {
    resetContextMenu(false);
    disconnect(plotter, SIGNAL(plotUpdated()), this, SLOT(redrawPlot()));
    disconnect(plotter, SIGNAL(beforePlotScalingRecalculate()), this, SLOT(intBeforePlotScalingRecalculate()));
    disconnect(plotter, SIGNAL(zoomChangedLocally(double, double, double, double, JKQTBasePlotter*)), this, SLOT(pzoomChangedLocally(double, double, double, double, JKQTBasePlotter*)));
    delete plotter;
}

void JKQTPlotter::fixBasePlotterSettings()
{
    if (plotterStyle.usePaletteColors) {
        plotter->setBackgroundColor(palette().color(QPalette::Window));
    }
}

void JKQTPlotter::updateToolbarActions()
{
    toolbar->clear();
    toolbar->setAutoFillBackground(true);
    toolbar->addSeparator();
    toolbar->addSeparator();
    populateToolbar(toolbar);

}

void JKQTPlotter::setToolbarIconSize(int value) {
    plotterStyle.toolbarIconSize=value;
    QSize s=QSize(plotterStyle.toolbarIconSize, plotterStyle.toolbarIconSize);
    toolbar->setIconSize(s);
}

int JKQTPlotter::getToolbarIconSize() {
    return plotterStyle.toolbarIconSize;
}

void JKQTPlotter::setToolbarEnabled(bool __value)
{
    if (plotterStyle.toolbarEnabled != __value) {
        plotterStyle.toolbarEnabled = __value;
        updateToolbar();
    }
}

bool JKQTPlotter::isToolbarEnabled() const
{
    return plotterStyle.toolbarEnabled;
}

void JKQTPlotter::setToolbarAlwaysOn(bool __value)
{
    if (plotterStyle.toolbarAlwaysOn != __value) {
        plotterStyle.toolbarAlwaysOn = __value;
        updateToolbar();
    }
}

bool JKQTPlotter::isToolbarAlwaysOn() const
{
    return plotterStyle.toolbarAlwaysOn;
}

void JKQTPlotter::setMousePositionShown(bool __value)
{
    plotterStyle.displayMousePosition = __value;
}

bool JKQTPlotter::isMousePositionShown() const
{
    return plotterStyle.displayMousePosition;
}


void JKQTPlotter::setUserActionOverlayPen(const QPen &__value)
{
    if (plotterStyle.userActionOverlayPen != __value) {
        plotterStyle.userActionOverlayPen = __value;
        update();
    }
}

QPen JKQTPlotter::getUserActionOverlayPen() const
{
    return plotterStyle.userActionOverlayPen;
}

void JKQTPlotter::setUserActionOverlayBrush(const QBrush &__value)
{
    if (plotterStyle.userActionOverlayBrush != __value) {
        plotterStyle.userActionOverlayBrush = __value;
        update();
    }
}

QBrush JKQTPlotter::getUserActionOverlayBrush() const
{
    return plotterStyle.userActionOverlayBrush;
}

void JKQTPlotter::setUserActionOpaquePen(const QPen &__value)
{
    if (plotterStyle.userActionOpaquePen != __value) {
        plotterStyle.userActionOpaquePen = __value;
        update();
    }
}

QPen JKQTPlotter::getUserActionOpaquePen() const
{
    return plotterStyle.userActionOpaquePen;
}

void JKQTPlotter::setUserActionOpaqueBrush(const QBrush &__value)
{
    if (plotterStyle.userActionOpaqueBrush != __value) {
        plotterStyle.userActionOpaqueBrush = __value;
        update();
    }
}

QBrush JKQTPlotter::getUserActionOpaqueBrush() const
{
    return plotterStyle.userActionOpaqueBrush;
}

void JKQTPlotter::setUserActionMarkerPen(const QPen &__value)
{
    if (plotterStyle.userActionMarkerPen != __value) {
        plotterStyle.userActionMarkerPen = __value;
        update();
    }
}

QPen JKQTPlotter::getUserActionMarkerPen() const
{
    return plotterStyle.userActionMarkerPen;
}

void JKQTPlotter::setUserActionMarkerBrush(const QBrush &__value)
{
    if (plotterStyle.userActionMarkerBrush != __value) {
        plotterStyle.userActionMarkerBrush = __value;
        update();
    }
}

QBrush JKQTPlotter::getUserActionMarkerBrush() const
{
    return plotterStyle.userActionMarkerBrush;
}


void JKQTPlotter::setMaxTooltipDistance(int v)
{
    plotterStyle.maxTooltipDistance=v;
}

int JKQTPlotter::getMaxTooltipDistance() const
{
    return plotterStyle.maxTooltipDistance;
}

void JKQTPlotter::setUserActionCatchSensitivity(int v)
{
    plotterStyle.userActionCatchSensitivity=v;
}

int JKQTPlotter::getUserActionCatchSensitivity() const
{
    return plotterStyle.userActionCatchSensitivity;
}

void JKQTPlotter::setUserActionMarkerDiameter(int v)
{
    plotterStyle.userActionMarkerDiameter=v;
}

void JKQTPlotter::setUserActionMarkerType(JKQTPUserActionMarkerType v)
{
    plotterStyle.userActionMarkerType=v;
}

int JKQTPlotter::getUserActionMarkerDiameter() const
{
    return plotterStyle.userActionMarkerDiameter;
}

JKQTPUserActionMarkerType JKQTPlotter::getUserActionMarkerType() const
{
    return plotterStyle.userActionMarkerType;
}



void JKQTPlotter::registerMouseWheelAction(Qt::KeyboardModifiers modifier, JKQTPMouseWheelActions action)
{
    //qDebug()<<"registerMouseWheelAction("<<modifier<<","<<action<<"): "<<plotterStyle.registeredMouseWheelActions;
    plotterStyle.registeredMouseWheelActions[modifier]=action;
    //qDebug()<<"registerMouseWheelAction("<<modifier<<","<<action<<"): "<<plotterStyle.registeredMouseWheelActions;
}

void JKQTPlotter::deregisterMouseWheelAction(Qt::KeyboardModifiers modifier)
{
    //qDebug()<<"deregisterMouseWheelAction("<<modifier<<"): "<<plotterStyle.registeredMouseWheelActions;
    plotterStyle.registeredMouseWheelActions.remove(modifier);
    //qDebug()<<"deregisterMouseWheelAction("<<modifier<<"): "<<plotterStyle.registeredMouseWheelActions;
}

void JKQTPlotter::clearAllMouseWheelActions()
{
    //qDebug()<<"clearAllMouseWheelActions(): "<<plotterStyle.registeredMouseWheelActions;
    plotterStyle.registeredMouseWheelActions.clear();
    //qDebug()<<"clearAllMouseWheelActions(): "<<plotterStyle.registeredMouseWheelActions;
}

void JKQTPlotter::registerMouseMoveAction(Qt::KeyboardModifiers modifier, JKQTPMouseMoveActions action)
{
    //qDebug()<<"registerMouseMoveAction("<<modifier<<","<<action<<"): "<<plotterStyle.registeredMouseMoveActions;
    plotterStyle.registeredMouseMoveActions[modifier]=action;
    //qDebug()<<"registerMouseMoveAction("<<modifier<<","<<action<<"): "<<plotterStyle.registeredMouseMoveActions;
}

void JKQTPlotter::deregisterMouseMoveAction(Qt::KeyboardModifiers modifier)
{
    //qDebug()<<"deregisterMouseMoveAction("<<modifier<<"): "<<plotterStyle.registeredMouseMoveActions;
    plotterStyle.registeredMouseMoveActions.remove(modifier);
    //qDebug()<<"deregisterMouseMoveAction("<<modifier<<"): "<<plotterStyle.registeredMouseMoveActions;
}

void JKQTPlotter::clearAllMouseMoveActions()
{
    //qDebug()<<"clearAllMouseMoveActions(): "<<plotterStyle.registeredMouseMoveActions;
    plotterStyle.registeredMouseMoveActions.clear();
    //qDebug()<<"clearAllMouseMoveActions(): "<<plotterStyle.registeredMouseMoveActions;
}




void JKQTPlotter::loadSettings(const QSettings& settings, const QString& group) {
    plotter->loadSettings(settings, group);
    plotterStyle.loadSettings(settings, group, JKQTPGetSystemDefaultStyle());


    updateToolbar();
    redrawPlot();
}

void JKQTPlotter::saveSettings(QSettings& settings, const QString& group) const {
    plotterStyle.saveSettings(settings, group);
    plotter->saveSettings(settings, group);
}


void JKQTPlotter::fillInternalStructForToolTipOfClosestDataPoint(double x0, double y0, bool emitEvent) {
    bool foundAll=false;
    QList<QString> labels, titles;
    QList<QColor> colors;
    QList<QPointF> posSyss;
    QList<QImage> keymarkers;
    QList<JKQTPPlotElement*> graphs;
    QList<double> ds;
    double dbest=0;
    for (size_t i=0; i<getPlotter()->getGraphCount(); i++) {
        JKQTPPlotElement* g=getPlotter()->getGraph(i);
        bool found=false;
        QString label;
        QPointF posSys;
        double d=0;
        if (g && g->isVisible()) {
            QPointF pp;
            QString ll;
            double dist=g->hitTest(QPointF(x0, y0), &pp, &ll, JKQTPPlotElement::HitTestXY);
            if (JKQTPIsOKFloat(dist)) {
                if (!found || dist<d) {
                    d=dist;
                    posSys=pp;
                    label=ll;
                    found=true;
                }
            }
        }
        if (found && g) {
            if (!foundAll) {
                dbest=d;
                labels.clear();
                labels<<label;
                titles.clear();
                titles<<g->getTitle();
                colors.clear();
                colors<<g->getKeyLabelColor();
                posSyss.clear();
                posSyss<<posSys;
                ds.clear();
                ds<<d;
                keymarkers.clear();
                keymarkers<<g->generateKeyMarker();
                graphs.clear();
                graphs<<g;
            } else {
                if (fabs(dbest-d)<=plotterStyle.userActionCatchSensitivity) {
                    labels<<label;
                    titles<<g->getTitle();
                    colors<<g->getKeyLabelColor();
                    posSyss<<posSys;
                    ds<<d;
                    keymarkers<<g->generateKeyMarker();
                    graphs<<g;
                } else {
                    if (d<dbest) {
                        labels.clear();
                        labels<<label;
                        titles.clear();
                        titles<<g->getTitle();
                        colors.clear();
                        colors<<g->getKeyLabelColor();
                        posSyss.clear();
                        posSyss<<posSys;
                        ds.clear();
                        ds<<d;
                        keymarkers.clear();
                        keymarkers<<g->generateKeyMarker();
                        graphs.clear();
                        graphs<<g;
                    }
                }
                if (d<dbest) dbest=d;
            }
            foundAll=true;
        }
    }
    mouseDragMarkers.clear();
    if (foundAll && dbest<getCurrentPlotterStyle().maxTooltipDistance) {
        QStringList entries;
        for (int i=0; i<posSyss.size(); i++) {
            const QPoint posPix=QPointF(getXAxis()->x2p(posSyss[i].x())*magnification, getYAxis()->x2p(posSyss[i].y())*magnification).toPoint();
            entries<<(titles[i]+": "+labels[i]);
            mouseDragMarkers<<MouseDragMarker(posPix, labels[i], titles[i], colors[i], keymarkers[i], graphs[i]);
        }
        //const QPoint globalPosPix=mapToGlobal(posPix);
        //const int rectsize=24;
        //qDebug()<<graph->getTitle()<<" clicked("<<event->pos()<<")  ==> posSys="<<posSys<<" / posPix="<<posPix<<" globalPosPix="<<globalPosPix<<", label="<<label<<", distance="<<d;
        //QToolTip::showText(globalPosPix, graph->getTitle()+":\n"+label, this);//, QRect(posPix.x()-rectsize/2, posPix.y()-rectsize/2, rectsize, rectsize));

        if (emitEvent) {
            if (mouseDragMarkers.size()>0) {
                emit tooltipDisplayed(posSyss[0].x(), posSyss[0].y(), entries, graphs);
            }
        }
    }
}

void JKQTPlotter::paintUserAction() {
    if ((currentMouseDragAction.isValid() && mouseDragingRectangle) || (!currentMouseMoveAction.isEmpty())) {
        image=oldImage;
        if (image.width()>0 && image.height()>0 && !image.isNull()) {
            JKQTPEnhancedPainter painter(&image);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
            painter.setRenderHint(JKQTPEnhancedPainter::NonCosmeticDefaultPen, true);
#endif
            painter.setRenderHint(JKQTPEnhancedPainter::Antialiasing, true);
            painter.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing, true);
            painter.setPen(plotterStyle.userActionOverlayPen);
            if (jkqtp_approximatelyUnequal(mouseDragRectXEnd,mouseDragRectXStart) && jkqtp_approximatelyUnequal(mouseDragRectYEnd,mouseDragRectYStart)) {
                const double x1=plotter->x2p(mouseDragRectXStart)*magnification;
                const double y1=plotter->y2p(mouseDragRectYStart)*magnification;
                const double x2=plotter->x2p(mouseDragRectXEnd)*magnification;
                const double y2=plotter->y2p(mouseDragRectYEnd)*magnification;
                const double dx=x2-x1;
                const double dy=y2-y1;
                if (currentMouseDragAction.isValid() && mouseDragingRectangle) {
                    if (currentMouseDragAction.mode==jkqtpmdaDrawRectangleForEvent) {
                        painter.fillRect(QRectF(x1, y1, x2-x1, y2-y1), plotterStyle.userActionOverlayBrush);
                        painter.setPen(plotterStyle.userActionOverlayPen);
                        painter.drawRect(QRectF(x1, y1, x2-x1, y2-y1));
                    } else if (currentMouseDragAction.mode==jkqtpmdaZoomByRectangle) {
                        double xmin=mouseDragRectXStart;
                        double xmax=mouseDragRectXEnd;
                        double ymin=mouseDragRectYStart;
                        double ymax=mouseDragRectYEnd;
                        plotter->correctXYRangeForAspectRatio(xmin,xmax,ymin,ymax);

                        const double xz1=plotter->x2p(xmin)*magnification;
                        const double yz1=plotter->y2p(ymin)*magnification;
                        const double xz2=plotter->x2p(xmax)*magnification;
                        const double yz2=plotter->y2p(ymax)*magnification;

                        painter.fillRect(QRectF(xz1, yz1, xz2-xz1, yz2-yz1), plotterStyle.userActionOverlayBrush);
                        painter.setPen(plotterStyle.userActionOverlayPen);
                        painter.drawRect(QRectF(xz1, yz1, xz2-xz1, yz2-yz1));
                    } else if (currentMouseDragAction.mode==jkqtpmdaDrawCircleForEvent) {
                        painter.setPen(plotterStyle.userActionOverlayPen);
                        painter.setBrush(plotterStyle.userActionOverlayBrush);
                        painter.drawEllipse(QPointF(x1, y1), qMin(fabs(dx), fabs(dy)), qMin(fabs(dx), fabs(dy)));
                    } else  if (currentMouseDragAction.mode==jkqtpmdaDrawEllipseForEvent) {
                        painter.setPen(plotterStyle.userActionOverlayPen);
                        painter.setBrush(plotterStyle.userActionOverlayBrush);
                        painter.drawEllipse(QPointF(x1, y1), fabs(dx), fabs(dy));
                    } else  if (currentMouseDragAction.mode==jkqtpmdaDrawLineForEvent) {
                        painter.setPen(plotterStyle.userActionOverlayPen);
                        painter.setBrush(plotterStyle.userActionOverlayBrush);
                        painter.drawLine(QPointF(x1,y1), QPointF(x2,y2));
                    } else  if (currentMouseDragAction.mode==jkqtpmdaRuler) {
                        painter.setPen(plotterStyle.userActionOpaquePen);
                        painter.setBrush(plotterStyle.userActionOpaqueBrush);
                        painter.drawLine(QPointF(x1,y1), QPointF(x2,y1));
                        painter.drawLine(QPointF(x2,y1), QPointF(x2,y2));
                        painter.drawLine(QPointF(x1,y1), QPointF(x2,y2));
                        const double dxy=sqrt(jkqtp_sqr(mouseDragRectXEnd-mouseDragRectXStart)+jkqtp_sqr(mouseDragRectYEnd-mouseDragRectYStart));
                        const double alpha=atan2((mouseDragRectYEnd-mouseDragRectYStart), (mouseDragRectXEnd-mouseDragRectXStart))/JKQTPSTATISTICS_PI*180.0;
                        const double dx=fabs(mouseDragRectXEnd-mouseDragRectXStart);
                        const double dy=fabs(mouseDragRectYEnd-mouseDragRectYStart);

                        painter.setBrush(plotterStyle.userActionOpaqueBrush);
                        QString txt;
                        double a=0,d=0,so=0,w=0;
                        getPlotter()->getMathText()->setFontSize(plotterStyle.userActionFontSize);
                        getPlotter()->getMathText()->setFontRomanOrSpecial(plotterStyle.userActionFontName);

                        txt=QString::fromStdString("\\delta_{x}="+jkqtp_floattolatexstr(dx, 3));
                        getPlotter()->getMathText()->parse(txt);
                        getPlotter()->getMathText()->getSizeDetail(painter, w, a, d, so);
                        if (y1>y2) {
                            QRectF rec((x1+x2)/2.0-w/2.0, y1+2, w, a+d);
                            painter.fillRect(rec, plotterStyle.userActionOpaqueBrush);
                            getPlotter()->getMathText()->draw(painter, Qt::AlignTop, rec,  getPlotter()->getCurrentPlotterStyle().debugShowTextBoxes);
                        } else {
                            QRectF rec((x1+x2)/2.0-w/2.0, y1-2-a-d, w, a+d);
                            painter.fillRect(rec, plotterStyle.userActionOpaqueBrush);
                            getPlotter()->getMathText()->draw(painter, Qt::AlignBottom, rec,  getPlotter()->getCurrentPlotterStyle().debugShowTextBoxes);
                        }

                        txt=jkqtp_floattolatexqstr(dy, 3);
                        getPlotter()->getMathText()->parse("\\delta_{y}="+txt);
                        getPlotter()->getMathText()->getSizeDetail(painter, w, a, d, so);
                        //double dyh=a+d;
                        if (x2>x1) {
                            QRectF rec(x2+2, (y1+y2)/2.0-(a+d)/2.0, w, a+d);
                            painter.fillRect(rec, plotterStyle.userActionOpaqueBrush);
                            getPlotter()->getMathText()->draw(painter, Qt::AlignVCenter|Qt::AlignLeft, rec,  getPlotter()->getCurrentPlotterStyle().debugShowTextBoxes);
                        } else {
                            QRectF rec(x2-2-w, (y1+y2)/2.0-(a+d)/2.0, w, a+d);
                            painter.fillRect(rec, plotterStyle.userActionOpaqueBrush);
                            getPlotter()->getMathText()->draw(painter, Qt::AlignVCenter|Qt::AlignRight, rec,  getPlotter()->getCurrentPlotterStyle().debugShowTextBoxes);
                        }


                        txt=QString::fromStdString("\\delta_{x,y}="+jkqtp_floattolatexstr(dxy)+", \\alpha="+jkqtp_floattolatexstr(alpha, 1)+"\\degree, \\stfrac{\\mathrm{d}y}{\\mathrm{d}x}="+jkqtp_floattolatexstr(dy/dx, 1));
                        getPlotter()->getMathText()->parse(txt);
                        getPlotter()->getMathText()->getSizeDetail(painter, w, a, d, so);
                        if (x2<x1) {
                            QRectF rec((x1+x2)/2.0, (y1+y2)/2.0-(a+d)/2.0, w, a+d);
                            painter.fillRect(rec, plotterStyle.userActionOpaqueBrush);
                            getPlotter()->getMathText()->draw(painter,Qt::AlignTop|Qt::AlignLeft, rec,  getPlotter()->getCurrentPlotterStyle().debugShowTextBoxes);
                        } else {
                            QRectF rec((x1+x2)/2.0-w, (y1+y2)/2.0-(a+d)/2.0, w, a+d);
                            painter.fillRect(rec, plotterStyle.userActionOpaqueBrush);
                            getPlotter()->getMathText()->draw(painter,Qt::AlignTop|Qt::AlignRight, rec,  getPlotter()->getCurrentPlotterStyle().debugShowTextBoxes);
                        }


                    }
                }
                if ((currentMouseDragAction.isValid() && currentMouseDragAction.mode==jkqtpmdaToolTipForClosestDataPoint) || currentMouseMoveAction.contains(jkqtpmmaToolTipForClosestDataPoint)) {
                    if (mouseDragMarkers.size()>0) {
                        const int markerD=getCurrentPlotterStyle().userActionMarkerDiameter;
                        if (getCurrentPlotterStyle().userActionMarkerType==jkqtpuamtCircle) {
                            painter.setBrush(plotterStyle.userActionMarkerBrush);
                            painter.setPen(plotterStyle.userActionMarkerPen);
                            painter.drawEllipse(QRectF(mouseDragMarkers[0].pos.x()-markerD/2, mouseDragMarkers[0].pos.y()-markerD/2, markerD, markerD));
                        } else if (getCurrentPlotterStyle().userActionMarkerType==jkqtpuamtCrossHair) {
                            painter.setBrush(plotterStyle.userActionMarkerBrush);
                            painter.setPen(plotterStyle.userActionMarkerPen);
                            painter.drawLine(QLineF(plotter->getInternalPlotBorderLeft(), mouseDragMarkers[0].pos.y(), image.width()-plotter->getInternalPlotBorderRight(), mouseDragMarkers[0].pos.y()));
                            painter.drawLine(QLineF(mouseDragMarkers[0].pos.x(), plotter->getInternalPlotBorderTop(), mouseDragMarkers[0].pos.x(), image.height()-plotter->getInternalPlotBorderBottom()));
                        } else if (getCurrentPlotterStyle().userActionMarkerType==jkqtpuamtCircleAndCrossHair) {
                            painter.setBrush(plotterStyle.userActionMarkerBrush);
                            painter.setPen(plotterStyle.userActionMarkerPen);
                            painter.drawEllipse(QRectF(mouseDragMarkers[0].pos.x()-markerD/2, mouseDragMarkers[0].pos.y()-markerD/2, markerD, markerD));
                            painter.drawLine(QLineF(plotter->getInternalPlotBorderLeft(), mouseDragMarkers[0].pos.y(), mouseDragMarkers[0].pos.x()-markerD/2, mouseDragMarkers[0].pos.y()));
                            painter.drawLine(QLineF(image.width()-plotter->getInternalPlotBorderRight(), mouseDragMarkers[0].pos.y(), mouseDragMarkers[0].pos.x()+markerD/2, mouseDragMarkers[0].pos.y()));
                            painter.drawLine(QLineF(mouseDragMarkers[0].pos.x(), plotter->getInternalPlotBorderTop(), mouseDragMarkers[0].pos.x(), mouseDragMarkers[0].pos.y()-markerD/2));
                            painter.drawLine(QLineF(mouseDragMarkers[0].pos.x(), image.height()-plotter->getInternalPlotBorderBottom(), mouseDragMarkers[0].pos.x(), mouseDragMarkers[0].pos.y()+markerD/2));
                        }
                        const int xoffset=qMax<int>(markerD*3/2,16);
                        const int x0=mouseDragMarkers[0].pos.x()+xoffset;
                        QRectF rall(0,0,0,0);
                        double y=mouseDragMarkers[0].pos.y();
                        QVector<QRectF> rects;
                        QVector<QString> txts;
                        QVector<QColor> colors;
                        const int txtoffset=4;
                        QString txt;
                        double ascent=0,descent=0,strikeout=0,width=0;
                        getPlotter()->getMathText()->setFontSize(plotterStyle.userActionFontSize);
                        getPlotter()->getMathText()->setFontRomanOrSpecial(plotterStyle.userActionFontName);


                        for (auto& m: mouseDragMarkers) {
                            QString txt="\\textcolor{"+jkqtp_QColor2String(m.color)+"}{"+m.title+": "+m.label+"}";

                            getPlotter()->getMathText()->parse(txt);
                            getPlotter()->getMathText()->getSizeDetail(painter, width, ascent, descent, strikeout);

                            double txtheight=ascent+descent;
                            QRectF rec(x0+txtoffset, y, width+1, txtheight+1);
                            if (rall.width()*rall.height()<=0) {
                                rall=rec.toRect();
                            } else {
                                rall.setBottom(rall.bottom()+txtheight+ascent/4);
                                rall.setRight(qMax(rall.right(), rec.right()));
                            }
                            rects << rec;
                            txts<<txt;
                            colors<<m.color;
                            //qDebug()<<y<<txt<<rall;
                            y=y+txtheight+ascent/4;
                        }
                        rall.setLeft(rall.left()-txtoffset);
                        rall.setRight(rall.right()+txtoffset);
                        rall.setTop(rall.top()-txtoffset);
                        rall.setBottom(rall.bottom()+txtoffset);

                        if (rall.right()>image.width()) {
                            rall.moveLeft(mouseDragMarkers[0].pos.x()-rall.width()-xoffset);
                            for (auto& r: rects) {
                                r.moveLeft(rall.left()+txtoffset);
                            }
                        }
                        if (rall.bottom()>image.height()) {
                            double oldY=rall.y();
                            rall.moveTop(image.height()-rall.height());
                            for (auto& r: rects) {
                                r.moveTop(r.y()-(oldY-rall.y()));
                            }
                        }
                        //qDebug()<<"rall="<<rall;
                        painter.setBrush(plotterStyle.userActionOpaqueBrush);
                        painter.setPen(plotterStyle.userActionOpaquePen);
                        JKQTPDrawTooltip(painter, mouseDragMarkers[0].pos.x(), mouseDragMarkers[0].pos.y(), rall);
                        for (int i=0; i<txts.size(); i++) {
                            getPlotter()->getMathText()->parse(txts[i]);
                            getPlotter()->getMathText()->draw(painter, Qt::AlignTop|Qt::AlignLeft, rects[i],  getPlotter()->getCurrentPlotterStyle().debugShowTextBoxes);
                            QStringList el=getPlotter()->getMathText()->getErrorList();
                            if (el.size()>0) {
                                qDebug()<<"error parsing LaTeX string fo tooltip ("<<txts[i]<<"):\n     - "<<el.join("\n     - ");
//                            } else {
//                                qDebug()<<txts[i];
                            }
                        }
                    }
                }
            }
        }
        update();
    }
}


void JKQTPlotter::mouseMoveEvent ( QMouseEvent * event ) {
    if (plotterStyle.displayMousePosition) {
        mousePosX=plotter->p2x(event->pos().x()/magnification);
        mousePosY=plotter->p2y((event->pos().y()-getPlotYOffset())/magnification);
        update();
    }

    if (plotterStyle.toolbarEnabled && (!plotterStyle.toolbarAlwaysOn) && (!toolbar->isVisible())) { // decide whether to display toolbar
        int y1=10;
        if (event->pos().y()/magnification>=0 && event->pos().y()/magnification<=y1) {
            toolbar->show();
            toolbar->move(1,1);
        }
    }
    if (!plotterStyle.toolbarEnabled) {
        toolbar->hide();
    }
    currentMouseMoveAction.clear();
    if (currentMouseDragAction.isValid()) {
        if (( (currentMouseDragAction.mode==jkqtpmdaZoomByRectangle) ||
              (currentMouseDragAction.mode==jkqtpmdaDrawRectangleForEvent) ||
              (currentMouseDragAction.mode==jkqtpmdaDrawCircleForEvent) ||
              (currentMouseDragAction.mode==jkqtpmdaDrawEllipseForEvent) ||
              (currentMouseDragAction.mode==jkqtpmdaScribbleForEvents) ||
              (currentMouseDragAction.mode==jkqtpmdaToolTipForClosestDataPoint) ||
              (currentMouseDragAction.mode==jkqtpmdaRuler) ||
              (currentMouseDragAction.mode==jkqtpmdaPanPlotOnMove) ||
              (currentMouseDragAction.mode==jkqtpmdaPanPlotOnRelease) ||
              (currentMouseDragAction.mode==jkqtpmdaDrawLineForEvent) ) &&
                mouseDragingRectangle)
        {
            if (currentMouseDragAction.mode==jkqtpmdaScribbleForEvents || currentMouseDragAction.mode==jkqtpmdaPanPlotOnMove) {
                // start is last event position
                mouseDragRectXStart=mouseDragRectXEnd;
                mouseDragRectYStart=mouseDragRectYEnd;
                mouseDragRectXStartPixel=mouseDragRectXEndPixel;
                mouseDragRectYStartPixel=mouseDragRectYEndPixel;
             }
            mouseDragRectXEnd=plotter->p2x(event->pos().x()/magnification);
            mouseDragRectYEnd=plotter->p2y((event->pos().y()-getPlotYOffset())/magnification);
            mouseDragRectXEndPixel=event->pos().x();
            mouseDragRectYEndPixel=event->pos().y();


            if (currentMouseDragAction.mode==jkqtpmdaToolTipForClosestDataPoint) {
                fillInternalStructForToolTipOfClosestDataPoint(mouseDragRectXEnd, mouseDragRectYEnd);

            }
            if (currentMouseDragAction.mode==jkqtpmdaRuler) {
                emit rulerDisplayed(mouseDragRectXStart, mouseDragRectYStart, mouseDragRectXEnd, mouseDragRectYEnd, event->modifiers());

            }

            paintUserAction();
            event->accept();
            //std::cout<<mouseZoomingTStart<<" -- "<<mouseZoomingTEnd<<std::endl;
            if (currentMouseDragAction.mode==jkqtpmdaZoomByRectangle) {
                double xmin=mouseDragRectXStart;
                double xmax=mouseDragRectXEnd;
                double ymin=mouseDragRectYStart;
                double ymax=mouseDragRectYEnd;
                plotter->correctXYRangeForAspectRatio(xmin,xmax,ymin,ymax);
                emit plotNewZoomRectangle(xmin,xmax,ymin,ymax, event->modifiers());
                //emit plotNewZoomRectangle(mouseDragRectXStart, mouseDragRectXEnd, mouseDragRectYStart, mouseDragRectYEnd, event->modifiers());
            }
            if ((currentMouseDragAction.mode==jkqtpmdaScribbleForEvents) && (jkqtp_approximatelyUnequal(mouseDragRectXStart,mouseDragRectXEnd) || jkqtp_approximatelyUnequal(mouseDragRectYStart,mouseDragRectYEnd)) ) {
                emit userScribbleClick(mouseDragRectXEnd, mouseDragRectYEnd, event->modifiers(), false, false);
            }
            if ((currentMouseDragAction.mode==jkqtpmdaPanPlotOnMove) && (jkqtp_approximatelyUnequal(mouseDragRectXStart,mouseDragRectXEnd) || jkqtp_approximatelyUnequal(mouseDragRectYStart,mouseDragRectYEnd)) ) {
                QRectF zoomRect= QRectF(QPointF(plotter->x2p(getXAxis()->getMin()),plotter->y2p(getYAxis()->getMax())), QPointF(plotter->x2p(getXAxis()->getMax()),plotter->y2p(getYAxis()->getMin())));
                if  ( (mouseLastClickX/magnification<plotter->getInternalPlotBorderLeft()) || (mouseLastClickX/magnification>plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft()) ) {
                    zoomRect.translate(0, mouseDragRectYStartPixel-mouseDragRectYEndPixel);
                } else if (((mouseLastClickY-getPlotYOffset())/magnification<plotter->getInternalPlotBorderTop()) || ((mouseLastClickY-getPlotYOffset())/magnification>plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
                    zoomRect.translate(mouseDragRectXStartPixel-mouseDragRectXEndPixel, 0);
                } else {
                    zoomRect.translate(mouseDragRectXStartPixel-mouseDragRectXEndPixel, mouseDragRectYStartPixel-mouseDragRectYEndPixel);
                }
                setXY(plotter->p2x(zoomRect.left()), plotter->p2x(zoomRect.right()), plotter->p2y(zoomRect.bottom()), plotter->p2y(zoomRect.top()));
            }

        } else {
            event->accept();
            /*if (emitSignals)*/ //emit plotMouseMove(x, y);
        }
    } else if (event->buttons()==Qt::NoButton) {
        bool foundIT=false;
        auto actionIT=findMatchingMouseMoveAction(event->modifiers(), &foundIT);
        if (foundIT) {
            // we found a matching action
            currentMouseMoveAction.insert(actionIT.value());
            if(actionIT.value()==jkqtpmmaToolTipForClosestDataPoint){
                mouseDragRectXStart=plotter->p2x(event->pos().x()/magnification);
                mouseDragRectYStart=plotter->p2y((event->pos().y()-getPlotYOffset())/magnification);
                mouseDragRectXStartPixel=event->pos().x();
                mouseDragRectYStartPixel=event->pos().y();
                fillInternalStructForToolTipOfClosestDataPoint(plotter->p2x(event->pos().x()/magnification), plotter->p2y((event->pos().y()-getPlotYOffset())/magnification));
            }
            paintUserAction();
        }
    }

    // emit move signal, if event occured inside plot only
    if  ( (event->pos().x()/magnification>=plotter->getInternalPlotBorderLeft()) && (event->pos().x()/magnification<=plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft())  &&
          ((event->pos().y()-getPlotYOffset())/magnification>=plotter->getInternalPlotBorderTop()) && ((event->pos().y()-getPlotYOffset())/magnification<=plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
        emit plotMouseMove(plotter->p2x(event->pos().x()/magnification), plotter->p2y((event->pos().y()-getPlotYOffset())/magnification));
    }
    updateCursor();
}



void JKQTPlotter::mousePressEvent ( QMouseEvent * event ){
    currentMouseDragAction.clear();
    mouseDragMarkers.clear();

    bool foundIT=false;
    auto actionIT=findMatchingMouseDragAction(event->button(), event->modifiers(), &foundIT);
    if (foundIT) {
        // we found a matching action
        mouseLastClickX=event->pos().x();
        mouseLastClickY=event->pos().y();
        mouseDragRectXStart=plotter->p2x(event->pos().x()/magnification);
        mouseDragRectYStart=plotter->p2y((event->pos().y()-getPlotYOffset())/magnification);
        mouseDragRectXEndPixel=mouseDragRectXStartPixel=event->pos().x();
        mouseDragRectYEndPixel=mouseDragRectYStartPixel=event->pos().y();
        currentMouseDragAction=MouseDragAction(actionIT.key().first, actionIT.key().second, actionIT.value());
        mouseDragingRectangle=true;
        oldImage=image;
        if (currentMouseDragAction.mode==jkqtpmdaScribbleForEvents) emit userScribbleClick(mouseDragRectXStart, mouseDragRectYStart, event->modifiers(), true, false);
        event->accept();
    } else if (event->button()==Qt::RightButton && event->modifiers()==Qt::NoModifier && contextMenuMode!=jkqtpcmmNoContextMenu) {
        mouseLastClickX=event->pos().x();
        mouseLastClickY=event->pos().y();
        openContextMenu(event->pos().x(), event->pos().y());
        event->accept();
    }

    // emit clicked signal, if event occured inside plot only
    if  ( (event->pos().x()/magnification>=plotter->getInternalPlotBorderLeft()) && (event->pos().x()/magnification<=plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft())  &&
          ((event->pos().y()-getPlotYOffset())/magnification>=plotter->getInternalPlotBorderTop()) && ((event->pos().y()-getPlotYOffset())/magnification<=plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
        emit plotMouseClicked(plotter->p2x(event->pos().x()/magnification), plotter->p2y((event->pos().y()-getPlotYOffset())/magnification), event->modifiers(), event->button());
        event->accept();
    }
    updateCursor();
    if (foundIT && actionIT.value()==jkqtpmdaToolTipForClosestDataPoint) {
        fillInternalStructForToolTipOfClosestDataPoint(mouseDragRectXStart, mouseDragRectYStart);
        paintUserAction();
    }
}

void JKQTPlotter::mouseReleaseEvent ( QMouseEvent * event ){
    if ((event->flags()&Qt::MouseEventCreatedDoubleClick)==Qt::MouseEventCreatedDoubleClick) {
        return;
    }
    if (currentMouseDragAction.isValid()) {
        mouseDragRectXEnd=plotter->p2x(event->pos().x()/magnification);
        mouseDragRectYEnd=plotter->p2y((event->pos().y()-getPlotYOffset())/magnification);
        mouseDragRectXEndPixel=event->pos().x();
        mouseDragRectYEndPixel=event->pos().y();
        image=oldImage;
        //update();
        mouseDragingRectangle=false;


        double x1=mouseDragRectXStart;
        double y1=mouseDragRectYStart;
        double x2=mouseDragRectXEnd;
        double y2=mouseDragRectYEnd;

        if (jkqtp_approximatelyUnequal(mouseDragRectXStart,mouseDragRectXEnd) && jkqtp_approximatelyUnequal(mouseDragRectYStart,mouseDragRectYEnd)) {
            if (currentMouseDragAction.mode==jkqtpmdaZoomByRectangle) {
                double xmin=mouseDragRectXStart;
                double xmax=mouseDragRectXEnd;

                double ymin=mouseDragRectYStart;
                double ymax=mouseDragRectYEnd;


                plotter->correctXYRangeForAspectRatio(xmin,xmax,ymin,ymax);

                emit zoomChangedLocally(xmin, xmax, ymin, ymax, this);
                plotter->setXY(xmin, xmax, ymin, ymax);
            } else if (currentMouseDragAction.mode==jkqtpmdaPanPlotOnRelease) {
                QRectF zoomRect= QRectF(QPointF(plotter->x2p(getXAxis()->getMin()),plotter->y2p(getYAxis()->getMax())), QPointF(plotter->x2p(getXAxis()->getMax()),plotter->y2p(getYAxis()->getMin())));
                if  ( (mouseLastClickX/magnification<plotter->getInternalPlotBorderLeft()) || (mouseLastClickX/magnification>plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft()) ) {
                    zoomRect.translate(0, mouseDragRectYStartPixel-mouseDragRectYEndPixel);
                } else if (((mouseLastClickY-getPlotYOffset())/magnification<plotter->getInternalPlotBorderTop()) || ((mouseLastClickY-getPlotYOffset())/magnification>plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
                    zoomRect.translate(mouseDragRectXStartPixel-mouseDragRectXEndPixel, 0);
                } else {
                    zoomRect.translate(mouseDragRectXStartPixel-mouseDragRectXEndPixel, mouseDragRectYStartPixel-mouseDragRectYEndPixel);
                }
                setXY(plotter->p2x(zoomRect.left()), plotter->p2x(zoomRect.right()), plotter->p2y(zoomRect.bottom()), plotter->p2y(zoomRect.top()));
            } else if (currentMouseDragAction.mode==jkqtpmdaDrawRectangleForEvent) {
                emit userRectangleFinished(x1, y1, x2-x1, y2-y1, event->modifiers());
            } else if (currentMouseDragAction.mode==jkqtpmdaDrawCircleForEvent) {
                emit userCircleFinished(x1, y1, qMin(fabs(x2-x1), fabs(y2-y1)), event->modifiers());
            } else if (currentMouseDragAction.mode==jkqtpmdaDrawEllipseForEvent) {
                emit userEllipseFinished(x1, y1, fabs(x2-x1), fabs(y2-y1), event->modifiers());
            } else if (currentMouseDragAction.mode==jkqtpmdaDrawLineForEvent) {
                emit userLineFinished(x1, y1, x2, y2, event->modifiers());
            }
        }
        if (currentMouseDragAction.mode!=jkqtpmdaZoomByRectangle && currentMouseDragAction.mode!=jkqtpmdaRuler && currentMouseDragAction.mode!=jkqtpmdaToolTipForClosestDataPoint) {
            update();
        }
        if (currentMouseDragAction.mode==jkqtpmdaScribbleForEvents) {
            emit userScribbleClick(x1, y1, event->modifiers(), false, true);
        }
        event->accept();
    }
    resetCurrentMouseDragAction();
}

void JKQTPlotter::resetCurrentMouseDragAction () {
    mouseDragingRectangle=false;
    currentMouseDragAction.clear();
    updateCursor();
}

void JKQTPlotter::mouseDoubleClickEvent ( QMouseEvent * event ){
    bool foundIT=false;
    JKQTPMouseDoubleClickActionsHashMapIterator itAction=findMatchingMouseDoubleClickAction(event->button(), event->modifiers(), &foundIT);
    if (foundIT)  {
        // we found an action to perform on this double-click
        if (itAction.value()==JKQTPMouseDoubleClickActions::jkqtpdcaClickOpensContextMenu) {
            openStandardContextMenu(event->pos().x(), event->pos().y());
        } else if (itAction.value()==JKQTPMouseDoubleClickActions::jkqtpdcaClickOpensSpecialContextMenu) {
            openSpecialContextMenu(event->pos().x(), event->pos().y());
        } else if (itAction.value()==JKQTPMouseDoubleClickActions::jkqtpdcaClickZoomsIn || itAction.value()==JKQTPMouseDoubleClickActions::jkqtpdcaClickZoomsOut) {
            double factor=4.0;
            if (itAction.value()==JKQTPMouseDoubleClickActions::jkqtpdcaClickZoomsOut) factor=1;

            double xmin=plotter->p2x(static_cast<double>(event->pos().x())/magnification-static_cast<double>(plotter->getPlotWidth())/factor);
            double xmax=plotter->p2x(static_cast<double>(event->pos().x())/magnification+static_cast<double>(plotter->getPlotWidth())/factor);
            double ymin=plotter->p2y(static_cast<double>(event->pos().y())/magnification-static_cast<double>(getPlotYOffset())+static_cast<double>(plotter->getPlotHeight())/factor);
            double ymax=plotter->p2y(static_cast<double>(event->pos().y())/magnification-static_cast<double>(getPlotYOffset())-static_cast<double>(plotter->getPlotHeight())/factor);
            if  ( (event->pos().x()/magnification<plotter->getInternalPlotBorderLeft()) || (event->pos().x()/magnification>plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft()) ) {
                xmin=getXMin();
                xmax=getXMax();
            } else if (((event->pos().y()-getPlotYOffset())/magnification<plotter->getInternalPlotBorderTop()) || ((event->pos().y()-getPlotYOffset())/magnification>plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
                ymin=getYMin();
                ymax=getYMax();
            }
            plotter->setXY(xmin, xmax, ymin, ymax);
            update();
        } else if (itAction.value()==JKQTPMouseDoubleClickActions::jkqtpdcaClickMovesViewport) {
            QRectF zoomRect= QRectF(QPointF(plotter->x2p(getXAxis()->getMin()),plotter->y2p(getYAxis()->getMax())), QPointF(plotter->x2p(getXAxis()->getMax()),plotter->y2p(getYAxis()->getMin())));
            if  ( (event->pos().x()/magnification<plotter->getInternalPlotBorderLeft()) || (event->pos().x()/magnification>plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft()) ) {
                zoomRect.moveCenter(QPointF(zoomRect.center().x(), event->pos().y()));
            } else if (((event->pos().y()-getPlotYOffset())/magnification<plotter->getInternalPlotBorderTop()) || ((event->pos().y()-getPlotYOffset())/magnification>plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
                zoomRect.moveCenter(QPointF(event->pos().x(), zoomRect.center().y()));
            } else {
                zoomRect.moveCenter(QPointF(event->pos().x(), event->pos().y()));
            }
            setXY(plotter->p2x(zoomRect.left()), plotter->p2x(zoomRect.right()), plotter->p2y(zoomRect.bottom()), plotter->p2y(zoomRect.top()));
        }
    }

    // only react on double clicks inside the widget
    if  ( (event->pos().x()/magnification>=plotter->getInternalPlotBorderLeft()) && (event->pos().x()/magnification<=plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft())  &&
          ((event->pos().y()-getPlotYOffset())/magnification>=plotter->getInternalPlotBorderTop()) && ((event->pos().y()-getPlotYOffset())/magnification<=plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {

        mouseLastClickX=event->pos().x();
        mouseLastClickY=event->pos().y();

        emit plotMouseDoubleClicked(plotter->p2x(event->pos().x()/magnification), plotter->p2y((event->pos().y()-getPlotYOffset())/magnification), event->modifiers(), event->button());

    }
    event->accept();
    updateCursor();
    currentMouseDragAction.clear();
}

void JKQTPlotter::keyReleaseEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    if (event->key()==Qt::Key_Escape && event->modifiers()==Qt::NoModifier) {
        if (mouseDragingRectangle || currentMouseDragAction.isValid()) {
            mouseDragingRectangle=false;
            image=oldImage;
            currentMouseDragAction.clear();
            update();
            event->accept();
        }
    }
    updateCursor();
}

void JKQTPlotter::wheelEvent ( QWheelEvent * event ) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    const double wheel_x=event->position().x();
    const double wheel_y=event->position().y();
#else
    const int wheel_x=event->pos().x();
    const int wheel_y=event->pos().y();
#endif

    //qDebug()<<"wheelEvent("<<event->modifiers()<<"): plotterStyle.registeredMouseWheelActions="<<plotterStyle.registeredMouseWheelActions;
    bool foundIT=false;
    auto itAction=findMatchingMouseWheelAction(event->modifiers(), &foundIT);
    //qDebug()<<"wheelEvent("<<event->modifiers()<<"): plotterStyle.registeredMouseWheelActions="<<plotterStyle.registeredMouseWheelActions;
    //qDebug()<<"wheelEvent("<<event->modifiers()<<"): itAction="<<itAction.key()<<","<<itAction.value()<<"  !=end:"<<(itAction!=plotterStyle.registeredMouseWheelActions.end())<<"  ==end:"<<(itAction==plotterStyle.registeredMouseWheelActions.end());

    if (foundIT) {
        if (itAction.value()==JKQTPMouseWheelActions::jkqtpmwaZoomByWheel) {
        //if (act==JKQTPMouseWheelActions::jkqtpmwaZoomByWheel) {
            //qDebug()<<"wheelEvent("<<event->modifiers()<<"):ZoomByWheel";
            const double factor=pow(2.0, 1.0*static_cast<double>(event->angleDelta().y())/120.0)*2.0;
            double xmin=plotter->p2x(static_cast<double>(wheel_x)/magnification-static_cast<double>(plotter->getPlotWidth())/factor);
            double xmax=plotter->p2x(static_cast<double>(wheel_x)/magnification+static_cast<double>(plotter->getPlotWidth())/factor);
            double ymin=plotter->p2y(static_cast<double>(wheel_y)/magnification-static_cast<double>(getPlotYOffset())+static_cast<double>(plotter->getPlotHeight())/factor);
            double ymax=plotter->p2y(static_cast<double>(wheel_y)/magnification-static_cast<double>(getPlotYOffset())-static_cast<double>(plotter->getPlotHeight())/factor);
            if  ( (wheel_x/magnification<plotter->getInternalPlotBorderLeft()) || (wheel_x/magnification>plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft()) ) {
                xmin=getXMin();
                xmax=getXMax();
            } else if (((wheel_y-getPlotYOffset())/magnification<plotter->getInternalPlotBorderTop()) || ((wheel_y-getPlotYOffset())/magnification>plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
                ymin=getYMin();
                ymax=getYMax();
            }
            plotter->setXY(xmin, xmax, ymin, ymax);
        } else if (itAction.value()==JKQTPMouseWheelActions::jkqtpmwaPanByWheel) {
        //} else if (act==JKQTPMouseWheelActions::jkqtpmwaPanByWheel) {
            //qDebug()<<"wheelEvent("<<event->modifiers()<<"):PanByWheel";
            QRectF zoomRect= QRectF(QPointF(plotter->x2p(getXAxis()->getMin()),plotter->y2p(getYAxis()->getMax())), QPointF(plotter->x2p(getXAxis()->getMax()),plotter->y2p(getYAxis()->getMin())));
            QPointF d=QPointF(event->angleDelta().x()/120.0*zoomRect.width()/10.0,
                              event->angleDelta().y()/120.0*zoomRect.height()/10.0);
            if (d.x()<-100) d.setX(-100);
            if (d.x()>100) d.setX(100);
            if (d.y()<-100) d.setY(-100);
            if (d.y()>100) d.setY(100);
            if (d.x()>=0 && d.x()<10) d.setX(10);
            if (d.x()<0 && d.x()>-10) d.setX(-10);
            if (d.y()>=0 && d.y()<10) d.setY(10);
            if (d.y()<0 && d.y()>-10) d.setY(-10);
            if  ( (wheel_x/magnification<plotter->getInternalPlotBorderLeft()) || (wheel_x/magnification>plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft()) ) {
                zoomRect.translate(0, d.y());
            } else if (((wheel_y-getPlotYOffset())/magnification<plotter->getInternalPlotBorderTop()) || ((wheel_y-getPlotYOffset())/magnification>plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
                zoomRect.translate(d.x(), 0);
            } else {
                zoomRect.translate(d.x(), d.y());
            }
            setXY(plotter->p2x(zoomRect.left()), plotter->p2x(zoomRect.right()), plotter->p2y(zoomRect.bottom()), plotter->p2y(zoomRect.top()));
        }
    }


    event->accept();

    emit plotMouseWheelOperated(plotter->p2x(wheel_x), plotter->p2x(wheel_y), event->modifiers(), event->angleDelta().x(), event->angleDelta().y());

    updateCursor();
    currentMouseDragAction.clear();
}

int JKQTPlotter::getPlotYOffset() {
     int plotYOffset=0;
     if (plotterStyle.displayMousePosition) {
        plotYOffset=plotYOffset+QFontMetrics(font()).height()+2;
     }
     if (plotterStyle.toolbarEnabled && plotterStyle.toolbarAlwaysOn) {
         plotYOffset=plotYOffset+toolbar->height();
     }
     return plotYOffset;
}

void JKQTPlotter::initContextMenu()
{
    resetContextMenu(true);

    contextMenu->addAction(plotter->getActionSaveData());
    contextMenu->addAction(plotter->getActionSavePlot());
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    contextMenu->addAction(plotter->getActionPrint());
#endif
    contextMenu->addSeparator();
    contextMenu->addAction(plotter->getActionCopyPixelImage());
    contextMenu->addAction(plotter->getActionCopyData());
    contextMenu->addAction(plotter->getActionCopyMatlab());
    contextMenu->addSeparator();
    contextMenu->addAction(plotter->getActionShowPlotData());
    contextMenu->addSection(tr("Zooming"));
    contextMenu->addAction(plotter->getActionZoomAll());
    contextMenu->addAction(plotter->getActionZoomIn());
    contextMenu->addAction(plotter->getActionZoomOut());
    contextMenu->addSeparator();
    contextMenu->addAction(actMouseMoveToolTip);
    contextMenu->addSection(tr("left mouse button tool"));
    contextMenu->addAction(actMouseLeftAsDefault);
    contextMenu->addAction(actMouseLeftAsPanView);
    contextMenu->addAction(actMouseLeftAsZoomRect);
    contextMenu->addAction(actMouseLeftAsRuler);
    contextMenu->addSeparator();
    QMenu* menVisibleGroup=new QMenu(tr("Graph Visibility"), contextMenu);
    for (size_t i=0; i<getPlotter()->getGraphCount(); i++) {
        QString tit=getPlotter()->getGraph(i)->getTitle();
        if (tit.isEmpty()) tit=tr("Graph %1").arg(static_cast<int>(i));
        QAction* act=new QAction(tit, menVisibleGroup);
        act->setCheckable(true);
        act->setChecked(getPlotter()->getGraph(i)->isVisible());
        act->setIcon(QIcon(QPixmap::fromImage(getPlotter()->getGraph(i)->generateKeyMarker(QSize(16,16)))));
        act->setData(static_cast<int>(i));
        connect(act, SIGNAL(toggled(bool)), this, SLOT(reactGraphVisible(bool)));
        menVisibleGroup->addAction(act);
    }
    contextMenu->addMenu(menVisibleGroup);

    if (actions().size()>0) {
        contextMenu->addSeparator();
        contextMenu->addActions(actions());
    }

    bool hasSep=false;
    JKQTBasePlotter::AdditionalActionsMap lst=getPlotter()->getLstAdditionalPlotterActions();
    JKQTBasePlotter::AdditionalActionsMapIterator it(lst);
    while (it.hasNext()) {
        it.next();
        if (it.value().size()>0) {
            bool hasMenu=false;
            for (int i=0; i<it.value().size(); i++) {
                if (it.value().at(i)) {
                    if (!hasMenu) {
                        contextSubMenus.append(new QMenu(it.key(), this));
                        if (!hasSep) {
                            contextMenu->addSeparator();
                            hasSep=true;
                        }
                        hasMenu=true;
                    }
                    contextSubMenus.last()->addAction(it.value().at(i));
                }
            }
            if (hasMenu) {
                contextMenu->addMenu(contextSubMenus.last());
            }
        }
    }


    modifyContextMenu(contextMenu);
}


void JKQTPlotter::updateCursor() {
    if (!currentMouseDragAction.isValid()) {
        if (currentMouseMoveAction.contains(jkqtpmmaToolTipForClosestDataPoint)) {
            setCursor(QCursor(Qt::CrossCursor));
        } else {
            setCursor(QCursor(Qt::ArrowCursor));
        }
    } else {
        if (currentMouseDragAction.mode==jkqtpmdaZoomByRectangle) {
            static QBitmap cursor(":/JKQTPlotter/jkqtp_cursor_zoom.png");
            static QBitmap mask(":/JKQTPlotter/jkqtp_cursor_zoom_mask.png");
            setCursor(QCursor(cursor, mask, 9, 14));
        } else if (currentMouseDragAction.mode==jkqtpmdaDrawRectangleForEvent) {
            static QBitmap cursor(":/JKQTPlotter/jkqtp_cursor_rectangle.png");
            static QBitmap mask(":/JKQTPlotter/jkqtp_cursor_rectangle_mask.png");
            setCursor(QCursor(cursor, mask, 9, 14));
        } else if (currentMouseDragAction.mode==jkqtpmdaPanPlotOnMove || currentMouseDragAction.mode==jkqtpmdaPanPlotOnRelease) {
            setCursor(QCursor(Qt::ClosedHandCursor));
        } else if (currentMouseDragAction.mode==jkqtpmdaDrawCircleForEvent) {
            static QBitmap cursor(":/JKQTPlotter/jkqtp_cursor_circle.png");
            static QBitmap mask(":/JKQTPlotter/jkqtp_cursor_circle_mask.png");
            setCursor(QCursor(cursor, mask, 9, 14));
        } else if (currentMouseDragAction.mode==jkqtpmdaDrawEllipseForEvent) {
            static QBitmap cursor(":/JKQTPlotter/jkqtp_cursor_ellipse.png");
            static QBitmap mask(":/JKQTPlotter/jkqtp_cursor_ellipse_mask.png");
            setCursor(QCursor(cursor, mask, 9, 14));
        } else if (currentMouseDragAction.mode==jkqtpmdaDrawLineForEvent) {
            static QBitmap cursor(":/JKQTPlotter/jkqtp_cursor_line.png");
            static QBitmap mask(":/JKQTPlotter/jkqtp_cursor_line_mask.png");
            setCursor(QCursor(cursor, mask, 9, 14));
        } else if (currentMouseDragAction.mode==jkqtpmdaScribbleForEvents) {
            static QBitmap cursor(":/JKQTPlotter/jkqtp_cursor_scribble.png");
            static QBitmap mask(":/JKQTPlotter/jkqtp_cursor_scribble_mask.png");
            setCursor(QCursor(cursor, mask, 9, 14));
        } else if (currentMouseDragAction.mode==jkqtpmdaToolTipForClosestDataPoint) {
            setCursor(QCursor(Qt::CrossCursor));
        } else if (currentMouseDragAction.mode==jkqtpmdaRuler) {
            static QBitmap cursor(":/JKQTPlotter/jkqtp_cursor_line.png");
            static QBitmap mask(":/JKQTPlotter/jkqtp_cursor_line_mask.png");
            setCursor(QCursor(cursor, mask, 9, 14));
        } else {
            setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}


void JKQTPlotter::synchronizeXAxis(double newxmin, double newxmax, double /*newymin*/, double /*newymax*/, JKQTPlotter * /*sender*/) {
    setX(newxmin, newxmax);
}

void JKQTPlotter::synchronizeYAxis(double /*newxmin*/, double /*newxmax*/, double newymin, double newymax, JKQTPlotter * /*sender*/) {
    setY(newymin, newymax);
}

void JKQTPlotter::synchronizeXYAxis(double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter * /*sender*/) {
    setXY(newxmin, newxmax, newymin, newymax);
}



void JKQTPlotter::redrawPlot() {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTPlotter::redrawPlot()"));
#endif
    if (!doDrawing) return;
    disconnect(plotter, SIGNAL(plotUpdated()), this, SLOT(redrawPlot()));
    plotter->setWidgetSize(jkqtp_roundTo<int>(width()/magnification), jkqtp_roundTo<int>(height()/magnification-getPlotYOffset()));
    JKQTPEnhancedPainter painter(&image);
    if (painter.isActive()) {
        painter.scale(magnification, magnification);
        //QTime t;
        //t.start();
        plotter->drawNonGrid(painter, QPoint(0,0));//, QRect(QPoint(0,0), QSize(plotter->getPlotWidth(), plotter->getPlotHeight())));
        //qDebug()<<"drawNonGrid"<<objectName()<<": "<<t.elapsed()<<"ms";
    }
    oldImage=image;
    connect(plotter, SIGNAL(plotUpdated()), this, SLOT(redrawPlot()));
    update();
}




void JKQTPlotter::paintEvent(QPaintEvent *event){
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTPlotter::paintEvent()"));
#endif
    // draw the saved image of the plot
    JKQTPEnhancedPainter* p = new JKQTPEnhancedPainter(this);

    if (p->isActive()) {

        //std::cout<<"getPlotYOffset()="<<getPlotYOffset()<<std::endl;
        p->save();
        p->setBrush(palette().brush(QPalette::Window));
        p->setPen(palette().color(QPalette::Window));
        p->drawRect(geometry());
        p->restore();
        if (plotterStyle.displayMousePosition ) {
            p->drawText(QPointF(plotter->getInternalPlotBorderLeft(), getPlotYOffset()-1), plotterStyle.mousePositionTemplate.arg(mousePosX).arg(mousePosY));
        }

        int plotImageWidth=width();
        int plotImageHeight=height();

        plotImageHeight=plotImageHeight-getPlotYOffset();
        if (image.width()!=plotImageWidth || image.height()!=plotImageHeight)     {
            p->drawImage(QRectF(0, getPlotYOffset(), plotImageWidth, plotImageHeight), image);
        } else {
            p->drawImage(QPoint(0, getPlotYOffset()), image);
        }
    }
    delete p;
    event->accept();
}

void JKQTPlotter::resizeEvent(QResizeEvent *event) {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTPlotter::resizeEvent()"));
#endif
    //qDebug()<<"resizeEvent  old="<<size()<<"   new="<<event->size();
     QWidget::resizeEvent(event);
     event->accept();
     bool sizeChanged=false;
     int plotImageWidth=width();
     int plotImageHeight=height();

     plotImageHeight=plotImageHeight-getPlotYOffset();
     if (plotImageWidth != image.width() || plotImageHeight != image.height()) {
         sizeChanged=true;
     }
     if (sizeChanged) {
         resizeTimer.setSingleShot(true);
         resizeTimer.start(jkqtp_RESIZE_DELAY);
     }

     //updateGeometry();
     //qDebug()<<"resize DONE width()="<<width()<<" height()="<<height()<<" plotImageWidth="<<plotImageWidth<<" plotImageHeight="<<plotImageHeight<<" sizeChanged="<<sizeChanged<<"\n";
}

void JKQTPlotter::delayedResizeEvent()
{
    qreal dpr = devicePixelRatioF();
    int plotImageWidth=width() * dpr;
    int plotImageHeight=height() * dpr;

    plotImageHeight=plotImageHeight-getPlotYOffset();
    //qDebug()<<"resize: "<<plotImageWidth<<" x "<<plotImageHeight<<std::endl;
    bool sizeChanged=false;
    if (plotImageWidth != image.width() || plotImageHeight != image.height()) {

        QImage newImage(QSize(plotImageWidth, plotImageHeight), QImage::Format_ARGB32);
        newImage.setDevicePixelRatio(dpr);

        image=newImage;
        sizeChanged=true;
    }

    if (sizeChanged) {
        emit widgetResized(width(), height(), this);
        redrawPlot();
    }
}

void JKQTPlotter::leaveEvent(QEvent * /*event*/) {
    if (!plotterStyle.toolbarAlwaysOn) toolbar->hide();
}

void JKQTPlotter::updateToolbar()
{
    toolbar->setIconSize(QSize(plotterStyle.toolbarIconSize, plotterStyle.toolbarIconSize));
    if (plotterStyle.toolbarEnabled) {
        if (plotterStyle.toolbarAlwaysOn) {
            toolbar->setToolbarVanishesEnabled(false);
            toolbar->show();
            toolbar->move(1,1);
        } else {
            toolbar->setToolbarVanishesEnabled(true);
        }
    } else {
        toolbar->hide();
    }
    update();
}


QSize JKQTPlotter::minimumSizeHint() const {
    QSize m=QWidget::minimumSizeHint();

    if (minSize.width()>m.width()) m.setWidth(minSize.width());
    if (minSize.height()>m.height()) m.setHeight(minSize.height());
    return m;
}

QSize JKQTPlotter::sizeHint() const {
    return QWidget::sizeHint();
}

void JKQTPlotter::synchronizeToMaster(JKQTPlotter *master, JKQTBasePlotter::SynchronizationDirection synchronizeDirection, bool synchronizeAxisLength, bool synchronizeZoomingMasterToSlave, bool synchronizeZoomingSlaveToMaster)
{
    if (masterPlotterX) disconnect(masterPlotterX->getPlotter(), SIGNAL(plotScalingRecalculated()), this, SLOT(masterPlotScalingRecalculated()));
    if (masterPlotterY) disconnect(masterPlotterY->getPlotter(), SIGNAL(plotScalingRecalculated()), this, SLOT(masterPlotScalingRecalculated()));

    plotter->synchronizeToMaster(master->getPlotter(), synchronizeDirection, synchronizeAxisLength, synchronizeZoomingMasterToSlave, synchronizeZoomingSlaveToMaster);

    if (synchronizeDirection==JKQTBasePlotter::sdXAxis || synchronizeDirection==JKQTBasePlotter::sdXYAxes) {
        masterPlotterX=master;
        if (masterPlotterX) connect(masterPlotterX->getPlotter(), SIGNAL(plotScalingRecalculated()), this, SLOT(masterPlotScalingRecalculated()));
    }
    if (synchronizeDirection==JKQTBasePlotter::sdYAxis || synchronizeDirection==JKQTBasePlotter::sdXYAxes) {
        masterPlotterY=master;
        if (masterPlotterY) connect(masterPlotterY->getPlotter(), SIGNAL(plotScalingRecalculated()), this, SLOT(masterPlotScalingRecalculated()));
    }

    redrawPlot();
}

void JKQTPlotter::synchronizeXToMaster(JKQTPlotter *master, bool synchronizeAxisLength, bool synchronizeZoomingMasterToSlave, bool synchronizeZoomingSlaveToMaster)
{
    synchronizeToMaster(master, JKQTBasePlotter::sdXAxis, synchronizeAxisLength, synchronizeZoomingMasterToSlave, synchronizeZoomingSlaveToMaster);
}

void JKQTPlotter::synchronizeYToMaster(JKQTPlotter *master, bool synchronizeAxisLength, bool synchronizeZoomingMasterToSlave, bool synchronizeZoomingSlaveToMaster)
{
    synchronizeToMaster(master, JKQTBasePlotter::sdYAxis, synchronizeAxisLength, synchronizeZoomingMasterToSlave, synchronizeZoomingSlaveToMaster);
}

void JKQTPlotter::resetMasterSynchronization(JKQTBasePlotter::SynchronizationDirection synchronizeDirection)
{
    synchronizeToMaster(nullptr, synchronizeDirection, false, false, false);
}

void JKQTPlotter::masterPlotScalingRecalculated() {
    //qDebug()<<"this="<<this<<", sender="<<sender()<<" --> masterPlotScalingRecalculated()";
    redrawPlot();
}

void JKQTPlotter::setGridPrinting(bool enabled)
{
    plotter->setGridPrinting(enabled);
}

void JKQTPlotter::addGridPrintingPlotter(size_t x, size_t y, JKQTPlotter *plotterOther)
{
    plotter->addGridPrintingPlotter(x,y,plotterOther->getPlotter());
}

void JKQTPlotter::clearGridPrintingPlotters()
{
    plotter->clearGridPrintingPlotters();
}

void JKQTPlotter::setGridPrintingCurrentX(size_t x)
{
    plotter->setGridPrintingCurrentX(x);
}

void JKQTPlotter::setGridPrintingCurrentY(size_t y)
{
    plotter->setGridPrintingCurrentY(y);
}

void JKQTPlotter::setGridPrintingCurrentPos(size_t x, size_t y)
{
    plotter->setGridPrintingCurrentPos(x,y);
}

bool JKQTPlotter::isPlotUpdateEnabled() const {
    return doDrawing;
}

void JKQTPlotter::pzoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQTBasePlotter* /*sender*/) {
    emit zoomChangedLocally(newxmin, newxmax, newymin, newymax, this);
    minSize=QSizeF(plotter->getInternalPlotBorderLeft()+plotter->getInternalPlotBorderRight()+10, plotter->getInternalPlotBorderTop()+plotter->getInternalPlotBorderBottom()+10).toSize();
}

void JKQTPlotter::intBeforePlotScalingRecalculate() {
    emit beforePlotScalingRecalculate();
}

void JKQTPlotter::reactGraphVisible(bool visible)
{
    QAction* act=dynamic_cast<QAction*>(sender());
    if (act) {
        getPlotter()->setGraphVisible(act->data().toInt(), visible);
    }
}

void JKQTPlotter::setMouseLeftActionAsPanView()
{
    setOverrideMouseDragAction(Qt::LeftButton, Qt::NoModifier, JKQTPMouseDragActions::jkqtpmdaPanPlotOnMove);
}

void JKQTPlotter::setMouseLeftActionAsZoomRect()
{
    setOverrideMouseDragAction(Qt::LeftButton, Qt::NoModifier, JKQTPMouseDragActions::jkqtpmdaZoomByRectangle);
}

void JKQTPlotter::setMouseLeftActionAsRuler()
{
    setOverrideMouseDragAction(Qt::LeftButton, Qt::NoModifier, JKQTPMouseDragActions::jkqtpmdaRuler);
}

void JKQTPlotter::setMouseLeftDragActionAsToolTip()
{
    setOverrideMouseDragAction(Qt::LeftButton, Qt::NoModifier, JKQTPMouseDragActions::jkqtpmdaToolTipForClosestDataPoint);
}

void JKQTPlotter::resetMouseLeftAction()
{
    resetOverrideMouseDragAction(Qt::LeftButton, Qt::NoModifier);
}

void JKQTPlotter::setMouseMoveActionAsToolTip(bool enabled)
{
    if (enabled) {
        registerMouseMoveAction(Qt::NoModifier, JKQTPMouseMoveActions::jkqtpmmaToolTipForClosestDataPoint);
    } else {
        deregisterMouseMoveAction(Qt::NoModifier);
    }
}

void JKQTPlotter::setMouseActionToolbarActionsActive(bool __value)
{
    actgrpMouseLeft->setVisible(__value);
    actMouseMoveToolTip->setVisible(__value);
}

QAction* JKQTPlotter::getActMouseLeftAsDefault()  {
    return actMouseLeftAsDefault;
}

QAction *JKQTPlotter::getActMouseLeftAsZoomRect()
{
    return actMouseLeftAsZoomRect;
}

QAction *JKQTPlotter::getActMouseLeftAsPanView()
{
    return actMouseLeftAsPanView;
}

QAction* JKQTPlotter::getActMouseLeftAsRuler()  {
    return actMouseLeftAsRuler;
}

const QAction* JKQTPlotter::getActMouseLeftAsDefault() const {
    return actMouseLeftAsDefault;
}

const QAction *JKQTPlotter::getActMouseLeftAsZoomRect() const
{
    return actMouseLeftAsZoomRect;
}

const QAction *JKQTPlotter::getActMouseLeftAsPanView() const
{
    return actMouseLeftAsPanView;
}

const QAction* JKQTPlotter::getActMouseLeftAsRuler() const {
    return actMouseLeftAsRuler;
}

const QAction* JKQTPlotter::getActMouseMoveToolTip() const {
    return actMouseMoveToolTip;
}

QAction* JKQTPlotter::getActMouseMoveToolTip() {
    return actMouseMoveToolTip;
}

void JKQTPlotter::setOverrideMouseDragAction(Qt::MouseButton button, Qt::KeyboardModifiers modifier, JKQTPMouseDragActions action)
{
    registeredOverrideMouseDragActionModes.insert(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifier), action);
}

void JKQTPlotter::resetOverrideMouseDragAction(Qt::MouseButton button, Qt::KeyboardModifiers modifier)
{
    registeredOverrideMouseDragActionModes.remove(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifier));
}

void JKQTPlotter::setContextMenuMode(JKQTPContextMenuModes mode) {
    contextMenuMode=mode;
}

QMenu *JKQTPlotter::getSpecialContextMenu() const {
    return this->menuSpecialContextMenu;
}

void JKQTPlotter::setSpecialContextMenu(QMenu *menu)
{
    menuSpecialContextMenu=menu;
    if (menuSpecialContextMenu) {
        menuSpecialContextMenu->setParent(this);
        menuSpecialContextMenu->close();
    }
}

double JKQTPlotter::getMouseContextX() const {
    return this->mouseContextX;
}

double JKQTPlotter::getMouseContextY() const {
    return this->mouseContextY;
}

int JKQTPlotter::getMouseLastClickX() const {
    return this->mouseLastClickX;
}

int JKQTPlotter::getMouseLastClickY() const {
    return this->mouseLastClickY;
}

JKQTPContextMenuModes JKQTPlotter::getContextMenuMode() const {
    return contextMenuMode;
}

const JKQTPlotterStyle &JKQTPlotter::getCurrentPlotterStyle() const
{
    return plotterStyle;
}

void JKQTPlotter::setCurrentPlotterStyle(const JKQTPlotterStyle &style)
{
    plotterStyle=style;
    updateToolbar();
    redrawPlot();
}

void JKQTPlotter::setCurrentPlotterStyle(const JKQTPlotterStyle &style, const JKQTBasePlotterStyle &baseStyle)
{
    plotterStyle=style;
    plotter->setCurrentPlotterStyle(baseStyle);
    updateToolbar();
    redrawPlot();
}

void JKQTPlotter::loadCurrentPlotterStyle(const QSettings &settings, const QString &group, bool alsoLoadBaseStyle)
{
    plotterStyle.loadSettings(settings, group);
    if (alsoLoadBaseStyle) plotter->loadCurrentPlotterStyle(settings, group);
    updateToolbar();
    redrawPlot();
}

void JKQTPlotter::saveCurrentPlotterStyle(QSettings &settings, const QString &group, bool alsoSaveBaseStyle) const
{
    plotterStyle.saveSettings(settings, group);
    if (alsoSaveBaseStyle) plotter->saveCurrentPlotterStyle(settings, group);
}

void JKQTPlotter::setMagnification(double m)
{
    magnification=m;
    redrawPlot();
}

void JKQTPlotter::modifyContextMenu(QMenu * /*menu*/)
{
}


void JKQTPlotter::populateToolbar(QToolBar *toolbar) const
{
    toolbar->addAction(plotter->getActionSaveData());
    toolbar->addAction(plotter->getActionSavePlot());
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    toolbar->addAction(plotter->getActionPrint());
#endif
    toolbar->addSeparator();
    toolbar->addAction(plotter->getActionCopyPixelImage());
    toolbar->addAction(plotter->getActionCopyData());
    toolbar->addAction(plotter->getActionCopyMatlab());
    toolbar->addSeparator();
    toolbar->addAction(plotter->getActionShowPlotData());
    toolbar->addSeparator();
    toolbar->addAction(plotter->getActionZoomAll());
    toolbar->addAction(plotter->getActionZoomIn());
    toolbar->addAction(plotter->getActionZoomOut());
    toolbar->addSeparator();
    toolbar->addAction(actMouseMoveToolTip);
    toolbar->addSeparator();
    toolbar->addAction(actMouseLeftAsDefault);
    toolbar->addAction(actMouseLeftAsPanView);
    toolbar->addAction(actMouseLeftAsZoomRect);
    toolbar->addAction(actMouseLeftAsRuler);
    if (actions().size()>0) {
        toolbar->addSeparator();
        toolbar->addActions(actions());
    }

}

void JKQTPlotter::openContextMenu()
{
    openContextMenu(mouseLastClickX, mouseLastClickY);

}

void JKQTPlotter::openContextMenu(int x, int y)
{
    if (contextMenuMode==JKQTPContextMenuModes::jkqtpcmmStandardContextMenu) {
        openStandardContextMenu(x,y);
    } else if (contextMenuMode==JKQTPContextMenuModes::jkqtpcmmSpecialContextMenu) {
        openSpecialContextMenu(x,y);
    } else if (contextMenuMode==JKQTPContextMenuModes::jkqtpcmmStandardAndSpecialContextMenu) {
        openStandardAndSpecialContextMenu(x,y);
    }
}


void JKQTPlotter::openStandardContextMenu()
{
    openStandardContextMenu(mouseLastClickX, mouseLastClickY);
}


void JKQTPlotter::resetContextMenu(bool createnew)
{
    if (contextMenu) {
        contextMenu->close();
        contextMenu->clear();
        qDeleteAll(contextSubMenus);
        contextSubMenus.clear();
        delete contextMenu;
    }
    if (createnew) {
        contextMenu=new QMenu(this);
    } else {
        contextMenu=nullptr;
    }
}

void JKQTPlotter::openStandardContextMenu(int x, int y)
{
    //qDebug()<<"openContextMenu("<<x<<y<<contextMenu<<")";
    mouseContextX=plotter->p2x(x/magnification);
    mouseContextY=plotter->p2y((y-getPlotYOffset())/magnification);
    initContextMenu();
    contextMenu->popup(mapToGlobal(QPoint(x,y)));
    //qDebug()<<" -> "<<mapToGlobal(QPoint(x,y))<<contextMenu->size()<<contextMenu->pos()<<contextMenu->parent();
    emit contextMenuOpened(mouseContextX, mouseContextY, contextMenu);
    //qDebug()<<"openContextMenu("<<x<<y<<contextMenu<<") ... DONE";
}

void JKQTPlotter::openSpecialContextMenu()
{
    openSpecialContextMenu(mouseLastClickX, mouseLastClickY);
}

void JKQTPlotter::openSpecialContextMenu(int x, int y)
{
    //qDebug()<<"openSpecialContextMenu("<<x<<y<<menuSpecialContextMenu<<")";
    if (menuSpecialContextMenu) {


        mouseContextX=plotter->p2x(x/magnification);
        mouseContextY=plotter->p2y((y-getPlotYOffset())/magnification);
        menuSpecialContextMenu->close();
        menuSpecialContextMenu->popup(mapToGlobal(QPoint(x,y)));
        menuSpecialContextMenu->resize(menuSpecialContextMenu->sizeHint());
        //qDebug()<<" -> "<<mapToGlobal(QPoint(x,y))<<menuSpecialContextMenu->size()<<menuSpecialContextMenu->pos()<<menuSpecialContextMenu->parent();
        emit contextMenuOpened(mouseContextX, mouseContextY, menuSpecialContextMenu);
        //qDebug()<<"openSpecialContextMenu("<<x<<y<<menuSpecialContextMenu<<") ... DONE";
    }
}

void JKQTPlotter::openStandardAndSpecialContextMenu()
{
    openStandardAndSpecialContextMenu(mouseLastClickX, mouseLastClickY);
}

void JKQTPlotter::openStandardAndSpecialContextMenu(int x, int y)
{
    //qDebug()<<"openContextMenu("<<x<<y<<contextMenu<<")";
    mouseContextX=plotter->p2x(x/magnification);
    mouseContextY=plotter->p2y((y-getPlotYOffset())/magnification);
    initContextMenu();

    if (menuSpecialContextMenu) {
        contextMenu->addSeparator();
        for (QAction* act: menuSpecialContextMenu->actions()) {
            contextMenu->addAction(act);
        }
    }

    contextMenu->popup(mapToGlobal(QPoint(x,y)));
    //qDebug()<<" -> "<<mapToGlobal(QPoint(x,y))<<contextMenu->size()<<contextMenu->pos()<<contextMenu->parent();
    emit contextMenuOpened(mouseContextX, mouseContextY, contextMenu);
    //qDebug()<<"openContextMenu("<<x<<y<<contextMenu<<") ... DONE";

}

JKQTPMouseDragActionsHashMapIterator JKQTPlotter::findMatchingMouseDragAction(Qt::MouseButton button, Qt::KeyboardModifiers modifiers, bool* found) const
{
    if (found) *found=false;
    JKQTPMouseDragActionsHashMapIterator it=registeredOverrideMouseDragActionModes.cbegin();
    while (it!=registeredOverrideMouseDragActionModes.cend() ) {
        if (it.key()==QPair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifiers)) {
            if (found) *found=true;
            return it;
        }
        ++it;
    }
    it=plotterStyle.registeredMouseDragActionModes.cbegin();
    while (it!=plotterStyle.registeredMouseDragActionModes.cend() ) {
        if (it.key()==QPair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifiers)) {
            if (found) *found=true;
            return it;
        }
        ++it;
    }
    return plotterStyle.registeredMouseDragActionModes.cend();
}

JKQTPMouseDoubleClickActionsHashMapIterator JKQTPlotter::findMatchingMouseDoubleClickAction(Qt::MouseButton button, Qt::KeyboardModifiers modifiers, bool*found) const
{
    if (found) *found=false;
    for (JKQTPMouseDoubleClickActionsHashMapIterator it=plotterStyle.registeredMouseDoubleClickActions.cbegin(); it!=plotterStyle.registeredMouseDoubleClickActions.cend(); ++it) {
        if (it.key()==QPair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifiers)) {
            if (found) *found=true;
            return it;
        }
    }
    return plotterStyle.registeredMouseDoubleClickActions.cend();
}

JKQTPMouseWheelActionsHashMapIterator JKQTPlotter::findMatchingMouseWheelAction(Qt::KeyboardModifiers modifiers, bool *found) const
{
    if (found) *found=false;
    for (JKQTPMouseWheelActionsHashMapIterator it=plotterStyle.registeredMouseWheelActions.begin(); it!=plotterStyle.registeredMouseWheelActions.end(); ++it) {
        if (it.key()==modifiers) {
            if (found) *found=true;
            return it;
        }
    }
    return plotterStyle.registeredMouseWheelActions.end();

    //return plotterStyle.registeredMouseWheelActions.find(modifiers);
}

JKQTPMouseMoveActionsHashMapIterator JKQTPlotter::findMatchingMouseMoveAction(Qt::KeyboardModifiers modifiers, bool *found) const
{
    if (found) *found=false;
    for (JKQTPMouseMoveActionsHashMapIterator it=plotterStyle.registeredMouseMoveActions.begin(); it!=plotterStyle.registeredMouseMoveActions.end(); ++it) {
        if (it.key()==modifiers) {
            if (found) *found=true;
            return it;
        }
    }
    return plotterStyle.registeredMouseMoveActions.end();

    //return plotterStyle.registeredMouseMoveActions.find(modifiers);
}

void JKQTPlotter::setPlotUpdateEnabled(bool enable)
{
    doDrawing=enable;
    plotter->setEmittingSignalsEnabled(enable);
    //qDebug()<<objectName()<<"  doDrawing="<<doDrawing;
}

void JKQTPlotter::registerMouseDragAction(Qt::MouseButton button, Qt::KeyboardModifiers modifier, JKQTPMouseDragActions action)
{
    plotterStyle.registeredMouseDragActionModes[QPair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifier)]=action;
    if (button==Qt::LeftButton && modifier==Qt::NoModifier) {
        actMouseLeftAsDefault->setChecked(true);
        resetMouseLeftAction();
    }
}

void JKQTPlotter::deregisterMouseDragAction(Qt::MouseButton button, Qt::KeyboardModifiers modifier)
{
    plotterStyle.registeredMouseDragActionModes.remove(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifier));
}

void JKQTPlotter::clearAllRegisteredMouseDragActions()
{
    plotterStyle.registeredMouseDragActionModes.clear();
}

void JKQTPlotter::registerMouseDoubleClickAction(Qt::MouseButton button, Qt::KeyboardModifiers modifier, JKQTPMouseDoubleClickActions action)
{
    plotterStyle.registeredMouseDoubleClickActions[QPair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifier)]=action;
}

void JKQTPlotter::deregisterMouseDoubleClickAction(Qt::MouseButton button, Qt::KeyboardModifiers modifier)
{
    plotterStyle.registeredMouseDoubleClickActions.remove(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifier));
}

void JKQTPlotter::clearAllRegisteredMouseDoubleClickActions()
{
    plotterStyle.registeredMouseDoubleClickActions.clear();
}


void initJKQTPlotterResources()
{
    initJKQTBasePlotterResources();
}

JKQTPlotter::MouseDragAction::MouseDragAction():
    mode(jkqtpmdaZoomByRectangle), modifier(Qt::NoModifier), mouseButton(Qt::LeftButton), valid(false)
{

}

JKQTPlotter::MouseDragAction::MouseDragAction(Qt::MouseButton _mouseButton, Qt::KeyboardModifiers _modifier, JKQTPMouseDragActions _mode):
    mode(_mode), modifier(_modifier), mouseButton(_mouseButton), valid(true)
{

}

bool JKQTPlotter::MouseDragAction::isValid() const {
    return valid;
}

void JKQTPlotter::MouseDragAction::clear()
{
    valid=false;
    mode=jkqtpmdaZoomByRectangle;
    modifier=Qt::NoModifier;
    mouseButton=Qt::LeftButton;
}
