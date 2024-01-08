/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)

    

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



#include "jkqtmathtext/nodes/jkqtmathtextnoopnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"



JKQTMathTextNoopNode::JKQTMathTextNoopNode(JKQTMathText *parent):
    JKQTMathTextNonDrawingBaseNode(parent)
{

}

JKQTMathTextNoopNode::~JKQTMathTextNoopNode()
{

}

QString JKQTMathTextNoopNode::getTypeName() const
{
    return "MTNoopNode";
}









JKQTMathTextBlockNode::JKQTMathTextBlockNode(JKQTMathTextNode *_child, JKQTMathText *parent):
    JKQTMathTextSingleChildNode(_child, parent)
{

}

JKQTMathTextBlockNode::~JKQTMathTextBlockNode()
{

}

QString JKQTMathTextBlockNode::getTypeName() const
{
    return "MTBlockNode";
}

double JKQTMathTextBlockNode::draw(QPainter &painter, double x, double y, JKQTMathTextEnvironment currentEv) const
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTMathTextBlockNode[]::draw()"));
#endif
    return child->draw(painter, x, y, currentEv);
}

bool JKQTMathTextBlockNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const
{
    return child->toHtml(html, currentEv, defaultEv);
}

JKQTMathTextNodeSize JKQTMathTextBlockNode::getSizeInternal(QPainter &painter, JKQTMathTextEnvironment currentEv) const
{
    return child->getSize(painter, currentEv);
}
