/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)

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

JKQTPEnhancedPainter::PainterFlags JKQTPEnhancedPainter::painterFlags() const {
    return m_flags;
}


void JKQTPEnhancedPainter::initQEnhacedPainter()
{

}
