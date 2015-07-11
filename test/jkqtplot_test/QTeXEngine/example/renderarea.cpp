#include <QtGui>
#include "renderarea.h"
#include "pixmaps.h"

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    setFixedWidth(600);
    setFixedHeight(500);
}

void RenderArea::draw(QPainter *painter)
{
	static const QPoint points[4] = {
        QPoint(10, 80),
        QPoint(20, 10),
        QPoint(80, 30),
        QPoint(90, 70)
    };

    QRect rect(10, 20, 80, 60);

    QPainterPath path;
    path.moveTo(20, 80);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);

	int startAngle = 20 * 16;
    int arcLength = 120 * 16;

	QLinearGradient linearGrad(QPointF(0, 0), QPointF(100, 100));
	linearGrad.setColorAt(0, Qt::black);
	linearGrad.setColorAt(1, Qt::red);

	QRadialGradient radialGrad(QPointF(50, 50), 50, QPointF(50, 50));
	radialGrad.setColorAt(0, Qt::red);
	radialGrad.setColorAt(1, Qt::green);

	/*QConicalGradient conGrad(QPointF(50, 50), 90);
	conGrad.setColorAt(0, Qt::blue);
	conGrad.setColorAt(0.5, Qt::red);
	conGrad.setColorAt(1, Qt::green);
	QBrush brush = QBrush(conGrad);*/

    QBrush brush = QBrush(Qt::gray);
	int brushStyle = Qt::NoBrush;
	int penStyle = Qt::NoPen;
	int shape = Rect;
	int drawnRects = 0;
	for (int x = 0; x < width(); x += 100){
        for (int y = 0; y < height(); y += 100){
        	painter->save();
        	painter->setRenderHint(QPainter::Antialiasing);

        	shape = (shape + 1)%14;

        	penStyle = (penStyle + 1)%4;
        	if(!penStyle)
				penStyle = 1;

			if (shape == RoundedRect && drawnRects < 2){
				if (drawnRects == 0)
					painter->setBrush(QBrush(linearGrad));
				else if (drawnRects == 1)
					painter->setBrush(QBrush(radialGrad));
				drawnRects++;
			} else {
				brushStyle = (brushStyle + 1)%14;
				if(!brushStyle)
					brushStyle = 1;
				brush.setStyle((Qt::BrushStyle)brushStyle);
				painter->setBrush(brush);
			}

            painter->setPen(QPen(Qt::black, 2, (Qt::PenStyle)penStyle, Qt::FlatCap, Qt::MiterJoin));
            painter->translate(x, y);
			switch (shape) {
				case Line:
					painter->drawLine(rect.bottomLeft(), rect.topRight());
					break;
				case Polyline:
					painter->drawPolyline(points, 4);
					break;
				case Polygon:
					painter->drawPolygon(points, 4);
					break;
				case Rect:
					painter->drawRect(rect);
					break;
				case RoundedRect:
					painter->drawRoundedRect(rect, 25, 25, Qt::RelativeSize);
					break;
				case Ellipse:
					painter->drawEllipse(rect);
					break;
				case Arc:
					painter->drawArc(rect, startAngle, arcLength);
					break;
				case Chord:
					painter->drawChord(rect, startAngle, arcLength);
					break;
				case Pie:
					painter->drawPie(rect, startAngle, arcLength);
					break;
				case Path:
					painter->drawPath(path);
					break;
				case Text:
					painter->drawText(rect, Qt::AlignCenter, "QTeXEngine");
					break;
				case Pixmap:
					painter->drawPixmap(10, 10, QPixmap(qt_logo_xpm));
					break;
				case TiledPixmap:
					painter->drawTiledPixmap (QRect(10, 10, 64, 64), QPixmap(brick_xpm));
					break;
			}
			painter->restore();
        }
	}

	painter->setPen(Qt::darkGray);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(QRect(9, 9, width() - 18, height() - 18));
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
	QPainter painter(this);
	draw(&painter);
}
