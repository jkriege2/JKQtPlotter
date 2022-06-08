/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)
    with contributions from: Razi Alavizadeh

    

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



#ifndef JKQTMATHTEXTLABEL_H
#define JKQTMATHTEXTLABEL_H

#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include <QWidget>
#include <QLabel>



/*! \brief A QLabel-derived class that draws an equation with LaTeX markup using JKQTMathText
    \ingroup jkqtmathtext

    \see JKQTMathText
*/
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextLabel: public QLabel {
        Q_OBJECT
    public:
        explicit JKQTMathTextLabel(QWidget* parent=nullptr);
        virtual ~JKQTMathTextLabel();

        /** \brief returns the internal JKQTMathText instance used for drawing
         *
         *  Use this function to set the font, font size and other properties of the used renderer.
         */
        JKQTMathText* getMathText() const;
        /** \brief set the equation to draw */
        void setMath(const QString& text, bool doRepaint=true);
    protected:
        JKQTMathText* m_mathText;
        QString lastText;
        QPixmap buffer;
        bool repaintDo;
        void internalPaint();

        void paintEvent(QPaintEvent * event);
};


#endif // JKQTMATHTEXTLABEL_H

