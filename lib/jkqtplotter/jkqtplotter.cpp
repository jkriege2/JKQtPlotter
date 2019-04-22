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




#include <QFileInfo>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtGlobal>
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphsscatter.h"


#define jkqtp_RESIZE_DELAY 100






/**************************************************************************************************************************
 * JKQTPlotter
 **************************************************************************************************************************/
JKQTPlotter::JKQTPlotter(bool datastore_internal, QWidget* parent, JKQTPDatastore* datast):
    QWidget(parent, Qt::Widget),
    plotterStyle(JKQTPGetSystemDefaultStyle())
{    
    initJKQTPlotterResources();

    menujkqtpcmmSpecialContextMenu=nullptr;
    mouseContextX=0;
    mouseContextY=0;
    setParent(parent);
    connect(&resizeTimer, SIGNAL(timeout()), this, SLOT(delayedResizeEvent()));
    doDrawing=false;
    magnification=1;
    plotter=new JKQTBasePlotter(datastore_internal, this, datast);
    plotter->setEmittingSignalsEnabled(false);
    fixBasePlotterSettings();


    mousePosX=0;
    mousePosY=0;

    connect(plotter, SIGNAL(plotUpdated()), this, SLOT(redrawPlot()));
    connect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(redrawOverlays()));
    connect(plotter, SIGNAL(beforePlotScalingRecalculate()), this, SLOT(intBeforePlotScalingRecalculate()));
    connect(plotter, SIGNAL(zoomChangedLocally(double, double, double, double, JKQTBasePlotter*)), this, SLOT(pzoomChangedLocally(double, double, double, double, JKQTBasePlotter*)));

    image=QImage(width(), height(), QImage::Format_ARGB32);
    oldImage=image;
    imageNoOverlays=image;
    mouseDragingRectangle=false;

    // set default user-interactions:
    contextMenuMode=JKQTPContextMenuModes::jkqtpcmmStandardContextMenu;

    // enable mouse-tracking, so mouseMoved-Events can be caught
    setMouseTracking(true);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    toolbar=new JKVanishQToolBar(this);
    toolbar->clear();
    toolbar->move(1,1);
    toolbar->hide();
    toolbar->setAutoFillBackground(true);
    toolbar->addSeparator();
    toolbar->addSeparator();
    populateToolbar(toolbar);

    contextMenu=new QMenu(this);

    QSize s=QSize(plotterStyle.toolbarIconSize, plotterStyle.toolbarIconSize);
    toolbar->setIconSize(s);

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
    disconnect(plotter, SIGNAL(plotUpdated()), this, SLOT(redrawPlot()));
    disconnect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(redrawOverlays()));
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


void JKQTPlotter::setUserActionColor(const QColor &__value)
{
    if (plotterStyle.userActionColor != __value) {
        plotterStyle.userActionColor = __value;
        update();
    }
}

QColor JKQTPlotter::getUserActionColor() const
{
    return plotterStyle.userActionColor;
}

void JKQTPlotter::setUserActionCompositionMode(const QPainter::CompositionMode &__value)
{
    if (plotterStyle.userActionCompositionMode != __value) {
        plotterStyle.userActionCompositionMode = __value;
        update();
    }
}



void JKQTPlotter::registerMouseWheelAction(Qt::KeyboardModifiers modifier, JKQTPMouseWheelActions action)
{
    plotterStyle.registeredMouseWheelActions[modifier]=action;
}

void JKQTPlotter::deregisterMouseWheelAction(Qt::KeyboardModifiers modifier)
{
    plotterStyle.registeredMouseWheelActions.remove(modifier);
}

void JKQTPlotter::clearAllMouseWheelActions()
{
    plotterStyle.registeredMouseWheelActions.clear();
}


QPainter::CompositionMode JKQTPlotter::getUserActionCompositionMode() const
{
    return plotterStyle.userActionCompositionMode;
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


void JKQTPlotter::paintUserAction() {
    if (currentMouseDragAction.isValid() && mouseDragingRectangle) {
        image=oldImage;
        if (image.width()>0 && image.height()>0 && !image.isNull()) {
            JKQTPEnhancedPainter painter(&image);
            QPen p=painter.pen();
            p.setWidthF(1);
            p.setColor(plotterStyle.userActionColor.darker());
            p.setStyle(Qt::DashLine);
            painter.setPen(p);
            QPainter::CompositionMode oldCMode=painter.compositionMode();
            painter.setCompositionMode(plotterStyle.userActionCompositionMode);
            if ((mouseDragRectXEnd!=mouseDragRectXStart) && (mouseDragRectYEnd!=mouseDragRectYStart)) {
                double x1=plotter->x2p(mouseDragRectXStart)*magnification;
                double y1=plotter->y2p(mouseDragRectYStart)*magnification;
                double x2=plotter->x2p(mouseDragRectXEnd)*magnification;
                double y2=plotter->y2p(mouseDragRectYEnd)*magnification;
                double dx=x2-x1;
                double dy=y2-y1;
                if ((currentMouseDragAction.mode==jkqtpmdaZoomByRectangle) || (currentMouseDragAction.mode==jkqtpmdaDrawRectangleForEvent)) {
                    painter.setOpacity(0.2);
                    painter.fillRect(QRectF(x1, y1, x2-x1, y2-y1), QBrush(plotterStyle.userActionColor));
                    painter.setOpacity(1.0);
                    painter.drawRect(QRectF(x1, y1, x2-x1, y2-y1));
                } else if (currentMouseDragAction.mode==jkqtpmdaDrawCircleForEvent) {
                    QColor zc=plotterStyle.userActionColor;
                    zc.setAlphaF(0.2);
                    painter.setBrush(QBrush(zc));
                    painter.drawEllipse(QPointF(x1, y1), qMin(fabs(dx), fabs(dy)), qMin(fabs(dx), fabs(dy)));
                } else  if (currentMouseDragAction.mode==jkqtpmdaDrawEllipseForEvent) {
                    QColor zc=plotterStyle.userActionColor;
                    zc.setAlphaF(0.2);
                    painter.setBrush(QBrush(zc));
                    painter.drawEllipse(QPointF(x1, y1), fabs(dx), fabs(dy));
                } else  if (currentMouseDragAction.mode==jkqtpmdaDrawLineForEvent) {
                    QPen pp=p;
                    pp.setColor(plotterStyle.userActionColor);
                    painter.setPen(pp);
                    painter.drawLine(QPointF(x1,y1), QPointF(x2,y2));
                }
            }
            painter.setCompositionMode(oldCMode);
        }
        update();
    }
}


void JKQTPlotter::mouseMoveEvent ( QMouseEvent * event ) {
    if (plotterStyle.displayMousePosition) {
        mousePosX=plotter->p2x(event->x()/magnification);
        mousePosY=plotter->p2y((event->y()-getPlotYOffset())/magnification);
        update();
    }

    if (plotterStyle.toolbarEnabled && (!plotterStyle.toolbarAlwaysOn) && (!toolbar->isVisible())) { // decide whether to display toolbar
        int y1=10;
        if (event->y()/magnification>=0 && event->y()/magnification<=y1) {
            toolbar->show();
            toolbar->move(1,1);
        }
    }
    if (!plotterStyle.toolbarEnabled) {
        toolbar->hide();
    }
    if (currentMouseDragAction.isValid()) {
        if (( (currentMouseDragAction.mode==jkqtpmdaZoomByRectangle) ||
              (currentMouseDragAction.mode==jkqtpmdaDrawRectangleForEvent) ||
              (currentMouseDragAction.mode==jkqtpmdaDrawCircleForEvent) ||
              (currentMouseDragAction.mode==jkqtpmdaDrawEllipseForEvent) ||
              (currentMouseDragAction.mode==jkqtpmdaScribbleForEvents) ||
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
            mouseDragRectXEnd=plotter->p2x(event->x()/magnification);
            mouseDragRectYEnd=plotter->p2y((event->y()-getPlotYOffset())/magnification);
            mouseDragRectXEndPixel=event->x();
            mouseDragRectYEndPixel=event->y();
            paintUserAction();
            event->accept();
            //std::cout<<mouseZoomingTStart<<" -- "<<mouseZoomingTEnd<<std::endl;
            if (currentMouseDragAction.mode==jkqtpmdaZoomByRectangle) {
                emit plotNewZoomRectangle(mouseDragRectXStart, mouseDragRectXEnd, mouseDragRectYStart, mouseDragRectYEnd, event->modifiers());
            }
            if ((currentMouseDragAction.mode==jkqtpmdaScribbleForEvents) && ((mouseDragRectXStart!=mouseDragRectXEnd) || (mouseDragRectYStart!=mouseDragRectYEnd)) ) {
                emit userScribbleClick(mouseDragRectXEnd, mouseDragRectYEnd, event->modifiers(), false, false);
            }
            if ((currentMouseDragAction.mode==jkqtpmdaPanPlotOnMove) && ((mouseDragRectXStart!=mouseDragRectXEnd) || (mouseDragRectYStart!=mouseDragRectYEnd)) ) {
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
    }

    // emit clicked signal, if event occured inside plot only
    if  ( (event->x()/magnification>=plotter->getInternalPlotBorderLeft()) && (event->x()/magnification<=plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft())  &&
          ((event->y()-getPlotYOffset())/magnification>=plotter->getInternalPlotBorderTop()) && ((event->y()-getPlotYOffset())/magnification<=plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
        emit plotMouseMove(plotter->p2x(event->x()/magnification), plotter->p2y((event->y()-getPlotYOffset())/magnification));
    }
    updateCursor();
}




void JKQTPlotter::mousePressEvent ( QMouseEvent * event ){
    currentMouseDragAction.clear();

    auto actionIT=findMatchingMouseDragAction(event->button(), event->modifiers());
    if (actionIT!=plotterStyle.registeredMouseDragActionModes.end()) {
        // we found a matching action
        currentMouseDragAction=MouseDragAction(actionIT.key().first, actionIT.key().second, actionIT.value());
        mouseLastClickX=event->x();
        mouseLastClickY=event->y();
        mouseDragRectXStart=plotter->p2x(event->x()/magnification);
        mouseDragRectYStart=plotter->p2y((event->y()-getPlotYOffset())/magnification);
        mouseDragRectXEndPixel=mouseDragRectXStartPixel=event->x();
        mouseDragRectYEndPixel=mouseDragRectYStartPixel=event->y();
        mouseDragingRectangle=true;
        oldImage=image;
        if (currentMouseDragAction.mode==jkqtpmdaScribbleForEvents) emit userScribbleClick(mouseDragRectXStart, mouseDragRectYStart, event->modifiers(), true, false);
        event->accept();
    } else if (event->button()==Qt::RightButton && event->modifiers()==Qt::NoModifier && contextMenuMode!=jkqtpcmmNoContextMenu) {
        mouseLastClickX=event->x();
        mouseLastClickY=event->y();
        openContextMenu(event->x(), event->y());
        event->accept();
    }

    // emit clicked signal, if event occured inside plot only
    if  ( (event->x()/magnification>=plotter->getInternalPlotBorderLeft()) && (event->x()/magnification<=plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft())  &&
          ((event->y()-getPlotYOffset())/magnification>=plotter->getInternalPlotBorderTop()) && ((event->y()-getPlotYOffset())/magnification<=plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
        emit plotMouseClicked(plotter->p2x(event->x()/magnification), plotter->p2y((event->y()-getPlotYOffset())/magnification), event->modifiers(), event->button());
        event->accept();
    }
    updateCursor();
}

void JKQTPlotter::mouseReleaseEvent ( QMouseEvent * event ){
    if ((event->flags()&Qt::MouseEventCreatedDoubleClick)==Qt::MouseEventCreatedDoubleClick) {
        return;
    }
    if (currentMouseDragAction.isValid()) {
        mouseDragRectXEnd=plotter->p2x(event->x()/magnification);
        mouseDragRectYEnd=plotter->p2y((event->y()-getPlotYOffset())/magnification);
        mouseDragRectXEndPixel=event->x();
        mouseDragRectYEndPixel=event->y();
        image=oldImage;
        //update();
        mouseDragingRectangle=false;


        double x1=mouseDragRectXStart;
        double y1=mouseDragRectYStart;
        double x2=mouseDragRectXEnd;
        double y2=mouseDragRectYEnd;

        if ((mouseDragRectXStart!=mouseDragRectXEnd) && (mouseDragRectYStart!=mouseDragRectYEnd)) {
            if (currentMouseDragAction.mode==jkqtpmdaZoomByRectangle) {
                double xmin=mouseDragRectXStart;
                double xmax=mouseDragRectXEnd;

                double ymin=mouseDragRectYStart;
                double ymax=mouseDragRectYEnd;

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
        if (currentMouseDragAction.mode!=jkqtpmdaZoomByRectangle) update();
        if (currentMouseDragAction.mode==jkqtpmdaScribbleForEvents) emit userScribbleClick(x1, y1, event->modifiers(), false, true);
        event->accept();
    }

    currentMouseDragAction.clear();
    updateCursor();
}

void JKQTPlotter::mouseDoubleClickEvent ( QMouseEvent * event ){

    auto itAction=findMatchingMouseDoubleClickAction(event->button(), event->modifiers());
    if (itAction!=plotterStyle.registeredMouseDoubleClickActions.end())  {
        // we found an action to perform on this double-click
        if (itAction.value()==JKQTPMouseDoubleClickActions::jkqtpdcaClickOpensContextMenu) {
            openStandardContextMenu(event->x(), event->y());
        } else if (itAction.value()==JKQTPMouseDoubleClickActions::jkqtpdcaClickOpensSpecialContextMenu) {
            openSpecialContextMenu(event->x(), event->y());
        } else if (itAction.value()==JKQTPMouseDoubleClickActions::jkqtpdcaClickZoomsIn || itAction.value()==JKQTPMouseDoubleClickActions::jkqtpdcaClickZoomsOut) {
            double factor=4.0;
            if (itAction.value()==JKQTPMouseDoubleClickActions::jkqtpdcaClickZoomsOut) factor=1;

            double xmin=plotter->p2x(static_cast<double>(event->x())/magnification-static_cast<double>(plotter->getPlotWidth())/factor);
            double xmax=plotter->p2x(static_cast<double>(event->x())/magnification+static_cast<double>(plotter->getPlotWidth())/factor);
            double ymin=plotter->p2y(static_cast<double>(event->y())/magnification-static_cast<double>(getPlotYOffset())+static_cast<double>(plotter->getPlotHeight())/factor);
            double ymax=plotter->p2y(static_cast<double>(event->y())/magnification-static_cast<double>(getPlotYOffset())-static_cast<double>(plotter->getPlotHeight())/factor);
            if  ( (event->x()/magnification<plotter->getInternalPlotBorderLeft()) || (event->x()/magnification>plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft()) ) {
                xmin=getXMin();
                xmax=getXMax();
            } else if (((event->y()-getPlotYOffset())/magnification<plotter->getInternalPlotBorderTop()) || ((event->y()-getPlotYOffset())/magnification>plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
                ymin=getYMin();
                ymax=getYMax();
            }
            plotter->setXY(xmin, xmax, ymin, ymax);
            update();
        } else if (itAction.value()==JKQTPMouseDoubleClickActions::jkqtpdcaClickMovesViewport) {
            QRectF zoomRect= QRectF(QPointF(plotter->x2p(getXAxis()->getMin()),plotter->y2p(getYAxis()->getMax())), QPointF(plotter->x2p(getXAxis()->getMax()),plotter->y2p(getYAxis()->getMin())));
            if  ( (event->x()/magnification<plotter->getInternalPlotBorderLeft()) || (event->x()/magnification>plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft()) ) {
                zoomRect.moveCenter(QPointF(zoomRect.center().x(), event->y()));
            } else if (((event->y()-getPlotYOffset())/magnification<plotter->getInternalPlotBorderTop()) || ((event->y()-getPlotYOffset())/magnification>plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
                zoomRect.moveCenter(QPointF(event->x(), zoomRect.center().y()));
            } else {
                zoomRect.moveCenter(QPointF(event->x(), event->y()));
            }
            setXY(plotter->p2x(zoomRect.left()), plotter->p2x(zoomRect.right()), plotter->p2y(zoomRect.bottom()), plotter->p2y(zoomRect.top()));
        }
    }

    // only react on double clicks inside the widget
    if  ( (event->x()/magnification>=plotter->getInternalPlotBorderLeft()) && (event->x()/magnification<=plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft())  &&
          ((event->y()-getPlotYOffset())/magnification>=plotter->getInternalPlotBorderTop()) && ((event->y()-getPlotYOffset())/magnification<=plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {

        mouseLastClickX=event->x();
        mouseLastClickY=event->y();

        emit plotMouseDoubleClicked(plotter->p2x(event->x()/magnification), plotter->p2y((event->y()-getPlotYOffset())/magnification), event->modifiers(), event->button());

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

    auto itAction=findMatchingMouseWheelAction(event->modifiers());
    if (itAction!=plotterStyle.registeredMouseWheelActions.end())  {
        if (itAction.value()==JKQTPMouseWheelActions::jkqtpmwaZoomByWheel) {
            double factor=pow(2.0, 1.0*static_cast<double>(event->delta())/120.0)*2.0;
            double xmin=plotter->p2x(static_cast<double>(event->x())/magnification-static_cast<double>(plotter->getPlotWidth())/factor);
            double xmax=plotter->p2x(static_cast<double>(event->x())/magnification+static_cast<double>(plotter->getPlotWidth())/factor);
            double ymin=plotter->p2y(static_cast<double>(event->y())/magnification-static_cast<double>(getPlotYOffset())+static_cast<double>(plotter->getPlotHeight())/factor);
            double ymax=plotter->p2y(static_cast<double>(event->y())/magnification-static_cast<double>(getPlotYOffset())-static_cast<double>(plotter->getPlotHeight())/factor);
            if  ( (event->x()/magnification<plotter->getInternalPlotBorderLeft()) || (event->x()/magnification>plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft()) ) {
                xmin=getXMin();
                xmax=getXMax();
            } else if (((event->y()-getPlotYOffset())/magnification<plotter->getInternalPlotBorderTop()) || ((event->y()-getPlotYOffset())/magnification>plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
                ymin=getYMin();
                ymax=getYMax();
            }
            plotter->setXY(xmin, xmax, ymin, ymax);
        } else if (itAction.value()==JKQTPMouseWheelActions::jkqtpmwaPanByWheel) {
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
            if  ( (event->x()/magnification<plotter->getInternalPlotBorderLeft()) || (event->x()/magnification>plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft()) ) {
                zoomRect.translate(0, d.y());
            } else if (((event->y()-getPlotYOffset())/magnification<plotter->getInternalPlotBorderTop()) || ((event->y()-getPlotYOffset())/magnification>plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
                zoomRect.translate(d.x(), 0);
            } else {
                zoomRect.translate(d.x(), d.y());
            }
            setXY(plotter->p2x(zoomRect.left()), plotter->p2x(zoomRect.right()), plotter->p2y(zoomRect.bottom()), plotter->p2y(zoomRect.top()));
        }
    }


    event->accept();

    emit plotMouseWheelOperated(plotter->p2x(event->x()), plotter->p2x(event->y()), event->modifiers(), event->angleDelta().x(), event->angleDelta().y());

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
    contextMenu->clear();
    qDeleteAll(contextSubMenus);
    contextSubMenus.clear();

    contextMenu->addAction(plotter->getActionSaveData());
    contextMenu->addAction(plotter->getActionSavePlot());
    contextMenu->addAction(plotter->getActionPrint());
    contextMenu->addSeparator();
    contextMenu->addAction(plotter->getActionCopyPixelImage());
    contextMenu->addAction(plotter->getActionCopyData());
    contextMenu->addAction(plotter->getActionCopyMatlab());
    contextMenu->addSeparator();
    contextMenu->addAction(plotter->getActionShowPlotData());
    contextMenu->addSeparator();
    contextMenu->addAction(plotter->getActionZoomAll());
    contextMenu->addAction(plotter->getActionZoomIn());
    contextMenu->addAction(plotter->getActionZoomOut());
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
        setCursor(QCursor(Qt::ArrowCursor));
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

void JKQTPlotter::redrawOverlays() {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTPlotter::redrawOverlays()"));
#endif
    if (!doDrawing) return;
    disconnect(plotter, SIGNAL(plotUpdated()), this, SLOT(redrawPlot()));
    disconnect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(redrawOverlays()));
    image=imageNoOverlays;
    JKQTPEnhancedPainter painter(&image);
    if (painter.isActive()) {
        painter.scale(magnification, magnification);
        plotter->drawNonGridOverlays(painter);
    }
    oldImage=image;
    connect(plotter, SIGNAL(plotUpdated()), this, SLOT(redrawPlot()));
    connect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(redrawOverlays()));
    repaint();
}

void JKQTPlotter::redrawPlot() {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTPlotter::redrawPlot()"));
#endif
    if (!doDrawing) return;
    disconnect(plotter, SIGNAL(plotUpdated()), this, SLOT(redrawPlot()));
    disconnect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(redrawOverlays()));
    plotter->setWidgetSize(width()/magnification, height()/magnification-getPlotYOffset());
    JKQTPEnhancedPainter painter(&image);
    if (painter.isActive()) {
        painter.scale(magnification, magnification);
        //QTime t;
        //t.start();
        plotter->drawNonGrid(painter, QPoint(0,0), false);//, QRect(QPoint(0,0), QSize(plotter->getPlotWidth(), plotter->getPlotHeight())));
        //qDebug()<<"drawNonGrid"<<objectName()<<": "<<t.elapsed()<<"ms";
        imageNoOverlays=image;
        plotter->drawNonGridOverlays(painter);
    }
    oldImage=image;
    connect(plotter, SIGNAL(plotUpdated()), this, SLOT(redrawPlot()));
    connect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(redrawOverlays()));
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
            p->drawText(plotter->getInternalPlotBorderLeft(), getPlotYOffset()-1, plotterStyle.mousePositionTemplate.arg(mousePosX).arg(mousePosY));
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
     //std::cout<<"resize DONE\n";
}

void JKQTPlotter::delayedResizeEvent()
{
    int plotImageWidth=width();
    int plotImageHeight=height();

    plotImageHeight=plotImageHeight-getPlotYOffset();
    //std::cout<<"resize: "<<plotImageWidth<<" x "<<plotImageHeight<<std::endl;
    bool sizeChanged=false;
    if (plotImageWidth != image.width() || plotImageHeight != image.height()) {

        QImage newImage(QSize(plotImageWidth, plotImageHeight), QImage::Format_ARGB32);
        image=newImage;
        sizeChanged=true;
    }

    if (sizeChanged) redrawPlot();
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
    minSize=QSize(plotter->getInternalPlotBorderLeft()+plotter->getInternalPlotBorderRight()+10, plotter->getInternalPlotBorderTop()+plotter->getInternalPlotBorderBottom()+10);
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

void JKQTPlotter::setContextMenuMode(JKQTPContextMenuModes mode) {
    contextMenuMode=mode;
}

QMenu *JKQTPlotter::getSpecialContextMenu() const {
    return this->menujkqtpcmmSpecialContextMenu;
}

void JKQTPlotter::setSpecialContextMenu(QMenu *menu)
{
    menujkqtpcmmSpecialContextMenu=menu;
    if (menujkqtpcmmSpecialContextMenu) {
        menujkqtpcmmSpecialContextMenu->setParent(this);
        menujkqtpcmmSpecialContextMenu->close();
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
    toolbar->addAction(plotter->getActionPrint());
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

void JKQTPlotter::openStandardContextMenu(int x, int y)
{
    //qDebug()<<"openContextMenu("<<x<<y<<contextMenu<<")";
    mouseContextX=plotter->p2x(x/magnification);
    mouseContextY=plotter->p2y((y-getPlotYOffset())/magnification);
    contextMenu->close();
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
    //qDebug()<<"openSpecialContextMenu("<<x<<y<<menujkqtpcmmSpecialContextMenu<<")";
    if (menujkqtpcmmSpecialContextMenu) {
        for (int i=0; i<menujkqtpcmmSpecialContextMenu->actions().size(); i++) {
            //qDebug()<<"  - "<<menujkqtpcmmSpecialContextMenu->actions().at(i)->text();
        }
        mouseContextX=plotter->p2x(x/magnification);
        mouseContextY=plotter->p2y((y-getPlotYOffset())/magnification);
        menujkqtpcmmSpecialContextMenu->close();
        menujkqtpcmmSpecialContextMenu->popup(mapToGlobal(QPoint(x,y)));
        menujkqtpcmmSpecialContextMenu->resize(menujkqtpcmmSpecialContextMenu->sizeHint());
        //qDebug()<<" -> "<<mapToGlobal(QPoint(x,y))<<menujkqtpcmmSpecialContextMenu->size()<<menujkqtpcmmSpecialContextMenu->pos()<<menujkqtpcmmSpecialContextMenu->parent();
        emit contextMenuOpened(mouseContextX, mouseContextY, menujkqtpcmmSpecialContextMenu);
        //qDebug()<<"openSpecialContextMenu("<<x<<y<<menujkqtpcmmSpecialContextMenu<<") ... DONE";
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
    contextMenu->close();
    initContextMenu();

    if (menujkqtpcmmSpecialContextMenu) {
        contextMenu->addSeparator();
        for (QAction* act: menujkqtpcmmSpecialContextMenu->actions()) {
            contextMenu->addAction(act);
        }
    }

    contextMenu->popup(mapToGlobal(QPoint(x,y)));
    //qDebug()<<" -> "<<mapToGlobal(QPoint(x,y))<<contextMenu->size()<<contextMenu->pos()<<contextMenu->parent();
    emit contextMenuOpened(mouseContextX, mouseContextY, contextMenu);
    //qDebug()<<"openContextMenu("<<x<<y<<contextMenu<<") ... DONE";

}

JKQTPMouseDragActionsHashMapIterator JKQTPlotter::findMatchingMouseDragAction(Qt::MouseButton button, Qt::KeyboardModifiers modifiers) const
{
    return plotterStyle.registeredMouseDragActionModes.find(qMakePair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifiers));
}

JKQTPMouseDoubleClickActionsHashMapIterator JKQTPlotter::findMatchingMouseDoubleClickAction(Qt::MouseButton button, Qt::KeyboardModifiers modifiers) const
{
    return plotterStyle.registeredMouseDoubleClickActions.find(qMakePair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifiers));
}

JKQTPMouseWheelActionsHashMapIterator JKQTPlotter::findMatchingMouseWheelAction(Qt::KeyboardModifiers modifiers) const
{
    return plotterStyle.registeredMouseWheelActions.find(modifiers);
}

void JKQTPlotter::setPlotUpdateEnabled(bool enable)
{
    doDrawing=enable;
    plotter->setEmittingSignalsEnabled(enable);
    //qDebug()<<objectName()<<"  doDrawing="<<doDrawing;
}

void JKQTPlotter::registerMouseDragAction(Qt::MouseButton button, Qt::KeyboardModifiers modifier, JKQTPMouseDragActions action)
{
    plotterStyle.registeredMouseDragActionModes[qMakePair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifier)]=action;
}

void JKQTPlotter::deregisterMouseDragAction(Qt::MouseButton button, Qt::KeyboardModifiers modifier)
{
    plotterStyle.registeredMouseDragActionModes.remove(qMakePair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifier));
}

void JKQTPlotter::clearAllRegisteredMouseDragActions()
{
    plotterStyle.registeredMouseDragActionModes.clear();
}

void JKQTPlotter::registerMouseDoubleClickAction(Qt::MouseButton button, Qt::KeyboardModifiers modifier, JKQTPMouseDoubleClickActions action)
{
    plotterStyle.registeredMouseDoubleClickActions[qMakePair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifier)]=action;
}

void JKQTPlotter::deregisterMouseDoubleClickAction(Qt::MouseButton button, Qt::KeyboardModifiers modifier)
{
    plotterStyle.registeredMouseDoubleClickActions.remove(qMakePair<Qt::MouseButton, Qt::KeyboardModifiers>(button, modifier));
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
