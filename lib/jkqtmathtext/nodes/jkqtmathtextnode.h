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




#ifndef JKQTMATHTEXTNODE_H
#define JKQTMATHTEXTNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include <QPainter>

class JKQTMathText; // forward

/** \brief subclass representing one node in the syntax tree
 *  \ingroup jkqtmathtext_items
 *
 * \image html jkqtmathtext_node_geo.png
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextNode {
    public:
        explicit JKQTMathTextNode(JKQTMathText* parentMathText);
        JKQTMathTextNode(const JKQTMathTextNode&)=delete;
        JKQTMathTextNode& operator=(const JKQTMathTextNode&)=delete;
        virtual ~JKQTMathTextNode();
        /** \brief determine the size of the node, calls getSizeInternal() implementation of the actual type \see getSizeInternal()
         *
         * \param painter painter to use for determining the size
         * \param currentEv current environment object
         * \param[out] width width of the block/node
         * \param[out] baselineHeight distance from the bottom of the block/node-box to the baseline
         * \param[out] overallHeight overall height (bottom to top) of the node, the ascent is \c overallHeight-baselineHeight
         * \param[out] strikeoutPos position of the strikeout-line
         * \param[in] prevNodeSize optional parameter, describing the size of the previous node (on the left). This may be used for layout of some nodes (e.g. sub/super to move correctly next to large parantheses ...)
         *
         */
        void getSize(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize=nullptr);
        /** \brief calculates the x-size-difference between the given (probably) italic (width externally calculated: \A width_potentiallyitalic, \a ev_potentiallyitalic) and the non-italic version of \a child */
        double getNonItalicXCorretion(QPainter &painter, double width_potentiallyitalic, const JKQTMathTextEnvironment &ev_potentiallyitalic, JKQTMathTextNode* child) const;
        /** \brief draw the contents at the designated position
         *
         * \param painter QPainter to use
         * \param x x-position, where the drawing starts [Pixel]
         * \param y Y-position of the baseline, where the drawing starts [Pixel]
         * \param currentEv JKQTMathTextEnvironment object describing the current drawing environment/settings
         * \param[in] prevNodeSize optional parameter, describing the size of the previous node (on the left). This may be used for layout of some nodes (e.g. sub/super to move correctly next to large parantheses ...)
         * \return the x position which to use for the next part of the text
         */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize=nullptr)=0;
        /** \brief convert node to HTML and returns \c true on success
         * \param[out] html new HTML code is APPENDED to this string
         * \param currentEv JKQTMathTextEnvironment object describing the current drawing environment/settings
         * \param defaultEv JKQTMathTextEnvironment object describing the default drawing environment/settings when starting to interpret a node tree
         * \return \c true on success
         */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv);

        /** \brief returns the drawing of colored boxes (for DEBUGGING) around the actual output of the node is enabled */
        bool getDrawBoxes() const;
        /** \brief enables the drawing of colored boxes (for DEBUGGING) around the actual output of the node */
        virtual void setDrawBoxes(bool draw);
        /** \brief return the name of this class as a string */
        virtual QString getTypeName() const;
        /** \copydoc parentNode */
        void setParentNode(JKQTMathTextNode* node);
        /** \copydoc parentNode */
        JKQTMathTextNode* getParentNode();
        /** \copydoc parentNode */
        const JKQTMathTextNode* getParentNode() const;
    protected:
        /** \brief determine the size of the node, overwrite this function in derived classes
         *
         * \param painter painter to use for determining the size
         * \param currentEv current environment object
         * \param[out] width width of the block/node
         * \param[out] baselineHeight distance from the bottom of the block/node-box to the baseline
         * \param[out] overallHeight overall height (bottom to top) of the node, the ascent is \c overallHeight-baselineHeight
         * \param[out] strikeoutPos position of the strikeout-line
         * \param[in] prevNodeSize optional parameter, describing the size of the previous node (on the left). This may be used for layout of some nodes (e.g. sub/super to move correctly next to large parantheses ...)
         *
         */
        virtual void getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize=nullptr)=0;

        /** \brief parent JKQTMathText object (required for several drawing operations */
        JKQTMathText* parentMathText;
        /** \brief parent node of this node (i.e. one level up, ode \c nullptr ) */
        JKQTMathTextNode* parentNode;
        /** \brief enables the drawing of colored boxes (for DEBUGGING) around the actual output of the node */
        bool drawBoxes;
        /** \brief draws colored boxes (for DEBUGGING) around the actual output of the node
         *
         * \param painter QPainter to use
         * \param x x-position, where the drawing starts [Pixel]
         * \param y Y-position of the baseline, where the drawing starts [Pixel]
         * \param currentEv JKQTMathTextEnvironment object describing the current drawing environment/settings
         */
        void doDrawBoxes(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv);


        /** \brief returns the list of parent, parent-of-parent, ... that can be cast to type \a T */
        template<class T>
        inline QList<T*> getParents() {
            QList<T*> lst;
            JKQTMathTextNode* p=getParentNode();
            T* pT=dynamic_cast<T*>(p);
            while (p!=nullptr) {
                if (pT!=nullptr) lst.append(pT);
                p=p->getParentNode();
                pT=dynamic_cast<T*>(p);
            }
            return lst;
        }

        /** \brief returns the list of parent, parent-of-parent, ... that can be cast to type \a T */
        template<class T>
        inline QList<const T*> getParents() const {
            QList<const T*> lst;
            const JKQTMathTextNode* p=getParentNode();
            const T* pT=dynamic_cast<const T*>(p);
            while (p!=nullptr) {
                if (pT!=nullptr) lst.append(pT);
                p=p->getParentNode();
                pT=dynamic_cast<const T*>(p);
            }
            return lst;
        }
};


/** \brief subclass representing a node in the syntax tree, that has one child
 *  \ingroup jkqtmathtext_items
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextSingleChildNode: public JKQTMathTextNode {
    public:
        explicit JKQTMathTextSingleChildNode(JKQTMathTextNode* _child, JKQTMathText* parentMathText);
        virtual ~JKQTMathTextSingleChildNode() override;

        /** \copydoc child */
        JKQTMathTextNode* getChild();
        /** \copydoc child */
        const JKQTMathTextNode* getChild() const;
        /** \copydoc JKQTMathTextNode::setDrawBoxes() */
        virtual void setDrawBoxes(bool draw) override;
    protected:
        /** \brief child node of this node */
        JKQTMathTextNode* child;
};

/** \brief subclass representing a node in the syntax tree, that has two children
 *  \ingroup jkqtmathtext_items
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextDualChildNode: public JKQTMathTextNode {
    public:
        explicit JKQTMathTextDualChildNode(JKQTMathTextNode* _child1, JKQTMathTextNode* _child2, JKQTMathText* parentMathText);
        virtual ~JKQTMathTextDualChildNode() override;

        /** \copydoc child1 */
        JKQTMathTextNode* getChild1();
        /** \copydoc child1 */
        const JKQTMathTextNode* getChild1() const;
        /** \copydoc child2 */
        JKQTMathTextNode* getChild2();
        /** \copydoc child2 */
        const JKQTMathTextNode* getChild2() const;
        /** \copydoc JKQTMathTextNode::setDrawBoxes() */
        virtual void setDrawBoxes(bool draw) override;
    protected:
        /** \brief first child node of this node */
        JKQTMathTextNode* child1;
        /** \brief second child node of this node */
        JKQTMathTextNode* child2;
};

#endif // JKQTMATHTEXTNODE_H
