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
  */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextBraceNode: public JKQTMathTextSingleChildNode {
    public:
        JKQTMathTextBraceNode(JKQTMathText* parent, const QString& openbrace, const QString& closebrace, JKQTMathTextNode* child, bool showOpeningBrace=true, bool showClosingBrace=true);
        virtual ~JKQTMathTextBraceNode() override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc openbrace */
        QString getOpenbrace() const;
        /** \copydoc closebrace */ 
        QString getClosebrace() const;
        /** \copydoc showRightBrace */
        bool getShowClosingBrace() const;
        /** \copydoc showOpeningBrace */
        bool getShowOpeningBrace() const;
    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal()
         *
         * \note This function internally calls getSizeInternalAndBrace() and returns part of its results.
         */
        virtual void getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        /** calculates the size of this node (also returned by getSizeInternal() ) and of the brace */
        void getSizeInternalAndBrace(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, double& bracewidth, double&braceheight, const JKQTMathTextNodeSize* prevNodeSize=nullptr);
        /**\brief opening brace */
        QString openbrace;
        /**\brief closing brace */
        QString closebrace;
        /**\brief if \c true, the closing (right hand side) brace is drawn */
        bool showClosingBrace;
        /**\brief if \c true, the opening (left hand side) brace is drawn */
        bool showOpeningBrace;
        /** \brief calculate the width of the brace */
        void getBraceSize(QPainter& painter, JKQTMathTextEnvironment currentEv, double baselineHeight, double overallHeight, double& bracewidth, double& braceheight) const;
};

#endif // JKQTMATHTEXTBRACENODE_H









