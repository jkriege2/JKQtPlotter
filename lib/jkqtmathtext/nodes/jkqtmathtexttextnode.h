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




#ifndef JKQTMATHTEXTTEXTNODE_H
#define JKQTMATHTEXTTEXTNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include <QPainter>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT


/** \brief subclass representing one text node in the syntax tree
 *  \ingroup jkqtmathtext_items
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextTextNode: public JKQTMathTextNode {
    public:
        explicit JKQTMathTextTextNode(JKQTMathText* parent, const QString& text, bool addWhitespace, bool stripInnerWhitepace=false);
        virtual ~JKQTMathTextTextNode() override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) override;
        /** \copydoc text */ 
        QString getText() const;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override ;
    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual void getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        /** \brief text-contents of the node */
        QString text;
        /** \brief transforms the text before sizing/drawing (may e.g. exchange special letters for other unicode symbols etc.) */
        virtual QString textTransform(const QString& text, JKQTMathTextEnvironment currentEv, bool forSize=false);
};

/** \brief subclass representing one text node in the syntax tree
 *  \ingroup jkqtmathtext_items
 */
class JKQTMATHTEXT_LIB_EXPORT MTplainTextNode: public JKQTMathTextTextNode {
    public:
        explicit MTplainTextNode(JKQTMathText* parent, const QString& text, bool addWhitespace, bool stripInnerWhitepace=false);
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
    protected:
        /** \copydoc JKQTMathTextTextNode::textTransform() */
        virtual QString textTransform(const QString& text, JKQTMathTextEnvironment currentEv, bool forSize=false) override;
};
/** \brief subclass representing one whitepsace node in the syntax tree
 *  \ingroup jkqtmathtext_items
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextWhitespaceNode: public JKQTMathTextTextNode {
    public:
        explicit JKQTMathTextWhitespaceNode(JKQTMathText* parent);
        virtual ~JKQTMathTextWhitespaceNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) override;
};

#endif // JKQTMATHTEXTTEXTNODE_H









