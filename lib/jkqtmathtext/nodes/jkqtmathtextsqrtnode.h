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




#ifndef JKQTMATHTEXTSQRTNODE_H
#define JKQTMATHTEXTSQRTNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include <QPainter>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT



/** \brief subclass representing a sqrt node
 *  \ingroup jkqtmathtext_items
 *
 *  This node renders square roots without and with an explicitly shown degree:
 *  \image html jkqtmathtext/jkqtmathtext_sqrt.png
 *  \image html jkqtmathtext/jkqtmathtext_cbrt.png
  */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextSqrtNode: public JKQTMathTextMultiChildNode {
    public:
        JKQTMathTextSqrtNode(JKQTMathText* parent, JKQTMathTextNode* child__, JKQTMathTextNode* childDegree__=nullptr);
        virtual ~JKQTMathTextSqrtNode() override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override ;

        /** \copydoc child */
        JKQTMathTextNode* getChild();
        /** \copydoc child */
        const JKQTMathTextNode* getChild() const;
        /** \copydoc childDegree */
        JKQTMathTextNode* getChildDegree();
        /** \copydoc childDegree */
        const JKQTMathTextNode* getChildDegree() const;

        /** \copydoc JKQTMathTextMultiChildNode::getChild() */
        virtual JKQTMathTextNode* getChild(int i) override;
        /** \copydoc JKQTMathTextMultiChildNode::getChild() */
        virtual const JKQTMathTextNode* getChild(int i) const override;
        /** \copydoc JKQTMathTextMultiChildNode::getChild() */
        virtual JKQTMathTextNode* replaceChild(int i, JKQTMathTextNode* newChild) override;
        /** \copydoc JKQTMathTextMultiChildNode::getChild() */
        virtual int childCount() const override;
        /** \copydoc JKQTMathTextMultiChildNode::getChild() */
        virtual void clearChildren(bool deleteChildren=true) override;
        /** \copydoc JKQTMathTextMultiChildNode::deleteChild() */
        virtual void deleteChild(int i) override;

    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual void getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos) override;
        /** \brief child node for the part under the root */
        JKQTMathTextNode* child;
        /** \brief second child node for the degree of the root (or nullptr if nothing) */
        JKQTMathTextNode* childDegree;
};

#endif // JKQTMATHTEXTSQRTNODE_H









