/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    last modification: $LastChangedDate$  (revision $Rev$)

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



#include "jkqtfastplotter.h"
#include <QLocale>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QFontMetricsF>
#include <QAction>
#include <QApplication>
#include <QClipboard>

JKQTFPPlot::JKQTFPPlot(JKQTFastPlotter* parent):
    QObject(parent)

{
    this->parent=parent;
    this->visible=true;
};

void JKQTFPPlot::replot() {
    if (parent) parent->update_data();
};

void JKQTFPPlot::paint(QPainter& painter) {
    if (visible) drawGraph(painter);
}

JKQTFastPlotter::JKQTFastPlotter(QWidget *parent) :
    JKQTFASTPLOTTER_BASEWIDGET_CONSTRUCTOR(parent)//, mutexRepaint(QMutex::Recursive), mutexRepaintData(QMutex::Recursive), mutexRepaintSystem(QMutex::Recursive)
{
    mouseDragStart=QPoint(0,0);
    mouseDragEnd=QPoint(0,0);
    dragging=true;

    dragShapePen=QPen(QColor("red"));
    dragShapePen.setStyle(Qt::DashLine);
    dragLine=false;
    doFullRepaint=false;
    plotBorderLeft=def_plotBorderLeft=50;
    plotBorderRight=def_plotBorderRight=30;
    plotBorderTop=def_plotBorderTop=5;
    plotBorderBottom=def_plotBorderBottom=30;

    synchronizeX=NULL;
    synchronizeY=NULL;

    backgroundColor=def_backgroundColor=palette().color(QPalette::Window);
    plotBackgroundColor=def_plotBackgroundColor=QColor("white");
    drawGrid=def_drawGrid=true;
    gridColor=def_gridColor=QColor("lightgrey");
    gridStyle=def_gridStyle=Qt::DashLine;
    gridWidth=def_gridWidth=1;
    labelFontSize=def_labelFontSize=font().pointSizeF();
    labelFontName=def_labelFontName=font().family();
    tickFontSize=def_tickFontSize=font().pointSizeF();
    tickFontName=def_tickFontName=font().family();
    tickLength=def_tickLength=3;
    drawSystemBox=def_drawSystemBox=true;
    drawZeroAxes=def_drawZeroAxes=true;
    systemColor=def_systemColor=QColor("black");
    systemWidth=def_systemWidth=2;
    xZeroTick=0;
    yZeroTick=0;
    xTickDistance=1;
    yTickDistance=1;
    xAxisLabel="x";
    yAxisLabel="y";
    xAxisLabelVisible=true;
    yAxisLabelVisible=true;
    xAxisLog=false;
    yAxisLog=false;
    aspectRatio=1;
    maintainAspectRatio=false;
    doDrawing=false;
    xMin=0;
    xMax=10;
    yMin=0;
    yMax=10;
    resize(400,200);
    setXRange(0, 10);
    setYRange(0,10);
    doDrawing=true;

    actCopyImage=new QAction(tr("copy image to clipboard"), this);
    connect(actCopyImage, SIGNAL(triggered()), this, SLOT(copyImage()));
    addAction(actCopyImage);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    update_plot();
    setMouseTracking(true);
}

void JKQTFastPlotter::clearPlots(bool remove) {
    if (remove) {
        for (int i=0; i<plots.size(); i++) {
            delete plots[i];
        }
    }
    plots.clear();
    update_plot();
}


void JKQTFastPlotter::addPlot(JKQTFPPlot* g) {
    g->setParent(this);
    plots.append(g);
    update_plot();
}


void JKQTFastPlotter::deletePlot(int i, bool remove) {
    if (i>=0 && i<plots.size()) {
        if (remove) delete plots[i];
        plots.remove(i);
        update_plot();
    }
}


void JKQTFastPlotter::deletePlot(JKQTFPPlot* g, bool remove) {
    int i=plots.indexOf(g);
    if (i>=0 && i<plots.size()) {
        if (remove) delete plots[i];
        plots.remove(i);
        update_plot();
    }
}

int JKQTFastPlotter::getPlotNum(JKQTFPPlot* g) {
    return plots.indexOf(g);
}

void JKQTFastPlotter::paintEvent(QPaintEvent *event) {
#ifdef DEBUG
    qDebug()<<"JKQTFastPlotter::paintEvent";
#endif
    // draw the saved image of the plot
    mutexRepaint.lock();
    QPainter* painter = new QPainter(this);
    //painter->setRenderHints(QPainter::HighQualityAntialiasing|QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    if (painter->isActive()) {
        //qDebug()<<"JKQTFastPlotter::paintEvent:   image: "<<image.width()<<" x "<<image.height()<<":   "<<image.isNull();
        //qDebug()<<"image.format: "<<image.format()<<"   painter.componsitionMode: "<<painter->compositionMode();
        //image.save(objectName()+"_jkqtfastplotter.png");
        painter->drawImage(QPoint(0, 0), image);
        if (dragging) {
            if (dragLine) {
                painter->setPen(dragShapePen);
                painter->drawLine(mouseDragStart, mouseDragEnd);
            }
        }
    }
    delete painter;
    event->accept();
    mutexRepaint.unlock();
#ifdef DEBUG
    qDebug()<<"JKQTFastPlotter::paintEvent ... done";
#endif
}

void JKQTFastPlotter::copyImage()
{
    mutexRepaint.lock();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setImage(image);
    mutexRepaint.unlock();
}


void JKQTFastPlotter::mouseDoubleClickEvent ( QMouseEvent * event ) {
   if (event->button()==Qt::LeftButton) {
        double x=p2x(event->x());
        double y=p2y(event->y());
        emit doubleClicked(x, y);
        emit doubleClicked(x, y, event->modifiers());
        event->accept();
   }
}

void JKQTFastPlotter::mouseMoveEvent ( QMouseEvent * event ) {
    double x=p2x(event->x());
    double y=p2y(event->y());
    emit mouseMoved(x, y);
    //qDebug()<<"JKQTFastPlotter::mouseMoveEvent  "<<x<<y;
    if (event->buttons()&Qt::LeftButton) {
        dragging=true;
        double xd=p2x(mouseDragStart.x());
        double yd=p2y(mouseDragStart.y());
        mouseDragEnd=event->pos();
        //qDebug()<<"JKQTFastPlotter::mouseMoveEvent: dragged:  "<<xd<<yd<<x<<y;
        emit mouseDragged(xd, yd, x, y, event->modifiers());
        if (dragLine) update();
    }
    event->accept();
}

void JKQTFastPlotter::mousePressEvent ( QMouseEvent * event ) {
   if (event->button()==Qt::LeftButton) {
        double x=p2x(event->x());
        double y=p2y(event->y());
        emit clicked(x, y);
        emit clicked(x, y, event->modifiers());
        mouseDragStart=event->pos();
        event->accept();
   }
}

void JKQTFastPlotter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button()==Qt::LeftButton) {
        double xd=p2x(mouseDragStart.x());
        double yd=p2y(mouseDragStart.y());
        double x=p2x(event->x());
        double y=p2y(event->y());
        emit mouseDragged(xd, yd, x, y, event->modifiers());
        emit mouseDragFinished(xd, yd, x, y, event->modifiers());
        dragging=false;
        if (dragLine) update();
    }
    event->accept();
}

void JKQTFastPlotter::resizeEvent(QResizeEvent *event) {
     JKQTFASTPLOTTER_BASEWIDGET::resizeEvent(event);
     if (width() > image.width() || height() > image.height()) {
         QImage newImage(QSize(width(), height()), QImage::Format_ARGB32);
         image=newImage;
         systemImage=newImage;
     }
     //std::cout<<"replotting after resize to ("<<width()<<" x "<<height()<<")...\n";
     update_plot();
     event->accept();
     updateGeometry();
}

QSize JKQTFastPlotter::minimumSizeHint() const
{
    if (isVisible()) return QSize((int)round((plotBorderLeft+plotBorderRight)*2.0), (int)round((plotBorderTop+plotBorderBottom)*2.0));
    else return QSize(0,0);
}

void JKQTFastPlotter::plotSystem(QPainter& painter) {
    #ifdef DEBUG_TIMING
    JKQTPHighResTimer timer;
    double time_sum=0;
    double time_gt=0;
    std::cout<<"timing plotSystem():\n";
    timer.start();
    #endif
    painter.save();
    ///////////////////////////////////////////////////////////////////
    // FILL WIDGET BACKGROUND
    ///////////////////////////////////////////////////////////////////
    if (backgroundColor!=QColor(Qt::transparent)) painter.fillRect(this->rect(), QBrush(backgroundColor));
    ///////////////////////////////////////////////////////////////////
    // FILL PLOT BACKGROUND
    ///////////////////////////////////////////////////////////////////
    //qDebug()<<plotBackgroundColor;
    painter.fillRect(QRectF(internalPlotBorderLeft, internalPlotBorderTop, plotWidth, plotHeight), QBrush(plotBackgroundColor));

    #ifdef DEBUG_TIMING
    time_gt=timer.get_time();
    time_sum+=time_gt;
    std::cout<<"    filling backgrounds: "<<time_gt/1000.0<<" ms\n";
    timer.start();
    #endif

    ///////////////////////////////////////////////////////////////////
    // CREATE DIFFERENT  QPENs, QFONTs ...
    ///////////////////////////////////////////////////////////////////
    QPen p=painter.pen();
    QPen pSystem(systemColor);
    pSystem.setWidthF(qMax(JKQTFASTPLOTTER_ABS_MIN_LINEWIDTH, systemWidth));
    pSystem.setJoinStyle(Qt::MiterJoin);
    pSystem.setCapStyle(Qt::SquareCap);
    QPen pGrid(gridColor);
    pGrid.setWidthF(qMax(JKQTFASTPLOTTER_ABS_MIN_LINEWIDTH, gridWidth));
    pGrid.setStyle(gridStyle);
    pGrid.setJoinStyle(Qt::MiterJoin);
    pGrid.setCapStyle(Qt::SquareCap);
    QFont fTicks(tickFontName);
    fTicks.setPointSizeF(tickFontSize);
    QFontMetricsF fmTicks(fTicks, &systemImage);
    QFont fLabels(labelFontName);
    fLabels.setPointSizeF(labelFontSize);
    QFontMetricsF fmLabels(fLabels, &systemImage);

    #ifdef DEBUG_TIMING
    time_gt=timer.get_time();
    time_sum+=time_gt;
    std::cout<<"    creating pens ...: "<<time_gt/1000.0<<" ms\n";
    timer.start();
    #endif

    ///////////////////////////////////////////////////////////////////
    // DRAW GRID AND TICKS into QPainterPaths
    ///////////////////////////////////////////////////////////////////
    painter.setFont(fTicks);
    painter.setPen(pSystem);
    QPainterPath gridPath, systemPath;
    double x=xZeroTick;
    if (xAxisLog && xZeroTick<=0) x=xMin;
    int cntr=0; // used to stop plotting after 1000 labels (both axes summed up)
    while (x<=xMax && cntr<1000) {
        if (xMin<=x && x<=xMax) {
            gridPath.moveTo(x2p(x), internalPlotBorderTop);
            gridPath.lineTo(x2p(x), internalPlotBorderTop+plotHeight);
            systemPath.moveTo(x2p(x), internalPlotBorderTop+plotHeight+tickLength);
            systemPath.lineTo(x2p(x), internalPlotBorderTop+plotHeight-tickLength);
            QString text=QLocale::system().toString(x);
            painter.drawText(QPointF(x2p(x)-fmTicks.width(text)/2.0, internalPlotBorderTop+plotHeight+fmTicks.ascent()+fmTicks.width("x")/2.0+tickLength), text);
        }
        if (xAxisLog) {
            x=x*10.0;
        } else {
             x=x+xTickDistance;
        }
        cntr++;
    }
    x=xZeroTick;
    if (xAxisLog && xZeroTick<=0) x=xMin;
    while (x>=xMin && cntr<1000) {
        if (xMin<=x && x<=xMax) {
            gridPath.moveTo(x2p(x), internalPlotBorderTop);
            gridPath.lineTo(x2p(x), internalPlotBorderTop+plotHeight);
            systemPath.moveTo(x2p(x), internalPlotBorderTop+plotHeight+tickLength);
            systemPath.lineTo(x2p(x), internalPlotBorderTop+plotHeight-tickLength);
            QString text=QLocale::system().toString(x);
            painter.drawText(QPointF(x2p(x)-fmTicks.width(text)/2.0, internalPlotBorderTop+plotHeight+fmTicks.ascent()+fmTicks.width("x")/2.0+tickLength), text);
        }
        if (xAxisLog) {
            x=x/10.0;
        } else {
             x=x-xTickDistance;
        }
        cntr++;
    }

    double y=yZeroTick;
    if (yAxisLog && yZeroTick<=0) y=yMin;
    while (y<=yMax && cntr<1000) {
        if (yMin<=y && y<=yMax) {
            gridPath.moveTo(internalPlotBorderLeft, y2p(y));
            gridPath.lineTo(internalPlotBorderLeft+plotWidth, y2p(y));
            systemPath.moveTo(internalPlotBorderLeft-tickLength, y2p(y));
            systemPath.lineTo(internalPlotBorderLeft+tickLength, y2p(y));
            QString text=QLocale::system().toString(y);
            painter.drawText(QPointF(internalPlotBorderLeft-fmTicks.width("x")/2.0-fmTicks.width(text)-tickLength, y2p(y)+fmTicks.ascent()/2.0), text);
        }
        if (yAxisLog) {
            y=y*10.0;
        } else {
             y=y+yTickDistance;
        }
        cntr++;
    }
    y=yZeroTick;
    if (yAxisLog && yZeroTick<=0) y=yMin;
    while (y>=yMin && cntr<1000) {
        if (yMin<=y && y<=yMax) {
            gridPath.moveTo(internalPlotBorderLeft, y2p(y));
            gridPath.lineTo(internalPlotBorderLeft+plotWidth, y2p(y));
            systemPath.moveTo(internalPlotBorderLeft-tickLength, y2p(y));
            systemPath.lineTo(internalPlotBorderLeft+tickLength, y2p(y));
            QString text=QLocale::system().toString(y);
            painter.drawText(QPointF(internalPlotBorderLeft-fmTicks.width("x")/2.0-fmTicks.width(text)-tickLength, y2p(y)+fmTicks.ascent()/2.0), text);
        }
        if (yAxisLog) {
            y=y/10.0;
        } else {
             y=y-yTickDistance;
        }
        cntr++;
    }
    #ifdef DEBUG_TIMING
    time_gt=timer.get_time();
    time_sum+=time_gt;
    std::cout<<"    calcing system & grid: "<<time_gt/1000.0<<" ms\n";
    timer.start();
    #endif


    ///////////////////////////////////////////////////////////////////
    // DRAW GRID
    ///////////////////////////////////////////////////////////////////
    if (drawGrid) {
        painter.setPen(pGrid);
        painter.drawPath(gridPath);
    }
    #ifdef DEBUG_TIMING
    time_gt=timer.get_time();
    time_sum+=time_gt;
    std::cout<<"    drawing grid: "<<time_gt/1000.0<<" ms\n";
    timer.start();
    #endif
    ///////////////////////////////////////////////////////////////////
    // DRAW SYSTEM TICKS
    ///////////////////////////////////////////////////////////////////
    painter.setPen(pSystem);
    painter.drawPath(systemPath);
    #ifdef DEBUG_TIMING
    time_gt=timer.get_time();
    time_sum+=time_gt;
    std::cout<<"    drawing system: "<<time_gt/1000.0<<" ms\n";
    timer.start();
    #endif

    ///////////////////////////////////////////////////////////////////
    // DRAW AXIS LABELS
    ///////////////////////////////////////////////////////////////////
    if (xAxisLabelVisible) {
        painter.setPen(pSystem);
        painter.setFont(fLabels);
        painter.drawText(QPointF(internalPlotBorderLeft+plotWidth-fmLabels.width(xAxisLabel), internalPlotBorderTop+plotHeight+fmTicks.height()+fmTicks.width("x")/2.0+fmLabels.ascent()+tickLength), xAxisLabel);
    }
    if (yAxisLabelVisible) {
        painter.save();
        painter.translate(fmLabels.ascent(), internalPlotBorderTop+fmLabels.width(yAxisLabel));
        painter.rotate(-90);
        painter.drawText(QPointF(0, 0), yAxisLabel);
        painter.restore();
    }
    #ifdef DEBUG_TIMING
    time_gt=timer.get_time();
    time_sum+=time_gt;
    std::cout<<"    drawing labels: "<<time_gt/1000.0<<" ms\n";
    timer.start();
    #endif

    ///////////////////////////////////////////////////////////////////
    // DRAW SYSTEM BOX
    ///////////////////////////////////////////////////////////////////
    if (drawSystemBox) {
        painter.setPen(pSystem);
        painter.drawRect(QRectF(internalPlotBorderLeft, internalPlotBorderTop, plotWidth, plotHeight));
    }

    ///////////////////////////////////////////////////////////////////
    // DRAW AXES at x==0 / y==0
    ///////////////////////////////////////////////////////////////////
    if (drawZeroAxes) {
        painter.setPen(pSystem);
        if (xMin<=0 && 0<=xMax) painter.drawLine(QLineF(x2p(0), internalPlotBorderTop, x2p(0), internalPlotBorderTop+plotHeight));
        if (yMin<=0 && 0<=yMax) painter.drawLine(QLineF(internalPlotBorderLeft, y2p(0), internalPlotBorderLeft+plotWidth, y2p(0)));
    }
    #ifdef DEBUG_TIMING
    time_gt=timer.get_time();
    time_sum+=time_gt;
    std::cout<<"    drawing boxes and axes: "<<time_gt/1000.0<<" ms\n";
    timer.start();
    #endif

    painter.restore();
    #ifdef DEBUG_TIMING
    time_gt=timer.get_time();
    time_sum+=time_gt;
    std::cout<<"  sum: "<<time_sum/1000.0<<" ms\n";
    timer.start();
    #endif
}

void JKQTFastPlotter::plotGraphs(QPainter& painter) {
    #ifdef DEBUG_TIMING
    JKQTPHighResTimer timer;
    double time_sum=0;
    double time_gt=0;
    std::cout<<"timing plotGraphs():\n";
    timer.start();
    #endif
    painter.save();
    for (int i=0; i<plots.size(); i++) {
        #ifdef DEBUG_TIMING
        timer.start();
        #endif
        plots[i]->paint(painter);
        #ifdef DEBUG_TIMING
        time_gt=timer.get_time();
        time_sum+=time_gt;
        std::cout<<"    drawing graph "<<i<<": "<<time_gt/1000.0<<" ms\n";
        timer.start();
        #endif
    }
    painter.restore();
    #ifdef DEBUG_TIMING
    time_gt=timer.get_time();
    time_sum+=time_gt;
    std::cout<<"  sum: "<<time_sum/1000.0<<" ms\n";
    timer.start();
    #endif
}

void JKQTFastPlotter::draw(QPainter* painter, QSize* size) {
    calcPlotScaling();
    if (size) *size=QSize(width(), height());
    painter->save();
    plotSystem(*painter);
    painter->setClipRect(QRectF(internalPlotBorderLeft+systemWidth/2.0, internalPlotBorderTop+systemWidth/2.0, plotWidth-systemWidth, plotHeight-systemWidth));
    plotGraphs(*painter);
    painter->restore();
}

void JKQTFastPlotter::draw(QPainter* painter, QColor background, QSize* size) {
    QColor oldb=backgroundColor;
    backgroundColor=background;
    calcPlotScaling();
    if (size) *size=QSize(width(), height());
    painter->save();
    plotSystem(*painter);
    //painter->setClipRect(QRectF(internalPlotBorderLeft+systemWidth/2.0, internalPlotBorderTop+systemWidth/2.0, plotWidth-systemWidth, plotHeight-systemWidth));
    plotGraphs(*painter);
    painter->restore();
    backgroundColor=oldb;
}

void JKQTFastPlotter::update_plot() {
    calcPlotScaling();
    if (!doDrawing) {
        doFullRepaint=true;
        return;
    }
    if (systemImage.isNull()) return;
    mutexRepaintSystem.lock();
    QPainter* painter=new QPainter(&systemImage);
    //painter->setRenderHints(QPainter::HighQualityAntialiasing|QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    if (painter->isActive()) {
        plotSystem(*painter);
    }
    delete painter;
    mutexRepaintSystem.unlock();
    doFullRepaint=false;
    update_data();
}

void JKQTFastPlotter::update_plot_immediate() {
    calcPlotScaling();
    if (!doDrawing) {
        doFullRepaint=true;
        return;
    }
    if (systemImage.isNull()) return;
    mutexRepaintSystem.lock();
    QPainter* painter=new QPainter(&systemImage);
    if (painter->isActive()) {
        plotSystem(*painter);
    }
    delete painter;
    mutexRepaintSystem.unlock();
    doFullRepaint=false;
    update_data_immediate();
}

void JKQTFastPlotter::update_data() {
    if (doFullRepaint) {
        // as doFullRepaint is set false in updtae_plot() this is NO endles loop!!!!!!!
        update_plot();
        return;
    }
    if (!doDrawing) return;
    if (image.isNull()) return;
    mutexRepaintData.lock();
    emit replotting();
    image=systemImage;
    QPainter* painter=new QPainter(&image);
    //painter->setRenderHints(QPainter::HighQualityAntialiasing|QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    if (painter->isActive()) {
        /*qDebug()<<"image.format: "<<image.format();
        qDebug()<<"painter.compositionMode: "<<painter->compositionMode();
        qDebug()<<"painter.renderHints: "<<painter->renderHints();*/
        //painter->setClipRect(QRectF(internalPlotBorderLeft+systemWidth/2.0, internalPlotBorderTop+systemWidth/2.0, plotWidth-systemWidth, plotHeight-systemWidth));
        plotGraphs(*painter);
    }
    delete painter;
    mutexRepaintData.unlock();
    update();
}

void JKQTFastPlotter::update_data_immediate() {
    if (doFullRepaint) {
        // as doFullRepaint is set false in updtae_plot() this is NO endles loop!!!!!!!
        update_plot_immediate();
        return;
    }
    if (!doDrawing) return;
    if (image.isNull()) return;
    mutexRepaintData.lock();
    emit replotting();
    image=systemImage;
    QPainter* painter=new QPainter(&image);
    //painter->setRenderHints(QPainter::HighQualityAntialiasing|QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    if (painter->isActive()) {
        //painter->setClipRect(QRectF(internalPlotBorderLeft+systemWidth/2.0, internalPlotBorderTop+systemWidth/2.0, plotWidth-systemWidth, plotHeight-systemWidth));
        plotGraphs(*painter);
    }
    delete painter;
    mutexRepaintData.unlock();
    repaint();
}

void JKQTFastPlotter::calcPlotScaling() {
    internalPlotBorderBottom=plotBorderBottom;
    internalPlotBorderTop=plotBorderTop;
    internalPlotBorderLeft=plotBorderLeft;
    internalPlotBorderRight=plotBorderRight;

    plotWidth=width()-internalPlotBorderLeft-internalPlotBorderRight;
    plotHeight=height()-internalPlotBorderBottom-internalPlotBorderTop;

    ////////////////////////////////////////////////////////////////////
    // X AXIS, PRESETTINGS
    ////////////////////////////////////////////////////////////////////
    if (xAxisLog) {
        if (xMin<=0) xMin=1e-30;
        if (xMax<=0) xMax=xMin+pow(10.0, (int)log10(xMin));
        if (xMax==xMin) {
            xMax=xMin+pow(10.0, (int)log10(xMin)+1);
        }
    }
    if (xMin>xMax) {
        double h=xMax;
        xMax=xMin;
        xMin=h;
    } else if (xMax==xMin) {
        xMax=xMin+1.0;
    }

    // this is the x- and y-range spanned by the plot
    xWidth=fabs(xMax-xMin);


    ////////////////////////////////////////////////////////////////////
    // Y AXIS, PRESETTING
    ////////////////////////////////////////////////////////////////////

    if (yAxisLog) {
        if (yMin<=0) yMin=1e-30;
        if (yMax<=0) yMax=yMin+pow(10.0, (int)log10(yMin));
        if (yMax==xMin) {
            yMax=yMin+pow(10.0, (int)log10(yMin)+1);
        }
    }
    if (yMin>yMax) {
        double h=yMax;
        yMax=yMin;
        yMin=h;
    } else if (yMax==yMin) {
        yMax=yMin+1.0;
    }


    // this is the x- and y-range spanned by the plot
    yWidth=fabs(yMax-yMin);



    ////////////////////////////////////////////////////////////////////
    // ENSURE ASPECT RATIO (if activated)
    ////////////////////////////////////////////////////////////////////
    //double pixelWidth=xWidth/(double)plotWidth;
    //double pixelHeight=yWidth/(double)plotHeight;
    if (maintainAspectRatio && (!xAxisLog) && (!yAxisLog)) {
        double currRatio=(double)plotWidth/(double)plotHeight;
        double newPlotWidth=plotWidth;
        double newPlotHeight=plotHeight;
        if (currRatio!=aspectRatio) {
            if (aspectRatio<=currRatio) {
                newPlotWidth=aspectRatio*(double)plotHeight;
            } else {
                newPlotHeight=(double)plotWidth/aspectRatio;
            }
        }
        double dx=plotWidth-newPlotWidth;
        double dy=plotHeight-newPlotHeight;
        internalPlotBorderBottom+=dy/2.0;
        internalPlotBorderTop+=dy/2.0;
        internalPlotBorderLeft+=dx/2.0;
        internalPlotBorderRight+=dx/2.0;
    }




    ////////////////////////////////////////////////////////////////////
    // PLOTTER SYNCHRONIZATION
    ////////////////////////////////////////////////////////////////////
    if (synchronizeX!=NULL) {
        internalPlotBorderLeft=synchronizeX->get_internalPlotBorderLeft();
        internalPlotBorderRight=synchronizeX->get_internalPlotBorderRight();
        xMin=synchronizeX->get_xMin();
        xMax=synchronizeX->get_xMax();
        xAxisLog=synchronizeX->get_xAxisLog();
        xAxisLabel=synchronizeX->get_xAxisLabel();
    }
    if (synchronizeY!=NULL) {
        internalPlotBorderTop=synchronizeY->get_internalPlotBorderTop();
        internalPlotBorderBottom=synchronizeY->get_internalPlotBorderBottom();
        yMin=synchronizeY->get_yMin();
        yMax=synchronizeY->get_yMax();
        yAxisLog=synchronizeY->get_yAxisLog();
        yAxisLabel=synchronizeY->get_yAxisLabel();
    }



    plotWidth=width()-internalPlotBorderLeft-internalPlotBorderRight;
    plotHeight=height()-internalPlotBorderBottom-internalPlotBorderTop;
    xWidth=fabs(xMax-xMin);
    yWidth=fabs(yMax-yMin);


    ////////////////////////////////////////////////////////////////////
    // X AXIS, CALC PARAMETERS
    ////////////////////////////////////////////////////////////////////
    if (xAxisLog) {
        xScale=plotWidth/(log10(xMax)-log10(xMin));
        xOffset=internalPlotBorderLeft-log10(xMin)*xScale;
    } else {
        xScale=plotWidth/xWidth;
        xOffset=internalPlotBorderLeft-xMin*xScale;

    }



    ////////////////////////////////////////////////////////////////////
    // Y AXIS, CALC PARAMETERS
    ////////////////////////////////////////////////////////////////////
    if (yAxisLog) {
        yScale=plotHeight/(log10(yMax)-log10(yMin));
        yOffset=internalPlotBorderTop+log10(yMax)*yScale;
    } else {
        yScale=plotHeight/yWidth;
        yOffset=internalPlotBorderTop+yMax*yScale;

    }

    emit plotterSizesChanged();
}

void JKQTFastPlotter::setXRange(double min, double max, bool logarithmic) {
    xMin=min;
    xMax=max;
    xAxisLog=logarithmic;
    calcPlotScaling();
    update_plot();
}

void JKQTFastPlotter::setYRange(double min, double max, bool logarithmic) {
    yMin=min;
    yMax=max;
    yAxisLog=logarithmic;
    calcPlotScaling();
    update_plot();
}

void JKQTFastPlotter::setXYRange(double xmin, double xmax, double ymin, double ymax, bool xlogarithmic, bool ylogarithmic) {
    xMin=xmin;
    xMax=xmax;
    xAxisLog=xlogarithmic;
    yMin=ymin;
    yMax=ymax;
    yAxisLog=ylogarithmic;
    calcPlotScaling();
    update_plot();
}

/*
void JKQTFastPlotter::synchronizeX(JKQTFastPlotter* toWhere) {
    bool olddo=doDrawing;
    doDrawing=false;
    xMin=toWhere->get_xMin();
    xMax=toWhere->get_xMax();
    xAxisLog=toWhere->get_xAxisLog();
    plotBorderLeft=toWhere->get_internalPlotBorderLeft();
    plotBorderRight=toWhere->get_internalPlotBorderRight();
    std::cout<<"syncX:    xMin="<<xMin<<"  xMax="<<xMax<<"  plotBorderLeft="<<plotBorderLeft<<"  plotBorderLeft="<<plotBorderRight<<"\n";
    doDrawing=olddo;
    update_plot();
}

void JKQTFastPlotter::synchronizeY(JKQTFastPlotter* toWhere) {
    bool olddo=doDrawing;
    doDrawing=false;
    yMin=toWhere->get_yMin();
    yMax=toWhere->get_yMax();
    yAxisLog=toWhere->get_yAxisLog();
    plotBorderTop=toWhere->get_internalPlotBorderTop();
    plotBorderBottom=toWhere->get_internalPlotBorderBottom();
    std::cout<<"syncY:     yMin="<<yMin<<"  yMax="<<yMax<<"  plotBorderTop="<<plotBorderTop<<"  plotBorderBottom="<<plotBorderBottom<<"\n";
    doDrawing=olddo;
    update_plot();
}
*/





JKQTFPLinePlot::JKQTFPLinePlot(JKQTFastPlotter* parent, unsigned int N, double* x, double* y, QColor color, Qt::PenStyle style, double width):
    JKQTFPPlot(parent)
{
    this->N=N;
    this->x=x;
    this->y=y;
    this->yerr=NULL;
    this->xv=NULL;
    this->yv=NULL;
    this->yerrv=NULL;
    this->color=color;
    this->errorColor=color.lighter();
    this->errorStyle=this->style=style;
    this->errorWidth=this->width=width;
    datatype=JKQTFPLPPointer;
}


JKQTFPLinePlot::JKQTFPLinePlot(JKQTFastPlotter* parent, QVector<double>* x, QVector<double>* y, QColor color, Qt::PenStyle style, double width):
    JKQTFPPlot(parent)
{
    this->N=x->size();
    this->x=NULL;
    this->y=NULL;
    this->yerr=NULL;
    this->xv=x;
    this->yv=y;
    this->yerrv=NULL;
    this->color=color;
    this->errorColor=color.lighter();
    this->errorStyle=this->style=style;
    this->errorWidth=this->width=width;
    datatype=JKQTFPLPVector;
}


void JKQTFPLinePlot::drawGraph(QPainter& painter) {
    if (datatype==JKQTFPLPPointer) if (N<=0 || !x || !y) return;
    if (datatype==JKQTFPLPVector) if (!xv || !yv) return;
    //std::cout<<"JKQTFPLinePlot::drawGraph()\n";
    QPen p(color);
    p.setStyle(style);
    p.setWidthF(qMax(JKQTFASTPLOTTER_ABS_MIN_LINEWIDTH, width));
    p.setCapStyle(Qt::RoundCap);
    p.setJoinStyle(Qt::RoundJoin);
    QPen pe(errorColor);
    pe.setStyle(errorStyle);
    pe.setWidthF(qMax(JKQTFASTPLOTTER_ABS_MIN_LINEWIDTH, errorWidth));
    pe.setCapStyle(Qt::RoundCap);
    pe.setJoinStyle(Qt::RoundJoin);
    QPainterPath path, epath;
    if (datatype==JKQTFPLPPointer) {
        if (N>0){
            path.moveTo(parent->x2p(x[0]), parent->y2p(y[0]));
            //std::cout<<"plotting graph, starting @ ("<<parent->x2p(x[0])<<", "<<parent->y2p(y[0])<<")\n";
        }
        for (unsigned int i=1; i<N; i++) {
            path.lineTo(parent->x2p(x[i]), parent->y2p(y[i]));
            //std::cout<<"-> ("<<parent->x2p(x[i])<<", "<<parent->y2p(y[i])<<")\n";
        }
        if (yerr!=NULL) {
            if (N>0){
                epath.moveTo(parent->x2p(x[0]), parent->y2p(y[0]+yerr[0]));
            }
            for (unsigned int i=1; i<N; i++) {
                epath.lineTo(parent->x2p(x[i]), parent->y2p(y[i]+yerr[i]));
            }
            if (N>0){
                epath.moveTo(parent->x2p(x[0]), parent->y2p(y[0]-yerr[0]));
            }
            for (unsigned int i=1; i<N; i++) {
                epath.lineTo(parent->x2p(x[i]), parent->y2p(y[i]-yerr[i]));
            }
        }
    } else if (datatype==JKQTFPLPVector) {
        if (xv->size()>1 && (yv->size()>=xv->size())){
            path.moveTo(parent->x2p((*xv)[0]), parent->y2p((*yv)[0]));
            //std::cout<<"plotting graph, starting @ ("<<parent->x2p(x[0])<<", "<<parent->y2p(y[0])<<")\n";
            for ( int i=1; i<xv->size(); i++) {
                path.lineTo(parent->x2p((*xv)[i]), parent->y2p((*yv)[i]));
                //std::cout<<"-> ("<<parent->x2p(x[i])<<", "<<parent->y2p(y[i])<<")\n";
            }
        }
        if (yerrv!=NULL) {
            if (yerrv->size()>1 && (xv->size()>=yerrv->size()) && (yv->size()>=yerrv->size())){
                epath.moveTo(parent->x2p((*xv)[0]), parent->y2p((*yv)[0]+(*yerrv)[0]));
                //std::cout<<"plotting graph, starting @ ("<<parent->x2p(x[0])<<", "<<parent->y2p(y[0])<<")\n";
                for ( int i=1; i<yerrv->size(); i++) {
                    epath.lineTo(parent->x2p((*xv)[i]), parent->y2p((*yv)[i]+(*yerrv)[i]));
                    //std::cout<<"-> ("<<parent->x2p(x[i])<<", "<<parent->y2p(y[i])<<")\n";
                }
                epath.moveTo(parent->x2p((*xv)[0]), parent->y2p((*yv)[0]-(*yerrv)[0]));
                //std::cout<<"plotting graph, starting @ ("<<parent->x2p(x[0])<<", "<<parent->y2p(y[0])<<")\n";
                for ( int i=1; i<yerrv->size(); i++) {
                    epath.lineTo(parent->x2p((*xv)[i]), parent->y2p((*yv)[i]-(*yerrv)[i]));
                    //std::cout<<"-> ("<<parent->x2p(x[i])<<", "<<parent->y2p(y[i])<<")\n";
                }
            }
        }
    }
    painter.setPen(p);
    painter.drawPath(path);
    painter.setPen(pe);
    painter.drawPath(epath);
    //std::cout<<"JKQTFPLinePlot::drawGraph() ... done\n";
}




JKQTFPVCrossPlot::JKQTFPVCrossPlot(JKQTFastPlotter* parent, unsigned int N, double* x, double* y, QColor color, Qt::PenStyle style, double width):
    JKQTFPLinePlot(parent, N, x, y, color, style, width)
{
    crossWidth=5;
}

JKQTFPVCrossPlot::JKQTFPVCrossPlot(JKQTFastPlotter* parent, QVector<double>* x, QVector<double>* y, QColor color, Qt::PenStyle style, double width):
    JKQTFPLinePlot(parent, x, y, color, style, width)
{
    crossWidth=5;
}

void JKQTFPVCrossPlot::drawGraph(QPainter& painter) {
    if (datatype==JKQTFPLPPointer) if (N<=0 || !x || !y) return;
    if (datatype==JKQTFPLPVector) if (!xv || !yv) return;
    //std::cout<<"JKQTFPVCrossPlot::drawGraph()\n";
    QPen p(color);
    p.setStyle(style);
    p.setWidthF(qMax(JKQTFASTPLOTTER_ABS_MIN_LINEWIDTH, width));
    p.setCapStyle(Qt::RoundCap);
    p.setJoinStyle(Qt::RoundJoin);
    QPen pe(errorColor);
    pe.setStyle(errorStyle);
    pe.setWidthF(qMax(JKQTFASTPLOTTER_ABS_MIN_LINEWIDTH, errorWidth));
    pe.setCapStyle(Qt::RoundCap);
    pe.setJoinStyle(Qt::RoundJoin);
    QPainterPath path, epath;
    if (datatype==JKQTFPLPPointer) {
        for (unsigned int i=0; i<N; i++) {
            path.moveTo(QPointF(parent->x2p(x[i])-crossWidth/2.0, parent->y2p(y[i])));
            path.lineTo(QPointF(parent->x2p(x[i])+crossWidth/2.0, parent->y2p(y[i])));
            path.moveTo(QPointF(parent->x2p(x[i]), parent->y2p(y[i])-crossWidth/2.0));
            path.lineTo(QPointF(parent->x2p(x[i]), parent->y2p(y[i])+crossWidth/2.0));
            //std::cout<<"-> ("<<parent->x2p(x[i])<<", "<<parent->y2p(y[i])<<")\n";
        }
        if (yerr!=NULL) {
            if (N>0){
                epath.moveTo(parent->x2p(x[0]), parent->y2p(y[0]+yerr[0]));
            }
            for (unsigned int i=1; i<N; i++) {
                epath.lineTo(parent->x2p(x[i]), parent->y2p(y[i]+yerr[i]));
            }
            if (N>0){
                epath.moveTo(parent->x2p(x[0]), parent->y2p(y[0]-yerr[0]));
            }
            for (unsigned int i=1; i<N; i++) {
                epath.lineTo(parent->x2p(x[i]), parent->y2p(y[i]-yerr[i]));
            }
        }
    } else if (datatype==JKQTFPLPVector) {
        if (xv->size()>0 && (yv->size()>=xv->size())){
            for ( int i=0; i<xv->size(); i++) {
                path.moveTo(QPointF(parent->x2p((*xv)[i])-crossWidth/2.0, parent->y2p((*yv)[i])));
                path.lineTo(QPointF(parent->x2p((*xv)[i])+crossWidth/2.0, parent->y2p((*yv)[i])));
                path.moveTo(QPointF(parent->x2p((*xv)[i]), parent->y2p((*yv)[i])-crossWidth/2.0));
                path.lineTo(QPointF(parent->x2p((*xv)[i]), parent->y2p((*yv)[i])+crossWidth/2.0));
                //std::cout<<"-> ("<<parent->x2p(x[i])<<", "<<parent->y2p(y[i])<<")\n";
            }
        }
        if (yerrv!=NULL) {
            if (yerrv->size()>0 && (xv->size()>=yerrv->size()) && (yv->size()>=yerrv->size())){
                epath.moveTo(parent->x2p((*xv)[0]), parent->y2p((*yv)[0]+(*yerrv)[0]));
                //std::cout<<"plotting graph, starting @ ("<<parent->x2p(x[0])<<", "<<parent->y2p(y[0])<<")\n";
                for ( int i=1; i<yerrv->size(); i++) {
                    epath.lineTo(parent->x2p((*xv)[i]), parent->y2p((*yv)[i]+(*yerrv)[i]));
                    //std::cout<<"-> ("<<parent->x2p(x[i])<<", "<<parent->y2p(y[i])<<")\n";
                }
                epath.moveTo(parent->x2p((*xv)[0]), parent->y2p((*yv)[0]-(*yerrv)[0]));
                //std::cout<<"plotting graph, starting @ ("<<parent->x2p(x[0])<<", "<<parent->y2p(y[0])<<")\n";
                for ( int i=1; i<yerrv->size(); i++) {
                    epath.lineTo(parent->x2p((*xv)[i]), parent->y2p((*yv)[i]-(*yerrv)[i]));
                    //std::cout<<"-> ("<<parent->x2p(x[i])<<", "<<parent->y2p(y[i])<<")\n";
                }
            }
        }
    }
    painter.setPen(p);
    painter.drawPath(path);
    painter.setPen(pe);
    painter.drawPath(epath);
    //std::cout<<"JKQTFPVCrossPlot::drawGraph() ... done\n";
}







JKQTFPVBarPlot::JKQTFPVBarPlot(JKQTFastPlotter* parent, unsigned int N, double* x, double* y, QColor color, Qt::PenStyle style, double width):
    JKQTFPLinePlot(parent, N, x, y, color, style, width)
{
}

JKQTFPVBarPlot::JKQTFPVBarPlot(JKQTFastPlotter* parent, QVector<double>* x, QVector<double>* y, QColor color, Qt::PenStyle style, double width):
    JKQTFPLinePlot(parent, x, y, color, style, width)
{
}

void JKQTFPVBarPlot::drawGraph(QPainter& painter) {
    if (datatype==JKQTFPLPPointer) if (N<=0 || !x || !y) return;
    if (datatype==JKQTFPLPVector) if (!xv || !yv) return;

    QPen p(color);
    p.setStyle(style);
    p.setWidthF(qMax(JKQTFASTPLOTTER_ABS_MIN_LINEWIDTH, width));
    p.setCapStyle(Qt::RoundCap);
    p.setJoinStyle(Qt::RoundJoin);

    painter.setPen(p);
    if (!parent->get_yAxisLog()) {
        if (datatype==JKQTFPLPPointer) {
            for (unsigned int i=0; i<N; i++) {
                double xval=parent->x2p(x[i]);
                QLineF l(xval, parent->y2p(0), xval, parent->y2p(y[i]));
                if (l.length()>0) painter.drawLine(l);
            }
        } else if (datatype==JKQTFPLPVector) {
            for ( int i=0; i<xv->size(); i++) {
                double xval=parent->x2p((*xv)[i]);
                QLineF l(xval, parent->y2p(0), xval, parent->y2p((*yv)[i]));
                if (l.length()>0) painter.drawLine(l);
            }
        }
    } else {
        double starty=parent->get_internalPlotBorderTop()+parent->get_plotHeight();
        if (datatype==JKQTFPLPPointer) {
            for (unsigned int i=0; i<N; i++) {
                double v=parent->y2p(y[i]);
                if (!std::isnan(v) && !std::isinf(v)) {
                    double xval=parent->x2p(x[i]);
                    QLineF l(xval, starty, xval, v);
                    if (l.length()>0) painter.drawLine(l);
                }
            }
        } else if (datatype==JKQTFPLPVector) {
            for ( int i=0; i<xv->size(); i++) {
                double v=parent->y2p((*yv)[i]);
                if (!std::isnan(v) && !std::isinf(v)) {
                    double xval=parent->x2p((*xv)[i]);
                    QLineF l(xval, starty, xval, v);
                   if (l.length()>0)  painter.drawLine(l);
                }
            }
        }
    }
}


JKQTFPXRangePlot::JKQTFPXRangePlot(JKQTFastPlotter* parent, double xmin, double xmax, QColor color, Qt::PenStyle style, double width, Qt::BrushStyle fillStyle):
    JKQTFPPlot(parent)
{
    this->xmin=xmin;
    this->xmax=xmax;
    this->centerline=(xmax+xmin)/2.0;
    this->showCenterline=false;
    this->color=color;
    this->style=style;
    this->width=width;
    this->fillColor=color;
    this->fillColor.setAlphaF(0.5);
    this->fillStyle=fillStyle;
}

void JKQTFPXRangePlot::drawGraph(QPainter& painter) {
    QRectF r(QPointF(parent->x2p(xmin), parent->get_internalPlotBorderTop()), QPointF(parent->x2p(xmax), parent->get_internalPlotBorderTop()+parent->get_plotHeight()));
    QBrush b(fillStyle);
    b.setColor(fillColor);
    QPen p(color);
    p.setStyle(style);
    p.setWidthF(qMax(JKQTFASTPLOTTER_ABS_MIN_LINEWIDTH, width));
    painter.setPen(p);
    painter.fillRect(r, b);
    if (showCenterline) {
        painter.drawLine(parent->x2p(centerline), parent->get_internalPlotBorderTop(), parent->x2p(centerline), parent->get_internalPlotBorderTop()+parent->get_plotHeight());
    }
    painter.drawRect(r);
}

JKQTFPQImagePlot::JKQTFPQImagePlot(JKQTFastPlotter* parent, QImage* image, double xmin, double xmax, double ymin, double ymax):
    JKQTFPPlot(parent)
{
    this->image=image;
    this->xmin=xmin;
    this->xmax=xmax;
    this->ymin=ymin;
    this->ymax=ymax;
}

JKQTFPQImagePlot::JKQTFPQImagePlot(JKQTFastPlotter* parent, QImage* image):
    JKQTFPPlot(parent)
{
    this->image=image;
    this->xmin=0;
    this->xmax=image->width();
    this->ymin=0;
    this->ymax=image->height();
}

void JKQTFPQImagePlot::drawGraph(QPainter& painter) {
    if (!image) return;
    if (image->isNull()) return;
    double pxmin=parent->x2p(xmin);
    double pxmax=parent->x2p(xmax);
    double dx=fabs(pxmax-pxmin);
    double pymin=parent->y2p(ymin);
    double pymax=parent->y2p(ymax);
    double dy=fabs(pymax-pymin);
    if (dx>0 && dy>0)
        painter.drawImage(QRectF(pxmin, pymax, dx, dy), *image);
}






JKQTFPimagePlot::JKQTFPimagePlot(JKQTFastPlotter* parent, void* image, JKQTFPImageFormat imageFormat, unsigned int width, unsigned int height, double xmin, double xmax, double ymin, double ymax, JKQTFPColorPalette palette):
    JKQTFPPlot(parent)
{
    this->image=image;
    this->width=width;
    this->height=height;
    this->xmin=xmin;
    this->xmax=xmax;
    this->ymin=ymin;
    this->ymax=ymax;
    colorMin=0;
    colorMax=0;
    this->palette=palette;
    drawColorBar=true;
    colorBarWidth=15;
    this->imageFormat=imageFormat;
    this->rotation=0;
}

JKQTFPimagePlot::JKQTFPimagePlot(JKQTFastPlotter* parent, void* image, JKQTFPImageFormat imageFormat, unsigned int width, unsigned int height, JKQTFPColorPalette palette):
    JKQTFPPlot(parent)
{
    this->image=image;
    this->width=width;
    this->height=height;
    colorMin=0;
    colorMax=0;
    this->palette=palette;
    drawColorBar=true;
    colorBarWidth=15;
    xmin=0;
    ymin=0;
    xmax=width;
    ymax=height;
    this->imageFormat=imageFormat;
    this->rotation=0;
}


QStringList JKQTFPimagePlot_getPalettes() {
    QStringList sl;
    sl<<QObject::tr("red");
    sl<<QObject::tr("inverted red");
    sl<<QObject::tr("green");
    sl<<QObject::tr("inverted green");
    sl<<QObject::tr("blue");
    sl<<QObject::tr("inverted blue");
    sl<<QObject::tr("gray");
    sl<<QObject::tr("inverted gray");
    sl<<QObject::tr("Matlab");
    sl<<QObject::tr("RYGB");
    sl<<QObject::tr("HSV");
    sl<<QObject::tr("inverted HSV");
    sl<<QObject::tr("rainbow");
    sl<<QObject::tr("AFM hot");
    sl<<QObject::tr("ocean");
    sl<<QObject::tr("trafficlight");
    sl<<QObject::tr("blue-magenta-yellow");
    sl<<QObject::tr("blue-yellow");
    sl<<QObject::tr("cyan");
    //sl<<QObject::tr("");
    return sl;
}

QIcon JKQTFPimagePlot_getPaletteIcon(int i) {
    QImage img;
    double pic[32];
    for (int j=0; j<32; j++) {
        pic[j]=j;
    }
    JKQTFPimagePlot_array2image<double>(pic, 32, 1, img, (JKQTFPColorPalette)i, 0, 31);
    QPixmap pix(16,8);
    QRect r(0,0,15,7);
    QPainter p(&pix);
    p.drawImage(r, img);
    p.setPen(QPen(QColor("black")));
    p.drawRect(r);
    p.end();
    return QIcon(pix);

}

void JKQTFPimagePlot::drawGraph(QPainter& painter) {
    if (!image) return;
    if (width<=0 && height<=0) return;

    //QPainter::CompositionMode cm=painter.compositionMode();
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    QImage img(width, height, QImage::Format_ARGB32);
    switch(imageFormat) {
        case JKQTFP_uint8: JKQTFPimagePlot_array2image<uint8_t>((uint8_t*)image, width, height, img, palette, colorMin, colorMax); break;
        case JKQTFP_uint16: JKQTFPimagePlot_array2image<uint16_t>((uint16_t*)image, width, height, img, palette, colorMin, colorMax); break;
        case JKQTFP_uint32: JKQTFPimagePlot_array2image<uint32_t>((uint32_t*)image, width, height, img, palette, colorMin, colorMax); break;
        case JKQTFP_int64: JKQTFPimagePlot_array2image<int64_t>((int64_t*)image, width, height, img, palette, colorMin, colorMax); break;
        case JKQTFP_float: JKQTFPimagePlot_array2image<float>((float*)image, width, height, img, palette, colorMin, colorMax); break;
        case JKQTFP_double: JKQTFPimagePlot_array2image<double>((double*)image, width, height, img, palette, colorMin, colorMax); break;
    }
    if (img.isNull()) return;
    double pxmin=parent->x2p(xmin);
    double pxmax=parent->x2p(xmax);
    double dx=fabs(pxmax-pxmin);
    double pymin=parent->y2p(ymin);
    double pymax=parent->y2p(ymax);
    double dy=fabs(pymax-pymin);
    QTransform trans;
    trans.reset();
    if (rotation==1) trans.rotate(90);
    else if (rotation==2) trans.rotate(180);
    else if (rotation==3) trans.rotate(270);

    if (dx>0 && dy>0)
        painter.drawImage(QRectF(pxmin, pymax, dx, dy), img.transformed(trans));
    if (drawColorBar && parent->get_plotHeight()>3) {
        uint8_t d[200];
        for (int i=0; i<200; i++) d[i]=i;
        QImage b(1, 200, QImage::Format_ARGB32);
        JKQTFPimagePlot_array2image<uint8_t>(d, 1, 200, b, palette, 0, 199);
        //std::cout<<"bar.width="<<b.width()<<"   bar.height="<<b.height()<<"\n";
        QRectF r(parent->get_internalPlotBorderLeft()+parent->get_plotWidth()+parent->get_tickLength()*2, parent->get_internalPlotBorderTop()+parent->get_plotHeight()*0.15, colorBarWidth, parent->get_plotHeight()*0.8);
        //std::cout<<"r.left="<<r.left()<<"   r.top="<<r.top()<<"      r.width="<<r.width()<<"   r.height="<<r.height()<<"\n";
        bool c=painter.hasClipping();
        painter.setClipping(false);
        painter.drawImage(r, b);
        painter.drawRect(r);
        painter.setClipping(c);
    }
    //painter.setCompositionMode(cm);
}





JKQTFPimageOverlayPlot::JKQTFPimageOverlayPlot(JKQTFastPlotter* parent, bool* image, unsigned int width, unsigned int height, double xmin, double xmax, double ymin, double ymax, QColor color):
    JKQTFPPlot(parent)
{
    this->image=image;
    this->width=width;
    this->height=height;
    this->xmin=xmin;
    this->xmax=xmax;
    this->ymin=ymin;
    this->ymax=ymax;
    this->color=color;
    this->rotation=0;
    this->showAsSymbols=false;
    this->linewidth=2;
    this->symboltype=stCircle;
}


JKQTFPimageOverlayPlot::JKQTFPimageOverlayPlot(JKQTFastPlotter* parent, bool* image, unsigned int width, unsigned int height, QColor color):
    JKQTFPPlot(parent)
{
    this->image=image;
    this->width=width;
    this->height=height;
    xmin=0;
    ymin=0;
    xmax=width;
    ymax=height;
    this->color=color;
    this->linewidth=2;
    this->rotation=0;
    this->showAsSymbols=false;
    this->symboltype=stCircle;
}


void JKQTFPimageOverlayPlot::drawGraph(QPainter& painter) {
    if (!image) return;
    if (width<=0 && height<=0) return;

    QTransform trans;
    trans.reset();
    if (rotation==1) trans.rotate(90);
    else if (rotation==2) trans.rotate(180);
    else if (rotation==3) trans.rotate(270);

    double pxmin=parent->x2p(xmin);
    double pxmax=parent->x2p(xmax);
    double dx=fabs(pxmax-pxmin);
    double pymin=parent->y2p(ymin);
    double pymax=parent->y2p(ymax);
    double dy=fabs(pymax-pymin);
    double ddx=dx/double(width);
    double ddy=dy/double(height);

    if (showAsSymbols) {
        QList<double> x,y;
        for (unsigned int j=0; j<height; ++j) {
            for (unsigned int i=0; i<width; ++i) {
                if (image[j*width+i]) {
                    x<<xmin+(double(i)+0.5)/double(width)*(xmax-xmin);
                    y<<ymin+(double(j)+0.5)/double(height)*(ymax-ymin);
                }
            }
        }
        painter.save();
        painter.setTransform(trans, true);
        QPen p=painter.pen();
        p.setColor(color);
        p.setWidthF(qMax(JKQTFASTPLOTTER_ABS_MIN_LINEWIDTH,linewidth));
        painter.setPen(p);
        for (int i=0; i<qMin(x.size(), y.size()); i++) {
            double px=parent->x2p(x[i]);
            double py=parent->y2p(y[i]);
            QRectF r(px-ddx/2.0, py-ddy/2.0, ddx, ddy);
            if (this->symboltype==stCircle) painter.drawEllipse(r);
            else painter.drawRect(r);
        }
        painter.restore();
    } else {
        QImage img(width, height, QImage::Format_ARGB32);
        QColor tc(Qt::transparent);
        for (unsigned int j=0; j<height; ++j) {
            QRgb* line=(QRgb*)(img.scanLine((int)height-1-(int)j));
            for (unsigned int i=0; i<width; ++i) {
                if (!image[j*width+i]) line[i]=tc.rgba();
                else line[i]=color.rgba();
            }
        }

        if (img.isNull()) return;


        if (dx>0 && dy>0) {
            //qDebug()<<painter.compositionMode()<<img.transformed(trans).format();
            painter.drawImage(QRectF(pxmin, pymax, dx, dy), img.transformed(trans, Qt::SmoothTransformation));
        }
    }
}













JKQTFPQScaleBarXPlot::JKQTFPQScaleBarXPlot(JKQTFastPlotter* parent, double width, const QString& label, QColor color, double lineWidth):
    JKQTFPPlot(parent)
{
    this->width=width;
    this->label=label;
    this->lineWidth=lineWidth;
    this->color=color;
    this->position=JKQTFPQScaleBarXPlot::TopRight;
    if (parent) {
        this->font.setFamily(parent->get_labelFontName());
        this->font.setPointSizeF(parent->get_labelFontSize());
    }
}

void JKQTFPQScaleBarXPlot::drawGraph(QPainter& painter) {
    painter.save();


    QRectF r(QPointF(parent->get_internalPlotBorderLeft(), parent->get_internalPlotBorderTop()), QPointF(parent->get_internalPlotBorderLeft()+parent->get_plotWidth(), parent->get_internalPlotBorderTop()+parent->get_plotHeight()));

    double borderfraction=0.1;
    int yDistance=(double)parent->get_plotHeight()*borderfraction;

    QPen p(color);
    p.setWidthF(qMax(JKQTFASTPLOTTER_ABS_MIN_LINEWIDTH, lineWidth));
    p.setColor(color);
    QString s=label.arg(width);

    double yy1, yy2, xx1, xx2, x1, x2;//, y2, y1;

    if (position==JKQTFPQScaleBarXPlot::TopRight) {
        yy1=parent->get_internalPlotBorderTop()+yDistance;
        yy2=yy1;
        x2=parent->get_xMax()-borderfraction*(parent->get_xMax()-parent->get_xMin());
        x1=x2-width;
        xx1=parent->x2p(x1);
        xx2=parent->x2p(x2);
        painter.drawLine(QLineF(xx1, yy1, xx2, yy2));
        painter.setFont(font);
        QFontMetrics fm=painter.fontMetrics();
        painter.drawText(xx1+(xx2-xx1)/2-fm.width(s)/2, yy1+3*lineWidth+fm.ascent(), s);
    } else if (position==JKQTFPQScaleBarXPlot::TopLeft) {
        yy1=parent->get_internalPlotBorderTop()+yDistance;
        yy2=yy1;
        x1=parent->get_xMin()+borderfraction*(parent->get_xMax()-parent->get_xMin());
        x2=x1+width;
        xx1=parent->x2p(x1);
        xx2=parent->x2p(x2);
        painter.drawLine(QLineF(xx1, yy1, xx2, yy2));
        painter.setFont(font);
        QFontMetrics fm=painter.fontMetrics();
        painter.drawText(xx1+(xx2-xx1)/2-fm.width(s)/2, yy1+3*lineWidth+fm.ascent(), s);
    } else if (position==JKQTFPQScaleBarXPlot::BottomLeft) {
        yy1=parent->get_internalPlotBorderTop()+parent->get_plotHeight()-yDistance;
        yy2=yy1;
        x1=parent->get_xMin()+borderfraction*(parent->get_xMax()-parent->get_xMin());
        x2=x1+width;
        xx1=parent->x2p(x1);
        xx2=parent->x2p(x2);
        painter.drawLine(QLineF(xx1, yy1, xx2, yy2));
        painter.setFont(font);
        QFontMetrics fm=painter.fontMetrics();
        painter.drawText(xx1+(xx2-xx1)/2-fm.width(s)/2, yy1-3*lineWidth-fm.descent(), s);
    } else if (position==JKQTFPQScaleBarXPlot::BottomRight) {
        yy1=parent->get_internalPlotBorderTop()+parent->get_plotHeight()-yDistance;
        yy2=yy1;
        x2=parent->get_xMax()-borderfraction*(parent->get_xMax()-parent->get_xMin());
        x1=x2-width;
        xx1=parent->x2p(x1);
        xx2=parent->x2p(x2);
        painter.drawLine(QLineF(xx1, yy1, xx2, yy2));
        painter.setFont(font);
        QFontMetrics fm=painter.fontMetrics();
        painter.drawText(xx1+(xx2-xx1)/2-fm.width(s)/2, yy1-3*lineWidth-fm.descent(), s);
    }


    painter.restore();
}


QIcon JKQTFPimagePlot_getPaletteIcon(JKQTFPColorPalette palette) {
    return JKQTFPimagePlot_getPaletteIcon((int)palette);
};


JKQTFPQOverlayLinearGridPlot::JKQTFPQOverlayLinearGridPlot(JKQTFastPlotter* parent, double width, QColor color, double lineWidth, Qt::PenStyle style):
    JKQTFPPlot(parent)
{
    this->width=width;
    this->color=color;
    this->lineWidth=lineWidth;
    this->style=style;
}

void JKQTFPQOverlayLinearGridPlot::drawGraph(QPainter& painter) {
    painter.save();

    QPen p(color);
    p.setWidthF(qMax(JKQTFASTPLOTTER_ABS_MIN_LINEWIDTH, lineWidth));
    p.setColor(color);
    p.setStyle(style);

    double xmin=parent->get_xMin();
    double xmax=parent->get_xMax();
    double ymin=parent->get_yMin();
    double ymax=parent->get_yMax();

    double xstart=(floor(xmin/width)-1.0)*width;
    double ystart=(floor(ymin/width)-1.0)*width;

    QPainterPath gridPath;
    double x=xstart;
    int cntr=0; // used to stop plotting after 1000 grid lines
    while (x<=xmax && cntr<1000) {
        if (xmin<=x && x<=xmax) {
            gridPath.moveTo(parent->x2p(x), parent->get_internalPlotBorderTop());
            gridPath.lineTo(parent->x2p(x), parent->get_internalPlotBorderTop()+parent->get_plotHeight());
        }
        x+=width;
        cntr++;
    }
    double y=ystart;
    cntr=0;
    while (y<=ymax && cntr<1000) {
        if (ymin<=y && y<=ymax) {
            gridPath.moveTo(parent->get_internalPlotBorderLeft(), parent->y2p(y));
            gridPath.lineTo(parent->get_internalPlotBorderLeft()+parent->get_plotWidth(), parent->y2p(y));
        }
        y+=width;
        cntr++;
    }

    painter.setPen(p);
    painter.drawPath(gridPath);
    painter.restore();
}



JKQTFPRGBImageOverlayPlot::JKQTFPRGBImageOverlayPlot(JKQTFastPlotter *parent, void *imageRed, JKQTFPImageFormat imageFormat, unsigned int width, unsigned int height, double xmin, double xmax, double ymin, double ymax):
    JKQTFPPlot(parent)
{
    this->width=width;
    this->height=height;
    this->xmin=xmin;
    this->xmax=xmax;
    this->ymin=ymin;
    this->ymax=ymax;
    colorMinRed=0;
    colorMaxRed=0;
    this->imageRed=imageRed;
    this->imageFormatRed=imageFormat;
    colorMinGreen=0;
    colorMaxGreen=0;
    this->imageGreen=NULL;
    this->imageFormatGreen=JKQTFP_uint16;
    colorMinBlue=0;
    colorMaxBlue=0;
    this->imageBlue=NULL;
    this->imageFormatBlue=JKQTFP_uint16;
    this->rotation=0;
}

JKQTFPRGBImageOverlayPlot::JKQTFPRGBImageOverlayPlot(JKQTFastPlotter *parent, void *imageRed, JKQTFPImageFormat imageFormat, unsigned int width, unsigned int height):
    JKQTFPPlot(parent)
{
    this->width=width;
    this->height=height;
    this->xmin=0;
    this->xmax=width-1;
    this->ymin=0;
    this->ymax=height-1;
    colorMinRed=0;
    colorMaxRed=0;
    this->imageRed=imageRed;
    this->imageFormatRed=imageFormat;
    colorMinGreen=0;
    colorMaxGreen=0;
    this->imageGreen=NULL;
    this->imageFormatGreen=JKQTFP_uint16;
    colorMinBlue=0;
    colorMaxBlue=0;
    this->imageBlue=NULL;
    this->imageFormatBlue=JKQTFP_uint16;
    this->rotation=0;
}

JKQTFPRGBImageOverlayPlot::JKQTFPRGBImageOverlayPlot(JKQTFastPlotter *parent):
    JKQTFPPlot(parent)
{
    this->width=width;
    this->height=height;
    this->xmin=0;
    this->xmax=width-1;
    this->ymin=0;
    this->ymax=height-1;
    colorMinRed=0;
    colorMaxRed=0;
    this->imageRed=NULL;
    this->imageFormatRed=JKQTFP_uint16;
    colorMinGreen=0;
    colorMaxGreen=0;
    this->imageGreen=NULL;
    this->imageFormatGreen=JKQTFP_uint16;
    colorMinBlue=0;
    colorMaxBlue=0;
    this->imageBlue=NULL;
    this->imageFormatBlue=JKQTFP_uint16;
    this->rotation=0;
}

void JKQTFPRGBImageOverlayPlot::drawGraph(QPainter &painter) {
    //qDebug()<<"JKQTFPRGBImageOverlayPlot::drawGraph() "<<imageRed<<imageGreen<<imageBlue<<width<<height;
    if (!imageRed && !imageGreen && !imageBlue) return;
    if (width<=0 && height<=0) return;

    QImage img(width, height, QImage::Format_ARGB32);
    img.fill(0);
    switch(imageFormatRed) {
        case JKQTFP_uint8: JKQTFPRGBImageOverlayPlot_array2image<uint8_t>((uint8_t*)imageRed, width, height, img, 0, colorMinRed, colorMaxRed); break;
        case JKQTFP_uint16: JKQTFPRGBImageOverlayPlot_array2image<uint16_t>((uint16_t*)imageRed, width, height, img, 0, colorMinRed, colorMaxRed); break;
        case JKQTFP_uint32: JKQTFPRGBImageOverlayPlot_array2image<uint32_t>((uint32_t*)imageRed, width, height, img, 0, colorMinRed, colorMaxRed); break;
        case JKQTFP_int64: JKQTFPRGBImageOverlayPlot_array2image<int64_t>((int64_t*)imageRed, width, height, img, 0, colorMinRed, colorMaxRed); break;
        case JKQTFP_float: JKQTFPRGBImageOverlayPlot_array2image<float>((float*)imageRed, width, height, img, 0, colorMinRed, colorMaxRed); break;
        case JKQTFP_double: JKQTFPRGBImageOverlayPlot_array2image<double>((double*)imageRed, width, height, img, 0, colorMinRed, colorMaxRed); break;
    }
    switch(imageFormatGreen) {
        case JKQTFP_uint8: JKQTFPRGBImageOverlayPlot_array2image<uint8_t>((uint8_t*)imageGreen, width, height, img, 1, colorMinGreen, colorMaxGreen); break;
        case JKQTFP_uint16: JKQTFPRGBImageOverlayPlot_array2image<uint16_t>((uint16_t*)imageGreen, width, height, img, 1, colorMinGreen, colorMaxGreen); break;
        case JKQTFP_uint32: JKQTFPRGBImageOverlayPlot_array2image<uint32_t>((uint32_t*)imageGreen, width, height, img, 1, colorMinGreen, colorMaxGreen); break;
        case JKQTFP_int64: JKQTFPRGBImageOverlayPlot_array2image<int64_t>((int64_t*)imageGreen, width, height, img, 1, colorMinGreen, colorMaxGreen); break;
        case JKQTFP_float: JKQTFPRGBImageOverlayPlot_array2image<float>((float*)imageGreen, width, height, img, 1, colorMinGreen, colorMaxGreen); break;
        case JKQTFP_double: JKQTFPRGBImageOverlayPlot_array2image<double>((double*)imageGreen, width, height, img, 1, colorMinGreen, colorMaxGreen); break;
    }
    switch(imageFormatBlue) {
        case JKQTFP_uint8: JKQTFPRGBImageOverlayPlot_array2image<uint8_t>((uint8_t*)imageBlue, width, height, img, 2, colorMinBlue, colorMaxBlue); break;
        case JKQTFP_uint16: JKQTFPRGBImageOverlayPlot_array2image<uint16_t>((uint16_t*)imageBlue, width, height, img, 2, colorMinBlue, colorMaxBlue); break;
        case JKQTFP_uint32: JKQTFPRGBImageOverlayPlot_array2image<uint32_t>((uint32_t*)imageBlue, width, height, img, 2, colorMinBlue, colorMaxBlue); break;
        case JKQTFP_int64: JKQTFPRGBImageOverlayPlot_array2image<int64_t>((int64_t*)imageBlue, width, height, img, 2, colorMinBlue, colorMaxBlue); break;
        case JKQTFP_float: JKQTFPRGBImageOverlayPlot_array2image<float>((float*)imageBlue, width, height, img, 2, colorMinBlue, colorMaxBlue); break;
        case JKQTFP_double: JKQTFPRGBImageOverlayPlot_array2image<double>((double*)imageBlue, width, height, img, 2, colorMinBlue, colorMaxBlue); break;
    }
    if (img.isNull()) return;
    double pxmin=parent->x2p(xmin);
    double pxmax=parent->x2p(xmax);
    double dx=fabs(pxmax-pxmin);
    double pymin=parent->y2p(ymin);
    double pymax=parent->y2p(ymax);
    double dy=fabs(pymax-pymin);
    QTransform trans;
    trans.reset();
    if (rotation==1) trans.rotate(90);
    else if (rotation==2) trans.rotate(180);
    else if (rotation==3) trans.rotate(270);

    if (dx>0 && dy>0)
        painter.drawImage(QRectF(pxmin, pymax, dx, dy), img.transformed(trans));
}


void JKQTFPRGBImageOverlayPlot::set_image(void* imageRed, JKQTFPImageFormat imageFormatRed, unsigned int width, unsigned int height){
    this->imageRed=imageRed;
    this->imageFormatRed=imageFormatRed;
    this->imageGreen=NULL;
    this->imageBlue=NULL;
    this->width=width;
    this->height=height;
    this->xmin=0;
    this->xmax=width-1;
    this->ymin=0;
    this->ymax=height-1;

    replot();
}
void JKQTFPRGBImageOverlayPlot::set_image(void* imageRed, JKQTFPImageFormat imageFormatRed, void* imageGreen, JKQTFPImageFormat imageFormatGreen, unsigned int width, unsigned int height) {
    this->imageRed=imageRed;
    this->imageFormatRed=imageFormatRed;
    this->imageGreen=imageGreen;
    this->imageFormatGreen=imageFormatGreen;
    this->imageBlue=NULL;
    this->width=width;
    this->height=height;
    this->xmin=0;
    this->xmax=width-1;
    this->ymin=0;
    this->ymax=height-1;

    replot();
}
void JKQTFPRGBImageOverlayPlot::set_image(void* imageRed, JKQTFPImageFormat imageFormatRed, void* imageGreen, JKQTFPImageFormat imageFormatGreen, void* imageBlue, JKQTFPImageFormat imageFormatBlue, unsigned int width, unsigned int height) {
    this->imageRed=imageRed;
    this->imageFormatRed=imageFormatRed;
    this->imageGreen=imageGreen;
    this->imageFormatGreen=imageFormatGreen;
    this->imageBlue=imageBlue;
    this->imageFormatBlue=imageFormatBlue;
    this->width=width;
    this->height=height;
    this->xmin=0;
    this->xmax=width-1;
    this->ymin=0;
    this->ymax=height-1;

    replot();
}


void JKQTFPRGBImageOverlayPlot::set_image(void* imageRed, JKQTFPImageFormat imageFormatRed, unsigned int width, unsigned int height, double xmin, double xmax, double ymin, double ymax){
    this->imageRed=imageRed;
    this->imageFormatRed=imageFormatRed;
    this->imageRed=NULL;
    this->imageBlue=NULL;
    this->width=width;
    this->height=height;
    this->xmin=xmin;
    this->xmax=xmax;
    this->ymin=ymin;
    this->ymax=ymax;

    replot();
}
void JKQTFPRGBImageOverlayPlot::set_image(void* imageRed, JKQTFPImageFormat imageFormatRed, void* imageGreen, JKQTFPImageFormat imageFormatGreen, unsigned int width, unsigned int height, double xmin, double xmax, double ymin, double ymax) {
    this->imageRed=imageRed;
    this->imageFormatRed=imageFormatRed;
    this->imageGreen=imageGreen;
    this->imageFormatGreen=imageFormatGreen;
    this->imageBlue=NULL;
    this->width=width;
    this->height=height;
    this->xmin=xmin;
    this->xmax=xmax;
    this->ymin=ymin;
    this->ymax=ymax;

    replot();
}
void JKQTFPRGBImageOverlayPlot::set_image(void* imageRed, JKQTFPImageFormat imageFormatRed, void* imageGreen, JKQTFPImageFormat imageFormatGreen, void* imageBlue, JKQTFPImageFormat imageFormatBlue, unsigned int width, unsigned int height, double xmin, double xmax, double ymin, double ymax) {
    this->imageRed=imageRed;
    this->imageFormatRed=imageFormatRed;
    this->imageGreen=imageGreen;
    this->imageFormatGreen=imageFormatGreen;
    this->imageBlue=imageBlue;
    this->imageFormatBlue=imageFormatBlue;
    this->width=width;
    this->height=height;
    this->xmin=xmin;
    this->xmax=xmax;
    this->ymin=ymin;
    this->ymax=ymax;

    replot();
}

JKQTFPYRangePlot::JKQTFPYRangePlot(JKQTFastPlotter *parent, double xmin, double xmax, QColor color, Qt::PenStyle style, double width, Qt::BrushStyle fillStyle):
    JKQTFPPlot(parent)
{
    this->ymin=xmin;
    this->ymax=xmax;
    this->centerline=(xmax+xmin)/2.0;
    this->showCenterline=false;
    this->color=color;
    this->style=style;
    this->width=width;
    this->fillColor=color;
    this->fillColor.setAlphaF(0.5);
    this->fillStyle=fillStyle;
}

void JKQTFPYRangePlot::drawGraph(QPainter& painter) {
    QRectF r(QPointF(parent->get_internalPlotBorderLeft(), parent->y2p(ymin)), QPointF(parent->get_internalPlotBorderLeft()+parent->get_plotWidth(), parent->y2p(ymax)));
    QBrush b(fillStyle);
    b.setColor(fillColor);
    QPen p(color);
    p.setStyle(style);
    p.setWidthF(qMax(JKQTFASTPLOTTER_ABS_MIN_LINEWIDTH, width));
    painter.setPen(p);
    painter.fillRect(r, b);
    if (showCenterline) {
        painter.drawLine(parent->get_internalPlotBorderLeft(), parent->y2p(centerline), parent->get_internalPlotBorderLeft()+parent->get_plotWidth(), parent->y2p(centerline));
    }
    painter.drawRect(r);
}


