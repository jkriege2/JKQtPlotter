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




#ifndef JKQTMATHTEXTDECORATEDNODE_H
#define JKQTMATHTEXTDECORATEDNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include <QPainter>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT


/** \brief subclass representing a decorated text m (e.g. \c \\vec \c \\hat ...) node
 *  \ingroup jkqtmathtext_items
 *
 *  \image html jkqtmathtext/decoration_sizing.png
 *
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextDecoratedNode: public JKQTMathTextSingleChildNode {
    public:
        /** \brief types of decoration available in a JKQTMathTextDecoratedNode
         */
        enum DecorationType {
            MTDvec,  /*!< \brief vector arrow over block \image html jkqtmathtext/MTDvec.png */
            MTDhat,  /*!< \brief small hat over block \image html jkqtmathtext/MTDhat.png */
            MTDwidehat,  /*!< \brief full-width hat over block \image html jkqtmathtext/MTDwidehat.png */
            MTDcheck,  /*!< \brief small v over block \image html jkqtmathtext/MTDcheck.png */
            MTDwidecheck,  /*!< \brief full-width v over block \image html jkqtmathtext/MTDwidecheck.png */
            MTDbreve,  /*!< \brief small tilde over block \image html jkqtmathtext/MTDbreve.png */
            MTDocirc,  /*!< \brief single circle over block \image html jkqtmathtext/MTDocirc.png */
            MTDdot,  /*!< \brief single dot over block \image html jkqtmathtext/MTDvec.png */
            MTDddot,  /*!< \brief double dot over block \image html jkqtmathtext/MTDddot.png */
            MTDbar,  /*!< \brief bar over block \image html jkqtmathtext/MTDbar.png */
            MTDarrow,  /*!< \brief arrow over block \image html jkqtmathtext/MTDarrow.png */
            MTDoverline,  /*!< \brief overline over block \image html jkqtmathtext/MTDoverline.png */
            MTDdoubleoverline,  /*!< \brief double overline over block \image html jkqtmathtext/MTDdoubleoverline.png */
            MTDunderline,  /*!< \brief underline under block \image html jkqtmathtext/MTDunderline.png */
            MTDdoubleunderline,  /*!< \brief double underline under block \image html jkqtmathtext/MTDdoubleunderline.png */
            MTDtilde,  /*!< \brief small tilde over block \image html jkqtmathtext/MTDtilde.png */
            MTDwidetilde,  /*!< \brief full width tilde over block \image html jkqtmathtext/MTDwidetilde.png */
            MTDcancel,  /*!< \brief cancel text with sloped line \image html jkqtmathtext/MTDcancel.png */
            MTDbcancel,  /*!< \brief cancel text with backward sloped line \image html jkqtmathtext/MTDbcancel.png */
            MTDxcancel,  /*!< \brief cancel text with X \image html jkqtmathtext/MTDxcancel.png */
            MTDstrike  /*!< \brief strikethrough text \image html jkqtmathtext/MTDstrike.png */
        };
        /** \brief convert a DecorationType into a string
         */
        static QString DecorationType2String(DecorationType mode);
        /** \brief returns the FracType corresponding to \a instructionName
         *  \see JKQTMathTextFracNode::FracType
         */
        static DecorationType InstructionName2DecorationType(const QString& mode);

        /** \brief returns true, if the given \a instructionName can be converted to a FracType
         *  \see JKQTMathTextFracNode::FracType
         */
        static bool supportsInstructionName(const QString& instructionName);

        JKQTMathTextDecoratedNode(JKQTMathText* parent, DecorationType decoration, JKQTMathTextNode* child);
        virtual ~JKQTMathTextDecoratedNode() override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override ;
        /** \copydoc decoration */ 
        DecorationType getDecoration() const;
    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual void getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        /** \brief type of decoration that is added to the child node */
        DecorationType decoration;
        /** \brief lists all supported instructions */
        static QHash<QString, DecorationType> instructions;
        /** \biref fills instructions */
        static void fillInstructions();
};
#endif // JKQTMATHTEXTDECORATEDNODE_H









