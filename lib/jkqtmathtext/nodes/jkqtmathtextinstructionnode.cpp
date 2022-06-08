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





JKQTMathTextInstruction1Node::JKQTMathTextInstruction1Node(JKQTMathText* _parent, const QString& name, JKQTMathTextNode* child, const QStringList& parameters):
    JKQTMathTextSingleChildNode(child, _parent)
{
    this->name=name;
    this->parameters=parameters;

    JKQTMathTextEnvironment ev;
    if (!setupMTenvironment(ev)) {
        parentMathText->addToErrorList(QObject::tr("unknown instruction '%1' found!").arg(name));
    }
}

JKQTMathTextInstruction1Node::~JKQTMathTextInstruction1Node() {
}

QString JKQTMathTextInstruction1Node::getTypeName() const
{
    return QLatin1String("MTinstruction1Node(")+name+")";
}

void JKQTMathTextInstruction1Node::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    JKQTMathTextEnvironment ev=currentEv;

    setupMTenvironment(ev);

    child->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);
    if (name=="colorbox" || name=="fbox" || name=="boxed") {
        QFontMetricsF fm(ev.getFont(parentMathText));
        double xw=fm.boundingRect("x").width();
        width+=xw;
        overallHeight+=xw;
        baselineHeight+=xw/2.0;
    }
}

double JKQTMathTextInstruction1Node::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathTextEnvironment ev=currentEv;

    setupMTenvironment(ev);

    QPen oldPen=painter.pen();
    double shiftX=0;
    if (name=="colorbox" || name=="fbox" || name=="boxed") {
        QColor fcol=currentEv.color;
        if (name=="colorbox") fcol=QColor(parameters.value(0, ev.color.name()));
        //qDebug()<<"COLOR="<<fcol;
        double width, baselineHeight, overallHeight, strikeoutPos;
        child->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);
        QPen p=painter.pen();
        QFontMetricsF fm(currentEv.getFont(parentMathText));
        double xw=fm.boundingRect("x").width();
        p.setColor(fcol);
        painter.setPen(p);
        painter.drawRect(QRectF(x,y-baselineHeight-xw/2,width+xw,overallHeight+xw));
        shiftX=xw/2.0;
    }

    double xnew= child->draw(painter, x+shiftX, y, ev);
    painter.setPen(oldPen);
    return xnew;
}

bool JKQTMathTextInstruction1Node::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
    JKQTMathTextEnvironment ev=currentEv;

    setupMTenvironment(ev);

    return child->toHtml(html, ev, defaultEv);
}

QString JKQTMathTextInstruction1Node::getName() const {
    return this->name;
}

QStringList JKQTMathTextInstruction1Node::getParameters() const {
    return this->parameters;
}

bool JKQTMathTextInstruction1Node::setupMTenvironment(JKQTMathTextEnvironment &ev) const
{
    if (name=="bf" || name=="textbf" || name=="mathbf") ev.bold=true;
    else if (name=="em") ev.italic=!ev.italic;
    else if (name=="it" || name=="textit" || name=="mathit") ev.italic=true;
    else if (name=="textcolor" || name=="mathcolor" || name=="color") ev.color=QColor(parameters.value(0, ev.color.name()));
    else if (name=="ensuremath" || name=="equation") { ev.italic=true; ev.insideMath=true; }
    else if (name=="sc" || name=="textsc" || name=="mathsc") ev.smallCaps=true;
    else if (name=="ul" || name=="underline" || name=="underlined") ev.underlined=true;
    else if (name=="ol" || name=="overline" || name=="overlined") ev.overline=true;
    else if (name=="strike") ev.strike=true;
    else if (name=="rm" || name=="textrm") { ev.font=JKQTMathTextEnvironmentFont::MTEroman; ev.italic=false; }
    else if (name=="mathrm" || name=="unit" ||  name=="operatorname") { ev.font=JKQTMathTextEnvironmentFont::MTEroman; ev.italic=false; }
    else if (name=="mathbfit" || name=="bfit" || name=="textbfit") { ev.bold=true; ev.italic=true; }
    else if (name=="text" || name=="mbox" || name=="ensuretext") { ev.insideMath=false; ev.font=JKQTMathTextEnvironmentFont::MTEroman; ev.italic=false; }
    else if (name=="mat") { ev.font=JKQTMathTextEnvironmentFont::MTEroman; ev.italic=false; ev.bold=true; }
    else if (name=="cal" || name=="textcal" || name=="mathcal") { ev.font=JKQTMathTextEnvironmentFont::MTEcaligraphic; ev.italic=false; }
    else if (name=="fcal" || name=="textfcal" || name=="mathfcal") { ev.font=JKQTMathTextEnvironmentFont::MTEcaligraphic; ev.bold=true; }
    else if (name=="frak" || name=="textfrak" || name=="mathfrak") { ev.font=JKQTMathTextEnvironmentFont::MTEfraktur; ev.italic=false; }
    else if (name=="ffrak" || name=="textffrak" || name=="mathffrak") { ev.font=JKQTMathTextEnvironmentFont::MTEfraktur; ev.bold=true; }
    else if (name=="bb" || name=="textbb" || name=="mathbb") { ev.font=JKQTMathTextEnvironmentFont::MTEblackboard; ev.italic=false; }
    else if (name=="tt" || name=="texttt" || name=="mathtt") { ev.font=JKQTMathTextEnvironmentFont::MTEtypewriter; ev.italic=false; }
    else if (name=="sf" || name=="textsf" || name=="mathsf") { ev.font=JKQTMathTextEnvironmentFont::MTEsans; ev.italic=false; }
    else if (name=="sfit" || name=="textsfit" || name=="mathsfit") { ev.font=JKQTMathTextEnvironmentFont::MTEsans; ev.italic=true; }
    else if (name=="script" || name=="scr" || name=="textscript" || name=="textscr" || name=="mathscript" || name=="mathscr") { ev.font=JKQTMathTextEnvironmentFont::MTEscript; ev.italic=false; }
    else if (name=="fscript" || name=="fscr" || name=="textfscript" || name=="textfscr" || name=="mathfscript" || name=="mathfscr") { ev.font=JKQTMathTextEnvironmentFont::MTEscript; ev.bold=true; ev.italic=false; }
    else if (name=="displaystyle") { ev.fontSize=ev.fontSize/0.8; }
    else if (name=="scriptstyle") { ev.fontSize=ev.fontSize*0.8; }
    else if (name=="scriptscriptstyle") { ev.fontSize=ev.fontSize*0.8*0.8; }
    else {
        return false;
    }

    return true;
}
