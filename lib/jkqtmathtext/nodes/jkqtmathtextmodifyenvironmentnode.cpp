/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)

    

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



#include "jkqtmathtext/nodes/jkqtmathtextmodifyenvironmentnode.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"
#include <cmath>
#include <QFontMetricsF>
#include <QDebug>
#include <QFontDatabase>
#include <QFontInfo>
#include <QApplication>
#include <QFont>





JKQTMathTextModifiedTextPropsInstructionNode::JKQTMathTextModifiedTextPropsInstructionNode(JKQTMathText* _parent, const QString& name, JKQTMathTextNode* child, const QStringList& parameters):
    JKQTMathTextInstruction1Node(_parent, name, child, parameters)
{
}

JKQTMathTextModifiedTextPropsInstructionNode::~JKQTMathTextModifiedTextPropsInstructionNode() {
}


QString JKQTMathTextModifiedTextPropsInstructionNode::getTypeName() const
{
    return QLatin1String("JKQTMathTextModifiedTextPropsInstructionNode(")+instructionName+")";
}

JKQTMathTextNodeSize JKQTMathTextModifiedTextPropsInstructionNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const {
    JKQTMathTextEnvironment ev=currentEv;

    executeInstruction(ev);

    return getChild()->getSize(painter, ev);
}

double JKQTMathTextModifiedTextPropsInstructionNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTMathTextModifiedTextPropsInstructionNode[]::draw()"));
#endif
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathTextEnvironment ev=currentEv;

    executeInstruction(ev);
    //std::cout<<"  MODNODE: "<<getInstructionName().toStdString()<<" ev.mathMode="<<ev.insideMath<<", ev.forceUpright="<<ev.insideMathForceDigitsUpright<<"\n";
    return getChild()->draw(painter, x, y, ev);
}

bool JKQTMathTextModifiedTextPropsInstructionNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const {
    JKQTMathTextEnvironment ev=currentEv;
    executeInstruction(ev);

    return getChild()->toHtml(html, ev, defaultEv);
}

bool JKQTMathTextModifiedTextPropsInstructionNode::supportsInstructionName(const QString &instructionName)
{
    return instructions().contains(instructionName);
}

size_t JKQTMathTextModifiedTextPropsInstructionNode::countParametersOfInstruction(const QString &instructionName)
{
    if (instructions().contains(instructionName)) return instructions()[instructionName].NParams;
    return 0;
}

void JKQTMathTextModifiedTextPropsInstructionNode::modifyInMathEnvironment(const QString &instructionName, bool &insideMath, bool& insideMathTextStyle, const QStringList& params)
{
    if (instructions().contains(instructionName)) {
        JKQTMathTextEnvironment ev;
        ev.insideMath=insideMath;
        ev.insideMathUseTextStyle=insideMathTextStyle;
        instructions()[instructionName].modifier(ev, params);
        insideMath=ev.insideMath;
        insideMathTextStyle=ev.insideMathUseTextStyle;
    }
}


void JKQTMathTextModifiedTextPropsInstructionNode::executeInstruction(JKQTMathTextEnvironment &ev) const
{
    instructions().value(getInstructionName(), InstructionProperties()).modifier(ev, getParameters());
}

const QHash<QString, JKQTMathTextModifiedTextPropsInstructionNode::InstructionProperties>& JKQTMathTextModifiedTextPropsInstructionNode::instructions() {
    static QHash<QString, JKQTMathTextModifiedTextPropsInstructionNode::InstructionProperties> table=[]()
        {

            QHash<QString, JKQTMathTextModifiedTextPropsInstructionNode::InstructionProperties> instructions;
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.bold=false;
                    ev.insideMathForceDigitsUpright=true;
                    ev.italic=false;
                    ev.underlined=false;
                    ev.overline=false;
                    ev.strike=false;
                    ev.font=MTEroman;
                }, 0);
                instructions["textnormal"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.bold=true;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["textbf"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.bold=true;
                    ev.italic=false;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["mathbf"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.italic=true;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["textit"] = i;
                instructions["mathit"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.italic=!ev.italic;
                }, 0);
                instructions["emph"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.italic=false;
                }, 0);
                instructions["textup"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& parameters) {
                    ev.color=jkqtp_String2QColor(parameters.value(0, ev.color.name()));
                }, 1);
                instructions["textcolor"] = i;
                instructions["mathcolor"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& parameters) {
                    ev.customFontName=parameters.value(0, "");
                    ev.font=MTECustomFont;
                }, 1);
                instructions["userfont"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.beginMathMode();
                }, 0);
                instructions["ensuremath"] = i;
                instructions["equation"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.capitalization=QFont::SmallCaps;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["textsc"] = i;
                instructions["mathsc"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.capitalization=QFont::AllUppercase;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["uppercase"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.capitalization=QFont::AllLowercase;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["lowercase"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.underlined=true;
                }, 0);
                instructions["ul"] = i;
                instructions["underlined"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.overline=true;
                }, 0);
                instructions["ol"] = i;
                instructions["overlined"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.strike=true;
                }, 0);
                instructions["strike"]= i;
                instructions["st"]= i;
                instructions["sout"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEroman;
                    ev.italic=false;
                }, 0);
                instructions["textrm"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEroman;
                    ev.italic=false;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["mathrm"] = i;
                instructions["unit"] = i;
                instructions["operatorname"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.bold=true;
                    ev.italic=true;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["mathbfit"] = i;
                instructions["textbfit"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.endMathMode();
                    ev.font=JKQTMathTextEnvironmentFont::MTEroman;
                }, 0);
                instructions["text"] = i;
                instructions["mbox"] = i;
                instructions["ensuretext"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEroman;
                    ev.italic=false;
                    ev.bold=true;
                }, 0);
                instructions["mat"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEcaligraphic;
                    ev.italic=false;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["mathcal"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEcaligraphic;
                }, 0);
                instructions["textcal"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEcaligraphic;
                    ev.italic=false;
                    ev.bold=true;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["mathbfcal"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEcaligraphic;
                    ev.bold=true;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["textfcal"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEfraktur;
                }, 0);
                instructions["textfrak"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEfraktur;
                    ev.italic=false;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["mathfrak"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEfraktur;
                    ev.bold=true;
                    ev.italic=false;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["mathbffrak"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEfraktur;
                    ev.bold=true;
                }, 0);
                instructions["textbffrak"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEblackboard;
                    ev.italic=false;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["textbb"] = i;
                instructions["mathbb"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEtypewriter;
                }, 0);
                instructions["texttt"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEtypewriter;
                    ev.italic=false;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["mathtt"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEtypewriter;
                    ev.bold=true;
                }, 0);
                instructions["textbftt"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEtypewriter;
                    ev.italic=false;
                    ev.bold=true;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["mathbftt"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEsans;
                }, 0);
                instructions["textsf"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEsans;
                    ev.italic=false;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["mathsf"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEsans;
                    ev.italic=true;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["textitsf"] = i;
                instructions["mathitsf"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEscript;
                }, 0);
                instructions["textscript"] = i;
                instructions["textscr"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEscript;
                    ev.italic=false;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["mathscript"] = i;
                instructions["mathscr"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEscript;
                    ev.bold=true;
                }, 0);
                instructions["textbfscript"] = i;
                instructions["textbfscr"] = i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.font=JKQTMathTextEnvironmentFont::MTEscript;
                    ev.bold=true;
                    ev.italic=false;
                    ev.insideMathForceDigitsUpright=false;
                }, 0);
                instructions["mathbfscript"] = i;
                instructions["mathbfscr"]= i;
            }


            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.fontSize=ev.fontSize*1.2;
                }, 0);
                instructions["bigsize"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.fontSize=ev.fontSize*1.85;
                }, 0);
                instructions["Bigsize"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.fontSize=ev.fontSize*2.4;
                }, 0);
                instructions["biggsize"]= i;
            }
            {
                InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/) {
                    ev.fontSize=ev.fontSize*3.1;
                }, 0);
                instructions["Biggsize"]= i;
            }
            return instructions;
        }();
    return table;
}

JKQTMathTextModifiedTextPropsInstructionNode::InstructionProperties::InstructionProperties():
    NParams(0),
    modifier([](JKQTMathTextEnvironment&, const QStringList&) {})
{

}

JKQTMathTextModifiedTextPropsInstructionNode::InstructionProperties::InstructionProperties(const ModifyEnvironmentFunctor &_modifier, size_t _NParams):
    NParams(_NParams),
    modifier(_modifier)
{

}



JKQTMathTextModifiedEnvironmentInstructionNode::JKQTMathTextModifiedEnvironmentInstructionNode(JKQTMathText *parent_, const QString &name_, const QStringList &parameters_):
    JKQTMathTextNonDrawingBaseNode(parent_), JKQTMathTextModifyEnvironmentNodeMixIn(),
    instructionName(name_),
    parameters(parameters_)
{
}

JKQTMathTextModifiedEnvironmentInstructionNode::~JKQTMathTextModifiedEnvironmentInstructionNode()
{

}

QString JKQTMathTextModifiedEnvironmentInstructionNode::getTypeName() const
{
    return "MTModifyEnvironmentInstructionNode";
}

const QString &JKQTMathTextModifiedEnvironmentInstructionNode::getInstructionName() const
{
    return instructionName;
}

const QStringList &JKQTMathTextModifiedEnvironmentInstructionNode::getParameters() const
{
    return parameters;
}

void JKQTMathTextModifiedEnvironmentInstructionNode::modifyEnvironment(JKQTMathTextEnvironment &currentEv) const
{
    instructions().value(getInstructionName(), InstructionProperties()).modifier(currentEv, getParameters(), parentMathText);
}

bool JKQTMathTextModifiedEnvironmentInstructionNode::supportsInstructionName(const QString &instructionName)
{
    return instructions().contains(instructionName);
}

size_t JKQTMathTextModifiedEnvironmentInstructionNode::countParametersOfInstruction(const QString &instructionName)
{
    if (instructions().contains(instructionName)) return instructions()[instructionName].NParams;
    return 0;
}

void JKQTMathTextModifiedEnvironmentInstructionNode::modifyInMathTextStyleEnvironment(const QString &instructionName, bool &insideMathTextStyle, JKQTMathText* parentMathText, const QStringList &params)
{
    if (instructions().contains(instructionName)) {
        JKQTMathTextEnvironment ev;
        ev.insideMathUseTextStyle=insideMathTextStyle;
        instructions()[instructionName].modifier(ev, params, parentMathText);
        insideMathTextStyle=ev.insideMathUseTextStyle;
    }
}

const QHash<QString, JKQTMathTextModifiedEnvironmentInstructionNode::InstructionProperties>& JKQTMathTextModifiedEnvironmentInstructionNode::instructions() {
    static QHash<QString, JKQTMathTextModifiedEnvironmentInstructionNode::InstructionProperties> table=[](){
        QHash<QString, JKQTMathTextModifiedEnvironmentInstructionNode::InstructionProperties> instructions;

        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.bold=false;
                ev.insideMathForceDigitsUpright=true;
                ev.italic=false;
                ev.underlined=false;
                ev.overline=false;
                ev.strike=false;
                ev.font=MTEroman;
                ev.fontSize=parentMathText->getFontSize();
                ev.capitalization=QFont::MixedCase;
                ev.color=parentMathText->getFontColor();
            }, 0);
            instructions["normalfont"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.bold=false;
                ev.italic=false;
                ev.insideMathForceDigitsUpright=false;
            }, 0);
            instructions["md"] = i;
            instructions["mdseries"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.bold=true;
                ev.insideMathForceDigitsUpright=false;
            }, 0);
            instructions["bf"] = i;
            instructions["bfseries"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.italic=true;
                ev.insideMathForceDigitsUpright=false;
            }, 0);
            instructions["it"] = i;
            instructions["itshape"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.italic=false;
                ev.insideMathForceDigitsUpright=false;
            }, 0);
            instructions["up"] = i;
            instructions["upshape"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.italic=!ev.italic;
            }, 0);
            instructions["em"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& parameters, const JKQTMathText* /*parentMathText*/) {
                ev.color=jkqtp_String2QColor(parameters.value(0, ev.color.name()));
            }, 1);
            instructions["color"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& parameters, const JKQTMathText* /*parentMathText*/) {
                ev.customFontName=parameters.value(0, "");
                ev.font=MTECustomFont;
            }, 1);
            instructions["setfont"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.capitalization=QFont::SmallCaps;
                ev.insideMathForceDigitsUpright=false;
            }, 0);
            instructions["sc"] = i;
            instructions["scshape"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.font=JKQTMathTextEnvironmentFont::MTEroman;
                ev.italic=false;
            }, 0);
            instructions["rm"] = i;
            instructions["rmfamily"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.bold=true;
                ev.italic=true;
                ev.insideMathForceDigitsUpright=false;
            }, 0);
            instructions["bfit"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.font=JKQTMathTextEnvironmentFont::MTEcaligraphic;
            }, 0);
            instructions["cal"] = i;
            instructions["calfamily"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.font=JKQTMathTextEnvironmentFont::MTEcaligraphic;
                ev.bold=true;
                ev.insideMathForceDigitsUpright=false;
            }, 0);
            instructions["bbfcal"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.font=JKQTMathTextEnvironmentFont::MTEfraktur;
            }, 0);
            instructions["frak"] = i;
            instructions["frakfamily"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.font=JKQTMathTextEnvironmentFont::MTEfraktur;
                ev.bold=true;
            }, 0);
            instructions["bffrak"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.font=JKQTMathTextEnvironmentFont::MTEblackboard;
                ev.italic=false;
                ev.insideMathForceDigitsUpright=false;
            }, 0);
            instructions["bb"] = i;
            instructions["bbfamily"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.font=JKQTMathTextEnvironmentFont::MTEtypewriter;
            }, 0);
            instructions["tt"] = i;
            instructions["ttfamily"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.font=JKQTMathTextEnvironmentFont::MTEtypewriter;
                ev.bold=true;
            }, 0);
            instructions["bftt"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.font=JKQTMathTextEnvironmentFont::MTEsans;
            }, 0);
            instructions["sf"] = i;
            instructions["sffamily"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.font=JKQTMathTextEnvironmentFont::MTEsans;
                ev.italic=true;
                ev.insideMathForceDigitsUpright=false;
            }, 0);
            instructions["itsf"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.font=JKQTMathTextEnvironmentFont::MTEscript;
            }, 0);
            instructions["script"] = i;
            instructions["scr"] = i;
            instructions["scriptseries"] = i;
            instructions["scrseries"] = i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* /*parentMathText*/) {
                ev.font=JKQTMathTextEnvironmentFont::MTEscript;
                ev.bold=true;
            }, 0);
            instructions["bfscript"] = i;
            instructions["bfscr"] = i;
        }

        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize();
                ev.insideMathUseTextStyle=false;
            }, 0);
            instructions["displaystyle"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize();
                ev.insideMathUseTextStyle=true;
            }, 0);
            instructions["textstyle"]= i;
        }

        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize()*0.75;
                ev.insideMathUseTextStyle=true;
            }, 0);
            instructions["scriptstyle"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize()*0.5;
                ev.insideMathUseTextStyle=true;
            }, 0);
            instructions["scriptscriptstyle"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize()*6.0/11.0;
            }, 0);
            instructions["tiny"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize()*7.0/11.0;
            }, 0);
            instructions["ssmall"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize()*8.0/11.0;
            }, 0);
            instructions["scriptsize"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize()*9.0/11.0;
            }, 0);
            instructions["footnotesize"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize()*10.0/11.0;
            }, 0);
            instructions["small"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize()*11.0/11.0;
            }, 0);
            instructions["normalsize"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize()*12.0/11.0;
            }, 0);
            instructions["large"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize()*14.4/11.0;
            }, 0);
            instructions["Large"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize()*17.28/11.0;
            }, 0);
            instructions["LARGE"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize()*20.74/11.0;
            }, 0);
            instructions["huge"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, const JKQTMathText* parentMathText) {
                ev.fontSize=parentMathText->getFontSize()*24.88/11.0;
            }, 0);
            instructions["Huge"]= i;
        }
        {
            InstructionProperties i([](JKQTMathTextEnvironment& ev, const QStringList& parameters, const JKQTMathText* /*parentMathText*/) {
                ev.fontSize=parameters.value(0, QString::number(ev.fontSize)).toDouble();
                ev.fontSizeUnit=JKQTMathTextEnvironment::POINTS;
            }, 1);
            instructions["userfontsize"]= i;
            instructions["fontsize"]= i;
        }
        return instructions;
    }();
    return table;
}

JKQTMathTextModifiedEnvironmentInstructionNode::InstructionProperties::InstructionProperties():
    NParams(0),
    modifier([](JKQTMathTextEnvironment&, const QStringList&, const JKQTMathText*) {})
{

}

JKQTMathTextModifiedEnvironmentInstructionNode::InstructionProperties::InstructionProperties(const ModifyEnvironmentFunctor &_modifier, size_t _NParams):
    NParams(_NParams),
    modifier(_modifier)
{

}
