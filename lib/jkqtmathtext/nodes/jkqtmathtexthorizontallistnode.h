/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)
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




#ifndef JKQTMATHTEXTHONRIZONTALLISTNODE_H
#define JKQTMATHTEXTHONRIZONTALLISTNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include <QPainter>
#include <QFontMetricsF>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT


/** \brief subclass representing a list of nodes in the syntax tree, layed out horizontally
 *  \ingroup jkqtmathtext_items
 *
 *  This type of node also implements typesetting sub-/superscript above/below the previous node (if
 *  JKQTMathTextNode::is() is \c true for that previus node. The drawing is done as defined in the
 *  following image:
 *    \image html jkqtmathtext_subsuper_with_limits.png
 *
 *  In addition this node also ensures that subscripts are moved to the left for italic previous
 *  text or nodes that require such a shift (e.g. \c \\nabla or \c \\int ):
 *
 *  \image html jkqtmathtext/jkqtmathtext_doc_subsuper_italiccorrection.png
 *
 *  \image html jkqtmathtext/jkqtmathtext_doc_subsuper_italiccorrection_boxes.png
 *
 *  This is implemented using JKQTMathTextNodeSize::baselineXCorrection .
 *
 *  In addition this class implements execution of instructions derived from
 *  JKQTMathTextModifyEnvironmentNodeMixIn.
 *
 *  \see JKQTMathTextVerticalListNode
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextHorizontalListNode: public JKQTMathTextMultiChildNode, public JKQTMathTextModifyEnvironmentNodeMixIn {
    public:
        explicit JKQTMathTextHorizontalListNode(JKQTMathText* parent);
        virtual ~JKQTMathTextHorizontalListNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;
        /** \brief add a child node */
        void addChild(JKQTMathTextNode* n);
        /** \copydoc JKQTMathTextMultiChildNode::getChildren() */
        virtual QList<JKQTMathTextNode*> getChildren() override;
        /** \copydoc JKQTMathTextMultiChildNode::childCount() */
        virtual int childCount() const override;
        /** \copydoc JKQTMathTextMultiChildNode::clearChildren() */
        virtual void clearChildren(bool deleteChildren=true) override;
        /** \copydoc JKQTMathTextMultiChildNode::deleteChild() */
        virtual void deleteChild(int i) override;
        /** \copydoc JKQTMathTextMultiChildNode::getChild() */
        virtual JKQTMathTextNode* getChild(int i) override;
        /** \copydoc JKQTMathTextMultiChildNode::getChild() */
        virtual const JKQTMathTextNode* getChild(int i) const override;
        /** \copydoc JKQTMathTextMultiChildNode::getChild() */
        virtual JKQTMathTextNode* replaceChild(int i, JKQTMathTextNode* newChild) override;
        /** \copydoc JKQTMathTextModifyEnvironmentNodeMixIn::modifyEnvironment() */
        virtual void modifyEnvironment(JKQTMathTextEnvironment& currentEv) const override;

    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;
        /** \brief list of all nodes within the line */
        QList<JKQTMathTextNode*> nodes;
    private:
        /** \brief  internal implementation of clearChildren() that is non-virtual, so can be used in the destructor */
        void clearChildrenImpl(bool deleteChildren);
};




#endif // JKQTMATHTEXTHONRIZONTALLISTNODE_H









