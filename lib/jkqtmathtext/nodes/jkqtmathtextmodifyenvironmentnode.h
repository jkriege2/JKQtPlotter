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




#ifndef JKQTMATHTEXTMODIFYTEXTPROPSINSTRUCTIONNODE_H
#define JKQTMATHTEXTMODIFYTEXTPROPSINSTRUCTIONNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextinstructionnode.h"
#include <QPainter>
#include <functional>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT

/** \brief subclass representing an instruction node which modifies the current
 *         font (as defined in JKQTMathTextEnvironment), for it's child  node,
 *         i.e. it represents instructions like \c \\textbf{...}, \c \\ul{underlinedText}, ...
 *  \ingroup jkqtmathtext_items
 *
 *  \note Instructions that modify the text for any further nodes WITHIN the
 *  current block are represented by JKQTMathTextModifiedEnvironmentInstructionNode
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextModifiedTextPropsInstructionNode: public JKQTMathTextInstruction1Node {
    public:
        explicit JKQTMathTextModifiedTextPropsInstructionNode(JKQTMathText* parent, const QString& name, JKQTMathTextNode* child, const QStringList& parameters=QStringList());
        virtual ~JKQTMathTextModifiedTextPropsInstructionNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;

        /** \brief returns true, if the given \a instructionName can be represented by this node
         *  \see instructions()
         */
        static bool supportsInstructionName(const QString& instructionName);
        /** \brief returns the number of additional string parameters, required for the given \a instructionName
         *  \see instructions()
         */
        static size_t countParametersOfInstruction(const QString& instructionName);
        /** \brief sets \a insideMath to \c true if inside the node is to be parsed in math mode and \c false else
         *  \see instructions()
         */
        static void modifyInMathEnvironment(const QString& instructionName, bool& insideMath, bool &insideMathTextStyle, const QStringList &params=QStringList());

    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;
        /** \brief defines the implementation of an instruction represented by JKQTMathTextModifiedTextPropsInstructionNode */
        struct InstructionProperties {
            /** \brief this functor implements the instruction */
            typedef std::function<void(JKQTMathTextEnvironment& ev, const QStringList& parameters)> ModifyEnvironmentFunctor;
            /** \brief default constructor, creates a NOP-instruction that does nothing */
            InstructionProperties();
            /** \brief constructor which gets a functor \a _modifier and a number of required parameters \a _NParams */
            InstructionProperties(const ModifyEnvironmentFunctor& _modifier, size_t _NParams=0);
            /** \brief number of parameters for this node */
            size_t NParams;
            /** \brief functor that modifies a JKQTMathTextEnvironment */
            ModifyEnvironmentFunctor modifier;
        };

        /** \brief defines all implemented instructions in this node
         *
         *  \note this is the customization point for new instructions!
         */
        static const QHash<QString, InstructionProperties>& instructions();
        /** \brief executes the instruction on \a ev */
        void executeInstruction(JKQTMathTextEnvironment& ev) const;
};



/** \brief subclass representing an instruction node which modify the
 *         current JKQTMathTextEnvironment, in the current block. This
 *         implements functions like \c \\bf or \c \\color{COL}
 *  \ingroup jkqtmathtext_items
 *
 *  \see JKQTMathTextModifyEnvironmentNodeMixIn
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextModifiedEnvironmentInstructionNode: public JKQTMathTextNonDrawingBaseNode, public JKQTMathTextModifyEnvironmentNodeMixIn {
    public:
        explicit JKQTMathTextModifiedEnvironmentInstructionNode(JKQTMathText* parent_, const QString& instructionName_, const QStringList& parameters_=QStringList());
        virtual ~JKQTMathTextModifiedEnvironmentInstructionNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc instructionName */
        const QString& getInstructionName() const;
        /** \copydoc parameters */
        const QStringList& getParameters() const;
        /** \copydoc JKQTMathTextModifyEnvironmentNodeMixIn::modifyEnvironment() */
        virtual void modifyEnvironment(JKQTMathTextEnvironment& currentEv) const override;

        /** \brief returns true, if the given \a instructionName can be represented by this node
         *  \see instructions()
         */
        static bool supportsInstructionName(const QString& instructionName);
        /** \brief returns the number of additional string parameters, required for the given \a instructionName
         *  \see instructions()
         */
        static size_t countParametersOfInstruction(const QString& instructionName);
        /** \brief sets \a insideMathTextStyle to \c true if textstyle is set inside math
         *  \see instructions()
         */
        static void modifyInMathTextStyleEnvironment(const QString& instructionName, bool &insideMathTextStyle, JKQTMathText *parentMathText, const QStringList &params=QStringList());
    protected:
        /** \brief instruction name */
        QString instructionName;
        /** \brief additional string-parameters */
        QStringList parameters;
        /** \brief defines the implementation of an instruction represented by JKQTMathTextModifiedTextPropsInstructionNode */
        struct InstructionProperties {
            /** \brief this functor implements the instruction */
            typedef std::function<void(JKQTMathTextEnvironment& ev, const QStringList& parameters, JKQTMathText* parentMathText)> ModifyEnvironmentFunctor;
            /** \brief default constructor, creates a NOP-instruction that does nothing */
            InstructionProperties();
            /** \brief constructor which gets a functor \a _modifier and a number of required parameters \a _NParams */
            InstructionProperties(const ModifyEnvironmentFunctor& _modifier, size_t _NParams=0);
            /** \brief number of parameters for this node */
            size_t NParams;
            /** \brief functor that modifies a JKQTMathTextEnvironment */
            ModifyEnvironmentFunctor modifier;
        };

        /** \brief defines all implemented instructions in this node
         *
         *  \note this is the customization point for new instructions!
         */
        static const QHash<QString, InstructionProperties>& instructions();
};




#endif // JKQTMATHTEXTMODIFYTEXTPROPSINSTRUCTIONNODE_H









