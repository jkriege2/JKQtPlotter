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




#ifndef JKQTMATHTEXTBOXINSTRUCTIONNODE_H
#define JKQTMATHTEXTBOXINSTRUCTIONNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextinstructionnode.h"
#include <QPainter>
#include <functional>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT


/** \brief subclass representing an instruction node which draws a (possibly colored) box around it's contents
 *         i.e. it represents instructions like \c \\fbox{...}, \c \\colorbox{color}{...}, ...
 *  \ingroup jkqtmathtext_items
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextBoxInstructionNode: public JKQTMathTextInstruction1Node {
    public:
        explicit JKQTMathTextBoxInstructionNode(JKQTMathText* parent, const QString& name, JKQTMathTextNode* child, const QStringList& parameters=QStringList());
        virtual ~JKQTMathTextBoxInstructionNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;

        /** \brief returns true, if the given \a instructionName can be represented by this node
         *  \see instructions
         */
        static bool supportsInstructionName(const QString& instructionName);
        /** \brief returns the number of additional string parameters, required for the given \a instructionName
         *  \see instructions
         */
        static size_t countParametersOfInstruction(const QString& instructionName);
        /** \brief sets \a insideMath to \c true if inside the node is to be parsed in math mode and \c false else
         *  \see instructions
         */
        static void modifyInMathEnvironment(const QString& instructionName, bool& insideMath, bool &insideMathTextStyle, const QStringList &params=QStringList());
    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;
        /** \brief set all properties in \a ev, as appropriate for the represented instruction */
        bool setupMTenvironment(JKQTMathTextEnvironment &ev) const;

        /** \brief defines the implementation of an instruction represented by JKQTMathTextModifiedTextPropsInstructionNode */
        struct InstructionProperties {
            /** \brief this functor implements the instruction */
            typedef std::function<void(JKQTMathTextEnvironment& ev, const QStringList& parameters)> ModifyEnvironmentFunctor;
            static ModifyEnvironmentFunctor NoModification;
            /** \brief this functor returns the QPen to use for the box outline */
            typedef std::function<QPen(JKQTMathTextEnvironment& ev, const QStringList& parameters, JKQTMathText* parent)> GetBoxPenFunctor;
            /** \brief generates a QPen with the lineWidth associated with the QFont of the environment (using QFontMetricsF::lineWidth() ) */
            static GetBoxPenFunctor DefaultPen;
            /** \brief generates an invisible pen with 0 width */
            static GetBoxPenFunctor NoPen;
            /** \brief this functor returns the QBrush to use for the box fill */
            typedef std::function<QBrush(JKQTMathTextEnvironment& ev, const QStringList& parameters, JKQTMathText* parent)> GetBoxBrushFunctor;
            /** \brief generates an invisible QBrush */
            static GetBoxBrushFunctor NoBrush;
            /** \brief default padding factor 0.5 xWidth */
            static double DefaultPadding;
            /** \brief default constructor, creates a NOP-instruction that does nothing */
            InstructionProperties();
            /** \brief constructor  */
            InstructionProperties(const ModifyEnvironmentFunctor& _modifier, const GetBoxPenFunctor& _pen, const GetBoxBrushFunctor& _brush, double _paddingFactor=DefaultPadding, size_t _NParams=0);
            /** \brief constructor  */
            InstructionProperties(const GetBoxPenFunctor& _pen, const GetBoxBrushFunctor& _brush, double _paddingFactor=DefaultPadding, size_t _NParams=0);
            /** \brief number of parameters for this node */
            size_t NParams;
            /** \brief functor that modifies a JKQTMathTextEnvironment */
            ModifyEnvironmentFunctor modifier;
            /** \brief functor that creates a QPen for the box outline */
            GetBoxPenFunctor pen;
            /** \brief functor that creates a QBrush for the box fill */
            GetBoxBrushFunctor brush;
            /** \brief padding of the box, as a factor to xWidth of the current font */
            double paddingFactor;
            /** \brief draw a double-line */
            bool doubleLine;
            /** \brief rounding radius (rx=ry) of the box, as a factor to xWidth of the current font */
            double roundingFactor;
        };

        /** \brief defines all implemented instructions in this node
         *
         *  \note this is the customization point for new instructions!
         */
        static const QHash<QString, InstructionProperties>& instructions();
};





#endif // JKQTMATHTEXTBOXINSTRUCTIONNODE_H









