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




#ifndef JKQTMATHTEXTNOOPNODE_H
#define JKQTMATHTEXTNOOPNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include <QPainter>

class JKQTMathText; // forward


/** \brief subclass representing a node that outputs nothing
 *  \ingroup jkqtmathtext_items
 *
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextNoopNode: public JKQTMathTextNonDrawingBaseNode {
    public:
        explicit JKQTMathTextNoopNode(JKQTMathText* parent);
        virtual ~JKQTMathTextNoopNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
    protected:

};


/** \brief subclass representing a \c {...} block
 *  \ingroup jkqtmathtext_items
 *
 *  This is necessray becaus JKQTMathtextHorizontalListNode
 *  implements JKQTMathTextModifyEnvironmentNodeMixIn, but a
 *  \c {...} block does not hand on its modified JKQTMathTextEnvironment.
 *
 *  This node simply hands on all calls to its child.
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextBlockNode: public JKQTMathTextSingleChildNode {
    public:
        explicit JKQTMathTextBlockNode(JKQTMathTextNode *_child, JKQTMathText* parent);
        virtual ~JKQTMathTextBlockNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;
    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;

};
#endif // JKQTMATHTEXTNOOPNODE_H









