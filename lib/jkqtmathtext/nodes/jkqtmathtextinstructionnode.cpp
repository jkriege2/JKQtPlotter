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



#include "jkqtmathtext/nodes/jkqtmathtextinstructionnode.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
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





JKQTMathTextInstruction1Node::JKQTMathTextInstruction1Node(JKQTMathText* _parent, const QString& _instructionName, JKQTMathTextNode* child, const QStringList& _parameters):
    JKQTMathTextSingleChildNode(child, _parent),
    instructionName(_instructionName),
    parameters(_parameters)
{
}

JKQTMathTextInstruction1Node::~JKQTMathTextInstruction1Node() {
}

const QString& JKQTMathTextInstruction1Node::getInstructionName() const {
    return this->instructionName;
}

const QStringList &JKQTMathTextInstruction1Node::getParameters() const {
    return this->parameters;
}






JKQTMathTextSimpleInstructionNode::JKQTMathTextSimpleInstructionNode(JKQTMathText *_parent, const QString &_name, const QStringList &_parameters):
    JKQTMathTextNode(_parent),
    instructionName(_name),
    parameters(_parameters)
{
    fillInstructions();

}

JKQTMathTextSimpleInstructionNode::~JKQTMathTextSimpleInstructionNode()
{

}

QString JKQTMathTextSimpleInstructionNode::getTypeName() const
{
    return QLatin1String("JKQTMathTextSimpleInstructionNode(")+instructionName+")";
}

double JKQTMathTextSimpleInstructionNode::draw(QPainter &painter, double x, double y, JKQTMathTextEnvironment currentEv) const
{
    doDrawBoxes(painter, x, y, currentEv);
    fillInstructions();
    QFont f=currentEv.getFont(parentMathText);
    f.setStyleStrategy(QFont::PreferDefault);
    const QFontMetricsF fm(f);
    const QString txt=executeInstruction();
    const QRectF bb=fm.boundingRect(txt);
    painter.setPen(currentEv.color);
    painter.setFont(f);
    painter.drawText(x,y,txt);
    return x+bb.width();
}

bool JKQTMathTextSimpleInstructionNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const
{
    fillInstructions();
    const QString txt=executeInstruction();
    html+=txt;
    return true;
}

const QString &JKQTMathTextSimpleInstructionNode::getInstructionName() const
{
    return instructionName;
}

const QStringList &JKQTMathTextSimpleInstructionNode::getParameters() const
{
    return parameters;
}

bool JKQTMathTextSimpleInstructionNode::supportsInstructionName(const QString &instructionName)
{
    fillInstructions();
    return instructions.contains(instructionName);
}

size_t JKQTMathTextSimpleInstructionNode::countParametersOfInstruction(const QString &instructionName)
{
    fillInstructions();
    if (instructions.contains(instructionName)) return instructions[instructionName].NParams;
    return 0;
}

JKQTMathTextNodeSize JKQTMathTextSimpleInstructionNode::getSizeInternal(QPainter &painter, JKQTMathTextEnvironment currentEv) const
{
    fillInstructions();
    QFont f=currentEv.getFont(parentMathText);
    f.setStyleStrategy(QFont::PreferDefault);
    const QFontMetricsF fm(f);
    const QString txt=executeInstruction();
    const QRectF bb=fm.boundingRect(txt);
    JKQTMathTextNodeSize s;
    s.width=bb.width();
    s.baselineHeight=-bb.y();
    s.overallHeight=bb.height();
    s.strikeoutPos=fm.strikeOutPos();
    return s;
}

QHash<QString, JKQTMathTextSimpleInstructionNode::InstructionProperties> JKQTMathTextSimpleInstructionNode::instructions;

void JKQTMathTextSimpleInstructionNode::fillInstructions()
{
    static std::mutex sMutex;
    std::lock_guard<std::mutex> lock(sMutex);
    if (instructions.size()>0) return;
    {
        InstructionProperties i([](const QStringList& parameters) -> QString {
            bool ok=false;
            qlonglong code=parameters.value(0, "0").toLongLong(&ok, 16);
            ok=ok&&(code>=0);
            if (ok&&(code>=0)&&(code<=0xFFFFFFFF)) return QString::fromStdString(jkqtp_UnicodeToUTF8(static_cast<uint32_t>(code)));
            else return QString();
        }, 1);
        instructions["unicode"]= i;
        instructions["usym"]= i;
    }
    {
        InstructionProperties i([](const QStringList& parameters) -> QString {
            bool ok=false;
            qlonglong code=parameters.value(0, "0").toLongLong(&ok, 16);
            ok=ok&&(code>=0);
            if (ok) {
                QByteArray bytes;
                while (code!=0) {
                    bytes.prepend(static_cast<char>(code&0xFF));
                    code=code>>8;
                }
                return QString::fromUtf8(bytes);
            }
            return QChar(0);
        }, 1);
        instructions["utfeight"]= i;
    }
}

QString JKQTMathTextSimpleInstructionNode::executeInstruction() const
{
    fillInstructions();
    return instructions.value(getInstructionName(), InstructionProperties()).evaluator(getParameters());
}

JKQTMathTextSimpleInstructionNode::InstructionProperties::InstructionProperties():
    NParams(0),
    evaluator([](const QStringList&) { return QString(); })

{

}

JKQTMathTextSimpleInstructionNode::InstructionProperties::InstructionProperties(const EvaluateInstructionFunctor &_evaluator, size_t _NParams):
    NParams(_NParams),
    evaluator(_evaluator)

{

}




