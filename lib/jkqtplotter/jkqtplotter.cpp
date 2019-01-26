/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

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




/** \file jkqtplotter.cpp
  * \ingroup jkqtpplotterclasses
  */


#include <QFileInfo>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtGlobal>
#include <QtWidgets>
#else
#include <QtGui>
#endif
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphs.h"


#define jkqtp_RESIZE_DELAY 100






/**************************************************************************************************************************
 * JKQTPlotter
 **************************************************************************************************************************/
JKQTPlotter::JKQTPlotter(bool datastore_internal, QWidget* parent, JKQTPDatastore* datast):
    QWidget(parent, Qt::Widget)
{    
    initJKQTPlotterResources();
    init(datastore_internal, parent, datast);
}

JKQTPlotter::JKQTPlotter(JKQTPDatastore *datast, QWidget *parent):
    QWidget(parent, Qt::Widget)
{
    initJKQTPlotterResources();
    init(false, parent, datast);
}

JKQTPlotter::JKQTPlotter(QWidget *parent):
    QWidget(parent, Qt::Widget)
{
    initJKQTPlotterResources();
    init(true, parent, nullptr);
}

void JKQTPlotter::init(bool datastore_internal, QWidget* parent, JKQTPDatastore* datast)
{
    leftDoubleClickAction=LeftDoubleClickDefault;
    menuSpecialContextMenu=nullptr;
    mouseContextX=0;
    mouseContextY=0;
    setParent(parent);
    connect(&resizeTimer, SIGNAL(timeout()), this, SLOT(delayedResizeEvent()));
    doDrawing=false;
    magnification=1;
    plotter=new JKQTBasePlotter(datastore_internal, this, datast);
    plotter->setEmittingSignalsEnabled(false);
    plotter->setBackgroundColor(palette().color(QPalette::Window));//QPalette::Window
    plotter->setDefaultBackgroundColor(palette().color(QPalette::Window));//QPalette::Window
    //plotter->setPlotBackgroundColor(palette().color(QPalette::Base));
    //plotter->setDefaultPlotBackgroundColor((palette().color(QPalette::Base));

    mousePosX=0;
    mousePosY=0;
    rightMouseButtonAction=JKQTPlotter::RightMouseButtonContextMenu;

    connect(plotter, SIGNAL(plotUpdated()), this, SLOT(redrawPlot()));
    connect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(redrawOverlays()));
    connect(plotter, SIGNAL(beforePlotScalingRecalculate()), this, SLOT(intBeforePlotScalingRecalculate()));
    connect(plotter, SIGNAL(zoomChangedLocally(double, double, double, double, JKQTBasePlotter*)), this, SLOT(pzoomChangedLocally(double, double, double, double, JKQTBasePlotter*)));

    image=QImage(width(), height(), QImage::Format_ARGB32);
    oldImage=image;
    imageNoOverlays=image;
    toolbarIconSize=16;
    mouseDragingRectangle=false;

    default_userActionColor=QColor("steelblue"); userActionColor=default_userActionColor;
    default_userActionCompositionMode=QPainter::CompositionMode_SourceOver; userActionCompositionMode=default_userActionCompositionMode;
    default_mousePositionTemplate=QString("(%1; %2)"); mousePositionTemplate=default_mousePositionTemplate;

    displayMousePosition=true;
    displayToolbar=true;
    toolbarAlwaysOn=false;
    mouseActionMode=JKQTPlotter::ZoomRectangle;
    //zoomByDoubleAndRightMouseClick=true;
    zoomByMouseWheel=true;

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




    QSize s=QSize(toolbarIconSize, toolbarIconSize);
    toolbar->setIconSize(s);

    //move(32,32);
    resize(400,300);
    doDrawing=true;
    plotter->setEmittingSignalsEnabled(true);
    redrawPlot();
}


JKQTPlotter::~JKQTPlotter() {
    disconnect(plotter, SIGNAL(plotUpdated()), this, SLOT(redrawPlot()));
    disconnect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(redrawOverlays()));
    disconnect(plotter, SIGNAL(beforePlotScalingRecalculate()), this, SLOT(intBeforePlotScalingRecalculate()));
    disconnect(plotter, SIGNAL(zoomChangedLocally(double, double, double, double, JKQTBasePlotter*)), this, SLOT(pzoomChangedLocally(double, double, double, double, JKQTBasePlotter*)));
    delete plotter;
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
    toolbarIconSize=value;
    QSize s=QSize(toolbarIconSize, toolbarIconSize);
    toolbar->setIconSize(s);
}

int JKQTPlotter::getToolbarIconSize() {
    return toolbarIconSize;
}

void JKQTPlotter::setToolbarVisible(bool __value)
{
    if (this->displayToolbar != __value) {
        this->displayToolbar = __value;
        updateToolbar();
    }
}

bool JKQTPlotter::isToolbarVisible() const
{
    return this->displayToolbar;
}

void JKQTPlotter::setToolbarAlwaysOn(bool __value)
{
    if (this->toolbarAlwaysOn != __value) {
        this->toolbarAlwaysOn = __value;
        updateToolbar();
    }
}

bool JKQTPlotter::isToolbarAlwaysOn() const
{
    return this->toolbarAlwaysOn;
}

void JKQTPlotter::setMousePositionShown(bool __value)
{
    this->displayMousePosition = __value;
}

bool JKQTPlotter::isMousePositionShown() const
{
    return this->displayMousePosition;
}


void JKQTPlotter::setUserActionColor(const QColor &__value)
{
    if (this->userActionColor != __value) {
        this->userActionColor = __value;
        update();
    }
}

QColor JKQTPlotter::getUserActionColor() const
{
    return this->userActionColor;
}

void JKQTPlotter::setUserActionCompositionMode(const QPainter::CompositionMode &__value)
{
    if (this->userActionCompositionMode != __value) {
        this->userActionCompositionMode = __value;
        update();
    }
}

QPainter::CompositionMode JKQTPlotter::getUserActionCompositionMode() const
{
    return this->userActionCompositionMode;
}

void JKQTPlotter::setMouseActionMode(const JKQTPlotter::MouseActionModes &__value)
{
    if (this->mouseActionMode != __value) {
        this->mouseActionMode = __value;
        updateCursor();
    }
}

JKQTPlotter::MouseActionModes JKQTPlotter::getMouseActionMode() const
{
    return this->mouseActionMode;
}

void JKQTPlotter::loadSettings(const QSettings& settings, const QString& group) {
    plotter->loadSettings(settings, group);


    setToolbarIconSize(settings.value(group+"toolbar_icon_size", toolbarIconSize).toInt());
    mousePositionTemplate=settings.value(group+"mouse_position_template", mousePositionTemplate).toString();
    userActionColor=QColor(settings.value(group+"zoomrect_color", jkqtp_QColor2String(userActionColor)).toString());

    redrawPlot();
}

void JKQTPlotter::saveSettings(QSettings& settings, const QString& group) const {
    plotter->saveSettings(settings, group);

    if (userActionColor!=default_userActionColor) settings.setValue(group+"zoomrect_color", jkqtp_QColor2String(userActionColor));
    if (toolbarIconSize!=default_toolbarIconSize) settings.setValue(group+"toolbar_icon_size", toolbarIconSize);
    if (mousePositionTemplate!=default_mousePositionTemplate) settings.setValue(group+"mouse_position_template", mousePositionTemplate);
}


void JKQTPlotter::paintUserAction() {
    if (mouseDragingRectangle) {
        image=oldImage;
        if (image.width()>0 && image.height()>0 && !image.isNull()) {
            JKQTPEnhancedPainter painter(&image);
            QPen p=painter.pen();
            p.setWidthF(1);
            p.setColor(QColor("black"));
            p.setStyle(Qt::DashLine);
            painter.setPen(p);
            QPainter::CompositionMode oldCMode=painter.compositionMode();
            painter.setCompositionMode(userActionCompositionMode);
            if ((mouseDragRectXEnd!=mouseDragRectXStart) && (mouseDragRectYEnd!=mouseDragRectYStart)) {
                double x1=plotter->x2p(mouseDragRectXStart)*magnification;
                double y1=plotter->y2p(mouseDragRectYStart)*magnification;
                double x2=plotter->x2p(mouseDragRectXEnd)*magnification;
                double y2=plotter->y2p(mouseDragRectYEnd)*magnification;
                double dx=x2-x1;
                double dy=y2-y1;
                if ((mouseActionMode==JKQTPlotter::ZoomRectangle) || (mouseActionMode==JKQTPlotter::RectangleEvents)) {
                    painter.setOpacity(0.2);
                    painter.fillRect(QRectF(x1, y1, x2-x1, y2-y1), QBrush(userActionColor));
                    painter.setOpacity(1.0);
                    painter.drawRect(QRectF(x1, y1, x2-x1, y2-y1));
                } else if (mouseActionMode==JKQTPlotter::CircleEvents) {
                    QColor zc=userActionColor;
                    zc.setAlphaF(0.2);
                    painter.setBrush(QBrush(zc));
                    painter.drawEllipse(QPointF(x1, y1), qMin(fabs(dx), fabs(dy)), qMin(fabs(dx), fabs(dy)));
                } else  if (mouseActionMode==JKQTPlotter::EllipseEvents) {
                    QColor zc=userActionColor;
                    zc.setAlphaF(0.2);
                    painter.setBrush(QBrush(zc));
                    painter.drawEllipse(QPointF(x1, y1), fabs(dx), fabs(dy));
                } else  if (mouseActionMode==JKQTPlotter::LineEvents) {
                    QPen pp=p;
                    pp.setColor(userActionColor);
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
    if (displayMousePosition) {
        mousePosX=plotter->p2x(event->x()/magnification);
        mousePosY=plotter->p2y((event->y()-getPlotYOffset())/magnification);
        update();
    }

    if (displayToolbar && (!toolbarAlwaysOn) && (!toolbar->isVisible())) { // decide whether to display toolbar
        int y1=10;
        if (event->y()/magnification>=0 && event->y()/magnification<=y1) {
            toolbar->show();
            toolbar->move(1,1);
        }
    }
    if (!displayToolbar) {
        toolbar->hide();
    }
    if (( (mouseActionMode==JKQTPlotter::ZoomRectangle) ||
          (mouseActionMode==JKQTPlotter::RectangleEvents) ||
          (mouseActionMode==JKQTPlotter::CircleEvents) ||
          (mouseActionMode==JKQTPlotter::EllipseEvents) ||
          (mouseActionMode==JKQTPlotter::ScribbleEvents) ||
          (mouseActionMode==JKQTPlotter::PanPlotOnMove) ||
          (mouseActionMode==JKQTPlotter::PanPlotOnRelease) ||
          (mouseActionMode==JKQTPlotter::LineEvents) ) &&
            mouseDragingRectangle && (event->buttons() & Qt::LeftButton))
    {
        if (mouseActionMode==JKQTPlotter::ScribbleEvents || mouseActionMode==JKQTPlotter::PanPlotOnMove) {
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
        if (mouseActionMode==JKQTPlotter::ZoomRectangle) {
            emit plotNewZoomRectangle(mouseDragRectXStart, mouseDragRectXEnd, mouseDragRectYStart, mouseDragRectYEnd, event->modifiers());
        }
        if ((mouseActionMode==JKQTPlotter::ScribbleEvents) && ((mouseDragRectXStart!=mouseDragRectXEnd) || (mouseDragRectYStart!=mouseDragRectYEnd)) ) {
            emit userScribbleClick(mouseDragRectXEnd, mouseDragRectYEnd, event->modifiers(), false, false);
        }
        if ((mouseActionMode==JKQTPlotter::PanPlotOnMove) && ((mouseDragRectXStart!=mouseDragRectXEnd) || (mouseDragRectYStart!=mouseDragRectYEnd)) ) {
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

    // emit clicked signal, if event occured inside plot only
    if  ( (event->x()/magnification>=plotter->getInternalPlotBorderLeft()) && (event->x()/magnification<=plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft())  &&
          ((event->y()-getPlotYOffset())/magnification>=plotter->getInternalPlotBorderTop()) && ((event->y()-getPlotYOffset())/magnification<=plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
        emit plotMouseMove(plotter->p2x(event->x()/magnification), plotter->p2y((event->y()-getPlotYOffset())/magnification));
    }
}



void JKQTPlotter::mousePressEvent ( QMouseEvent * event ){
    if (event->button()==Qt::LeftButton) {
        mouseLastClickX=event->x();
        mouseLastClickY=event->y();
        if ( (mouseActionMode!=JKQTPlotter::NoMouseAction))
        {
            mouseDragRectXStart=plotter->p2x(event->x()/magnification);
            mouseDragRectYStart=plotter->p2y((event->y()-getPlotYOffset())/magnification);
            mouseDragRectXEndPixel=mouseDragRectXStartPixel=event->x();
            mouseDragRectYEndPixel=mouseDragRectYStartPixel=event->y();
            mouseDragingRectangle=true;
            oldImage=image;
            event->accept();
            if (mouseActionMode==JKQTPlotter::ScribbleEvents) emit userScribbleClick(mouseDragRectXStart, mouseDragRectYStart, event->modifiers(), true, false);
            if (mouseActionMode==JKQTPlotter::ClickEvents) emit userClickFinished(mouseDragRectXStart, mouseDragRectYStart, event->modifiers());
        }
    } else if (event->button()==Qt::RightButton) {
        mouseLastClickX=event->x();
        mouseLastClickY=event->y();
        if (rightMouseButtonAction==JKQTPlotter::RightMouseButtonZoom) {
            double xmin=plotter->p2x((long)round(static_cast<double>(plotter->getInternalPlotBorderLeft())-static_cast<double>(plotter->getPlotWidth())/2.0));
            double xmax=plotter->p2x((long)round(static_cast<double>(plotter->getInternalPlotBorderLeft())+1.5*static_cast<double>(plotter->getPlotWidth())));
            double ymin=plotter->p2y((long)round(static_cast<double>(plotter->getInternalPlotBorderTop())+1.5*static_cast<double>(plotter->getPlotHeight())));
            double ymax=plotter->p2y((long)round(static_cast<double>(plotter->getInternalPlotBorderTop())-static_cast<double>(plotter->getPlotHeight())/2.0));
            if (plotter->getXAxis()->isLogAxis()) {
                if (xmin<=0) xmin=1;
                if (xmax<=0) xmax=10;
            }
            if (plotter->getYAxis()->isLogAxis()) {
                if (ymin<=0) ymin=1;
                if (ymax<=0) ymax=10;
            }
            /*plotter->getXAxis->setRange(xmin, xmax);
            plotter->getYAxis->setRange(ymin, ymax);*/
            //redrawPlot();
            /*if (plotter->isEmittingSignalsEnabled())*/ emit zoomChangedLocally(xmin, xmax, ymin, ymax, this);
            plotter->setXY(xmin, xmax, ymin, ymax);
            //update();
            event->accept();
        } else if (rightMouseButtonAction==JKQTPlotter::RightMouseButtonContextMenu) {
            openContextMenu(event->x(), event->y());

            event->accept();
        }
    }

    // emit clicked signal, if event occured inside plot only
    if  ( (event->x()/magnification>=plotter->getInternalPlotBorderLeft()) && (event->x()/magnification<=plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft())  &&
          ((event->y()-getPlotYOffset())/magnification>=plotter->getInternalPlotBorderTop()) && ((event->y()-getPlotYOffset())/magnification<=plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
        emit plotMouseClicked(plotter->p2x(event->x()/magnification), plotter->p2y((event->y()-getPlotYOffset())/magnification), event->modifiers(), event->button());
        event->accept();
    }
}

void JKQTPlotter::mouseReleaseEvent ( QMouseEvent * event ){
    if ((event->flags()&Qt::MouseEventCreatedDoubleClick)==Qt::MouseEventCreatedDoubleClick) {
        return;
    }
    if (mouseDragingRectangle && event->button()==Qt::LeftButton) {
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
            if (mouseActionMode==JKQTPlotter::ZoomRectangle) {
                double xmin=mouseDragRectXStart;
                double xmax=mouseDragRectXEnd;

                double ymin=mouseDragRectYStart;
                double ymax=mouseDragRectYEnd;

                emit zoomChangedLocally(xmin, xmax, ymin, ymax, this);
                plotter->setXY(xmin, xmax, ymin, ymax);
            } else if (mouseActionMode==JKQTPlotter::PanPlotOnRelease) {
                QRectF zoomRect= QRectF(QPointF(plotter->x2p(getXAxis()->getMin()),plotter->y2p(getYAxis()->getMax())), QPointF(plotter->x2p(getXAxis()->getMax()),plotter->y2p(getYAxis()->getMin())));
                if  ( (mouseLastClickX/magnification<plotter->getInternalPlotBorderLeft()) || (mouseLastClickX/magnification>plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft()) ) {
                    zoomRect.translate(0, mouseDragRectYStartPixel-mouseDragRectYEndPixel);
                } else if (((mouseLastClickY-getPlotYOffset())/magnification<plotter->getInternalPlotBorderTop()) || ((mouseLastClickY-getPlotYOffset())/magnification>plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {
                    zoomRect.translate(mouseDragRectXStartPixel-mouseDragRectXEndPixel, 0);
                } else {
                    zoomRect.translate(mouseDragRectXStartPixel-mouseDragRectXEndPixel, mouseDragRectYStartPixel-mouseDragRectYEndPixel);
                }
                setXY(plotter->p2x(zoomRect.left()), plotter->p2x(zoomRect.right()), plotter->p2y(zoomRect.bottom()), plotter->p2y(zoomRect.top()));
            } else if (mouseActionMode==JKQTPlotter::RectangleEvents) {
                emit userRectangleFinished(x1, y1, x2-x1, y2-y1, event->modifiers());
            } else if (mouseActionMode==JKQTPlotter::CircleEvents) {
                emit userCircleFinished(x1, y1, qMin(fabs(x2-x1), fabs(y2-y1)), event->modifiers());
            } else if (mouseActionMode==JKQTPlotter::EllipseEvents) {
                emit userEllipseFinished(x1, y1, fabs(x2-x1), fabs(y2-y1), event->modifiers());
            } else if (mouseActionMode==JKQTPlotter::LineEvents) {
                emit userLineFinished(x1, y1, x2, y2, event->modifiers());
            } else if (mouseActionMode==JKQTPlotter::ScribbleEvents) {
                emit userScribbleClick(x1, y1, event->modifiers(), false, true);
            }

        }
        if (mouseActionMode!=JKQTPlotter::ZoomRectangle) update();
        event->accept();
    }
}

void JKQTPlotter::mouseDoubleClickEvent ( QMouseEvent * event ){
    // only react on double clicks inside the widget
    if  ( (event->x()/magnification>=plotter->getInternalPlotBorderLeft()) && (event->x()/magnification<=plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft())  &&
          ((event->y()-getPlotYOffset())/magnification>=plotter->getInternalPlotBorderTop()) && ((event->y()-getPlotYOffset())/magnification<=plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) {

        mouseLastClickX=event->x();
        mouseLastClickY=event->y();
        if (event->button()==Qt::LeftButton) {
            if (leftDoubleClickAction==LeftDoubleClickContextMenu) {
                openContextMenu(event->x(), event->y());
                event->accept();
            } else if (leftDoubleClickAction==LeftDoubleClickSpecialContextMenu) {
                openSpecialContextMenu(event->x(), event->y());
                event->accept();
            }
        }

        if (rightMouseButtonAction==JKQTPlotter::RightMouseButtonZoom && event->button()==Qt::RightButton) {
            double factor=4.0;
            if (event->button()==Qt::RightButton) factor=1;
            double xmin=plotter->p2x((long)round(static_cast<double>(event->x())/magnification-static_cast<double>(plotter->getPlotWidth())/factor));
            double xmax=plotter->p2x((long)round(static_cast<double>(event->x())/magnification+static_cast<double>(plotter->getPlotWidth())/factor));
            double ymin=plotter->p2y((long)round(static_cast<double>(event->y())/magnification-static_cast<double>(getPlotYOffset())+static_cast<double>(plotter->getPlotHeight())/factor));
            double ymax=plotter->p2y((long)round(static_cast<double>(event->y())/magnification-static_cast<double>(getPlotYOffset())-static_cast<double>(plotter->getPlotHeight())/factor));

            event->accept();
            //xAxis->setRange(xmin, xmax);
            //yAxis->setRange(ymin, ymax);
            //redrawPlot();
            /*if (plotter->isEmittingSignalsEnabled())*/ emit zoomChangedLocally(xmin, xmax, ymin, ymax, this);
            plotter->setXY(xmin, xmax, ymin, ymax);
            update();
        }
        emit plotMouseDoubleClicked(plotter->p2x(event->x()/magnification), plotter->p2y((event->y()-getPlotYOffset())/magnification), event->modifiers(), event->button());

    } else { event->ignore(); }
}

void JKQTPlotter::keyReleaseEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    if (event->key()==Qt::Key_Escape && event->modifiers()==Qt::NoModifier) {
        if (mouseDragingRectangle) {
            mouseDragingRectangle=false;
            image=oldImage;
            update();
            event->accept();
        }
    }
}

void JKQTPlotter::wheelEvent ( QWheelEvent * event ) {
    // only react on wheel turns inside the widget, turning forward will zoom out and turning backwards will zoom in by a factor of 2
    if  ( zoomByMouseWheel && ((event->x()/magnification>=plotter->getInternalPlotBorderLeft()) && (event->x()/magnification<=plotter->getPlotWidth()+plotter->getInternalPlotBorderLeft())  &&
          ((event->y()-getPlotYOffset())/magnification>=plotter->getInternalPlotBorderTop()) && ((event->y()-getPlotYOffset())/magnification<=plotter->getPlotHeight()+plotter->getInternalPlotBorderTop()) ) ) {
        double factor=pow(2.0, 1.0*static_cast<double>(event->delta())/120.0)*2.0;
        double xmin=plotter->p2x((long)round(static_cast<double>(event->x())/magnification-static_cast<double>(plotter->getPlotWidth())/factor));
        double xmax=plotter->p2x((long)round(static_cast<double>(event->x())/magnification+static_cast<double>(plotter->getPlotWidth())/factor));
        double ymin=plotter->p2y((long)round(static_cast<double>(event->y())/magnification-static_cast<double>(getPlotYOffset())+static_cast<double>(plotter->getPlotHeight())/factor));
        double ymax=plotter->p2y((long)round(static_cast<double>(event->y())/magnification-static_cast<double>(getPlotYOffset())-static_cast<double>(plotter->getPlotHeight())/factor));

        event->accept();
        plotter->setXY(xmin, xmax, ymin, ymax);
    } else {
        event->ignore();
    }
}

int JKQTPlotter::getPlotYOffset() {
     int plotYOffset=0;
     if (displayMousePosition) {
        plotYOffset=plotYOffset+QFontMetrics(font()).height()+2;
     }
     if (displayToolbar && toolbarAlwaysOn) {
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
        act->setChecked(getPlotter()->getGraph(i)->getVisible());
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
    if (mouseActionMode==JKQTPlotter::ZoomRectangle) {
        QBitmap cursor(":/JKQTPlotter/jkqtp_cursor_zoom.png");
        QBitmap mask(":/JKQTPlotter/jkqtp_cursor_zoom_mask.png");
        setCursor(QCursor(cursor, mask, 9, 14));
    } else if (mouseActionMode==JKQTPlotter::RectangleEvents) {
        QBitmap cursor(":/JKQTPlotter/jkqtp_cursor_rectangle.png");
        QBitmap mask(":/JKQTPlotter/jkqtp_cursor_rectangle_mask.png");
        setCursor(QCursor(cursor, mask, 9, 14));
    } else if (mouseActionMode==JKQTPlotter::PanPlotOnMove || mouseActionMode==JKQTPlotter::PanPlotOnRelease) {
        setCursor(QCursor(Qt::OpenHandCursor));
     } else if (mouseActionMode==JKQTPlotter::CircleEvents) {
        QBitmap cursor(":/JKQTPlotter/jkqtp_cursor_circle.png");
        QBitmap mask(":/JKQTPlotter/jkqtp_cursor_circle_mask.png");
        setCursor(QCursor(cursor, mask, 9, 14));
    } else if (mouseActionMode==JKQTPlotter::EllipseEvents) {
        QBitmap cursor(":/JKQTPlotter/jkqtp_cursor_ellipse.png");
        QBitmap mask(":/JKQTPlotter/jkqtp_cursor_ellipse_mask.png");
        setCursor(QCursor(cursor, mask, 9, 14));
    } else if (mouseActionMode==JKQTPlotter::LineEvents) {
        QBitmap cursor(":/JKQTPlotter/jkqtp_cursor_line.png");
        QBitmap mask(":/JKQTPlotter/jkqtp_cursor_line_mask.png");
        setCursor(QCursor(cursor, mask, 9, 14));
    } else if (mouseActionMode==JKQTPlotter::ClickEvents) {
        setCursor(QCursor(Qt::CrossCursor));
    } else if (mouseActionMode==JKQTPlotter::ScribbleEvents) {
        QBitmap cursor(":/JKQTPlotter/jkqtp_cursor_scribble.png");
        QBitmap mask(":/JKQTPlotter/jkqtp_cursor_scribble_mask.png");
        setCursor(QCursor(cursor, mask, 9, 14));
    } else {
        setCursor(QCursor(Qt::ArrowCursor));
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
    plotter->resize(width()/magnification, height()/magnification-getPlotYOffset());
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
        if (displayMousePosition ) {
            p->drawText(plotter->getInternalPlotBorderLeft(), getPlotYOffset()-1, mousePositionTemplate.arg(mousePosX).arg(mousePosY));
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
    if (!toolbarAlwaysOn) toolbar->hide();
}

void JKQTPlotter::updateToolbar()
{
    if (displayToolbar) {
        if (toolbarAlwaysOn) {
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



void JKQTPlotter::masterPlotScalingRecalculated() {
    if (masterPlotter) {
        if (plotter->getMasterSynchronizeHeight()||plotter->getMasterSynchronizeWidth()) {
            redrawPlot();
        }
    }
}

void JKQTPlotter::synchronizeToMaster(JKQTPlotter* master, bool synchronizeWidth, bool synchronizeHeight) {
    if (!master) {
        resetMasterSynchronization();
    }
    plotter->synchronizeToMaster(master->getPlotter(), synchronizeWidth, synchronizeHeight);
    masterPlotter=master;
    if (masterPlotter) connect(masterPlotter->getPlotter(), SIGNAL(plotScalingRecalculated()), this, SLOT(masterPlotScalingRecalculated()));
    redrawPlot();
}

void JKQTPlotter::resetMasterSynchronization() {
    if (masterPlotter) disconnect(masterPlotter->getPlotter(), SIGNAL(plotScalingRecalculated()), this, SLOT(masterPlotScalingRecalculated()));
    plotter->resetMasterSynchronization();
    redrawPlot();
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

void JKQTPlotter::setZoomByMouseRectangle(bool zomByrectangle) {
    if (zomByrectangle) mouseActionMode=JKQTPlotter::ZoomRectangle;
    else mouseActionMode=JKQTPlotter::NoMouseAction;
}

void JKQTPlotter::setRightMouseButtonAction(const JKQTPlotter::RightMouseButtonAction &__value)
{
    this->rightMouseButtonAction = __value;
}

JKQTPlotter::RightMouseButtonAction JKQTPlotter::getActionRightMouseButton() const
{
    return this->rightMouseButtonAction;
}

void JKQTPlotter::setLeftDoubleClickAction(const JKQTPlotter::LeftDoubleClickAction &__value)
{
    this->leftDoubleClickAction = __value;
}

JKQTPlotter::LeftDoubleClickAction JKQTPlotter::getActionLeftDoubleClick() const
{
    return this->leftDoubleClickAction;
}

QMenu *JKQTPlotter::getMenuSpecialContextMenu() const {
    return this->menuSpecialContextMenu;
}

void JKQTPlotter::setMenuSpecialContextMenu(QMenu *menu)
{
    menuSpecialContextMenu=menu;
    if (menuSpecialContextMenu) {
        menuSpecialContextMenu->setParent(this);
        menuSpecialContextMenu->close();
    }
}

void JKQTPlotter::setZoomByMouseWheel(bool __value)
{
    this->zoomByMouseWheel = __value;
}

bool JKQTPlotter::getZoomByMouseWheel() const
{
    return this->zoomByMouseWheel;
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

void JKQTPlotter::setMagnification(double m)
{
    magnification=m;
    redrawPlot();
}

bool JKQTPlotter::getZoomByMouseRectangle() const {
    return (mouseActionMode==JKQTPlotter::ZoomRectangle);
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
    //qDebug()<<"openSpecialContextMenu("<<x<<y<<menuSpecialContextMenu<<")";
    if (menuSpecialContextMenu) {
        for (int i=0; i<menuSpecialContextMenu->actions().size(); i++) {
            //qDebug()<<"  - "<<menuSpecialContextMenu->actions().at(i)->text();
        }
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


void JKQTPlotter::setPlotUpdateEnabled(bool enable)
{
    doDrawing=enable;
    plotter->setEmittingSignalsEnabled(enable);
    //qDebug()<<objectName()<<"  doDrawing="<<doDrawing;
}


void initJKQTPlotterResources()
{
    initJKQTBasePlotterResources();
}
