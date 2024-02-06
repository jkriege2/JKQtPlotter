/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)
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
#include <functional>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT

/** \brief subclass representing an instruction node with exactly one argument and possibly additional parameters in the syntax tree
 *         This is a base-class without concrete implementations ... Implementations can be found in derived classes!
 *  \ingroup jkqtmathtext_items
 *
 *  The parameters are sinple strings (e.g. directly for instructions like \c \\textcolor{color}{coloredLatexText},
 *  but they might also be translated into numbers or other properties.
 *
 *  Typically derived nodes will provide static methods to determine the number of instructions
 *  and whether an instruction name is supported.
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextInstruction1Node: public JKQTMathTextSingleChildNode {
    public:
        explicit JKQTMathTextInstruction1Node(JKQTMathText* parent, const QString& name, JKQTMathTextNode* child, const QStringList& parameters=QStringList());
        virtual ~JKQTMathTextInstruction1Node() override;
        /** \copydoc instructionName */
        const QString& getInstructionName() const;
        /** \copydoc parameters */
        const QStringList& getParameters() const;
    protected:
        /** \brief instruction name */
        QString instructionName;
        /** \brief additional string-parameters */
        QStringList parameters;
};



/** \brief subclass representing a simple instruction node which only accepts string arguments, not LaTeX arguments
 *         i.e. it represents instructions like \c \\unicode{...}, ...
 *  \ingroup jkqtmathtext_items
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextSimpleInstructionNode: public JKQTMathTextNode {
    public:
        explicit JKQTMathTextSimpleInstructionNode(JKQTMathText* parent, const QString& name, const QStringList& parameters=QStringList());
        virtual ~JKQTMathTextSimpleInstructionNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;
        /** \copydoc instructionName */
        const QString& getInstructionName() const;
        /** \copydoc parameters */
        const QStringList& getParameters() const;

        /** \brief returns true, if the given \a instructionName can be represented by this node
         *  \see instructions
         */
        static bool supportsInstructionName(const QString& instructionName);
        /** \brief returns the number of additional string parameters, required for the given \a instructionName
         *  \see instructions
         */
        static size_t countParametersOfInstruction(const QString& instructionName);

    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;
        /** \brief defines the implementation of an instruction represented by JKQTMathTextModifiedTextPropsInstructionNode */
        struct InstructionProperties {
            /** \brief this functor implements the instruction */
            typedef std::function<QString(const QStringList& parameters)> EvaluateInstructionFunctor;
            /** \brief default constructor, creates a NOP-instruction that does nothing */
            InstructionProperties();
            /** \brief constructor which gets a functor \a _modifier and a number of required parameters \a _NParams */
            InstructionProperties(const EvaluateInstructionFunctor& _evaluator, size_t _NParams=0);
            /** \brief number of parameters for this node */
            size_t NParams;
            /** \brief output of the instruction */
            EvaluateInstructionFunctor evaluator;
        };
        /** \brief defines all implemented instructions in this node
         *
         *  \note this is the customization point for new instructions!
         */
        static const QHash<QString, InstructionProperties>& instructions();
        /** \brief executes the instruction on \a ev */
        QString executeInstruction() const;
        /** \brief instruction name */
        QString instructionName;
        /** \brief additional string-parameters */
        QStringList parameters;
};



#endif // JKQTMATHTEXTINSTRUCTIONNODE_H









