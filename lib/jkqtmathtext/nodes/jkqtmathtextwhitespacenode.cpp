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



#include "jkqtmathtext/nodes/jkqtmathtextwhitespacenode.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtcommon/jkqtpcodestructuring.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include <cmath>
#include <QFontMetricsF>
#include <QDebug>
#include <QFontDatabase>
#include <QFontInfo>
#include <QApplication>
#include <QFont>




JKQTMathTextWhitespaceNode::JKQTMathTextWhitespaceNode(Types type, JKQTMathText *_parent):
    JKQTMathTextWhitespaceNode(type, 1, _parent)
{

}

JKQTMathTextWhitespaceNode::JKQTMathTextWhitespaceNode(JKQTMathText *_parent):
    JKQTMathTextWhitespaceNode(WSTNormal, 1, _parent)
{

}

JKQTMathTextWhitespaceNode::JKQTMathTextWhitespaceNode(const QString &_type, JKQTMathText *parent):
    JKQTMathTextWhitespaceNode(parent)
{
    fillSupportedInstructions();
    whitespace=supportedInstructions[_type];
}

JKQTMathTextWhitespaceNode::JKQTMathTextWhitespaceNode(const QString &_type, size_t count, JKQTMathText *parent):
    JKQTMathTextWhitespaceNode(parent)
{
    fillSupportedInstructions();
    whitespace=supportedInstructions[_type];
    whitespace.count=whitespace.count*count;
}

JKQTMathTextWhitespaceNode::JKQTMathTextWhitespaceNode(Types type, size_t count, JKQTMathText *parent):
    JKQTMathTextNode(parent),
    whitespace(type, count)
{
    fillSupportedInstructions();
}

JKQTMathTextWhitespaceNode::~JKQTMathTextWhitespaceNode() {

}

QString JKQTMathTextWhitespaceNode::getTypeName() const
{
    return QLatin1String("JKQTMathTextWhitespaceNode(")+Type2String(whitespace.type)+", count="+QString::number(whitespace.count)+")";
}

bool JKQTMathTextWhitespaceNode::toHtml(QString &html, JKQTMathTextEnvironment /*currentEv*/, JKQTMathTextEnvironment /*defaultEv*/) {
    for (size_t i=0; i<whitespace.count; i++) {
        html=html+Type2HTML(whitespace.type);
    }
    return true;
}

JKQTMathTextWhitespaceNode::WhitespaceProps::WhitespaceProps(JKQTMathTextWhitespaceNode::Types _type, size_t _count):
    type(_type), count(_count)
{

}

JKQTMathTextWhitespaceNode::Types JKQTMathTextWhitespaceNode::getWhitespaceType() const
{
    return whitespace.type;
}

size_t JKQTMathTextWhitespaceNode::getWhitespaceCount() const
{
    return whitespace.count;
}

double JKQTMathTextWhitespaceNode::draw(QPainter &painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize *prevNodeSize)
{
    double width=0, bh=0, oh=0, sp=0;
    getSize(painter, currentEv, width, bh, oh, sp, prevNodeSize);
    return x+width;
}

void JKQTMathTextWhitespaceNode::getSizeInternal(QPainter &painter, JKQTMathTextEnvironment currentEv, double &width, double &baselineHeight, double &overallHeight, double &strikeoutPos, const JKQTMathTextNodeSize *prevNodeSize)
{
    const double singelWidthPT=Type2PointWidth(whitespace.type, currentEv);
    const double singelWidthPIX=singelWidthPT/72.0*painter.device()->logicalDpiX();
    const QFontMetricsF fm(currentEv.getFont(parentMathText));
    width=singelWidthPIX*static_cast<double>(whitespace.count);
    baselineHeight=0;
    overallHeight=0;
    strikeoutPos=fm.strikeOutPos();
}

QHash<QString, JKQTMathTextWhitespaceNode::WhitespaceProps> JKQTMathTextWhitespaceNode::supportedInstructions;

void JKQTMathTextWhitespaceNode::fillSupportedInstructions()
{
    if (supportedInstructions.size()==0) {
        supportedInstructions[" "]=WhitespaceProps(WSTthicker, 1);
        supportedInstructions["nbsp"]=WhitespaceProps(WSTNonbreaking, 1);
        supportedInstructions["enspace"]=WhitespaceProps(WST1en, 1);
        supportedInstructions["quad"]=supportedInstructions["emspace"]=WhitespaceProps(WSTQuad, 1);
        supportedInstructions["qquad"]=WhitespaceProps(WSTQuad, 2);
        supportedInstructions[","]=supportedInstructions["thinspace"]=WhitespaceProps(WSTthin, 1);
        supportedInstructions[":"]=supportedInstructions["medspace"]=WhitespaceProps(WSTmedium, 1);
        supportedInstructions[";"]=supportedInstructions["thickspace"]=WhitespaceProps(WSTthick, 1);
        supportedInstructions["!"]=supportedInstructions["negthinspace"]=WhitespaceProps(WSTnegthin, 1);
        supportedInstructions["negmedspace"]=WhitespaceProps(WSTnegmedium, 1);
        supportedInstructions["negthickspace"]=WhitespaceProps(WSTnegthick, 1);
    }

}

QString JKQTMathTextWhitespaceNode::Type2String(Types type)
{
    switch (type) {
    case WSTNormal: return "WSTNormal";
    case WSTNonbreaking: return "WSTNonbreaking";
    case WST1en: return "WST1en";
    case WST1em: return "WST1em";
    case WSThair: return "WSThair";
    case WSTthin: return "WSTthin";
    case WSTnegthin: return "WSTnegthin";
    case WSTmedium: return "WSTmedium";
    case WSTnegmedium: return "WSTnegmedium";
    case WSTthick: return "WSTthick";
    case WSTnegthick: return "WSTnegthick";
    case WSTthicker: return "WSTthicker";
    }
    return "???";
}

double JKQTMathTextWhitespaceNode::Type2PointWidth(Types type, JKQTMathTextEnvironment currentEv) const
{
    const double em=currentEv.fontSize;
    const double en=em/2.0;
    switch (type) {
        case WSTNormal: return QFontMetricsF(currentEv.getFont(parentMathText)).width(' ');
        case WSTNonbreaking: return QFontMetricsF(currentEv.getFont(parentMathText)).width(' ');
        case WST1en: return en;
        case WST1em: return em;
        case WSThair: return em/12.0;
        case WSTthin: return em/6.0;
        case WSTnegthin: return -em/6.0;
        case WSTmedium: return em*2.0/9.0;
        case WSTnegmedium: return -em*2.0/9.0;
        case WSTthick: return em*5.0/18.0;
        case WSTnegthick: return -em*5.0/18.0;
        case WSTthicker: return em/3.0;
    }
    return 0.0;
}


bool JKQTMathTextWhitespaceNode::supportsInstructionName(const QString &instruction)
{
    fillSupportedInstructions();
    return supportedInstructions.contains(instruction);
}


QString JKQTMathTextWhitespaceNode::Type2HTML(Types type)
{
    switch (type) {
    case WSTNonbreaking: return "&nbsp;";
    case WST1en: return "&ensp;";
    case WST1em: return "&emsp;";
    case WSThair: return "&hairsp";
    case WSTthin: return "&thinsp;";
    case WSTnegthin: return "";
    case WSTmedium: return "&emsp14;";
    case WSTnegmedium: return "";
    case WSTthick: return "&emsp13;";
    case WSTnegthick: return "";
    case WSTthicker: return "&thinsp;&thinsp;";
    case WSTNormal:
    default:
        return " ";
    }
    return " ";
}

