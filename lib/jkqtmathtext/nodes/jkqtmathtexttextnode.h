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


/** \brief base class for nodes representing text in the syntax tree
 *  \ingroup jkqtmathtext_items
 *
 *  This node is a collection of tools, necessary to draw text. It
 *  is the base for nodes, such as:
 *    - JKQTMathTextTextNode
 *    - JKQTMathTextVerbatimNode
 *  .
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextTextBaseNode: public JKQTMathTextNode {
    public:
        explicit JKQTMathTextTextBaseNode(JKQTMathText* parent, const QString& text);
        virtual ~JKQTMathTextTextBaseNode() override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;
        /** \copydoc text */
        QString getText() const;
    protected:
        /** \brief text-contents of the node */
        QString text;
        /** \brief transforms the \a text before sizing/drawing (may e.g. exchange special letters for other unicode symbols etc.) */
        virtual QString textTransform(const QString& text, const JKQTMathTextEnvironment& currentEv) const;
};


/** \brief subclass representing one text node in the syntax tree
 *  \ingroup jkqtmathtext_items
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextTextNode: public JKQTMathTextTextBaseNode {
    public:
        explicit JKQTMathTextTextNode(JKQTMathText* parent, const QString& text, bool addWhitespace, bool stripInnerWhitepace=false);
        virtual ~JKQTMathTextTextNode() override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override ;
        /** \brief remove trailing whitespace, is used by simplifyJKQTMathTextNode() */
        void removeTrailingWhitespace();
        /** \brief remove leading whitespace, is used by simplifyJKQTMathTextNode()  */
        void removeLeadingWhitespace();
    protected:
        /** \brief defines how a character shold be drawn, used by splitTextForLayout */
        enum FontMode {
            FMasDefined,    /*!< \brief use current font */
            FMasDefinedForceUpright,   /*!< \brief  use current font, but force it upright (e.g. for digits in math mode) */
            FMasDefinedOutline,   /*!< \brief use current font and draw as outline, e.g. used for simulating  blackboard fonts  */
            FMroman,   /*!< \brief use JKQTMathText::getFontRoman() */
            FMfallbackSymbol,   /*!< \brief use JKQTMathText::getFallbackFontSymbols() */
        };
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;

        /** \brief describes the layout of the whole node */
        struct LayoutInfo: public JKQTMathTextNodeSize {
            LayoutInfo();
            LayoutInfo(const LayoutInfo& other);
            LayoutInfo(const JKQTMathTextNodeSize& other);
            LayoutInfo& operator=(const JKQTMathTextNodeSize& other);
            LayoutInfo& operator=(const LayoutInfo& other);
            /** \brief the text that shall be printed is split up into different parts (with different formatting each) */
            QStringList textpart;
            /** \brief formatting for each entry in textpart */
            QList<FontMode> fontMode;
            /** \brief drawing x-position for each entry in textpart */
            QList<double> textpartXPos;
        };
        /** \brief calculates the size of the node, much like JKQTMathTextNode::getSizeInternal(), but returns additional properties that can be reused for drawing */
        LayoutInfo calcLayout(QPainter& painter, JKQTMathTextEnvironment currentEv) const ;
        /** \brief split text for Math-Modelayout into sections, where each section has a defined way of output
         *
         *  \param painter the QPainter to use for sizing/drawing
         *  \param currentEv the environment that defines the formatting of the text
         *  \param txt the text to split up
         *  \param[out] textpart the input \A txt split up into sections
         *  \param[out] fontMode formating of each section in \a textpart
         */
        void splitTextForLayout(QPainter &painter, JKQTMathTextEnvironment currentEv, const QString& txt, QStringList& textpart, QList<FontMode>& fontMode) const;
        /** \brief translation table for blackboard-font characters from "normal" Latin-1 encoding to unicode-encoding of blackboards */
        static QHash<QChar, uint32_t> blackboardUnicodeTable;
        /** \brief fill static data */
        static void fillStaticTables();
        /** \copydoc JKQTMathTextTextBaseNode::textTransform() */
        virtual QString textTransform(const QString& text, const JKQTMathTextEnvironment& currentEv) const override;
};


/** \brief subclass representing a verbatim (plain-text) node with support for line-breaks in the syntax tree
 *  \ingroup jkqtmathtext_items
 *
 *  The layout of the lines can left-aligned, right-aligned or centered.
 *
 *  \image html jkqtmathtext_verticallist.png
 *
 *  \image html jkqtmathtext_verticalalignment.png
 *
 *  \image html jkqtmathtext_horizontalalignment.png
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextVerbatimNode: public JKQTMathTextTextBaseNode {
    public:
        explicit JKQTMathTextVerbatimNode(JKQTMathText* parent, const QString& text, bool visibleWhitespace=false, JKQTMathTextHorizontalAlignment _alignment=MTHALeft, double _linespacingFactor=1.0, JKQTMathTextVerticalOrientation _verticalOrientation=MTVOFirstLine, size_t tabSize_=4);
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc alignment */
        JKQTMathTextHorizontalAlignment getAlignment() const;
        /** \copydoc verticalOrientation */
        JKQTMathTextVerticalOrientation getVerticalOrientation() const;
        /** \copydoc lineSpacingFactor */
        double getLineSpacingFactor() const;
        /** \copydoc visibleWhitespace */
        bool getVisibleWhitespace() const;
        /** \copydoc tabSize */
        size_t getTabSize() const;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;
    protected:
        /** \brief alignment scheme used to lay out all lines
          *
          * \image html jkqtmathtext_horizontalalignment.png
          */
        JKQTMathTextHorizontalAlignment alignment;
        /** \brief spacing of the separate lines, as factor of the default line-spacing [Default: 1].
         *
         *  This property can be used to move the lines closer together or farther apart.
         *
         *  \image html jkqtmathtext_verticallist.png
         */
        double lineSpacingFactor;
        /** \brief vertical orientation of the baseline of the whole block (with respect to the single lines)
         *
         *  \image html jkqtmathtext_verticalorientation.png
         */
        JKQTMathTextVerticalOrientation verticalOrientation;
        /** \brief when \c true, whitespaces are displayed with a visible character */
        bool visibleWhitespace;
        /** \brief number of whitespaces, each tab character stands for */
        size_t tabSize;

        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;
        /** \brief sets all necessary settings in \a currentEv for drawing this node */
        virtual void transformEnvironment(JKQTMathTextEnvironment& currentEv) const;

        /** \brief describes the layout of the whole node */
        struct LayoutInfo: public JKQTMathTextNodeSize {
            LayoutInfo();
            /** \brief the text from JKQTMathTextVerbatimNode::text, split into lines */
            QStringList lines;
            /** \brief drawing position for each line */
            QList<QPointF> X;
        };
        /** \brief calclates the layout of the whole block/node
         *
         *  \note This function does NOT call transformEnvironment();
         *        it has to be called before calling this!
         */
        LayoutInfo calcLayout(QPainter& painter, const JKQTMathTextEnvironment& currentEv) const;
        /** \brief transforms the \a text before sizing/drawing (may e.g. exchange special letters for other unicode symbols etc.) */
        virtual QString textTransform(const QString& text, const JKQTMathTextEnvironment& currentEv) const override;
};


#endif // JKQTMATHTEXTTEXTNODE_H









