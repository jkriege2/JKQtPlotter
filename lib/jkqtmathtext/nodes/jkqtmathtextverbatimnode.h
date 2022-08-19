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




#ifndef JKQTMATHTEXTVERBATIMNODE_H
#define JKQTMATHTEXTVERBATIMNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include "jkqtmathtext/nodes/jkqtmathtexttextnode.h"
#include <QPainter>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT



/** \brief subclass representing a verbatim (plain-text) node with support for line-breaks in the syntax tree
 *  \ingroup jkqtmathtext_items
 *
 *  The layout of the lines can left-aligned, right-aligned or centered.
 *
 *  \image html jkqtmathtext_verticallist.png
 *
 *  \image html jkqtmathtext_verticalalignment.png
 *
 *  \image html jkqtmathtext_horizontalalignment.png
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextVerbatimNode: public JKQTMathTextTextBaseNode {
    public:
        explicit JKQTMathTextVerbatimNode(JKQTMathText* parent, const QString& text, bool visibleWhitespace=false, JKQTMathTextHorizontalAlignment _alignment=MTHALeft, double _linespacingFactor=1.0, JKQTMathTextVerticalOrientation _verticalOrientation=MTVOFirstLine, size_t tabSize_=4);
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc alignment */
        JKQTMathTextHorizontalAlignment getAlignment() const;
        /** \copydoc verticalOrientation */
        JKQTMathTextVerticalOrientation getVerticalOrientation() const;
        /** \copydoc lineSpacingFactor */
        double getLineSpacingFactor() const;
        /** \copydoc visibleWhitespace */
        bool getVisibleWhitespace() const;
        /** \copydoc tabSize */
        size_t getTabSize() const;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;
    protected:
        /** \brief alignment scheme used to lay out all lines
          *
          * \image html jkqtmathtext_horizontalalignment.png
          */
        JKQTMathTextHorizontalAlignment alignment;
        /** \brief spacing of the separate lines, as factor of the default line-spacing [Default: 1].
         *
         *  This property can be used to move the lines closer together or farther apart.
         *
         *  \image html jkqtmathtext_verticallist.png
         */
        double lineSpacingFactor;
        /** \brief vertical orientation of the baseline of the whole block (with respect to the single lines)
         *
         *  \image html jkqtmathtext_verticalorientation.png
         */
        JKQTMathTextVerticalOrientation verticalOrientation;
        /** \brief when \c true, whitespaces are displayed with a visible character */
        bool visibleWhitespace;
        /** \brief number of whitespaces, each tab character stands for */
        size_t tabSize;

        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;
        /** \brief sets all necessary settings in \a currentEv for drawing this node */
        virtual void transformEnvironment(JKQTMathTextEnvironment& currentEv) const;

        /** \brief describes the layout of the whole node */
        struct LayoutInfo: public JKQTMathTextNodeSize {
            LayoutInfo();
            /** \brief the text from JKQTMathTextVerbatimNode::text, split into lines */
            QStringList lines;
            /** \brief drawing position for each line */
            QList<QPointF> X;
        };
        /** \brief calclates the layout of the whole block/node
         *
         *  \note This function does NOT call transformEnvironment();
         *        it has to be called before calling this!
         */
        LayoutInfo calcLayout(QPainter& painter, const JKQTMathTextEnvironment& currentEv) const;
        /** \brief transforms the \a text before sizing/drawing (may e.g. exchange special letters for other unicode symbols etc.) */
        virtual QString textTransform(const QString& text, const JKQTMathTextEnvironment& currentEv) const override;
};


#endif // JKQTMATHTEXTVERBATIMNODE_H









