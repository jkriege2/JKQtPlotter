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



#include "jkqtmathtext/nodes/jkqtmathtextdecoratednode.h"
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






QString JKQTMathTextDecoratedNode::DecorationType2String(JKQTMathTextDecoratedNode::DecorationType mode)
{
    switch(mode) {
        case MTDvec:
            return "vec";
        case MTDgrave:
            return "grave";
        case MTDacute:
            return "acute";
        case MTDtilde:
            return "tilde";
        case MTDbreve:
            return "breve";
        case MTDwidetilde:
            return "widetilde";
        case MTDhat:
            return "hat";
        case MTDwidehat:
            return "widehat";
        case MTDcheck:
            return "check";
        case MTDwidecheck:
            return "widecheck";
        case MTDocirc:
            return "ocirc";
        case MTDdot:
            return "dot";
        case MTDddot:
            return "ddot";
        case MTDbar:
            return "bar";
        case MTDarrow:
            return "arrow";
        case MTDoverline:
            return "overline";
        case MTDdoubleoverline:
            return "double overline";
        case MTDunderline:
            return "underline";
        case MTDdoubleunderline:
            return "double underline";
        case MTDcancel:
            return "cancel";
        case MTDbcancel:
            return "bcancel";
        case MTDxcancel:
            return "xcancel";
        case MTDstrike:
            return "strike";
        case MTDoverleftarrow:
            return "overleftarrow";
        case MTDoverrightarrow:
            return "overrightarrow";
        case MTDoverleftrightarrow:
            return "overleftrightarrow";
        case MTDunderleftarrow:
            return "underleftarrow";
        case MTDunderrightarrow:
            return "underrightarrow";
        case MTDunderleftrightarrow:
            return "underleftrightarrow";
        case MTDunderlineDashed:
            return "underlineDashed";
        case MTDunderlineDotted:
            return "MTDunderlineDotted";
    }
    return "unknown";
}

JKQTMathTextDecoratedNode::DecorationType JKQTMathTextDecoratedNode::InstructionName2DecorationType(const QString &mode)
{
    fillInstructions();
    return instructions[mode];
}

bool JKQTMathTextDecoratedNode::supportsInstructionName(const QString &instructionName)
{
    fillInstructions();
    return instructions.contains(instructionName);
}



JKQTMathTextDecoratedNode::JKQTMathTextDecoratedNode(JKQTMathText* _parent, DecorationType decoration, JKQTMathTextNode* child):
    JKQTMathTextSingleChildNode(child, _parent)
{
    this->decoration=decoration;
}

JKQTMathTextDecoratedNode::~JKQTMathTextDecoratedNode() {
}

void JKQTMathTextDecoratedNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos) {
    JKQTMathTextEnvironment ev=currentEv;
    double cwidth=0, cbaselineHeight=0, coverallHeight=0, cstrikeoutPos=0;
    getChild()->getSize(painter, ev, cwidth, cbaselineHeight, coverallHeight, cstrikeoutPos);
    const double cDescent=coverallHeight-cbaselineHeight;
    const QFont font=ev.getFont(parentMathText);
    const QFontMetricsF fm(font, painter.device());
    const double decoSeparation=parentMathText->getDecorationSeparationFactor()*fm.ascent();
    const double deco_height=parentMathText->getDecorationHeightFactor()*fm.ascent();
    const double deco_ypos=cbaselineHeight+decoSeparation;
    const double decoAboveAscent_ypos=fm.ascent()+decoSeparation;
    const double decobelow_ypos=cDescent+decoSeparation;
    const double italic_xcorrection=getNonItalicXCorretion(painter, cwidth, ev, getChild());
    const double deco_miniwidth=((decoration==MTDtilde||decoration==MTDbreve)?fm.boundingRect("~").width():fm.boundingRect("^").width())-italic_xcorrection;
    const double linewidth=qMax(parentMathText->ABS_MIN_LINEWIDTH, fm.lineWidth());



    double descent=coverallHeight-cbaselineHeight;
    double ascent=cbaselineHeight;
    if (decoration==MTDbar) {
        ascent=std::max<double>(baselineHeight+decoSeparation, decoAboveAscent_ypos)+linewidth/2.0;
    } else if (decoration==MTDunderline || decoration==MTDunderlineDashed || decoration==MTDunderlineDotted) {
        descent=std::max<double>(decobelow_ypos, cDescent)+linewidth/2.0;
    } else if (decoration==MTDdoubleunderline) {
        descent=std::max<double>(decobelow_ypos, cDescent)+3.5*linewidth;
    } else if (decoration==MTDunderleftarrow || decoration==MTDunderrightarrow || decoration==MTDunderleftrightarrow) {
        descent=std::max<double>(decobelow_ypos, cDescent)+0.5*linewidth+deco_height;
    } else {
        ascent=deco_ypos+deco_height;
    }
    overallHeight=ascent+descent;
    baselineHeight=ascent;
    strikeoutPos=cstrikeoutPos;
    width=std::max<double>(deco_miniwidth,cwidth);
}

QHash<QString, JKQTMathTextDecoratedNode::DecorationType> JKQTMathTextDecoratedNode::instructions;

void JKQTMathTextDecoratedNode::fillInstructions()
{
    if (instructions.size()>0) return;

    instructions["vec"]=MTDvec;
    instructions["overline"]=MTDoverline;
    instructions["oline"]=MTDoverline;
    instructions["ol"]=MTDoverline;
    instructions["underline"]=MTDunderline;
    instructions["uline"]=MTDunderline;
    instructions["ul"]=MTDunderline;
    instructions["dashuline"]=MTDunderlineDashed;
    instructions["dotuline"]=MTDunderlineDotted;
    instructions["uuline"]=MTDdoubleunderline;
    instructions["uul"]=MTDdoubleunderline;
    instructions["ooline"]=MTDdoubleoverline;
    instructions["ool"]=MTDdoubleoverline;
    instructions["arrow"]=MTDarrow;
    instructions["overrightarrow"]=MTDarrow;
    instructions["overarrow"]=MTDarrow;
    instructions["hat"]=MTDhat;
    instructions["^"]=MTDhat;
    instructions["widehat"]=MTDwidehat;
    instructions["check"]=MTDcheck;
    instructions["v"]=MTDcheck;
    instructions["widecheck"]=MTDwidecheck;
    instructions["bar"]=MTDbar;
    instructions["="]=MTDbar;
    instructions["dot"]=MTDdot;
    instructions["."]=MTDdot;
    instructions["ocirc"]=MTDocirc;
    instructions["tilde"]=MTDtilde;
    instructions["~"]=MTDtilde;
    instructions["acute"]=MTDacute;
    instructions["'"]=MTDacute;
    instructions["grave"]=MTDgrave;
    instructions["`"]=MTDgrave;
    instructions["breve"]=MTDbreve;
    instructions["u"]=MTDbreve;
    instructions["widetilde"]=MTDwidetilde;
    instructions["ddot"]=MTDddot;
    instructions["cancel"]=MTDcancel;
    instructions["xcancel"]=MTDxcancel;
    instructions["bcancel"]=MTDbcancel;
    instructions["strike"]=MTDstrike;
    instructions["st"]=MTDstrike;
    instructions["sout"]=MTDstrike;
    instructions["overleftarrow"]=MTDoverleftarrow;
    instructions["overrightarrow"]=MTDoverrightarrow;
    instructions["overleftrightarrow"]=MTDoverleftrightarrow;
    instructions["underleftarrow"]=MTDunderleftarrow;
    instructions["underrightarrow"]=MTDunderrightarrow;
    instructions["underleftrightarrow"]=MTDunderleftrightarrow;

}

double JKQTMathTextDecoratedNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathTextEnvironment ev=currentEv;
    double cwidth=0, cbaselineHeight=0, coverallHeight=0, cstrikeoutPos=0;
    getChild()->getSize(painter, ev, cwidth, cbaselineHeight, coverallHeight, cstrikeoutPos);
    const double cDescent=coverallHeight-cbaselineHeight;
    const QFont font=ev.getFont(parentMathText);
    const QFontMetricsF fm(font, painter.device());
    const double width_X=fm.boundingRect("X").width();
    const double width_x=fm.boundingRect("x").width();
    const double width_dot=fm.boundingRect(".").width()/2.0;
    const double decoSeparation=parentMathText->getDecorationSeparationFactor()*fm.ascent();
    const double linewidth=qMax(parentMathText->ABS_MIN_LINEWIDTH, fm.lineWidth());
    const double linewidthArrow=linewidth*0.65;
    const double deco_height=parentMathText->getDecorationHeightFactor()*fm.ascent();
    const double decoAboveAscent_ypos=y-fm.ascent()-decoSeparation;
    const double strike_ypos=y-cbaselineHeight/2.0;
    const double decobelow_ypos=y+cDescent+decoSeparation;
    const double italic_xcorrection=getNonItalicXCorretion(painter, cwidth, ev, getChild());
    const double deco_xoffset=parentMathText->getDecorationWidthReductionXFactor()*width_X/2.0;
    const double deco_width=std::max<double>(width_x*0.5,cwidth-2.0*deco_xoffset-italic_xcorrection);
    const double deco_vecwidth=width_x*0.18;
    const double deco_vecheight=deco_height*0.25;
    const double deco_accentwidth=deco_height/4.0;
    const double deco_miniwidth=((decoration==MTDtilde||decoration==MTDbreve)?fm.boundingRect("j").width():fm.boundingRect("^").width())-italic_xcorrection;
    const double decotop_xcenter=x+italic_xcorrection+(cwidth-italic_xcorrection)/2.0;
    const double decotop_xstart=decotop_xcenter-deco_width/2.0+linewidth/2.0;
    const double decotop_xend=decotop_xcenter+deco_width/2.0-linewidth/2.0;
    const double decobot_xstart=x+linewidth/2.0;
    const double decobot_xend=x+cwidth-italic_xcorrection-linewidth/2.0;
    //const double decobot_xcenter=(decobot_xstart+decobot_xend)/2.0;
    const double deco_ytopbot=y-cbaselineHeight-decoSeparation-linewidth/2.0;
    const double deco_ytoptop=y-cbaselineHeight-decoSeparation-deco_height+linewidth/2.0;
    const double deco_ytopcenter=y-cbaselineHeight-decoSeparation-deco_height/2.0;



    QPen pold=painter.pen();
    QPen p=pold;
    p.setColor(ev.color);
    p.setWidthF(linewidthArrow);
    p.setCapStyle(Qt::SquareCap);
    p.setJoinStyle(Qt::RoundJoin);

    QPen pul=p;
    pul.setWidthF(linewidth);
    pul.setCapStyle(Qt::SquareCap);



    double xnew=getChild()->draw(painter, x, y, ev);

    auto fDrawFontAccent=[&](QChar aDirect=QChar(), QChar aFallback=QChar()) -> bool {
        if (!aDirect.isNull() && fm.inFont(aDirect)) {
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            const QRectF tbra=fm.tightBoundingRect(aDirect);
            painter.translate(decotop_xcenter-tbra.width()/2.0, (deco_ytopcenter+deco_ytoptop)/2.0);
            //painter.setPen("red");
            //painter.drawEllipse(0-2,0-2,4,4);
            painter.translate(-tbra.x(), -tbra.y());
            //painter.setPen("magenta");
            //painter.drawEllipse(0-2,0-2,4,4);
            if (tbra.width()>0 && tbra.height()>0) {
                painter.setFont(font);
                painter.drawText(0,0,aDirect);
                //painter.setPen("blue");
                //painter.drawRect(tbra);
                //painter.drawEllipse(0-2,0-2,4,4);
                return true;
            }
        }
        if (!aFallback.isNull() && fm.inFont(aFallback)) {
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            const QRectF tbra=fm.tightBoundingRect(aFallback);
            painter.translate(decotop_xcenter-tbra.width()/2.0, (deco_ytopcenter+deco_ytoptop)/2.0);
            //painter.setPen("yellow");
            //painter.drawEllipse(0-2,0-2,4,4);
            painter.translate(-tbra.x(), -tbra.y());
            //painter.setPen("cyan");
            //painter.drawEllipse(0-2,0-2,4,4);
            if (tbra.width()>0 && tbra.height()>0) {
                painter.setFont(font);
                painter.drawText(0,0,aFallback);
                //painter.setPen("green");
                //painter.drawRect(tbra);
                //painter.drawEllipse(0-2,0-2,4,4);
                return true;
            }
        }
        return false;
    };

    if (decoration==MTDvec) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(decotop_xstart, deco_ytopbot)<<QPointF(decotop_xend, deco_ytopbot)<<QPointF(decotop_xend-deco_vecwidth, deco_ytopbot-deco_vecheight);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    } else if (decoration==MTDoverline) {
        painter.setPen(pul);
        const QLineF l(decotop_xstart, deco_ytopbot, decotop_xend, deco_ytopbot);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDbar) {
        painter.setPen(pul);
        const QLineF l(decotop_xstart, decoAboveAscent_ypos, decotop_xend, decoAboveAscent_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDdoubleoverline) {
        painter.setPen(pul);
        const QLineF l(decotop_xstart, deco_ytopbot, decotop_xend, deco_ytopbot);
        if (l.length()>0) painter.drawLine(l);
        const QLineF l2(decotop_xstart, deco_ytopbot-2.5*pul.widthF(), decotop_xend, deco_ytopbot-2.5*pul.widthF());
        if (l2.length()>0) painter.drawLine(l2);
        painter.setPen(pold);
    } else if (decoration==MTDunderline || decoration==MTDunderlineDashed || decoration==MTDunderlineDotted) {
        if (decoration==MTDunderlineDashed) pul.setStyle(Qt::DashLine);
        if (decoration==MTDunderlineDotted) pul.setStyle(Qt::DotLine);
        painter.setPen(pul);
        const QLineF l(decobot_xstart, decobelow_ypos, decobot_xend, decobelow_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDdoubleunderline) {
        painter.setPen(pul);
        const QLineF l(decobot_xstart, decobelow_ypos, decobot_xend, decobelow_ypos);
        if (l.length()>0) painter.drawLine(l);
        QLineF l2(decobot_xstart, decobelow_ypos+2.5*pul.widthF(), decobot_xend, decobelow_ypos+2.5*pul.widthF());
        if (l2.length()>0) painter.drawLine(l2);
        painter.setPen(pold);
    } else if (decoration==MTDarrow) {
        painter.setPen(p);
        const QLineF l(decotop_xstart, deco_ytopcenter, decotop_xend, deco_ytopcenter);
        if (l.length()>0) painter.drawLine(l);
        QPolygonF poly;
        poly<<QPointF(decotop_xend-deco_vecwidth, deco_ytopbot)<<QPointF(decotop_xend, deco_ytopcenter)<<QPointF(decotop_xend-deco_vecwidth, deco_ytoptop);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    } else if (decoration==MTDoverrightarrow) {
        painter.setPen(p);
        const QPainterPath path=JKQTMathTextMakeArrow(decotop_xstart, deco_ytopcenter, deco_width, deco_height, false, true);
        painter.drawPath(path);
        painter.setPen(pold);
    } else if (decoration==MTDoverleftarrow) {
        painter.setPen(p);
        const QPainterPath path=JKQTMathTextMakeArrow(decotop_xstart, deco_ytopcenter, deco_width, deco_height, true, false);
        painter.drawPath(path);
        painter.setPen(pold);
    } else if (decoration==MTDoverleftrightarrow) {
        painter.setPen(p);
        const QPainterPath path=JKQTMathTextMakeArrow(decotop_xstart, deco_ytopcenter, deco_width, deco_height, true, true);
        painter.drawPath(path);
        painter.setPen(pold);
    } else if (decoration==MTDunderrightarrow) {
        painter.setPen(p);
        const QPainterPath path=JKQTMathTextMakeArrow(decobot_xstart, decobelow_ypos+deco_height/2.0, decobot_xend-decobot_xstart, deco_height, false, true);
        painter.drawPath(path);
        painter.setPen(pold);
    } else if (decoration==MTDunderleftarrow) {
        painter.setPen(p);
        const QPainterPath path=JKQTMathTextMakeArrow(decobot_xstart, decobelow_ypos+deco_height/2.0, decobot_xend-decobot_xstart, deco_height, true, false);
        painter.drawPath(path);
        painter.setPen(pold);
    } else if (decoration==MTDunderleftrightarrow) {
        painter.setPen(p);
        const QPainterPath path=JKQTMathTextMakeArrow(decobot_xstart, decobelow_ypos+deco_height/2.0, decobot_xend-decobot_xstart, deco_height, true, true);
        painter.drawPath(path);
        painter.setPen(pold);
    } else if (decoration==MTDhat) {
        if (!fDrawFontAccent(QChar(0x302), QChar(0x2c6))) {
            painter.setPen(p);
            QPolygonF poly;
            poly<<QPointF(decotop_xcenter-0.5*deco_miniwidth, deco_ytopbot)<<QPointF(decotop_xcenter, deco_ytopcenter)<<QPointF(decotop_xcenter+0.5*deco_miniwidth, deco_ytopbot);
            painter.drawPolyline(poly);
            painter.setPen(pold);
        }
    } else if (decoration==MTDwidehat) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(decotop_xstart, deco_ytopbot)<<QPointF(decotop_xcenter, deco_ytoptop)<<QPointF(decotop_xend, deco_ytopbot);
        painter.drawPolyline(poly);
        painter.setPen(pold);

    } else if (decoration==MTDcheck) {
        if (!fDrawFontAccent(QChar(0x30C), QChar(0x2C7))) {
            painter.setPen(p);
            QPolygonF poly;
            poly<<QPointF(decotop_xcenter-0.5*deco_miniwidth, deco_ytopcenter)<<QPointF(decotop_xcenter, deco_ytopbot)<<QPointF(decotop_xcenter+0.5*deco_miniwidth, deco_ytopcenter);
            painter.drawPolyline(poly);
            painter.setPen(pold);
        }
    } else if (decoration==MTDwidecheck) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(decotop_xstart, deco_ytoptop)<<QPointF(decotop_xcenter, deco_ytopbot)<<QPointF(decotop_xend, deco_ytoptop);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    } else if (decoration==MTDbreve) {
        if (!fDrawFontAccent(QChar(0x306), QChar(0x2D8))) {
            painter.setPen(p);
            QPainterPath path;
            const double x0=decotop_xcenter-deco_miniwidth/2.0;
            const double x1=decotop_xcenter+deco_miniwidth/2.0;
            path.moveTo(x0, deco_ytopbot);
            path.cubicTo(x0, deco_ytoptop,  x1, deco_ytoptop    , x1, deco_ytopbot);
            painter.drawPath(path);
            painter.setPen(pold);
        }
    } else if (decoration==MTDtilde) {
        if (!fDrawFontAccent(QChar(0x2DC), QChar('~'))) {
            painter.setPen(p);
            QPainterPath path;
            const double x0=decotop_xcenter-deco_miniwidth/2.0;
            const double x1=decotop_xcenter+deco_miniwidth/2.0;
            path.moveTo(x0, deco_ytopcenter);
            path.cubicTo((x0+x1)/2.0, deco_ytopbot,  (x0+x1)/2.0, deco_ytoptop    , x1, deco_ytopcenter);
            painter.drawPath(path);
            painter.setPen(pold);
        }
    } else if (decoration==MTDacute) {
        if (!fDrawFontAccent(QChar(0x301), QChar(0x2CA))) {
            painter.setPen(pul);
            painter.drawLine(QLineF(decotop_xcenter, deco_ytopbot, decotop_xcenter-deco_accentwidth, deco_ytopcenter));
            painter.setPen(pold);
        }
    } else if (decoration==MTDgrave) {
        if (!fDrawFontAccent(QChar(0x300), QChar(0x2CB))) {
            painter.setPen(pul);
            painter.drawLine(QLineF(decotop_xcenter, deco_ytopbot, decotop_xcenter+deco_accentwidth, deco_ytopcenter));
            painter.setPen(pold);
        }

    } else if (decoration==MTDwidetilde) {
        painter.setPen(p);
        QPainterPath path;
        const double x0=decotop_xstart;
        const double x1=decotop_xend;
        path.moveTo(x0, deco_ytopcenter);
        path.cubicTo((x0+x1)/2.0, deco_ytopbot,  (x0+x1)/2.0, deco_ytoptop    , x1, deco_ytopcenter);
        painter.drawPath(path);
        painter.setPen(pold);

    } else if (decoration==MTDocirc) {
        if (!fDrawFontAccent(QChar(0x2DA), QChar(0xBA))) {
            painter.setPen(p);
            const QBrush bold=painter.brush();
            painter.setBrush(Qt::NoBrush);
            const double r=qMin(width_dot/1.5, deco_height/2.0-linewidth/2.0);
            painter.drawEllipse(QPointF(decotop_xcenter, deco_ytopcenter), r, r);
            painter.setBrush(bold);
            painter.setPen(pold);
        }
    } else if (decoration==MTDdot) {
        if (!fDrawFontAccent(QChar(0x2D9), QChar('.'))) {
            p.setWidthF(0.01);
            painter.setPen(p);
            QBrush b=painter.brush();
            const QBrush bold=b;
            b.setColor(ev.color);
            b.setStyle(Qt::SolidPattern);
            painter.setBrush(b);
            painter.drawEllipse(QPointF(decotop_xcenter, deco_ytopcenter), width_dot/2.0, width_dot/2.0);
            painter.setBrush(bold);
            painter.setPen(pold);
        }
    } else if (decoration==MTDddot) {
        if (!fDrawFontAccent(QChar(0x308))) {
            p.setWidthF(0.01);
            painter.setPen(p);
            QBrush b=painter.brush();
            const QBrush bold=b;
            b.setColor(ev.color);
            b.setStyle(Qt::SolidPattern);
            painter.setBrush(b);
            painter.drawEllipse(QPointF(decotop_xcenter-width_dot, deco_ytopcenter), width_dot/2.0, width_dot/2.0);
            painter.drawEllipse(QPointF(decotop_xcenter+width_dot, deco_ytopcenter), width_dot/2.0, width_dot/2.0);
            painter.setBrush(bold);
            painter.setPen(pold);
            painter.setBrush(bold);
        }
    } else if (decoration==MTDstrike) {
        painter.setPen(pul);
        const QLineF l((decotop_xstart+decobot_xstart)/2.0, strike_ypos, (decotop_xend+decobot_xend)/2.0, strike_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDcancel) {
        painter.setPen(pul);
        const QLineF l(decobot_xstart, decobelow_ypos, decotop_xend, deco_ytopbot);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDbcancel) {
        painter.setPen(pul);
        const QLineF l(decobot_xstart, deco_ytopbot, decotop_xend, decobelow_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDxcancel) {
        painter.setPen(pul);
        const QLineF l(decobot_xstart, deco_ytopbot, decotop_xend, decobelow_ypos);
        if (l.length()>0) painter.drawLine(l);
        const QLineF l1(decobot_xstart, decobelow_ypos, decotop_xend, deco_ytopbot);
        if (l1.length()>0) painter.drawLine(l1);
        painter.setPen(pold);
    }
    return xnew;
}

bool JKQTMathTextDecoratedNode::toHtml(QString &/*html*/, JKQTMathTextEnvironment /*currentEv*/, JKQTMathTextEnvironment /*defaultEv*/) {
    //QString f;
    //JKQTMathTextEnvironment ev=currentEv;

    //bool ok=child->toHtml(html, ev, defaultEv);

    return false;
}

QString JKQTMathTextDecoratedNode::getTypeName() const
{
    return "MTdecoratedNode";
}

JKQTMathTextDecoratedNode::DecorationType JKQTMathTextDecoratedNode::getDecoration() const {
    return this->decoration;
}


