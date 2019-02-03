/*
Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/




#include "jkqtplottertools/jkqtpenhancedpainter.h"



JKQTPEnhancedPainter::JKQTPEnhancedPainter(QPaintDevice *device):
    QPainter(device)
{
    initQEnhacedPainter();
}

JKQTPEnhancedPainter::JKQTPEnhancedPainter():
    QPainter()
{
    initQEnhacedPainter();
}

JKQTPEnhancedPainter::~JKQTPEnhancedPainter()
= default;

//void JKQTPEnhancedPainter::drawLines(const QLineF *lines, int lineCount)
//{

//}

//void JKQTPEnhancedPainter::drawLines(const QPointF *lines, int lineCount)
//{

//}

//void JKQTPEnhancedPainter::drawLines(const QLine *lines, int lineCount)
//{

//}

//void JKQTPEnhancedPainter::drawLines(const QPoint *lines, int lineCount)
//{

//}


void JKQTPEnhancedPainter::initQEnhacedPainter()
{

}
