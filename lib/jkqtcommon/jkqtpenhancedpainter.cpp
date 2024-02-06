/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)

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




#include "jkqtcommon/jkqtpenhancedpainter.h"
#include <QPainterPath>


JKQTPEnhancedPainter::JKQTPEnhancedPainter(QPaintDevice *device):
    QPainter(device),
    m_flags(DefaultPaintMode)
{
    initQEnhacedPainter();
}

JKQTPEnhancedPainter::JKQTPEnhancedPainter():
    QPainter()
{
    initQEnhacedPainter();
}

void JKQTPEnhancedPainter::setPainterFlag(PainterFlag flag, bool enabled)
{
    if (!enabled && m_flags.testFlag(flag)) m_flags &= ~flag;
    else if (enabled && !m_flags.testFlag(flag)) m_flags |= flag;
}

void JKQTPEnhancedPainter::setPainterFlag(PainterFlags flags)
{
    m_flags=flags;
}

void JKQTPEnhancedPainter::drawPolylineFast(const QPointF *points, int pointCount)
{
    if (!points || pointCount<2) return;
    if (m_flags.testFlag(VectorPainting) || pen().style()!=Qt::SolidLine || pen().widthF()>device()->devicePixelRatioF()+0.01 || pen().widthF()<device()->devicePixelRatioF()*0.8) {
        //qDebug()<<"drawFastPolyline(): widthF="<<pen().widthF();
        drawPolyline(points, pointCount);
    } else {
        //qDebug()<<"drawFastPolyline()";
        // we use a specialized algorithm only in a very defined circumstances
        for (int i=1; i<pointCount; i++) {
            QLineF l(points[i-1], points[i]);
            if (!l.isNull() && l.length()>0) drawLine(l);
        }
    }
}

void JKQTPEnhancedPainter::drawPolylineFast(const QPoint *points, int pointCount)
{
    if (!points || pointCount<2) return;
    if (m_flags.testFlag(VectorPainting) || pen().style()!=Qt::SolidLine || pen().widthF()>1.01) {
        drawPolyline(points, pointCount);
    } else {
        // we use a specialized algorithm only in a very defined circumstances
        for (int i=1; i<pointCount; i++) {
            QLine l(points[i-1], points[i]);
            if (!l.isNull() && (l.dx()>0||l.dy()>0)) drawLine(l);
        }
    }
}

void JKQTPEnhancedPainter::drawComplexRoundedRect(const QRectF &rin, double rTopLeft, double rTopRight, double rBottomLeft, double rBottomRight, Qt::SizeMode mode)
{
    QRectF r=rin;
    const double rTLX=(mode==Qt::RelativeSize)?(rTopLeft/100.0*r.width()):rTopLeft;
    const double rTLY=(mode==Qt::RelativeSize)?(rTopLeft/100.0*r.height()):rTopLeft;

    const double rTRX=(mode==Qt::RelativeSize)?(rTopRight/100.0*r.width()):rTopRight;
    const double rTRY=(mode==Qt::RelativeSize)?(rTopRight/100.0*r.height()):rTopRight;

    const double rBLX=(mode==Qt::RelativeSize)?(rBottomLeft/100.0*r.width()):rBottomLeft;
    const double rBLY=(mode==Qt::RelativeSize)?(rBottomLeft/100.0*r.height()):rBottomLeft;

    const double rBRX=(mode==Qt::RelativeSize)?(rBottomRight/100.0*r.width()):rBottomRight;
    const double rBRY=(mode==Qt::RelativeSize)?(rBottomRight/100.0*r.height()):rBottomRight;

    QPainterPath path;
    path.moveTo(r.left()+rTLX,r.top());
    path.lineTo(r.right()-rTRX,r.top());
    path.quadTo(r.topRight(), QPointF(r.right(),r.top()+rTRY));
    path.lineTo(r.right(),r.bottom()-rBRY);
    path.quadTo(r.bottomRight(), QPointF(r.right()-rBRX,r.bottom()));
    path.lineTo(r.left()+rBLX,r.bottom());
    path.quadTo(r.bottomLeft(), QPointF(r.left(),r.bottom()-rBLY));
    path.lineTo(r.left(),r.top()+rTLY);
    path.quadTo(r.topLeft(), QPointF(r.left()+rTLX,r.top()));
    path.closeSubpath();
    drawPath(path);
}

void JKQTPEnhancedPainter::drawRoundedRectOrRect(const QRectF &r, double radius, Qt::SizeMode /*mode*/)
{
    if (radius<=0) {
        drawRect(r);
    } else {
        drawRoundedRect(r, radius, radius);
    }
}

JKQTPEnhancedPainter::PainterFlags JKQTPEnhancedPainter::painterFlags() const {
    return m_flags;
}


void JKQTPEnhancedPainter::initQEnhacedPainter()
{

}
