#include "jkqtpemfengineadapter.h"

JKQTPEmfEngineAdapter::JKQTPEmfEngineAdapter()
{
}

QString JKQTPEmfEngineAdapter::getFilter() const
{
    return QObject::tr("Enhanced Metafile (*.emf)");
}

QString JKQTPEmfEngineAdapter::getFormatName() const
{
    return QObject::tr("EMF");
}

QString JKQTPEmfEngineAdapter::getFormatID() const
{
    return QObject::tr("EMF");
}

QStringList JKQTPEmfEngineAdapter::getFileExtension() const
{
    return QStringList("emf");
}

bool JKQTPEmfEngineAdapter::getSetAbsolutePaperSize() const
{
    return true;
}

double JKQTPEmfEngineAdapter::getPrintSizeXInMM() const
{
    return -1;
}

double JKQTPEmfEngineAdapter::getPrintSizeYInMM() const
{
    return -1;
}

bool JKQTPEmfEngineAdapter::isPrinter() const
{
    return false;
}

QPaintDevice *JKQTPEmfEngineAdapter::createPaintdevice(const QString &filename, int widthPix, int heightPix) const
{
    qDebug()<<"create EMF-device for "<<filename<<QSize(widthPix, heightPix);
    EmfPaintDevice* emf=new EmfPaintDevice(QSize(widthPix, heightPix), filename);
    return emf;
}
