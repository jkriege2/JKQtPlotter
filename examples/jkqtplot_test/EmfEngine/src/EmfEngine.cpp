/***************************************************************************
    File                 : EmfEngine.cpp
    Project              : EmfEngine
    --------------------------------------------------------------------
	Copyright            : (C) 2009 - 2010 by Ion Vasilief
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

EmfPaintEngine::EmfPaintEngine(const QString& f) : QPaintEngine(QPaintEngine::AllFeatures)
{
	fname = f;
}

bool EmfPaintEngine::begin(QPaintDevice* p)
{
	if (!p)
		return false;

	setPaintDevice(p);

	HWND desktop = GetDesktopWindow();
	HDC dc = GetDC(desktop);
	PCSTR description = "Metafile created\0with EmfEngine\0";

	metaDC = CreateEnhMetaFileA(dc, fname.toLocal8Bit().data(), &d_rect, description);//create null rectangle metafile

	d_rect.left = 0;
	d_rect.top = 0;
	d_rect.right = 100*p->width()*GetDeviceCaps(metaDC, HORZSIZE)/(double)GetDeviceCaps(metaDC, HORZRES);
	d_rect.bottom = 100*p->height()*GetDeviceCaps(metaDC, VERTSIZE)/(double)GetDeviceCaps(metaDC, VERTRES);

	end();//delete the dummy metafile

	metaDC = CreateEnhMetaFileA(dc, fname.toLocal8Bit().data(), &d_rect, description);

	SetWindowExtEx(metaDC, p->width(), p->height(), 0);
	SetViewportExtEx(metaDC, p->width(), p->height(), 0);

	return true;
}

bool EmfPaintEngine::end()
{
	HENHMETAFILE  metafile = CloseEnhMetaFile( metaDC );
	DeleteEnhMetaFile( metafile );
	DeleteDC( metaDC );
 	return true;
}

void EmfPaintEngine::drawPoints ( const QPointF * points, int pointCount )
{
	setClipping();

	QColor color = painter()->pen().color();
	HBRUSH wbrush = CreateSolidBrush(RGB(color.red(), color.green(), color.blue()));
	SelectObject(metaDC, wbrush);

	int lw = painter()->pen().width();
	QMatrix m = painter()->worldMatrix();
	for (int i = 0; i < pointCount; i++){
		QPointF p = m.map(points[i]);
		int x = qRound(p.x());
		int y = qRound(p.y());
		Rectangle(metaDC, x, y, x + lw, y + lw);
	}

	resetClipping();
	DeleteObject(wbrush);
}

void EmfPaintEngine::drawLines ( const QLineF * lines, int lineCount )
{
   setClipping();

	HPEN wpen = convertPen(painter()->pen());
	SelectObject(metaDC, wpen);

	QMatrix m = painter()->worldMatrix();

	for (int i = 0; i < lineCount; i++) {
		POINT *pts = new POINT[2];

		QPointF p1 = m.map(lines[i].p1());
		QPointF p2 = m.map(lines[i].p2());

		pts[0].x = qRound(p1.x());
		pts[0].y = qRound(p1.y());
		pts[1].x = qRound(p2.x());
		pts[1].y = qRound(p2.y());
		Polyline(metaDC, pts, 2);
		delete [] pts;
	}

	resetClipping();
	DeleteObject(wpen);
}

void EmfPaintEngine::drawPolygon ( const QPointF * points, int pointCount, PolygonDrawMode mode )
{
	setClipping();

	HPEN wpen = convertPen(painter()->pen());
	SelectObject(metaDC, wpen);
	HBRUSH wbrush = convertBrush(painter()->brush());
	SelectObject(metaDC, wbrush);

	POINT *pts = new POINT[pointCount];
	QMatrix m = painter()->worldMatrix();
	for (int i = 0; i < pointCount; i++){
		QPointF p = m.map (points[i]);
		pts[i].x = qRound(p.x());
		pts[i].y = qRound(p.y());
	}

	if (mode == QPaintEngine::PolylineMode)
		Polyline(metaDC, pts, pointCount);
	else if (mode == QPaintEngine::OddEvenMode)
		Polygon(metaDC, pts, pointCount);
	else
		qWarning("EmfEngine: drawPolygon with unsupported mode.\n");

	resetClipping();
	delete [] pts;
	DeleteObject(wpen);
	DeleteObject(wbrush);
}

void EmfPaintEngine::drawTextItem ( const QPointF & p, const QTextItem & textItem )
{
	setClipping();

	SetBkMode( metaDC, TRANSPARENT );

	QFont f = textItem.font();
	QFontMetrics fm(f);
	HFONT wfont = CreateFontA(fm.height() - 1, fm.averageCharWidth(), 0, 0,
				10*f.weight(), f.italic(), f.underline (), f.strikeOut(),
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH, f.family().toAscii().data());
	SelectObject( metaDC, wfont);

	QColor colour = painter()->pen().color();
	SetTextColor( metaDC, RGB(colour.red(), colour.green(), colour.blue()));

	QString text = textItem.text();
	int size = text.size();

	QMatrix m = painter()->worldMatrix();

	XFORM xf;
	xf.eM11 = m.m11();
	xf.eM12 = m.m12();
	xf.eM21 = m.m21();
	xf.eM22 = m.m22();
	xf.eDx = m.dx();
	xf.eDy = m.dy();
	SetWorldTransform(metaDC, &xf);

#ifdef Q_WS_WIN
	wchar_t *wtext = (wchar_t *)malloc(size*sizeof(wchar_t));
	if (!wtext){
		qWarning("EmfEngine: Not enough memory in drawTextItem().");
		return;
	}
	
	size = text.toWCharArray(wtext);
	TextOutW(metaDC, qRound(p.x()), qRound(p.y() - 0.85*fm.height()), wtext, size);
	free(wtext);
#else
	TextOutA(metaDC, qRound(p.x()), qRound(p.y() - 0.85*fm.height()), text.toLocal8Bit().data(), size);
#endif

	xf.eM11 = 1.0;
	xf.eM12 = 0.0;
	xf.eM21 = 0.0;
	xf.eM22 = 1.0;
	xf.eDx = 0.0;
	xf.eDy = 0.0;
	SetWorldTransform(metaDC, &xf);

	resetClipping();
	DeleteObject(wfont);
}

void EmfPaintEngine::drawRects ( const QRectF * rects, int rectCount )
{
	setClipping();

	HPEN wpen = convertPen(painter()->pen());
	SelectObject(metaDC, wpen);
	HBRUSH wbrush = convertBrush(painter()->brush());
	SelectObject(metaDC, wbrush);

	QMatrix m = painter()->worldMatrix();
	for (int i = 0; i < rectCount; i++){
		QRectF r = m.mapRect(rects[i]);
		Rectangle(metaDC, qRound(r.left()), qRound(r.top()), qRound(r.right()), qRound(r.bottom()));
	}

	resetClipping();
	DeleteObject(wpen);
	DeleteObject(wbrush);
}

void EmfPaintEngine::drawEllipse ( const QRectF & rect )
{
	setClipping();

	HPEN wpen = convertPen(painter()->pen());
	SelectObject(metaDC, wpen);
	HBRUSH wbrush = convertBrush(painter()->brush());
	SelectObject(metaDC, wbrush);

	QRectF r = painter()->worldMatrix().mapRect(rect);
	Ellipse(metaDC, qRound(r.left()), qRound(r.top()), qRound(r.right()), qRound(r.bottom()));

	resetClipping();
	DeleteObject(wpen);
	DeleteObject(wbrush);
}

void EmfPaintEngine::drawPath ( const QPainterPath & path )
{
	setClipping();

	int points = path.elementCount();
	POINT *pts = new POINT[points];
	BYTE *types = new BYTE[points];

	POINT *bzs = new POINT[3];
	int bez = 0;

	BeginPath(metaDC);

	QMatrix m = painter()->worldMatrix();
	for (int i = 0; i < points; i++){
		QPainterPath::Element el = path.elementAt(i);
		QPointF p = m.map(QPointF(el.x, el.y));
		int x = qRound(p.x());
		int y = qRound(p.y());
		pts[i].x = x;
		pts[i].y = y;

		switch(el.type){
			case QPainterPath::MoveToElement:
				types[i] = PT_MOVETO;
			#ifndef Q_WS_WIN
				MoveToEx (metaDC, x, y, 0);
			#endif
			break;

			case QPainterPath::LineToElement:
				types[i] = PT_LINETO;
			#ifndef Q_WS_WIN
				LineTo(metaDC, x, y);
			#endif
			break;

			case QPainterPath::CurveToElement:
				types[i] = PT_BEZIERTO;
			#ifndef Q_WS_WIN
				bzs[bez] = pts[i];
				bez++;
			#endif
			break;

			case QPainterPath::CurveToDataElement:
				types[i] = PT_BEZIERTO;
			#ifndef Q_WS_WIN
				bzs[bez] = pts[i];
				if (bez == 2){
					PolyBezierTo(metaDC, bzs, 3);
					bez = 0;
				} else
					bez++;
			#endif
			break;
		}
	}

	HPEN wpen = convertPen(painter()->pen());
	SelectObject(metaDC, wpen);
#ifdef Q_WS_WIN
	PolyDraw(metaDC, pts, types, points);
#else
	StrokePath(metaDC);
#endif

	HBRUSH wbrush = convertBrush(painter()->brush());
	SelectObject(metaDC, wbrush);

	EndPath(metaDC);

	if(QPoint(pts[0].x, pts[0].y) == QPoint(pts[points - 1].x, pts[points - 1].y))
		StrokeAndFillPath(metaDC);
	else {
		FillPath(metaDC);
	#ifdef Q_WS_WIN
		PolyDraw(metaDC, pts, types, points);
	#else
		StrokePath(metaDC);
	#endif
	}

	resetClipping();
	DeleteObject(wbrush);
	DeleteObject(wpen);
	delete [] pts;
	delete [] types;
}

void EmfPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &)
{
	setClipping();

	QMatrix m = painter()->worldMatrix();
	QPointF p = m.map(r.topLeft());
	int x = qRound(p.x());
	int y = qRound(p.y());
	int width = qRound(r.width());
	int height = qRound(r.height());

#ifdef Q_WS_WIN
	HBITMAP hbtmp = nullptr;
	DWORD op = SRCCOPY;
	if (pm.hasAlpha()){
		QImage image = pm.scaled(width, height).toImage();
		image.invertPixels();
		hbtmp = QPixmap::fromImage (image).toWinHBITMAP();
		op = SRCINVERT;
	} else
		hbtmp = pm.scaled(width, height).toWinHBITMAP();

	HDC hDC = CreateCompatibleDC(metaDC);
    SelectObject(hDC, hbtmp);

    BitBlt(metaDC, x, y, width, height, hDC, 0, 0, op);
    DeleteObject(hbtmp);
    DeleteDC(hDC);
#else
	QImage image = pm.scaled(width, height).toImage();
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			QRgb rgb = image.pixel(i, j);
			if (qAlpha(rgb) == 255)
				SetPixel(metaDC, x + i, y + j, RGB(qRed(rgb), qGreen(rgb), qBlue(rgb)));
		}
	}
#endif
	resetClipping();
}

void EmfPaintEngine::drawTiledPixmap(const QRectF &r, const QPixmap & pix, const QPointF &)
{
	setClipping();

#ifdef Q_WS_WIN
	HBITMAP hBmp = pix.toWinHBITMAP();
	HBRUSH wbrush = CreatePatternBrush(hBmp);

	QMatrix m = painter()->worldMatrix();
	QRectF dr = m.mapRect(r);

	RECT rect;
	rect.left = qRound(dr.left());
	rect.top = qRound(dr.top());
	rect.right = qRound(dr.right());
	rect.bottom = qRound(dr.bottom());

	FillRect(metaDC, &rect, wbrush);

	DeleteObject(hBmp);
	DeleteObject(wbrush);
#else
	int width = qRound(r.width());
	int height = qRound(r.height());

	QPixmap pixmap(width, height);
	QPainter p(&pixmap);
	p.drawTiledPixmap(0, 0, width, height, pix);
	p.end();

	drawPixmap(r, pixmap, QRectF());
#endif

	resetClipping();
}

void EmfPaintEngine::drawImage(const QRectF & r, const QImage & image, const QRectF &, Qt::ImageConversionFlags flags)
{
	QMatrix m = painter()->worldMatrix();
	QPointF p = m.map(r.topLeft());
	int x = qRound(p.x());
	int y = qRound(p.y());
	int width = qRound(r.width());
	int height = qRound(r.height());

#ifdef Q_WS_WIN
	setClipping();
	QPixmap pix = QPixmap::fromImage (image.scaled(width, height), flags);

	HBITMAP hbtmp = pix.toWinHBITMAP();
	HDC hDC = CreateCompatibleDC(metaDC);
    SelectObject(hDC, hbtmp);
    BitBlt(metaDC, x, y, width, height, hDC, 0, 0, SRCCOPY);
    DeleteObject(hbtmp);
    DeleteDC(hDC);

	resetClipping();
#else
	QImage imag = image.scaled(width, height);
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			QRgb rgb = imag.pixel(i, j);
			if (qAlpha(rgb) == 255)
				SetPixel(metaDC, x + i, y + j, RGB(qRed(rgb), qGreen(rgb), qBlue(rgb)));
		}
	}
#endif
}

void EmfPaintEngine::setClipping()
{
#ifdef Q_WS_WIN
	if (painter()->hasClipping()) {
		QRect rect = painter()->clipRegion().boundingRect();
		HRGN hrgn = CreateRectRgn(rect.left(), rect.top(), rect.right(), rect.bottom());
		SelectClipRgn(metaDC, hrgn);
		DeleteObject(hrgn);
	}
#endif
}

void EmfPaintEngine::resetClipping()
{
#ifdef Q_WS_WIN
	if (painter()->hasClipping())
		SelectClipRgn(metaDC, nullptr);
#endif
}

HPEN EmfPaintEngine::convertPen(const QPen& pen)
{
	INT style = PS_nullptr;
	switch (pen.style()){
		case Qt::SolidLine:
			style = PS_SOLID;
		break;

		case Qt::DashLine:
			style = PS_DASH;
		break;

		case Qt::DotLine:
			style = PS_DOT;
		break;

		case Qt::DashDotLine:
			style = PS_DASHDOT;
		break;

		case Qt::DashDotDotLine:
			style = PS_DASHDOTDOT;
		break;

		default:
		break;
	}

	INT capStyle = PS_ENDCAP_FLAT;
	switch (pen.capStyle()){
		case Qt::FlatCap:
		break;

		case Qt::SquareCap:
			capStyle = PS_ENDCAP_SQUARE;
		break;

		case Qt::RoundCap:
			capStyle = PS_ENDCAP_ROUND;
		break;

		default:
			break;
	}

	INT joinStyle = PS_JOIN_MITER;
	switch (pen.joinStyle()){
		case Qt::MiterJoin:
		break;

		case Qt::BevelJoin:
			joinStyle = PS_JOIN_BEVEL;
		break;

		case Qt::RoundJoin:
			joinStyle = PS_JOIN_ROUND;
		break;

		case Qt::SvgMiterJoin:
			joinStyle = PS_JOIN_MITER;
		break;

		default:
			break;
	}

	LOGBRUSH lbrush = {BS_SOLID, RGB(pen.color().red(),pen.color().green(),pen.color().blue()), 0};
	return ExtCreatePen(PS_GEOMETRIC | style | capStyle | joinStyle, pen.width(), &lbrush, 0, nullptr);
}

HBRUSH EmfPaintEngine::convertBrush(const QBrush& brush)
{
	LOGBRUSH lbrush = {BS_nullptr, 0, 0};

	if (!brush.color().alpha())
		return CreateBrushIndirect( &lbrush );

	if (brush.color().alpha() < 255){//semi-transparent brush color
		qWarning ("Semi-transparent brushes are not supported by EmfEngine.");

		#ifdef Q_WS_WIN
		QPixmap pix(4, 4);
		pix.fill(Qt::white);
		QPainter p;
		p.begin(&pix);
		p.setPen(QPen(Qt::NoPen));
		p.setBrush(brush);
		p.drawRect(QRect(0, 0, 4, 4));
		p.end();

		HBITMAP hBmp = pix.toWinHBITMAP();
		HBRUSH wbrush = CreatePatternBrush(hBmp);
		DeleteObject(hBmp);
		return wbrush;
		#endif
	}

	LONG lbHatch = HS_HORIZONTAL;
	UINT lbStyle = BS_HATCHED;
	switch(brush.style()){
		case Qt::NoBrush:
			lbStyle = BS_nullptr;
		break;

		case Qt::SolidPattern:
			lbStyle = BS_SOLID;
		break;

		case Qt::Dense1Pattern:
		case Qt::Dense2Pattern:
		case Qt::Dense3Pattern:
		case Qt::Dense4Pattern:
		case Qt::Dense5Pattern:
		case Qt::Dense6Pattern:
		case Qt::Dense7Pattern:
		{
		#ifdef Q_WS_WIN
			QPixmap pix(4, 4);
			pix.fill(Qt::white);
			QPainter p;
			p.begin(&pix);
			p.setPen(QPen(Qt::NoPen));
			p.setBrush(brush);
			p.drawRect(QRect(0, 0, 4, 4));
			p.end();

			HBITMAP hbm = pix.toWinHBITMAP();
			HBRUSH wbrush = CreatePatternBrush(hbm);
			DeleteObject(hbm);
			return wbrush;
		#endif
		}
		break;

		case Qt::HorPattern:
		break;

		case Qt::VerPattern:
			lbHatch = HS_VERTICAL;
		break;

		case Qt::CrossPattern:
			lbHatch = HS_CROSS;
		break;

		case Qt::BDiagPattern:
			lbHatch = HS_BDIAGONAL;
		break;

		case Qt::FDiagPattern:
			lbHatch = HS_FDIAGONAL;
		break;

		case Qt::DiagCrossPattern:
			lbHatch = HS_DIAGCROSS;
		break;

		case Qt::LinearGradientPattern:
		{
			qWarning("EmfEngine: Qt::LinearGradientPattern is not supported.");
			return CreateBrushIndirect( &lbrush );
		}
		break;

		case Qt::ConicalGradientPattern:
		{
			qWarning("EmfEngine: Qt::ConicalGradientPattern is not supported.");
			return CreateBrushIndirect( &lbrush );
		}
		break;

		case Qt::RadialGradientPattern:
		{
			qWarning("EmfEngine: Qt::RadialGradientPattern is not supported.");
			return CreateBrushIndirect( &lbrush );
		}

		case Qt::TexturePattern:
		#ifdef Q_WS_WIN
		{
			HBITMAP hbm = brush.texture().toWinHBITMAP();
			HBRUSH wbrush = CreatePatternBrush(hbm);
			DeleteObject(hbm);
			return wbrush;
		}
		#endif
		break;

		default:
			lbStyle = BS_SOLID;
		break;
	}

	LOGBRUSH logbrush = {lbStyle, RGB(brush.color().red(), brush.color().green(), brush.color().blue()), lbHatch};
	return CreateBrushIndirect( &logbrush );
}
