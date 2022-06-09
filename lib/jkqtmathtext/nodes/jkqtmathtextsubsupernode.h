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




#ifndef JKQTMATHTEXTSUBSUPERSCRIPTNODE_H
#define JKQTMATHTEXTSUBSUPERSCRIPTNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include <QPainter>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT


/** \brief subclass representing an subscript node with exactly one argument in the syntax tree
*  \ingroup jkqtmathtext_items
*
* \image html jkqtmathtext/jkqtmathtext_subscriptnode_getSizeInternal.png
*/
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextSubscriptNode: public JKQTMathTextSingleChildNode {
public:
    explicit JKQTMathTextSubscriptNode(JKQTMathText* parent, JKQTMathTextNode* child);
    virtual ~JKQTMathTextSubscriptNode() override;
    /** \copydoc JKQTMathTextNode::draw() */
    virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
    /** \copydoc JKQTMathTextNode::getTypeName() */
    virtual QString getTypeName() const override;                /** \brief returns the child node  */
    /** \copydoc JKQTMathTextNode::toHtml() */
    virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) override;
protected:
    /** \copydoc JKQTMathTextNode::getSizeInternal() */
    virtual void getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
};

/** \brief subclass representing an superscript node with exactly one argument in the syntax tree
*  \ingroup jkqtmathtext_items
*
* \image html jkqtmathtext/jkqtmathtext_subscriptnode_getSizeInternal.png
*
* \note a MTlistNode might modify the positioning slightly for special cases (e.g. \c \\int , \c \\sum ... or after braces)
*/
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextSuperscriptNode: public JKQTMathTextSingleChildNode {
public:
    explicit JKQTMathTextSuperscriptNode(JKQTMathText* parent, JKQTMathTextNode* child);
    virtual ~JKQTMathTextSuperscriptNode() override;
    /** \copydoc JKQTMathTextNode::draw() */
    virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
    /** \copydoc JKQTMathTextNode::getTypeName() */
    virtual QString getTypeName() const override;
    /** \copydoc JKQTMathTextNode::toHtml() */
    virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) override;
protected:
    /** \copydoc JKQTMathTextNode::getSizeInternal() */
    virtual void getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
};

#endif // JKQTMATHTEXTSUBSUPERSCRIPTNODE_H









