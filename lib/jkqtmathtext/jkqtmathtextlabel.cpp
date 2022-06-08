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



#include "jkqtmathtext/jkqtmathtextlabel.h"
#include <QDebug>
#include <QApplication>

JKQTMathTextLabel::JKQTMathTextLabel(QWidget *parent):
    QLabel(parent)
{
    m_mathText=new JKQTMathText(this);
    m_mathText->useXITS();
    m_mathText->setFontSize(font().pointSizeF()*1.3);
    lastText="";
    repaintDo=true;
    buffer=QPixmap();
}

JKQTMathTextLabel::~JKQTMathTextLabel()
= default;

JKQTMathText *JKQTMathTextLabel::getMathText() const
{
    return m_mathText;
}

void JKQTMathTextLabel::setMath(const QString &text, bool doRepaint)
{
    if (text!=lastText || doRepaint){
        lastText=text;
        repaintDo=true;
        internalPaint();
        update();
    }
}


void JKQTMathTextLabel::internalPaint()
{
    //return;
    //qDebug()<<"internalPaint "<<lastText<<repaintDo;
    //if (repaintDo) {
        QSizeF size;
        {
            repaintDo=false;
            //qDebug()<<"internalPaint(): parse "<<m_mathText->parse(lastText)<<"\n  "<<m_mathText->getErrorList().join("\n")<<"\n\n";
            if (!m_mathText->parse(lastText)) {
                //qDebug()<<"JKQTMathTextLabel::internalPaint(): parse '"<<lastText<<"': "<<m_mathText->parse(lastText)<<"\n  "<<m_mathText->getErrorList().join("\n")<<"\n\n";
            }
            if (buffer.width()<=0 || buffer.height()<=0) {
                const qreal dpr = devicePixelRatioF();
                buffer=QPixmap(1000*dpr,100*dpr);
                buffer.setDevicePixelRatio(dpr);
            }
            //qDebug()<<"internalPaint(): buffer "<<buffer.size();
            QPainter p;
            //qDebug()<<"internalPaint(): "<<p.begin(&buffer);
            p.begin(&buffer);
            p.setRenderHint(QPainter::Antialiasing);
#if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
            p.setRenderHint(QPainter::HighQualityAntialiasing);
#endif
            p.setRenderHint(QPainter::TextAntialiasing);
            size=m_mathText->getSize(p);
            p.end();
        }
        const qreal dpr = devicePixelRatioF();
        buffer=QPixmap(static_cast<int>(qMax(32.0,size.width()*1.2))*dpr, static_cast<int>(qMax(10.0,size.height()*1.1))*dpr);
        buffer.setDevicePixelRatio(dpr);
        buffer.fill(Qt::transparent);
        {
            qDebug()<<"internalPaint(): buffer.size()="<<buffer.size()<<" size="<<size<<" dpr="<<dpr;
            QPainter p;
            //qDebug()<<"internalPaint(): "<<p.begin(&buffer);
            p.begin(&buffer);
            p.setRenderHint(QPainter::Antialiasing);
#if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
            p.setRenderHint(QPainter::HighQualityAntialiasing);
#endif
            p.setRenderHint(QPainter::TextAntialiasing);
            m_mathText->draw(p,alignment(), QRectF(QPointF(0,0), size));
            p.end();
        }
        setPixmap(buffer);
    //}
    //qDebug()<<"internalPaint(): setPixmap";
    QApplication::processEvents();
    //qDebug()<<"internalPaint(): DONE";
}

void JKQTMathTextLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
}


