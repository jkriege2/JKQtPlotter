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



#include "jkqtmathtext/nodes/jkqtmathtextboxinstructionnode.h"
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







JKQTMathTextBoxInstructionNode::JKQTMathTextBoxInstructionNode(JKQTMathText* _parent, const QString& name, JKQTMathTextNode* child, const QStringList& parameters):
    JKQTMathTextInstruction1Node(_parent, name, child, parameters)
{
    fillInstructions();
}

JKQTMathTextBoxInstructionNode::~JKQTMathTextBoxInstructionNode() {
}


QString JKQTMathTextBoxInstructionNode::getTypeName() const
{
    return QLatin1String("JKQTMathTextBoxInstructionNode(")+instructionName+")";
}

JKQTMathTextNodeSize JKQTMathTextBoxInstructionNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const {
    JKQTMathTextEnvironment ev=currentEv;

    const auto& inst=instructions.value(getInstructionName());
    inst.modifier(ev, getParameters());
    const QPen p=inst.pen(ev, getParameters(), parentMathText);
    const QBrush b=inst.brush(ev, getParameters(), parentMathText);
    const QFontMetricsF fmNonItalic(JKQTMathTextGetNonItalic(currentEv.getFont(parentMathText)));
    const double lw=p.widthF();
    const double padding=inst.paddingFactor*fmNonItalic.tightBoundingRect("x").width();

    const JKQTMathTextNodeSize cs=getChild()->getSize(painter, ev);
    JKQTMathTextNodeSize s;
    s.width=cs.width+2.0*(padding+lw/2.0);
    s.baselineHeight=cs.baselineHeight+padding+lw/2.0;
    s.overallHeight=cs.overallHeight+2.0*(padding+lw/2.0);
    s.strikeoutPos=cs.strikeoutPos;
    return s;
}

double JKQTMathTextBoxInstructionNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathTextEnvironment ev=currentEv;

    const auto& inst=instructions.value(getInstructionName());
    inst.modifier(ev, getParameters());
    const QPen p=inst.pen(ev, getParameters(), parentMathText);
    const QBrush b=inst.brush(ev, getParameters(), parentMathText);
    const QFontMetricsF fmNonItalic(JKQTMathTextGetNonItalic(currentEv.getFont(parentMathText)));
    const double lw=p.widthF();
    const double padding=inst.paddingFactor*fmNonItalic.tightBoundingRect("x").width();
    const double rr=inst.roundingFactor*fmNonItalic.tightBoundingRect("x").width();
    const JKQTMathTextNodeSize cs=getChild()->getSize(painter, ev);

    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setBrush(b);
        painter.setPen(p);
        const QRectF rect(x+lw/2.0, y-cs.baselineHeight-padding-lw/2.0, cs.width+2.0*padding, cs.overallHeight+2.0*padding);
        if (rr>0) painter.drawRoundedRect(rect, rr, rr, Qt::AbsoluteSize);
        else painter.drawRect(rect);
        if (inst.doubleLine) {
            painter.setBrush(Qt::NoBrush);
            QPen p2=p;
            p2.setWidthF(p.widthF()*0.6);
            const QRectF recti(x+lw*2.5, y-cs.baselineHeight-lw/2.0-padding+2.0*lw, cs.width+2.0*padding-4.0*lw, cs.overallHeight+2.0*padding-4.0*lw);
            if (rr>0) painter.drawRoundedRect(recti, rr, rr, Qt::AbsoluteSize);
            else painter.drawRect(recti);
        }
    }

    double xnew = getChild()->draw(painter, x+padding+lw/2.0, y, ev);

    return xnew+padding+lw/2.0;
}

bool JKQTMathTextBoxInstructionNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const {
    JKQTMathTextEnvironment ev=currentEv;
    fillInstructions();
    const auto& inst=instructions.value(getInstructionName());
    inst.modifier(ev, getParameters());
    const QPen p=inst.pen(ev, getParameters(), parentMathText);
    const QBrush b=inst.brush(ev, getParameters(), parentMathText);
    const QFontMetricsF fmNonItalic(JKQTMathTextGetNonItalic(currentEv.getFont(parentMathText)));
    //const double lw=p.widthF();
    const double padding=inst.paddingFactor*fmNonItalic.tightBoundingRect("x").width();
    //const double rr=inst.roundingFactor*fmNonItalic.tightBoundingRect("x").width();
    QString s=QString("padding: %1px").arg(padding);
    if (p!=Qt::NoPen) {
        if (s.size()>0 && s.right(2)!="; ") s=s+"; ";
        if (p.widthF()>0 && p.color()!=Qt::transparent) s=s+"border-color: "+p.color().name();
        if (s.size()>0 && s.right(2)!="; ") s=s+"; ";
        if (p.widthF()>0 && p.color()!=Qt::transparent) s=s+QString("border-width: %1px").arg(p.width());
    }
    if (b!=Qt::NoBrush) {
        if (s.size()>0 && s.right(2)!="; ") s=s+"; ";
        if (b.color()!=Qt::transparent) s=s+"background-color: "+b.color().name();
    }

    if (s.size()>0) html=html+ QString("<span style=\"%1\">").arg(s);
    bool ok=getChild()->toHtml(html, ev, defaultEv);
    if (s.size()>0) html=html+"</span>";
    return ok;
}

bool JKQTMathTextBoxInstructionNode::supportsInstructionName(const QString &instructionName)
{
    fillInstructions();
    return instructions.contains(instructionName);
}

size_t JKQTMathTextBoxInstructionNode::countParametersOfInstruction(const QString &instructionName)
{
    fillInstructions();
    if (instructions.contains(instructionName)) return instructions[instructionName].NParams;
    return 0;
}

void JKQTMathTextBoxInstructionNode::modifyInMathEnvironment(const QString &instructionName, bool &insideMath, bool& insideMathTextStyle, const QStringList& params)
{
    fillInstructions();
    if (instructions.contains(instructionName)) {
        JKQTMathTextEnvironment ev;
        ev.insideMath=insideMath;
        ev.insideMathUseTextStyle=insideMathTextStyle;
        instructions[instructionName].modifier(ev, params);
        insideMath=ev.insideMath;
        insideMathTextStyle=ev.insideMathUseTextStyle;
    }
}


QHash<QString, JKQTMathTextBoxInstructionNode::InstructionProperties> JKQTMathTextBoxInstructionNode::instructions;

void JKQTMathTextBoxInstructionNode::fillInstructions()
{

    {
        InstructionProperties i(InstructionProperties::NoModification,
                                InstructionProperties::DefaultPen,
                                InstructionProperties::NoBrush,
                                InstructionProperties::DefaultPadding,
                                /*Nparams=*/0);
        instructions["fbox"] = i;
        instructions["framebox"] = i;
        instructions["boxed"] = i;
        instructions["framed"] = i;
    }
    {
        InstructionProperties i(InstructionProperties::NoModification,
                                InstructionProperties::DefaultPen,
                                InstructionProperties::NoBrush,
                                InstructionProperties::DefaultPadding,
                                /*Nparams=*/0);
        i.doubleLine=true;
        instructions["doublebox"] = i;
    }
    {
        InstructionProperties i(InstructionProperties::NoModification,
                                InstructionProperties::DefaultPen,
                                InstructionProperties::NoBrush,
                                InstructionProperties::DefaultPadding,
                                /*Nparams=*/0);
        i.roundingFactor=0.7;
        instructions["ovalbox"] = i;
    }
    {
        InstructionProperties i(InstructionProperties::NoModification,
                                [](JKQTMathTextEnvironment& ev, const QStringList& parameters, JKQTMathText* parent){
                                    QPen p=InstructionProperties::DefaultPen(ev, parameters, parent);
                                    p.setWidthF(p.widthF()*1.5);
                                    return p;
                                },
                                InstructionProperties::NoBrush,
                                InstructionProperties::DefaultPadding,
                                /*Nparams=*/0);
        i.roundingFactor=0.8;
        instructions["Ovalbox"] = i;
    }
    {
        InstructionProperties i(InstructionProperties::NoModification,
                                InstructionProperties::DefaultPen,
                                InstructionProperties::NoBrush,
                                InstructionProperties::DefaultPadding,
                                /*Nparams=*/0);
        i.roundingFactor=0.7;
        i.doubleLine=true;
        instructions["ovaldoublebox"] = i;
    }
    {
        InstructionProperties i(InstructionProperties::NoModification,
                                [](JKQTMathTextEnvironment& ev, const QStringList& parameters, JKQTMathText* parent){
                                    QPen p=InstructionProperties::DefaultPen(ev, parameters, parent);
                                    p.setColor(jkqtp_String2QColor(parameters.value(0, p.color().name())));
                                    return p;
                                },
                                InstructionProperties::NoBrush,
                                InstructionProperties::DefaultPadding,
                                /*Nparams=*/1);
        instructions["colorbox"] = i;
    }
    {
        InstructionProperties i(InstructionProperties::NoModification,
                                [](JKQTMathTextEnvironment& ev, const QStringList& parameters, JKQTMathText* parent){
                                    QPen p=InstructionProperties::DefaultPen(ev, parameters, parent);
                                    p.setStyle(Qt::DashLine);
                                    return p;
                                },
                                InstructionProperties::NoBrush,
                                InstructionProperties::DefaultPadding,
                                /*Nparams=*/0);
        instructions["dashbox"] = i;
        instructions["dashedbox"] = i;
        instructions["dbox"] = i;
    }
    {
        InstructionProperties i(InstructionProperties::NoModification,
                                [](JKQTMathTextEnvironment& ev, const QStringList& parameters, JKQTMathText* parent){
                                    QPen p=InstructionProperties::DefaultPen(ev, parameters, parent);
                                    p.setStyle(Qt::DotLine);
                                    return p;
                                },
                                InstructionProperties::NoBrush,
                                InstructionProperties::DefaultPadding,
                                /*Nparams=*/0);
        instructions["dottedbox"] = i;
        instructions["dotbox"] = i;
    }
    {
        InstructionProperties i(InstructionProperties::NoModification,
                                InstructionProperties::NoPen,
                                [](JKQTMathTextEnvironment& /*ev*/, const QStringList& parameters, JKQTMathText* /*parent*/){
                                    return QBrush(jkqtp_String2QColor(parameters.value(0, QColor(Qt::transparent).name())), Qt::SolidPattern);
                                },
                                InstructionProperties::DefaultPadding,
                                /*Nparams=*/1);
        instructions["shaded"] = i;
    }
    {
        InstructionProperties i(InstructionProperties::NoModification,
                                InstructionProperties::NoPen,
                                [](JKQTMathTextEnvironment& /*ev*/, const QStringList& parameters, JKQTMathText* /*parent*/){
                                    return QBrush(jkqtp_String2QColor(parameters.value(0, QColor(Qt::transparent).name())), Qt::SolidPattern);
                                },
                                0,
                                /*Nparams=*/1);
        instructions["snugshade"] = i;
    }
    {
        InstructionProperties i(InstructionProperties::NoModification,
                                InstructionProperties::DefaultPen,
                                InstructionProperties::NoBrush,
                                0,
                                /*Nparams=*/0);
        instructions["snugbox"] = i;
    }
    {
        InstructionProperties i(InstructionProperties::NoModification,
                                [](JKQTMathTextEnvironment& ev, const QStringList& parameters, JKQTMathText* parent){
                                    QPen p=InstructionProperties::DefaultPen(ev, parameters, parent);
                                    p.setColor(jkqtp_String2QColor(parameters.value(0, p.color().name())));
                                    return p;
                                },
                                [](JKQTMathTextEnvironment& /*ev*/, const QStringList& parameters, JKQTMathText* /*parent*/){
                                    return QBrush(jkqtp_String2QColor(parameters.value(1, QColor(Qt::transparent).name())), Qt::SolidPattern);
                                },
                                InstructionProperties::DefaultPadding,
                                /*Nparams=*/2);
        instructions["fcolorbox"] = i;
    }
}

JKQTMathTextBoxInstructionNode::InstructionProperties::ModifyEnvironmentFunctor JKQTMathTextBoxInstructionNode::InstructionProperties::NoModification=
        [](JKQTMathTextEnvironment& /*ev*/, const QStringList& /*parameters*/){};

JKQTMathTextBoxInstructionNode::InstructionProperties::GetBoxPenFunctor JKQTMathTextBoxInstructionNode::InstructionProperties::DefaultPen=
        [](JKQTMathTextEnvironment& ev, const QStringList& /*parameters*/, JKQTMathText* parent){ return QPen(ev.color, QFontMetricsF(ev.getFont(parent)).lineWidth(), Qt::SolidLine); };

JKQTMathTextBoxInstructionNode::InstructionProperties::GetBoxPenFunctor JKQTMathTextBoxInstructionNode::InstructionProperties::NoPen=
        [](JKQTMathTextEnvironment& /*ev*/, const QStringList& /*parameters*/, JKQTMathText* /*parent*/){ return Qt::NoPen; };

JKQTMathTextBoxInstructionNode::InstructionProperties::GetBoxBrushFunctor JKQTMathTextBoxInstructionNode::InstructionProperties::NoBrush=
        [](JKQTMathTextEnvironment& /*ev*/, const QStringList& /*parameters*/, JKQTMathText* /*parent*/){ return Qt::NoBrush; };

double JKQTMathTextBoxInstructionNode::InstructionProperties::DefaultPadding=0.5;

JKQTMathTextBoxInstructionNode::InstructionProperties::InstructionProperties():
    InstructionProperties(NoModification, DefaultPen, NoBrush, DefaultPadding, 0)
{

}

JKQTMathTextBoxInstructionNode::InstructionProperties::InstructionProperties(const GetBoxPenFunctor &_pen, const GetBoxBrushFunctor &_brush, double _paddingFactor, size_t _NParams):
    InstructionProperties(NoModification, _pen, _brush, _paddingFactor, _NParams)

{

}

JKQTMathTextBoxInstructionNode::InstructionProperties::InstructionProperties(const ModifyEnvironmentFunctor &_modifier, const GetBoxPenFunctor &_pen, const GetBoxBrushFunctor &_brush, double _paddingFactor, size_t _NParams):
    NParams(_NParams),
    modifier(_modifier),
    pen(_pen),
    brush(_brush),
    paddingFactor(_paddingFactor),
    doubleLine(false),
    roundingFactor(0.0)
{

}
