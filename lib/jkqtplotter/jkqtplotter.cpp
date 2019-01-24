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
    plotter->set_emitSignals(false);
    plotter->set_backgroundColor(palette().color(QPalette::Window));//QPalette::Window
    plotter->set_def_backgroundColor(palette().color(QPalette::Window));//QPalette::Window
    //plotter->set_plotBackgroundColor(palette().color(QPalette::Base));
    //plotter->set_def_plotBackgroundColor((palette().color(QPalette::Base));

    mousePosX=0;
    mousePosY=0;
    rightMouseButtonAction=JKQTPlotter::RightMouseButtonContextMenu;

    connect(plotter, SIGNAL(plotUpdated()), this, SLOT(update_plot()));
    connect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(replot_overlays()));
    connect(plotter, SIGNAL(beforePlotScalingRecalculate()), this, SLOT(intBeforePlotScalingRecalculate()));
    connect(plotter, SIGNAL(zoomChangedLocally(double, double, double, double, JKQTBasePlotter*)), this, SLOT(pzoomChangedLocally(double, double, double, double, JKQTBasePlotter*)));

    image=QImage(width(), height(), QImage::Format_ARGB32);
    oldImage=image;
    imageNoOverlays=image;
    toolbarIconSize=16;
    mouseDragingRectangle=false;

    def_userActionColor=QColor("steelblue"); userActionColor=def_userActionColor;
    def_userActionCompositionMode=QPainter::CompositionMode_SourceOver; userActionCompositionMode=def_userActionCompositionMode;
    def_mousePositionTemplate=QString("(%1; %2)"); mousePositionTemplate=def_mousePositionTemplate;

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
    plotter->set_emitSignals(true);
    update_plot();
}


JKQTPlotter::~JKQTPlotter() {
    disconnect(plotter, SIGNAL(plotUpdated()), this, SLOT(update_plot()));
    disconnect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(replot_overlays()));
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

void JKQTPlotter::set_toolbarIconSize(int value) {
    toolbarIconSize=value;
    QSize s=QSize(toolbarIconSize, toolbarIconSize);
    toolbar->setIconSize(s);
}

int JKQTPlotter::get_toolbarIconSize() {
    return toolbarIconSize;
}

void JKQTPlotter::set_displayToolbar(bool __value)
{
    if (this->displayToolbar != __value) {
        this->displayToolbar = __value;
        updateToolbar();
    }
}

bool JKQTPlotter::get_displayToolbar() const
{
    return this->displayToolbar;
}

void JKQTPlotter::set_toolbarAlwaysOn(bool __value)
{
    if (this->toolbarAlwaysOn != __value) {
        this->toolbarAlwaysOn = __value;
        updateToolbar();
    }
}

bool JKQTPlotter::get_toolbarAlwaysOn() const
{
    return this->toolbarAlwaysOn;
}

void JKQTPlotter::set_displayMousePosition(bool __value)
{
    this->displayMousePosition = __value;
}

bool JKQTPlotter::get_displayMousePosition() const
{
    return this->displayMousePosition;
}


void JKQTPlotter::set_userActionColor(const QColor &__value)
{
    if (this->userActionColor != __value) {
        this->userActionColor = __value;
        update();
    }
}

QColor JKQTPlotter::get_userActionColor() const
{
    return this->userActionColor;
}

void JKQTPlotter::set_userActionCompositionMode(const QPainter::CompositionMode &__value)
{
    if (this->userActionCompositionMode != __value) {
        this->userActionCompositionMode = __value;
        update();
    }
}

QPainter::CompositionMode JKQTPlotter::get_userActionCompositionMode() const
{
    return this->userActionCompositionMode;
}

void JKQTPlotter::set_mouseActionMode(const JKQTPlotter::MouseActionModes &__value)
{
    if (this->mouseActionMode != __value) {
        this->mouseActionMode = __value;
        updateCursor();
    }
}

JKQTPlotter::MouseActionModes JKQTPlotter::get_mouseActionMode() const
{
    return this->mouseActionMode;
}

void JKQTPlotter::loadSettings(QSettings& settings, QString group) {
    plotter->loadSettings(settings, group);


    set_toolbarIconSize(settings.value(group+"toolbar_icon_size", toolbarIconSize).toInt());
    mousePositionTemplate=settings.value(group+"mouse_position_template", mousePositionTemplate).toString();
    userActionColor=QColor(settings.value(group+"zoomrect_color", jkqtp_QColor2String(userActionColor)).toString());

    update_plot();
}

void JKQTPlotter::saveSettings(QSettings& settings, QString group) {
    plotter->saveSettings(settings, group);

    if (userActionColor!=def_userActionColor) settings.setValue(group+"zoomrect_color", jkqtp_QColor2String(userActionColor));
    if (toolbarIconSize!=def_toolbarIconSize) settings.setValue(group+"toolbar_icon_size", toolbarIconSize);
    if (mousePositionTemplate!=def_mousePositionTemplate) settings.setValue(group+"mouse_position_template", mousePositionTemplate);
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
          (mouseActionMode==JKQTPlotter::LineEvents) ) &&
            mouseDragingRectangle && (event->buttons() & Qt::LeftButton))
    {
        if (mouseActionMode==JKQTPlotter::ScribbleEvents) {
            mouseDragRectXStart=mouseDragRectXEnd;
            mouseDragRectYStart=mouseDragRectYEnd;
        }
        mouseDragRectXEnd=plotter->p2x(event->x()/magnification);
        mouseDragRectYEnd=plotter->p2y((event->y()-getPlotYOffset())/magnification);
        paintUserAction();
        event->accept();
        //std::cout<<mouseZoomingTStart<<" -- "<<mouseZoomingTEnd<<std::endl;
        if (mouseActionMode==JKQTPlotter::ZoomRectangle) {
            emit plotNewZoomRectangle(mouseDragRectXStart, mouseDragRectXEnd, mouseDragRectYStart, mouseDragRectYEnd);
        }
        if ((mouseActionMode==JKQTPlotter::ScribbleEvents) && ((mouseDragRectXStart!=mouseDragRectXEnd) || (mouseDragRectYStart!=mouseDragRectYEnd)) ) {
            emit userScribbleClick(mouseDragRectXEnd, mouseDragRectYEnd, event->modifiers(), false, false);
        }
    } else {
        event->accept();
        /*if (emitSignals)*/ //emit plotMouseMove(x, y);
    }

    // emit clicked signal, if event occured inside plot only
    if  ( (event->x()/magnification>=plotter->get_iplotBorderLeft()) && (event->x()/magnification<=plotter->get_plotWidth()+plotter->get_iplotBorderLeft())  &&
          ((event->y()-getPlotYOffset())/magnification>=plotter->get_iplotBorderTop()) && ((event->y()-getPlotYOffset())/magnification<=plotter->get_plotHeight()+plotter->get_iplotBorderTop()) ) {
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
            double xmin=plotter->p2x((long)round((double)plotter->get_iplotBorderLeft()-(double)plotter->get_plotWidth()/2.0));
            double xmax=plotter->p2x((long)round((double)plotter->get_iplotBorderLeft()+1.5*(double)plotter->get_plotWidth()));
            double ymin=plotter->p2y((long)round((double)plotter->get_iplotBorderTop()+1.5*(double)plotter->get_plotHeight()));
            double ymax=plotter->p2y((long)round((double)plotter->get_iplotBorderTop()-(double)plotter->get_plotHeight()/2.0));
            if (plotter->get_xAxis()->isLogAxis()) {
                if (xmin<=0) xmin=1;
                if (xmax<=0) xmax=10;
            }
            if (plotter->get_yAxis()->isLogAxis()) {
                if (ymin<=0) ymin=1;
                if (ymax<=0) ymax=10;
            }
            /*plotter->get_xAxis->setRange(xmin, xmax);
            plotter->get_yAxis->setRange(ymin, ymax);*/
            //update_plot();
            /*if (plotter->get_emitSignals())*/ emit zoomChangedLocally(xmin, xmax, ymin, ymax, this);
            plotter->setXY(xmin, xmax, ymin, ymax);
            //update();
            event->accept();
        } else if (rightMouseButtonAction==JKQTPlotter::RightMouseButtonContextMenu) {
            openContextMenu(event->x(), event->y());

            event->accept();
        }
    }

    // emit clicked signal, if event occured inside plot only
    if  ( (event->x()/magnification>=plotter->get_iplotBorderLeft()) && (event->x()/magnification<=plotter->get_plotWidth()+plotter->get_iplotBorderLeft())  &&
          ((event->y()-getPlotYOffset())/magnification>=plotter->get_iplotBorderTop()) && ((event->y()-getPlotYOffset())/magnification<=plotter->get_plotHeight()+plotter->get_iplotBorderTop()) ) {
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
    if  ( (event->x()/magnification>=plotter->get_iplotBorderLeft()) && (event->x()/magnification<=plotter->get_plotWidth()+plotter->get_iplotBorderLeft())  &&
          ((event->y()-getPlotYOffset())/magnification>=plotter->get_iplotBorderTop()) && ((event->y()-getPlotYOffset())/magnification<=plotter->get_plotHeight()+plotter->get_iplotBorderTop()) ) {

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
            double xmin=plotter->p2x((long)round((double)event->x()/magnification-(double)plotter->get_plotWidth()/factor));
            double xmax=plotter->p2x((long)round((double)event->x()/magnification+(double)plotter->get_plotWidth()/factor));
            double ymin=plotter->p2y((long)round((double)event->y()/magnification-(double)getPlotYOffset()+(double)plotter->get_plotHeight()/factor));
            double ymax=plotter->p2y((long)round((double)event->y()/magnification-(double)getPlotYOffset()-(double)plotter->get_plotHeight()/factor));

            event->accept();
            //xAxis->setRange(xmin, xmax);
            //yAxis->setRange(ymin, ymax);
            //update_plot();
            /*if (plotter->get_emitSignals())*/ emit zoomChangedLocally(xmin, xmax, ymin, ymax, this);
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
    if  ( (event->x()/magnification>=plotter->get_iplotBorderLeft()) && (event->x()/magnification<=plotter->get_plotWidth()+plotter->get_iplotBorderLeft())  &&
          ((event->y()-getPlotYOffset())/magnification>=plotter->get_iplotBorderTop()) && ((event->y()-getPlotYOffset())/magnification<=plotter->get_plotHeight()+plotter->get_iplotBorderTop()) ) {
        double factor=pow(2.0, 1.0*(double)event->delta()/120.0)*2.0;
        //std::cout<<(double)event->delta()/120.0<<":   "<<factor<<std::endl;
        double xmin=plotter->p2x((long)round((double)event->x()/magnification-(double)plotter->get_plotWidth()/factor));
        double xmax=plotter->p2x((long)round((double)event->x()/magnification+(double)plotter->get_plotWidth()/factor));
        double ymin=plotter->p2y((long)round((double)event->y()/magnification-(double)getPlotYOffset()+(double)plotter->get_plotHeight()/factor));
        double ymax=plotter->p2y((long)round((double)event->y()/magnification-(double)getPlotYOffset()-(double)plotter->get_plotHeight()/factor));

        //std::cout<<"t="<<t<<", w="<<w<<"     =>     "<<tmin<<" -- "<<tmax<<std::endl;
        event->accept();
        /*if (plotter->get_emitSignals())*/ emit zoomChangedLocally(xmin, xmax, ymin, ymax, this);
        plotter->setXY(xmin, xmax, ymin, ymax);
        //update();
        /*xAxis->setRange(xmin, xmax);
        yAxis->setRange(ymin, ymax);
        //update_plot();
        if (emitSignals) emit zoomChangedLocally(xAxis->getMin(), xAxis->getMax(), yAxis->getMin(), yAxis->getMax(), this);*/
    } else { event->ignore(); }
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

    contextMenu->addAction(plotter->get_actSaveData());
    contextMenu->addAction(plotter->get_actSavePlot());
    contextMenu->addAction(plotter->get_actPrint());
    contextMenu->addSeparator();
    contextMenu->addAction(plotter->get_actCopyPixelImage());
    contextMenu->addAction(plotter->get_actCopyData());
    contextMenu->addAction(plotter->get_actCopyMatlab());
    contextMenu->addSeparator();
    contextMenu->addAction(plotter->get_actShowPlotData());
    contextMenu->addSeparator();
    contextMenu->addAction(plotter->get_actZoomAll());
    contextMenu->addAction(plotter->get_actZoomIn());
    contextMenu->addAction(plotter->get_actZoomOut());
    contextMenu->addSeparator();
    QMenu* menVisibleGroup=new QMenu(tr("Graph Visibility"), contextMenu);
    for (size_t i=0; i<get_plotter()->getGraphCount(); i++) {
        QString tit=get_plotter()->getGraph(i)->get_title();
        if (tit.isEmpty()) tit=tr("Graph %1").arg(static_cast<int>(i));
        QAction* act=new QAction(tit, menVisibleGroup);
        act->setCheckable(true);
        act->setChecked(get_plotter()->getGraph(i)->get_visible());
        act->setIcon(QIcon(QPixmap::fromImage(get_plotter()->getGraph(i)->generateKeyMarker(QSize(16,16)))));
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
    JKQTBasePlotter::AdditionalActionsMap lst=get_plotter()->get_lstAdditionalPlotterActions();
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

void JKQTPlotter::update_overlays() {
    if (!doDrawing) return;
    disconnect(plotter, SIGNAL(plotUpdated()), this, SLOT(update_plot()));
    disconnect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(replot_overlays()));
    image=imageNoOverlays;
    JKQTPEnhancedPainter painter(&image);
    if (painter.isActive()) {
        painter.scale(magnification, magnification);
        plotter->drawNonGridOverlays(painter);
    }
    oldImage=image;
    connect(plotter, SIGNAL(plotUpdated()), this, SLOT(update_plot()));
    connect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(replot_overlays()));
    update();
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

void JKQTPlotter::replot_overlays() {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTPlotter::replot_overlays()"));
#endif
    if (!doDrawing) return;
    disconnect(plotter, SIGNAL(plotUpdated()), this, SLOT(update_plot()));
    disconnect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(replot_overlays()));
    image=imageNoOverlays;
    JKQTPEnhancedPainter painter(&image);
    if (painter.isActive()) {
        painter.scale(magnification, magnification);
        plotter->drawNonGridOverlays(painter);
    }
    oldImage=image;
    connect(plotter, SIGNAL(plotUpdated()), this, SLOT(update_plot()));
    connect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(replot_overlays()));
    repaint();
}

void JKQTPlotter::update_plot() {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTPlotter::update_plot()"));
#endif
    if (!doDrawing) return;
    disconnect(plotter, SIGNAL(plotUpdated()), this, SLOT(update_plot()));
    disconnect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(replot_overlays()));
    plotter->resize(width()/magnification, height()/magnification-getPlotYOffset());
    JKQTPEnhancedPainter painter(&image);
    if (painter.isActive()) {
        painter.scale(magnification, magnification);
        //QTime t;
        //t.start();
        plotter->drawNonGrid(painter, QPoint(0,0), false);//, QRect(QPoint(0,0), QSize(plotter->get_plotWidth(), plotter->get_plotHeight())));
        //qDebug()<<"drawNonGrid"<<objectName()<<": "<<t.elapsed()<<"ms";
        imageNoOverlays=image;
        plotter->drawNonGridOverlays(painter);
    }
    oldImage=image;
    connect(plotter, SIGNAL(plotUpdated()), this, SLOT(update_plot()));
    connect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(replot_overlays()));
    update();
}

void JKQTPlotter::replot_plot() {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaaot(QString("JKQTPlotter::replot_plot()"));
#endif
    if (!doDrawing) return;
    disconnect(plotter, SIGNAL(plotUpdated()), this, SLOT(update_plot()));
    disconnect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(replot_overlays()));

    plotter->resize(width()/magnification, height()/magnification-getPlotYOffset());
    JKQTPEnhancedPainter painter(&image);
    if (painter.isActive()) {
        painter.scale(magnification, magnification);
        plotter->drawNonGrid(painter);//, QRect(QPoint(0,0), QSize(plotter->get_plotWidth(), plotter->get_plotHeight())));
    }
    oldImage=image;
    connect(plotter, SIGNAL(plotUpdated()), this, SLOT(update_plot()));
    connect(plotter, SIGNAL(overlaysUpdated()), this, SLOT(replot_overlays()));
    repaint();
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
            p->drawText(plotter->get_iplotBorderLeft(), getPlotYOffset()-1, mousePositionTemplate.arg(mousePosX).arg(mousePosY));
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

    if (sizeChanged) update_plot();
}

void JKQTPlotter::leaveEvent(QEvent * /*event*/) {
    if (!toolbarAlwaysOn) toolbar->hide();
}

void JKQTPlotter::updateToolbar()
{
    if (displayToolbar) {
        if (toolbarAlwaysOn) {
            toolbar->set_toolbarVanishes(false);
            toolbar->show();
            toolbar->move(1,1);
        } else {
            toolbar->set_toolbarVanishes(true);
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
    return m;//QSize((int)round((plotter->get_iplotBorderLeft()+plotter->get_iplotBorderRight())*1.5), (int)round((plotter->get_iplotBorderTop()+plotter->get_iplotBorderBottom())*1.5));
}

QSize JKQTPlotter::sizeHint() const {
    return QWidget::sizeHint(); //minimumSizeHint()+QSize(200,100);
}



void JKQTPlotter::masterPlotScalingRecalculated() {
    if (masterPlotter) {
        if (plotter->get_masterSynchronizeHeight()||plotter->get_masterSynchronizeWidth()) {
            update_plot();
        }
    }
}

void JKQTPlotter::synchronizeToMaster(JKQTPlotter* master, bool synchronizeWidth, bool synchronizeHeight) {
    if (!master) {
        resetMasterSynchronization();
    }
    plotter->synchronizeToMaster(master->get_plotter(), synchronizeWidth, synchronizeHeight);
    masterPlotter=master;
    if (masterPlotter) connect(masterPlotter->get_plotter(), SIGNAL(plotScalingRecalculated()), this, SLOT(masterPlotScalingRecalculated()));
    update_plot();
}

void JKQTPlotter::resetMasterSynchronization() {
    if (masterPlotter) disconnect(masterPlotter->get_plotter(), SIGNAL(plotScalingRecalculated()), this, SLOT(masterPlotScalingRecalculated()));
    plotter->resetMasterSynchronization();
    update_plot();
}

bool JKQTPlotter::get_doDrawing() const {
    return doDrawing;
}

void JKQTPlotter::pzoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQTBasePlotter* /*sender*/) {
    emit zoomChangedLocally(newxmin, newxmax, newymin, newymax, this);
    minSize=QSize(plotter->get_iplotBorderLeft()+plotter->get_iplotBorderRight()+10, plotter->get_iplotBorderTop()+plotter->get_iplotBorderBottom()+10);
}

void JKQTPlotter::intBeforePlotScalingRecalculate() {
    emit beforePlotScalingRecalculate();
}

void JKQTPlotter::reactGraphVisible(bool visible)
{
    QAction* act=dynamic_cast<QAction*>(sender());
    if (act) {
        get_plotter()->setGraphVisible(act->data().toInt(), visible);
    }
}

void JKQTPlotter::set_zoomByMouseRectangle(bool zomByrectangle) {
    if (zomByrectangle) mouseActionMode=JKQTPlotter::ZoomRectangle;
    else mouseActionMode=JKQTPlotter::NoMouseAction;
}

void JKQTPlotter::set_rightMouseButtonAction(const JKQTPlotter::RightMouseButtonAction &__value)
{
    this->rightMouseButtonAction = __value;
}

JKQTPlotter::RightMouseButtonAction JKQTPlotter::get_rightMouseButtonAction() const
{
    return this->rightMouseButtonAction;
}

void JKQTPlotter::set_leftDoubleClickAction(const JKQTPlotter::LeftDoubleClickAction &__value)
{
    this->leftDoubleClickAction = __value;
}

JKQTPlotter::LeftDoubleClickAction JKQTPlotter::get_leftDoubleClickAction() const
{
    return this->leftDoubleClickAction;
}

QMenu *JKQTPlotter::get_menuSpecialContextMenu() const {
    return this->menuSpecialContextMenu;
}

void JKQTPlotter::set_menuSpecialContextMenu(QMenu *menu)
{
    menuSpecialContextMenu=menu;
    if (menuSpecialContextMenu) {
        menuSpecialContextMenu->setParent(this);
        menuSpecialContextMenu->close();
    }
}

void JKQTPlotter::set_zoomByMouseWheel(bool __value)
{
    this->zoomByMouseWheel = __value;
}

bool JKQTPlotter::get_zoomByMouseWheel() const
{
    return this->zoomByMouseWheel;
}

double JKQTPlotter::get_mouseContextX() const {
    return this->mouseContextX;
}

double JKQTPlotter::get_mouseContextY() const {
    return this->mouseContextY;
}

int JKQTPlotter::get_mouseLastClickX() const {
    return this->mouseLastClickX;
}

int JKQTPlotter::get_mouseLastClickY() const {
    return this->mouseLastClickY;
}

void JKQTPlotter::setMagnification(double m)
{
    magnification=m;
    update_plot();
}

bool JKQTPlotter::get_zoomByMouseRectangle() const {
    return (mouseActionMode==JKQTPlotter::ZoomRectangle);
}

void JKQTPlotter::modifyContextMenu(QMenu * /*menu*/)
{
}

void JKQTPlotter::populateToolbar(QToolBar *toolbar) const
{
    toolbar->addAction(plotter->get_actSaveData());
    toolbar->addAction(plotter->get_actSavePlot());
    toolbar->addAction(plotter->get_actPrint());
    toolbar->addSeparator();
    toolbar->addAction(plotter->get_actCopyPixelImage());
    toolbar->addAction(plotter->get_actCopyData());
    toolbar->addAction(plotter->get_actCopyMatlab());
    toolbar->addSeparator();
    toolbar->addAction(plotter->get_actShowPlotData());
    toolbar->addSeparator();
    toolbar->addAction(plotter->get_actZoomAll());
    toolbar->addAction(plotter->get_actZoomIn());
    toolbar->addAction(plotter->get_actZoomOut());

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


void JKQTPlotter::set_doDrawing(bool enable)
{
    doDrawing=enable;
    plotter->set_emitSignals(enable);
    //qDebug()<<objectName()<<"  doDrawing="<<doDrawing;
}


void initJKQTPlotterResources()
{
    initJKQTBasePlotterResources();
}
