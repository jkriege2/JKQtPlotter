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




#ifndef JKQTMATHTEXTNODETOOLS_H
#define JKQTMATHTEXTNODETOOLS_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"

class JKQTMathTextNode; // forward


/** \brief simplifies the node \a node and the tree below it. You can put the return value in place of \a node after the call
 *  \ingroup jkqtmathtext_items
 *
 *  Basically this takes does the following steps (recursively):
 *    - remove any JKQTMathTextHorizontalListNode that has only one child
 *  .
 */
JKQTMATHTEXT_LIB_EXPORT JKQTMathTextNode* simplifyJKQTMathTextNode(JKQTMathTextNode* node);

/** \brief calls simplifyJKQTMathTextNode(). In addition it tries to clear whitespace at the start and end of the tree
 *  \ingroup jkqtmathtext_items
 *
 *  \see simplifyJKQTMathTextNode()
 */
JKQTMATHTEXT_LIB_EXPORT JKQTMathTextNode* simplifyAndTrimJKQTMathTextNode(JKQTMathTextNode* node);

/*! \brief converts a node-tree with the given \a root into a string, representing the node-tree
   \ingroup jkqtmathtext_items

   \see This method uses JKQTMathText::getTypeName()

   Here is an example output for
   \code
     $x_{1/2}=\frac{-b\pm\sqrt{b^2-4ac}}{2a}$
   \endcode

   The output looks like this:
   \verbatim
JKQTMathTextModifiedTextPropsInstructionNode(equation)
+--MTHorizontalListNode
|  +--JKQTMathTextTextNode(x)
|  +--MTsubscriptNode
|  |  +--MTHorizontalListNode
|  |  |  +--JKQTMathTextTextNode(1)
|  |  |  +--JKQTMathTextTextNode(/)
|  |  |  +--JKQTMathTextTextNode(2)
|  +--JKQTMathTextSymbolNode(=)
|  +--MTfracNode
|  |  +--MTHorizontalListNode
|  |  |  +--JKQTMathTextSymbolNode(-)
|  |  |  +--JKQTMathTextTextNode(b)
|  |  |  +--JKQTMathTextSymbolNode(pm)
|  |  |  +--MTsqrtNode
|  |  |  |  +--MTHorizontalListNode
|  |  |  |  |  +--JKQTMathTextTextNode(b)
|  |  |  |  |  +--MTsuperscriptNode
|  |  |  |  |  |  +--JKQTMathTextTextNode(2)
|  |  |  |  |  +--JKQTMathTextSymbolNode(-)
|  |  |  |  |  +--JKQTMathTextTextNode(4ac)
|  |  +--JKQTMathTextTextNode(2a)
   \endverbatim
 */
JKQTMATHTEXT_LIB_EXPORT QString JKQTMathTextNodeTree2String(JKQTMathTextNode* root);

#endif // JKQTMATHTEXTNODETOOLS_H









