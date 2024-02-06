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
    /** \brief like draw(), i.e. draws the node, but allows to draw the subscript with special placement rules after large blocks (e.g. braces), when \a prevNodeSizeForSpecialPlacement is not \c nullptr
     *
     * \param painter QPainter to use
     * \param x x-position, where the drawing starts [Pixel]
     * \param y Y-position of the baseline, where the drawing starts [Pixel]
     * \param currentEv JKQTMathTextEnvironment object describing the current drawing environment/settings
     * \param[in] prevNodeSizeForSpecialPlacement optional parameter, describing the size of the previous node (on the left). This may be used for layout of some nodes (e.g. sub/super to move correctly next to large parantheses ...)
     *
     * \return the x position which to use for the next part of the text
     */
    double drawWithSpecialPlacement(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSizeForSpecialPlacement) const;
    /** \copydoc JKQTMathTextNode::draw() */
    virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
    /** \copydoc JKQTMathTextNode::getTypeName() */
    virtual QString getTypeName() const override;                /** \brief returns the child node  */
    /** \copydoc JKQTMathTextNode::toHtml() */
    virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;
    /** \brief like getSize(), i.e. determine the size of the node, but cares for subscript with special placement rules after large blocks (e.g. braces), when \a prevNodeSizeForSpecialPlacement is not \c nullptr
     *
     * \param painter painter to use for determining the size
     * \param currentEv current environment object
     * \param[in] prevNodeSizeForSpecialPlacement optional parameter, describing the size of the previous node (on the left). This may be used for layout of some nodes (e.g. sub/super to move correctly next to large parantheses ...)
     *
     */
    JKQTMathTextNodeSize getSizeWithSpecialPlacement(QPainter& painter, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSizeForSpecialPlacement) const;
protected:
    /** \copydoc JKQTMathTextNode::getSizeInternal() */
    virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;
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
    /** \brief like draw(), but allows to draw the superscript with special placement rules after large blocks (e.g. braces), when \a prevNodeSizeForSpecialPlacement is not \c nullptr
     *
     * \param painter QPainter to use
     * \param x x-position, where the drawing starts [Pixel]
     * \param y Y-position of the baseline, where the drawing starts [Pixel]
     * \param currentEv JKQTMathTextEnvironment object describing the current drawing environment/settings
     * \param[in] prevNodeSizeForSpecialPlacement optional parameter, describing the size of the previous node (on the left). This may be used for layout of some nodes (e.g. sub/super to move correctly next to large parantheses ...)
     *
     * \return the x position which to use for the next part of the text
     */
    double drawWithSpecialPlacement(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSizeForSpecialPlacement) const;
    /** \copydoc JKQTMathTextNode::draw() */
    virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
    /** \copydoc JKQTMathTextNode::getTypeName() */
    virtual QString getTypeName() const override;
    /** \copydoc JKQTMathTextNode::toHtml() */
    virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;
    /** \brief like getSize(), i.e. determine the size of the node, but cares for superscript with special placement rules after large blocks (e.g. braces), when \a prevNodeSizeForSpecialPlacement is not \c nullptr
     *
     * \param painter painter to use for determining the size
     * \param currentEv current environment object
     * \param[in] prevNodeSizeForSpecialPlacement optional parameter, describing the size of the previous node (on the left). This may be used for layout of some nodes (e.g. sub/super to move correctly next to large parantheses ...)
     *
     */
    JKQTMathTextNodeSize getSizeWithSpecialPlacement(QPainter& painter, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSizeForSpecialPlacement) const;
protected:
    /** \copydoc JKQTMathTextNode::getSizeInternal() */
    virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;
};

#endif // JKQTMATHTEXTSUBSUPERSCRIPTNODE_H









