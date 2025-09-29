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



#include "jkqtmathtext/nodes/jkqtmathtextsymbolnode.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtcommon/jkqtpcodestructuring.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"
#include <cmath>
#include <QFontMetricsF>
#include <QDebug>
#include <QFontDatabase>
#include <QFontInfo>
#include <QApplication>
#include <QFont>




JKQTMathTextSymbolNode::JKQTMathTextSymbolNode(JKQTMathText* _parent, const QString& name):
    JKQTMathTextNode(_parent), symbolName(name)
{
}

JKQTMathTextSymbolNode::~JKQTMathTextSymbolNode() {

}

QString JKQTMathTextSymbolNode::getTypeName() const
{
    return QLatin1String("JKQTMathTextSymbolNode(")+symbolName+QLatin1String(")");
}


JKQTMathTextNodeSize JKQTMathTextSymbolNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const {
    return getSymbolSize(painter, currentEv).sliceToNodeSize();
}

QRectF JKQTMathTextSymbolNode::getBoundingRect(const QFont &f, const QString &text, GlobalSymbolFlags globalFlags, QPaintDevice *pd)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTMathTextSymbolNode[]::getBoundingRect()"));
#endif
    if (has(globalFlags, MakeWhitespaceHalf) && text.contains(' ')) {
        const QStringList str=text.simplified().trimmed().split(' ');
        const QRectF brSp=JKQTMathTextGetBoundingRect(f,"i",pd);
        QRectF br;
        for (int i=0; i<str.size(); i++) {
            const QRectF lbr=JKQTMathTextGetBoundingRect(f,str[i],pd);
            if (i==0) br=lbr;
            else {
                br.setWidth(br.width()+brSp.width()/2.0+lbr.width());
                if (lbr.top()>br.top()) {
                    br.setTop(lbr.top());
                }
                if (lbr.bottom()<br.bottom()) {
                    br.setBottom(lbr.bottom());
                }
            }
        }
        return br;
    } else {
        return JKQTMathTextGetBoundingRect(f,text,pd);
    }
}

QRectF JKQTMathTextSymbolNode::getTightBoundingRect(const QFont &f, const QString &text, GlobalSymbolFlags globalFlags, QPaintDevice* pd)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTMathTextSymbolNode[]::getTightBoundingRect()"));
#endif
    if (has(globalFlags, MakeWhitespaceHalf) && text.contains(' ')) {
        const QStringList str=text.simplified().trimmed().split(' ');
        const QRectF brSp=JKQTMathTextGetBoundingRect(f,"i",pd);
        QRectF br;
        for (int i=0; i<str.size(); i++) {
            const QRectF lbr=JKQTMathTextGetTightBoundingRect(f,str[i],pd);
            if (i==0) br=lbr;
            else {
                br.setWidth(br.width()+brSp.width()/2.0+lbr.width());
                if (lbr.top()>br.top()) {
                    br.setTop(lbr.top());
                }
                if (lbr.bottom()<br.bottom()) {
                    br.setBottom(lbr.bottom());
                }
            }
        }
        return br;
    } else {
        return JKQTMathTextGetTightBoundingRect(f,text,pd);
    }
}

void JKQTMathTextSymbolNode::drawText(QPainter &p, const QString &text, GlobalSymbolFlags globalFlags, SymbolFlags symflags)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTMathTextSymbolNode[]::drawText()"));
#endif
    if (has(globalFlags, MakeWhitespaceHalf) && text.contains(' ')) {
        const QStringList str=text.simplified().trimmed().split(' ');
        const QRectF brSp=JKQTMathTextGetBoundingRect(p.font(), "i", p.device());
        double x=0;
        for (int i=0; i<str.size(); i++) {
            p.drawText(QPointF(x,0), str[i]);
            x=x+JKQTMathTextGetBoundingRect(p.font(), str[i], p.device()).width()+brSp.width()/2.0;
        }
    } else {
        const QRectF tbr=JKQTMathTextGetBoundingRect(p.font(), text, p.device());
        p.save(); auto __finalpaint=JKQTPFinally([&p]() {p.restore();});
        p.translate(tbr.center());
        if (has(symflags, RotateSymbol90)) {
            //qDebug()<<"  --> RotateSymbol90";
            p.rotate(90);
        }
        if (has(symflags, FlipSymbolUpDown)) {
            //qDebug()<<"  --> FlipSymbolUpDown";
            p.scale(1,-1);
        }
        if (has(symflags, FlipSymbolLeftRight)) {
            //qDebug()<<"  --> FlipSymbolLeftRight";
            p.scale(-1,1);
        }
        p.translate(-tbr.center());
        p.drawText(QPointF(0,0), text);
    }
}


double JKQTMathTextSymbolNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTMathTextSymbolNode[]::draw()"));
#endif
    const NodeSize s=getSymbolSize(painter, currentEv);
    doDrawBoxes(painter, x, y, s);

    const auto fullProps=symbols().value(symbolName, SymbolFullProps());
    const GlobalSymbolFlags globalFlags=fullProps.globalFlags;
    const auto drawProps=fullProps.getDrawingData(currentEv, parentMathText, painter);
    const QFont f=drawProps.first;
    const QFont fnonItalic=JKQTMathTextGetNonItalic(drawProps.first);
    const JKQTMathTextSymbolNode::SymbolProps symprops=drawProps.second;
    const SymbolFlags symflags=symprops.flags;
    const QString sym=symprops.symbol;
    const QRectF tbr=getTightBoundingRect(f, sym, globalFlags, painter.device());
    const QRectF tbrNonItalic=getTightBoundingRect(fnonItalic, sym, globalFlags, painter.device());
    //const QRectF br=getBoundingRect(fm, sym, globalFlags);
    const QRectF tbrNoSymbol=JKQTMathTextGetTightBoundingRect(f, "X", painter.device());
    const double yShift=symprops.yShiftFactor*tbr.height();
    const double xShift=(s.width-tbr.width())/2.0;
    const QPointF x0(x+xShift-tbr.x(), y+yShift);
    double italic_xcorrection=fabs(tbr.width()-tbrNonItalic.width());
    if (fabs(italic_xcorrection)<1e-6) italic_xcorrection=double(JKQTMathTextGetBoundingRect(f, " ", painter.device()).width())*0.4;

    //std::cout<<"SYMB::draw(): symbolName="<<symbolName.toStdString()<<" font="<<f.family().toStdString()<<" sym="<<sym.toStdString()<<"(0x"<<std::hex<<((sym.size()==0)?uint64_t(0):uint64_t(sym[0].unicode()))<<") yShiftFactor="<<symprops.yShiftFactor<<"\n";

    if (!sym.isEmpty()) {
        // if the symbol has been recognized in the constructor: draw the symbol
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setFont(f);
        painter.setPen(currentEv.color);
        painter.translate(x0);
        drawText(painter, sym, globalFlags, symflags);
        if (has(symflags, DrawLeftHBar) || has (symflags, DrawRightHBar)) {
            //qDebug()<<"  -> DrawLeftHBar or DrawRightHBar";
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(QPen(currentEv.color, JKQTMathTextGetFontLineWidth(f, painter.device())));
            const double xh=JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).height();
            const double ybar=-xh*1.1;
            const double deltaybar=xh*0.2;
            const double barwidth=tbrNonItalic.width()/2.0;
            const double xbarstart=(has(symflags, DrawLeftHBar))?italic_xcorrection:(tbr.width()-barwidth);
            painter.drawLine(xbarstart, ybar, xbarstart+barwidth, ybar-deltaybar);
        }
        if (has(symflags, DrawVertLine)) {
            //qDebug()<<"  -> DrawVertLine";
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(QPen(currentEv.color, JKQTMathTextGetFontLineWidth(f, painter.device())));
            const double ybar=tbr.top();
            const double xbarstart=italic_xcorrection+tbrNonItalic.width()/2.0;
            const double xbarend=tbrNonItalic.width()/2.0;
            painter.drawLine(xbarstart, ybar, xbarend, ybar+tbr.height());
        }
        if (has(symflags, DrawSlash)) {
            //qDebug()<<"  -> DrawSlash";
            painter.drawText(QPointF((s.width-JKQTMathTextGetBoundingRect(f, "/", painter.device()).width())/2.0,0),"/");
        }
        if (has(symflags, DrawBackSlash)) {
            //qDebug()<<"  -> DrawBackSlash";
            painter.drawText(QPointF((s.width-JKQTMathTextGetBoundingRect(f, "\\", painter.device()).width())/2.0,0),"\\");
        }

    } else { // draw a box to indicate an unavailable symbol
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setPen(QPen(currentEv.color, JKQTMathTextGetFontLineWidth(f, painter.device())));
        painter.drawRect(QRectF(x0.x(), x0.y()-tbrNoSymbol.height(), tbrNoSymbol.width(), tbrNoSymbol.height()));
    }

    return x+s.width;
}

bool JKQTMathTextSymbolNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const {
    bool ok=true;
    const auto props=symbols().value(symbolName, SymbolFullProps());
    QString s=props.html.symbol;
    JKQTMathTextEnvironment ev=currentEv;
    ev.fontSize=ev.fontSize*props.html.fontScalingFactor;
    if (has(props.globalFlags, ExtendWidthInMathmode)) s="&thinsp;"+s+"&thinsp;";
    if (has(props.globalFlags, MakeWhitespaceHalf)) s.replace(" ", "&thinsp;");
    if (ok) html=html+ev.toHtmlStart(defaultEv, parentMathText)+s+ev.toHtmlAfter(defaultEv, parentMathText);
    return ok;
}

QString JKQTMathTextSymbolNode::getSymbolName() const {
    return this->symbolName;
}

JKQTMathTextSymbolNode::NodeSize JKQTMathTextSymbolNode::getSymbolSize(QPainter &painter, JKQTMathTextEnvironment currentEv) const
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTMathTextSymbolNode[]::getSymbolSize()"));
#endif
    NodeSize s;

    const auto fullProps=symbols().value(symbolName, SymbolFullProps());
    const GlobalSymbolFlags globalFlags=fullProps.globalFlags;
    const auto drawProps=fullProps.getDrawingData(currentEv, parentMathText, painter);
    const QFont f=drawProps.first;
    const JKQTMathTextSymbolNode::SymbolProps symprops=drawProps.second;
    const SymbolFlags symflags=symprops.flags;
    const QString sym=symprops.symbol;
    const QRectF tbr=getTightBoundingRect(f, sym, globalFlags, painter.device());
    const QRectF br=getBoundingRect(f, sym, globalFlags, painter.device());
    const QRectF tbrNoSymbol=JKQTMathTextGetTightBoundingRect(f, "X", painter.device());
    const QRectF mintbr=JKQTMathTextGetTightBoundingRect(f, "(", painter.device());
    const QRectF dottbr=JKQTMathTextGetTightBoundingRect(f, ".", painter.device());
    //const double yShift=symprops.yShiftFactor*tbr.height();

    if (currentEv.insideMath) {
        s.width=qMax(br.width(), mintbr.width());
        if (has(globalFlags, SmallExtendWidthInMathmode)) {
            if (!symprops.getSymbolSingleChar().isNull()) s.width=s.width*(1.0+(parentMathText->getMathoperatorWidthFactor()-1.0)/5.0);
            else s.width=s.width+mintbr.width()*0.15;
        } else if (has(globalFlags, ExtendWidthInMathmode)) {
            if (!symprops.getSymbolSingleChar().isNull()) s.width=s.width*parentMathText->getMathoperatorWidthFactor();
            else s.width=s.width+mintbr.width()*0.5;
        }
    } else {
        s.width=br.width();
    }
    if (!sym.isEmpty()) {
        s.overallHeight=tbr.height();
        s.baselineHeight=tbr.height()-tbr.bottom();
    } else {
        s.overallHeight=tbrNoSymbol.height();
        s.baselineHeight=tbrNoSymbol.height()-tbrNoSymbol.bottom();
        s.width=tbrNoSymbol.width();
    }
    const double oldDescent=s.overallHeight-s.baselineHeight;

    if (has(symflags, HeightIsAscent)) {
        s.baselineHeight=JKQTMathTextGetFontAscent(f, painter.device());
        s.overallHeight=s.baselineHeight+oldDescent;
    }
    if (has(symflags, RotateSymbol90)) {
        s.width=qMax(s.overallHeight, s.width);
    }
    s.strikeoutPos=JKQTMathTextGetFontStrikoutPos(f, painter.device());

    if (has(globalFlags, IntLikeSymbolCorrection)) {
        if (has(globalFlags, SubSuperscriptBelowAboveSymbol)) {
            s.subSuperAboveBelowXCorrection=parentMathText->getIntSubSuperXCorrectionFactor()*tbr.width();
        }
        s.baselineXCorrection=-parentMathText->getIntSubBesidesXCorrectionXFactor()*JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width();
        s.topXCorrection=dottbr.width();
    }
    if (has(globalFlags, SubscriptCorrection) && sym.size()>0) {
        s.baselineXCorrection=JKQTMathTextGetRightBearing(f,sym[sym.size()-1],painter.device());
    }

    return s;
}

bool JKQTMathTextSymbolNode::hasSymbol(const QString &symbolName)
{
    return symbols().contains(symbolName);
}

QStringList JKQTMathTextSymbolNode::getSymbols()
{
    return symbols().keys();
}

bool JKQTMathTextSymbolNode::isSubSuperscriptBelowAboveSymbol(const QString &symbolName)
{
    if (symbols().contains(symbolName)) {
        return has(symbols()[symbolName].globalFlags, SubSuperscriptBelowAboveSymbol);
    }
    return false;
}

bool JKQTMathTextSymbolNode::isExtendedWidthSymbol(const QString &symbolName)
{
    if (symbols().contains(symbolName)) {
        return has(symbols()[symbolName].globalFlags, ExtendWidthInMathmode) ||  has(symbols()[symbolName].globalFlags, SmallExtendWidthInMathmode);
    }
    return false;
}

int JKQTMathTextSymbolNode::getSymbolLength(const QString &symbolName)
{
    if (symbols().contains(symbolName)) {
        return symbols()[symbolName].props.value(MTFEUnicode, symbols()[symbolName].props.value(MTFEStandard, SymbolProps())).symbol.size();
    }
    return 0;
}



JKQTMathTextSymbolNode::SymbolProps::SymbolProps():
    SymbolProps(QString())
{

}

JKQTMathTextSymbolNode::SymbolProps::SymbolProps(const QString &_symbol, SymbolFlags _flags, double _fontScalingFactor, double _yShiftFactor):
    symbol(_symbol),
    flags(_flags),
    fontScalingFactor(_fontScalingFactor),
    yShiftFactor(_yShiftFactor)
{
}


JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::MathOperatorSymbol(const QString &op)
{
    return MathOperatorSymbol(op, op);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::MathOperatorSymbol(const QString &op, const QString &ophtml)
{
    return SymbolFullProps(SymbolProps(op, ItalicOff|BoldOff, 1.0, 0.0)).addGlobalFlags(ExtendWidthInMathmode).addHtml(ophtml, ItalicOff|BoldOff, 1.0, 0.0);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::MathOperatorText(const QString &op)
{
    return MathOperatorText(op, op);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::MathOperatorText(const QString &op, const QString &ophtml)
{
    return SymbolFullProps(SymbolProps(op, ItalicOff|BoldOff|HeightIsAscent, 1.0, 0.0)).addGlobalFlags(ExtendWidthInMathmode|MakeWhitespaceHalf).addHtml(ophtml, ItalicOff|BoldOff|HeightIsAscent, 1.0, 0.0);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::NarrowMathOperatorText(const QString &op)
{
    return NarrowMathOperatorText(op, op);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::NarrowMathOperatorText(const QString &op, const QString &ophtml)
{
    return SymbolFullProps(SymbolProps(op, ItalicOff|BoldOff|HeightIsAscent, 1.0, 0.0)).addGlobalFlags(SmallExtendWidthInMathmode|MakeWhitespaceHalf).addHtml(ophtml, ItalicOff|BoldOff|HeightIsAscent, 1.0, 0.0);
}

const QHash<QString, JKQTMathTextSymbolNode::SymbolFullProps> &JKQTMathTextSymbolNode::symbols()
{
    static QHash<QString, JKQTMathTextSymbolNode::SymbolFullProps> s_symbols=[](){
        QHash<QString, JKQTMathTextSymbolNode::SymbolFullProps> symbols;

        /**************************************************************************************
         * STANDARD Symbols available in all standard fonts
         **************************************************************************************/
        symbols["#"]=SimpleTextSymbol("#", "&num;");
        symbols["%"]=SimpleTextSymbol("%", "&percent;");
        symbols["&"]=SimpleTextSymbol("&", "&amp;");
        symbols["("]=SimpleUprightTextSymbol("(");
        symbols[")"]=SimpleUprightTextSymbol(")");
        symbols["["]=SimpleUprightTextSymbol("[");
        symbols["]"]=SimpleUprightTextSymbol("]");
        symbols["_"]=SimpleTextSymbol("_");
        symbols["{"]=SimpleUprightTextSymbol("{");
        symbols["|"]=SimpleUprightTextSymbol("||", "&Vert;").addUprightUnicode(QChar(0x2016));
        symbols["}"]=SimpleUprightTextSymbol("}");
        symbols["AC"]=UprightSymbolUnicode(QChar(0x223F)).addUprightHtml("&acd;").addUprightStd("~");
        symbols["circonflex"]=SimpleTextSymbol("^");
        symbols["aa"]=SimpleTextSymbol(QChar(0xE5));
        symbols["ae"]=SimpleTextSymbol(QChar(0xE6));
        symbols["AE"]=SimpleTextSymbol(QChar(0xC6));
        symbols["AA"]=SimpleTextSymbol(QChar(0xC5));
        symbols["oe"]=UnicodeSymbol(QChar(0x153)).addStd("oe").addHtml("&oelig;");
        symbols["OE"]=UnicodeSymbol(QChar(0x152)).addStd("OE").addHtml("&OElig;");
        symbols["ss"]=SimpleTextSymbol(QChar(0xDF)).addHtml("&szlig;");
        symbols["l"]=UnicodeSymbol(QChar(0x141)).addHtml("&lstrok;");
        symbols["L"]=UnicodeSymbol(QChar(0x142)).addHtml("&Lstrok;");
        symbols["o"]=SimpleTextSymbol(QChar(0xF8)).addHtml("&oslash;");
        symbols["O"]=SimpleTextSymbol(QChar(0xD8)).addHtml("&Oslash;");
        { auto s=UprightSymbolUnicode(QChar(0x212B)).addUprightStd(QChar(0xC5));
          symbols["Angstrom"]=s; symbols["angstrom"]=s; }
        { auto s=UnicodeSymbol(QChar(0x2136)).addHtml("&beth;");
          symbols["Beth"]=s; symbols["Bet"]=s; symbols["beth"]=s; symbols["bet"]=s; }
        symbols["Box"]=UprightSymbolUnicode(QChar(0x25A1));
        symbols["DC"]=UnicodeSymbol(QChar(0x2393)).addWinSymbol(QChar(0xBB)).addStd("=");
        symbols["EUR"]=UnicodeSymbol(QChar(0x20AC));
        symbols["Im"]=UprightSymbolUnicode(QChar(0x2111)).addHtml("&image;").addWinSymbol(QChar(0xC1));
        symbols["No"]=UnicodeSymbol(QChar(0x2116));
        { auto s=UprightSymbolUnicode(QChar(0x2126)).addUprightWinSymbol("W").addUprightHtml("&Omega;");
          symbols["Ohm"]=s; symbols["ohm"]=s; }
        symbols["P"]=SimpleTextSymbol(QChar(0xB6)).addHtml("&para;");
        symbols["Re"]=UnicodeSymbol(QChar(0x211C)).addHtml("&Rfr;").addWinSymbol(QChar(0xC2));
        { auto s=SimpleTextSymbol(QChar(0xA7)).addHtml("&sect;");
            symbols["S"]=s; symbols["§"]=s;}
        symbols["accurrent"]=UnicodeSymbol(QChar(0x23E6));
        { auto s=UnicodeSymbol(QChar(0x2135)).addHtml("&aleph;").addWinSymbol(QChar(0xC0));
            symbols["alef"]=s; symbols["aleph"]=s; symbols["Aleph"]=s; symbols["Alef"]=s; }
        symbols["angle"]=UprightSymbolUnicode(QChar(0x2220)).addHtml("&angle;").addWinSymbol(QChar(0xD0));
        symbols["backslash"]=SimpleTextSymbol("\\");
        symbols["benzene"]=UprightSymbolUnicode(QChar(0x232C));
        symbols["benzenr"]=UprightSymbolUnicode(QChar(0x23E3));
        symbols["blacksquare"]=UprightSymbolUnicode(QChar(0x220E)).addUprightHtml("&squ;");
        { auto s=UprightSymbolUnicode(QChar(0x231E));
            symbols["blcorner"]=s; symbols["llcorner"]=s; }
        { auto s=UprightSymbolUnicode(QChar(0x231F));
            symbols["brcorner"]=s; symbols["lrcorner"]=s; }
        { auto s=UprightSymbolUnicode(QChar(0x2022)).addUprightHtml("&bull;").addUprightWinSymbol(QChar(0xB7));
            symbols["bullet"]=s; symbols["textbullet"]=s; }
        symbols["cdots"]=UprightSymbolUnicode(QChar(0x22EF)).addUprightHtml("&middot;&middot;&middot;").addUprightStd(QString(3, QChar(0xB7)));
        { auto s=UnicodeSymbol(QChar(0x2103)).addUprightStd("°C").addUprightHtml("&deg;C");
            symbols["celsius"]=s; symbols["degC"]=s; symbols["degreeCelsius"]=s; }
        symbols["ell"]=UprightSymbolUnicode(QChar(0x2113), "&ell;");
        symbols["wp"]=UprightSymbolUnicode(QChar(0x2118), "&wp;").addGlobalFlags(SubscriptCorrection);
        symbols["mho"]=UprightSymbolUnicode(QChar(0x2127), "&mho;");
        symbols["lozenge"]=UprightSymbolUnicode(QChar(0x25CA), "&loz;");
        symbols["cent"]=SimpleTextSymbol(QChar(0xA2), "&cent;");
        symbols["checkmark"]=UprightSymbolStd(QChar(0x2713)).addUprightHtml("&check;");
        symbols["circ"]=UprightSymbolStd(QChar(0x2218)).addUprightHtml("&SmallCircle;").addStd("o", ItalicOff,0.7, -0.25);
        symbols["co"]=UprightSymbolUnicode(QChar(0x2105));
        { auto s=SimpleTextSymbol(QChar(0xA9), "&copy;");
            symbols["copyright"]=s; symbols["textcopyright"]=s; symbols["circledC"]=s; }
        { auto s=UprightSymbolUnicode(QChar(0x2020)).addUprightHtml("&dagger;");
            symbols["dagger"]=s; symbols["dag"]=s; symbols["textdagger"]=s; }
        { auto s=UprightSymbolUnicode(QChar(0x2021)).addUprightHtml("&ddagger;");
            symbols["ddagger"]=s; symbols["ddag"]=s; symbols["textdaggerdbl"]=s; }
        { auto s=UnicodeSymbol(QChar(0x2138)).addHtml("&daleth;");
            symbols["dalet"]=s; symbols["Dalet"]=s; symbols["daleth"]=s; symbols["Daleth"]=s; }
        symbols["dd"] = SymbolFullProps(SymbolProps("d", Upright|BoldOff), "d", Upright|BoldOff);
        symbols["diamond"]=UprightSymbolUnicode(QChar(0x22C4)).addHtml("&diam;").addWinSymbol(QChar(0xE0));
        symbols["dollar"]=UnicodeSymbol(QChar(0x0024));
        { auto s=SimpleTextSymbol(QChar(0x24), "&dollar;");
            symbols["dollar"]=s; symbols["$"]=s; }
        symbols["dprime"]=UnicodeSymbol(QChar(0x2033)).addHtml("&Prime;").addStd("''");
        symbols["complement"] = SymbolFullProps(SymbolProps("C", Upright|BoldOff), "C", Upright|BoldOff).addUnicode(QChar(0x2201), Upright|BoldOff);
        symbols["ee"] = SymbolFullProps(SymbolProps("e", Upright|BoldOff), "e", Upright|BoldOff);
        { auto s=UnicodeSymbol(QChar(0x2026)).addHtml("&mldr;").addWinSymbol(QChar(0xBC)).addStd("...");
            symbols["ellipsis"]=s; symbols["dots"]=s; symbols["ldots"]=s; }
        { auto s=SimpleTextSymbol(QChar(0x20AC), "&euro;");
            symbols["euro"]=s; symbols["EUR"]=s; }
        { auto s=UnicodeSymbol(QChar(0x2109));
            symbols["fahrenheit"]=s; symbols["degF"]=s; }
        symbols["female"]=UnicodeSymbol(QChar(0x2640)).addHtml("&female;");
        symbols["flq"]=UnicodeSymbol(QChar(0x2039)).addHtml("&lsaquo;").addStd("<");
        symbols["flqq"]=UnicodeSymbol(QChar(0x00AB)).addHtml("&laquo;").addStd(QChar(0xAB));
        //symbols["frown"]=UnicodeSymbol(QChar(0x2322)).addHtml("&frown;");
        symbols["frq"]=UnicodeSymbol(QChar(0x203A)).addHtml("&rsaquo;").addStd(">");
        symbols["frqq"]=UnicodeSymbol(QChar(0x00BB)).addHtml("&raquo;").addStd(QChar(0xBB));
        { auto s=UnicodeSymbol(QChar(0x2137)).addHtml("&gimel;");
            symbols["gimel"]=s; symbols["Gimel"]=s; }
        symbols["glq"]=UnicodeSymbol(QChar(0x2018)).addHtml("&OpenCurlyQuote;").addStd("'");
        symbols["glqq"]=UnicodeSymbol(QChar(0x201C)).addHtml("&OpenCurlyDoubleQuote;").addStd("\"");
        symbols["grq"]=UnicodeSymbol(QChar(0x2019)).addHtml("&CloseCurlyQuote;").addStd("'");
        symbols["grqq"]=UnicodeSymbol(QChar(0x201D)).addHtml("&CloseCurlyDoubleQuote;").addStd("\"");
        symbols["hbar"]=UprightSymbolUnicode(QChar(0x210F)).addUprightHtml("&hbar;").addStd("h", ItalicOn|DrawLeftHBar).addGlobalFlags(SubscriptCorrection);
        symbols["hexagon"]=UprightSymbolUnicode(QChar(0x2394));
        symbols["ii"] = SymbolFullProps(SymbolProps("i", Upright|BoldOff), "i", Upright|BoldOff);
        symbols["infty"]=UprightSymbolUnicode(QChar(0x221E)).addUprightHtml("&infin;").addUprightWinSymbol(QChar(0xA5)).addUprightStd("8", RotateSymbol90);
        symbols["langle"]=UprightSymbolUnicode(QChar(0x2329)).addWinSymbol(QChar(0xE1));
        symbols["lceil"]=UprightSymbolUnicode(QChar(0x2308)).addUprightHtml("&LeftCeiling;").addUprightWinSymbol(QChar(0xE9));
        symbols["lfloor"]=UprightSymbolUnicode(QChar(0x230A)).addUprightHtml("&LeftFloor;").addUprightWinSymbol(QChar(0xEB));
        symbols["lightning"]=UnicodeSymbol(QChar(0x21AF));
        symbols["male"]=UnicodeSymbol(QChar(0x2642)).addHtml("&male;");
        symbols["measuredangle"]=UprightSymbolUnicode(QChar(0x2221)).addUprightHtml("&measuredangle;");
        symbols["micro"] = SimpleTextSymbol(QChar(0xB5), "&micro;");
        symbols["ohm"]=UprightSymbolUnicode(QChar(0x2126)).addUprightHtml("&Omega;").addUprightWinSymbol(QChar(0x57));
        symbols["partial"]=UprightSymbolUnicode(QChar(0x2202)).addUprightHtml("&part;").addUprightWinSymbol(QChar(0xB6));
        { auto s=UnicodeSymbol(QChar(0x2030)).addHtml("&permil;");
            symbols["perthousand"]=s; symbols["permil"]=s; }
        symbols["pound"]=SimpleTextSymbol(QChar(0xA3), "&pound;");
        symbols["pound"]=UnicodeSymbol(QChar(0x00A3));
        symbols["prime"]=UnicodeSymbol(QChar(0x2032)).addHtml("&prime;").addStd("'");
        symbols["arcminute"]=UnicodeSymbol(QChar(0x2032)).addHtml("&prime;").addStd("'");
        symbols["arcsecond"]=UnicodeSymbol(QChar(0x2033)).addHtml("&dprime;").addStd("'");
        symbols["rangle"]=UprightSymbolUnicode(QChar(0x232A)).addUprightWinSymbol(QChar(0xF1));
        symbols["rceil"]=UprightSymbolUnicode(QChar(0x2309)).addUprightHtml("&RightCeiling;").addUprightWinSymbol(QChar(0xF9));
        { auto s=SimpleTextSymbol(QChar(0xAE), "&reg;");
            symbols["registered"]=s; symbols["textregistered"]=s; symbols["circledR"]=s; }
        symbols["rfloor"]=UprightSymbolUnicode(QChar(0x230B)).addUprightHtml("&RightFloor;").addUprightWinSymbol(QChar(0xFB));
        symbols["rightangle"]=UprightSymbolUnicode(QChar(0x221F)).addUprightHtml("&angrt;");
        //symbols["smile"]=UprightSymbolUnicode(QChar(0x2323)).addUprightHtml("&smile;");
        symbols["sphericalangle"]=UprightSymbolUnicode(QChar(0x2222)).addUprightHtml("&angsph;");
        symbols["star"]=UprightSymbolUnicode(QChar(0x22C6));
        symbols["tcohm"]=UnicodeSymbol(QChar(0x2126));
        { auto s=UnicodeSymbol(QChar(0x2014), "&#11840;");
            symbols["dblhyphen"]=s; symbols["textdblhyphen"]=s; symbols["textdblhyphenchar"]=s; }
        { auto s=UnicodeSymbol(QChar(0x2014), "&mdash;");
            symbols["---"]=s; symbols["textemdash"]=s; symbols["emdash"]=s; }
        { auto s=UnicodeSymbol(QChar(0x2013), "&ndash;");
            symbols["--"]=s; symbols["textendash"]=s; symbols["endash"]=s; }
        { auto s=SimpleTextSymbol("-");
            symbols["texthyphen"]=s; symbols["hyphen"]=s; }
        symbols["textbar"]=SimpleTextSymbol("|", "&VerticalLine;");
        { auto s=SimpleTextSymbol(QChar(0xB0), "&deg;");
            symbols["textdegree"]=s; symbols["degree"] = s; }
        symbols["textgreater"]=SimpleTextSymbol(">", "&gt;");
        symbols["textless"]=SimpleTextSymbol("<", "&lt;");
        symbols["textquestiondown"]=SimpleTextSymbol(QChar(0xBF), "&iquest;");
        symbols["textexclamdown"]=SimpleTextSymbol(QChar(0xA1), "&iexcl;");
        { auto s=UnicodeSymbol(QChar(0x231C));
            symbols["tlcorner"]=s; symbols["ulcorner"]=s; }
        symbols["trademark"]=UnicodeSymbol(QChar(0x2122)).addHtml("&trade;").addWinSymbol(QChar(0xD4)).addStd("(TM)");
        symbols["trapezium"]=UnicodeSymbol(QChar(0x23E2)).addHtml("&trpezium;");
        { auto s=UprightSymbolUnicode(QChar(0x231D));
            symbols["trcorner"]=s; symbols["urcorner"]=s; }
        symbols["trprime"]=UnicodeSymbol(QChar(0x2034)).addHtml("&tprime;").addStd("'''");
        symbols["varcarriagereturn"]=UnicodeSymbol(QChar(0x23CE)).addWinSymbol(QChar(0xBF));
        symbols["varhexagonlrbonds"]=UnicodeSymbol(QChar(0x232C));
        symbols["yen"]=SimpleTextSymbol(QChar(0xA5), "&yen;");





        /**************************************************************************************
         * STANDARD MathOperator Strings
         **************************************************************************************/
        symbols["Pr"] = NarrowMathOperatorText("Pr");
        symbols["acos"] = NarrowMathOperatorText("acos");
        symbols["arccos"] = NarrowMathOperatorText("arccos");
        symbols["arcsin"] = NarrowMathOperatorText("arcsin");
        symbols["arctan"] = NarrowMathOperatorText("arctan");
        symbols["arg"] = NarrowMathOperatorText("arg").addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        symbols["argmax"] = NarrowMathOperatorText("arg max", "arg&thinsp;max").addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        symbols["argmin"] = NarrowMathOperatorText("arg min", "arg&thinsp;min").addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        symbols["asin"] = NarrowMathOperatorText("asin");
        symbols["atan"] = NarrowMathOperatorText("atan");
        symbols["cos"] = NarrowMathOperatorText("cos");
        symbols["cosh"] = NarrowMathOperatorText("cosh");
        symbols["cot"] = NarrowMathOperatorText("cot");
        symbols["coth"] = NarrowMathOperatorText("coth");
        symbols["coth"] = NarrowMathOperatorText("coth");
        symbols["deg"] = NarrowMathOperatorText("deg");
        symbols["det"] = NarrowMathOperatorText("det");
        symbols["dim"] = NarrowMathOperatorText("dim");
        symbols["exp"] = NarrowMathOperatorText("exp");
        symbols["gcd"] = NarrowMathOperatorText("gcd");
        symbols["hom"] = NarrowMathOperatorText("hom");
        symbols["ker"] = NarrowMathOperatorText("ker");
        symbols["lb"] = NarrowMathOperatorText("lb");
        symbols["ld"] = NarrowMathOperatorText("ld");
        symbols["lim"] = NarrowMathOperatorText("lim").addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        symbols["liminf"] = NarrowMathOperatorText("lim inf", "lim&thinsp;inf").addGlobalFlags(SubSuperscriptBelowAboveSymbol).addGlobalFlags(SubscriptCorrection);
        symbols["limsup"] = NarrowMathOperatorText("lim sup", "lim&thinsp;sup").addGlobalFlags(SubSuperscriptBelowAboveSymbol).addGlobalFlags(SubscriptCorrection);
        symbols["ln"] = NarrowMathOperatorText("ln");
        symbols["log"] = NarrowMathOperatorText("log");
        symbols["max"] = NarrowMathOperatorText("max");
        symbols["median"] = NarrowMathOperatorText("median");
        symbols["min"] = NarrowMathOperatorText("min");
        symbols["mod"] = NarrowMathOperatorText("mod");
        symbols["sec"] = NarrowMathOperatorText("sec");
        symbols["sgn"] = NarrowMathOperatorText("sgn");
        symbols["sign"] = NarrowMathOperatorText("sign");
        symbols["sin"] = NarrowMathOperatorText("sin");
        symbols["sinh"] = NarrowMathOperatorText("sinh");
        symbols["tan"] = NarrowMathOperatorText("tan");
        symbols["tanh"] = NarrowMathOperatorText("tanh");

        /**************************************************************************************
         * STANDARD MathOperator Symbols
         **************************************************************************************/
        { auto s=MathOperatorSymbolUnicode(QChar(0x2217)).addMathOperatorStd("*").addMathOperatorHtml("*");
            symbols["*"]=s; symbols["ast"]=s; symbols["asterisk"]=s; }
        symbols["/"]=NarrowMathOperatorSymbolStd("/");
        symbols["+"]=MathOperatorSymbolUnicode(QChar(0x2B)).addMathOperatorHtml("+").addMathOperatorStd("+");
        symbols["-"]=MathOperatorSymbolUnicode(QChar(0x2212)).addMathOperatorHtml("-").addMathOperatorStd("-");
        symbols["<"]=MathOperatorSymbol("<", "&lt;");
        symbols["="]=MathOperatorSymbol("=");
        symbols[">"]=MathOperatorSymbol(">", "&gt;");
        symbols["Downarrow"]=UprightSymbolUnicode(QChar(0x21D3)).addUprightHtml("&DoubleDownArrow;").addUprightWinSymbol(QChar(0xDF));
        symbols["Leftarrow"]=UprightSymbolUnicode(QChar(0x21D0)).addUprightHtml("&DoubleLeftArrow;").addUprightWinSymbol(QChar(0xDC));
        symbols["Rightarrow"]=UprightSymbolUnicode(QChar(0x21D2)).addUprightHtml("&Rightarrow;").addUprightWinSymbol(QChar(0xDE));
        symbols["Uparrow"]=UprightSymbolUnicode(QChar(0x21D1)).addUprightHtml("&DoubleUpArrow;").addUprightWinSymbol(QChar(0xDD));
        symbols["Updownarrow"]=UprightSymbolUnicode(QChar(0x21D5)).addUprightHtml("&Updownarrow;");
        symbols["approx"]=MathOperatorSymbolUnicode(QChar(0x2248)).addMathOperatorHtml("&asymp;").addMathOperatorWinSymbol(QChar(0xBB));
        symbols["bbC"]=UnicodeSymbol(QChar(0x2102));
        symbols["bbH"]=UnicodeSymbol(QChar(0x210D));
        symbols["bbN"]=UnicodeSymbol(QChar(0x2115));
        symbols["bbP"]=UnicodeSymbol(QChar(0x2119));
        symbols["bbQ"]=UnicodeSymbol(QChar(0x211A));
        symbols["bbR"]=UnicodeSymbol(QChar(0x211D));
        symbols["bbZ"]=UnicodeSymbol(QChar(0x2124));
        symbols["because"]=MathOperatorSymbolUnicode(QChar(0x2235)).addMathOperatorHtml("&because;");
        symbols["bigcap"]=NarrowMathOperatorSymbolUnicode(QChar(0x22C2)).addMathOperatorHtml("&Intersection;").addMathOperatorWinSymbol(QChar(0xC7), 1.8).addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        symbols["bigcup"]=NarrowMathOperatorSymbolUnicode(QChar(0x22C3)).addMathOperatorHtml("&xcup;").addMathOperatorWinSymbol(QChar(0xC8), 1.8).addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        { auto s=NarrowMathOperatorSymbolUnicode(QChar(0x22C0)).addMathOperatorHtml("&Wedge;").addMathOperatorWinSymbol(QChar(0xD9), 1.8).addGlobalFlags(SubSuperscriptBelowAboveSymbol);
            symbols["bigwedge"]=s; symbols["bighat"]=s; }
        symbols["bigvee"]=NarrowMathOperatorSymbolUnicode(QChar(0x22C1)).addMathOperatorHtml("&xvee;").addMathOperatorWinSymbol(QChar(0xDA), 1.8).addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        symbols["bigotimes"]=NarrowMathOperatorSymbolUnicode(QChar(0x2A02)).addMathOperatorHtml("&bigotimes;").addMathOperatorWinSymbol(QChar(0xC4), 1.8).addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        symbols["bigoplus"]=NarrowMathOperatorSymbolUnicode(QChar(0x2A01)).addMathOperatorHtml("&bigoplus;").addMathOperatorWinSymbol(QChar(0xC5), 1.8).addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        symbols["bigodot"]=NarrowMathOperatorSymbolUnicode(QChar(0x2A00)).addMathOperatorHtml("&bigodot;").addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        symbols["biguplus"]=NarrowMathOperatorSymbolUnicode(QChar(0x2A04)).addMathOperatorHtml("&biguplus;").addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        symbols["bigsqcup"]=NarrowMathOperatorSymbolUnicode(QChar(0x2A06)).addMathOperatorHtml("&bigsqcup;").addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        { auto s=MathOperatorSymbolUnicode(QChar(0x22A5)).addMathOperatorHtml("&UpTee;");
            symbols["bot"]=s; symbols["perp"]=s; }
        { auto s=MathOperatorSymbolUnicode(QChar(0x2229)).addMathOperatorHtml("&cap;").addMathOperatorWinSymbol(QChar(0xC7));
            symbols["cap"]=s; symbols["land"]=s; }
        symbols["cdot"]=MathOperatorSymbol(QChar(0xB7)).addMathOperatorHtml("&middot;").addMathOperatorWinSymbol(QChar(0xD7));
        symbols["cong"]=MathOperatorSymbolUnicode(QChar(0x2245)).addMathOperatorHtml("&TildeFullEqual;");
        symbols["coprod"]=NarrowMathOperatorSymbolUnicode(QChar(0x2210)).addMathOperatorHtml("&Coproduct;").addWinSymbol(QChar(0xD5), ItalicOff|BoldOff|FlipSymbolUpDown, 1.8, 0.1).addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        { auto s=MathOperatorSymbolUnicode(QChar(0x222A)).addMathOperatorHtml("&cup;").addMathOperatorWinSymbol(QChar(0xC8));
            symbols["cup"]=s; symbols["lor"]=s; }
        symbols["ddots"]=UprightSymbolUnicode(QChar(0x22F1)).addMathOperatorHtml("&dtdot;");
        symbols["div"]=MathOperatorSymbolUnicode(QChar(0x00F7)).addMathOperatorHtml("&div;").addMathOperatorWinSymbol(QChar(0xB8));
        symbols["downarrow"]=UprightSymbolUnicode(QChar(0x2193)).addUprightHtml("&ShortDownArrow;").addUprightWinSymbol(QChar(0xAF));
        symbols["downharpoonleft"]=UprightSymbolUnicode(QChar(0x21C3)).addUprightHtml("&LeftDownVector;");
        symbols["downharpoonright"]=UprightSymbolUnicode(QChar(0x21C2)).addUprightHtml("&RightDownVector;");
        { auto s=MathOperatorSymbolUnicode(QChar(0x2205)).addMathOperatorHtml("&varnothing;").addMathOperatorWinSymbol(QChar(0xC6)).addStd("0", BoldOff|ItalicOff|DrawSlash);
            symbols["emptyset"]=s; symbols["varnothing"]=s; }
        symbols["equiv"]=MathOperatorSymbolUnicode(QChar(0x2261)).addMathOperatorHtml("&equiv;").addMathOperatorWinSymbol(QChar(0xBA));
        symbols["exists"]=NarrowMathOperatorSymbolUnicode(QChar(0x2203)).addMathOperatorHtml("&exist;").addMathOperatorWinSymbol(QChar(0x24)).addStd("E", ItalicOff|BoldOff|FlipSymbolLeftRight);
        symbols["forall"]=NarrowMathOperatorSymbolUnicode(QChar(0x2200)).addMathOperatorHtml("&forall;").addMathOperatorWinSymbol(QChar(0x22)).addStd("A", ItalicOff|BoldOff|FlipSymbolUpDown).addGlobalFlags(SubscriptCorrection);
        { auto s=MathOperatorSymbolUnicode(QChar(0x2265)).addMathOperatorHtml("&GreaterEqual;").addMathOperatorWinSymbol(QChar(0xB3));
            symbols["geq"]=s; symbols["ge"]=s; }
        symbols["geqq"]=MathOperatorSymbolUnicode(QChar(0x2267)).addMathOperatorHtml("&GreaterFullEqual;");
        symbols["gg"]=MathOperatorSymbolUnicode(QChar(0x226B)).addMathOperatorHtml("&NestedGreaterGreater;").addMathOperatorStd(">>");
        symbols["iddots"]=UprightSymbolUnicode(QChar(0x22F0)).addMathOperatorHtml("&utdot;");
        { auto s=UprightSymbolUnicode(QChar(0x21D4)).addUprightHtml("&DoubleLeftRightArrow;").addUprightWinSymbol(QChar(0xDB));
            symbols["iff"]=s; symbols["Leftrightarrow"]=s; }
        symbols["iiint"]=NarrowMathOperatorSymbolUnicode(QChar(0x222D)).addGlobalFlags(IntLikeSymbolCorrection | SubSuperscriptBelowAboveSymbol).addMathOperatorHtml("&tint;").addMathOperatorWinSymbol(QString(3, QChar(0xF2)), 1.8, 0.1);
        symbols["iint"]=NarrowMathOperatorSymbolUnicode(QChar(0x222C)).addGlobalFlags(IntLikeSymbolCorrection | SubSuperscriptBelowAboveSymbol).addMathOperatorHtml("&Int;").addMathOperatorWinSymbol(QString(2, QChar(0xF2)), 1.8, 0.1);
        symbols["in"]=MathOperatorSymbolUnicode(QChar(0x2208)).addMathOperatorHtml("&isin;").addMathOperatorWinSymbol(QChar(0xCE));
        symbols["int"]=NarrowMathOperatorSymbolUnicode(QChar(0x222B)).addGlobalFlags(IntLikeSymbolCorrection | SubSuperscriptBelowAboveSymbol).addMathOperatorHtml("&int;").addMathOperatorWinSymbol(QChar(0xF2), 1.8, 0.1);
        symbols["leftarrow"]=UprightSymbolUnicode(QChar(0x2190)).addUprightHtml("&larr;").addUprightWinSymbol(QChar(0xAC));
        symbols["longleftarrow"]=UprightSymbolUnicode(QChar(0x27F5)).addUprightHtml("&longleftarrow;");
        symbols["longrightarrow"]=UprightSymbolUnicode(QChar(0x27F6)).addUprightHtml("&longrightarrow;");
        symbols["longleftrightarrow"]=UprightSymbolUnicode(QChar(0x27F7)).addUprightHtml("&longleftrightarrow;");
        symbols["Longleftarrow"]=UprightSymbolUnicode(QChar(0x27F8)).addUprightHtml("&DoubleLongLeftArrow;");
        symbols["Longrightarrow"]=UprightSymbolUnicode(QChar(0x27F9)).addUprightHtml("&DoubleLongRightArrow;");
        symbols["Longleftrightarrow"]=UprightSymbolUnicode(QChar(0x27FA)).addUprightHtml("&Longleftrightarrow;");
        symbols["leftharpoondown"]=UprightSymbolUnicode(QChar(0x21BD)).addUprightHtml("&leftharpoondown;");
        symbols["leftharpoonup"]=UprightSymbolUnicode(QChar(0x21BC)).addUprightHtml("&LeftVector;");
        symbols["leftrightarrow"]=UprightSymbolUnicode(QChar(0x2194)).addUprightHtml("&harr;").addUprightWinSymbol(QChar(0xAB));
        symbols["leftrightharpoon"]=UprightSymbolUnicode(QChar(0x21CB)).addUprightHtml("&ReverseEquilibrium;");
        { auto s=MathOperatorSymbolUnicode(QChar(0x2264)).addMathOperatorHtml("&leq;").addMathOperatorWinSymbol(QChar(0xA3));
            symbols["leq"]=s; symbols["le"]=s; }
        symbols["leqq"]=MathOperatorSymbolUnicode(QChar(0x2266)).addMathOperatorHtml("&LessFullEqual;");
        symbols["ll"]=MathOperatorSymbolUnicode(QChar(0x226A)).addMathOperatorHtml("&NestedLessLess;").addMathOperatorStd("<<");
        symbols["lnot"]=MathOperatorSymbolUnicode(QChar(0xAC)).addMathOperatorWinSymbol(QChar(0xD8)).addMathOperatorHtml("&not;");
        symbols["mapimage"]=MathOperatorSymbolUnicode(QChar(0x22B7)).addMathOperatorHtml("&imof;");
        symbols["maporiginal"]=MathOperatorSymbolUnicode(QChar(0x22B6)).addMathOperatorHtml("&origof;");
        symbols["mapsto"]=MathOperatorSymbolUnicode(QChar(0x21A6)).addMathOperatorHtml("&RightTeeArrow;");
        symbols["mid"]=MathOperatorSymbolUnicode(QChar(0x2223)).addMathOperatorHtml("&VerticalBar;").addMathOperatorWinSymbol(QChar(0xBD)).addMathOperatorStd("|");
        symbols["mp"]=MathOperatorSymbolUnicode(QChar(0x2213)).addMathOperatorHtml("&mp;").addWinSymbol(QChar(0xB1),ItalicOff|BoldOff|FlipSymbolUpDown).addStd(QChar(0xB1),ItalicOff|BoldOff|FlipSymbolUpDown);
        symbols["multimap"]=MathOperatorSymbolUnicode(QChar(0x22B8)).addMathOperatorHtml("&mumap;");
        symbols["nabla"]=NarrowMathOperatorSymbolUnicode(QChar(0x2207)).addMathOperatorHtml("&nabla;").addMathOperatorWinSymbol(QChar(0xD1)).addGlobalFlags(IntLikeSymbolCorrection);
        symbols["ne"]=NarrowMathOperatorSymbolUnicode(QChar(0x2260)).addMathOperatorHtml("&ne;").addMathOperatorWinSymbol(QChar(0xB9));
        symbols["nearrow"]=UprightSymbolUnicode(QChar(0x2197)).addUprightHtml("&UpperRightArrow;");
        { auto s=MathOperatorSymbolUnicode(QChar(0x00AC)).addMathOperatorHtml("&not;").addMathOperatorWinSymbol(QChar(0xD8));
            symbols["neg"]=s; symbols["lnot"]=s; }
        symbols["neq"]=MathOperatorSymbolUnicode(QChar(0x2260)).addMathOperatorHtml("&ne;").addMathOperatorWinSymbol(QChar(0xB9)).addStd("=", ItalicOff|BoldOff|DrawSlash);
        symbols["nexists"]=NarrowMathOperatorSymbolUnicode(QChar(0x2204)).addMathOperatorHtml("&NotExists;").addStd("E", ItalicOff|BoldOff|FlipSymbolLeftRight|DrawSlash).addMathOperatorWinSymbol(QChar(0x24), ItalicOff|BoldOff|DrawSlash);
        symbols["ni"]=NarrowMathOperatorSymbolUnicode(QChar(0x220B)).addMathOperatorHtml("&ReverseElement;").addMathOperatorWinSymbol(QChar(0xCE), ItalicOff|BoldOff|FlipSymbolLeftRight);
        symbols["nmid"]=NarrowMathOperatorSymbolUnicode(QChar(0x2224)).addMathOperatorHtml("&NotVerticalBar;");
        symbols["notin"]=NarrowMathOperatorSymbolUnicode(QChar(0x2209)).addMathOperatorHtml("&notin;").addMathOperatorWinSymbol(QChar(0xCF));
        symbols["notni"]=NarrowMathOperatorSymbolUnicode(QChar(0x220C)).addMathOperatorHtml("&NotReverseElement;");
        symbols["nparallel"]=MathOperatorSymbolUnicode(QChar(0x2226)).addMathOperatorHtml("&NotDoubleVerticalBar;");
        symbols["nwarrow"]=UprightSymbolUnicode(QChar(0x2196)).addUprightHtml("&UpperLeftArrow;");
        symbols["odot"]=MathOperatorSymbolUnicode(QChar(0x2299)).addMathOperatorHtml("&odot;");
        symbols["oiiint"]=NarrowMathOperatorSymbolUnicode(QChar(0x2230)).addGlobalFlags(IntLikeSymbolCorrection | SubSuperscriptBelowAboveSymbol).addMathOperatorHtml("&Cconint;");
        symbols["oiint"]=NarrowMathOperatorSymbolUnicode(QChar(0x222F)).addGlobalFlags(IntLikeSymbolCorrection | SubSuperscriptBelowAboveSymbol).addMathOperatorHtml("&DoubleContourIntegral;");
        symbols["oint"]=NarrowMathOperatorSymbolUnicode(QChar(0x222E)).addGlobalFlags(IntLikeSymbolCorrection | SubSuperscriptBelowAboveSymbol).addMathOperatorHtml("&ContourIntegral;");
        symbols["ominus"]=MathOperatorSymbolUnicode(QChar(0x2296)).addMathOperatorHtml("&ominus;");
        symbols["oplus"]=MathOperatorSymbolUnicode(QChar(0x2295)).addMathOperatorHtml("&CirclePlus;").addMathOperatorWinSymbol(QChar(0xC5));
        symbols["oslash"]=MathOperatorSymbolUnicode(QChar(0x2298)).addMathOperatorHtml("&osol;");
        symbols["otimes"]=MathOperatorSymbolUnicode(QChar(0x2297)).addMathOperatorHtml("&CircleTimes;").addMathOperatorWinSymbol(QChar(0xC4));
        symbols["parallel"]=NarrowMathOperatorSymbolUnicode(QChar(0x2225)).addMathOperatorHtml("&shortparallel;").addMathOperatorStd("||");
        symbols["pm"] = MathOperatorSymbol(QChar(0xB1), "&plusmn;").addMathOperatorWinSymbol(QChar(0xB1));
        symbols["prec"]=MathOperatorSymbolUnicode(QChar(0x227A)).addMathOperatorHtml("&prec;");
        symbols["prod"]=NarrowMathOperatorSymbolUnicode(QChar(0x220F)).addMathOperatorWinSymbol(QChar(0xD5), 1.8, 0.1).addMathOperatorHtml("&prod;").addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        symbols["propto"]=MathOperatorSymbolUnicode(QChar(0x221D)).addMathOperatorWinSymbol(QChar(0xB5)).addMathOperatorHtml("&Proportional;");
        symbols["rightharpoondown"]=UprightSymbolUnicode(QChar(0x21C1)).addUprightHtml("&rightharpoondown;");
        symbols["rightharpoonup"]=UprightSymbolUnicode(QChar(0x21C0)).addUprightHtml("&RightVector;");
        symbols["rightleftharpoon"]=UprightSymbolUnicode(QChar(0x21CC)).addUprightHtml("&Equilibrium;");
        symbols["searrow"]=UprightSymbolUnicode(QChar(0x2198)).addUprightHtml("&LowerRightArrow;");
        symbols["setminus"]=MathOperatorSymbolUnicode(QChar(0x2216)).addMathOperatorHtml("&smallsetminus;");
        { auto s=MathOperatorSymbolUnicode(QChar(0x223C)).addMathOperatorHtml("&tilde;").addMathOperatorStd("~");
            symbols["~"]=s; symbols["sim"]=s; }
        symbols["simeq"]=MathOperatorSymbolUnicode(QChar(0x2243)).addMathOperatorHtml("&TildeEqual;");
        symbols["sqcap"]=MathOperatorSymbolUnicode(QChar(0x2293)).addMathOperatorHtml("&SquareIntersection;");
        symbols["sqcup"]=MathOperatorSymbolUnicode(QChar(0x2294)).addMathOperatorHtml("&SquareUnion;");
        symbols["square"]=MathOperatorSymbolUnicode(QChar(0x25A1));
        symbols["subset"]=MathOperatorSymbolUnicode(QChar(0x2282)).addMathOperatorHtml("&sub;").addMathOperatorWinSymbol(QChar(0xCC));
        symbols["subseteq"]=MathOperatorSymbolUnicode(QChar(0x2286)).addMathOperatorHtml("&SubsetEqual;").addMathOperatorWinSymbol(QChar(0xCD));
        symbols["subsetnot"]=MathOperatorSymbolUnicode(QChar(0x2284)).addMathOperatorHtml("&nsub;").addMathOperatorWinSymbol(QChar(0xCB));
        symbols["succ"]=MathOperatorSymbolUnicode(QChar(0x227B)).addMathOperatorHtml("&succ;");
        symbols["sum"]=NarrowMathOperatorSymbolUnicode(QChar(0x2211)).addMathOperatorWinSymbol(QChar(0xE5), 1.8, 0.1).addMathOperatorHtml("&sum;").addGlobalFlags(SubSuperscriptBelowAboveSymbol);
        symbols["supset"]=MathOperatorSymbolUnicode(QChar(0x2283)).addMathOperatorHtml("&sup;").addMathOperatorWinSymbol(QChar(0xC9));
        symbols["supseteq"]=MathOperatorSymbolUnicode(QChar(0x2287)).addMathOperatorHtml("&SupersetEqual;").addMathOperatorWinSymbol(QChar(0xCA));
        symbols["supsetnot"]=MathOperatorSymbolUnicode(QChar(0x2285)).addMathOperatorHtml("&nsup;");
        symbols["swarrow"]=UprightSymbolUnicode(QChar(0x2199)).addUprightHtml("&LowerLeftArrow;");
        symbols["therefore"]=MathOperatorSymbolUnicode(QChar(0x2234)).addMathOperatorHtml("&therefore;").addMathOperatorWinSymbol(QChar(0x5C));
        symbols["times"] = MathOperatorSymbol(QChar(0xD7), "&times;").addMathOperatorWinSymbol(QChar(0xB4));
        { auto s=UprightSymbolUnicode(QChar(0x2192)).addUprightHtml("&rarr;").addUprightWinSymbol(QChar(0xAE));
          symbols["to"]=s; symbols["rightarrow"]=s; }
        symbols["top"]=MathOperatorSymbolUnicode(QChar(0x22A4)).addMathOperatorHtml("&top;").addMathOperatorWinSymbol(QChar(0x5E)).addUprightStd("T");
        symbols["triangle"]=NarrowMathOperatorSymbolUnicode(QChar(0x2206));
        symbols["uparrow"]=UprightSymbolUnicode(QChar(0x2191)).addUprightHtml("&ShortUpArrow;").addUprightWinSymbol(QChar(0xAD));
        symbols["updownarrow"]=UprightSymbolUnicode(QChar(0x2195)).addUprightHtml("&updownarrow;");
        symbols["upharpoonleft"]=UprightSymbolUnicode(QChar(0x21BF)).addUprightHtml("&LeftUpVector;");
        symbols["upharpoonright"]=UprightSymbolUnicode(QChar(0x21BE)).addUprightHtml("&RightUpVector;");
        symbols["vartriangleleft"]=NarrowMathOperatorSymbolUnicode(QChar(0x22B2)).addMathOperatorHtml("&LeftTriangle;");
        symbols["vdots"]=UprightSymbolUnicode(QChar(0x22EE)).addMathOperatorHtml("&vellip;");
        symbols["vee"]=MathOperatorSymbolUnicode(QChar(0x2228)).addMathOperatorHtml("&vee;").addMathOperatorWinSymbol(QChar(0xDA));
        symbols["vdash"]=MathOperatorSymbolUnicode(QChar(0x22A2)).addMathOperatorHtml("&vdash;");
        symbols["dashv"]=MathOperatorSymbolUnicode(QChar(0x22A3)).addMathOperatorHtml("&dashv;");
        symbols["vDash"]=MathOperatorSymbolUnicode(QChar(0x22A8)).addMathOperatorHtml("&DoubleRightTee;");
        symbols["nvdash"]=MathOperatorSymbolUnicode(QChar(0x22AC)).addMathOperatorHtml("&nvdash;");
        symbols["Vdash"]=MathOperatorSymbolUnicode(QChar(0x22A9)).addMathOperatorHtml("&Vdash;");
        symbols["models"]=MathOperatorSymbolUnicode(QChar(0x22A7)).addMathOperatorHtml("&models;");
        symbols["wedge"]=MathOperatorSymbolUnicode(QChar(0x2227)).addMathOperatorHtml("&wedge;").addMathOperatorWinSymbol(QChar(0xD9));



        /**************************************************************************************
         * GREEK letters
         **************************************************************************************/
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "alpha", "a", QChar(0x3B1), "&alpha;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "beta", "b", QChar(0x3B2), "&beta;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "gamma", "g", QChar(0x3B3), "&gamma;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "delta", "d", QChar(0x3B4), "&delta;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "epsilon", "e", QChar(0x3F5), "&varepsilon;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "varepsilon", "e", QChar(0x3B5), "&epsi;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "zeta", "z", QChar(0x3B6),"&zeta;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "eta", "h", QChar(0x3B7),"&eta;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "theta", "q", QChar(0x3B8),"&theta;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "vartheta", "J", QChar(0x3D1),"&thetasym;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "iota", "i", QChar(0x3B9),"&iota;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "kappa", "k", QChar(0x3BA),"&kappa;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "lambda", "l", QChar(0x3BB),"&lambda;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "mu", "m", QChar(0x3BC),"&mu;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "nu", "n", QChar(0x3BD),"&nu;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "xi", "x", QChar(0x3BE),"&xi;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "pi", "p", QChar(0x3C0),"&pi;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "varpi", "v", QChar(0x3D6),"&piv;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "rho", "r", QChar(0x3C1),"&rho;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "varrho", "r", QChar(0x3F1),"&varrho;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "sigma", "s", QChar(0x3C3),"&sigma;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "varsigma", "V", QChar(0x3C2),"&varsigma;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "tau", "t", QChar(0x3C4),"&tau;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "upsilon", "u", QChar(0x3C5),"&upsilon;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "phi", "f", QChar(0x3D5),"&straightphi;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "varphi", "j", QChar(0x3C6),"&phi;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "chi", "c", QChar(0x3C7),"&chi;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "psi", "y", QChar(0x3C8),"&psi;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "omega", "w", QChar(0x3C9),"&omega;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "Gamma", "G", QChar(0x393),"&Gamma;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "Delta", "D", QChar(0x394),"&Delta;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "Theta", "Q", QChar(0x398),"&Theta;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "Lambda", "L", QChar(0x39B),"&Lambda;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "Omega", "W", QChar(0x3A9),"&Omega;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "Xi", "X", QChar(0x39E),"&Xi;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "Pi", "P", QChar(0x3A0),"&Pi;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "Sigma", "S", QChar(0x3A3),"&Sigma;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "Upsilon", "U", QChar(0x3A5),"&Upsilon;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "Phi", "F", QChar(0x3A6),"&Phi;");
        addGreekLetterVariants_WinSymbol_Unicode_Html(symbols, "Psi", "Y", QChar(0x3A8),"&Psi;");


        /**************************************************************************************
         * SYMBOLS from special fonts
         **************************************************************************************/
    #if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
            QFontDatabase fdb;
            const auto fonts=fdb.families();
    #else
            const auto fonts=QFontDatabase::families();
    #endif
        if (fonts.contains("Wingdings")) {
            { auto s=SymbolFullProps("Wingdings", QChar(0x46));
            symbols["lefthand"]=s; symbols["HandRight"]=s;}
            { auto s=SymbolFullProps("Wingdings", QChar(0x45));
            symbols["righthand"]=s; symbols["HandLeft"]=s;}
        }

        return symbols;
    }();
    return s_symbols;

}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::MathOperatorSymbolUnicode(const QString &unicode)
{
    return SymbolFullProps(MTFEUnicode, SymbolProps(unicode, ItalicOff|BoldOff, 1.0, 0.0)).addGlobalFlags(ExtendWidthInMathmode|MakeWhitespaceHalf);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::NarrowMathOperatorSymbolUnicode(const QString &unicode)
{
    return SymbolFullProps(MTFEUnicode, SymbolProps(unicode, ItalicOff|BoldOff, 1.0, 0.0)).addGlobalFlags(SmallExtendWidthInMathmode|MakeWhitespaceHalf);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::NarrowMathOperatorSymbolStd(const QString &symbol)
{
    return NarrowMathOperatorSymbolStd(symbol,symbol);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::NarrowMathOperatorSymbolStd(const QString &symbol, const QString &/*symbolHTML*/)
{
    return SymbolFullProps(MTFEStandard, SymbolProps(symbol, ItalicOff|BoldOff, 1.0, 0.0)).addHtml(symbol, ItalicOff|BoldOff, 1.0, 0.0).addGlobalFlags(SmallExtendWidthInMathmode|MakeWhitespaceHalf);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::MathGreekLetter_WinSymbol_Unicode_Html(const QString &letterWinSymbol, const QString &letterUnicode, const QString &html)
{
    return SymbolFullProps(MTFEUnicode, SymbolProps(letterUnicode, ItalicOn), MTFEWinSymbol, SymbolProps(letterWinSymbol, ItalicOn), html);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::AsOutsiudeGreekLetter_WinSymbol_Unicode_Html(const QString &letterWinSymbol, const QString &letterUnicode, const QString &html)
{
    return SymbolFullProps(MTFEUnicode, SymbolProps(letterUnicode, AsOutside), MTFEWinSymbol, SymbolProps(letterWinSymbol, AsOutside), html);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::UprightGreekLetter_WinSymbol_Unicode_Html(const QString &letterWinSymbol, const QString &letterUnicode, const QString &html)
{
    return SymbolFullProps(MTFEUnicode, SymbolProps(letterUnicode, ItalicOff), MTFEWinSymbol, SymbolProps(letterWinSymbol, ItalicOff), html, ItalicOff);
}

void JKQTMathTextSymbolNode::addGreekLetterVariants_WinSymbol_Unicode_Html(QHash<QString, JKQTMathTextSymbolNode::SymbolFullProps>& symbols, const QString &baseInstructionName, const QString &letterWinSymbol, const QString &letterUnicode, const QString &html)
{
    symbols[baseInstructionName]=MathGreekLetter_WinSymbol_Unicode_Html(letterWinSymbol, letterUnicode, html).addGlobalFlags(SubscriptCorrection);
    symbols["text"+baseInstructionName]=AsOutsiudeGreekLetter_WinSymbol_Unicode_Html(letterWinSymbol, letterUnicode, html);
    symbols["up"+baseInstructionName]=UprightGreekLetter_WinSymbol_Unicode_Html(letterWinSymbol, letterUnicode, html);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::StdSymbol(const QString &symbol, const QString &html)
{
    return SymbolFullProps(MTFEStandard, SymbolProps(symbol), html);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::UnicodeSymbol(const QString &symbol, SymbolFlags _flags, double _fontScalingFactor, double _yShiftFactor)
{
    return SymbolFullProps(MTFEUnicode, SymbolProps(symbol, _flags, _fontScalingFactor, _yShiftFactor));
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::UprightSymbolStd(const QString &symbol)
{
    QString html=symbol;
    return UprightSymbolStd(symbol, html);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::UnicodeSymbol(const QString &symbol, const QString &html)
{
    return SymbolFullProps(MTFEUnicode, SymbolProps(symbol), html);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::StdSymbol(const QString &symbol, SymbolFlags _flags, double _fontScalingFactor, double _yShiftFactor)
{
    return SymbolFullProps(MTFEStandard, SymbolProps(symbol, _flags, _fontScalingFactor, _yShiftFactor), symbol);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::UprightSymbolStd(const QString &symbol, const QString &html)
{
    return SymbolFullProps(MTFEStandard, SymbolProps(symbol, ItalicOff), html, ItalicOff);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::UprightSymbolUnicode(const QString &symbol)
{
    QString html=symbol;
    return UprightSymbolUnicode(symbol, html);

}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::UprightSymbolUnicode(const QString &symbol, const QString &html)
{
    return SymbolFullProps(MTFEUnicode, SymbolProps(symbol, ItalicOff), html, ItalicOff);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::SimpleTextSymbol(const QString &symbol, const QString &html)
{
    return SymbolFullProps(MTFEStandard, SymbolProps(symbol), html);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::SimpleTextSymbol(const QString &symbol, SymbolFlags _flags, double _fontScalingFactor, double _yShiftFactor)
{
    return SymbolFullProps(MTFEStandard, SymbolProps(symbol, _flags, _fontScalingFactor, _yShiftFactor), symbol);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::SimpleUprightTextSymbol(const QString &symbol, const QString &html)
{
    return SymbolFullProps(MTFEStandard, SymbolProps(symbol, ItalicOff), html, ItalicOff);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::SimpleUprightTextSymbol(const QString &symbol)
{
    return SymbolFullProps(MTFEStandard, SymbolProps(symbol, ItalicOff), symbol, ItalicOff);
}






JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps():
    fontType(MTECurrentFont),
    customFontFamily(),
    html(),
    globalFlags(NoGLobalSymbolFlags)
{

}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(const QString &_font, const SymbolProps &props, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    fontType(MTECustomFont),
    customFontFamily(_font),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[MTFEStandard]=props;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(const QString &_font, const QString &symbol, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    fontType(MTECustomFont),
    customFontFamily(_font),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[MTFEStandard]=SymbolProps(symbol);
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(const SymbolProps &props, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    fontType(MTECurrentFont),
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[MTFEStandard]=props;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, const SymbolProps &props, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    fontType(_fontType),
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[MTFEStandard]=props;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, const QString &symbol, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    fontType(_fontType),
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[MTFEStandard]=SymbolProps(symbol);
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextFontEncoding enc0, const SymbolProps &props0, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    fontType(MTECurrentFont),
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[enc0]=props0;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, JKQTMathTextFontEncoding enc0, const SymbolProps &props0, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    fontType(_fontType),
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[enc0]=props0;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextFontEncoding enc0, const SymbolProps &props0, JKQTMathTextFontEncoding enc1, const SymbolProps &props1, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    fontType(MTECurrentFont),
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[enc0]=props0;
    this->props[enc1]=props1;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, JKQTMathTextFontEncoding enc0, const SymbolProps &props0, JKQTMathTextFontEncoding enc1, const SymbolProps &props1, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    fontType(_fontType),
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[enc0]=props0;
    this->props[enc1]=props1;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextFontEncoding enc0, const SymbolProps &props0, JKQTMathTextFontEncoding enc1, const SymbolProps &props1, JKQTMathTextFontEncoding enc2, const SymbolProps &props2, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    fontType(MTECurrentFont),
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[enc0]=props0;
    this->props[enc1]=props1;
    this->props[enc2]=props2;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, JKQTMathTextFontEncoding enc0, const SymbolProps &props0, JKQTMathTextFontEncoding enc1, const SymbolProps &props1, JKQTMathTextFontEncoding enc2, const SymbolProps &props2, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    fontType(_fontType),
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[enc0]=props0;
    this->props[enc1]=props1;
    this->props[enc2]=props2;
}

JKQTMathTextSymbolNode::SymbolProps JKQTMathTextSymbolNode::SymbolFullProps::getProps(JKQTMathTextFontEncoding key, const SymbolProps &defaultValue, JKQTMathTextFontEncoding *foundEncodingOut) const
{
    if (props.contains(key)) {
        if (foundEncodingOut) *foundEncodingOut=key;
        return props[key];
    }
    if (key==MTFEUnicode && props.contains(MTFEStandard))  {
        if (foundEncodingOut) *foundEncodingOut=MTFEStandard;
        return props[MTFEStandard];
    }
    if (foundEncodingOut) *foundEncodingOut=key;
    return defaultValue;
}

JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::add(JKQTMathTextFontEncoding enc, const SymbolProps &_props)
{
    props[enc]=_props;
    return *this;
}

JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::addWinSymbol(const QString &_symbol, SymbolFlags _flags, double _fontScalingFactor, double _yShiftFactor)
{
    props[MTFEWinSymbol]=SymbolProps(_symbol, _flags, _fontScalingFactor, _yShiftFactor);
    return *this;
}

JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::addMathOperatorWinSymbol(const QString &_symbol, double _fontScalingFactor, double _yShiftFactor)
{
    props[MTFEWinSymbol]=SymbolProps(_symbol, ItalicOff|BoldOff, _fontScalingFactor, _yShiftFactor);
    return *this;
}

JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::addUprightWinSymbol(const QString &_symbol, double _fontScalingFactor, double _yShiftFactor)
{
    props[MTFEWinSymbol]=SymbolProps(_symbol, ItalicOff, _fontScalingFactor, _yShiftFactor);
    return *this;
}


JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::addUnicode(const QString &_symbol, SymbolFlags _flags, double _fontScalingFactor, double _yShiftFactor)
{
    props[MTFEUnicode]=SymbolProps(_symbol, _flags, _fontScalingFactor, _yShiftFactor);
    return *this;
}

JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::addUprightUnicode(const QString &_symbol, double _fontScalingFactor, double _yShiftFactor)
{
    props[MTFEUnicode]=SymbolProps(_symbol, ItalicOff, _fontScalingFactor, _yShiftFactor);
    return *this;
}

JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::addMathOperatorUnicode(const QString &_symbol, double _fontScalingFactor, double _yShiftFactor)
{
    props[MTFEUnicode]=SymbolProps(_symbol, ItalicOff|BoldOff, _fontScalingFactor, _yShiftFactor);
    return *this;
}

JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::addStd(const QString &_symbol, SymbolFlags _flags, double _fontScalingFactor, double _yShiftFactor)
{
    props[MTFEStandard]=SymbolProps(_symbol, _flags, _fontScalingFactor, _yShiftFactor);
    return *this;
}

JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::addMathOperatorStd(const QString &_symbol, double _fontScalingFactor, double _yShiftFactor)
{
    props[MTFEStandard]=SymbolProps(_symbol, ItalicOff|BoldOff, _fontScalingFactor, _yShiftFactor);
    return *this;
}

JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::addUprightStd(const QString &_symbol, double _fontScalingFactor, double _yShiftFactor)
{
    props[MTFEStandard]=SymbolProps(_symbol, ItalicOff, _fontScalingFactor, _yShiftFactor);
    return *this;
}

JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::addHtml(const QString &_symbol, SymbolFlags _flags, double _fontScalingFactor, double _yShiftFactor)
{
    html=SymbolProps(_symbol, _flags, _fontScalingFactor, _yShiftFactor);
    return *this;
}

JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::addMathOperatorHtml(const QString &_symbol, double _fontScalingFactor, double _yShiftFactor)
{
    html=SymbolProps(_symbol, ItalicOff|BoldOff, _fontScalingFactor, _yShiftFactor);
    return *this;
}

JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::addUprightHtml(const QString &_symbol, double _fontScalingFactor, double _yShiftFactor)
{
    html=SymbolProps(_symbol, ItalicOff, _fontScalingFactor, _yShiftFactor);
    return *this;
}

JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::addGlobalFlags(GlobalSymbolFlags addglobalFlags)
{
    globalFlags |= addglobalFlags;
    return *this;
}

JKQTMathTextSymbolNode::SymbolFullProps &JKQTMathTextSymbolNode::SymbolFullProps::setGlobalFlags(GlobalSymbolFlags addglobalFlags)
{
    globalFlags = addglobalFlags;
    return *this;
}

QPair<QFont, JKQTMathTextSymbolNode::SymbolProps> JKQTMathTextSymbolNode::SymbolFullProps::getDrawingData(JKQTMathTextEnvironment currentEv, JKQTMathText* parent, QPainter& painter) const
{
    //qDebug()<<"    ---------------------";
    SymbolProps outProps;
    QFont outFont;
    const JKQTMathTextFontEncoding currentEnc=currentEv.getFontEncoding(parent);
    QFont currentFont=currentEv.getFont(parent);
    currentFont.setStyleStrategy(QFont::NoFontMerging);
    const QFontMetricsF currentFM(currentFont, painter.device());
    QFont fallbackSymbolsFont=parent->getFallbackFontSymbols();
    const JKQTMathTextFontEncoding fallbackSymbolsFontEnc=parent->getFontEncodingFallbackFontSymbols();
    fallbackSymbolsFont.setStyleStrategy(QFont::NoFontMerging);
    const QFontMetricsF fallbackSymbolsFM(fallbackSymbolsFont, painter.device());
    const QChar chFallbackSym=props.value(fallbackSymbolsFontEnc, SymbolProps()).getSymbolSingleChar();

    if (fontType==MTECustomFont) {
        //qDebug()<<"      ==>1 fontType==MTECustomFont";
        currentEv.customFontName=customFontFamily;
        currentEv.font=MTECustomFont;
        outProps=getProps(MTFEUnicode, outProps);
    } else if (fontType==MTEFallbackSymbols) {
        //qDebug()<<"      ==>2 fontType==MTEFallbackSymbols";
        if (!chFallbackSym.isNull() && fallbackSymbolsFM.inFont(chFallbackSym)) {
            //qDebug()<<"      ==>2.1 !chFallbackSym.isNull() && fallbackSymbolsFM.inFont(chFallbackSym)";
            currentEv.font=MTEFallbackSymbols;
            outProps=getProps(fallbackSymbolsFontEnc, outProps);
        } else {
            //qDebug()<<"      ==>2.2 else";
            outProps=getProps(currentEnc, outProps);
        }
    } else if (fontType==MTECurrentFont) {
        //qDebug()<<"      ==>2 fontType==MTECurrentFont";
        JKQTMathTextFontEncoding localEnc=currentEnc;
        outProps=getProps(currentEnc, outProps, &localEnc);

        const QChar chLocal=outProps.getSymbolSingleChar();
        bool foundConfig=false;
        if (outProps.symbol.size()>1 ||(!chLocal.isNull() && currentFM.inFont(chLocal))) {
            foundConfig=true;
        } else if (localEnc!=MTFEStandard && props.contains(MTFEStandard)) {
            outProps=props[MTFEStandard];
            const QChar chStd=outProps.getSymbolSingleChar();
            if (outProps.symbol.size()>1 || (!chStd.isNull() && currentFM.inFont(chStd))) {
                foundConfig=true;
            }
        }
        if (!foundConfig && !chFallbackSym.isNull() && fallbackSymbolsFM.inFont(chFallbackSym)) {
            //qDebug()<<"          ==>2.1.1 !chFallbackSym.isNull() && fallbackSymbolsFM.inFont(chFallbackSym)";
            outProps=props[fallbackSymbolsFontEnc];
            currentEv.font=MTEFallbackSymbols;
            foundConfig=true;
        }
        if (!foundConfig) {
            currentEv.font=MTECurrentFont;
            outProps=getProps(currentEnc, outProps, &localEnc);
        }
    } else {
        //qDebug()<<"      ==>3 else";
        currentEv.font=fontType;
        outProps=getProps(currentEv.getFontEncoding(parent), outProps);
        QFont localFont=currentEv.getFont(parent);
        localFont.setStyleStrategy(QFont::NoFontMerging);
        const QFontMetricsF localFM(localFont, painter.device());
        const QChar chLocal=outProps.getSymbolSingleChar();

        if (!chLocal.isNull() && !localFM.inFont(chLocal) && fallbackSymbolsFM.inFont(chFallbackSym)) {
            //qDebug()<<"      ==>3.1 !chLocal.isNull() && !localFM.inFont(chLocal) && fallbackSymbolsFM.inFont(chFallbackSym)";
            currentEv.font=MTEFallbackSymbols;
            outProps=getProps(fallbackSymbolsFontEnc, outProps);
        }
    }


    outFont=currentEv.getFont(parent);
    outFont.setStyleStrategy(QFont::PreferDefault);
    if (outProps.fontScalingFactor!=1) outFont.setPointSizeF(outFont.pointSizeF()*outProps.fontScalingFactor);
    if (has(outProps.flags, ItalicOn)) outFont.setItalic(true);
    if (has(outProps.flags, ItalicOff)) outFont.setItalic(false);
    if (has(outProps.flags, BoldOn)) outFont.setBold(true);
    if (has(outProps.flags, BoldOff)) outFont.setBold(false);
    return QPair<QFont, SymbolProps>(outFont, outProps);
}

JKQTMathTextSymbolNode::NodeSize::NodeSize():
    JKQTMathTextNodeSize(),
    subSuperAboveBelowXCorrection(0.0)
{

}

JKQTMathTextSymbolNode::NodeSize &JKQTMathTextSymbolNode::NodeSize::operator=(const JKQTMathTextNodeSize &other)
{
    subSuperAboveBelowXCorrection=0;
    JKQTMathTextNodeSize::operator=(other);
    return *this;
}

JKQTMathTextSymbolNode::NodeSize &JKQTMathTextSymbolNode::NodeSize::operator=(const NodeSize &other)
{
    subSuperAboveBelowXCorrection=other.subSuperAboveBelowXCorrection;
    JKQTMathTextNodeSize::operator=(other);
    return *this;
}

JKQTMathTextSymbolNode::NodeSize::NodeSize(const JKQTMathTextNodeSize &other):
    NodeSize()
{
    operator=(other);
}

JKQTMathTextSymbolNode::NodeSize::NodeSize(const NodeSize &other):
    NodeSize()
{
    operator=(other);
}
