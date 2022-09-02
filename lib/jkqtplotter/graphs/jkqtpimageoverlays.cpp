/*
    Copyright (c) 2008-2022 Jan W. Krieger (jan@jkrieger.de)

    

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


#include "jkqtplotter/graphs/jkqtpimageoverlays.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtpimagetools.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtcommon/jkqtpenhancedpainter.h"
#include "jkqtplotter/jkqtplotter.h"
#include <QDebug>
#include <QImageWriter>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>
#include <QClipboard>


JKQTPOverlayImage::JKQTPOverlayImage(double x, double y, double width, double height, const bool *data, int Nx, int Ny, QColor colTrue, JKQTBasePlotter* parent):
    JKQTPImageBase(x, y, width, height, parent)
{
    actSaveImage=new QAction(tr("Save JKQTPOverlayImage ..."), this);
    connect(actSaveImage, SIGNAL(triggered()), this, SLOT(saveImagePlotAsImage()));
    actCopyImage=new QAction(tr("Copy JKQTPOverlayImage ..."), this);
    connect(actCopyImage, SIGNAL(triggered()), this, SLOT(copyImagePlotAsImage()));
    this->Nx=Nx;
    this->Ny=Ny;
    this->data=data;
    this->trueColor=colTrue;
    this->falseColor=QColor(Qt::transparent);
}

JKQTPOverlayImage::JKQTPOverlayImage(JKQTBasePlotter *parent):
    JKQTPImageBase(0,0,1,1, parent)
{
    actSaveImage=new QAction(tr("Save JKQTPOverlayImage ..."), this);
    connect(actSaveImage, SIGNAL(triggered()), this, SLOT(saveImagePlotAsImage()));
    actCopyImage=new QAction(tr("Copy JKQTPOverlayImage ..."), this);
    connect(actCopyImage, SIGNAL(triggered()), this, SLOT(copyImagePlotAsImage()));
    this->Nx=0;
    this->Ny=0;
    this->data=nullptr;
    this->trueColor=QColor("red");
}

JKQTPOverlayImage::JKQTPOverlayImage(double x, double y, double width, double height, const bool* data, int Nx, int Ny, QColor colTrue, JKQTPlotter* parent):
    JKQTPImageBase(x, y, width, height, parent)
{
    actSaveImage=new QAction(tr("Save JKQTPOverlayImage ..."), this);
    connect(actSaveImage, SIGNAL(triggered()), this, SLOT(saveImagePlotAsImage()));
    actCopyImage=new QAction(tr("Copy JKQTPOverlayImage ..."), this);
    connect(actCopyImage, SIGNAL(triggered()), this, SLOT(copyImagePlotAsImage()));
    this->Nx=Nx;
    this->Ny=Ny;
    this->data=data;
    this->trueColor=colTrue;
    this->falseColor=QColor(Qt::transparent);
}

JKQTPOverlayImage::JKQTPOverlayImage(JKQTPlotter *parent):
    JKQTPImageBase(0,0,1,1, parent)
{
    actSaveImage=new QAction(tr("Save JKQTPOverlayImage ..."), this);
    connect(actSaveImage, SIGNAL(triggered()), this, SLOT(saveImagePlotAsImage()));
    actCopyImage=new QAction(tr("Copy JKQTPOverlayImage ..."), this);
    connect(actCopyImage, SIGNAL(triggered()), this, SLOT(copyImagePlotAsImage()));
    this->Nx=0;
    this->Ny=0;
    this->data=nullptr;
    this->trueColor=QColor("red");
}

void JKQTPOverlayImage::draw(JKQTPEnhancedPainter& painter) {
    if (!data) return;
    QImage img=drawImage();
    plotImage(painter, img, x, y, width, height);
}

QImage JKQTPOverlayImage::drawImage() {
    if (!data) return QImage();
    QImage img(Nx, Ny, QImage::Format_ARGB32);

    //QRgb tc=trueColor.rgba();
    //QRgb fc=falseColor.rgba();
    QRgb tc=qRgba(jkqtp_roundTo<int>(trueColor.red()*trueColor.alphaF()), jkqtp_roundTo<int>(trueColor.green()*trueColor.alphaF()), jkqtp_roundTo<int>(trueColor.blue()*trueColor.alphaF()), trueColor.alpha());
    QRgb fc=qRgba(jkqtp_roundTo<int>(falseColor.red()*falseColor.alphaF()), jkqtp_roundTo<int>(falseColor.green()*falseColor.alphaF()), jkqtp_roundTo<int>(falseColor.blue()*falseColor.alphaF()), falseColor.alpha());

    for (int32_t y=0; y<Ny; y++) {
        QRgb* line=(QRgb*)img.scanLine(Ny-1-y);
        for (int32_t x=0; x<Nx; x++) {
            if (data[x+y*Nx]) {
                line[x]=tc;
            } else {
                line[x]=fc;
            }

        }
    }

    return img;
}

void JKQTPOverlayImage::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    double w=rect.width()/2;
    QRectF r1=QRectF(rect.topLeft(), QSizeF(w, rect.height()));
    QRectF r2=QRectF(QPointF(rect.left()+w, rect.top()), QSizeF(w, rect.height()));
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(Qt::transparent);
    painter.setBrush(QBrush(trueColor));
    painter.fillRect(r1, QBrush(trueColor));
    painter.setBrush(QBrush(falseColor));
    painter.fillRect(r2, QBrush(falseColor));
}

QColor JKQTPOverlayImage::getKeyLabelColor() const {
    QColor c=trueColor;
    c.setAlpha(255);
    return c;
}

void JKQTPOverlayImage::setTrueColor(const QColor &__value)
{
    this->trueColor = __value;
}

QColor JKQTPOverlayImage::getTrueColor() const
{
    return this->trueColor;
}

void JKQTPOverlayImage::setFalseColor(const QColor &__value)
{
    this->falseColor = __value;
}

QColor JKQTPOverlayImage::getFalseColor() const
{
    return this->falseColor;
}

void JKQTPOverlayImage::setNx(int __value)
{
    this->Nx = __value;
}

void JKQTPOverlayImage::setNx(size_t __value)
{
    this->Nx = static_cast<int>(__value);
}

int JKQTPOverlayImage::getNx() const
{
    return this->Nx;
}

void JKQTPOverlayImage::setNy(int __value)
{
    this->Ny = __value;
}

void JKQTPOverlayImage::setNy(size_t __value)
{
    this->Ny = static_cast<int>(__value);
}
int JKQTPOverlayImage::getNy() const
{
    return this->Ny;
}

void JKQTPOverlayImage::setData(const bool *__value)
{
    this->data = __value;
}

const bool *JKQTPOverlayImage::getData() const
{
    return this->data;
}

void JKQTPOverlayImage::setData(const bool *data, int Nx, int Ny) {
    this->data=data;
    this->Nx=Nx;
    this->Ny=Ny;
}


JKQTPOverlayImageEnhanced::JKQTPOverlayImageEnhanced(double x, double y, double width, double height, bool* data, int Nx, int Ny, QColor colTrue, JKQTBasePlotter* parent):
    JKQTPOverlayImage(x, y, width, height, data, Nx, Ny, colTrue, parent)
{
    symbol=JKQTPTarget;
    symbolLineWidth=1;
    drawMode=OverlayImageEnhancedDrawMode::DrawAsRectangles;
    symbolSizeFactor=0.9;
    m_symbolFontName=parent->getDefaultTextFontName();

}

JKQTPOverlayImageEnhanced::JKQTPOverlayImageEnhanced(JKQTBasePlotter *parent):
    JKQTPOverlayImage(0,0,1,1,nullptr,0,0, QColor("red"), parent)
{
    symbol=JKQTPTarget;
    symbolLineWidth=1;
    drawMode=OverlayImageEnhancedDrawMode::DrawAsRectangles;
    symbolSizeFactor=0.9;
    m_symbolFontName=parent->getDefaultTextFontName();

}

JKQTPOverlayImageEnhanced::JKQTPOverlayImageEnhanced(double x, double y, double width, double height, bool* data, int Nx, int Ny, QColor colTrue, JKQTPlotter* parent):
    JKQTPOverlayImage(x, y, width, height, data, Nx, Ny, colTrue, parent)
{
    symbol=JKQTPTarget;
    symbolLineWidth=1;
    drawMode=OverlayImageEnhancedDrawMode::DrawAsRectangles;
    symbolSizeFactor=0.9;
    m_symbolFontName=parent->getPlotter()->getDefaultTextFontName();

}

JKQTPOverlayImageEnhanced::JKQTPOverlayImageEnhanced(JKQTPlotter *parent):
    JKQTPOverlayImage(0,0,1,1,nullptr,0,0, QColor("red"), parent)
{
    symbol=JKQTPTarget;
    symbolLineWidth=1;
    drawMode=OverlayImageEnhancedDrawMode::DrawAsRectangles  ;
    symbolSizeFactor=0.9;
    m_symbolFontName=parent->getPlotter()->getDefaultTextFontName();

}

void JKQTPOverlayImageEnhanced::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    if (drawMode!=OverlayImageEnhancedDrawMode::DrawAsSymbols) JKQTPOverlayImage::drawKeyMarker(painter, rect);
    else JKQTPPlotSymbol(painter, rect.center().x(), rect.center().y(), symbol, qMin(rect.width(), rect.height()), parent->pt2px(painter, symbolLineWidth*parent->getLineWidthMultiplier()), trueColor, trueColor.lighter(), m_symbolFontName);
}

void JKQTPOverlayImageEnhanced::setSymbolType(JKQTPGraphSymbols __value)
{
    this->symbol = __value;
}

JKQTPGraphSymbols JKQTPOverlayImageEnhanced::getSymbol() const
{
    return this->symbol;
}


void JKQTPOverlayImageEnhanced::setSymbolFontName(const QString& __value)
{
    this->m_symbolFontName = __value;
}

QString JKQTPOverlayImageEnhanced::getSymbolFontName() const
{
    return this->m_symbolFontName;
}

void JKQTPOverlayImageEnhanced::setSymbolLineWidth(double __value)
{
    this->symbolLineWidth = __value;
}

double JKQTPOverlayImageEnhanced::getSymbolLineWidth() const
{
    return this->symbolLineWidth;
}

JKQTPOverlayImageEnhanced::OverlayImageEnhancedDrawMode JKQTPOverlayImageEnhanced::getDrawMode() const
{
    return drawMode;
}

void JKQTPOverlayImageEnhanced::setDrawMode(OverlayImageEnhancedDrawMode __value)
{
    this->drawMode = __value;
}


void JKQTPOverlayImageEnhanced::setSymbolSizeFactor(double __value)
{
    this->symbolSizeFactor = __value;
}

double JKQTPOverlayImageEnhanced::getSymbolSizeFactor() const
{
    return this->symbolSizeFactor;
}

void JKQTPOverlayImageEnhanced::draw(JKQTPEnhancedPainter& painter) {
    if (!data) return;

    if (drawMode==DrawAsImage) {
        JKQTPOverlayImage::draw(painter);
    } else {

        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        double dx=width/static_cast<double>(Nx);
        double dy=height/static_cast<double>(Ny);
        for (int ix=0; ix<static_cast<int64_t>(Nx); ix++) {
            for (int iy=0; iy<static_cast<int64_t>(Ny); iy++) {
                QPointF p1=transform(x+static_cast<double>(ix)*dx, y+static_cast<double>(iy)*dy);
                QPointF p2=transform(x+static_cast<double>(ix+1)*dx, y+static_cast<double>(iy+1)*dx);
                if (drawMode==DrawAsRectangles) {
                    if (data[ix+iy*Nx]) {
                        if (trueColor.alpha()>0) {
                            painter.fillRect(QRectF(qMin(p1.x(), p2.x())-1.0, qMin(p2.y(), p1.y())-1.0, fabs(p2.x()-p1.x())+1.0, fabs(p2.y()-p1.y())+1.0), QBrush(trueColor));
                            //painter.setPen(QPen(trueColor));
                            //painter.drawRect(QRectF(qMin(p1.x(), p2.x()), qMin(p2.y(), p1.y()), fabs(p2.x()-p1.x()), fabs(p2.y()-p1.y())));
                        }
                    } else {
                        if (falseColor.alpha()>0) {
                            //painter.setPen(QPen(falseColor));
                            painter.fillRect(QRectF(qMin(p1.x(), p2.x())-1.0, qMin(p2.y(), p1.y())-1.0, fabs(p2.x()-p1.x())+1.0, fabs(p2.y()-p1.y())+1.0), QBrush(falseColor));
                            //painter.drawRect(QRectF(qMin(p1.x(), p2.x()), qMin(p2.y(), p1.y()), fabs(p2.x()-p1.x()), fabs(p2.y()-p1.y())));
                        }
                    }
                } else if (drawMode==DrawAsSymbols){
                    QPointF p=(p1+p2)/2.0;
                    if (data[ix+iy*Nx]) {
                        JKQTPPlotSymbol(painter, p.x(), p.y(), symbol, fabs(p2.x()-p1.x())*symbolSizeFactor, parent->pt2px(painter, symbolLineWidth*parent->getLineWidthMultiplier()), trueColor, trueColor.lighter(),QFont(m_symbolFontName,10));
                    }
                }
            }
        }

    }
}















JKQTPColumnOverlayImageEnhanced::JKQTPColumnOverlayImageEnhanced(JKQTBasePlotter *parent):
    JKQTPOverlayImageEnhanced(parent)
{
    imageColumn=-1;
}

JKQTPColumnOverlayImageEnhanced::JKQTPColumnOverlayImageEnhanced(JKQTPlotter *parent):
    JKQTPOverlayImageEnhanced(parent)
{
    imageColumn=-1;
}

void JKQTPColumnOverlayImageEnhanced::setImageColumn(int __value)
{
    this->imageColumn = __value;
    if (parent && __value>=0 && parent->getDatastore()) {
        setNx(parent->getDatastore()->getColumnImageWidth(__value));
        setNy(parent->getDatastore()->getColumnImageHeight(__value));
    }
}

int JKQTPColumnOverlayImageEnhanced::getImageColumn() const
{
    return this->imageColumn;
}
void JKQTPColumnOverlayImageEnhanced::draw(JKQTPEnhancedPainter &painter) {
    const double* d=parent->getDatastore()->getColumnPointer(imageColumn,0);
    size_t imgSize=parent->getDatastore()->getRows(imageColumn);
    QVector<bool> locData(imgSize,false);
    this->data=locData.data();
    this->Ny= static_cast<int>(imgSize/this->Nx);
    for (size_t i=0; i<imgSize; i++) {
        locData[i]=(d[i]!=0.0);
    }
    JKQTPOverlayImageEnhanced::draw(painter);
    this->data=nullptr;
}

bool JKQTPColumnOverlayImageEnhanced::usesColumn(int c) const
{
    return (c==imageColumn);
}




QVector<double> JKQTPOverlayImage::getDataAsDoubleVector() const
{
    return JKQTPImagePlot_BarrayToDVector(data, Nx*Ny);

}

void JKQTPOverlayImage::setTitle(const QString &title)
{
    JKQTPImageBase::setTitle(title);
    QString t=title;
    if (t.isEmpty()) t="JKQTPOverlayImage";
    actSaveImage->setText(tr("Save %1 ...").arg(t));
    actCopyImage->setText(tr("Copy %1 ...").arg(t));

}

void JKQTPOverlayImage::setParent(JKQTBasePlotter *parent)
{
    if (this->parent) {
        this->parent->deregisterAdditionalAction(actSaveImage);
        this->parent->deregisterAdditionalAction(actCopyImage);

    }
    JKQTPImageBase::setParent(parent);

    if (parent) {
        parent->registerAdditionalAction(tr("Save Image Plot Images ..."), actSaveImage);
        parent->registerAdditionalAction(tr("Copy Image Plot Images ..."), actCopyImage);

    }
    actSaveImage->setEnabled(parent);
    actCopyImage->setEnabled(parent);

}

void JKQTPOverlayImage::saveImagePlotAsImage(const QString &filename, const QByteArray &outputFormat)
{
    if (parent) {
        parent->loadUserSettings();
        QString currentSaveDirectory=parent->getCurrentSaveDirectory();
        QString currentFileFormat=parent->getCurrentFileFormat();
        QString fn=filename;
        QStringList filt;
        QList<QByteArray> writerformats=QImageWriter::supportedImageFormats();
        for (int i=0; i<writerformats.size(); i++) {
            filt<<QString("%1 Image (*.%2)").arg(QString(writerformats[i]).toUpper()).arg(QString(writerformats[i].toLower()));
        }
        QString selFormat;
        if (fn.isEmpty()) {
            selFormat=currentFileFormat;
            fn = QFileDialog::getSaveFileName(nullptr, tr("Save Image Plot As Image ..."),
                                        currentSaveDirectory,
                                              filt.join(";;"), &selFormat);
            if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
        }

        parent->setCurrentFileFormat(currentFileFormat);
        parent->setCurrentSaveDirectory(currentSaveDirectory);
        parent->saveUserSettings();
        if (!fn.isEmpty()) {
            int filtID=filt.indexOf(selFormat);
            QString form="NONE";
            if (filtID>=0 && filtID<writerformats.size()) {
                form=writerformats[filtID];
            }
            if (outputFormat.size()>0) {
                form =outputFormat;
            }

            QImage image=drawImage();

            if (form=="NONE") image.save(fn);
            else image.save(fn, form.toLatin1().data());
        }
    }
}

void JKQTPOverlayImage::copyImagePlotAsImage()
{
    QClipboard* clip=QApplication::clipboard();
    if (clip) {
        clip->setPixmap(QPixmap::fromImage(drawImage()));
    }
}

