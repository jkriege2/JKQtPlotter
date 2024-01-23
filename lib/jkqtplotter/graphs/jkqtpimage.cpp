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


#include "jkqtplotter/graphs/jkqtpimage.h"
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
    JKQTPImageBase(x, y, width, height, parent->getPlotter())
{
}

JKQTPImageBase::JKQTPImageBase(JKQTPlotter *parent):
    JKQTPImageBase(parent->getPlotter())
{
}

void JKQTPImageBase::drawKeyMarker(JKQTPEnhancedPainter& /*painter*/, const QRectF& /*rect*/) {

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

QColor JKQTPImageBase::getKeyLabelColor() const {
    return QColor("black");
}

void JKQTPImageBase::setX(double __value)
{
    this->x = __value;
}

double JKQTPImageBase::getX() const
{
    return this->x;
}

void JKQTPImageBase::setY(double __value)
{
    this->y = __value;
}

double JKQTPImageBase::getY() const
{
    return this->y;
}

void JKQTPImageBase::setWidth(double __value)
{
    this->width = __value;
}

double JKQTPImageBase::getWidth() const
{
    return this->width;
}

void JKQTPImageBase::setHeight(double __value)
{
    this->height = __value;
}

double JKQTPImageBase::getHeight() const
{
    return this->height;
}

void JKQTPImageBase::plotImage(JKQTPEnhancedPainter& painter, QImage& image, double x, double y, double width, double height) {
    if ((!JKQTPIsOKFloat(x))||(!JKQTPIsOKFloat(y))||(!JKQTPIsOKFloat(width))||(!JKQTPIsOKFloat(height))||(width==0) || (height==0) || image.isNull() || (image.width()<=0) || (image.height()<=0)) {
        return;
    }
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

    // determine full shown plot rectangle in pixel coordinates
    const double xmin=parent->getXMin();
    const double xmax=parent->getXMax();
    const double ymin=parent->getYMin();
    const double ymax=parent->getYMax();
    QPointF pix_plot_topleft=transform(xmin,ymax);
    QPointF pix_plot_bottomright=transform(xmax,ymin);
    QRectF pix_plotrectangle(pix_plot_topleft, pix_plot_bottomright);

    // these flags say, whether to mirror the image in one or the other direction, before plotting
    bool mirrx=false;
    bool mirry=false;

    // determine pixel coordinates of image (x..x+width / y..y+width)
    QPointF pix_topelft=transform(x,y+height);
    QPointF pix_bottomright=transform(x+width,y);
    if (pix_topelft.x()>pix_bottomright.x()) {
        double tmp=pix_topelft.x();
        pix_topelft.setX(pix_bottomright.x());
        pix_bottomright.setX(tmp);
        tmp=pix_plot_topleft.x();
        pix_plot_topleft.setX(pix_plot_bottomright.x());
        pix_plot_bottomright.setX(tmp);
        mirrx=true;
    }
    if (pix_topelft.y()>pix_bottomright.y()) {
        double tmp=pix_topelft.y();
        pix_topelft.setY(pix_bottomright.y());
        pix_bottomright.setY(tmp);
        tmp=pix_plot_topleft.y();
        pix_plot_topleft.setY(pix_plot_bottomright.y());
        pix_plot_bottomright.setY(tmp);
        mirry=true;
    }
    const QRectF pix_imagerect(pix_topelft, pix_bottomright);



    if (image.width()>0 && image.height()>0 && !image.isNull()) {
        // now we determine, whether to directly draw the image (if its size is smaller than twice the plot rectangle,
        // or not ...
        if (pix_imagerect.width()<2.0*pix_plotrectangle.width() && pix_imagerect.height()<2.0*pix_plotrectangle.height()) {
            //painter.drawImage(QRectF(p1.x(), p2.y(), fabs(p2.x()-p1.x()), fabs(p2.y()-p1.y())), image);
            painter.drawImage(QPointF(pix_topelft.x(), pix_topelft.y()), image.mirrored(mirrx, mirry).scaled(QSize(fabs(pix_bottomright.x()-pix_topelft.x()), fabs(pix_bottomright.y()-pix_topelft.y())), Qt::IgnoreAspectRatio, Qt::FastTransformation));
            //qDebug()<<"\nimage.size  = "<<image.size() <<" SIMPLE!";
        } else {
            // ... if the image is much larger than the plot rectangle, we cut a potion from the image, before plotting
            const double pixwidth=fabs(pix_bottomright.x()-pix_topelft.x())/static_cast<double>(image.width());
            const double pixheight=fabs(pix_bottomright.y()-pix_topelft.y())/static_cast<double>(image.height());
            //qDebug()<<"\nimage.size  = "<<image.size();
            //qDebug()<<"size  = "<<QSizeF(width, height);
            //qDebug()<<"pixsize  = "<<QSizeF(pixwidth, pixheight);
            //qDebug()<<"p1  = "<<p1<< "    p2  = "<<p2;
            //qDebug()<<"pp1 = "<<pp1<<"    pp2 = "<<pp2;
            QPointF ps1(floor((pix_plot_topleft.x()-pix_topelft.x())/pixwidth), floor((pix_plot_topleft.y()-pix_topelft.y())/pixheight));
            QPointF ps2=ps1+QPointF(ceil(fabs(pix_plot_bottomright.x()-pix_plot_topleft.x())/pixwidth), ceil(fabs(pix_plot_bottomright.y()-pix_plot_topleft.y())/pixheight));
            if (ps1.x()<0) ps1.setX(0);
            if (ps1.y()<0) ps1.setY(0);
            if (ps2.x()>image.width()) ps2.setX(image.width());
            if (ps2.y()>image.height()) ps2.setY(image.height());
            const QRectF source(ps1, ps2);
            const QRectF target(pix_topelft.x()+ps1.x()*pixwidth, pix_topelft.y()+ps1.y()*pixheight, source.width()*pixwidth, source.height()*pixheight);
            //qDebug()<<"source = "<<source;
            //qDebug()<<"target = "<<target;
            painter.drawImage(target, image.mirrored(mirrx, mirry), source);
        }
    }

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

void JKQTPImage::drawKeyMarker(JKQTPEnhancedPainter &painter, const QRectF &rect)
{
    painter.drawImage(rect, QPixmap(":/JKQTPlotter/jkqtp_plot_image.png").toImage());
}

void JKQTPImage::setImage(const QImage &image)
{
    clear_image();
    this->image=new QImage(image.mirrored(false, true));
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
    actCopyImage=new QAction(tr("Copy JKQTPImage ..."), this);
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








JKQTPMathImageBase::JKQTPMathImageBase(JKQTBasePlotter *parent):
    JKQTPImageBase(parent),
    data(nullptr),
    datatype(JKQTPMathImageDataType::DoubleArray),
    Nx(0), Ny(0),
    dataModifier(nullptr), datatypeModifier(JKQTPMathImageDataType::DoubleArray),
    internalDataMin(0.0), internalDataMax(0.0),
    internalModifierMin(0.0), internalModifierMax(0.0)
{

}

JKQTPMathImageBase::JKQTPMathImageBase(double x, double y, double width, double height, JKQTBasePlotter *parent):
    JKQTPImageBase(x,y,width,height,parent),
    data(nullptr),
    datatype(JKQTPMathImageDataType::DoubleArray),
    Nx(0), Ny(0),
    dataModifier(nullptr), datatypeModifier(JKQTPMathImageDataType::DoubleArray),
    internalDataMin(0.0), internalDataMax(0.0),
    internalModifierMin(0.0), internalModifierMax(0.0)
{

}

JKQTPMathImageBase::JKQTPMathImageBase(JKQTPlotter *parent):
    JKQTPMathImageBase(parent->getPlotter())
{
}

JKQTPMathImageBase::JKQTPMathImageBase(double x, double y, double width, double height, JKQTPlotter *parent):
    JKQTPMathImageBase(x,y,width,height,parent->getPlotter())
{
}

JKQTPMathImageBase::JKQTPMathImageBase(double x, double y, double width, double height, JKQTPMathImageDataType datatype, const void* data, int Nx, int Ny, JKQTBasePlotter* parent):
    JKQTPMathImageBase(x, y, width, height, parent)
{
    this->data=data;
    this->datatype=datatype;
    this->Nx=Nx;
    this->Ny=Ny;
    dataModifier=nullptr;
    datatypeModifier=JKQTPMathImageDataType::DoubleArray;
}


JKQTPMathImageBase::JKQTPMathImageBase(double x, double y, double width, double height, JKQTPMathImageDataType datatype, const void* data, int Nx, int Ny, JKQTPlotter* parent):
    JKQTPMathImageBase(x, y, width, height, datatype, data, Nx, Ny, parent->getPlotter())
{

}
void JKQTPMathImageBase::drawKeyMarker(JKQTPEnhancedPainter &/*painter*/, const QRectF &/*rect*/)
{

}

void JKQTPMathImageBase::setNx(int __value)
{
    Nx = __value;
}

void JKQTPMathImageBase::setNx(size_t __value)
{
    Nx = static_cast<int>(__value);
}

int JKQTPMathImageBase::getNx() const
{
    return Nx;
}

void JKQTPMathImageBase::setNy(int __value)
{
    Ny = __value;
}

void JKQTPMathImageBase::setNy(size_t __value)
{
    Ny = static_cast<int>(__value);
}

int JKQTPMathImageBase::getNy() const
{
    return Ny;
}

void JKQTPMathImageBase::setData(const void *__value)
{
    data = __value;
}

const void *JKQTPMathImageBase::getData() const
{
    return data;
}

void JKQTPMathImageBase::setDatatype(JKQTPMathImageDataType __value)
{
    datatype = __value;
}

JKQTPMathImageDataType JKQTPMathImageBase::getDatatype() const
{
    return datatype;
}

void JKQTPMathImageBase::setDataModifier(const void *__value)
{
    dataModifier = __value;
}

const void *JKQTPMathImageBase::getDataModifier() const
{
    return dataModifier;
}

void JKQTPMathImageBase::setDatatypeModifier(JKQTPMathImageDataType __value)
{
    datatypeModifier = __value;
}

JKQTPMathImageDataType JKQTPMathImageBase::getDatatypeModifier() const
{
    return datatypeModifier;
}




void JKQTPMathImageBase::setData(const void *data_, int Nx_, int Ny_, JKQTPMathImageDataType datatype_) {
    data=data_;
    datatype=datatype_;
    Nx=Nx_;
    Ny=Ny_;
}

void JKQTPMathImageBase::setData(const void *data_, int Nx_, int Ny_) {
    data=data_;
    Nx=Nx_;
    Ny=Ny_;
}

void JKQTPMathImageBase::setDataModifier(const void *data, JKQTPMathImageDataType datatype)
{
    dataModifier=data;
    datatypeModifier=datatype;
}


void JKQTPMathImageBase::getDataMinMax(double& imin, double& imax) {
    ensureImageData();
    imin=imax=0;
    if (!data) return;
    switch(datatype) {
        case JKQTPMathImageDataType::DoubleArray:
            imin= JKQTPImagePlot_getImageMin<double>(static_cast<const double*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<double>(static_cast<const double*>(data), Nx, Ny);
            break;
        case JKQTPMathImageDataType::FloatArray:
            imin= JKQTPImagePlot_getImageMin<float>(static_cast<const float*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<float>(static_cast<const float*>(data), Nx, Ny);
            break;
        case JKQTPMathImageDataType::UInt8Array:
            imin= JKQTPImagePlot_getImageMin<uint8_t>(static_cast<const uint8_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint8_t>(static_cast<const uint8_t*>(data), Nx, Ny);
            break;
        case JKQTPMathImageDataType::UInt16Array:
            imin= JKQTPImagePlot_getImageMin<uint16_t>(static_cast<const uint16_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint16_t>(static_cast<const uint16_t*>(data), Nx, Ny);
            break;
        case JKQTPMathImageDataType::UInt32Array:
            imin= JKQTPImagePlot_getImageMin<uint32_t>(static_cast<const uint32_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint32_t>(static_cast<const uint32_t*>(data), Nx, Ny);
            break;
        case JKQTPMathImageDataType::UInt64Array:
            imin= JKQTPImagePlot_getImageMin<uint64_t>(static_cast<const uint64_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint64_t>(static_cast<const uint64_t*>(data), Nx, Ny);
            break;
        case JKQTPMathImageDataType::Int8Array:
            imin= JKQTPImagePlot_getImageMin<int8_t>(static_cast<const int8_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int8_t>(static_cast<const int8_t*>(data), Nx, Ny);
            break;
        case JKQTPMathImageDataType::Int16Array:
            imin= JKQTPImagePlot_getImageMin<int16_t>(static_cast<const int16_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int16_t>(static_cast<const int16_t*>(data), Nx, Ny);
            break;
        case JKQTPMathImageDataType::Int32Array:
            imin= JKQTPImagePlot_getImageMin<int32_t>(static_cast<const int32_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int32_t>(static_cast<const int32_t*>(data), Nx, Ny);
            break;
        case JKQTPMathImageDataType::Int64Array:
            imin= JKQTPImagePlot_getImageMin<int64_t>(static_cast<const int64_t*>(data), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int64_t>(static_cast<const int64_t*>(data), Nx, Ny);
            break;
    }
}

void JKQTPMathImageBase::getModifierMinMax(double &imin, double &imax)
{
    ensureImageData();
    imin=imax=0;
    if (!dataModifier) return;
    switch(datatypeModifier) {
        case JKQTPMathImageDataType::DoubleArray:
            imin= JKQTPImagePlot_getImageMin<double>(static_cast<const double*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<double>(static_cast<const double*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageDataType::FloatArray:
            imin= JKQTPImagePlot_getImageMin<float>(static_cast<const float*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<float>(static_cast<const float*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageDataType::UInt8Array:
            imin= JKQTPImagePlot_getImageMin<uint8_t>(static_cast<const uint8_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint8_t>(static_cast<const uint8_t*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageDataType::UInt16Array:
            imin= JKQTPImagePlot_getImageMin<uint16_t>(static_cast<const uint16_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint16_t>(static_cast<const uint16_t*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageDataType::UInt32Array:
            imin= JKQTPImagePlot_getImageMin<uint32_t>(static_cast<const uint32_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint32_t>(static_cast<const uint32_t*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageDataType::UInt64Array:
            imin= JKQTPImagePlot_getImageMin<uint64_t>(static_cast<const uint64_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<uint64_t>(static_cast<const uint64_t*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageDataType::Int8Array:
            imin= JKQTPImagePlot_getImageMin<int8_t>(static_cast<const int8_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int8_t>(static_cast<const int8_t*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageDataType::Int16Array:
            imin= JKQTPImagePlot_getImageMin<int16_t>(static_cast<const int16_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int16_t>(static_cast<const int16_t*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageDataType::Int32Array:
            imin= JKQTPImagePlot_getImageMin<int32_t>(static_cast<const int32_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int32_t>(static_cast<const int32_t*>(dataModifier), Nx, Ny);
            break;
        case JKQTPMathImageDataType::Int64Array:
            imin= JKQTPImagePlot_getImageMin<int64_t>(static_cast<const int64_t*>(dataModifier), Nx, Ny);
            imax= JKQTPImagePlot_getImageMax<int64_t>(static_cast<const int64_t*>(dataModifier), Nx, Ny);
            break;
    }
}

QVector<double> JKQTPMathImageBase::getDataAsDoubleVector() const
{
    switch(datatype) {
        case JKQTPMathImageDataType::DoubleArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<const double*>(data), Nx*Ny);
            break;
        case JKQTPMathImageDataType::FloatArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<const float*>(data), Nx*Ny);
            break;
        case JKQTPMathImageDataType::UInt8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const uint8_t*>(data), Nx*Ny);
            break;
        case JKQTPMathImageDataType::UInt16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const uint16_t*>(data), Nx*Ny);
            break;
        case JKQTPMathImageDataType::UInt32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const uint32_t*>(data), Nx*Ny);
            break;
        case JKQTPMathImageDataType::UInt64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const uint64_t*>(data), Nx*Ny);
            break;
        case JKQTPMathImageDataType::Int8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const int8_t*>(data), Nx*Ny);
            break;
        case JKQTPMathImageDataType::Int16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const int16_t*>(data), Nx*Ny);
            break;
        case JKQTPMathImageDataType::Int32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const int32_t*>(data), Nx*Ny);
            break;
        case JKQTPMathImageDataType::Int64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const int64_t*>(data), Nx*Ny);
            break;
    }
    QVector<double> res;
    return res;
}

QVector<double> JKQTPMathImageBase::getDataModifierAsDoubleVector() const
{
    switch(datatypeModifier) {
        case JKQTPMathImageDataType::DoubleArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<const double*>(dataModifier), Nx*Ny);
        case JKQTPMathImageDataType::FloatArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<const float*>(dataModifier), Nx*Ny);
        case JKQTPMathImageDataType::UInt8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const uint8_t*>(dataModifier), Nx*Ny);
        case JKQTPMathImageDataType::UInt16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const uint16_t*>(dataModifier), Nx*Ny);
        case JKQTPMathImageDataType::UInt32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const uint32_t*>(dataModifier), Nx*Ny);
        case JKQTPMathImageDataType::UInt64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const uint64_t*>(dataModifier), Nx*Ny);
        case JKQTPMathImageDataType::Int8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const int8_t*>(dataModifier), Nx*Ny);
        case JKQTPMathImageDataType::Int16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const int16_t*>(dataModifier), Nx*Ny);
        case JKQTPMathImageDataType::Int32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const int32_t*>(dataModifier), Nx*Ny);
        case JKQTPMathImageDataType::Int64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<const int64_t*>(dataModifier), Nx*Ny);
    }
    QVector<double> res;
    return res;
}

void JKQTPMathImageBase::ensureImageData()
{
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



    palette=JKQTPMathImageGRAY;
    autoModifierRange=true;
}

JKQTPMathImage::JKQTPMathImage(double x, double y, double width, double height, JKQTPMathImageDataType datatype, const void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette, JKQTBasePlotter* parent):
    JKQTPMathImageBase(x, y, width, height, datatype, data, Nx, Ny, parent),
    JKQTPColorPaletteWithModifierStyleAndToolsMixin(parent),
    actSaveImage(nullptr),
    actCopyImage(nullptr),
    actSavePalette(nullptr),
    actCopyPalette(nullptr)
{
    initJKQTPMathImage();
    this->palette=palette;
}

JKQTPMathImage::JKQTPMathImage(JKQTBasePlotter *parent):
    JKQTPMathImage(0, 0, 1, 1, JKQTPMathImageDataType::UInt8Array, nullptr, 0, 0, JKQTPMathImageGRAY, parent)
{
    if (parent) this->palette=parent->getCurrentPlotterStyle().graphsStyle.defaultPalette;
}



JKQTPMathImage::JKQTPMathImage(double x, double y, double width, double height, JKQTPMathImageDataType datatype, const void *data, int Nx, int Ny, JKQTPMathImageColorPalette palette, JKQTPlotter* parent):
    JKQTPMathImage(x, y, width, height, datatype, data, Nx, Ny, palette, parent->getPlotter())
{
}

JKQTPMathImage::JKQTPMathImage(JKQTPlotter *parent):
    JKQTPMathImage(parent->getPlotter())
{
}

void JKQTPMathImage::setParent(JKQTBasePlotter* parent) {
    if (this->parent) {
        this->parent->deregisterAdditionalAction(actSaveImage);
        this->parent->deregisterAdditionalAction(actCopyImage);
        this->parent->deregisterAdditionalAction(actSavePalette);
        this->parent->deregisterAdditionalAction(actCopyPalette);
    }
    JKQTPMathImageBase::setParent(parent);

    cbSetParent(parent);

    if (parent) {
        parent->registerAdditionalAction(tr("Save Image Plot Images ..."), actSaveImage);
        parent->registerAdditionalAction(tr("Copy Image Plot Images ..."), actCopyImage);
        parent->registerAdditionalAction(tr("Save Image Plot Palettes ..."), actSavePalette);
        parent->registerAdditionalAction(tr("Copy Image Plot Palettes ..."), actCopyPalette);
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
            fn = QFileDialog::getSaveFileName(nullptr, tr("Save Palette Plot As Image ..."),
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
    JKQTPMathImageBase::getOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
    cbGetOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
}

void JKQTPMathImage::drawOutside(JKQTPEnhancedPainter& painter, QRect leftSpace, QRect rightSpace, QRect topSpace, QRect bottomSpace) {
    JKQTPMathImageBase::drawOutside(painter, leftSpace, rightSpace, topSpace, bottomSpace);
    if (showColorBar) cbDrawOutside(painter, leftSpace, rightSpace, topSpace, bottomSpace);
}

QImage JKQTPMathImage::drawOutsidePalette(uint8_t steps)
{

    uint8_t h=1;
    if (modifierMode!=JKQTPMathImageModifierMode::ModifyNone) {
        h=50;
    }
    std::vector<double> d(steps*h, 0.0);
    std::vector<double> dd(steps*h, 0.0);
    for (uint8_t i=0; i<steps; i++) {
        for (uint8_t j=0; j<h; j++) {
            d[i*h+j]=i;
            dd[i*h+j]=j;
            //qDebug()<<"x="<<j<<", y="<<i<<": d="<<d[i*h+j]<<" dd="<<dd[i*h+j];
        }
    }

    QImage b(h,200, QImage::Format_ARGB32);
    JKQTPImageTools::array2image<double>(d.data(),h,steps, b, palette, 0, steps-1);
    //b.save("c:/temp/b1.png");
    if (modifierMode!=JKQTPMathImageModifierMode::ModifyNone) {
        modifyImage(b, dd.data(), JKQTPMathImageDataType::DoubleArray, h,steps, 0, h-1);
        //b.save("c:/temp/b2.png");
    }
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



void JKQTPMathImage::drawKeyMarker(JKQTPEnhancedPainter &painter, const QRectF &rect)
{
    painter.drawImage(rect, getPaletteKeyImage(palette, 32,32));
}

void JKQTPMathImage::cbGetDataMinMax(double &imin, double &imax)
{
    getDataMinMax(imin, imax);
}

void JKQTPMathImage::cbGetModifierDataMinMax(double &imin, double &imax)
{
    getModifierMinMax(imin, imax);
}





QImage JKQTPMathImage::drawImage() {
    if (!data) return QImage();
    QImage img(Nx, Ny, QImage::Format_ARGB32);
    getDataMinMax(internalDataMin, internalDataMax);
    getModifierMinMax(internalModifierMin, internalModifierMax);
    switch(datatype) {
        case JKQTPMathImageDataType::DoubleArray: JKQTPImageTools::array2image<double>(static_cast<const double*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageDataType::FloatArray: JKQTPImageTools::array2image<float>(static_cast<const float*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageDataType::UInt8Array: JKQTPImageTools::array2image<uint8_t>(static_cast<const uint8_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageDataType::UInt16Array: JKQTPImageTools::array2image<uint16_t>(static_cast<const uint16_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageDataType::UInt32Array: JKQTPImageTools::array2image<uint32_t>(static_cast<const uint32_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageDataType::UInt64Array: JKQTPImageTools::array2image<uint64_t>(static_cast<const uint64_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageDataType::Int8Array: JKQTPImageTools::array2image<int8_t>(static_cast<const int8_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageDataType::Int16Array: JKQTPImageTools::array2image<int16_t>(static_cast<const int16_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageDataType::Int32Array: JKQTPImageTools::array2image<int32_t>(static_cast<const int32_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
        case JKQTPMathImageDataType::Int64Array: JKQTPImageTools::array2image<int64_t>(static_cast<const int64_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rangeMinFailAction, rangeMaxFailAction, rangeMinFailColor, rangeMaxFailColor, nanColor, infColor); break;
    }
    modifyImage(img, dataModifier, datatypeModifier, Nx, Ny, internalModifierMin, internalModifierMax);
    return img;
}





JKQTPColumnMathImage::JKQTPColumnMathImage(JKQTBasePlotter *parent):
    JKQTPMathImage(parent)
{
    this->modifierColumn=-1;
    this->imageColumn=-1;
    this->datatype=JKQTPMathImageDataType::DoubleArray;
}

JKQTPColumnMathImage::JKQTPColumnMathImage(double x, double y, double width, double height, JKQTBasePlotter *parent):
    JKQTPMathImage(x,y,width,height,JKQTPMathImageDataType::DoubleArray,nullptr,0,0,JKQTPMathImageGRAY,parent)
{
    this->modifierColumn=-1;
    this->imageColumn=-1;
    this->datatype=JKQTPMathImageDataType::DoubleArray;
}

JKQTPColumnMathImage::JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, JKQTPMathImageColorPalette palette, JKQTBasePlotter *parent):
    JKQTPMathImage(x,y,width,height,JKQTPMathImageDataType::DoubleArray,nullptr,0,0,palette,parent)
{
    this->modifierColumn=-1;
    this->imageColumn=imageColumn;
    if (parent && imageColumn>=0 && parent->getDatastore()) {
        Nx= static_cast<int>(parent->getDatastore()->getColumnImageWidth(imageColumn));
        Ny= static_cast<int>(parent->getDatastore()->getColumnImageHeight(imageColumn));
    }
    this->datatype=JKQTPMathImageDataType::DoubleArray;
}

JKQTPColumnMathImage::JKQTPColumnMathImage(JKQTPlotter *parent):
    JKQTPColumnMathImage(parent->getPlotter())
{
}

JKQTPColumnMathImage::JKQTPColumnMathImage(double x, double y, double width, double height, JKQTPlotter *parent):
    JKQTPColumnMathImage(x,y,width,height,parent->getPlotter())
{
}

JKQTPColumnMathImage::JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, JKQTPMathImageColorPalette palette, JKQTPlotter *parent):
    JKQTPColumnMathImage(x,y,width,height,imageColumn,palette,parent->getPlotter())
{
}

JKQTPColumnMathImage::JKQTPColumnMathImage(double x, double y, double width, double height, int imageColumn, JKQTPlotter *parent):
    JKQTPColumnMathImage(x,y,width,height,imageColumn,JKQTPMathImageGRAY,parent->getPlotter())
{
}

void JKQTPColumnMathImage::setImageColumn(size_t __value)
{
    setImageColumn(static_cast<int>(__value));
}

void JKQTPColumnMathImage::setModifierColumn(size_t __value)
{
    setModifierColumn(static_cast<int>(__value));
}
void JKQTPColumnMathImage::setImageColumn(int __value)
{
    this->imageColumn = __value;
    if (parent && __value>=0 && parent->getDatastore()) {
        setNx(parent->getDatastore()->getColumnImageWidth(__value));
        setNy(parent->getDatastore()->getColumnImageHeight(__value));
    }
}

int JKQTPColumnMathImage::getImageColumn() const
{
    return this->imageColumn;
}

void JKQTPColumnMathImage::setModifierColumn(int __value)
{
    this->modifierColumn = __value;
    if (parent && __value>=0 && parent->getDatastore()) {
        setNx(parent->getDatastore()->getColumnImageWidth(__value));
        setNy(parent->getDatastore()->getColumnImageHeight(__value));
    }
}

int JKQTPColumnMathImage::getModifierColumn() const
{
    return this->modifierColumn;
}
bool JKQTPColumnMathImage::usesColumn(int c) const
{
    return (c==imageColumn)||(c==modifierColumn);
}


void JKQTPColumnMathImage::ensureImageData()
{

    if (this->Nx==0 || imageColumn<0 || !parent->getDatastore()->getColumnPointer(imageColumn,0)) {
        this->Ny=0;
        this->data=nullptr;
        this->datatype=JKQTPMathImageDataType::DoubleArray;
    } else {
        this->datatype=JKQTPMathImageDataType::DoubleArray;
        this->data=parent->getDatastore()->getColumnPointer(imageColumn,0);
        this->Ny= static_cast<int>(parent->getDatastore()->getRows(imageColumn)/this->Nx);
    }
    if (this->Nx==0 || modifierColumn<0 || !parent->getDatastore()->getColumnPointer(modifierColumn,0)) {
        this->dataModifier=nullptr;
    } else {
        this->datatypeModifier=JKQTPMathImageDataType::DoubleArray;
        this->dataModifier=parent->getDatastore()->getColumnPointer(modifierColumn,0);
    }
}

