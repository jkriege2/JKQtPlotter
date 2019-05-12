/*
    Copyright (c) 2008-2019 Jan W. Krieger

    

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

    double xmin=parent->getXMin();
    double xmax=parent->getXMax();
    double ymin=parent->getYMin();
    double ymax=parent->getYMax();
    QPointF pp1=transform(xmin,ymax);
    QPointF pp2=transform(xmax,ymin);
    QRectF pr(pp1, pp2);

    bool mirrx=false;
    bool mirry=false;

    QPointF p1=transform(x,y+height);
    QPointF p2=transform(x+width,y);
    if (p1.x()>p2.x()) {
        double tmp=p1.x();
        p1.setX(p2.x());
        p2.setX(tmp);
        tmp=pp1.x();
        pp1.setX(pp2.x());
        pp2.setX(tmp);
        mirrx=true;
    }
    if (p1.y()>p2.y()) {
        double tmp=p1.y();
        p1.setY(p2.y());
        p2.setY(tmp);
        tmp=pp1.y();
        pp1.setY(pp2.y());
        pp2.setY(tmp);
        mirry=true;
    }
    QRectF r(p1, p2);



    if (image.width()>0 && image.height()>0 && !image.isNull()) {
        if (r.width()<2*pr.width() && r.height()<2*pr.height()) {
            //painter.drawImage(QRectF(p1.x(), p2.y(), fabs(p2.x()-p1.x()), fabs(p2.y()-p1.y())), image);
            painter.drawImage(QPoint(p1.x(), p1.y()), image.mirrored(mirrx, mirry).scaled(QSize(fabs(p2.x()-p1.x()), fabs(p2.y()-p1.y())), Qt::IgnoreAspectRatio, Qt::FastTransformation));
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

void JKQTPImage::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
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

void JKQTPMathImageBase::setNx(int __value)
{
    this->Nx = __value;
}

int JKQTPMathImageBase::getNx() const
{
    return this->Nx;
}

void JKQTPMathImageBase::setNy(int __value)
{
    this->Ny = __value;
}

int JKQTPMathImageBase::getNy() const
{
    return this->Ny;
}

void JKQTPMathImageBase::setData(void *__value)
{
    this->data = __value;
}

void *JKQTPMathImageBase::getData() const
{
    return this->data;
}

void JKQTPMathImageBase::setDatatype(JKQTPMathImageBase::DataType __value)
{
    this->datatype = __value;
}

JKQTPMathImageBase::DataType JKQTPMathImageBase::getDatatype() const
{
    return this->datatype;
}

void JKQTPMathImageBase::setDataModifier(void *__value)
{
    this->dataModifier = __value;
}

void *JKQTPMathImageBase::getDataModifier() const
{
    return this->dataModifier;
}

void JKQTPMathImageBase::setDatatypeModifier(JKQTPMathImageBase::DataType __value)
{
    this->datatypeModifier = __value;
}

JKQTPMathImageBase::DataType JKQTPMathImageBase::getDatatypeModifier() const
{
    return this->datatypeModifier;
}

void JKQTPMathImageBase::setModifierMode(const JKQTPMathImageBase::ModifierMode &__value)
{
    this->modifierMode = __value;
}

JKQTPMathImageBase::ModifierMode JKQTPMathImageBase::getModifierMode() const
{
    return this->modifierMode;
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
    colorBarRightAxis->setDrawMode1(JKQTPCADMLine);
    colorBarRightAxis->setDrawMode2(JKQTPCADMcomplete);
    colorBarRightAxis->setAxisLabel("");
    colorBarRightAxis->setMinTicks(5);
    colorBarRightAxis->setMinorTicks(0);
    colorBarRightAxis->setTickOutsideLength(0);
    colorBarRightAxis->setMinorTickOutsideLength(0);
    colorBarRightAxis->setShowZeroAxis(false);
    colorBarTopAxis=new JKQTPHorizontalIndependentAxis(0, 100, 0, 100, parent);
    colorBarTopAxis->setDrawMode1(JKQTPCADMLine);
    colorBarTopAxis->setDrawMode2(JKQTPCADMcomplete);
    colorBarTopAxis->setAxisLabel("");
    colorBarTopAxis->setMinTicks(3);
    colorBarTopAxis->setMinorTicks(0);
    colorBarTopAxis->setTickOutsideLength(0);
    colorBarTopAxis->setMinorTickOutsideLength(0);
    colorBarTopAxis->setShowZeroAxis(false);

    modifierColorBarTopAxis=new JKQTPVerticalIndependentAxis(0, 100, 0, 100, parent);
    modifierColorBarTopAxis->setDrawMode1(JKQTPCADMLine);
    modifierColorBarTopAxis->setDrawMode2(JKQTPCADMcomplete);
    modifierColorBarTopAxis->setAxisLabel("");
    modifierColorBarTopAxis->setMinTicks(3);
    modifierColorBarTopAxis->setShowZeroAxis(false);
    modifierColorBarTopAxis->setMinorTicks(0);
    modifierColorBarTopAxis->setTickOutsideLength(0);
    modifierColorBarTopAxis->setMinorTickOutsideLength(0);
    modifierColorBarRightAxis=new JKQTPHorizontalIndependentAxis (0, 100, 0, 100, parent);
    modifierColorBarRightAxis->setDrawMode1(JKQTPCADMLine);
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
    this->imageNameFontSize=parent->getDefaultTextSize();
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
    if (parent) this->palette=parent->getCurrentPlotterStyle().defaultPalette;
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
    if (parent) this->palette=parent->getPlotter()->getCurrentPlotterStyle().defaultPalette;

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
            QSizeF s1=colorBarRightAxis->getSize2(painter);
            QSizeF s2=colorBarRightAxis->getSize1(painter);

            QSizeF names=parent->getTextSizeSize(parent->getCurrentPlotterStyle().defaultFontName, imageNameFontSize*parent->getFontSizeMultiplier(), imageName, painter);
            rightSpace+=qMax(static_cast<double>(s1.width()+s2.width()), static_cast<double>(names.width()));
            //qDebug()<<"osr: "<<imageName<<rightSpace<<colorBarModifiedWidth<<colorBarWidth;
            if (modifierMode!=ModifyNone) {
                rightSpace=rightSpace+(2.0*parent->pt2px(painter, colorBarModifiedWidth-colorBarWidth));
            }
            //qDebug()<<"     "<<imageName<<rightSpace;
        }
        if (colorBarTopVisible) {
            //topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
            colorBarTopAxis->setRange(internalDataMin, internalDataMax);
            colorBarTopAxis->setAxisWidth(colorBarRelativeHeight*parent->getPlotWidth());
            modifierColorBarTopAxis->setRange(internalModifierMin, internalModifierMax);
            modifierColorBarTopAxis->setAxisWidth(parent->pt2px(painter, colorBarModifiedWidth));
            QSizeF s1=colorBarTopAxis->getSize2(painter);
            QSizeF s2=colorBarTopAxis->getSize1(painter);
            QSizeF names=parent->getTextSizeSize(parent->getCurrentPlotterStyle().defaultFontName, imageNameFontSize*parent->getFontSizeMultiplier(), imageName, painter);
            //qDebug()<<topSpace<<s<<imageName<<names;
            //qDebug()<<colorBarWidth<<colorBarOffset<<parent->pt2px(painter, colorBarWidth+colorBarOffset);
            topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+qMax(static_cast<double>(s1.height()+s2.height()), static_cast<double>(names.height()));
            if (modifierMode!=ModifyNone) {
                topSpace=topSpace+(2.0*parent->pt2px(painter, colorBarModifiedWidth-colorBarWidth));
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
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});



            QImage b=drawOutsidePalette(200);


            QSizeF names=parent->getTextSizeSize(parent->getCurrentPlotterStyle().defaultFontName, imageNameFontSize*parent->getFontSizeMultiplier(), imageName, painter);

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
            p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, colorBarRightAxis->getLineWidth()*parent->getLineWidthMultiplier())));
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
            parent->getMathText()->setFontRomanOrSpecial(parent->getCurrentPlotterStyle().defaultFontName);
            parent->getMathText()->parse(imageName);
            parent->getMathText()->draw(painter, Qt::AlignHCenter|Qt::AlignVCenter, QRect(rightSpace.x(), rightSpace.y(), rightSpace.width(), (rightSpace.height()-barHeight)/2));


        }
        if (colorBarTopVisible) {
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});





            QImage b=drawOutsidePalette(200);


            QSizeF names=parent->getTextSizeSize(parent->getCurrentPlotterStyle().defaultFontName, imageNameFontSize*parent->getFontSizeMultiplier(), imageName, painter);

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
            p.setWidthF(qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, colorBarTopAxis->getLineWidth()*parent->getLineWidthMultiplier())));
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
            parent->getMathText()->setFontRomanOrSpecial(parent->getCurrentPlotterStyle().defaultFontName);
            parent->getMathText()->parse(imageName);
            parent->getMathText()->draw(painter, Qt::AlignHCenter|Qt::AlignVCenter, QRect(topSpace.right()-(topSpace.width()-barWidth)/2, topSpace.y(), (topSpace.width()-barWidth)/2, topSpace.height()));


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
    QImage img=getPaletteImage(i, JKQTPImageTools::PALETTE_ICON_WIDTH);
    QPixmap pix(JKQTPImageTools::PALETTE_ICON_WIDTH,8);
    QRect r(0,0,JKQTPImageTools::PALETTE_ICON_WIDTH-1,7);
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
    QImage img=getPaletteKeyImage(i, JKQTPImageTools::PALETTE_ICON_WIDTH, JKQTPImageTools::PALETTE_IMAGEICON_HEIGHT);
    QPixmap pix(JKQTPImageTools::PALETTE_ICON_WIDTH,JKQTPImageTools::PALETTE_IMAGEICON_HEIGHT);
    QRect r(0,0,JKQTPImageTools::PALETTE_ICON_WIDTH-1,JKQTPImageTools::PALETTE_IMAGEICON_HEIGHT-1);
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

void JKQTPMathImage::setPalette(const JKQTPMathImageColorPalette &__value)
{
    this->palette = __value;
}

JKQTPMathImageColorPalette JKQTPMathImage::getPalette() const
{
    return this->palette;
}

void JKQTPMathImage::setRangeMinFailAction(const JKQTPMathImageColorRangeFailAction &__value)
{
    this->rangeMinFailAction = __value;
}

JKQTPMathImageColorRangeFailAction JKQTPMathImage::getActionRangeMinFail() const
{
    return this->rangeMinFailAction;
}

void JKQTPMathImage::setRangeMaxFailAction(const JKQTPMathImageColorRangeFailAction &__value)
{
    this->rangeMaxFailAction = __value;
}

JKQTPMathImageColorRangeFailAction JKQTPMathImage::getActionRangeMaxFail() const
{
    return this->rangeMaxFailAction;
}

void JKQTPMathImage::setRangeMinFailColor(const QColor &__value)
{
    this->rangeMinFailColor = __value;
}

QColor JKQTPMathImage::getRangeMinFailColor() const
{
    return this->rangeMinFailColor;
}

void JKQTPMathImage::setRangeMaxFailColor(const QColor &__value)
{
    this->rangeMaxFailColor = __value;
}

QColor JKQTPMathImage::getRangeMaxFailColor() const
{
    return this->rangeMaxFailColor;
}

void JKQTPMathImage::setNanColor(const QColor &__value)
{
    this->nanColor = __value;
}

QColor JKQTPMathImage::getNanColor() const
{
    return this->nanColor;
}

void JKQTPMathImage::setInfColor(const QColor &__value)
{
    this->infColor = __value;
}

QColor JKQTPMathImage::getInfColor() const
{
    return this->infColor;
}

void JKQTPMathImage::setShowColorBar(bool __value)
{
    this->showColorBar = __value;
}

bool JKQTPMathImage::getShowColorBar() const
{
    return this->showColorBar;
}

void JKQTPMathImage::setColorBarWidth(double __value)
{
    this->colorBarWidth = __value;
}

double JKQTPMathImage::getColorBarWidth() const
{
    return this->colorBarWidth;
}

void JKQTPMathImage::setColorBarModifiedWidth(double __value)
{
    this->colorBarModifiedWidth = __value;
}

double JKQTPMathImage::getColorBarModifiedWidth() const
{
    return this->colorBarModifiedWidth;
}

void JKQTPMathImage::setColorBarOffset(double __value)
{
    this->colorBarOffset = __value;
}

double JKQTPMathImage::getColorBarOffset() const
{
    return this->colorBarOffset;
}

void JKQTPMathImage::setColorBarRelativeHeight(double __value)
{
    this->colorBarRelativeHeight = __value;
}

double JKQTPMathImage::getColorBarRelativeHeight() const
{
    return this->colorBarRelativeHeight;
}

void JKQTPMathImage::setImageMin(double __value)
{
    this->imageMin = __value;
}

double JKQTPMathImage::getImageMin() const
{
    return this->imageMin;
}

void JKQTPMathImage::setImageMax(double __value)
{
    this->imageMax = __value;
}

double JKQTPMathImage::getImageMax() const
{
    return this->imageMax;
}

void JKQTPMathImage::setAutoImageRange(bool __value)
{
    this->autoImageRange = __value;
}

bool JKQTPMathImage::getAutoImageRange() const
{
    return this->autoImageRange;
}

void JKQTPMathImage::setImageName(const QString &__value)
{
    this->imageName = __value;
}

QString JKQTPMathImage::getImageName() const
{
    return this->imageName;
}

void JKQTPMathImage::setImageNameFontSize(double __value)
{
    this->imageNameFontSize = __value;
}

double JKQTPMathImage::getImageNameFontSize() const
{
    return this->imageNameFontSize;
}

JKQTPVerticalIndependentAxis *JKQTPMathImage::getColorBarRightAxis()  {
    return this->colorBarRightAxis;
}

JKQTPHorizontalIndependentAxis *JKQTPMathImage::getColorBarTopAxis()  {
    return this->colorBarTopAxis;
}

JKQTPVerticalIndependentAxis *JKQTPMathImage::getModifierColorBarTopAxis()  {
    return this->modifierColorBarTopAxis;
}

JKQTPHorizontalIndependentAxis *JKQTPMathImage::getModifierColorBarRightAxis()   {
    return this->modifierColorBarRightAxis ;
}

const JKQTPVerticalIndependentAxis *JKQTPMathImage::getColorBarRightAxis() const {
    return this->colorBarRightAxis;

}

const JKQTPHorizontalIndependentAxis *JKQTPMathImage::getColorBarTopAxis() const {
    return this->colorBarTopAxis;
}

const JKQTPVerticalIndependentAxis *JKQTPMathImage::getModifierColorBarTopAxis() const {
    return this->modifierColorBarTopAxis;
}

const JKQTPHorizontalIndependentAxis *JKQTPMathImage::getModifierColorBarRightAxis() const {
    return this->modifierColorBarRightAxis ;
}

void JKQTPMathImage::setColorBarTopVisible(bool __value)
{
    this->colorBarTopVisible = __value;
}

bool JKQTPMathImage::getColorBarTopVisible() const
{
    return this->colorBarTopVisible;
}

void JKQTPMathImage::setColorBarRightVisible(bool __value)
{
    this->colorBarRightVisible = __value;
}

bool JKQTPMathImage::getColorBarRightVisible() const
{
    return this->colorBarRightVisible;
}

void JKQTPMathImage::setAutoModifierRange(bool __value)
{
    this->autoModifierRange = __value;
}

bool JKQTPMathImage::getAutoModifierRange() const
{
    return this->autoModifierRange;
}

void JKQTPMathImage::setModifierMin(double __value)
{
    this->modifierMin = __value;
}

double JKQTPMathImage::getModifierMin() const
{
    return this->modifierMin;
}

void JKQTPMathImage::setModifierMax(double __value)
{
    this->modifierMax = __value;
}

double JKQTPMathImage::getModifierMax() const
{
    return this->modifierMax;
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
    palette=static_cast<JKQTPMathImageColorPalette>(pal);
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
    JKQTPColumnMathImage(parent->getPlotter())
{
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

void JKQTPColumnMathImage::setImageColumn(int __value)
{
    this->imageColumn = __value;
}

int JKQTPColumnMathImage::getImageColumn() const
{
    return this->imageColumn;
}

void JKQTPColumnMathImage::setModifierColumn(int __value)
{
    this->modifierColumn = __value;
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
        this->datatype=JKQTPMathImageBase::DoubleArray;
    } else {
        this->datatype=JKQTPMathImageBase::DoubleArray;
        this->data=parent->getDatastore()->getColumnPointer(imageColumn,0);
        this->Ny=parent->getDatastore()->getRows(imageColumn)/this->Nx;
    }
    if (this->Nx==0 || modifierColumn<0 || !parent->getDatastore()->getColumnPointer(modifierColumn,0)) {
        this->dataModifier=nullptr;
    } else {
        this->datatypeModifier=JKQTPMathImageBase::DoubleArray;
        this->dataModifier=parent->getDatastore()->getColumnPointer(modifierColumn,0);
    }
}

