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
  */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextBraceNode: public JKQTMathTextNode {
    public:
        JKQTMathTextBraceNode(JKQTMathText* parent, const QString& openbrace, const QString& closebrace, JKQTMathTextNode* child, bool showRightBrace=true);
        virtual ~JKQTMathTextBraceNode() override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) override;
        /** \copydoc JKQTMathTextNode::setDrawBoxes() */
        virtual void setDrawBoxes(bool draw) override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \brief returns the child node  */
        inline JKQTMathTextNode* getChild() const {
            return this->child; 
        }
        /** \copydoc openbrace */ 
        inline QString getOpenbrace() const { 
            return this->openbrace; 
        }
        /** \copydoc closebrace */ 
        inline QString getClosebrace() const { 
            return this->closebrace; 
        }
        /** \copydoc showRightBrace */ 
        inline bool getShowRightBrace() const { 
            return this->showRightBrace; 
        }
    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual void getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        JKQTMathTextNode* child;
        QString openbrace;
        QString closebrace;
        bool showRightBrace;

        void getBraceWidth(QPainter& painter, JKQTMathTextEnvironment currentEv, double baselineHeight, double overallHeight, double& bracewidth, double& braceheight);
};

#endif // JKQTMATHTEXTBRACENODE_H









