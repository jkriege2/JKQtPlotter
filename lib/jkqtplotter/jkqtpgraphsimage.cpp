/*
    Copyright (c) 2008-2019 Jan W. Krieger & Sebastian Isbaner (contour plot)

    

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


#include "jkqtplotter/jkqtpgraphsimage.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplottertools/jkqtpimagetools.h"
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtpenhancedpainter.h"
#include "jkqtplotter/jkqtplotter.h"
#include <QDebug>
#include <QImageWriter>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>
#include <QClipboard>


JKQTPImageBase::JKQTPImageBase(double x, double y, double width, double height, JKQTBasePlotter* parent):
    JKQTPGraph(parent)
{
    title="";
    this->width=width;
    this->height=height;
    this->x=x;
    this->y=y;
}

JKQTPImageBase::JKQTPImageBase(JKQTBasePlotter *parent):
    JKQTPGraph(parent)
{
    title="";
    this->width=0;
    this->height=0;
    this->x=0;
    this->y=0;
}

JKQTPImageBase::JKQTPImageBase(double x, double y, double width, double height, JKQTPlotter* parent):
    JKQTPGraph(parent)
{
    title="";
    this->width=width;
    this->height=height;
    this->x=x;
    this->y=y;
}

JKQTPImageBase::JKQTPImageBase(JKQTPlotter *parent):
    JKQTPGraph(parent)
{
    title="";
    this->width=0;
    this->height=0;
    this->x=0;
    this->y=0;
}

void JKQTPImageBase::drawKeyMarker(JKQTPEnhancedPainter& /*painter*/, QRectF& /*rect*/) {

}

bool JKQTPImageBase::getXMinMax(double& minx, double& maxx, double& smallestGreaterZero) {
    minx=x;
    maxx=x+width;
    smallestGreaterZero=0;
    if (x>10.0*DBL_MIN) smallestGreaterZero=x;
    return true;
}

bool JKQTPImageBase::getYMinMax(double& miny, double& maxy, double& smallestGreaterZero) {
    miny=y;
    maxy=y+height;
    smallestGreaterZero=0;
    if (y>10.0*DBL_MIN) smallestGreaterZero=y;
    return true;
}

QColor JKQTPImageBase::getKeyLabelColor() {
    return QColor("black");
}

void JKQTPImageBase::plotImage(JKQTPEnhancedPainter& painter, QImage& image, double x, double y, double width, double height) {
    if ((!JKQTPIsOKFloat(x))||(!JKQTPIsOKFloat(y))||(!JKQTPIsOKFloat(width))||(!JKQTPIsOKFloat(height))||(width<=0) || (height<=0) || image.isNull() || (image.width()<=0) || (image.height()<=0)) {
        return;
    }
    painter.save();

    double xmin=parent->getXMin();
    double xmax=parent->getXMax();
    double ymin=parent->getYMin();
    double ymax=parent->getYMax();
    QPointF pp1=transform(xmin,ymax);
    QPointF pp2=transform(xmax,ymin);
    QRectF pr(pp1, pp2);

    QPointF p1=transform(x,y+height);
    QPointF p2=transform(x+width,y);
    QRectF r(p1, p2);
    if (image.width()>0 && image.height()>0 && !image.isNull()) {
        if (r.width()<2*pr.width() && r.height()<2*pr.height()) {
            //painter.drawImage(QRectF(p1.x(), p2.y(), fabs(p2.x()-p1.x()), fabs(p2.y()-p1.y())), image);
            painter.drawImage(QPoint(p1.x(), p1.y()), image.scaled(QSize(fabs(p2.x()-p1.x()), fabs(p2.y()-p1.y())), Qt::IgnoreAspectRatio, Qt::FastTransformation));
            //qDebug()<<"\nimage.size  = "<<image.size() <<" SIMPLE!";
        } else {
            double pixwidth=fabs(p2.x()-p1.x())/static_cast<double>(image.width());
            double pixheight=fabs(p2.y()-p1.y())/static_cast<double>(image.height());
            //qDebug()<<"\nimage.size  = "<<image.size();
            //qDebug()<<"size  = "<<QSizeF(width, height);
            //qDebug()<<"pixsize  = "<<QSizeF(pixwidth, pixheight);
            //qDebug()<<"p1  = "<<p1<< "    p2  = "<<p2;
            //qDebug()<<"pp1 = "<<pp1<<"    pp2 = "<<pp2;
            QPoint ps1(floor((pp1.x()-p1.x())/pixwidth), floor((pp1.y()-p1.y())/pixheight));
            QPoint ps2=ps1+QPoint(ceil(fabs(pp2.x()-pp1.x())/pixwidth), ceil(fabs(pp2.y()-pp1.y())/pixheight));
            if (ps1.x()<0) ps1.setX(0);
            if (ps1.y()<0) ps1.setY(0);
            if (ps2.x()>image.width()) ps2.setX(image.width());
            if (ps2.y()>image.height()) ps2.setY(image.height());
            QRect source(ps1, ps2);
            QRectF target(p1.x()+ps1.x()*pixwidth, p1.y()+ps1.y()*pixheight, source.width()*pixwidth, source.height()*pixheight);
            //qDebug()<<"source = "<<source;
            //qDebug()<<"target = "<<target;
            painter.drawImage(target, image, source);
        }
    }
    painter.restore();
}











JKQTPImage::JKQTPImage(JKQTBasePlotter *parent):
    JKQTPImageBase(parent), image_owned(false)
{
    this->image=nullptr;
    createImageActions();
}

JKQTPImage::JKQTPImage(double x, double y, double width, double height, QImage* image, JKQTBasePlotter* parent):
    JKQTPImageBase(x, y, width, height, parent), image_owned(false)
{
    this->image=image;
    createImageActions();
}

JKQTPImage::JKQTPImage(JKQTPlotter *parent):
    JKQTPImageBase(parent), image_owned(false)
{
    this->image=nullptr;
    createImageActions();
}

JKQTPImage::JKQTPImage(double x, double y, double width, double height, QImage* image, JKQTPlotter* parent):
    JKQTPImageBase(x, y, width, height, parent), image_owned(false)
{
    this->image=image;
    createImageActions();
}

JKQTPImage::JKQTPImage(double x, double y, double width, double height, const QImage &image, JKQTBasePlotter *parent):
    JKQTPImageBase(x, y, width, height, parent), image_owned(true)
{
    this->image=new QImage(image);
    createImageActions();
}

JKQTPImage::JKQTPImage(double x, double y, double width, double height, const QImage &image, JKQTPlotter *parent):
    JKQTPImageBase(x, y, width, height, parent), image_owned(true)
{
    this->image=new QImage(image);
    createImageActions();
}

JKQTPImage::~JKQTPImage()
{
    clear_image();
}


void JKQTPImage::draw(JKQTPEnhancedPainter& painter)  {
    if (image) plotImage(painter, *image, x, y, width, height);
}

void JKQTPImage::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    painter.drawImage(rect, QPixmap(":/JKQTPlotter/jkqtp_plot_image.png").toImage());
}

void JKQTPImage::setImage(const QImage &image)
{
    clear_image();
    this->image=new QImage(image);
    image_owned=true;
    createImageActions();
}

void JKQTPImage::setImage(QImage *image)
{
    clear_image();
    this->image=image;
    image_owned=false;
    createImageActions();
}

void JKQTPImage::clear_image()
{
    if (image_owned && image!=nullptr) {
        delete image;
    }
    image=nullptr;
    image_owned=false;
}

void JKQTPImage::createImageActions()
{
    actSaveImage=new QAction(tr("Save JKQTPImage ..."), this);
    connect(actSaveImage, SIGNAL(triggered()), this, SLOT(saveImagePlotAsImage()));
    actCopyImage=new QAction(tr("Copy JKQTPOverlayImage ..."), this);
    connect(actCopyImage, SIGNAL(triggered()), this, SLOT(copyImagePlotAsImage()));

}

void JKQTPImage::setParent(JKQTBasePlotter *parent)
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

void JKQTPImage::setTitle(const QString &title)
{
    JKQTPImageBase::setTitle(title);
    QString t=title;
    if (t.isEmpty()) t="JKQTPImage";
    actSaveImage->setText(tr("Save %1 ...").arg(t));
    actCopyImage->setText(tr("Copy %1 ...").arg(t));
}

void JKQTPImage::saveImagePlotAsImage(const QString &filename, const QByteArray &outputFormat)
{
    if (parent && image) {
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

            if (form=="NONE") image->save(fn);
            else image->save(fn, form.toLatin1().data());
        }
    }
}

void JKQTPImage::copyImagePlotAsImage()
{
    QClipboard* clip=QApplication::clipboard();
    if (clip && image) {
        clip->setPixmap(QPixmap::fromImage(*image));
    }
}








JKQTPMathImageBase::JKQTPMathImageBase(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTBasePlotter* parent):
    JKQTPImageBase(x, y, width, height, parent)
{
    this->data=data;
    this->datatype=datatype;
    this->Nx=Nx;
    this->Ny=Ny;
    dataModifier=nullptr;
    datatypeModifier=DoubleArray;
    modifierMode=ModifyNone;
}


JKQTPMathImageBase::JKQTPMathImageBase(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTPlotter* parent):
    JKQTPImageBase(x, y, width, height, parent)
{
    this->data=data;
    this->datatype=datatype;
    this->Nx=Nx;
    this->Ny=Ny;
    dataModifier=nullptr;
    datatypeModifier=DoubleArray;
    modifierMode=ModifyNone;
}
void JKQTPMathImageBase::drawKeyMarker(JKQTPEnhancedPainter &/*painter*/, QRectF &/*rect*/)
{

}

JKQTPMathImageBase::ModifierMode JKQTPMathImageBase::StringToModifierMode(const QString &mode) {
    QString m=mode.toLower();
    if (m=="value" ) return ModifyValue;
    if (m=="saturation" ) return ModifySaturation;
    if (m=="alpha" ) return ModifyAlpha;

    return ModifyNone;
}

QString JKQTPMathImageBase::ModifierModeToString(const JKQTPMathImageBase::ModifierMode &mode) {

    if (mode==ModifyValue) return "value";
    if (mode==ModifySaturation) return "saturation";
    if (mode==ModifyAlpha) return "alpha";
    if (mode==ModifyLuminance) return "luminance";
    if (mode==ModifyHue) return "hue";
    return "none";
}

JKQTPMathImageBase::JKQTPMathImageBase(JKQTBasePlotter *parent):
    JKQTPImageBase(parent)
{
    this->data=nullptr;
    this->Nx=0;
    this->Ny=0;
    this->datatype=DoubleArray;
    dataModifier=nullptr;
    datatypeModifier=DoubleArray;
    modifierMode=ModifyNone;
}

JKQTPMathImageBase::JKQTPMathImageBase(double x, double y, double width, double height, JKQTBasePlotter *parent):
    JKQTPImageBase(x,y,width,height,parent)
{
    this->data=nullptr;
    this->Nx=0;
    this->Ny=0;
    this->datatype=DoubleArray;
    dataModifier=nullptr;
    datatypeModifier=DoubleArray;
    modifierMode=ModifyNone;
}

JKQTPMathImageBase::JKQTPMathImageBase(JKQTPlotter *parent):
    JKQTPImageBase(parent)
{
    this->data=nullptr;
    this->Nx=0;
    this->Ny=0;
    this->datatype=DoubleArray;
    dataModifier=nullptr;
    datatypeModifier=DoubleArray;
    modifierMode=ModifyNone;
}

JKQTPMathImageBase::JKQTPMathImageBase(double x, double y, double width, double height, JKQTPlotter *parent):
    JKQTPImageBase(x,y,width,height,parent)
{
    this->data=nullptr;
    this->Nx=0;
    this->Ny=0;
    this->datatype=DoubleArray;
    dataModifier=nullptr;
    datatypeModifier=DoubleArray;
    modifierMode=ModifyNone;
}

void JKQTPMathImageBase::setData(void* data, int Nx, int Ny, DataType datatype) {
    this->data=data;
    this->datatype=datatype;
    this->Nx=Nx;
    this->Ny=Ny;
}

void JKQTPMathImageBase::setData(void* data, int Nx, int Ny) {
    this->data=data;
    this->Nx=Nx;
    this->Ny=Ny;
}

void JKQTPMathImageBase::setDataModifier(void *data, JKQTPMathImageBase::DataType datatype)
{
    this->dataModifier=data;
    this->datatypeModifier=datatype;
}


void JKQTPMathImageBase::getDataMinMax(double& imin, double& imax) {
    ensureImageData();
    imin=imax=0;
    if (!data) return;
    switch(datatype) {
        case JKQTPMathImageBase::DoubleArray:
            imin= JKQTPImagePlot_getImageMin<double>(static_cast<double*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<double>(static_cast<double*>(data), Nx, Ny);
            break;
        case JKQTPMathImageBase::FloatArray:
            imin= JKQTPImagePlot_getImageMin<float>(static_cast<float*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<float>(static_cast<float*>(data), Nx, Ny);
            break;
        case JKQTPMathImageBase::UInt8Array:
            imin= JKQTPImagePlot_getImageMin<uint8_t>(static_cast<uint8_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint8_t>(static_cast<uint8_t*>(data), Nx, Ny);
            break;
        case JKQTPMathImageBase::UInt16Array:
            imin= JKQTPImagePlot_getImageMin<uint16_t>(static_cast<uint16_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint16_t>(static_cast<uint16_t*>(data), Nx, Ny);
            break;
        case JKQTPMathImageBase::UInt32Array:
            imin= JKQTPImagePlot_getImageMin<uint32_t>(static_cast<uint32_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint32_t>(static_cast<uint32_t*>(data), Nx, Ny);
            break;
        case JKQTPMathImageBase::UInt64Array:
            imin= JKQTPImagePlot_getImageMin<uint64_t>(static_cast<uint64_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint64_t>(static_cast<uint64_t*>(data), Nx, Ny);
            break;
        case JKQTPMathImageBase::Int8Array:
            imin= JKQTPImagePlot_getImageMin<int8_t>(static_cast<int8_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int8_t>(static_cast<int8_t*>(data), Nx, Ny);
            break;
        case JKQTPMathImageBase::Int16Array:
            imin= JKQTPImagePlot_getImageMin<int16_t>(static_cast<int16_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int16_t>(static_cast<int16_t*>(data), Nx, Ny);
            break;
        case JKQTPMathImageBase::Int32Array:
            imin= JKQTPImagePlot_getImageMin<int32_t>(static_cast<int32_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int32_t>(static_cast<int32_t*>(data), Nx, Ny);
            break;
        case JKQTPMathImageBase::Int64Array:
            imin= JKQTPImagePlot_getImageMin<int64_t>(static_cast<int64_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int64_t>(static_cast<int64_t*>(data), Nx, Ny);
            break;
    }
}

void JKQTPMathImageBase::getModifierMinMax(double &imin, double &imax)
{
    ensureImageData();
    imin=imax=0;
    if (!dataModifier) return;
    switch(datatypeModifier) {
        case JKQTPMathImageBase::DoubleArray:
            imin= JKQTPImagePlot_getImageMin<double>(static_cast<double*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<double>(static_cast<double*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageBase::FloatArray:
            imin= JKQTPImagePlot_getImageMin<float>(static_cast<float*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<float>(static_cast<float*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageBase::UInt8Array:
            imin= JKQTPImagePlot_getImageMin<uint8_t>(static_cast<uint8_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint8_t>(static_cast<uint8_t*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageBase::UInt16Array:
            imin= JKQTPImagePlot_getImageMin<uint16_t>(static_cast<uint16_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint16_t>(static_cast<uint16_t*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageBase::UInt32Array:
            imin= JKQTPImagePlot_getImageMin<uint32_t>(static_cast<uint32_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint32_t>(static_cast<uint32_t*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageBase::UInt64Array:
            imin= JKQTPImagePlot_getImageMin<uint64_t>(static_cast<uint64_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint64_t>(static_cast<uint64_t*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageBase::Int8Array:
            imin= JKQTPImagePlot_getImageMin<int8_t>(static_cast<int8_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int8_t>(static_cast<int8_t*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageBase::Int16Array:
            imin= JKQTPImagePlot_getImageMin<int16_t>(static_cast<int16_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int16_t>(static_cast<int16_t*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageBase::Int32Array:
            imin= JKQTPImagePlot_getImageMin<int32_t>(static_cast<int32_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int32_t>(static_cast<int32_t*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageBase::Int64Array:
            imin= JKQTPImagePlot_getImageMin<int64_t>(static_cast<int64_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int64_t>(static_cast<int64_t*>(dataModifier), Nx, Ny);
            break;
    }
}

QVector<double> JKQTPMathImageBase::getDataAsDoubleVector() const
{
    switch(datatype) {
        case JKQTPMathImageBase::DoubleArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<double*>(data), Nx*Ny);
            break;
        case JKQTPMathImageBase::FloatArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<float*>(data), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint8_t*>(data), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint16_t*>(data), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint32_t*>(data), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint64_t*>(data), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int8_t*>(data), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int16_t*>(data), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int32_t*>(data), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int64_t*>(data), Nx*Ny);
            break;
    }
    QVector<double> res;
    return res;
}

QVector<double> JKQTPMathImageBase::getDataModifierAsDoubleVector() const
{
    switch(datatypeModifier) {
        case JKQTPMathImageBase::DoubleArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<double*>(dataModifier), Nx*Ny);
            break;
        case JKQTPMathImageBase::FloatArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<float*>(dataModifier), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint8_t*>(dataModifier), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint16_t*>(dataModifier), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint32_t*>(dataModifier), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint64_t*>(dataModifier), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int8_t*>(dataModifier), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int16_t*>(dataModifier), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int32_t*>(dataModifier), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int64_t*>(dataModifier), Nx*Ny);
            break;
    }
    QVector<double> res;
    return res;
}

void JKQTPMathImageBase::ensureImageData()
{
}


void JKQTPMathImageBase::modifyImage(QImage &img)
{
    getModifierMinMax(internalModifierMin, internalModifierMax);
    modifyImage(img, dataModifier, datatypeModifier, Nx, Ny, internalModifierMin,  internalModifierMax);
}

void JKQTPMathImageBase::modifyImage(QImage &img, void *dataModifier, JKQTPMathImageBase::DataType datatypeModifier, int Nx, int Ny, double internalModifierMin, double internalModifierMax)
{
    if (!dataModifier) return;
    //getModifierMinMax(internalModifierMin, internalModifierMax);
    if (modifierMode!=ModifyNone) {
        JKQTPRGBMathImageRGBMode rgbModMode=JKQTPRGBMathImageModeRGBMode;
        int modChannel=3;
        if (modifierMode==ModifyValue) {
            modChannel=2;
            rgbModMode=JKQTPRGBMathImageModeHSVMode;
        } else if (modifierMode==ModifySaturation) {
            modChannel=1;
            rgbModMode=JKQTPRGBMathImageModeHSVMode;
        } else if (modifierMode==ModifyAlpha) {
            modChannel=3;
            rgbModMode=JKQTPRGBMathImageModeRGBMode;
        } else if (modifierMode==ModifyLuminance) {
            modChannel=2;
            rgbModMode=JKQTPRGBMathImageModeHSLMode;
        } else if (modifierMode==ModifyHue) {
            modChannel=0;
            rgbModMode=JKQTPRGBMathImageModeHSLMode;
        }
        //qDebug()<<"mod: "<<modifierMode<<"  ch:"<<modChannel<<"  rgb:"<<rgbModMode;
        switch(datatypeModifier) {
            case JKQTPMathImageBase::DoubleArray: JKQTPImagePlot_array2RGBimage<double>(static_cast<double*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageBase::FloatArray: JKQTPImagePlot_array2RGBimage<float>(static_cast<float*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageBase::UInt8Array: JKQTPImagePlot_array2RGBimage<uint8_t>(static_cast<uint8_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageBase::UInt16Array: JKQTPImagePlot_array2RGBimage<uint16_t>(static_cast<uint16_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageBase::UInt32Array: JKQTPImagePlot_array2RGBimage<uint32_t>(static_cast<uint32_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageBase::UInt64Array: JKQTPImagePlot_array2RGBimage<uint64_t>(static_cast<uint64_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageBase::Int8Array: JKQTPImagePlot_array2RGBimage<int8_t>(static_cast<int8_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageBase::Int16Array: JKQTPImagePlot_array2RGBimage<int16_t>(static_cast<int16_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageBase::Int32Array: JKQTPImagePlot_array2RGBimage<int32_t>(static_cast<int32_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
            case JKQTPMathImageBase::Int64Array: JKQTPImagePlot_array2RGBimage<int64_t>(static_cast<int64_t*>(dataModifier), Nx, Ny, img, modChannel, internalModifierMin, internalModifierMax, rgbModMode); break;
        }
    }
}





void JKQTPMathImage::initJKQTPMathImage() {
    actSaveImage=new QAction(tr("Save JKQTPMathImage ..."), this);
    connect(actSaveImage, SIGNAL(triggered()), this, SLOT(saveImagePlotAsImage()));
    actCopyImage=new QAction(tr("Copy JKQTPMathImage ..."), this);
    connect(actCopyImage, SIGNAL(triggered()), this, SLOT(copyImagePlotAsImage()));
    actSavePalette=new QAction(tr("Save JKQTPMathImage Palette/Colorbar ..."), this);
    connect(actSavePalette, SIGNAL(triggered()), this, SLOT(saveColorbarPlotAsImage()));
    actCopyPalette=new QAction(tr("Copy JKQTPMathImage Palette/Colorbar ..."), this);
    connect(actCopyPalette, SIGNAL(triggered()), this, SLOT(copyColorbarPlotAsImage()));

    colorBarRightAxis=new JKQTPVerticalIndependentAxis(0, 100, 0, 100, parent);
    if (parent) colorBarRightAxis->loadSettings(parent->getYAxis());
    colorBarRightAxis->setDrawMode1(JKQTPCADMline);
    colorBarRightAxis->setDrawMode2(JKQTPCADMcomplete);
    colorBarRightAxis->setAxisLabel("");
    colorBarRightAxis->setMinTicks(5);
    colorBarRightAxis->setMinorTicks(0);
    colorBarRightAxis->setTickOutsideLength(0);
    colorBarRightAxis->setMinorTickOutsideLength(0);
    colorBarRightAxis->setShowZeroAxis(false);
    colorBarTopAxis=new JKQTPHorizontalIndependentAxis(0, 100, 0, 100, parent);
    if (parent) colorBarTopAxis->loadSettings(parent->getXAxis());
    colorBarTopAxis->setDrawMode1(JKQTPCADMline);
    colorBarTopAxis->setDrawMode2(JKQTPCADMcomplete);
    colorBarTopAxis->setAxisLabel("");
    colorBarTopAxis->setMinTicks(3);
    colorBarTopAxis->setMinorTicks(0);
    colorBarTopAxis->setTickOutsideLength(0);
    colorBarTopAxis->setMinorTickOutsideLength(0);
    colorBarTopAxis->setShowZeroAxis(false);

    modifierColorBarTopAxis=new JKQTPVerticalIndependentAxis(0, 100, 0, 100, parent);
    if (parent) modifierColorBarTopAxis->loadSettings(parent->getXAxis());
    modifierColorBarTopAxis->setDrawMode1(JKQTPCADMline);
    modifierColorBarTopAxis->setDrawMode2(JKQTPCADMcomplete);
    modifierColorBarTopAxis->setAxisLabel("");
    modifierColorBarTopAxis->setMinTicks(3);
    modifierColorBarTopAxis->setShowZeroAxis(false);
    modifierColorBarTopAxis->setMinorTicks(0);
    modifierColorBarTopAxis->setTickOutsideLength(0);
    modifierColorBarTopAxis->setMinorTickOutsideLength(0);
    modifierColorBarRightAxis=new JKQTPHorizontalIndependentAxis (0, 100, 0, 100, parent);
    if (parent) modifierColorBarRightAxis->loadSettings(parent->getYAxis());
    modifierColorBarRightAxis->setDrawMode1(JKQTPCADMline);
    modifierColorBarRightAxis->setDrawMode2(JKQTPCADMcomplete);
    modifierColorBarRightAxis->setAxisLabel("");
    modifierColorBarRightAxis->setMinTicks(5);
    modifierColorBarRightAxis->setShowZeroAxis(false);
    modifierColorBarRightAxis->setMinorTicks(0);
    modifierColorBarRightAxis->setTickOutsideLength(0);
    modifierColorBarRightAxis->setMinorTickOutsideLength(0);
    this->colorBarModifiedWidth=80;

    this->colorBarTopVisible=true;
    this->colorBarRightVisible=true;
    this->palette=JKQTPMathImageGRAY;
    this->imageNameFontName=parent->getKeyFont();
    this->imageNameFontSize=parent->getKeyFontSize();
    this->imageName="";
    this->showColorBar=true;
    this->colorBarWidth=14;
    this->colorBarRelativeHeight=0.75;
    this->autoImageRange=true;
    this->imageMin=0;
    this->imageMax=1;
    this->colorBarOffset=4;
    this->rangeMinFailAction=JKQTPMathImageLastPaletteColor;
    this->rangeMaxFailAction=JKQTPMathImageLastPaletteColor;
    this->rangeMinFailColor=QColor("black");
    this->rangeMaxFailColor=QColor("black");
    this->nanColor=QColor("black");
    this->infColor=QColor("black");
    this->colorBarTopVisible=false;
    this->colorBarRightVisible=true;
    this->autoModifierRange=true;
}

JKQTPMathImage::JKQTPMathImage(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette, JKQTBasePlotter* parent):
    JKQTPMathImageBase(x, y, width, height, datatype, data, Nx, Ny, parent)
{
    initJKQTPMathImage();
    this->palette=palette;
}

JKQTPMathImage::JKQTPMathImage(JKQTBasePlotter *parent):
    JKQTPMathImageBase(0, 0, 1, 1, JKQTPMathImageBase::UInt8Array, nullptr, 0, 0, parent)
{
    initJKQTPMathImage();
}



JKQTPMathImage::JKQTPMathImage(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette, JKQTPlotter* parent):
    JKQTPMathImageBase(x, y, width, height, datatype, data, Nx, Ny, parent)
{
    initJKQTPMathImage();
    this->palette=palette;
}

JKQTPMathImage::JKQTPMathImage(JKQTPlotter *parent):
    JKQTPMathImageBase(0, 0, 1, 1, JKQTPMathImageBase::UInt8Array, nullptr, 0, 0, parent)
{
    initJKQTPMathImage();
}

void JKQTPMathImage::setParent(JKQTBasePlotter* parent) {
    if (this->parent) {
        this->parent->deregisterAdditionalAction(actSaveImage);
        this->parent->deregisterAdditionalAction(actCopyImage);
        this->parent->deregisterAdditionalAction(actSavePalette);
        this->parent->deregisterAdditionalAction(actCopyPalette);
    }
    JKQTPMathImageBase::setParent(parent);
    colorBarRightAxis->setParent(parent);
    colorBarTopAxis->setParent(parent);
    modifierColorBarRightAxis->setParent(parent);
    modifierColorBarTopAxis->setParent(parent);
    if (parent) {
        parent->registerAdditionalAction(tr("Save Image Plot Images ..."), actSaveImage);
        parent->registerAdditionalAction(tr("Copy Image Plot Images ..."), actCopyImage);
        parent->registerAdditionalAction(tr("Save Image Plot Images ..."), actSavePalette);
        parent->registerAdditionalAction(tr("Copy Image Plot Images ..."), actCopyPalette);
    }
    actSaveImage->setEnabled(parent);
    actCopyImage->setEnabled(parent);
    actSavePalette->setEnabled(parent);
    actCopyPalette->setEnabled(parent);
}

void JKQTPMathImage::setTitle(const QString &title)
{
    JKQTPMathImageBase::setTitle(title);
    QString t=title;
    if (t.isEmpty()) t="JKQTPMathImage";
    actSaveImage->setText(tr("Save %1 ...").arg(t));
    actCopyImage->setText(tr("Copy %1 ...").arg(t));
}

void JKQTPMathImage::saveImagePlotAsImage(const QString &filename, const QByteArray &outputFormat)
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

void JKQTPMathImage::copyImagePlotAsImage()
{
    QClipboard* clip=QApplication::clipboard();
    if (clip) {
        clip->setPixmap(QPixmap::fromImage(drawImage()));
    }
}

void JKQTPMathImage::saveColorbarPlotAsImage(const QString &filename, const QByteArray &outputFormat)
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

            QImage image=drawOutsidePalette(200);

            if (form=="NONE") image.save(fn);
            else image.save(fn, form.toLatin1().data());
        }
    }
}

void JKQTPMathImage::copyColorbarPlotAsImage()
{
    QClipboard* clip=QApplication::clipboard();
    if (clip) {
        clip->setPixmap(QPixmap::fromImage(drawOutsidePalette(20)));
    }
}

void JKQTPMathImage::draw(JKQTPEnhancedPainter& painter) {
    ensureImageData();
    if (!data) return;
    QImage img=drawImage();
    plotImage(painter, img, x, y, width, height);
}



void JKQTPMathImage::getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace) {
    JKQTPGraph::getOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
    if (showColorBar) {
        getDataMinMax(internalDataMin, internalDataMax);
        getModifierMinMax(internalModifierMin, internalModifierMax);
        if (colorBarRightVisible) {
            rightSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
            colorBarRightAxis->setRange(internalDataMin, internalDataMax);
            colorBarRightAxis->setAxisWidth(colorBarRelativeHeight*parent->getPlotHeight());
            modifierColorBarRightAxis->setRange(internalModifierMin, internalModifierMax);
            modifierColorBarRightAxis->setAxisWidth(parent->pt2px(painter, colorBarModifiedWidth));
            QSize s=colorBarRightAxis->getSize2(painter);
            /*parent->getMathText()->setFontSize(imageNameFontSize*parent->getFontSizeMultiplier());
            parent->getMathText()->setFontRoman(imageNameFontName);
            parent->getMathText()->parse(imageName);
            QSizeF names=parent->getMathText()->getSize(painter);*/
            QSizeF names=parent->getTextSizeSize(imageNameFontName, imageNameFontSize*parent->getFontSizeMultiplier(), imageName, painter);
            rightSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+qMax(static_cast<double>(s.width()), static_cast<double>(names.width()));
            //qDebug()<<"osr: "<<imageName<<rightSpace<<colorBarModifiedWidth<<colorBarWidth;
            if (modifierMode!=ModifyNone) {
                rightSpace=rightSpace+(2*parent->pt2px(painter, colorBarModifiedWidth-colorBarWidth));
            }
            //qDebug()<<"     "<<imageName<<rightSpace;
        }
        if (colorBarTopVisible) {
            //topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
            colorBarTopAxis->setRange(internalDataMin, internalDataMax);
            colorBarTopAxis->setAxisWidth(colorBarRelativeHeight*parent->getPlotWidth());
            modifierColorBarTopAxis->setRange(internalModifierMin, internalModifierMax);
            modifierColorBarTopAxis->setAxisWidth(parent->pt2px(painter, colorBarModifiedWidth));
            QSize s=colorBarTopAxis->getSize2(painter);
            /*parent->getMathText()->setFontSize(imageNameFontSize*parent->getFontSizeMultiplier());
            parent->getMathText()->setFontRoman(imageNameFontName);
            parent->getMathText()->parse(imageName);
            QSizeF names=parent->getMathText()->getSize(painter);*/
            QSizeF names=parent->getTextSizeSize(imageNameFontName, imageNameFontSize*parent->getFontSizeMultiplier(), imageName, painter);
            //qDebug()<<topSpace<<s<<imageName<<names;
            //qDebug()<<colorBarWidth<<colorBarOffset<<parent->pt2px(painter, colorBarWidth+colorBarOffset);
            topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+qMax(static_cast<double>(s.height()), static_cast<double>(names.height()));
            if (modifierMode!=ModifyNone) {
                topSpace=topSpace+(2*parent->pt2px(painter, colorBarModifiedWidth-colorBarWidth));
            }
            //qDebug()<<topSpace;
            //qDebug()<<"ost: "<<imageName<<topSpace<<colorBarModifiedWidth<<colorBarWidth;
        }
    }
}

void JKQTPMathImage::drawOutside(JKQTPEnhancedPainter& painter, QRect /*leftSpace*/, QRect rightSpace, QRect topSpace, QRect /*bottomSpace*/) {
    if (showColorBar) {
        getDataMinMax(internalDataMin, internalDataMax);
        getModifierMinMax(internalModifierMin, internalModifierMax);
        if (colorBarRightVisible) {
            painter.save();



            QImage b=drawOutsidePalette(200);


            QSizeF names=parent->getTextSizeSize(imageNameFontName, imageNameFontSize*parent->getFontSizeMultiplier(), imageName, painter);

            double icolorBarRelativeHeight=colorBarRelativeHeight;
            int barHeight=rightSpace.height()*icolorBarRelativeHeight;
            if (barHeight<1) barHeight=1;
            // find a height for the bar that allows to show the image name
            while ((barHeight>1) && ((rightSpace.height()-barHeight)/2<names.height())) {
                barHeight--;
            }

            //qDebug()<<"r: "<<imageName<<rightSpace;

            QRect cb(rightSpace.x()+parent->pt2px(painter, colorBarOffset), rightSpace.top()+(rightSpace.height()-barHeight)/2, parent->pt2px(painter, (modifierMode==ModifyNone)?colorBarWidth:colorBarModifiedWidth), barHeight);
            painter.drawImage(cb, b.mirrored(true, false));
            QPen p=painter.pen();
            p.setColor(colorBarRightAxis->getAxisColor());
            p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, colorBarRightAxis->getLineWidth()*parent->getLineWidthMultiplier())));
            painter.setPen(p);
            painter.drawRect(cb);

            colorBarRightAxis->setRange(internalDataMin, internalDataMax);
            colorBarRightAxis->setAxisWidth(cb.height());
            colorBarRightAxis->setAxisOffset(cb.top());
            colorBarRightAxis->setOtherAxisOffset(cb.left());
            colorBarRightAxis->setOtherAxisWidth(cb.width());
            colorBarRightAxis->drawAxes(painter);

            if (modifierMode!=ModifyNone) {
                modifierColorBarRightAxis->setRange(internalModifierMin, internalModifierMax);
                modifierColorBarRightAxis->setAxisWidth(parent->pt2px(painter, colorBarModifiedWidth));
                modifierColorBarRightAxis->setAxisOffset(cb.left());
                modifierColorBarRightAxis->setOtherAxisOffset(cb.top());
                modifierColorBarRightAxis->setOtherAxisWidth(cb.height());
                modifierColorBarRightAxis->drawAxes(painter);
            }

            parent->getMathText()->setFontSize(imageNameFontSize*parent->getFontSizeMultiplier());
            parent->getMathText()->setFontRoman(imageNameFontName);
            parent->getMathText()->parse(imageName);
            parent->getMathText()->draw(painter, Qt::AlignHCenter|Qt::AlignVCenter, QRect(rightSpace.x(), rightSpace.y(), rightSpace.width(), (rightSpace.height()-barHeight)/2));

            painter.restore();
        }
        if (colorBarTopVisible) {
            painter.save();





            QImage b=drawOutsidePalette(200);


            QSizeF names=parent->getTextSizeSize(imageNameFontName, imageNameFontSize*parent->getFontSizeMultiplier(), imageName, painter);

            double icolorBarRelativeHeight=colorBarRelativeHeight;
            int barWidth=topSpace.width()*icolorBarRelativeHeight;
            if (barWidth<1) barWidth=1;
            // find a height for the bar that allows to show the image name
            while ((barWidth>1) && ((topSpace.width()-barWidth)/2<names.width())) {
                barWidth--;
            }


            QRect cb(topSpace.x()+(topSpace.width()-barWidth)/2, topSpace.bottom()-parent->pt2px(painter, colorBarOffset+((modifierMode==ModifyNone)?colorBarWidth:colorBarModifiedWidth)), barWidth, parent->pt2px(painter, (modifierMode==ModifyNone)?colorBarWidth:colorBarModifiedWidth));
            //qDebug()<<"t: "<<imageName<<topSpace<<topSpace.bottom()<<colorBarOffset<<"\n     "<<cb;
            QMatrix rm;
            rm.rotate(90);
            painter.drawImage(cb, b.transformed(rm));
            QPen p=painter.pen();
            p.setColor(colorBarTopAxis->getAxisColor());
            p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, colorBarTopAxis->getLineWidth()*parent->getLineWidthMultiplier())));
            painter.setPen(p);
            painter.drawRect(cb);


            colorBarTopAxis->setRange(internalDataMin, internalDataMax);
            colorBarTopAxis->setAxisWidth(cb.width());
            colorBarTopAxis->setAxisOffset(cb.left());
            colorBarTopAxis->setOtherAxisOffset(cb.top());
            colorBarTopAxis->setOtherAxisWidth(cb.height());
            colorBarTopAxis->drawAxes(painter);

            if (modifierMode!=ModifyNone) {
                modifierColorBarTopAxis->setRange(internalModifierMin, internalModifierMax);
                modifierColorBarTopAxis->setAxisWidth(parent->pt2px(painter, colorBarModifiedWidth));
                modifierColorBarTopAxis->setAxisOffset(cb.top());
                modifierColorBarTopAxis->setOtherAxisOffset(cb.left());
                modifierColorBarTopAxis->setOtherAxisWidth(cb.width());
                modifierColorBarTopAxis->drawAxes(painter);
            }

            parent->getMathText()->setFontSize(imageNameFontSize*parent->getFontSizeMultiplier());
            parent->getMathText()->setFontRoman(imageNameFontName);
            parent->getMathText()->parse(imageName);
            parent->getMathText()->draw(painter, Qt::AlignHCenter|Qt::AlignVCenter, QRect(topSpace.right()-(topSpace.width()-barWidth)/2, topSpace.y(), (topSpace.width()-barWidth)/2, topSpace.height()));

            painter.restore();
        }
    }
}

QImage JKQTPMathImage::drawOutsidePalette(uint8_t steps)
{

    uint8_t h=1;
    if (modifierMode!=ModifyNone) {
        h=50;
    }
    double* d=new double[steps*h];
    double* dd=new double[steps*h];
    for (uint8_t i=0; i<steps; i++) {
        for (uint8_t j=0; j<h; j++) {
            d[i*h+j]=i;
            dd[i*h+j]=j;
            //qDebug()<<"x="<<j<<", y="<<i<<": d="<<d[i*h+j]<<" dd="<<dd[i*h+j];
        }
    }

    QImage b(h,200, QImage::Format_ARGB32);
    JKQTPImagePlot_array2image<double>(d,h,steps, b, palette, 0, steps-1);
    //b.save("c:/temp/b1.png");
    if (modifierMode!=ModifyNone) {
        modifyImage(b, dd, DoubleArray, h,steps, 0, h-1);
        //b.save("c:/temp/b2.png");
    }
    delete[] d;
    delete[] dd;
    return b;
}

void JKQTPMathImage::getDataMinMax(double& imin, double& imax) {
    ensureImageData();
    if (autoImageRange) {
        JKQTPMathImageBase::getDataMinMax(imin, imax);
    } else {
        imin=imageMin;
        imax=imageMax;
    }
}

void JKQTPMathImage::getModifierMinMax(double &imin, double &imax)
{
    ensureImageData();
    if (autoModifierRange) {
        JKQTPMathImageBase::getModifierMinMax(imin, imax);
    } else {
        imin=modifierMin;
        imax=modifierMax;
    }
}

double JKQTPMathImage::getValueAt(double x, double y)
{
    ensureImageData();
    int xx=trunc((x-this->x)/width*double(Nx));
    int yy=trunc((y-this->y)/height*double(Ny));
    if (xx>=0 && xx<(int64_t)Nx && yy>=0 && yy<(int64_t)Ny) {
        switch(datatype) {
            case JKQTPMathImageBase::DoubleArray: return (static_cast<double*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::FloatArray: return (static_cast<float*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::UInt8Array: return (static_cast<uint8_t*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::UInt16Array: return (static_cast<uint16_t*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::UInt32Array: return (static_cast<uint32_t*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::UInt64Array: return (static_cast<uint64_t*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::Int8Array: return (static_cast<int8_t*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::Int16Array: return (static_cast<int16_t*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::Int32Array: return (static_cast<int32_t*>(data))[yy*Nx+xx]; break;
            case JKQTPMathImageBase::Int64Array: return (static_cast<int64_t*>(data))[yy*Nx+xx]; break;
        }    }
    return 0.0;
}

void JKQTPMathImage::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    painter.drawImage(rect, getPaletteKeyImage(palette, 32,32));
}

QStringList JKQTPMathImage::getPalettes()  {
    return JKQTPImagePlot_getPredefinedPalettes();
}

int JKQTPMathImage::getPalettesCount()
{
    return getPalettes().size();
}

QIcon JKQTPMathImage::getPaletteIcon(int i)  {
    QImage img=getPaletteImage(i, jkqtp_PALETTE_ICON_WIDTH);
    QPixmap pix(jkqtp_PALETTE_ICON_WIDTH,8);
    QRect r(0,0,jkqtp_PALETTE_ICON_WIDTH-1,7);
    JKQTPEnhancedPainter p(&pix);
    p.drawImage(r, img);
    p.setPen(QPen(QColor("black")));
    p.drawRect(r);
    p.end();
    return QIcon(pix);

}

QIcon JKQTPMathImage::getPaletteIcon(JKQTPMathImageColorPalette palette)  {
    return getPaletteIcon(static_cast<int>(palette));
}

QImage JKQTPMathImage::getPaletteImage(int i, int width, int height)
{
    QImage img;
    double* pic=static_cast<double*>(malloc(width*sizeof(double)));
    for (int j=0; j<width; j++) {
        pic[j]=j;
    }
    JKQTPImagePlot_array2image<double>(pic, width, qMax(1,height), img, (JKQTPMathImageColorPalette)i, 0, width-1);
    free(pic);
    return img;
}

QImage JKQTPMathImage::getPaletteImage(JKQTPMathImageColorPalette palette, int width, int height)
{
    return getPaletteImage(static_cast<int>(palette), width,height);
}

QIcon JKQTPMathImage::getPaletteKeyIcon(int i)
{
    QImage img=getPaletteKeyImage(i, jkqtp_PALETTE_ICON_WIDTH, jkqtp_PALETTE_IMAGEICON_HEIGHT);
    QPixmap pix(jkqtp_PALETTE_ICON_WIDTH,jkqtp_PALETTE_IMAGEICON_HEIGHT);
    QRect r(0,0,jkqtp_PALETTE_ICON_WIDTH-1,jkqtp_PALETTE_IMAGEICON_HEIGHT-1);
    JKQTPEnhancedPainter p(&pix);
    p.drawImage(r, img);
    p.setPen(QPen(QColor("black")));
    p.drawRect(r);
    p.end();
    return QIcon(pix);
}

QIcon JKQTPMathImage::getPaletteKeyIcon(JKQTPMathImageColorPalette palette)
{
    return getPaletteIcon(static_cast<int>(palette));
}

QImage JKQTPMathImage::getPaletteKeyImage(int i, int width, int height)
{
    QImage img;

    const double x01=double(width)/3.0;
    const double y01=double(height)/3.0*2.0;
    const double w1x=double(width*width)/(5.0*5.0);
    const double w1y=double(height*height)/(2.0*2.0);
    const double x02=double(width)/3.0*2.0;
    const double y02=double(height)/4.0;
    const double w2x=double(width*width)/(8.0*8.0);
    const double w2y=double(height*height)/(8.0*8.0);

    double* pic=static_cast<double*>(malloc(width*height*sizeof(double)));
    double mmax=0;
    for (int j=0; j<width*height; j++) {
        const int x=j%width;
        const int y=j/width;
        pic[j]=exp(-0.5*(double((x-x01)*double(x-x01))/w1x+double((y-y01)*double(y-y01))/w1y))+0.7*exp(-0.5*(double((x-x02)*double(x-x02))/w2x+double((y-y02)*double(y-y02))/w2y));
        if (pic[j]>mmax) mmax=pic[j];
    }
    JKQTPImagePlot_array2image<double>(pic, width, height, img, (JKQTPMathImageColorPalette)i, 0, mmax);
    free(pic);
    return img;
}

QImage JKQTPMathImage::getPaletteKeyImage(JKQTPMathImageColorPalette palette, int width, int height)
{
    return getPaletteKeyImage(static_cast<int>(palette), width, height);
}



QImage JKQTPMathImage::drawImage() {
    if (!data) return QImage();
    QImage img(Nx, Ny, QImage::Format_ARGB32);
    getDataMinMax(internalDataMin, internalDataMax);
    switch(datatype) {
        case JKQTPMathImageBase::DoubleArray: JKQTPImagePlot_array2image<double>(static_cast<double*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageBase::FloatArray: JKQTPImagePlot_array2image<float>(static_cast<float*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageBase::UInt8Array: JKQTPImagePlot_array2image<uint8_t>(static_cast<uint8_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageBase::UInt16Array: JKQTPImagePlot_array2image<uint16_t>(static_cast<uint16_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageBase::UInt32Array: JKQTPImagePlot_array2image<uint32_t>(static_cast<uint32_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageBase::UInt64Array: JKQTPImagePlot_array2image<uint64_t>(static_cast<uint64_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageBase::Int8Array: JKQTPImagePlot_array2image<int8_t>(static_cast<int8_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageBase::Int16Array: JKQTPImagePlot_array2image<int16_t>(static_cast<int16_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageBase::Int32Array: JKQTPImagePlot_array2image<int32_t>(static_cast<int32_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageBase::Int64Array: JKQTPImagePlot_array2image<int64_t>(static_cast<int64_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
    }
    modifyImage(img);
    return img;
}


void JKQTPMathImage::setPalette(int pal) {
    palette=(JKQTPMathImageColorPalette)pal;
}






















JKQTPOverlayImage::JKQTPOverlayImage(double x, double y, double width, double height, bool* data, int Nx, int Ny, QColor colTrue, JKQTBasePlotter* parent):
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

JKQTPOverlayImage::JKQTPOverlayImage(double x, double y, double width, double height, bool* data, int Nx, int Ny, QColor colTrue, JKQTPlotter* parent):
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
    QRgb tc=qRgba(round(trueColor.red()*trueColor.alphaF()), round(trueColor.green()*trueColor.alphaF()), round(trueColor.blue()*trueColor.alphaF()), trueColor.alpha());
    QRgb fc=qRgba(round(falseColor.red()*falseColor.alphaF()), round(falseColor.green()*falseColor.alphaF()), round(falseColor.blue()*falseColor.alphaF()), falseColor.alpha());

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
    int w=rect.width()/2;
    QRectF r1=QRectF(rect.topLeft(), QSizeF(w, rect.height()));
    QRectF r2=QRectF(QPoint(rect.left()+w, rect.top()), QSizeF(w, rect.height()));
    painter.save();
    painter.setPen(Qt::transparent);
    painter.setBrush(QBrush(trueColor));
    painter.fillRect(r1, QBrush(trueColor));
    painter.setBrush(QBrush(falseColor));
    painter.fillRect(r2, QBrush(falseColor));
    painter.restore();

}

QColor JKQTPOverlayImage::getKeyLabelColor() {
    QColor c=trueColor;
    c.setAlpha(255);
    return c;
}

void JKQTPOverlayImage::setData(bool* data, int Nx, int Ny) {
    this->data=data;
    this->Nx=Nx;
    this->Ny=Ny;
}


JKQTPOverlayImageEnhanced::JKQTPOverlayImageEnhanced(double x, double y, double width, double height, bool* data, int Nx, int Ny, QColor colTrue, JKQTBasePlotter* parent):
    JKQTPOverlayImage(x, y, width, height, data, Nx, Ny, colTrue, parent)
{
    symbol=JKQTPTarget;
    symbolWidth=1;
    drawAsRectangles=true;
    symbolSizeFactor=0.9;
    rectanglesAsImageOverlay=false;
}

JKQTPOverlayImageEnhanced::JKQTPOverlayImageEnhanced(JKQTBasePlotter *parent):
    JKQTPOverlayImage(0,0,1,1,nullptr,0,0, QColor("red"), parent)
{
    symbol=JKQTPTarget;
    symbolWidth=1;
    drawAsRectangles=true;
    symbolSizeFactor=0.9;
    rectanglesAsImageOverlay=false;
}

JKQTPOverlayImageEnhanced::JKQTPOverlayImageEnhanced(double x, double y, double width, double height, bool* data, int Nx, int Ny, QColor colTrue, JKQTPlotter* parent):
    JKQTPOverlayImage(x, y, width, height, data, Nx, Ny, colTrue, parent)
{
    symbol=JKQTPTarget;
    symbolWidth=1;
    drawAsRectangles=true;
    symbolSizeFactor=0.9;
    rectanglesAsImageOverlay=false;
}

JKQTPOverlayImageEnhanced::JKQTPOverlayImageEnhanced(JKQTPlotter *parent):
    JKQTPOverlayImage(0,0,1,1,nullptr,0,0, QColor("red"), parent)
{
    symbol=JKQTPTarget;
    symbolWidth=1;
    drawAsRectangles=true;
    symbolSizeFactor=0.9;
    rectanglesAsImageOverlay=false;
}
void JKQTPOverlayImageEnhanced::drawKeyMarker(JKQTPEnhancedPainter& painter, QRectF& rect) {
    if (drawAsRectangles) JKQTPOverlayImage::drawKeyMarker(painter, rect);
    else JKQTPPlotSymbol(painter, rect.center().x(), rect.center().y(), symbol, qMin(rect.width(), rect.height()), parent->pt2px(painter, symbolWidth*parent->getLineWidthMultiplier()), trueColor, trueColor.lighter());
}

void JKQTPOverlayImageEnhanced::draw(JKQTPEnhancedPainter& painter) {
    if (!data) return;

    if (drawAsRectangles && rectanglesAsImageOverlay) {
        JKQTPOverlayImage::draw(painter);
    } else {

        painter.save();

        double dx=width/static_cast<double>(Nx);
        double dy=height/static_cast<double>(Ny);
        for (int ix=0; ix<static_cast<int64_t>(Nx); ix++) {
            for (int iy=0; iy<static_cast<int64_t>(Ny); iy++) {
                QPointF p1=transform(x+static_cast<double>(ix)*dx, y+static_cast<double>(iy)*dy);
                QPointF p2=transform(x+static_cast<double>(ix+1)*dx, y+static_cast<double>(iy+1)*dx);
                if (drawAsRectangles) {
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
                } else {
                    QPointF p=(p1+p2)/2.0;
                    if (data[ix+iy*Nx]) {
                        JKQTPPlotSymbol(painter, p.x(), p.y(), symbol, fabs(p2.x()-p1.x())*symbolSizeFactor, parent->pt2px(painter, symbolWidth*parent->getLineWidthMultiplier()), trueColor, trueColor.lighter());
                    }
                }
            }
        }
        painter.restore();
    }
}


















void JKQTPRGBMathImage::initObject()
{
    actSaveImage=new QAction(tr("Save JKQTPRGBMathImage ..."), this);
    connect(actSaveImage, SIGNAL(triggered()), this, SLOT(saveImagePlotAsImage()));
    actCopyImage=new QAction(tr("Copy JKQTPOverlayImage ..."), this);
    connect(actCopyImage, SIGNAL(triggered()), this, SLOT(copyImagePlotAsImage()));

    rgbMode=JKQTPRGBMathImageModeRGBMode;
    colorBarRightAxis=new JKQTPVerticalIndependentAxis(0, 100, 0, 100, parent);
    if (parent) colorBarRightAxis->loadSettings(parent->getYAxis());
    colorBarRightAxis->setDrawMode1(JKQTPCADMline);
    colorBarRightAxis->setDrawMode2(JKQTPCADMcomplete);
    colorBarRightAxis->setAxisLabel("");
    colorBarRightAxis->setMinTicks(5);
    colorBarRightAxis->setMinorTicks(0);
    colorBarRightAxis->setTickOutsideLength(0);
    colorBarRightAxis->setMinorTickOutsideLength(0);
    colorBarRightAxis->setShowZeroAxis(false);
    colorBarTopAxis=new JKQTPHorizontalIndependentAxis(0, 100, 0, 100, parent);
    if (parent) colorBarTopAxis->loadSettings(parent->getXAxis());
    colorBarTopAxis->setDrawMode1(JKQTPCADMline);
    colorBarTopAxis->setDrawMode2(JKQTPCADMcomplete);
    colorBarTopAxis->setAxisLabel("");
    colorBarTopAxis->setMinTicks(3);
    colorBarTopAxis->setMinorTicks(0);
    colorBarTopAxis->setTickOutsideLength(0);
    colorBarTopAxis->setMinorTickOutsideLength(0);
    colorBarTopAxis->setShowZeroAxis(false);

    colorBarRightAxisG=new JKQTPVerticalIndependentAxis(0, 100, 0, 100, parent);
    if (parent) colorBarRightAxisG->loadSettings(parent->getYAxis());
    colorBarRightAxisG->setDrawMode1(JKQTPCADMline);
    colorBarRightAxisG->setDrawMode2(JKQTPCADMcomplete);
    colorBarRightAxisG->setAxisLabel("");
    colorBarRightAxisG->setMinTicks(5);
    colorBarRightAxisG->setShowZeroAxis(false);
    colorBarRightAxisG->setMinorTicks(0);
    colorBarRightAxisG->setTickOutsideLength(0);
    colorBarRightAxisG->setMinorTickOutsideLength(0);
    colorBarTopAxisG=new JKQTPHorizontalIndependentAxis(0, 100, 0, 100, parent);
    if (parent) colorBarTopAxisG->loadSettings(parent->getXAxis());
    colorBarTopAxisG->setDrawMode1(JKQTPCADMline);
    colorBarTopAxisG->setDrawMode2(JKQTPCADMcomplete);
    colorBarTopAxisG->setAxisLabel("");
    colorBarTopAxisG->setMinTicks(3);
    colorBarTopAxisG->setShowZeroAxis(false);
    colorBarTopAxisG->setMinorTicks(0);
    colorBarTopAxisG->setTickOutsideLength(0);
    colorBarTopAxisG->setMinorTickOutsideLength(0);

    colorBarRightAxisB=new JKQTPVerticalIndependentAxis(0, 100, 0, 100, parent);
    if (parent) colorBarRightAxisB->loadSettings(parent->getYAxis());
    colorBarRightAxisB->setDrawMode1(JKQTPCADMline);
    colorBarRightAxisB->setDrawMode2(JKQTPCADMcomplete);
    colorBarRightAxisB->setAxisLabel("");
    colorBarRightAxisB->setMinTicks(5);
    colorBarRightAxisB->setShowZeroAxis(false);
    colorBarRightAxisB->setMinorTicks(0);
    colorBarRightAxisB->setTickOutsideLength(0);
    colorBarRightAxisB->setMinorTickOutsideLength(0);
    colorBarTopAxisB=new JKQTPHorizontalIndependentAxis(0, 100, 0, 100, parent);
    if (parent) colorBarTopAxisB->loadSettings(parent->getXAxis());
    colorBarTopAxisB->setDrawMode1(JKQTPCADMline);
    colorBarTopAxisB->setDrawMode2(JKQTPCADMcomplete);
    colorBarTopAxisB->setAxisLabel("");
    colorBarTopAxisB->setMinTicks(3);
    colorBarTopAxisB->setShowZeroAxis(false);
    colorBarTopAxisB->setMinorTicks(0);
    colorBarTopAxisB->setTickOutsideLength(0);
    colorBarTopAxisB->setMinorTickOutsideLength(0);


    this->colorBarTopVisible=true;
    this->colorBarRightVisible=true;
    this->imageNameFontName=parent->getKeyFont();
    this->imageNameFontSize=parent->getKeyFontSize();
    this->imageName="";
    this->showColorBar=true;
    this->colorBarWidth=14;
    this->colorBarRelativeHeight=0.75;
    this->autoImageRange=true;
    this->imageMin=0;
    this->imageMax=1;
    this->imageMinG=0;
    this->imageMaxG=1;
    this->imageMinB=0;
    this->imageMaxB=1;
    this->colorBarOffset=4;
    this->colorBarTopVisible=false;
    this->colorBarRightVisible=true;
    this->colorbarsSideBySide=true;
}



JKQTPRGBMathImage::JKQTPRGBMathImage(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTBasePlotter *parent):
    JKQTPMathImageBase(x, y, width, height, datatype, data, Nx, Ny, parent)
{
    initObject();
}


JKQTPRGBMathImage::JKQTPRGBMathImage(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTPlotter *parent):
    JKQTPMathImageBase(x, y, width, height, datatype, data, Nx, Ny, parent)
{
    initObject();
}

JKQTPRGBMathImage::JKQTPRGBMathImage(JKQTBasePlotter *parent):
    JKQTPMathImageBase(0,0,0,0, DoubleArray, nullptr, 0, 0, parent)
{
    initObject();
}


JKQTPRGBMathImage::JKQTPRGBMathImage(JKQTPlotter *parent):
    JKQTPMathImageBase(0,0,0,0, DoubleArray, nullptr, 0, 0, parent)
{
    initObject();
}

void JKQTPRGBMathImage::setParent(JKQTBasePlotter* parent) {
    if (this->parent) {
        this->parent->deregisterAdditionalAction(actSaveImage);
        this->parent->deregisterAdditionalAction(actCopyImage);
    }
    JKQTPMathImageBase::setParent(parent);
    colorBarRightAxis->setParent(parent);
    colorBarTopAxis->setParent(parent);

    if (parent) {
        parent->registerAdditionalAction(tr("Save Image Plot Images ..."), actSaveImage);
        parent->registerAdditionalAction(tr("Copy Image Plot Images ..."), actCopyImage);
    }
    actSaveImage->setEnabled(parent);
    actCopyImage->setEnabled(parent);

}



void JKQTPRGBMathImage::draw(JKQTPEnhancedPainter& painter) {
    ensureImageData();
    if (!data && !dataG && !dataB) return;
    QImage img=drawImage();
    plotImage(painter, img, x, y, width, height);
}



void JKQTPRGBMathImage::getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace) {
    ensureImageData();
    JKQTPGraph::getOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
    if (showColorBar) {
        int visibleColorBars=0;
        if (data) visibleColorBars++;
        if (dataG) visibleColorBars++;
        if (dataB) visibleColorBars++;
        double sizeFactor=0.8/double(visibleColorBars);
        if (!colorbarsSideBySide) sizeFactor=1;

        bool firstC=false;

        getDataMinMax(internalDataMin, internalDataMax);
        if (data) {
            if (colorBarRightVisible) {
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
                colorBarRightAxis->setRange(internalDataMin, internalDataMax);
                colorBarRightAxis->setAxisWidth(sizeFactor*colorBarRelativeHeight*parent->getPlotHeight());
                colorBarRightAxisB->setAxisLabel(imageName);
                QSize s=colorBarRightAxis->getSize2(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+static_cast<double>(s.width());
                /*QSize s=colorBarRightAxis->getSize2(painter);
                parent->getMathText()->setFontSize(imageNameFontSize*parent->getFontSizeMultiplier());
                parent->getMathText()->setFontRoman(imageNameFontName);
                parent->getMathText()->parse(imageName);
                QSizeF names=parent->getMathText()->getSize(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=colorBarWidth+colorBarOffset+qMax(static_cast<double>(s.width()), static_cast<double>(names.width()));*/
            }
            if (colorBarTopVisible) {
                //if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
                colorBarTopAxis->setRange(internalDataMin, internalDataMax);
                colorBarTopAxis->setAxisWidth(sizeFactor*colorBarRelativeHeight*parent->getPlotWidth());
                colorBarTopAxisB->setAxisLabel(imageName);
                QSize s=colorBarTopAxisB->getSize2(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+static_cast<double>(s.height());
                /*QSize s=colorBarTopAxis->getSize2(painter);
                parent->getMathText()->setFontSize(imageNameFontSize*parent->getFontSizeMultiplier());
                parent->getMathText()->setFontRoman(imageNameFontName);
                parent->getMathText()->parse(imageName);
                QSizeF names=parent->getMathText()->getSize(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=colorBarWidth+colorBarOffset+qMax(static_cast<double>(s.height()), static_cast<double>(names.height()));*/
            }
            firstC=true;
        }

        getDataMinMaxG(internalDataMinG, internalDataMaxG);
        if (dataG) {
            if (colorBarRightVisible) {
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
                colorBarRightAxisG->setRange(internalDataMinG, internalDataMaxG);
                colorBarRightAxisG->setAxisWidth(sizeFactor*colorBarRelativeHeight*parent->getPlotHeight());
                colorBarRightAxisB->setAxisLabel(imageNameG);
                QSize s=colorBarRightAxis->getSize2(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+static_cast<double>(s.width());
                /*QSize s=colorBarRightAxis->getSize2(painter);
                parent->getMathText()->setFontSize(imageNameFontSize*parent->getFontSizeMultiplier());
                parent->getMathText()->setFontRoman(imageNameFontName);
                parent->getMathText()->parse(imageNameG);
                QSizeF names=parent->getMathText()->getSize(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=colorBarWidth+colorBarOffset+qMax(static_cast<double>(s.width()), static_cast<double>(names.width()));*/
            }
            if (colorBarTopVisible) {
                //if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
                colorBarTopAxisG->setRange(internalDataMinG, internalDataMaxG);
                colorBarTopAxisG->setAxisWidth(sizeFactor*colorBarRelativeHeight*parent->getPlotWidth());
                colorBarTopAxisB->setAxisLabel(imageNameG);
                QSize s=colorBarTopAxisB->getSize2(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+static_cast<double>(s.height());
                /*QSize s=colorBarTopAxisG->getSize2(painter);
                parent->getMathText()->setFontSize(imageNameFontSize*parent->getFontSizeMultiplier());
                parent->getMathText()->setFontRoman(imageNameFontName);
                parent->getMathText()->parse(imageNameG);
                QSizeF names=parent->getMathText()->getSize(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=colorBarWidth+colorBarOffset+qMax(static_cast<double>(s.height()), static_cast<double>(names.height()));*/
            }
            firstC=true;
        }

        getDataMinMaxB(internalDataMinB, internalDataMaxB);
        if (dataB) {
            if (colorBarRightVisible) {
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
                colorBarRightAxisB->setRange(internalDataMinB, internalDataMaxB);
                colorBarRightAxisB->setAxisWidth(sizeFactor*colorBarRelativeHeight*parent->getPlotHeight());
                colorBarRightAxisB->setAxisLabel(imageNameB);
                QSize s=colorBarRightAxis->getSize2(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+static_cast<double>(s.width());
                /*
                parent->getMathText()->setFontSize(imageNameFontSize*parent->getFontSizeMultiplier());
                parent->getMathText()->setFontRoman(imageNameFontName);
                parent->getMathText()->parse(imageNameB);
                QSizeF names=parent->getMathText()->getSize(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=colorBarWidth+colorBarOffset+qMax(static_cast<double>(s.width()), static_cast<double>(names.width()));*/
            }
            if (colorBarTopVisible) {
                //if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
                colorBarTopAxisB->setRange(internalDataMinB, internalDataMaxB);
                colorBarTopAxisB->setAxisWidth(sizeFactor*colorBarRelativeHeight*parent->getPlotWidth());
                colorBarTopAxisB->setAxisLabel(imageNameB);
                QSize s=colorBarTopAxisB->getSize2(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+static_cast<double>(s.height());
                /*parent->getMathText()->setFontSize(imageNameFontSize*parent->getFontSizeMultiplier());
                parent->getMathText()->setFontRoman(imageNameFontName);
                parent->getMathText()->parse(imageNameB);
                QSizeF names=parent->getMathText()->getSize(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=colorBarWidth+colorBarOffset+qMax(static_cast<double>(s.height()), static_cast<double>(names.height()));*/
            }
            firstC=true;
        }
    }
}

struct RGBOutsizeData {
    double internalDataMin;
    double internalDataMax;
    void* data;
    JKQTPVerticalIndependentAxis* colorBarRightAxis;
    JKQTPHorizontalIndependentAxis* colorBarTopAxis;
    QString name;
    JKQTPMathImageColorPalette palette;
    QImage paletteImage;
};

void JKQTPRGBMathImage::drawOutside(JKQTPEnhancedPainter& painter, QRect /*leftSpace*/, QRect rightSpace, QRect topSpace, QRect /*bottomSpace*/) {
    ensureImageData();
    if (showColorBar) {
        QList<RGBOutsizeData> l;
        int visibleColorBars=0;
        const int pd_size=200;
        uint8_t pd[pd_size];
        for (int i=0; i<pd_size; i++) pd[i]=i;

        if (data) {
            RGBOutsizeData d;
            d.data=data;
            getDataMinMax(internalDataMin, internalDataMax);
            d.internalDataMin=internalDataMin;
            d.internalDataMax=internalDataMax;
            d.colorBarRightAxis=colorBarRightAxis;
            d.colorBarTopAxis=colorBarTopAxis;
            d.name=imageName;
            if (rgbMode==JKQTPRGBMathImageModeRGBMode) {
                d.palette=JKQTPMathImageRED;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, pd_size-1);
            } else if (rgbMode==JKQTPRGBMathImageModeCMYMode) {
                d.palette=JKQTPMathImageINVERTED_CYANWHITE;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, pd_size-1);
            } else if (rgbMode==JKQTPRGBMathImageModeHSVMode || rgbMode==JKQTPRGBMathImageModeHSLMode) {
                d.palette=JKQTPMathImageHSV;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, pd_size-1);
            }

            l<<d;
            visibleColorBars++;
        }
        if (dataG) {
            RGBOutsizeData d;
            d.data=dataG;
            getDataMinMaxG(internalDataMinG, internalDataMaxG);
            d.internalDataMin=internalDataMinG;
            d.internalDataMax=internalDataMaxG;
            d.colorBarRightAxis=colorBarRightAxisG;
            d.colorBarTopAxis=colorBarTopAxisG;
            d.name=imageNameG;
            JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, 199);
            if (rgbMode==JKQTPRGBMathImageModeRGBMode) {
                d.palette=JKQTPMathImageGREEN;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, pd_size-1);
            } else if (rgbMode==JKQTPRGBMathImageModeCMYMode) {
                d.palette=JKQTPMathImageINVERTED_MAGENTAWHITE;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, pd_size-1);
            } else if (rgbMode==JKQTPRGBMathImageModeHSVMode) {
                d.palette=JKQTPMathImageGRAY;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                //JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, l[li].palette, 0, pd_size-1);
                QRgb* line=reinterpret_cast<QRgb *>(d.paletteImage.scanLine(0));
                for (int i=0; i<pd_size; i++) {
                    QColor c=QColor("red");
                    c.setHsv(c.hue(), double(i)/double(pd_size-1)*255.0, c.value(), c.alpha());
                    line[i]=c.rgba();
                }
            } else if (rgbMode==JKQTPRGBMathImageModeHSLMode) {
                d.palette=JKQTPMathImageGRAY;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                //JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, l[li].palette, 0, pd_size-1);
                QRgb* line=reinterpret_cast<QRgb *>(d.paletteImage.scanLine(0));
                for (int i=0; i<pd_size; i++) {
                    QColor c=QColor("red");
                    c.setHsl(c.hue(), double(i)/double(pd_size-1)*255.0, c.lightness(), c.alpha());
                    line[i]=c.rgba();
                }
            }

            l<<d;
            visibleColorBars++;
        }
        if (dataB) {
            RGBOutsizeData d;
            d.data=dataB;
            getDataMinMax(internalDataMinB, internalDataMaxB);
            d.internalDataMin=internalDataMinB;
            d.internalDataMax=internalDataMaxB;
            d.colorBarRightAxis=colorBarRightAxisB;
            d.colorBarTopAxis=colorBarTopAxisB;
            d.name=imageNameB;
            JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, 199);
            if (rgbMode==JKQTPRGBMathImageModeRGBMode) {
                d.palette=JKQTPMathImageBLUE;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, pd_size-1);
            } else if (rgbMode==JKQTPRGBMathImageModeCMYMode) {
                d.palette=JKQTPMathImageINVERTED_YELLOWWHITE;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, pd_size-1);
            } else if (rgbMode==JKQTPRGBMathImageModeHSVMode) {
                d.palette=JKQTPMathImageGRAY;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                //JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, l[li].palette, 0, pd_size-1);
                QRgb* line=reinterpret_cast<QRgb *>(d.paletteImage.scanLine(0));
                for (int i=0; i<pd_size; i++) {
                    QColor c=QColor("red");
                    c.setHsv(c.hue(), c.saturation(), double(i)/double(pd_size-1)*255.0, c.alpha());
                    line[i]=c.rgba();
                }
            } else if (rgbMode==JKQTPRGBMathImageModeHSLMode) {
                d.palette=JKQTPMathImageGRAY;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                //JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, l[li].palette, 0, pd_size-1);
                QRgb* line=reinterpret_cast<QRgb *>(d.paletteImage.scanLine(0));
                for (int i=0; i<pd_size; i++) {
                    QColor c=QColor("red");
                    c.setHsl(c.hue(), c.saturation(), double(i)/double(pd_size-1)*255.0, c.alpha());
                    line[i]=c.rgba();
                }
            }
            l<<d;
            visibleColorBars++;
        }


        double sizeFactor=0.9/double(visibleColorBars);
        double shiftSizeFactor=1.0/double(visibleColorBars);
        if (!colorbarsSideBySide) {
            sizeFactor=1;
            shiftSizeFactor=1;
        }
        double icolorBarRelativeHeight=colorBarRelativeHeight*sizeFactor;

        int gbarHeight=qMax(1, static_cast<int>(round(static_cast<double>(rightSpace.height())*icolorBarRelativeHeight)));
        int gbarWidth=qMax(1, static_cast<int>(round(static_cast<double>(topSpace.width())*icolorBarRelativeHeight)));



        double rX=rightSpace.x()+parent->pt2px(painter, colorBarOffset);
        double rY=rightSpace.top()+(rightSpace.height()-gbarHeight)/2;
        double tX=topSpace.x()+(topSpace.width()-gbarWidth)/2;
        double tY=topSpace.bottom()-parent->pt2px(painter, colorBarOffset+colorBarWidth);
        if(colorbarsSideBySide) {
            rY=rightSpace.top()+(rightSpace.height()-gbarHeight*visibleColorBars)/2;
            tX=topSpace.x()+(topSpace.width()-gbarWidth*visibleColorBars)/2;
        }


        for (int li=0; li<l.size(); li++) {
            if (colorBarRightVisible) {
                painter.save();


                QRect cb(rX, rY, parent->pt2px(painter, colorBarWidth), gbarHeight);
                painter.drawImage(cb, l[li].paletteImage.mirrored(true, false));
                QPen p=painter.pen();
                p.setColor(l[li].colorBarRightAxis->getAxisColor());
                p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, l[li].colorBarRightAxis->getLineWidth()*parent->getLineWidthMultiplier())));
                painter.setPen(p);
                painter.drawRect(cb);

                l[li].colorBarRightAxis->setRange(l[li].internalDataMin, l[li].internalDataMax);
                l[li].colorBarRightAxis->setAxisWidth(cb.height());
                l[li].colorBarRightAxis->setAxisOffset(cb.top());
                l[li].colorBarRightAxis->setOtherAxisOffset(cb.left());
                l[li].colorBarRightAxis->setOtherAxisWidth(cb.width());
                l[li].colorBarRightAxis->setLabelFontSize(imageNameFontSize*parent->getFontSizeMultiplier());
                l[li].colorBarRightAxis->setLabelFont(imageNameFontName);
                l[li].colorBarRightAxis->setAxisLabel(l[li].name);
                l[li].colorBarRightAxis->drawAxes(painter);

                painter.restore();

                if (!colorbarsSideBySide) {
                    rX=rX+static_cast<double>(rightSpace.width())/static_cast<double>(visibleColorBars);
                } else {
                    rY=rY+static_cast<double>(rightSpace.height())*shiftSizeFactor*colorBarRelativeHeight;
                }
            }
            if (colorBarTopVisible) {
                painter.save();


                QRect cb(tX, tY, gbarWidth, parent->pt2px(painter, colorBarWidth));
                QMatrix mt;
                mt.rotate(90);
                painter.drawImage(cb, l[li].paletteImage.transformed(mt));
                QPen p=painter.pen();
                p.setColor(l[li].colorBarTopAxis->getAxisColor());
                p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, l[li].colorBarTopAxis->getLineWidth()*parent->getLineWidthMultiplier())));
                painter.setPen(p);
                painter.drawRect(cb);


                l[li].colorBarTopAxis->setRange(l[li].internalDataMin, l[li].internalDataMax);
                l[li].colorBarTopAxis->setAxisWidth(cb.width());
                l[li].colorBarTopAxis->setAxisOffset(cb.left());
                l[li].colorBarTopAxis->setOtherAxisOffset(cb.top());
                l[li].colorBarTopAxis->setOtherAxisWidth(cb.height());
                l[li].colorBarTopAxis->setLabelFontSize(imageNameFontSize*parent->getFontSizeMultiplier());
                l[li].colorBarTopAxis->setLabelFont(imageNameFontName);
                l[li].colorBarTopAxis->setAxisLabel(l[li].name);
                l[li].colorBarTopAxis->drawAxes(painter);

                painter.restore();
                if (!colorbarsSideBySide) {
                    tY=tY-topSpace.height()/double(visibleColorBars);
                } else {
                    tX=tX+topSpace.width()*shiftSizeFactor*colorBarRelativeHeight;
                }
            }
        }
    }

}

void JKQTPRGBMathImage::getDataMinMax(double& imin, double& imax) {
    ensureImageData();
    if (autoImageRange) {
        JKQTPMathImageBase::getDataMinMax(imin, imax);
    } else {
        imin=imageMin;
        imax=imageMax;
    }
}

void JKQTPRGBMathImage::getDataMinMaxG(double& imin, double& imax) {
    ensureImageData();
    if (autoImageRange) {
        imin=imax=0;
        if (!dataG) return;
        switch(datatype) {
            case JKQTPMathImageBase::DoubleArray:
                imin= JKQTPImagePlot_getImageMin<double>(static_cast<double*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<double>(static_cast<double*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::FloatArray:
                imin= JKQTPImagePlot_getImageMin<float>(static_cast<float*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<float>(static_cast<float*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt8Array:
                imin= JKQTPImagePlot_getImageMin<uint8_t>(static_cast<uint8_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint8_t>(static_cast<uint8_t*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt16Array:
                imin= JKQTPImagePlot_getImageMin<uint16_t>(static_cast<uint16_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint16_t>(static_cast<uint16_t*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt32Array:
                imin= JKQTPImagePlot_getImageMin<uint32_t>(static_cast<uint32_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint32_t>(static_cast<uint32_t*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt64Array:
                imin= JKQTPImagePlot_getImageMin<uint64_t>(static_cast<uint64_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint64_t>(static_cast<uint64_t*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int8Array:
                imin= JKQTPImagePlot_getImageMin<int8_t>(static_cast<int8_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int8_t>(static_cast<int8_t*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int16Array:
                imin= JKQTPImagePlot_getImageMin<int16_t>(static_cast<int16_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int16_t>(static_cast<int16_t*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int32Array:
                imin= JKQTPImagePlot_getImageMin<int32_t>(static_cast<int32_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int32_t>(static_cast<int32_t*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int64Array:
                imin= JKQTPImagePlot_getImageMin<int64_t>(static_cast<int64_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int64_t>(static_cast<int64_t*>(dataG), Nx, Ny);
                break;
        }
    } else {
        imin=imageMinG;
        imax=imageMaxG;
    }
}

void JKQTPRGBMathImage::getDataMinMaxB(double& imin, double& imax) {
    ensureImageData();
    if (autoImageRange) {
        imin=imax=0;
        if (!dataG) return;
        switch(datatype) {
            case JKQTPMathImageBase::DoubleArray:
                imin= JKQTPImagePlot_getImageMin<double>(static_cast<double*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<double>(static_cast<double*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::FloatArray:
                imin= JKQTPImagePlot_getImageMin<float>(static_cast<float*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<float>(static_cast<float*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt8Array:
                imin= JKQTPImagePlot_getImageMin<uint8_t>(static_cast<uint8_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint8_t>(static_cast<uint8_t*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt16Array:
                imin= JKQTPImagePlot_getImageMin<uint16_t>(static_cast<uint16_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint16_t>(static_cast<uint16_t*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt32Array:
                imin= JKQTPImagePlot_getImageMin<uint32_t>(static_cast<uint32_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint32_t>(static_cast<uint32_t*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt64Array:
                imin= JKQTPImagePlot_getImageMin<uint64_t>(static_cast<uint64_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint64_t>(static_cast<uint64_t*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int8Array:
                imin= JKQTPImagePlot_getImageMin<int8_t>(static_cast<int8_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int8_t>(static_cast<int8_t*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int16Array:
                imin= JKQTPImagePlot_getImageMin<int16_t>(static_cast<int16_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int16_t>(static_cast<int16_t*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int32Array:
                imin= JKQTPImagePlot_getImageMin<int32_t>(static_cast<int32_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int32_t>(static_cast<int32_t*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int64Array:
                imin= JKQTPImagePlot_getImageMin<int64_t>(static_cast<int64_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int64_t>(static_cast<int64_t*>(dataB), Nx, Ny);
                break;
        }
    } else {
        imin=imageMinB;
        imax=imageMaxB;
    }
}

double JKQTPRGBMathImage::getValueAt(double x, double y, int channel)
{
    ensureImageData();
    void* dd=data;
    if (channel==0) dd=data;
    if (channel==1) dd=dataG;
    if (channel==2) dd=dataB;
    int xx=static_cast<int>(trunc((x-this->x)/width*Nx));
    int yy=static_cast<int>(trunc((y-this->y)/height*Ny));
    if (xx>=0 && xx<Nx && yy>=0 && yy<Ny) {
        switch(datatype) {
            case JKQTPMathImageBase::DoubleArray: return static_cast<double>((static_cast<double*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::FloatArray: return static_cast<double>((static_cast<float*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::UInt8Array: return static_cast<double>((static_cast<uint8_t*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::UInt16Array: return static_cast<double>((static_cast<uint16_t*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::UInt32Array: return static_cast<double>((static_cast<uint32_t*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::UInt64Array: return static_cast<double>((static_cast<uint64_t*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::Int8Array: return static_cast<double>((static_cast<int8_t*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::Int16Array: return static_cast<double>((static_cast<int16_t*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::Int32Array: return static_cast<double>((static_cast<int32_t*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::Int64Array: return static_cast<double>((static_cast<int64_t*>(dd))[yy*Nx+xx]); break;
        }    }
    return 0.0;
}

void JKQTPRGBMathImage::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    painter.drawImage(rect, QPixmap(":/JKQTPlotter/jkqtp_plot_rgbimage.png").toImage());
}

void JKQTPRGBMathImage::setTitle(const QString &title)
{
    JKQTPImageBase::setTitle(title);
    QString t=title;
    if (t.isEmpty()) t="JKQTPRGBMathImage";
    actSaveImage->setText(tr("Save %1 ...").arg(t));
    actCopyImage->setText(tr("Copy %1 ...").arg(t));
}

void JKQTPRGBMathImage::saveImagePlotAsImage(const QString &filename, const QByteArray &outputFormat)
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

void JKQTPRGBMathImage::copyImagePlotAsImage()
{
    QClipboard* clip=QApplication::clipboard();
    if (clip) {
        clip->setPixmap(QPixmap::fromImage(drawImage()));
    }
}

QImage JKQTPRGBMathImage::drawImage() {
    ensureImageData();
    if (!data && !dataG && !dataB) return QImage();
    QImage img(Nx, Ny, QImage::Format_ARGB32);

    if (rgbMode==JKQTPRGBMathImageModeRGBMode) {
        img.fill(Qt::transparent);
    } else if (rgbMode==JKQTPRGBMathImageModeCMYMode) {
        img.fill(Qt::white);
    } else if (rgbMode==JKQTPRGBMathImageModeHSVMode) {
        QColor c("white");
        c.setHsv(0,255,255);
        img.fill(c.rgba());
    } else if (rgbMode==JKQTPRGBMathImageModeHSLMode) {
        QColor c("white");
        c.setHsv(0,255,127);
        img.fill(c.rgba());
    }
    getDataMinMax(internalDataMin, internalDataMax);
    getDataMinMaxG(internalDataMinG, internalDataMaxG);
    getDataMinMaxB(internalDataMinB, internalDataMaxB);
    int palette = 0;
    if (data) {
        switch(datatype) {
            case JKQTPMathImageBase::DoubleArray: JKQTPImagePlot_array2RGBimage<double>(static_cast<double*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::FloatArray: JKQTPImagePlot_array2RGBimage<float>(static_cast<float*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::UInt8Array: JKQTPImagePlot_array2RGBimage<uint8_t>(static_cast<uint8_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::UInt16Array: JKQTPImagePlot_array2RGBimage<uint16_t>(static_cast<uint16_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::UInt32Array: JKQTPImagePlot_array2RGBimage<uint32_t>(static_cast<uint32_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::UInt64Array: JKQTPImagePlot_array2RGBimage<uint64_t>(static_cast<uint64_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::Int8Array: JKQTPImagePlot_array2RGBimage<int8_t>(static_cast<int8_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::Int16Array: JKQTPImagePlot_array2RGBimage<int16_t>(static_cast<int16_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::Int32Array: JKQTPImagePlot_array2RGBimage<int32_t>(static_cast<int32_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::Int64Array: JKQTPImagePlot_array2RGBimage<int64_t>(static_cast<int64_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
        }
    }
    palette = 1;
    if (dataG) {
        switch(datatypeG) {
            case JKQTPMathImageBase::DoubleArray: JKQTPImagePlot_array2RGBimage<double>(static_cast<double*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::FloatArray: JKQTPImagePlot_array2RGBimage<float>(static_cast<float*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::UInt8Array: JKQTPImagePlot_array2RGBimage<uint8_t>(static_cast<uint8_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::UInt16Array: JKQTPImagePlot_array2RGBimage<uint16_t>(static_cast<uint16_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::UInt32Array: JKQTPImagePlot_array2RGBimage<uint32_t>(static_cast<uint32_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::UInt64Array: JKQTPImagePlot_array2RGBimage<uint64_t>(static_cast<uint64_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::Int8Array: JKQTPImagePlot_array2RGBimage<int8_t>(static_cast<int8_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::Int16Array: JKQTPImagePlot_array2RGBimage<int16_t>(static_cast<int16_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::Int32Array: JKQTPImagePlot_array2RGBimage<int32_t>(static_cast<int32_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::Int64Array: JKQTPImagePlot_array2RGBimage<int64_t>(static_cast<int64_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
        }
    }
    palette = 2;
    if (dataB) {
        switch(datatypeB) {
            case JKQTPMathImageBase::DoubleArray: JKQTPImagePlot_array2RGBimage<double>(static_cast<double*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::FloatArray: JKQTPImagePlot_array2RGBimage<float>(static_cast<float*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::UInt8Array: JKQTPImagePlot_array2RGBimage<uint8_t>(static_cast<uint8_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::UInt16Array: JKQTPImagePlot_array2RGBimage<uint16_t>(static_cast<uint16_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::UInt32Array: JKQTPImagePlot_array2RGBimage<uint32_t>(static_cast<uint32_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::UInt64Array: JKQTPImagePlot_array2RGBimage<uint64_t>(static_cast<uint64_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::Int8Array: JKQTPImagePlot_array2RGBimage<int8_t>(static_cast<int8_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::Int16Array: JKQTPImagePlot_array2RGBimage<int16_t>(static_cast<int16_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::Int32Array: JKQTPImagePlot_array2RGBimage<int32_t>(static_cast<int32_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::Int64Array: JKQTPImagePlot_array2RGBimage<int64_t>(static_cast<int64_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
        }
    }

    modifyImage(img);

    return img;
}

void JKQTPRGBMathImage::setData(void* data, int Nx, int Ny, DataType datatype) {
    this->data=data;
    this->datatype=datatype;
    this->dataG=nullptr;
    this->dataB=nullptr;
    this->Nx=Nx;
    this->Ny=Ny;
}

void JKQTPRGBMathImage::setData(void* data, int Nx, int Ny) {
    this->data=data;
    this->Nx=Nx;
    this->Ny=Ny;
    this->dataG=nullptr;
    this->dataB=nullptr;
}


void JKQTPRGBMathImage::setData(void *data, void *dataG, void *dataB, int Nx, int Ny, JKQTPMathImageBase::DataType datatype) {
    this->data=data;
    this->datatype=datatype;
    this->datatypeG=datatype;
    this->datatypeB=datatype;
    this->dataG=dataG;
    this->dataB=dataB;
    this->Nx=Nx;
    this->Ny=Ny;
}

void JKQTPRGBMathImage::setData(void *data, void *dataG, void *dataB, int Nx, int Ny) {
    this->data=data;
    this->dataG=dataG;
    this->dataB=dataB;
    this->Nx=Nx;
    this->Ny=Ny;
}



JKQTPColumnMathImage::JKQTPColumnMathImage(JKQTBasePlotter *parent):
    JKQTPMathImage(parent)
{
    this->modifierColumn=-1;
    this->imageColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnMathImage::JKQTPColumnMathImage(double x, double y, double width, double height, int Nx, int Ny, JKQTBasePlotter *parent):
    JKQTPMathImage(x,y,width,height,JKQTPMathImageBase::DoubleArray,nullptr,Nx,Ny,JKQTPMathImageGRAY,parent)
{
    this->modifierColumn=-1;
    this->imageColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnMathImage::JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, int Nx, int Ny, JKQTPMathImageColorPalette palette, JKQTBasePlotter *parent):
    JKQTPMathImage(x,y,width,height,JKQTPMathImageBase::DoubleArray,nullptr,Nx,Ny,palette,parent)
{
    this->modifierColumn=-1;
    this->imageColumn=imageColumn;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnMathImage::JKQTPColumnMathImage(JKQTPlotter *parent):
    JKQTPMathImage(parent)
{
    this->modifierColumn=-1;
    this->imageColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnMathImage::JKQTPColumnMathImage(double x, double y, double width, double height, int Nx, int Ny, JKQTPlotter *parent):
    JKQTPMathImage(x,y,width,height,JKQTPMathImageBase::DoubleArray,nullptr,Nx,Ny,JKQTPMathImageGRAY,parent)
{
    this->modifierColumn=-1;
    this->imageColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnMathImage::JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, int Nx, int Ny, JKQTPMathImageColorPalette palette, JKQTPlotter *parent):
    JKQTPMathImage(x,y,width,height,JKQTPMathImageBase::DoubleArray,nullptr,Nx,Ny,palette,parent)
{
    this->modifierColumn=-1;
    this->imageColumn=imageColumn;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}
JKQTPColumnMathImage::JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, int Nx, int Ny, JKQTPlotter *parent):
    JKQTPMathImage(x,y,width,height,JKQTPMathImageBase::DoubleArray,nullptr,Nx,Ny,JKQTPMathImageGRAY,parent)
{
    this->modifierColumn=-1;
    this->imageColumn=imageColumn;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}
bool JKQTPColumnMathImage::usesColumn(int c) const
{
    return (c==imageColumn)||(c==modifierColumn);
}


void JKQTPColumnMathImage::ensureImageData()
{

    if (this->Nx==0 || imageColumn<0 || !parent->getDatastore()->getColumn(imageColumn).getPointer(0)) {
        this->Ny=0;
        this->data=nullptr;
        this->datatype=JKQTPMathImageBase::DoubleArray;
    } else {
        this->datatype=JKQTPMathImageBase::DoubleArray;
        this->data=parent->getDatastore()->getColumn(imageColumn).getPointer(0);
        this->Ny=parent->getDatastore()->getColumn(imageColumn).getRows()/this->Nx;
    }
    if (this->Nx==0 || modifierColumn<0 || !parent->getDatastore()->getColumn(modifierColumn).getPointer(0)) {
        this->dataModifier=nullptr;
    } else {
        this->datatypeModifier=JKQTPMathImageBase::DoubleArray;
        this->dataModifier=parent->getDatastore()->getColumn(modifierColumn).getPointer(0);
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
void JKQTPColumnOverlayImageEnhanced::draw(JKQTPEnhancedPainter &painter) {
    double* d=parent->getDatastore()->getColumn(imageColumn).getPointer(0);
    int imgSize=parent->getDatastore()->getColumn(imageColumn).getRows();
    this->data=(bool*)malloc(imgSize*sizeof(bool));
    this->Ny=imgSize/this->Nx;
    for (int i=0; i<imgSize; i++) {
        data[i]=d[i];
    }
    JKQTPOverlayImageEnhanced::draw(painter);
    free(data);
    data=nullptr;
}

bool JKQTPColumnOverlayImageEnhanced::usesColumn(int c) const
{
    return (c==imageColumn);
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(JKQTBasePlotter *parent):
    JKQTPRGBMathImage(0,0,0,0,DoubleArray,nullptr,0,0,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=-1;
    this->imageGColumn=-1;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int Nx, int Ny, JKQTBasePlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=-1;
    this->imageGColumn=-1;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int Nx, int Ny, JKQTBasePlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=imageRColumn;
    this->imageGColumn=-1;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int Nx, int Ny, JKQTBasePlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=imageRColumn;
    this->imageGColumn=imageGColumn;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int imageBColumn, int Nx, int Ny, JKQTBasePlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=imageRColumn;
    this->imageGColumn=imageGColumn;
    this->imageBColumn=imageBColumn;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(JKQTPlotter *parent):
    JKQTPRGBMathImage(0,0,0,0,DoubleArray,nullptr,0,0,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=-1;
    this->imageGColumn=-1;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int Nx, int Ny, JKQTPlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=-1;
    this->imageGColumn=-1;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int Nx, int Ny, JKQTPlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=imageRColumn;
    this->imageGColumn=-1;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int Nx, int Ny, JKQTPlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=imageRColumn;
    this->imageGColumn=imageGColumn;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int imageBColumn, int Nx, int Ny, JKQTPlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=imageRColumn;
    this->imageGColumn=imageGColumn;
    this->imageBColumn=imageBColumn;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}
bool JKQTPColumnRGBMathImage::usesColumn(int c) const
{
    return (c==imageRColumn)||(c==imageBColumn)||(c==imageGColumn)||(c==modifierColumn);
}


void JKQTPColumnRGBMathImage::ensureImageData()
{
    this->datatype=JKQTPMathImageBase::DoubleArray;
    this->datatypeG=JKQTPMathImageBase::DoubleArray;
    this->datatypeB=JKQTPMathImageBase::DoubleArray;
    this->data=parent->getDatastore()->getColumn(imageRColumn).getPointer(0);
    this->dataG=parent->getDatastore()->getColumn(imageGColumn).getPointer(0);
    this->dataB=parent->getDatastore()->getColumn(imageBColumn).getPointer(0);
    this->Ny=parent->getDatastore()->getColumn(imageRColumn).getRows()/this->Nx;
    this->datatypeModifier=JKQTPMathImageBase::DoubleArray;
    this->dataModifier=parent->getDatastore()->getColumn(modifierColumn).getPointer(0);
}


QVector<double> JKQTPRGBMathImage::getDataGAsDoubleVector() const
{
    switch(datatype) {
        case JKQTPMathImageBase::DoubleArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<double*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::FloatArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<float*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint8_t*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint16_t*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint32_t*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint64_t*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int8_t*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int16_t*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int32_t*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int64_t*>(dataG), Nx*Ny);
            break;
    }
    QVector<double> res;
    return res;
}

QVector<double> JKQTPRGBMathImage::getDataBAsDoubleVector() const
{
    switch(datatype) {
        case JKQTPMathImageBase::DoubleArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<double*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::FloatArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<float*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint8_t*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint16_t*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint32_t*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint64_t*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int8_t*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int16_t*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int32_t*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int64_t*>(dataB), Nx*Ny);
            break;
    }
    QVector<double> res;
    return res;
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

# include <QVector3D>
void JKQTPContour::draw(JKQTPEnhancedPainter &painter)
{
    //qDebug()<<"JKQTPContourPlot::draw";

    if(contourLevels.isEmpty()) createContourLevels(numberOfLevels);
    else {
        numberOfLevels=contourLevels.size();
//        qSort(contourLevels);
    }

    if(contourLines.isEmpty()) { // contour lines are only calculated once
        for(int i =0; i<contourLevels.size();++i) {
            contourLines.append(QVector<QLineF> (0));
        }
        this->calcContourLines(contourLines);
    }



    // draw lines
    painter.save();
    QPen p;
    p.setStyle(style);
    p.setColor(lineColor);
    p.setWidthF(qMax(JKQTPLOTTER_ABS_MIN_LINEWIDTH, parent->pt2px(painter, lineWidth*parent->getLineWidthMultiplier())));

    painter.setPen(p);
    QImage colorLevels = getPaletteImage(palette,numberOfLevels);
    QVector<QLineF> contourLinesTransformedSingleLevel;
    QLineF lineTranformed;
    for(int i =0; i<numberOfLevels;++i) {
        if(colorFromPalette) {
            p.setColor(QColor(colorLevels.pixel(i,0)));
            painter.setPen(p);
        }
        // transform into plot coordinates
        for(QVector<QLineF >::const_iterator line =contourLines.at(i).begin(); line!=contourLines.at(i).end();++line ) {
            lineTranformed.setP1(transform(x+line->p1().x()/double(Nx-1)*width, y+line->p1().y()/double(Ny-1)*height));
            lineTranformed.setP2(transform(x+line->p2().x()/double(Nx-1)*width, y+line->p2().y()/double(Ny-1)*height));
            contourLinesTransformedSingleLevel.append(lineTranformed);
        }
        painter.drawLines(contourLinesTransformedSingleLevel);
        contourLinesTransformedSingleLevel.clear();
    }

    painter.restore();
}

void JKQTPContour::createContourLevels(int nLevels)
{
    if (!data) return;
    if (nLevels<1) return;
    double min,max;
    getDataMinMax(min,max);
    double delta=(max-min)/static_cast<double>(nLevels+1);

    for(int i=1; i<=nLevels; ++i) {
        contourLevels.append(min + i*delta);
    }
    relativeLevels=false;

}

void JKQTPContour::createContourLevelsLog(int nLevels, int m)
{
    if (!data) return;
    if (nLevels<1) return;
    double min,max;
    getDataMinMax(min,max);
    if(min<=0) min=1; // FIXME get smallest number greater zero

    int S=floor((log10(max)-log10(min))/log10(m));
    if(S<2) S=1;
    int P = floor(static_cast<double>(nLevels)/static_cast<double>(S));
    if(P<1) P=1;

    double delta=min;
    contourLevels.append(2*delta);
    for (long s=0; s<S; s++) {
        for (long p=0; p<P; p++) {
            {
                contourLevels.append(contourLevels.last()+delta);
            }
        }
        delta=delta*m;
    }

    if(nLevels!=contourLevels.size()) {
        //qDebug()<<"nLevels="<<nLevels<<"contourLevels.size()="<<contourLevels.size();
        //qDebug()<<"adapt m";
    }

    relativeLevels=false;
}

void JKQTPContour::setImageColumn(size_t columnID)
{
    datatype=JKQTPMathImageBase::DoubleArray;
    data=parent->getDatastore()->getColumn(columnID).getPointer(0);
}


void JKQTPContour::ensureImageData()
{
}

double JKQTPContour::value(int xIdx, int yIdx)
{
    // row-major in datastore
    ensureImageData();
    if (!data) return 0;
    switch(datatype) {
        case JKQTPMathImageBase::DoubleArray:
            return (static_cast<double*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::FloatArray:
            return (static_cast<float*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt8Array:
            return (static_cast<uint8_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt16Array:
            return (static_cast<uint16_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt32Array:
            return (static_cast<uint32_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt64Array:
            return (static_cast<uint64_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int8Array:
            return (static_cast<int8_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int16Array:
            return (static_cast<int16_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int32Array:
            return (static_cast<int32_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int64Array:
            return (static_cast<int64_t*>(data))[yIdx*getNx()+xIdx];
    default:
        return 0;
    }
}

bool JKQTPContour::intersect(QLineF &line, const QVector3D &vertex1,const QVector3D &vertex2,const QVector3D &vertex3,double level)
{
    bool found = true;

    // Are the vertices below (-1), on (0) or above (1) the plane ?
    const int eq1 = compare2level(vertex1,level);
    const int eq2 = compare2level(vertex2,level);
    const int eq3 = compare2level(vertex3,level);

    /*
        (a) All the vertices lie below the contour level.
        (b) Two vertices lie below and one on the contour level.
        (c) Two vertices lie below and one above the contour level.
        (d) One vertex lies below and two on the contour level.
        (e) One vertex lies below, one on and one above the contour level.
        (f) One vertex lies below and two above the contour level.
        (g) Three vertices lie on the contour level.
        (h) Two vertices lie on and one above the contour level.
        (i) One vertex lies on and two above the contour level.
        (j) All the vertices lie above the contour level.
     */

    static const int caseLUT[3][3][3] =
    {
        // jump table to avoid nested case statements
        { { 0, 0, 8 }, { 0, 2, 5 }, { 7, 6, 9 } },
        { { 0, 3, 4 }, { 1, 10, 1 }, { 4, 3, 0 } },
        { { 9, 6, 7 }, { 5, 2, 0 }, { 8, 0, 0 } }
    };

    const int caseType = caseLUT[eq1+1][eq2+1][eq3+1];
    switch (caseType)
    {
        case 1:
            // d(0,0,-1), h(0,0,1)
            line.setP1(vertex1.toPointF());
            line.setP2(vertex2.toPointF());
            break;
        case 2:
            // d(-1,0,0), h(1,0,0)
            line.setP1(vertex2.toPointF());
            line.setP2(vertex3.toPointF());
            break;
        case 3:
            // d(0,-1,0), h(0,1,0)
            line.setP1(vertex3.toPointF());
            line.setP2(vertex1.toPointF());
            break;
        case 4:
            // e(0,-1,1), e(0,1,-1)
            line.setP1(vertex1.toPointF());
            line.setP2(interpolatePoint(vertex2, vertex3, level));
            break;
        case 5:
            // e(-1,0,1), e(1,0,-1)
            line.setP1(vertex2.toPointF());
            line.setP2(interpolatePoint(vertex3, vertex1, level));
            break;
        case 6:
            // e(-1,1,0), e(1,0,-1)
            line.setP1(vertex3.toPointF());
            line.setP2(interpolatePoint(vertex1, vertex2, level));
            break;
        case 7:
            // c(-1,1,-1), f(1,1,-1)
            line.setP1(interpolatePoint(vertex1, vertex2, level));
            line.setP2(interpolatePoint(vertex2, vertex3, level));
            break;
        case 8:
            // c(-1,-1,1), f(1,1,-1)
            line.setP1(interpolatePoint(vertex2, vertex3, level));
            line.setP2(interpolatePoint(vertex3, vertex1, level));
            break;
        case 9:
            // f(-1,1,1), c(1,-1,-1)
            line.setP1(interpolatePoint(vertex3, vertex1, level));
            line.setP2(interpolatePoint(vertex1, vertex2, level));
            break;
        case 10:
            // g(0,0,0)
            // The CONREC algorithm has no satisfying solution for
            // what to do, when all vertices are on the plane.

            if ( ignoreOnPlane )
                found = false;
            else
            {
                line.setP1(vertex3.toPointF());
                line.setP2(vertex1.toPointF());
            }
            break;
        default:
            found = false;
    }
//    qDebug()<<caseType;
//    qDebug()<<line;
    return found;
}


JKQTPContour::JKQTPContour(JKQTBasePlotter *parent) :
    JKQTPMathImage(parent)
{
    lineColor=QColor("red");
    colorBarRightVisible=false;
    lineWidth=1;
    style=Qt::SolidLine;
    ignoreOnPlane=false;
    numberOfLevels=1;
    colorFromPalette=true;
    datatype=JKQTPMathImageBase::DoubleArray;
    relativeLevels=false;

    if (parent) { // get style settings from parent object
        lineColor=parent->getPlotStyle(parent->getNextStyle()).color();
    }
}

JKQTPContour::JKQTPContour(double x, double y, double width, double height, void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette, DataType datatype, JKQTBasePlotter* parent) :
    JKQTPMathImage( x, y, width, height, datatype, data, Nx, Ny, palette, parent)
{
    lineColor=QColor("red");
    colorBarRightVisible=false;
    lineWidth=1;
    style=Qt::SolidLine;
    ignoreOnPlane=false;
    numberOfLevels=1;
    colorFromPalette=true;
    relativeLevels=false;
}


JKQTPContour::JKQTPContour(JKQTPlotter *parent) :
    JKQTPMathImage(parent)
{
    lineColor=QColor("red");
    colorBarRightVisible=false;
    lineWidth=1;
    style=Qt::SolidLine;
    ignoreOnPlane=false;
    numberOfLevels=1;
    colorFromPalette=true;
    datatype=JKQTPMathImageBase::DoubleArray;
    relativeLevels=false;

    if (parent) { // get style settings from parent object
        lineColor=parent->getPlotStyle(parent->getNextStyle()).color();
    }
}

JKQTPContour::JKQTPContour(double x, double y, double width, double height, void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette, DataType datatype, JKQTPlotter* parent) :
    JKQTPMathImage( x, y, width, height, datatype, data, Nx, Ny, palette, parent)
{
    lineColor=QColor("red");
    colorBarRightVisible=false;
    lineWidth=1;
    style=Qt::SolidLine;
    ignoreOnPlane=false;
    numberOfLevels=1;
    colorFromPalette=true;
    relativeLevels=false;
}
int JKQTPContour::compare2level(const QVector3D &vertex, double level)
{
    if (vertex.z() > level)
        return 1;

    if (vertex.z() < level)
        return -1;

    return 0;
}

void JKQTPContour::calcContourLines(QList<QVector<QLineF> > &ContourLines)
{

    double scale=1; ///< scale of the contour levels;

    if(relativeLevels) {
        double min;
        double max;
        getDataMinMax(min,max);
        scale=1/(max-min);
    }

    enum Position
    {
        // the positions of points of one box
        //      vertex 1 +-------------------+ vertex 2
        //               | \               / |
        //               |   \    m=3    /   |
        //               |     \       /     |
        //               |       \   /       |
        //               |  m=2    X   m=2   |       the center is vertex 0
        //               |       /   \       |
        //               |     /       \     |
        //               |   /    m=1    \   |
        //               | /               \ |
        //      vertex 4 +-------------------+ vertex 3
        Center=0,

        TopLeft=1,
        TopRight=2,
        BottomRight=3,
        BottomLeft=4,

        NumPositions=5
    };


    for ( int yp = 0; yp < (int64_t)getNy() - 1; ++yp ) { // go through image (pixel coordinates) in row major order
        QVector<QVector3D> vertices(NumPositions);

        for ( int xp = 0; xp < (int64_t)getNy() - 1; ++xp ) {

            if ( xp == 0 )
            {
                vertices[TopRight].setX(xp); // will be used for TopLeft later
                vertices[TopRight].setY(yp);
                vertices[TopRight].setZ(
                            value( vertices[TopRight].x(), vertices[TopRight].y())*scale
                );

                vertices[BottomRight].setX(xp);
                vertices[BottomRight].setY(yp+1);
                vertices[BottomRight].setZ(
                            value(vertices[BottomRight].x(), vertices[BottomRight].y())*scale
                );
            }

            vertices[TopLeft] = vertices[TopRight]; // use right vertices of the last box as new left vertices
            vertices[BottomLeft] = vertices[BottomRight];

            vertices[TopRight].setX(xp + 1);
            vertices[TopRight].setY(yp); // <----
            vertices[TopRight].setZ(
                value(vertices[TopRight].x(), vertices[TopRight].y())*scale
            );

            vertices[BottomRight].setX(xp + 1);
            vertices[BottomRight].setY(yp + 1);
            vertices[BottomRight].setZ(
                value(vertices[BottomRight].x(), vertices[BottomRight].y())*scale
                        );

            double zMin = vertices[TopLeft].z();
            double zMax = zMin;
            double zSum = zMin;

            for ( int i = TopRight; i <= BottomLeft; ++i ) {
                const double z = vertices[i].z();

                zSum += z;
                if ( z < zMin )
                    zMin = z;
                if ( z > zMax )
                    zMax = z;
            }


            if ( zMax >= contourLevels.first() && zMin <= contourLevels.last() ) {

                vertices[Center].setX(xp + 0.5); // pseudo pixel coordinates
                vertices[Center].setY(yp + 0.5);
                vertices[Center].setZ(0.25 * zSum);
                for (int levelIdx=0; levelIdx<contourLevels.size(); ++levelIdx) {
                    if ( contourLevels.at(levelIdx) >= zMin && contourLevels.at(levelIdx) <= zMax ) {

                        QLineF line;
                        QVector<QVector3D> triangle(3);

                       /*                     triangle[1]
                                                 X
                                               /   \
                                             /       \
                                           /     m     \
                                         /               \
                           triangle[2] +-------------------+ triangle[0]
                        */

                        for (int m = TopLeft; m < NumPositions; m++) { // construct triangles
                            triangle[0] = vertices[m];
                            triangle[1] = vertices[Center];
                            triangle[2] = vertices[(m!=BottomLeft)?(m + 1):TopLeft];

                            const bool intersects =intersect(line, triangle.at(0),triangle.at(1),triangle.at(2),
                                                             contourLevels.at(levelIdx));
                            if ( intersects ) {
                                ContourLines[levelIdx]<<line;
                            }
                        }
                    }
                }
            }
        }
    }
}

QPointF JKQTPContour::interpolatePoint(const QVector3D &point1, const QVector3D &point2,double level)
{  
    const double h1 = point1.z() - level; // height above contour level
    const double h2 = point2.z() - level;

//    // check if h1 or h2 is zero
//    Division by zero is not possible (the intersect function is not called if h2-h1 is zero, !)
//    if(h2-h1==0||h1==0||h2==0) {
//        qDebug()<<h1<<h2;
//        qDebug()<<"interpolate p1="<<point1<<", p2="<<point2<<" level="<<level;
//    }

    const double x = (h2 * point1.x() - h1 * point2.x()) / (h2 - h1); // linear interpolation in x.direction (independent of y)
    const double y = (h2 * point1.y() - h1 * point2.y()) / (h2 - h1);

//    const double alpha=(level-point1.z())/(point2.z()-point1.z());
//    const double x=point1.x()+alpha*(point2.x()-point1.x());
//    const double y=point1.y()+alpha*(point2.y()-point1.y());

    return QPointF(x, y);
}

