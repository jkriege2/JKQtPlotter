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
 *  \image html jkqtmathtext/jkqtmathtext_fracnode_geo.png
 *
 *  slanted fractions are drawn like this:
 *  \image html jkqtmathtext/jkqtmathtext_sfracnode_geo.png
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextFracNode: public JKQTMathTextDualChildNode {
    public:

        /** \brief type of ffractions represented by JKQTMathTextFracNode
         *  \see JKQTMathTextFracNode, JKQTMathTextFracNode::FracType2String()
         */
        enum FracType {
            MTFMfrac,  /*!< \brief normal fraction \image html jkqtmathtext/MTFMfrac.png */
            MTFMdfrac,  /*!< \brief normal fraction, without scaling of under/over text \image html jkqtmathtext/MTFMdfrac.png */
            MTFMtfrac,  /*!< \brief text fraction (smaller than MTFMfrac) \image html jkqtmathtext/MTFMtfrac.png */
            MTFMsfrac,  /*!< \brief slanted fraction \image html jkqtmathtext/MTFMsfrac.png */
            MTFMstfrac,  /*!< \brief slanted text fraction \image html jkqtmathtext/MTFMstfrac.png */
            MTFMunderbrace,  /*!< \brief curly underbrace \image html jkqtmathtext/MTFMunderbrace.png */
            MTFMoverbrace,  /*!< \brief curly overbrace \image html jkqtmathtext/MTFMoverbrace.png */
            MTFMunderbracket,  /*!< \brief square underbracket \image html jkqtmathtext/MTFMunderbracket.png */
            MTFMoverbracket,  /*!< \brief square overbracket \image html jkqtmathtext/MTFMoverbracket.png */
            MTFMstackrel,  /*!< \brief binom/fraction without line \image html jkqtmathtext/MTFMstackrel.png */
            MTFMunderset,  /*!< \brief underset text \image html jkqtmathtext/MTFMunderset.png */
            MTFMoverset  /*!< \brief overset text \image html jkqtmathtext/MTFMoverset.png */
        };

        /** \brief convert a JKQTMathTextFracNode::FracType into a QString
         *  \see JKQTMathTextFracNode::FracType
         */
        static QString FracType2String(FracType mode);

        /** \brief returns the FracType corresponding to \a instructionName
         *  \see JKQTMathTextFracNode::FracType
         */
        static FracType InstructionName2FracType(const QString& mode);

        /** \brief returns true, if the given \a instructionName can be converted to a FracType
         *  \see JKQTMathTextFracNode::FracType
         */
        static bool supportsInstructionName(const QString& instructionName);


        JKQTMathTextFracNode(JKQTMathText* parent, JKQTMathTextNode* child_top, JKQTMathTextNode* child_bottom, JKQTMathTextFracNode::FracType mode);
        virtual ~JKQTMathTextFracNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;
        /** \copydoc mode */ 
        JKQTMathTextFracNode::FracType getMode() const;
    protected:
        /** \brief lists all supported instructions */
        static QHash<QString, FracType> instructions;
        /** \brief fills instructions */
        static void fillInstructions();
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;
        /** \brief actual display type of fraction object */
        JKQTMathTextFracNode::FracType mode;
        /** \brief returns the nesting level of the node (of same type of \a sameType \c ==true) */
        int getNestingLevel(bool sameType=false) const;
        /** \brief determines the scaling factor of the fraction (takes into account the nesting level) */
        double getFracScalingFactor() const;
};


#endif // JKQTMATHTEXTFRACNODE_H









