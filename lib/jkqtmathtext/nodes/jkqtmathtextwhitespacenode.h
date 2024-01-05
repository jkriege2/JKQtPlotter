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




#ifndef JKQTMATHTEXTWHITESPACENODE_H
#define JKQTMATHTEXTWHITESPACENODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextinstructionnode.h"
#include <QPainter>

class JKQTMathText; // forward


/** \brief subclass representing one whitepsace node in the syntax tree
 *  \ingroup jkqtmathtext_items
 *
 *  It can represent different types of spaces, see Types and different multiples of that space.
 *  so \c JKQTMathTextWhitespaceNode(WST1em,2,parent) will represent two 1em wide spaces, i.e. a 2em space.
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextWhitespaceNode: public JKQTMathTextNode {
    public:
        enum Types {
            WSTNormal,  /*!< \brief a normal-width whitespace */
            WSTNonbreaking,  /*!< \brief a normal-width, non-breaking whitespace */
            WST1en,  /*!< \brief 1en (= 0.5 * pointsize of the font) whitespace */
            WST1em,  /*!< \brief 1em (= pointsize of the font) whitespace */
            WSTQuad=WST1em,  /*!< \copydoc WST1em */
            WSThair,  /*!< \brief hair (thinner than thin) whitespace (1/12 em = pointsize/12) */
            WSTthin,  /*!< \brief thin whitespace (1/6 em = pointsize/6) */
            WSTnegthin,  /*!< \brief negative thin whitespace (-1/6 em = -pointsize/6) */
            WSTmedium,  /*!< \brief medium whitespace (2/9 em = pointsize*2/9) */
            WSTnegmedium,  /*!< \brief negative medium whitespace (-2/9 em = -pointsize*2/9) */
            WSTthick,  /*!< \brief thick whitespace (5/18 em = pointsize*5/18) */
            WSTnegthick,  /*!< \brief negative thick whitespace (-5/18 em = -pointsize*5/18) */
            WSTthicker,  /*!< \brief thicker whitespace (1/3 em = pointsize/3) */

        };
        /** \brief converts Types \a type into a string */
        static QString Type2String(Types type);
        /** \brief converts Types \a type into its width in pixels, based on \a currentEv and \a pd */
        double Type2PixelWidth(Types type, JKQTMathTextEnvironment currentEv, QPaintDevice *pd) const;
        /** \brief checks whether a given LaTeX instruction name is supported by this node class type */
        static bool supportsInstructionName(const QString& instruction);
        /** \brief constructs a node with count=1 and type=WSTNormal */
        explicit JKQTMathTextWhitespaceNode(JKQTMathText* parent);
        /** \brief constructs a node with count=1 and the width derived from the instruction name \a type */
        explicit JKQTMathTextWhitespaceNode(const QString& type, JKQTMathText* parent);
        /** \brief constructs a node with the specified \a count and the width derived from the instruction name \a type */
        explicit JKQTMathTextWhitespaceNode(const QString& type, size_t count, JKQTMathText* parent);
        /** \brief constructs a node with count=1 and the given \a  type */
        explicit JKQTMathTextWhitespaceNode(Types type, JKQTMathText* parent);
        /** \brief constructs a node with the given \a type and \a count */
        explicit JKQTMathTextWhitespaceNode(Types type, size_t cound, JKQTMathText* parent);
        virtual ~JKQTMathTextWhitespaceNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;
        /** \copydoc WhitespaceProps::type */
        Types getWhitespaceType() const;
        /** \copydoc WhitespaceProps::count */
        size_t getWhitespaceCount() const;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;
        /** \brief describes a whitespace */
        struct WhitespaceProps {
            WhitespaceProps(Types type=WSTNormal, size_t count=1);
            WhitespaceProps(const WhitespaceProps& other);
            WhitespaceProps& operator=(const WhitespaceProps& other);
            /** \brief type of the whitespace represented by this node */
            Types type;
            /** \brief number of whitespaces of the given type, represented by this node (default: 1) */
            size_t count;
        };
        /** \brief properties of the whitespace represented by this node */
        WhitespaceProps whitespace;
        /** \brief converts Types \a type into its HTML representation */
        static QString Type2HTML(Types type);
        /** \brief translation table between latex instruction and WhitespaceProps
         *
         *  \note This is a customization point for additional whitespace instructions!
         */
        static const QHash<QString, WhitespaceProps>& supportedInstructions();
};


/** \brief subclass representing an empty bbox with defined width/height in the syntax tree
 *  \ingroup jkqtmathtext_items
 *
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextEmptyBoxNode: public JKQTMathTextNode {
    public:
        enum Units {
            EBUem,  /*!< \brief 1em = width('M') */
            EBUex,  /*!< \brief 1ex = xHeight */
        };
        /** \brief converts Types \a type into a string */
        static QString Units2String(Units type);
        /** \brief converts Types \a type into a string */
        static Units String2Units(QString type);
        /** \brief converts Types \a type into its width in pixels, based on \a currentEv and \a pd */
        double Units2PixelWidth(double value, Units unit, JKQTMathTextEnvironment currentEv, QPaintDevice *pd) const;
        /** \brief constructs a node */
        explicit JKQTMathTextEmptyBoxNode(JKQTMathText* parent, double width_, Units widthUnit_, double height_, Units heightUnit_);
        virtual ~JKQTMathTextEmptyBoxNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;
        /** \copydoc widthUnit */
        Units getWidthUnit() const;
        /** \copydoc width */
        double getWidth() const;
        /** \copydoc heightUnit */
        Units getHeightUnit() const;
        /** \copydoc height */
        double getHeight() const;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;
        /** \brief width of the (empty) box, units of this value defined in widthUnit */
        double width;
        /** \brief units to interpret width */
        Units widthUnit;
        /** \brief height of the (empty) box, units of this value defined in heightUnit */
        double height;
        /** \brief units to interpret height */
        Units heightUnit;
};




/** \brief generates whitespace with the size of the contained node
 *  \ingroup jkqtmathtext_items
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextPhantomNode: public JKQTMathTextInstruction1Node {
    public:
        /** \brief type of the phantom instrcution */
        enum Mode {
            FMwidthAndHeight, /*!< \brief implementes \c \\phantom{CHILD} which is whitespace in the width and height of \c CHILD */
            FMwidth, /*!< \brief implementes \c \\hphantom{CHILD} which is whitespace in the width of \c CHILD and height 0. */
            FMheight /*!< \brief implementes \c \\vphantom{CHILD} which is whitespace in the height of \c CHILD and width  0. */
        };
        /** \brief convert a Mode into a LaTeX instruction name */
        static QString Mode2Instruction(Mode mode);
        explicit JKQTMathTextPhantomNode(JKQTMathText* parent, const QString& mode, JKQTMathTextNode* child);
        explicit JKQTMathTextPhantomNode(JKQTMathText* parent, Mode mode, JKQTMathTextNode* child);
        virtual ~JKQTMathTextPhantomNode() override;
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

    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;
        /** \brief defines all implemented instructions in this node
         *
         *  \note this is the customization point for new instructions!
          */
        static const QHash<QString, Mode>& instructions();
};
#endif // JKQTMATHTEXTWHITESPACENODE_H









