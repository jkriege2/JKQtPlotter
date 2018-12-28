#ifndef JKQTPEMFENGINEADAPTER_H
#define JKQTPEMFENGINEADAPTER_H

#include "EmfEngine/src/EmfEngine.h"
#include <QString>
#include <QStringList>
#include "../../../jkqtpbaseplotter.h"

class JKQTPEmfEngineAdapter : public JKQtBasePlotter::JKQTPPaintDeviceAdapter
{
    public:
        JKQTPEmfEngineAdapter();

        virtual QString getFilter() const;
        virtual QString getFormatName() const;
        virtual QString getFormatID() const;
        virtual QStringList getFileExtension() const;
        virtual bool getSetAbsolutePaperSize() const;
        virtual double getPrintSizeXInMM() const ;
        virtual double getPrintSizeYInMM() const ;
        virtual bool isPrinter() const;
        virtual QPaintDevice* createPaintdevice(const QString& filename, int widthPix, int heightPix) const;

};

#endif // JKQTPEMFENGINEADAPTER_H
