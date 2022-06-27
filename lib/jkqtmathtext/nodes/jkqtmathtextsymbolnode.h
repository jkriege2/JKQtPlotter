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




#ifndef JKQTMATHTEXTSYMBOLNODE_H
#define JKQTMATHTEXTSYMBOLNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include <QPainter>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT


/** \brief subclass representing one symbol (e.g. \c \\alpha , \c \\cdot ...) node in the syntax tree
 *  \ingroup jkqtmathtext_items
 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextSymbolNode: public JKQTMathTextNode {
    public:
        explicit JKQTMathTextSymbolNode(JKQTMathText* parent, const QString& name, bool addWhitespace);
        virtual ~JKQTMathTextSymbolNode() override;
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) override;
        /** \copydoc symbolName */ 
        QString getSymbolName() const;
        /** \brief get font name of the symbol */
        QString getSymbolfontName() const;
        /** \copydoc addWhitespace */
        bool getAddWhitespace() const;
        /** \brief determine the size of the node, calls getSizeInternal() implementation of the actual type \see getSizeInternal()
         *
         * \param painter painter to use for determining the size
         * \param currentEv current environment object
         * \param[out] width width of the block/node
         * \param[out] baselineHeight distance from the bottom of the block/node-box to the baseline
         * \param[out] overallHeight overall height (bottom to top) of the node, the ascent is \c overallHeight-baselineHeight
         * \param[out] strikeoutPos position of the strikeout-line
         * \param[out] subSuperXCorrection x-correction as described for JKQTMathParser::intsubsuper_xcorrection_factor for placing sub-/superscript below/above the symbol
         * \param[out] subBesidesXCorrection x-correction as described for JKQTMathParser::intsubbesides_xcorrection_xfactor for placing sub-/superscript below/above the symbol
         * \param[in] prevNodeSize optional parameter, describing the size of the previous node (on the left). This may be used for layout of some nodes (e.g. sub/super to move correctly next to large parantheses ...)
         *
         */
        void getSymbolSize(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, double& subSuperXCorrection, double& subBesidesXCorrection, const JKQTMathTextNodeSize* prevNodeSize=nullptr);
    protected:
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual void getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize=nullptr) override;
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual void getSymbolSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, double& subSuperXCorrection, double& subBesidesXCorrection, const JKQTMathTextNodeSize* prevNodeSize=nullptr) ;

        /** \brief this string will be sent to the drawText method with properly set fonts */
        QString symbolName;
        /** \brief add a whitespace to the symbol? */
        bool addWhitespace;
        struct SymbolProps {
            /** \brief the symbol name supplied to the constructor */
            QString symbol;
            /** \brief font to use for output */
            QString font;
            /** \brief magnification factor for the font size */
            double fontFactor;
            /** \brief 0: leave italic setting as is, >0: set italic, <0 set italic to false */
            char italic;
            /** \brief 0: leave bold setting as is, >0: set bold, <0 set bold to false */
            char bold;
            /** \brief this corrects the y position of a symbol: draws at y <- y+ height*yfactor) */
            double yfactor;
            /** \brief indicates whether to draw a bar (like for \c \\hbar ) */
            bool drawBar;
            bool heightIsAscent;
            bool exactAscent;
            bool extendWidthInMathmode;
        };
        /** \brief retrieve the properties to render the given symbol \a symName in the current environment \a currentEv */
        SymbolProps getSymbolProp(const QString& symName, const JKQTMathTextEnvironment& currentEv) const;
        /** \brief fill \a props for the symbol named \a n in the given environment \a currentEv and with the given \a mathFontFactor , returns \c true if the symbol can be drawn using Unicode font (or WinSymbol as Fallback)*/
        bool getSymbolProp(JKQTMathTextSymbolNode::SymbolProps &props, const QString &n, const JKQTMathTextEnvironment &currentEv, double mathFontFactor) const;
        /** \brief fill \a props for the greek letter symbol named \a n in the given environment \a currentEv and with the given \a mathFontFactor , returns \c true if the symbol can be drawn using Unicode font (or WinSymbol as Fallback) */
        bool getGreekSymbolProp(JKQTMathTextSymbolNode::SymbolProps &props, const QString &n, const JKQTMathTextEnvironment &currentEv, double mathFontFactor) const;
        /** \brief fill \a props for the symbol named \a n in the given environment \a currentEv and with the given \a mathFontFactor , returns \c true if the symbol can be drawn using WinSymbol font */
        bool getWinSymbolProp(JKQTMathTextSymbolNode::SymbolProps &props, const QString &n, const JKQTMathTextEnvironment &currentEv, double mathFontFactor) const;
        /** \brief fill \a props for the symbol named \a n  , returns \c true if the symbol can be drawn using any font, does not alter the font name!!! */
        bool getStandardTextSymbolProp(JKQTMathTextSymbolNode::SymbolProps &props, const QString &n) const;
        /** \brief fill \a props for the symbol named \a n  , returns \c true if the symbol can be drawn using any unicode font, does not alter the font name!!! */
        bool getUnicodeBaseSymbolProp(JKQTMathTextSymbolNode::SymbolProps &props, const QString &n) const;
        /** \brief fill \a props for the symbol named \a n  , returns \c true if the symbol can be drawn using a full unicode font, does not alter the font name!!! */
        bool getUnicodeFullSymbolProp(JKQTMathTextSymbolNode::SymbolProps &props, const QString &n, double mathFontFactor) const;
};

#endif // JKQTMATHTEXTSYMBOLNODE_H









