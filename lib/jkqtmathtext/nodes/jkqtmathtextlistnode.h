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




#ifndef JKQTMATHTEXTLISTNODE_H
#define JKQTMATHTEXTLISTNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include <QPainter>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT


/** \brief subclass representing a list of nodes in the syntax tree
 *  \ingroup jkqtmathtext_items
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextListNode: public JKQTMathTextMultiChildNode {
    public:
        explicit JKQTMathTextListNode(JKQTMathText* parent);
        virtual ~JKQTMathTextListNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) override;
        /** \brief add a child node */
        void addChild(JKQTMathTextNode* n);
        /** \copydoc JKQTMathTextMultiChildNode::getChildren() */
        virtual QList<JKQTMathTextNode*> getChildren() override;
        /** \copydoc JKQTMathTextMultiChildNode::childCount() */
        virtual int childCount() const override;
        /** \copydoc JKQTMathTextMultiChildNode::clearChildren() */
        virtual void clearChildren(bool deleteChildren=true) override;
        /** \copydoc JKQTMathTextMultiChildNode::getChild() */
        virtual JKQTMathTextNode* getChild(int i) override;
        /** \copydoc JKQTMathTextMultiChildNode::getChild() */
        virtual const JKQTMathTextNode* getChild(int i) const override;
        /** \copydoc JKQTMathTextMultiChildNode::getChild() */
        virtual JKQTMathTextNode* replaceChild(int i, JKQTMathTextNode* newChild) override;
    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual void getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        QList<JKQTMathTextNode*> nodes;
        /** \brief the instructions in this can have the sub-script/superscript set below/above, not besides the node */
        static QSet<QString> subsupOperations;
};

#endif // JKQTMATHTEXTLISTNODE_H









