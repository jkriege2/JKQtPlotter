/***************************************************************************
    File                 : QTeXPaintEngine.cpp
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

#include <math.h>
#include <QDir>
#include <QTextStream>
#include <QDateTime>

QTeXPaintEngine::QTeXPaintEngine(const QString& f, QTeXPaintDevice::Unit u)
: QPaintEngine(QPaintEngine::AllFeatures),
fname(f),
d_pixmap_index(1),
d_unit(u)
{
	d_pgf_mode = false;
	d_open_scope = false;
	d_gray_scale = false;
	d_document_mode = false;
	d_escape_text = true;
	d_font_size = true;
	d_clip_path = QPainterPath();
	d_current_color = QColor();
	d_pattern_color = QColor();
	d_horizontal_alignment = Qt::AlignHCenter;
}

bool QTeXPaintEngine::begin(QPaintDevice* p)
{
	setPaintDevice(p);
	file = new QFile(fname);
	if (file->open(QIODevice::WriteOnly)){
		QTextStream t(file);
		t.setCodec("UTF-8");
		if (d_document_mode){
			t << "\\documentclass{article}\n";
			t << "\\usepackage[left=0.2cm,top=0.1cm,right=0.2cm,nohead,nofoot]{geometry}\n";

			if (d_pgf_mode){
				t << "\\usepackage{pgf}\n";
				t << "\\usepgflibrary{patterns}\n";
			} else {
				t << "\\usepackage{tikz}\n";
				t << "\\usetikzlibrary{patterns}\n";
			}

			t << "\\usepackage{ulem}\n";//used for striked out fonts (\sout command)
			t << "\\begin{document}\n";
		}

		QString pictureEnv = "\\begin{tikzpicture}{0";
		if (d_pgf_mode)
			pictureEnv = "\\begin{pgfpicture}{0";

		QString u = unit();
		t << pictureEnv + u + "}{0" + u + "}{";
		t << QString::number(p->width()) + u + "}{";
		t << QString::number(p->height()) + u + "}\n";

		if (!d_pgf_mode){
			QPainterPath path;
			path.addRect(QRect(0, 0, p->width(), p->height()));
			t << "\t\\clip" + tikzPath(path);
		}
		return true;
	}

	delete file;
	return false;
}

bool QTeXPaintEngine::end()
{
	QTextStream t(file);
	t.setCodec("UTF-8");
	if (d_open_scope)
		t << endScope();

	if (d_pgf_mode)
		t << "\\end{pgfpicture}\n";
	else
		t << "\\end{tikzpicture}\n";

	if (d_document_mode)
		t << "\\end{document}\n";

	file->close();
	return true;
}

void QTeXPaintEngine::drawPoints ( const QPointF * points, int pointCount )
{
	if (emptyStringOperation())
		return;

	QMatrix m = painter()->worldMatrix();
	double lw = painter()->pen().widthF();

	QString s = QString::null;
	if (addNewPenColor()){
		d_current_color = painter()->pen().color();
		s = color(d_current_color);
	}

	for (int i = 0; i < pointCount; i++){
		QPointF p = m.map(points[i]);
		if (d_pgf_mode){
			QString path = pgfPoint(convertPoint(p));
			path += pgfPoint(QPointF(lw, lw)) + "\n";
			s += "\\pgfrect[fill]" + path;
		} else {
			QString path = tikzPoint(convertPoint(p));
			path += " rectangle " + tikzPoint(convertPoint(QPointF(p.x() + lw, p.y() + lw))) + ";\n";
			s += "\\fill " + path;
		}
	}

	writeToFile(s);
}

void QTeXPaintEngine::drawLines ( const QLineF * lines, int lineCount )
{
	if (painter()->pen().style() == Qt::NoPen)
		return;

	QString s;
	for (int i = 0; i < lineCount; i++){
		QPainterPath path(lines[i].p1());
		path.lineTo(lines[i].p2());

		s += drawShape(Line, this->path(path));
	}

	writeToFile(s);
}

void QTeXPaintEngine::drawPolygon ( const QPointF * points, int pointCount, PolygonDrawMode mode )
{
	if (emptyStringOperation())
		return;

	QVector<QPointF> pts;
	for (int i = 0; i < pointCount; i++)
		pts << points[i];

	QPainterPath path;
	path.addPolygon(QPolygonF(pts));
	if (mode != QPaintEngine::PolylineMode)
		path.closeSubpath ();

	QString s;;
	if (mode != QPaintEngine::PolylineMode){
		path.closeSubpath ();
		s += drawShape(Polygon, this->path(path));
	} else
		s += drawShape(Polyline, this->path(path));

	writeToFile(s);
}

void QTeXPaintEngine::drawTextItem ( const QPointF & p, const QTextItem & textItem )
{
	QString s = QString::null;
	if (addNewPenColor()){
		s = color(painter()->pen().color());
		d_current_color = painter()->pen().color();
	}

	QMatrix m = painter()->worldMatrix();

	s += "\\pgftext[";

	QPointF origin = p;
	switch(d_horizontal_alignment){
		case Qt::AlignLeft:
			s += "left";
		break;

		case Qt::AlignHCenter:
		case Qt::AlignJustify:
			origin = QPointF(p.x() + 0.5*textItem.width(), p.y());
			s += "center";
		break;

		case Qt::AlignRight:
			origin = QPointF(p.x() + textItem.width(), p.y());
			s += "right";
		break;

		default:
		break;
	}

	s += ", base, at=";
	s += pgfPoint(convertPoint(m.map(origin)));

	if (painter()->transform().isRotating ()){
		double angle = 180.0/M_PI*acos(m.m11());
		if (m.m11() != 0.0 && m.m12() > 0)
			angle = -angle;
		s += ",rotate=" + QString::number(angle);
	}
	s += "]{";

	QFont f = textItem.font();
	if (d_font_size)
		s += "\\fontsize{" + QString::number(int(f.pointSizeF())) + "}{0}\\selectfont{";

	if (f.underline())
		s += "\\underline{";
	if (f.italic())
		s += "\\textit{";
	if (f.bold())
		s += "\\textbf{";
	if (f.strikeOut())
		s += "\\sout{";

	QString text = textItem.text();
	text.remove(QRegExp("~\\"));
	if (d_escape_text){
		text.replace("$", "\\$");
		text.replace("_", "\\_");
		text.replace("{", "\\{");
		text.replace("}", "\\}");
		text.replace("^", "\\^");
		text.replace("&", "\\&");
		text.replace("%", "\\%");
		text.replace("#", "\\#");
	}

	s += text;
	if (d_font_size)
		s += "}";

	if (f.italic())
		s += "}";
	if (f.bold())
		s += "}";
	if (f.underline())
		s += "}";
	if (f.strikeOut())
		s += "}";

	s += "}\n";

	writeToFile(s);
}

void QTeXPaintEngine::drawRects ( const QRectF * rects, int rectCount )
{
	if (emptyStringOperation())
		return;

	QString s;
	for (int i = 0; i < rectCount; i++){
		QPainterPath path;
		path.addPolygon(QPolygonF(rects[i]));
		s += drawShape(Path, this->path(path));
	}

	writeToFile(s);
}

void QTeXPaintEngine::drawEllipse ( const QRectF & rect )
{
	if (emptyStringOperation())
		return;

	QPointF p = painter()->worldMatrix().map(rect.bottomLeft());

	QString path;
	if (d_pgf_mode){
		path = pgfPoint(convertPoint(QPointF(p.x() + 0.5*rect.width(), p.y() - 0.5*rect.height())));
		path += pgfPoint(QPointF(0, 0.5*rect.height()*resFactorY()));
		path += pgfPoint(QPointF(0.5*rect.width()*resFactorX(), 0)) + "\n";
	} else {
		path = tikzPoint(convertPoint(QPointF(p.x() + 0.5*rect.width(), p.y() - 0.5*rect.height())));
		path += " ellipse (";
		QString u = unit();
		path += QString::number(0.5*rect.width()*resFactorX()) + u + " and ";
		path += QString::number(0.5*rect.height()*resFactorY()) + u + ");\n";
	}

	writeToFile(drawShape(Ellipse, path));
}

void QTeXPaintEngine::drawPath ( const QPainterPath & path )
{
	if (emptyStringOperation())
		return;

	writeToFile(drawShape(Path, this->path(path)));
}

QString QTeXPaintEngine::drawPgfShape(Shape shape, const QString & path)
{
	if (path.isEmpty())
		return QString::null;

	QString stroke_command = path + "\\pgfstroke\n";
	QString fill_command = path + "\\pgffill\n";
	switch(shape){
		case Line:
		case Polygon:
		case Polyline:
		case Path:
		case Points:
		break;

		case Rect:
			stroke_command = "\\pgfrect[stroke]" + path;
			fill_command = "\\pgfrect[fill]" + path;
		break;

		case Ellipse:
			stroke_command = "\\pgfellipse[stroke]" + path;
			fill_command = "\\pgfellipse[fill]" + path;
		break;
	}

	QString s = QString::null;
	if (shape != Line && shape != Polyline && painter()->brush().style() != Qt::NoBrush){
		// fill the background
		s += pgfBrush(painter()->brush());
		s += fill_command;
	}

	if (painter()->pen().style() != Qt::NoPen){// draw the contour
		s += pgfPen(painter()->pen());
		s += stroke_command;
	}

	return s;
}

QString QTeXPaintEngine::drawShape(Shape shape, const QString & path)
{
	if (d_pgf_mode)
		return drawPgfShape(shape, path);

	return drawTikzShape(shape, path);
}

QString QTeXPaintEngine::drawTikzShape(Shape shape, const QString & path)
{
	QString s = QString::null;
	if (path.isEmpty())
		return s;

	if (shape != Line && shape != Polyline && painter()->brush().style() != Qt::NoBrush)
		// fill the background
		s += tikzBrush(painter()->brush()) + path;
	if (painter()->pen().style() != Qt::NoPen)// draw the contour
		s += tikzPen(painter()->pen()) + path;

	return s;
}

void QTeXPaintEngine::drawImage(const QRectF & r, const QImage & image, const QRectF & sr, Qt::ImageConversionFlags flags)
{
	drawPixmap(QPixmap::fromImage(image, flags).copy(sr.toAlignedRect()), r);
}

void QTeXPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
	drawPixmap(pm.copy(sr.toAlignedRect()), r);
}

void QTeXPaintEngine::drawPixmap(const QPixmap &pix, const QRectF &r)
{
	QFileInfo fi(*file);

	QString base = fi.baseName() + "-images-";
	base += QDateTime::currentDateTime().toString("ddMMyy-hhmmss");
	if (!fi.dir().exists(base)){
		if (!fi.dir().mkdir(base))
			return;
	}

	QString name = fi.dir().absolutePath();
	name += "/" + base + "/image" + QString::number(d_pixmap_index);
	name = QDir::cleanPath(name);
	if (!pix.save(name + ".png", "PNG"))
		return;

	d_pixmap_index++;

	QTextStream t(file);
	t.setCodec("UTF-8");

	t << "\\pgfputat";
	t << pgfPoint(convertPoint(painter()->worldMatrix().map(r.bottomLeft())));
	t << "{\\pgfimage[interpolate=false,width=";

	QString u = unit();
	t << QString::number(r.width()*resFactorX()) + u + ",height=";
	t << QString::number(r.height()*resFactorY()) + u + "]{";
	t << name;
	t << "}}\n";
}

QString QTeXPaintEngine::clipPath()
{
	if (painter()->hasClipping()){
		QPainterPath path = painter()->clipPath().simplified();
		if (path.elementCount() > 1000)//latex has a limited main memory size
			return QString::null;

		if (d_pgf_mode)
			return pgfPath(path) + "\\pgfclip\n";
		else
			return "\\clip" + tikzPath(path);
	}
	return QString::null;
}

QString QTeXPaintEngine::defineColor(const QColor& c, const QString& name)
{
	QString col = "\\definecolor{" + name + "}{";
	if (d_gray_scale){
		col += "gray}{";
		double gray = qGray(c.rgb())/255.0;
		col += QString::number(gray) + "}\n";
	} else {
		col += "rgb}{";
		col += QString::number(c.redF()) + ",";
		col += QString::number(c.greenF()) + ",";
		col += QString::number(c.blueF()) + "}\n";
	}
	return col;
}

QString QTeXPaintEngine::tikzBrush(const QBrush& brush)
{
	QString options = QString::null;
	if (addNewPatternColor()){
		options = defineColor(brush.color(), "c");
		d_pattern_color = brush.color();
	}

	options += "\\fill [pattern color=c, pattern=";

	switch(brush.style()){
		case Qt::NoBrush:
			return QString::null;
		break;

		case Qt::SolidPattern:
		{
			QString s = QString::null;
			if (addNewBrushColor()){
				d_current_color = brush.color();
				s = color(brush.color());
			}

			s += "\\fill";

			double alpha = painter()->brush().color().alphaF();
			if(alpha > 0.0 && alpha < 1.0)
				s += "[opacity=" + QString::number(alpha) + "]";

			return s;
		}
		break;

		case Qt::Dense1Pattern:
		case Qt::Dense2Pattern:
		case Qt::Dense3Pattern:
		case Qt::Dense4Pattern:
			options += "crosshatch dots";
		break;

		case Qt::Dense5Pattern:
		case Qt::Dense6Pattern:
		case Qt::Dense7Pattern:
			options += "dots";
		break;

		case Qt::HorPattern:
			options += "horizontal lines";
		break;
		case Qt::VerPattern:
			options += "vertical lines";
		break;
		case Qt::CrossPattern:
			options += "grid";
		break;
		case Qt::BDiagPattern:
			options += "north east lines";
		break;
		case Qt::FDiagPattern:
			options += "north west lines";
		break;
		case Qt::DiagCrossPattern:
			options += "crosshatch";
		break;

		case Qt::LinearGradientPattern:
		{
			const QLinearGradient *qtgradient = (const QLinearGradient *)brush.gradient();
			QGradientStops stops = qtgradient->stops();

			QString lc = defineColor(stops.first().second, "lc");
			QString rc = defineColor(stops.last().second, "rc");

			QMatrix m = painter()->worldMatrix();
			QPointF sp = m.map(qtgradient->start());
			QPointF ep = m.map(qtgradient->finalStop());

			options = lc + rc + "\\fill [";
			options += "left color=lc, ";
			options += "right color=rc, ";
			options += "shading angle=" + QString::number(-QLineF(sp, ep).angle());
		}
		break;

		case Qt::RadialGradientPattern:
		{
			const QRadialGradient *qtgradient = (const QRadialGradient *)brush.gradient();
			QGradientStops stops = qtgradient->stops();

			QString colors;
			int count = stops.count();
			colors += defineColor(stops[0].second, "c1");
			colors += defineColor(stops[count - 1].second, "c2");

			/*for (int i = 0; i < count; i++){
				QGradientStop stop = stops[i];
				colors += defineColor(stop.second, "c" + QString::number(i + 1));
			}*/

			options = colors + "\\fill [";
			options += "inner color=c1, ";
			options += "outer color=c2, ";
			options += "shading=radial";
			qWarning("QTeXEngine: Uncentered Qt::RadialGradientPattern with more than two colors not supported.");

		}
		break;

		case Qt::ConicalGradientPattern:
		{
			qWarning("QTeXEngine: Qt::ConicalGradientPattern is not supported.");
			return QString::null;
		}
		break;

		default:
		break;
	}
	return options + "]";
}

QString QTeXPaintEngine::pgfBrush(const QBrush& brush)
{
	QString s = QString::null;
	QColor c = brush.color();
	QString col = defineColor(c, "c");
	QString command = "\\pgfsetfillpattern{";
	switch(brush.style()){
		case Qt::NoBrush:
		break;

		case Qt::SolidPattern:
			s += color(c);
		break;

		case Qt::Dense1Pattern:
		case Qt::Dense2Pattern:
		case Qt::Dense3Pattern:
		case Qt::Dense4Pattern:
			s += col + command + "crosshatch dots}{c}\n";
		break;

		case Qt::Dense5Pattern:
		case Qt::Dense6Pattern:
		case Qt::Dense7Pattern:
			s += col + command + "dots}{c}\n";
		break;

		case Qt::HorPattern:
			s += col + command + "horizontal lines}{c}\n";
		break;
		case Qt::VerPattern:
			s += col + command + "vertical lines}{c}\n";
		break;
		case Qt::CrossPattern:
			s += col + command + "grid}{c}\n";
		break;
		case Qt::BDiagPattern:
			s += col + command + "north east lines}{c}\n";
		break;
		case Qt::FDiagPattern:
			s += col + command + "north west lines}{c}\n";
		break;
		case Qt::DiagCrossPattern:
			s += col + command + "crosshatch}{c}\n";
		break;

		default:
		break;
	}
	return s;
}

QString QTeXPaintEngine::path(const QPainterPath & path)
{
	if (path.isEmpty ())
		return QString::null;

	if (d_pgf_mode)
		return pgfPath(path);

	return tikzPath(path);
}

QString QTeXPaintEngine::pgfPath(const QPainterPath & path)
{
	QString s = QString::null;
	int points = path.elementCount();
	QMatrix m = painter()->worldMatrix();
	int curvePoints = 0;
	for (int i = 0; i < points; i++){
		QPainterPath::Element el = path.elementAt(i);
		QPointF p = m.map(QPointF(el.x, el.y));

		switch(el.type){
			case QPainterPath::MoveToElement:
				s += "\\pgfmoveto" + pgfPoint(convertPoint(p)) + "\n";
			break;

			case QPainterPath::LineToElement:
				s += "\\pgflineto" + pgfPoint(convertPoint(p)) + "\n";
			break;

			case QPainterPath::CurveToElement:
				s += "\\pgfcurveto" + pgfPoint(convertPoint(p));
				curvePoints = 0;
			break;

			case QPainterPath::CurveToDataElement:
				s += pgfPoint(convertPoint(p));
				curvePoints++;
				if (curvePoints == 2)
					s += "\n";
			break;
		}
	}
	return s;
}

QString QTeXPaintEngine::tikzPath(const QPainterPath & path)
{
	QString s = QString::null;
	if (path.isEmpty())
		return s;

	int points = path.elementCount();
	QMatrix m = painter()->worldMatrix();
	int curvePoints = 0;
	for (int i = 0; i < points; i++){
		QPainterPath::Element el = path.elementAt(i);
		QPointF p = m.map(QPointF(el.x, el.y));
		switch(el.type){
			case QPainterPath::MoveToElement:
				s += tikzPoint(convertPoint(p));
			break;

			case QPainterPath::LineToElement:
				s += " -- " + tikzPoint(convertPoint(p));
			break;

			case QPainterPath::CurveToElement:
				s += " .. controls " + tikzPoint(convertPoint(p));
				curvePoints = 0;
			break;

			case QPainterPath::CurveToDataElement:
				curvePoints++;
				if (curvePoints == 1)
					s += " and " + tikzPoint(convertPoint(p));
				else if (curvePoints == 2)
					s += " .. " + tikzPoint(convertPoint(p));
			break;
		}
	}
	return s + ";\n";
}

QPointF QTeXPaintEngine::convertPoint( const QPointF& p)
{
	return QPointF(resFactorX()*p.x(), paintDevice()->height() - resFactorY()*p.y());
}

double QTeXPaintEngine::unitFactor()
{
	double factor = 1.0;
	switch (d_unit){
		case QTeXPaintDevice::pt:
			factor = 72.27;
		break;
		case QTeXPaintDevice::bp:
			factor = 72;
		break;
		case QTeXPaintDevice::mm:
			factor = 25.4;
		break;
		case QTeXPaintDevice::cm:
			factor = 2.54;
		break;
		case QTeXPaintDevice::in:
		case QTeXPaintDevice::ex:
		case QTeXPaintDevice::em:
		break;
	}
	return factor;
}

double QTeXPaintEngine::resFactorX()
{
	return unitFactor()/(double)paintDevice()->logicalDpiX();
}

double QTeXPaintEngine::resFactorY()
{
	return unitFactor()/(double)paintDevice()->logicalDpiY();
}

QString QTeXPaintEngine::pgfPoint( const QPointF& p)
{
	QString u = unit();
	QString s = "{\\pgfpoint{" + QString::number(p.x());
	s += u + "}{" + QString::number(p.y()) + u + "}}";
	return s;
}

QString QTeXPaintEngine::tikzPoint(const QPointF & p)
{
	QString u = unit();
	QString s = "(" + QString::number(p.x());
	s += u + "," + QString::number(p.y()) + u + ")";
	return s;
}

QString QTeXPaintEngine::color( const QColor& col)
{
	QString s = "\\color[";
	if (d_gray_scale){
		s += "gray]{";
		double gray = qGray(col.rgb())/255.0;
		s += QString::number(gray) + "}\n";
	} else {
		s += "rgb]{";
		s += QString::number(col.redF()) + ",";
		s += QString::number(col.greenF()) + ",";
		s += QString::number(col.blueF()) + "}\n";
	}

	return s;
}

QString QTeXPaintEngine::pgfPen(const QPen& pen)
{
	QString s = QString::null;
	if (pen.style() == Qt::NoPen)
		return s;

	s += color(pen.color());
	s += "\\pgfsetlinewidth{" + QString::number(painter()->pen().widthF()) + "pt}\n";

	QString aux = "\\pgfsetdash{";
	QString term = "}{0cm}\n";

	double space_length = 0.08*pen.widthF();
	double dot_length = 0.3*space_length;
	double dash_length = 1.5*space_length;

	QString dash = "{" + QString::number(dash_length) + "cm}";
	QString dot = "{" + QString::number(dot_length) + "cm}";
	QString space = "{" + QString::number(space_length) + "cm}";

	switch (pen.style()){
		case Qt::SolidLine:
		break;
		case Qt::DashLine:
			s += aux + dash + space + term;
		break;
		case Qt::DotLine:
			s += aux + dot + space + term;
		break;
		case Qt::DashDotLine:
			s += aux + dash + space + dot + space + term;
		break;
		case Qt::DashDotDotLine:
			s += aux + dash + space + dot + space + dot + space + term;
		break;

		case Qt::CustomDashLine:
		{
			s += aux;
			QVector<qreal> pattern = pen.dashPattern();
			int count = pattern.count();
			QString u = unit();
			for (int i = 0; i < count; i++)
				s += "{" + QString::number(pattern[i]) + u + "}";

			s += term;
			break;
		}

		default:
			break;
	}

	switch (pen.joinStyle()){
		case Qt::MiterJoin:
			s += "\\pgfsetmiterjoin\n";
			//s += "\\pgfsetmiterlimit{" + QString::number(pen.miterLimit()) + "pt}\n";
		break;
		case Qt::BevelJoin:
			s += "\\pgfsetbeveljoin\n";
		break;
		case Qt::RoundJoin:
			s += "\\pgfsetroundjoin\n";
		break;
		case Qt::SvgMiterJoin:
			s += "\\pgfsetmiterjoin\n";
		break;
		default:
			break;
	}

	switch (pen.capStyle()){
		case Qt::FlatCap:
			s += "\\pgfsetrectcap\n";
		break;
		case Qt::SquareCap:
			s += "\\pgfsetrectcap\n";
		break;
		case Qt::RoundCap:
			s += "\\pgfsetroundcap\n";
		break;
		default:
			break;
	}

	return s;
}

QString QTeXPaintEngine::tikzPen(const QPen& pen)
{
	if (pen.style() == Qt::NoPen)
		return QString::null;

	QString col = QString::null;
	if (addNewPenColor()){
		col = color(pen.color());
		d_current_color = pen.color();
	}

	QString options = "[line width=";
	options += QString::number(painter()->pen().widthF()) + "pt, ";

	double space_length = 0.08*pen.widthF();
	double dot_length = 0.3*space_length;
	double dash_length = 1.5*space_length;

	QString dash = "on " + QString::number(dash_length) + "cm";
	QString dot = "on " + QString::number(dot_length) + "cm";
	QString space = " off " + QString::number(space_length) + "cm";

	QString aux = "dash pattern=";
	QString term = ", dash phase=0pt, ";
	switch (pen.style()){
		case Qt::SolidLine:
		break;
		case Qt::DashLine:
			options += aux + dash + space + term;
		break;
		case Qt::DotLine:
			options += aux + dot + space + term;
		break;
		case Qt::DashDotLine:
			options += aux + dash + space + dot + space + term;
		break;
		case Qt::DashDotDotLine:
			options += aux + dash + space + dot + space + dot + space + term;
		break;

		case Qt::CustomDashLine:
		{
			options += aux;
			QVector<qreal> pattern = pen.dashPattern();
			int count = pattern.count();
			QString u = unit();
			for (int i = 0; i < count; i++){
				QString s = "on ";
				if (i%2)
					s = " off ";
				options += s + QString::number(pattern[i]) + u;
			}

			options += term;
			break;
		}

		default:
			break;
	}

	options += "line join=";
	switch (pen.joinStyle()){
		case Qt::MiterJoin:
			options += "miter, ";
		break;
		case Qt::BevelJoin:
			options += "bevel, ";
		break;
		case Qt::RoundJoin:
			options += "round, ";
		break;
		case Qt::SvgMiterJoin:
			options += "miter, ";
		break;
		default:
			break;
	}

	options += "line cap=";
	switch (pen.capStyle()){
		case Qt::FlatCap:
			options += "rect]";
		break;
		case Qt::SquareCap:
			options += "rect]";
		break;
		case Qt::RoundCap:
			options += "round]";
		break;
		default:
			break;
	}

	return col + "\\draw" + options;
}

QString QTeXPaintEngine::indentString(const QString& s)
{
	QStringList lst = s.split("\n", QString::SkipEmptyParts);
	for(int i = 0; i < lst.count(); i++)
		lst[i].prepend("\t");

	return lst.join("\n") + "\n";
}

QString QTeXPaintEngine::beginScope()
{
	QString s = "\\begin{scope}\n";
	if (d_pgf_mode)
		s = "\\begin{pgfscope}\n";

	if (painter()->hasClipping()){
		QString clip = clipPath();
		if (!clip.isEmpty())
			s += indentString(clip);
	}

	d_pattern_color = QColor();
	d_current_color = QColor();
	return s;
}

QString QTeXPaintEngine::endScope()
{
	if (d_pgf_mode)
		return "\\end{pgfscope}\n";

	return "\\end{scope}\n";
}

void QTeXPaintEngine::writeToFile(const QString& s)
{
	QTextStream t(file);
	t.setCodec("UTF-8");

	if (d_pgf_mode){
		t << beginScope();
		t << indentString(s);
		t << endScope();
		return;
	}

	QString scope;
	if (d_open_scope)
		scope = endScope();

	if (changedClipping()){
		scope += beginScope();
		scope += indentString(s);
		t << scope;

		d_open_scope = true;

		if (painter()->hasClipping())
			d_clip_path = painter()->clipPath();
		else
			d_clip_path = QPainterPath();
	} else
		t << indentString(s);
}

bool QTeXPaintEngine::emptyStringOperation()
{
	if ((painter()->brush().style() == Qt::NoBrush ||
		(painter()->brush().color().alpha() == 0)) &&
		painter()->pen().style() == Qt::NoPen)
		return true;

	return false;
}

bool QTeXPaintEngine::changedClipping()
{
	QPainterPath clipPath = QPainterPath();
	if (painter()->hasClipping()){
		if (painter()->clipPath().elementCount() > 1000)
			return false;
		clipPath = painter()->clipPath();
	}

	if (clipPath != d_clip_path)
		return true;

	return false;
}

bool QTeXPaintEngine::addNewPatternColor()
{
	Qt::BrushStyle style = painter()->brush().style();
	if (style <= Qt::SolidPattern || style >= Qt::LinearGradientPattern)
		return false;

	if (!d_pattern_color.isValid() ||
		d_pattern_color != painter()->brush().color())
		return true;

	return false;
}

bool QTeXPaintEngine::addNewBrushColor()
{
	if (!d_current_color.isValid() || changedClipping() ||
		d_current_color.name() != painter()->brush().color().name())
		return true;

	return false;
}

bool QTeXPaintEngine::addNewPenColor()
{
	if (!d_current_color.isValid() ||
		(changedClipping() && painter()->brush().style() == Qt::NoBrush) ||
		d_current_color.name() != painter()->pen().color().name())
		return true;

	return false;
}

QString QTeXPaintEngine::unit()
{
	switch (d_unit){
		case QTeXPaintDevice::pt:
			return "pt";
		break;
		case QTeXPaintDevice::bp:
			return "bp";
		break;
		case QTeXPaintDevice::mm:
			return "mm";
		break;
		case QTeXPaintDevice::cm:
			return "cm";
		break;
		case QTeXPaintDevice::in:
			return "in";
		break;
		case QTeXPaintDevice::ex:
			return "ex";
		break;
		case QTeXPaintDevice::em:
			return "em";
		break;
	}
	return "pt";
}
