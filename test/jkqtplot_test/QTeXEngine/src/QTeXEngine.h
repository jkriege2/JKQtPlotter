/***************************************************************************
    File                 : QTeXEngine.h
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

#ifndef Q_TEX_ENGINE_H
#define Q_TEX_ENGINE_H

#include <QPaintDevice>
#include <QPaintEngine>
#include <QPrinter>

class QFile;
class QTeXPaintEngine;

class QTeXPaintDevice : public QPaintDevice
{
public:
	enum Unit{pt, bp, mm, cm, in, ex, em};
	enum OutputMode{Tikz, Pgf};

	QTeXPaintDevice(const QString& fileName, const QSize& s = QSize(), Unit u = pt);
	~QTeXPaintDevice();

	virtual QPaintEngine * paintEngine () const;
	//! Set color mode (Color or GrayScale)
	void setColorMode(QPrinter::ColorMode mode);
	//! Set output mode (Tikz or Pgf)
	void setOutputMode(OutputMode mode);
	//! Set length unit
	void setUnit(Unit u);
	//! Set size
	void setSize(const QSize& s){d_size = s;};
	//! Enables/Disables document tags
	void setDocumentMode(bool on = true);
	//! Enables/Disables escaping of special characters in texts
	void setEscapeTextMode(bool on = true);
	//! Enables/Disables exporting of font sizes
	void exportFontSizes(bool on = true);
	//! Set horizontal alignment
	void setTextHorizontalAlignment(Qt::Alignment alignment);

protected:
	virtual int metric ( PaintDeviceMetric ) const;

private:
    //! Size in pixels
    QSize d_size;
	QTeXPaintEngine* engine;
};

class QTeXPaintEngine : public QPaintEngine
{
public:
	QTeXPaintEngine(const QString&, QTeXPaintDevice::Unit u = QTeXPaintDevice::pt);
	~QTeXPaintEngine(){};
	virtual bool begin(QPaintDevice*);
	virtual bool end();
	virtual void updateState( const QPaintEngineState & ) {};
	virtual void drawEllipse(const QRectF &);
	virtual QPaintEngine::Type type() const {return QPaintEngine::User;};
	virtual void drawPoints ( const QPointF * points, int pointCount );
	virtual void drawLines ( const QLineF * , int );
	virtual void drawPath ( const QPainterPath & path );
	virtual void drawPolygon ( const QPointF * , int , PolygonDrawMode );
	virtual void drawTextItem ( const QPointF & , const QTextItem & );
	virtual void drawRects ( const QRectF * , int );
	virtual void drawPixmap ( const QRectF &, const QPixmap &, const QRectF &);
	virtual void drawImage(const QRectF &, const QImage &, const QRectF &, Qt::ImageConversionFlags);

	//! Set length unit
	void setUnit(QTeXPaintDevice::Unit u){d_unit = u;};
	//! Enables/Disables gray scale output
	void setGrayScale(bool on = true){d_gray_scale = on;};
	//! Set output syntax
	void setOutputMode(QTeXPaintDevice::OutputMode mode){d_pgf_mode = (mode == QTeXPaintDevice::Pgf) ? true : false;};
	void setDocumentMode(bool on = true){d_document_mode = on;};
	//! Enables/Disables escaping of special characters in texts
	void setEscapeTextMode(bool on = true){d_escape_text = on;};
	void exportFontSizes(bool on = true){d_font_size = on;};
	void setTextHorizontalAlignment(Qt::Alignment alignment){d_horizontal_alignment = alignment;};

private:
	enum Shape{Line, Polygon, Polyline, Rect, Ellipse, Path, Points};
	//! Returns true if draw operation has NoBrush and NoPen
	bool emptyStringOperation();
	QString unit();
	double unitFactor();
	double resFactorX();
	double resFactorY();

	QString pgfPoint(const QPointF& p);
	QString tikzPoint(const QPointF& p);

	QPointF convertPoint(const QPointF& p);
	QString color(const QColor& col);
	QString defineColor(const QColor& c, const QString& name);

	QString pgfPen(const QPen& pen);
	QString tikzPen(const QPen& pen);

	QString pgfBrush(const QBrush& brush);
	QString tikzBrush(const QBrush& brush);

	QString beginScope();
	QString endScope();

	QString clipPath();
	bool changedClipping();

	QString path(const QPainterPath & path);
	QString pgfPath(const QPainterPath & path);
	QString tikzPath(const QPainterPath & path);

	QString drawShape(Shape shape, const QString & path);
	QString drawPgfShape(Shape shape, const QString & path);
	QString drawTikzShape(Shape shape, const QString & path);

	//! Draws pixmap pix in a given rectangle
    void drawPixmap(const QPixmap &pix, const QRectF &p);
    void writeToFile(const QString& s);
    QString indentString(const QString& s);
    //! Returns true if a new color command should be added
    bool addNewBrushColor();
    bool addNewPatternColor();
    bool addNewPenColor();

	QFile *file;
	//! Name of the output file
	QString fname;
	int d_pixmap_index;
	bool d_pgf_mode;
	bool d_open_scope;
	bool d_gray_scale;
	bool d_document_mode;
	bool d_escape_text;
	bool d_font_size;
	QPainterPath d_clip_path;
	QColor d_current_color, d_pattern_color;
	QTeXPaintDevice::Unit d_unit;
	Qt::Alignment d_horizontal_alignment;
};
#endif
