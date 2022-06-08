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




#ifndef JKQTMATHTEXTINSTRUCTIONNODE_H
#define JKQTMATHTEXTINSTRUCTIONNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include <QPainter>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT

/** \brief subclass representing an instruction node with exactly one argument in the syntax tree
 *  \ingroup jkqtmathtext_items
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextInstruction1Node: public JKQTMathTextNode {
    public:
        explicit JKQTMathTextInstruction1Node(JKQTMathText* parent, const QString& name, JKQTMathTextNode* child, const QStringList& parameters=QStringList());
        virtual ~JKQTMathTextInstruction1Node() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        /** \brief convert node to HTML and returns \c true on success */
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) override;
        /** \copydoc JKQTMathTextNode::setDrawBoxes() */
        virtual void setDrawBoxes(bool draw) override;
        /** \brief returns the child node  */
        JKQTMathTextNode* getChild() const;
        /** \copydoc name */ 
        QString getName() const;
        /** \copydoc parameters */ 
        QStringList getParameters() const;
    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual void getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        bool setupMTenvironment(JKQTMathTextEnvironment &ev);

        JKQTMathTextNode* child;
        QString name;
        QStringList parameters;
};





#endif // JKQTMATHTEXTINSTRUCTIONNODE_H









