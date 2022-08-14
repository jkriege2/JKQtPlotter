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

JKQTMathTextWhitespaceNode::WhitespaceProps::WhitespaceProps(const WhitespaceProps &other):
    type(other.type), count(other.count)
{

}

JKQTMathTextWhitespaceNode::WhitespaceProps& JKQTMathTextWhitespaceNode::WhitespaceProps::operator=(const WhitespaceProps &other)
{
    type=other.type;
    count=other.count;
    return *this;
}

JKQTMathTextWhitespaceNode::Types JKQTMathTextWhitespaceNode::getWhitespaceType() const
{
    return whitespace.type;
}

size_t JKQTMathTextWhitespaceNode::getWhitespaceCount() const
{
    return whitespace.count;
}

double JKQTMathTextWhitespaceNode::draw(QPainter &painter, double x, double y, JKQTMathTextEnvironment currentEv)
{
    doDrawBoxes(painter, x,y,currentEv);
    double width=0, bh=0, oh=0, sp=0;
    getSize(painter, currentEv, width, bh, oh, sp);
    return x+width;
}

void JKQTMathTextWhitespaceNode::getSizeInternal(QPainter &painter, JKQTMathTextEnvironment currentEv, double &width, double &baselineHeight, double &overallHeight, double &strikeoutPos)
{
    const double singelWidthPIX=Type2PixelWidth(whitespace.type, currentEv, painter.device());
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
        supportedInstructions["enskip"]=WhitespaceProps(WST1en, 1);
        supportedInstructions["quad"]=WhitespaceProps(WSTQuad, 1);
        supportedInstructions["emspace"]=WhitespaceProps(WSTQuad, 1);
        supportedInstructions["qquad"]=WhitespaceProps(WSTQuad, 2);
        supportedInstructions[","]=WhitespaceProps(WSTthin, 1);
        supportedInstructions["thinspace"]=WhitespaceProps(WSTthin, 1);
        supportedInstructions[":"]=WhitespaceProps(WSTmedium, 1);
        supportedInstructions["medspace"]=WhitespaceProps(WSTmedium, 1);
        supportedInstructions[";"]=WhitespaceProps(WSTthick, 1);
        supportedInstructions["thickspace"]=WhitespaceProps(WSTthick, 1);
        supportedInstructions["!"]=WhitespaceProps(WSTnegthin, 1);
        supportedInstructions["negthinspace"]=WhitespaceProps(WSTnegthin, 1);
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

double JKQTMathTextWhitespaceNode::Type2PixelWidth(Types type, JKQTMathTextEnvironment currentEv, QPaintDevice* pd) const
{
    const QFontMetricsF fm(currentEv.getFont(parentMathText), pd);
#if (QT_VERSION>=QT_VERSION_CHECK(5, 15, 0))
    const double em=fm.horizontalAdvance(QChar(0x2003));//currentEv.fontSize;
    const double sp=fm.horizontalAdvance(' ');//currentEv.fontSize;
#else
    const double em=fm.width(QChar(0x2003));//currentEv.fontSize;
    const double sp=fm.width(' ');//currentEv.fontSize;
#endif
    const double en=em/2.0;
    switch (type) {
        case WSTNormal: return sp;
        case WSTNonbreaking: return sp;
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


QString JKQTMathTextEmptyBoxNode::Units2String(Units type)
{
    switch(type) {
    case EBUem: return "em";
    case EBUex: return "ex";
    }
    return "?";
}

JKQTMathTextEmptyBoxNode::Units JKQTMathTextEmptyBoxNode::String2Units(QString type)
{
    type=type.toLower().trimmed();
    if (type=="ex") return EBUex;
    if (type=="em") return EBUem;
    return EBUem;
}

double JKQTMathTextEmptyBoxNode::Units2PixelWidth(double value, Units unit, JKQTMathTextEnvironment currentEv, QPaintDevice *pd) const
{
    QFont f=currentEv.getFont(parentMathText);
    f.setStyleStrategy(QFont::PreferDefault);
    const QFontMetricsF fm(f, pd);
    if (unit==EBUem) {
#if (QT_VERSION>=QT_VERSION_CHECK(5, 15, 0))
        const double em=fm.horizontalAdvance(QChar(0x2003));//currentEv.fontSize;
#else
        const double em=fm.width(QChar(0x2003));//currentEv.fontSize;
#endif
        //qDebug()<<"em="<<em<<"pix";
        return value*em;
    } else if (unit==EBUex) {
        const double ex=fm.xHeight();
        //qDebug()<<"ex="<<ex<<"pix";
        return value*ex;
    } else {
        //qDebug()<<"JKQTMathTextEmptyBoxNode::Units2PixelWidth(): UNKOWN UNIT";
    }
    return 0;
}

JKQTMathTextEmptyBoxNode::JKQTMathTextEmptyBoxNode(JKQTMathText *parent, double width_, Units widthUnit_, double height_, Units heightUnit_):
    JKQTMathTextNode(parent),
    width(width_), widthUnit(widthUnit_),
    height(height_),heightUnit(heightUnit_)
{

}

JKQTMathTextEmptyBoxNode::~JKQTMathTextEmptyBoxNode()
{

}

QString JKQTMathTextEmptyBoxNode::getTypeName() const
{
    return QString("JKQTMathTextEmptyBoxNode(%1%2 x %3%4)").arg(getWidth()).arg(JKQTMathTextEmptyBoxNode::Units2String(getWidthUnit())).arg(getHeight()).arg(JKQTMathTextEmptyBoxNode::Units2String(getHeightUnit()));
}

bool JKQTMathTextEmptyBoxNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv)
{
    return true;
}

JKQTMathTextEmptyBoxNode::Units JKQTMathTextEmptyBoxNode::getWidthUnit() const
{
    return widthUnit;
}

double JKQTMathTextEmptyBoxNode::getWidth() const
{
    return width;
}

JKQTMathTextEmptyBoxNode::Units JKQTMathTextEmptyBoxNode::getHeightUnit() const
{
    return heightUnit;
}

double JKQTMathTextEmptyBoxNode::getHeight() const
{
    return height;
}

double JKQTMathTextEmptyBoxNode::draw(QPainter &painter, double x, double y, JKQTMathTextEnvironment currentEv)
{
    doDrawBoxes(painter, x,y,currentEv);
    const auto s=getSize(painter, currentEv);
    return x+s.width;
}

void JKQTMathTextEmptyBoxNode::getSizeInternal(QPainter &painter, JKQTMathTextEnvironment currentEv, double &width, double &baselineHeight, double &overallHeight, double &strikeoutPos)
{
    const QFontMetricsF fm(currentEv.getFont(parentMathText), painter.device());
    width=Units2PixelWidth(this->width, widthUnit, currentEv, painter.device());
    overallHeight=Units2PixelWidth(height, heightUnit, currentEv, painter.device());
    if (height>0) {
        baselineHeight=overallHeight;
    } else {
        baselineHeight=0;
    }
    strikeoutPos=fm.strikeOutPos();
}













QString JKQTMathTextPhantomNode::Mode2Instruction(Mode mode)
{
    switch(mode) {
    case FMwidthAndHeight: return "phantom";
    case FMwidth: return "hphantom";
    case FMheight: return "vphantom";
    }

    return "phantom";
}

JKQTMathTextPhantomNode::JKQTMathTextPhantomNode(JKQTMathText *parent, const QString &mode, JKQTMathTextNode *child):
    JKQTMathTextInstruction1Node(parent, mode, child)
{
    fillInstructions();
}

JKQTMathTextPhantomNode::JKQTMathTextPhantomNode(JKQTMathText* _parent, Mode mode, JKQTMathTextNode* child):
    JKQTMathTextInstruction1Node(_parent, Mode2Instruction(mode), child)
{
    fillInstructions();
}

JKQTMathTextPhantomNode::~JKQTMathTextPhantomNode() {
}


QString JKQTMathTextPhantomNode::getTypeName() const
{
    return QLatin1String("JKQTMathTextPhantomNode(")+instructionName+")";
}

void JKQTMathTextPhantomNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos) {
    fillInstructions();
    JKQTMathTextEnvironment ev=currentEv;

    getChild()->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);

    switch(instructions[getInstructionName()]) {
        case FMwidth:
            overallHeight=0;
            baselineHeight=0;
            strikeoutPos=0;
            break;
        case FMwidthAndHeight:
            break;
        case FMheight:
            width=0;
            break;
    }
}

double JKQTMathTextPhantomNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathTextEnvironment ev=currentEv;

    const JKQTMathTextNodeSize s=getSize(painter, currentEv);
    return x+s.width;
}

bool JKQTMathTextPhantomNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
    JKQTMathTextEnvironment ev=currentEv;
    fillInstructions();
    return "&nbsp;";
}

bool JKQTMathTextPhantomNode::supportsInstructionName(const QString &instructionName)
{
    fillInstructions();
    return instructions.contains(instructionName);
}

QHash<QString, JKQTMathTextPhantomNode::Mode> JKQTMathTextPhantomNode::instructions;

void JKQTMathTextPhantomNode::fillInstructions()
{
    instructions["phantom"] = FMwidthAndHeight;
    instructions["hphantom"] = FMwidth;
    instructions["vphantom"] = FMheight;
}

