/***************************************************************************
    File                 : EmfPaintDevice.cpp
    Project              : EmfEngine
    --------------------------------------------------------------------
	Copyright            : (C) 2009 -2010 by Ion Vasilief
    Email (use @ for *)  : ion_vasilief*yahoo.fr
    Description          : Enables the export of QPainter grafics to
						   Windows Enhanced Metafiles (.emf) by using GDI calls
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
#include "EmfEngine.h"

#include <QApplication>
#include <QDesktopWidget>

EmfPaintDevice::EmfPaintDevice(const QSize& s, const QString& fileName) : QPaintDevice(),
d_size(s)
{
	engine = new EmfPaintEngine(fileName);
}

EmfPaintDevice::~EmfPaintDevice()
{
	delete engine;
}

QPaintEngine * EmfPaintDevice::paintEngine () const
{
	return engine;
}

int EmfPaintDevice::metric ( PaintDeviceMetric metric ) const
{
	int xdpi = QApplication::desktop()->logicalDpiX();
	int ydpi = QApplication::desktop()->logicalDpiY();

	switch (metric){
		case QPaintDevice::PdmWidth:
			return d_size.width();
		case QPaintDevice::PdmHeight:
			return d_size.height();
		case QPaintDevice::PdmWidthMM:
			return int(25.4*d_size.width()/(double)xdpi);
		case QPaintDevice::PdmHeightMM:
			return int(25.4*d_size.height()/(double)ydpi);
		case QPaintDevice::PdmNumColors:
			return 65536;//should it be millions?
		case QPaintDevice::PdmDepth:
			return 32;
		case QPaintDevice::PdmDpiX:
		case QPaintDevice::PdmPhysicalDpiX:
			return xdpi;
		case QPaintDevice::PdmDpiY:
		case QPaintDevice::PdmPhysicalDpiY:
			return ydpi;
		default:
			qWarning ("EmfPaintDevice::Strange metric asked");
			return 0;
	}
}
