/***************************************************************************
    File                 : QTeXPaintDevice.cpp
    Project              : QTeXEngine GNU GPL v. 3.0
    --------------------------------------------------------------------
    Copyright            : (C) 2009 by Ion Vasilief
    Email (use @ for *)  : ion_vasilief*yahoo.fr
    Description          : Enables the export of QPainter grafics to .tex files
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/

#include "QTeXEngine.h"

#include <QApplication>
#include <QDesktopWidget>

QTeXPaintDevice::QTeXPaintDevice(const QString& fileName, const QSize& s, Unit u)
: QPaintDevice()
{
	d_size = s;

	if (!d_size.isValid())
		d_size = QSize(500, 400);

	engine = new QTeXPaintEngine(fileName, u);
}

QTeXPaintDevice::~QTeXPaintDevice()
{
	delete engine;
}

QPaintEngine * QTeXPaintDevice::paintEngine () const
{
	return engine;
}

void QTeXPaintDevice::setColorMode(QPrinter::ColorMode mode)
{
	engine->setGrayScale(mode == QPrinter::GrayScale);
}

void QTeXPaintDevice::setOutputMode(OutputMode mode)
{
	engine->setOutputMode(mode);
}

void QTeXPaintDevice::setUnit(Unit u)
{
	engine->setUnit(u);
}

void QTeXPaintDevice::setDocumentMode(bool on)
{
	engine->setDocumentMode(on);
}

void QTeXPaintDevice::setEscapeTextMode(bool on)
{
	engine->setEscapeTextMode(on);
}

void QTeXPaintDevice::exportFontSizes(bool on)
{
	engine->exportFontSizes(on);
}

void QTeXPaintDevice::setTextHorizontalAlignment(Qt::Alignment alignment)
{
	engine->setTextHorizontalAlignment(alignment);
}

int QTeXPaintDevice::metric ( PaintDeviceMetric metric ) const
{
	QDesktopWidget *desktop = QApplication::desktop();
	int dpi_x = desktop->logicalDpiX();
	int dpi_y = desktop->logicalDpiY();
	switch (metric){
		case QPaintDevice::PdmWidth:
			return d_size.width();
		case QPaintDevice::PdmHeight:
			return d_size.height();
		case QPaintDevice::PdmWidthMM:
			return int(25.4*d_size.width()/(double)dpi_x);
		case QPaintDevice::PdmHeightMM:
			return int(25.4*d_size.height()/(double)dpi_y);
		case QPaintDevice::PdmNumColors:
			return 65536;//should it be millions?
		case QPaintDevice::PdmDepth:
			return 32;
		case QPaintDevice::PdmDpiX:
		case QPaintDevice::PdmPhysicalDpiX:
			return dpi_x;
		case QPaintDevice::PdmDpiY:
		case QPaintDevice::PdmPhysicalDpiY:
			return dpi_y;
		default:
			qWarning ("QTeXPaintDevice::Unknown metric asked");
			return 0;
	}
}
