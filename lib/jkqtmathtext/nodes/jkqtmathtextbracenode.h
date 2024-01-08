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




#ifndef JKQTMATHTEXTBRACENODE_H
#define JKQTMATHTEXTBRACENODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include <QPainter>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT


/** \brief subclass representing a brace node
 *  \ingroup jkqtmathtext_items
 *
 *  \image html jkqtmathtext/jkqtmathtext_bracenode_geo.png
 *
 *  This node supports the brace-types encoded by JKQTMathTextBraceType:
 *  \copydetails JKQTMathTextBraceType
 *
 *  In addition it is possible to switch the opening and the closing braces independently on and off.
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextBraceNode: public JKQTMathTextSingleChildNode {
    public:

        JKQTMathTextBraceNode(JKQTMathText* parent, JKQTMathTextBraceType openbrace, JKQTMathTextBraceType closebrace, JKQTMathTextNode* child);
        virtual ~JKQTMathTextBraceNode() override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc openbrace */
        JKQTMathTextBraceType getOpenbrace() const;
        /** \copydoc closebrace */ 
        JKQTMathTextBraceType getClosebrace() const;
    protected:
        /** \brief defines the size of the node (JKQTMathTextNodeSize) and additional information  */
        struct NodeSize: public JKQTMathTextNodeSize {
            NodeSize();
            NodeSize(const NodeSize& other);
            NodeSize(const JKQTMathTextNodeSize& other);
            NodeSize& operator=(const NodeSize& other);
            NodeSize& operator=(const JKQTMathTextNodeSize& other);
            /** \brief width of the opening brace */
            double openBraceWidth;
            /** \brief height of the opening brace */
            double openBraceHeight;
            /** \brief width of the closing brace */
            double closeBraceWidth;
            /** \brief height of the closing brace */
            double closeBraceHeight;
        };
        /** \copydoc JKQTMathTextNode::getSizeInternal()
         *
         * \note This function internally calls getSizeInternalAndBrace() and returns part of its results.
         */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;
        /** calculates the size of this node (also returned by getSizeInternal() ) and of the brace */
        NodeSize getSizeInternalAndBrace(QPainter& painter, JKQTMathTextEnvironment currentEv) const;
        /**\brief opening brace */
        JKQTMathTextBraceType openbrace;
        /**\brief closing brace */
        JKQTMathTextBraceType closebrace;
        /** \brief calculate the width of the braces, fills JKQTMathTextBraceNode::NodeSize::openBraceWidth/JKQTMathTextBraceNode::NodeSize::closeBraceWidth and JKQTMathTextBraceNode::NodeSize::openBraceHeight/JKQTMathTextBraceNode::NodeSize::closeBraceHeight in \a out, based on \a childSize
         *
         *  \param[out] out this function fills JKQTMathTextBraceNode::NodeSize::openBraceWidth/JKQTMathTextBraceNode::NodeSize::closeBraceWidth and JKQTMathTextBraceNode::NodeSize::openBraceHeight/JKQTMathTextBraceNode::NodeSize::closeBraceHeight in \a out
         *  \param painter a QPainter used for size-calculation/drawing
         *  \param currentEv font environment
         *  \param childSize size of the child node surrounded by the node
         */
        void calcBraceSizes(NodeSize& out, QPainter& painter, const JKQTMathTextEnvironment& currentEv, const JKQTMathTextNodeSize &childSize) const;
        /** \brief calculate the width and height of a brace, based on \a childSize
         *
         *  \param fm QFontMetricsF object describing the font of the current environment
         *  \param bracetype brace type
         *  \param childSize size of the child node surrounded by the node
         *
         *  \return width and hieght of the brace
         */
        QSizeF calcBraceSize(const QFont& fm, QPaintDevice* pd, JKQTMathTextBraceType bracetype, const JKQTMathTextNodeSize &childSize) const;
};

#endif // JKQTMATHTEXTBRACENODE_H









