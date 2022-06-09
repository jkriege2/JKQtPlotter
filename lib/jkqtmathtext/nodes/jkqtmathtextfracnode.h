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




#ifndef JKQTMATHTEXTFRACNODE_H
#define JKQTMATHTEXTFRACNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include <QPainter>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT


/** \brief subclass representing a \\frac node
 *  \ingroup jkqtmathtext_items
 *
 *  all fractions re drawn like this:
 *  \image html jkqtmathtext_fracnode_geo.png
 *
 *  slanted fractions are drawn like this:
 *  \image html jkqtmathtext_sfracnode_geo.png
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextFracNode: public JKQTMathTextDualChildNode {
    public:
        JKQTMathTextFracNode(JKQTMathText* parent, JKQTMathTextNode* child_top, JKQTMathTextNode* child_bottom, JKQTMathTextFracMode mode);
        virtual ~JKQTMathTextFracNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) override;
        /** \copydoc mode */ 
        JKQTMathTextFracMode getMode() const;
    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual void getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        /** \brief actual display type of fraction object */
        JKQTMathTextFracMode mode;
        /** \brief returns the nesting level of the node (of same type of \a sameType \c ==true) */
        int getNestingLevel(bool sameType=false) const;
        /** \brief determines the scaling factor of the fraction (takes into account the nesting level) */
        double getFracScalingFactor() const;
};


#endif // JKQTMATHTEXTFRACNODE_H









