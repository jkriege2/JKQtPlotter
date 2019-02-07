/***************************************************************************
    File                 : EmfEngine.h
    Project              : EmfEngine
    --------------------------------------------------------------------
	Copyright            : (C) 2009 - 2010 by Ion Vasilief
    Email (use @ for *)  : ion_vasilief*yahoo.fr
    Description          : Enables the export of QPainter grafics to
						   Windows Enhanced Metafiles (.emf)
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2.1 of the License, or      *
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

#ifndef EMFENGINE_H
#define EMFENGINE_H

#include <QPaintDevice>
#include <QPaintEngine>

#ifdef Q_WS_WIN
	#include <windows.h>
#endif

#ifdef EMFENGINE_DLL
#  ifdef EMFENGINE_DLL_BUILD
#    define EMFENGINEJKQTP_LIB_EXPORT Q_DECL_EXPORT
#  else
#    define EMFENGINEJKQTP_LIB_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define EMFENGINEJKQTP_LIB_EXPORT
#endif

#ifdef HAVE_GDIPLUS
	#include <gdiplus.h>
	using namespace Gdiplus;
#else
	#ifdef Q_WS_X11
		#include <emf.h>
	#endif

	#ifdef Q_WS_MAC
		#include <emf.h>
	#endif

	#undef min
	#undef max
#endif

class EmfPaintEngine : public QPaintEngine
{
public:
	EmfPaintEngine(const QString&);
	~EmfPaintEngine(){};
	virtual bool begin(QPaintDevice*);
	virtual bool end();
	virtual void updateState( const QPaintEngineState & ) {};
	virtual void drawEllipse(const QRectF &);
	virtual void drawImage(const QRectF& r, const QImage& im, const QRectF& sr) {drawPixmap(r,QPixmap::fromImage(im),sr);};
	virtual QPaintEngine::Type type() const {return QPaintEngine::User;};
	virtual void drawPoints ( const QPointF * points, int pointCount );
	virtual void drawLines ( const QLineF * , int );
	virtual void drawPath ( const QPainterPath & path );
	virtual void drawPolygon ( const QPointF * , int , PolygonDrawMode );
	virtual void drawTextItem ( const QPointF & , const QTextItem & );
	virtual void drawRects ( const QRectF * , int );
	virtual void drawPixmap ( const QRectF &, const QPixmap &, const QRectF &);
	virtual void drawTiledPixmap(const QRectF &, const QPixmap &, const QPointF &);
	virtual void drawImage(const QRectF &, const QImage &, const QRectF &, Qt::ImageConversionFlags);

#ifdef HAVE_GDIPLUS
	Graphics *graphicsObject(){return d_grx;};
	GraphicsPath *convertPath (const QPainterPath & path);
#endif

private:
#ifdef HAVE_GDIPLUS
	Metafile *metafile;
	Graphics *d_grx;
	GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    //! Converts a QPen to a GDI+ Pen structure
    Pen *convertPen(const QPen& pen);
    //! Converts a QBrush to a GDI+ Brush structure
    Brush *convertBrush(const QBrush& brush, GraphicsPath *origPath = nullptr);
    //! Draws pixmap pix in a given rectangle
    void drawPixmap(const QPixmap &pix, const RectF &p);
    //! Constructs a texture brush based on pixmap pix
    TextureBrush *textureBrush(const QPixmap & pix);
#else
	//! Converts a QPen to a HPEN structure
	HPEN convertPen(const QPen&);
	//! Converts a QBrush to a HBRUSH structure
	HBRUSH convertBrush(const QBrush&);
	//! Pointer to the metafile device context
	HDC metaDC;
	RECT d_rect;
#endif
	//! Name of the output metafile
	QString fname;

	void setClipping();
	void resetClipping();
};

class EMFENGINEJKQTP_LIB_EXPORT EmfPaintDevice : public QPaintDevice
{
public:
	EmfPaintDevice(const QSize& s, const QString& fileName);
	~EmfPaintDevice();
	virtual QPaintEngine * paintEngine () const;

protected:
	virtual int metric ( PaintDeviceMetric ) const;

private:
    //! Size in pixels
    QSize d_size;
	EmfPaintEngine* engine;
};
#endif
