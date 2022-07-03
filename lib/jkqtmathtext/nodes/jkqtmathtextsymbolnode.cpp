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



#include "jkqtmathtext/nodes/jkqtmathtextsymbolnode.h"
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




JKQTMathTextSymbolNode::JKQTMathTextSymbolNode(JKQTMathText* _parent, const QString& name, bool _addWhitespace):
    JKQTMathTextNode(_parent), symbolName(name), addWhitespace(_addWhitespace)
{
    fillSymbolTables();
}

JKQTMathTextSymbolNode::~JKQTMathTextSymbolNode() {

}

QString JKQTMathTextSymbolNode::getTypeName() const
{
    return QLatin1String("JKQTMathTextSymbolNode(")+symbolName+QLatin1String(")");
}


void JKQTMathTextSymbolNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize) {
    double dummy1, dummy2;
    getSymbolSizeInternal(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos, dummy1, dummy2, prevNodeSize);
}

void JKQTMathTextSymbolNode::getSymbolSizeInternal(QPainter &painter, JKQTMathTextEnvironment currentEv, double &width, double &baselineHeight, double &overallHeight, double &strikeoutPos, double &subSuperXCorrection, double &subBesidesXCorrection, const JKQTMathTextNodeSize *prevNodeSize)
{
    const auto fullProps=symbols.value(symbolName, SymbolFullProps());
    const GlobalSymbolFlags globalFlags=fullProps.globalFlags;
    const auto drawProps=fullProps.getDrawingData(currentEv, parentMathText);
    const QFont f=drawProps.first;
    const QFontMetricsF fm(f);
    const JKQTMathTextSymbolNode::SymbolProps symprops=drawProps.second;
    const SymbolFlags symflags=symprops.flags;
    const QString sym=symprops.symbol;
    const QRectF tbr=getTightBoundingRect(fm, sym, globalFlags);
    const QRectF br=getBoundingRect(fm, sym, globalFlags);
    const QRectF tbrNoSymbol=JKQTMathTextGetTightBoundingRect(f, "X", painter.device());
    const QRectF mintbr=JKQTMathTextGetTightBoundingRect(f, "(", painter.device());
    const double yShift=symprops.yShiftFactor*tbr.height();


    if (currentEv.insideMath) {
        width=qMax(br.width(), mintbr.width());
        if (has(globalFlags, ExtendWidthInMathmode)) {
            if (!symprops.getSymbolSingleChar().isNull()) width=width*parentMathText->getMathoperatorWidthFactor();
            else width=width+mintbr.width();
        }
    } else {
        width=br.width();
    }
    if (!sym.isEmpty()) {
        overallHeight=tbr.height();
        baselineHeight=tbr.height()-tbr.bottom();
    } else {
        overallHeight=tbrNoSymbol.height();
        baselineHeight=tbrNoSymbol.height()-tbrNoSymbol.bottom();
    }
    const double oldDescent=overallHeight-baselineHeight;

    if (has(symflags, HeightIsAscent)) {
        baselineHeight=fm.ascent();
        overallHeight=baselineHeight+oldDescent;
    }
    strikeoutPos=fm.strikeOutPos();

    if (has(globalFlags, IntLikeSymbolCorrection)) {
        subSuperXCorrection=parentMathText->getIntSubSuperXCorrectionFactor()*tbr.width();
        subBesidesXCorrection=parentMathText->getIntSubBesidesXCorrectionXFactor()*JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width();
    }
}

QRectF JKQTMathTextSymbolNode::getBoundingRect(const QFontMetricsF &fm, const QString &text, GlobalSymbolFlags globalFlags)
{
    if (has(globalFlags, MakeWhitespaceHalf) && text.contains(' ')) {
        const QStringList str=text.simplified().trimmed().split(' ');
        const QRectF brSp=fm.boundingRect("i");
        QRectF br;
        for (int i=0; i<str.size(); i++) {
            const QRectF lbr=fm.boundingRect(str[i]);
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
        return fm.boundingRect(text);
    }
}

QRectF JKQTMathTextSymbolNode::getTightBoundingRect(const QFontMetricsF &fm, const QString &text, GlobalSymbolFlags globalFlags)
{
    if (has(globalFlags, MakeWhitespaceHalf) && text.contains(' ')) {
        const QStringList str=text.simplified().trimmed().split(' ');
        const QRectF brSp=fm.boundingRect("i");
        QRectF br;
        for (int i=0; i<str.size(); i++) {
            const QRectF lbr=fm.tightBoundingRect(str[i]);
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
        return fm.tightBoundingRect(text);
    }
}

void JKQTMathTextSymbolNode::drawText(QPainter &p, const QString &text, GlobalSymbolFlags globalFlags)
{
    if (has(globalFlags, MakeWhitespaceHalf) && text.contains(' ')) {
        const QStringList str=text.simplified().trimmed().split(' ');
        const QFontMetricsF fm(p.font());
        const QRectF brSp=fm.boundingRect("i");
        double x=0;
        for (int i=0; i<str.size(); i++) {
            p.drawText(QPointF(x,0), str[i]);
            x=x+fm.boundingRect(str[i]).width()+brSp.width()/2.0;
        }
    } else {
        p.drawText(QPointF(0,0), text);
    }
}


double JKQTMathTextSymbolNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    double width=0;
    double baselineHeight=0;
    double overallHeight=0, strikeoutPos=0;
    getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);

    const auto fullProps=symbols.value(symbolName, SymbolFullProps());
    const GlobalSymbolFlags globalFlags=fullProps.globalFlags;
    const auto drawProps=fullProps.getDrawingData(currentEv, parentMathText);
    const QFont f=drawProps.first;
    const QFontMetricsF fm(f);
    const JKQTMathTextSymbolNode::SymbolProps symprops=drawProps.second;
    const SymbolFlags symflags=symprops.flags;
    const QString sym=symprops.symbol;
    const QRectF tbr=getTightBoundingRect(fm, sym, globalFlags);
    const QRectF br=getBoundingRect(fm, sym, globalFlags);
    const QRectF tbrNoSymbol=JKQTMathTextGetTightBoundingRect(f, "X", painter.device());
    const double yShift=symprops.yShiftFactor*tbr.height();
    const double xShift=(width-tbr.width())/2.0;
    const QPointF x0(x+xShift-tbr.x(), y+yShift);

    //qDebug()<<"SYMB::draw(): symbolName="<<symbolName<<" font="<<f<<" sym="<<sym<<" yShiftFactor="<<symprops.yShiftFactor;

    if (!sym.isEmpty()) {
        // if the symbol has been recognized in the constructor: draw the symbol
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setFont(f);
        painter.setPen(currentEv.color);
        painter.translate(x0);
        drawText(painter, sym, globalFlags);
        if (has(symflags, DrawBar)) {
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.setPen(QPen(currentEv.color, fm.lineWidth()));
            const double ybar=0.66*tbr.top();
            painter.drawLine(br.width()-tbrNoSymbol.width()/2.0, ybar, br.width(), ybar);
        }
        if (has(symflags, DrawSlash)) {
            painter.drawText(QPointF(0,0),"/");
        }
        /*painter.save();
        painter.setPen(QPen(QColor("red"), 0.5, Qt::DotLine));
        painter.drawEllipse(0,0,5,5);
        painter.drawRect(tbr);
        painter.setPen(QPen(QColor("blue"), 0.5, Qt::DashLine));
        painter.drawRect(br);
        painter.restore();*/
    } else { // draw a box to indicate an unavailable symbol
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.setPen(QPen(currentEv.color, fm.lineWidth()));
        painter.drawRect(QRectF(x0.x(), x0.y()-tbrNoSymbol.height(), tbrNoSymbol.width(), tbrNoSymbol.height()));
    }

    return x+width;
}

bool JKQTMathTextSymbolNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
    bool ok=true;
    const auto props=symbols.value(symbolName, SymbolFullProps());
    QString s=props.html.symbol;
    JKQTMathTextEnvironment ev=currentEv;
    ev.fontSize=ev.fontSize*props.html.fontScalingFactor;
    if (has(props.globalFlags, ExtendWidthInMathmode)) s="&thinsp;"+s+"&thinsp;";
    if (has(props.globalFlags, MakeWhitespaceHalf)) s.replace(" ", "&thinsp;");
    if (ok) html=html+ev.toHtmlStart(defaultEv)+s+ev.toHtmlAfter(defaultEv);
    return ok;
}

QString JKQTMathTextSymbolNode::getSymbolName() const {
    return this->symbolName;
}

bool JKQTMathTextSymbolNode::getAddWhitespace() const
{
    return addWhitespace;
}

void JKQTMathTextSymbolNode::getSymbolSize(QPainter &painter, JKQTMathTextEnvironment currentEv, double &width, double &baselineHeight, double &overallHeight, double &strikeoutPos, double &subSuperXCorrection, double &subBesidesXCorrection, const JKQTMathTextNodeSize *prevNodeSize)
{
    double w=width, b=baselineHeight, o=overallHeight, s=strikeoutPos;
    getSymbolSizeInternal(painter, currentEv, w, b, o, s, subSuperXCorrection, subBesidesXCorrection, prevNodeSize);

    if (w<1e5) width=w;
    if (b<1e5) baselineHeight=b;
    if (o<1e5) overallHeight=o;
    if (s<1e5) strikeoutPos=s;

}

bool JKQTMathTextSymbolNode::hasSymbol(const QString &symbolName)
{
    fillSymbolTables();
    return symbols.contains(symbolName);
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
    return SymbolFullProps(SymbolProps(op, ItalicOff|BoldOff, 1.0, 0.0)).setGlobalFlags(ExtendWidthInMathmode).addHtml(ophtml, ItalicOff|BoldOff, 1.0, 0.0);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::MathOperatorText(const QString &op)
{
    return MathOperatorText(op, op);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::MathOperatorText(const QString &op, const QString &ophtml)
{
    return SymbolFullProps(SymbolProps(op, ItalicOff|BoldOff|HeightIsAscent, 1.0, 0.0)).setGlobalFlags(ExtendWidthInMathmode|MakeWhitespaceHalf).addHtml(ophtml, ItalicOff|BoldOff|HeightIsAscent, 1.0, 0.0);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::MathOperatorSymbolUnicode(const QString &unicode)
{
    return SymbolFullProps(MTFEUnicode, SymbolProps(unicode, ItalicOff|BoldOff, 1.0, 0.0)).setGlobalFlags(ExtendWidthInMathmode|MakeWhitespaceHalf);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::GreekLetter_WinSymbol_Unicode_Html(const QString &letterWinSymbol, const QString &letterUnicode, const QString &html)
{
    return SymbolFullProps(MTEFallbackGreek, MTFEUnicode, SymbolProps(letterUnicode), MTFEWinSymbol, SymbolProps(letterWinSymbol), html);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::SymbolStd(const QString &symbol, const QString &html)
{
    return SymbolFullProps(MTFEStandard, SymbolProps(symbol), html);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::SymbolUnicode(const QString &symbol, const QString &html)
{
    return SymbolFullProps(MTFEUnicode, SymbolProps(symbol), html);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::UprightSymbolStd(const QString &symbol, const QString &html)
{
    return SymbolFullProps(MTFEStandard, SymbolProps(symbol, ItalicOff), html, ItalicOff);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::UprightSymbolUnicode(const QString &symbol, const QString &html)
{
    return SymbolFullProps(MTFEUnicode, SymbolProps(symbol, ItalicOff), html, ItalicOff);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::SimpleTextSymbol(const QString &symbol, const QString &html)
{
    return SymbolFullProps(MTFEStandard, SymbolProps(symbol), html);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::SimpleTextSymbol(const QString &symbol)
{
    return SymbolFullProps(MTFEStandard, SymbolProps(symbol), symbol);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::SimpleUprightTextSymbol(const QString &symbol, const QString &html)
{
    return SymbolFullProps(MTFEStandard, SymbolProps(symbol, ItalicOff), html, ItalicOff);
}

JKQTMathTextSymbolNode::SymbolFullProps JKQTMathTextSymbolNode::SimpleUprightTextSymbol(const QString &symbol)
{
    return SymbolFullProps(MTFEStandard, SymbolProps(symbol, ItalicOff), symbol, ItalicOff);
}



QHash<QString, JKQTMathTextSymbolNode::SymbolFullProps> JKQTMathTextSymbolNode::symbols=QHash<QString, JKQTMathTextSymbolNode::SymbolFullProps>();

void JKQTMathTextSymbolNode::fillSymbolTables()
{
    if (symbols.size()>0) return; // tables have already been filled! So nothing to do here

    /**************************************************************************************
     * STANDARD Symbols available in all standard fonts
     **************************************************************************************/
    symbols[" "]=SimpleTextSymbol(" ", "&nbsp;");
    symbols[""]=SimpleTextSymbol(" ", "&nbsp;");
    symbols["#"]=SimpleTextSymbol("#", "&num;");
    symbols["%"]=SimpleTextSymbol("%", "&NestedGreaterGreater;");
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
    symbols["Angstrom"]= symbols["Angstroem"]=UprightSymbolUnicode(QChar(0x212B)).addUprightStd(QChar(0xC5));
    symbols["Beth"]=symbols["Bet"]=symbols["beth"]=symbols["bet"]=SymbolUnicode(QChar(0x2136)).addHtml("&beth;");
    symbols["Box"]=UprightSymbolUnicode(QChar(0x25A1));
    symbols["DC"]=SymbolUnicode(QChar(0x2393)).addWinSymbol(QChar(0xBB)).addStd("=");
    symbols["EUR"]=SymbolUnicode(QChar(0x20AC));
    symbols["Im"]=UprightSymbolUnicode(QChar(0x2111)).addHtml("&image;").addWinSymbol(QChar(0xC1));
    symbols["No"]=SymbolUnicode(QChar(0x2116));
    symbols["Ohm"]=symbols["ohm"]=UprightSymbolUnicode(QChar(0x2126)).addUprightWinSymbol("W").addUprightHtml("&Omega;");
    symbols["P"]=SimpleTextSymbol(QChar(0xB6)).addHtml("&para;");
    symbols["Re"]=SymbolUnicode(QChar(0x211C)).addHtml("&Rfr;").addWinSymbol(QChar(0xC2));
    symbols["S"]=symbols["§"]=SimpleTextSymbol(QChar(0xA7)).addHtml("&sect;");
    symbols["accurrent"]=SymbolUnicode(QChar(0x23E6));
    symbols["alef"]=symbols["aleph"]=symbols["Aleph"]=symbols["Alef"]=SymbolUnicode(QChar(0x2135)).addHtml("&aleph;").addWinSymbol(QChar(0xC0));
    symbols["angle"]=UprightSymbolUnicode(QChar(0x2220)).addHtml("&angle;").addWinSymbol(QChar(0xD0));
    symbols["backslash"]=SimpleTextSymbol("\\");
    symbols["benzene"]=UprightSymbolUnicode(QChar(0x232C));
    symbols["benzenr"]=UprightSymbolUnicode(QChar(0x23E3));
    symbols["blacksquare"]=UprightSymbolUnicode(QChar(0x220E)).addHtml("&squ;");
    symbols["blcorner"]=symbols["llcorner"]=UprightSymbolUnicode(QChar(0x231E));
    symbols["brcorner"]=symbols["lrcorner"]=UprightSymbolUnicode(QChar(0x231F));
    symbols["bullet"]=UprightSymbolUnicode(QChar(0x2022)).addUprightHtml("&bull;").addUprightWinSymbol(QChar(0xB7));
    symbols["cdots"]=SymbolUnicode(QChar(0x22EF)).addHtml("&middot;&middot;&middot;").addStd(QString(3, QChar(0xB7)));
    symbols["celsius"]=symbols["degC"]=SymbolUnicode(QChar(0x2103)).addUprightStd("°C").addUprightHtml("&deg;C");
    symbols["cent"]=SimpleTextSymbol(QChar(0xA2), "&cent;");
    symbols["circ"]=UprightSymbolStd(QChar(0x2218));
    symbols["co"]=UprightSymbolUnicode(QChar(0x2105));
    symbols["copyright"]=symbols["textcopyright"]=SimpleTextSymbol(QChar(0xA5), "&copy;");
    symbols["dalet"]=symbols["Dalet"]=symbols["daleth"]=symbols["Daleth"]=SymbolUnicode(QChar(0x2138)).addHtml("&daleth;");
    symbols["dd"] = SymbolFullProps(SymbolProps("d", Upright|BoldOff), "d", Upright|BoldOff);
    symbols["diamond"]=UprightSymbolUnicode(QChar(0x22C4)).addHtml("&diam;").addWinSymbol(QChar(0xE0));
    symbols["dollar"]=SymbolUnicode(QChar(0x0024));
    symbols["dollar"]=symbols["$"]=SimpleTextSymbol(QChar(0x24), "&dollar;");
    symbols["dprime"]=SymbolUnicode(QChar(0x2033)).addHtml("&Prime;").addStd("''");
    symbols["ellipsis"]=symbols["dots"]=symbols["ldots"]=SymbolUnicode(QChar(0x2026)).addHtml("&mldr;").addWinSymbol(QChar(0xBC)).addStd("...");
    symbols["euro"]=symbols["EUR"]=SimpleTextSymbol(QChar(0x20AC), "&euro;");
    symbols["fahrenheit"]=symbols["degF"]=SymbolUnicode(QChar(0x2109));
    symbols["female"]=SymbolUnicode(QChar(0x2640)).addHtml("&female;");
    symbols["flq"]=SymbolUnicode(QChar(0x2039)).addHtml("&lsaquo;").addStd("<");
    symbols["flqq"]=SymbolUnicode(QChar(0x00AB)).addHtml("&laquo;").addStd(QChar(0xAB));
    symbols["frown"]=SymbolUnicode(QChar(0x2322)).addHtml("&frown;");
    symbols["frq"]=SymbolUnicode(QChar(0x203A)).addHtml("&rsaquo;").addStd(">");
    symbols["frqq"]=SymbolUnicode(QChar(0x00BB)).addHtml("&raquo;").addStd(QChar(0xBB));
    symbols["gimel"]=symbols["Gimel"]=SymbolUnicode(QChar(0x2137)).addHtml("&gimel;");
    symbols["glq"]=SymbolUnicode(QChar(0x2018)).addHtml("&OpenCurlyQuote;").addStd("'");
    symbols["glqq"]=SymbolUnicode(QChar(0x201C)).addHtml("&OpenCurlyDoubleQuote;").addStd("\"");
    symbols["grq"]=SymbolUnicode(QChar(0x2019)).addHtml("&CloseCurlyQuote;").addStd("'");
    symbols["grqq"]=SymbolUnicode(QChar(0x201D)).addHtml("&CloseCurlyDoubleQuote;").addStd("\"");
    symbols["hbar"]=SymbolUnicode(QChar(0x210F)).addHtml("&hbar;").addStd("h", DrawBar);
    symbols["hexagon"]=UprightSymbolUnicode(QChar(0x2394));
    symbols["ii"] = SymbolFullProps(SymbolProps("i", Upright|BoldOff), "i", Upright|BoldOff);
    symbols["infty"]=SymbolUnicode(QChar(0x221E)).addHtml("&infin;").addWinSymbol(QChar(0xA5)).addStd("8", Rotate90);
    symbols["langle"]=UprightSymbolUnicode(QChar(0x2329)).addWinSymbol(QChar(0xE1));
    symbols["lceil"]=UprightSymbolUnicode(QChar(0x2308)).addHtml("&LeftCeiling;").addWinSymbol(QChar(0xE9));
    symbols["ldots"]=symbols["dots"]=SimpleTextSymbol("...");
    symbols["lfloor"]=UprightSymbolUnicode(QChar(0x230A)).addHtml("&LeftFloor;").addWinSymbol(QChar(0xEB));
    symbols["lightning"]=SymbolUnicode(QChar(0x21AF));
    symbols["male"]=SymbolUnicode(QChar(0x2642)).addHtml("&male;");
    symbols["measuredangle"]=UprightSymbolUnicode(QChar(0x2221)).addHtml("&measuredangle;");
    symbols["micro"] = SimpleTextSymbol(QChar(0xB5), "&micro;");
    symbols["ohm"]=SymbolUnicode(QChar(0x2126));
    symbols["partial"]=UprightSymbolUnicode(QChar(0x2202)).addHtml("&part;").addWinSymbol(QChar(0xB6));
    symbols["perthousand"]=symbols["permil"]=SymbolUnicode(QChar(0x2030)).addHtml("&permil;");
    symbols["pound"]=SimpleTextSymbol(QChar(0xA3), "&pound;");
    symbols["pound"]=SymbolUnicode(QChar(0x00A3));
    symbols["prime"]=SymbolUnicode(QChar(0x2032)).addHtml("&prime;").addStd("'");
    symbols["rangle"]=UprightSymbolUnicode(QChar(0x232A)).addWinSymbol(QChar(0xF1));
    symbols["rceil"]=UprightSymbolUnicode(QChar(0x2309)).addHtml("&RightCeiling;").addWinSymbol(QChar(0xF9));
    symbols["registered"]=symbols["textregistered"]=symbols["circledR"]=SimpleTextSymbol(QChar(0xAA), "&reg;");
    symbols["rfloor"]=UprightSymbolUnicode(QChar(0x230B)).addHtml("&RightFloor;").addWinSymbol(QChar(0xFB));
    symbols["rightangle"]=UprightSymbolUnicode(QChar(0x221F)).addHtml("&angrt;");
    symbols["smile"]=UprightSymbolUnicode(QChar(0x2323)).addHtml("&smile;");
    symbols["sphericalangle"]=UprightSymbolUnicode(QChar(0x2222)).addHtml("&angsph;");
    symbols["star"]=UprightSymbolUnicode(QChar(0x22C6));
    symbols["tcohm"]=SymbolUnicode(QChar(0x2126));
    symbols["textbar"]=SimpleTextSymbol("|", "&VerticalLine;");
    symbols["textdegree"]=symbols["degree"] = SimpleTextSymbol(QChar(0xB0), "&deg;");
    symbols["textgreater"]=SimpleTextSymbol(">", "&gt;");
    symbols["textless"]=SimpleTextSymbol("<", "&lt;");
    symbols["tlcorner"]=symbols["ulcorner"]=SymbolUnicode(QChar(0x231C));
    symbols["trademark"]=SymbolUnicode(QChar(0x2122)).addHtml("&trade;").addWinSymbol(QChar(0xD4)).addStd("(TM)");
    symbols["trapezium"]=SymbolUnicode(QChar(0x23E2)).addHtml("&trpezium;");
    symbols["trcorner"]=symbols["urcorner"]=UprightSymbolUnicode(QChar(0x231D));
    symbols["trprime"]=SymbolUnicode(QChar(0x2034)).addHtml("&tprime;").addStd("'''");
    symbols["varcarriagereturn"]=SymbolUnicode(QChar(0x23CE)).addWinSymbol(QChar(0xBF));
    symbols["varhexagonlrbonds"]=SymbolUnicode(QChar(0x232C));
    symbols["yen"]=SimpleTextSymbol(QChar(0xA5), "&yen;");
    symbols["yen"]=SymbolUnicode(QChar(0x00A5));





    /**************************************************************************************
     * STANDARD MathOperator Strings
     **************************************************************************************/
    symbols["Pr"] = MathOperatorText("Pr");
    symbols["acos"] = MathOperatorText("acos");
    symbols["arccos"] = MathOperatorText("arccos");
    symbols["arcsin"] = MathOperatorText("arcsin");
    symbols["arctan"] = MathOperatorText("arctan");
    symbols["arg"] = MathOperatorText("arg");
    symbols["argmax"] = MathOperatorText("arg max", "arg&thinsp;max");
    symbols["argmin"] = MathOperatorText("arg min", "arg&thinsp;min");
    symbols["asin"] = MathOperatorText("asin");
    symbols["atan"] = MathOperatorText("atan");
    symbols["cos"] = MathOperatorText("cos");
    symbols["cosh"] = MathOperatorText("cosh");
    symbols["cot"] = MathOperatorText("cot");
    symbols["coth"] = MathOperatorText("coth");
    symbols["coth"] = MathOperatorText("coth");
    symbols["deg"] = MathOperatorText("deg");
    symbols["det"] = MathOperatorText("det");
    symbols["dim"] = MathOperatorText("dim");
    symbols["exp"] = MathOperatorText("exp");
    symbols["gcd"] = MathOperatorText("gcd");
    symbols["hom"] = MathOperatorText("hom");
    symbols["ker"] = MathOperatorText("ker");
    symbols["lb"] = MathOperatorText("lb");
    symbols["ld"] = MathOperatorText("ld");
    symbols["lim"] = MathOperatorText("lim");
    symbols["liminf"] = MathOperatorText("lim inf", "lim&thinsp;inf");
    symbols["limsup"] = MathOperatorText("lim sup", "lim&thinsp;sup");
    symbols["ln"] = MathOperatorText("ln");
    symbols["log"] = MathOperatorText("log");
    symbols["max"] = MathOperatorText("max");
    symbols["median"] = MathOperatorText("median");
    symbols["min"] = MathOperatorText("min");
    symbols["mod"] = MathOperatorText("mod");
    symbols["sec"] = MathOperatorText("sec");
    symbols["sgn"] = MathOperatorText("sgn");
    symbols["sign"] = MathOperatorText("sign");
    symbols["sin"] = MathOperatorText("sin");
    symbols["sinh"] = MathOperatorText("sinh");
    symbols["tan"] = MathOperatorText("tan");
    symbols["tanh"] = MathOperatorText("tanh");

    /**************************************************************************************
     * STANDARD MathOperator Symbols
     **************************************************************************************/
    symbols["*"]=symbols["ast"]=symbols["asterisk"]=MathOperatorSymbolUnicode(QChar(0x2217)).addMathOperatorStd("*").addMathOperatorHtml("*");
    symbols["+"]=MathOperatorSymbolUnicode(QChar(0x2B)).addMathOperatorHtml("+").addMathOperatorStd("+");
    symbols["-"]=MathOperatorSymbolUnicode(QChar(0x2212)).addMathOperatorHtml("-").addMathOperatorStd("-");
    symbols["<"]=MathOperatorSymbol("<", "&lt;");
    symbols["="] = MathOperatorSymbol("=");
    symbols[">"]=MathOperatorSymbol(">", "&gt;");
    symbols["Downarrow"]=UprightSymbolUnicode(QChar(0x21D3)).addUprightHtml("&DoubleDownArrow;").addUprightWinSymbol(QChar(0xDF));
    symbols["Leftarrow"]=UprightSymbolUnicode(QChar(0x21D0)).addUprightHtml("&DoubleLeftArrow;").addUprightWinSymbol(QChar(0xDC));
    symbols["Rightarrow"]=UprightSymbolUnicode(QChar(0x21D2)).addUprightHtml("&Rightarrow;").addUprightWinSymbol(QChar(0xDE));
    symbols["Uparrow"]=UprightSymbolUnicode(QChar(0x21D1)).addUprightHtml("&DoubleUpArrow;").addUprightWinSymbol(QChar(0xDD));
    symbols["Updownarrow"]=UprightSymbolUnicode(QChar(0x21D5)).addUprightHtml("&Updownarrow;");
    symbols["approx"]=MathOperatorSymbolUnicode(QChar(0x2248)).addMathOperatorHtml("&asymp;").addMathOperatorWinSymbol(QChar(0xBB));
    symbols["bbC"]=SymbolUnicode(QChar(0x2102));
    symbols["bbH"]=MathOperatorSymbolUnicode(QChar(0x210D));
    symbols["bbN"]=MathOperatorSymbolUnicode(QChar(0x2115));
    symbols["bbP"]=MathOperatorSymbolUnicode(QChar(0x2119));
    symbols["bbQ"]=MathOperatorSymbolUnicode(QChar(0x211A));
    symbols["bbR"]=MathOperatorSymbolUnicode(QChar(0x211D));
    symbols["bbZ"]=MathOperatorSymbolUnicode(QChar(0x2124));
    symbols["because"]=MathOperatorSymbolUnicode(QChar(0x2235)).addMathOperatorHtml("&because;");
    symbols["bigcap"]=MathOperatorSymbolUnicode(QChar(0x22C2)).addMathOperatorHtml("&int;").addMathOperatorWinSymbol(QChar(0xC7), 1.8);
    symbols["bigcup"]=MathOperatorSymbolUnicode(QChar(0x22C3)).addMathOperatorHtml("&int;").addMathOperatorWinSymbol(QChar(0xC8), 1.8);
    symbols["bighat"]=MathOperatorSymbolUnicode(QChar(0x22C0)).addMathOperatorHtml("&int;").addMathOperatorWinSymbol(QChar(0xD9), 1.8);
    symbols["bigvee"]=MathOperatorSymbolUnicode(QChar(0x22C1)).addMathOperatorHtml("&int;").addMathOperatorWinSymbol(QChar(0xDA), 1.8);
    symbols["bot"]=symbols["perp"]=MathOperatorSymbolUnicode(QChar(0x22A5)).addMathOperatorHtml("&UpTee;");
    symbols["cap"]=symbols["land"]=MathOperatorSymbolUnicode(QChar(0x2229)).addMathOperatorHtml("&cap;").addMathOperatorWinSymbol(QChar(0xC7));
    symbols["cdot"]=MathOperatorSymbol(QChar(0xB7)).addMathOperatorHtml("&middot;").addMathOperatorWinSymbol(QChar(0xD7));
    symbols["cong"]=MathOperatorSymbolUnicode(QChar(0x2245)).addMathOperatorHtml("&TildeFullEqual;");
    symbols["coprod"]=MathOperatorSymbolUnicode(QChar(0x2210)).addMathOperatorHtml("&Coproduct;").addWinSymbol(QChar(0xD5), ItalicOff|BoldOff|FlipUpDown, 1.8, 0.1);
    symbols["cup"]=symbols["lor"]=MathOperatorSymbolUnicode(QChar(0x222A)).addMathOperatorHtml("&cup;").addMathOperatorWinSymbol(QChar(0xC8));
    symbols["ddots"]=MathOperatorSymbolUnicode(QChar(0x22F1)).addMathOperatorHtml("&dtdot;");
    symbols["div"]=MathOperatorSymbolUnicode(QChar(0x00F7)).addMathOperatorHtml("&div;").addMathOperatorWinSymbol(QChar(0xB8));
    symbols["downarrow"]=UprightSymbolUnicode(QChar(0x2193)).addUprightHtml("&ShortDownArrow;").addUprightWinSymbol(QChar(0xAF));
    symbols["downharpoonleft"]=UprightSymbolUnicode(QChar(0x21C3)).addUprightHtml("&LeftDownVector;");
    symbols["downharpoonright"]=UprightSymbolUnicode(QChar(0x21C2)).addUprightHtml("&RightDownVector;");
    symbols["emptyset"]=symbols["varnothing"]=MathOperatorSymbolUnicode(QChar(0x2205)).addMathOperatorHtml("&varnothing;").addMathOperatorWinSymbol(QChar(0xC6)).addStd("0", BoldOff|ItalicOff|DrawSlash);
    symbols["equiv"]=MathOperatorSymbolUnicode(QChar(0x2261)).addMathOperatorHtml("&equiv;").addMathOperatorWinSymbol(QChar(0xBA));
    symbols["exists"]=MathOperatorSymbolUnicode(QChar(0x2203)).addMathOperatorHtml("&exist;").addMathOperatorWinSymbol(QChar(0x24)).addStd("E", ItalicOff|BoldOff|FlipLeftRight);
    symbols["forall"]=MathOperatorSymbolUnicode(QChar(0x2200)).addMathOperatorHtml("&forall;").addMathOperatorWinSymbol(QChar(0x22)).addStd("A", ItalicOff|BoldOff|FlipUpDown);
    symbols["geq"]=symbols["ge"]=MathOperatorSymbolUnicode(QChar(0x2265)).addMathOperatorHtml("&GreaterEqual;").addMathOperatorWinSymbol(QChar(0xB3));
    symbols["geqq"]=MathOperatorSymbolUnicode(QChar(0x2267)).addMathOperatorHtml("&GreaterFullEqual;");
    symbols["gg"]=MathOperatorSymbolUnicode(QChar(0x226B)).addMathOperatorHtml("&NestedGreaterGreater;").addMathOperatorStd(">>");
    symbols["iddots"]=MathOperatorSymbolUnicode(QChar(0x22F0)).addMathOperatorHtml("&utdot;");
    symbols["iff"]=symbols["Leftrightarrow"]=UprightSymbolUnicode(QChar(0x21D4)).addUprightHtml("&DoubleLeftRightArrow;").addUprightWinSymbol(QChar(0xDB));
    symbols["iiint"]=MathOperatorSymbolUnicode(QChar(0x222D)).setGlobalFlags(IntLikeSymbolCorrection).addMathOperatorHtml("&tint;").addMathOperatorWinSymbol(QString(3, QChar(0xF2)), 1.8, 0.1);
    symbols["iint"]=MathOperatorSymbolUnicode(QChar(0x222C)).setGlobalFlags(IntLikeSymbolCorrection).addMathOperatorHtml("&Int;").addMathOperatorWinSymbol(QString(2, QChar(0xF2)), 1.8, 0.1);
    symbols["in"]=MathOperatorSymbolUnicode(QChar(0x2208)).addMathOperatorHtml("&isin;").addMathOperatorWinSymbol(QChar(0xCE));
    symbols["int"]=MathOperatorSymbolUnicode(QChar(0x222B)).setGlobalFlags(IntLikeSymbolCorrection).addMathOperatorHtml("&int;").addMathOperatorWinSymbol(QChar(0xF2), 1.8, 0.1);
    symbols["leftarrow"]=UprightSymbolUnicode(QChar(0x2190)).addUprightHtml("&larr;").addUprightWinSymbol(QChar(0xAC));
    symbols["leftharpoondown"]=UprightSymbolUnicode(QChar(0x21BD)).addUprightHtml("&leftharpoondown;");
    symbols["leftharpoonup"]=UprightSymbolUnicode(QChar(0x21BC)).addUprightHtml("&LeftVector;");
    symbols["leftrightarrow"]=UprightSymbolUnicode(QChar(0x2194)).addUprightHtml("&harr;").addUprightWinSymbol(QChar(0xAB));
    symbols["leftrightharpoon"]=UprightSymbolUnicode(QChar(0x21CB)).addUprightHtml("&ReverseEquilibrium;");
    symbols["leq"]=symbols["le"]=MathOperatorSymbolUnicode(QChar(0x2264)).addMathOperatorHtml("&leq;").addMathOperatorWinSymbol(QChar(0xA3));
    symbols["leqq"]=MathOperatorSymbolUnicode(QChar(0x2266)).addMathOperatorHtml("&LessFullEqual;");
    symbols["ll"]=MathOperatorSymbolUnicode(QChar(0x226A)).addMathOperatorHtml("&NestedLessLess;").addMathOperatorStd("<<");
    symbols["lnot"]=MathOperatorSymbolUnicode(QChar(0xAC)).addMathOperatorWinSymbol(QChar(0xD8)).addMathOperatorHtml("&not;");
    symbols["mapimage"]=MathOperatorSymbolUnicode(QChar(0x22B7)).addMathOperatorHtml("&imof;");
    symbols["maporiginal"]=MathOperatorSymbolUnicode(QChar(0x22B6)).addMathOperatorHtml("&origof;");
    symbols["mapsto"]=MathOperatorSymbolUnicode(QChar(0x21A6)).addMathOperatorHtml("&RightTeeArrow;");
    symbols["mid"]=MathOperatorSymbolUnicode(QChar(0x2223)).addMathOperatorHtml("&VerticalBar;").addMathOperatorWinSymbol(QChar(0xBD)).addMathOperatorStd("|");
    symbols["mp"]=MathOperatorSymbolUnicode(QChar(0x2213)).addMathOperatorHtml("&mp;");
    symbols["multimap"]=MathOperatorSymbolUnicode(QChar(0x22B8)).addMathOperatorHtml("&mumap;");
    symbols["nabla"]=MathOperatorSymbolUnicode(QChar(0x2207)).addMathOperatorHtml("&nabla;").addMathOperatorWinSymbol(QChar(0xD1));
    symbols["ne"]=MathOperatorSymbolUnicode(QChar(0x2260)).addMathOperatorHtml("&ne;").addMathOperatorWinSymbol(QChar(0xB9));
    symbols["nearrow"]=UprightSymbolUnicode(QChar(0x2197)).addUprightHtml("&UpperRightArrow;");
    symbols["neg"]=symbols["lnot"]=MathOperatorSymbolUnicode(QChar(0x00AC)).addMathOperatorHtml("&not;").addMathOperatorWinSymbol(QChar(0xD8));
    symbols["neq"]=MathOperatorSymbolUnicode(QChar(0x2260)).addMathOperatorHtml("&ne;").addMathOperatorWinSymbol(QChar(0xB9)).addStd("=", ItalicOff|BoldOff|DrawSlash);
    symbols["nexists"]=MathOperatorSymbolUnicode(QChar(0x2204)).addMathOperatorHtml("&NotExists;").addStd("E", ItalicOff|BoldOff|FlipLeftRight|DrawSlash).addMathOperatorWinSymbol(QChar(0x24), ItalicOff|BoldOff|DrawSlash);
    symbols["ni"]=MathOperatorSymbolUnicode(QChar(0x220B)).addMathOperatorHtml("&ReverseElement;").addMathOperatorWinSymbol(QChar(0xCE), ItalicOff|BoldOff|FlipLeftRight);
    symbols["nmid"]=MathOperatorSymbolUnicode(QChar(0x2224)).addMathOperatorHtml("&NotVerticalBar;");
    symbols["notin"]=MathOperatorSymbolUnicode(QChar(0x2209)).addMathOperatorHtml("&notin;").addMathOperatorWinSymbol(QChar(0xCF));
    symbols["notni"]=MathOperatorSymbolUnicode(QChar(0x220C)).addMathOperatorHtml("&NotReverseElement;");
    symbols["nparallel"]=MathOperatorSymbolUnicode(QChar(0x2226)).addMathOperatorHtml("&NotDoubleVerticalBar;");
    symbols["nwarrow"]=UprightSymbolUnicode(QChar(0x2196)).addUprightHtml("&UpperLeftArrow;");
    symbols["odot"]=MathOperatorSymbolUnicode(QChar(0x2299)).addMathOperatorHtml("&odot;");
    symbols["oiiint"]=MathOperatorSymbolUnicode(QChar(0x2230)).setGlobalFlags(IntLikeSymbolCorrection).addMathOperatorHtml("&Cconint;");
    symbols["oiint"]=MathOperatorSymbolUnicode(QChar(0x222F)).setGlobalFlags(IntLikeSymbolCorrection).addMathOperatorHtml("&DoubleContourIntegral;");
    symbols["oint"]=MathOperatorSymbolUnicode(QChar(0x222E)).setGlobalFlags(IntLikeSymbolCorrection).addMathOperatorHtml("&ContourIntegral;");
    symbols["ominus"]=MathOperatorSymbolUnicode(QChar(0x2296)).addMathOperatorHtml("&ominus;");
    symbols["oplus"]=MathOperatorSymbolUnicode(QChar(0x2295)).addMathOperatorHtml("&CirclePlus;").addMathOperatorWinSymbol(QChar(0xC5));
    symbols["oslash"]=MathOperatorSymbolUnicode(QChar(0x2298)).addMathOperatorHtml("&osol;");
    symbols["otimes"]=MathOperatorSymbolUnicode(QChar(0x2297)).addMathOperatorHtml("&CircleTimes;").addMathOperatorWinSymbol(QChar(0xC4));
    symbols["parallel"]=MathOperatorSymbolUnicode(QChar(0x2225)).addMathOperatorHtml("&shortparallel;").addMathOperatorStd("||");
    symbols["pm"] = MathOperatorSymbol(QChar(0xB1), "&plusmn;").addMathOperatorWinSymbol(QChar(0xB1));
    symbols["prec"]=MathOperatorSymbolUnicode(QChar(0x227A)).addMathOperatorHtml("&prec;");
    symbols["prod"]=MathOperatorSymbolUnicode(QChar(0x220F)).addMathOperatorWinSymbol(QChar(0xD5), 1.8, 0.1).addMathOperatorHtml("&prod;");
    symbols["propto"]=MathOperatorSymbolUnicode(QChar(0x221D)).addMathOperatorWinSymbol(QChar(0xB5)).addMathOperatorHtml("&Proportional;");
    symbols["rightharpoondown"]=UprightSymbolUnicode(QChar(0x21C1)).addUprightHtml("&rightharpoondown;");
    symbols["rightharpoonup"]=UprightSymbolUnicode(QChar(0x21C0)).addUprightHtml("&RightVector;");
    symbols["rightleftharpoon"]=UprightSymbolUnicode(QChar(0x21CC)).addUprightHtml("&Equilibrium;");
    symbols["searrow"]=UprightSymbolUnicode(QChar(0x2198)).addUprightHtml("&LowerRightArrow;");
    symbols["setminus"]=MathOperatorSymbolUnicode(QChar(0x2216)).addMathOperatorHtml("&smallsetminus;");
    symbols["~"]=symbols["sim"]=symbols["tilde"]=MathOperatorSymbolUnicode(QChar(0x223C)).addMathOperatorHtml("&tilde;").addMathOperatorStd("~");
    symbols["simeq"]=MathOperatorSymbolUnicode(QChar(0x2243)).addMathOperatorHtml("&TildeEqual;");
    symbols["sqcap"]=MathOperatorSymbolUnicode(QChar(0x2293)).addMathOperatorHtml("&SquareIntersection;");
    symbols["sqcup"]=MathOperatorSymbolUnicode(QChar(0x2294)).addMathOperatorHtml("&SquareUnion;");
    symbols["square"]=MathOperatorSymbolUnicode(QChar(0x25A1));
    symbols["subset"]=MathOperatorSymbolUnicode(QChar(0x2282)).addMathOperatorHtml("&sub;").addMathOperatorWinSymbol(QChar(0xCC));
    symbols["subseteq"]=MathOperatorSymbolUnicode(QChar(0x2286)).addMathOperatorHtml("&SubsetEqual;").addMathOperatorWinSymbol(QChar(0xCD));
    symbols["subsetnot"]=MathOperatorSymbolUnicode(QChar(0x2284)).addMathOperatorHtml("&nsub;").addMathOperatorWinSymbol(QChar(0xCB));
    symbols["succ"]=MathOperatorSymbolUnicode(QChar(0x227B)).addMathOperatorHtml("&succ;");
    symbols["sum"]=MathOperatorSymbolUnicode(QChar(0x2211)).addMathOperatorWinSymbol(QChar(0xE5), 1.8, 0.1).addMathOperatorHtml("&sum;");
    symbols["supset"]=MathOperatorSymbolUnicode(QChar(0x2283)).addMathOperatorHtml("&sup;").addMathOperatorWinSymbol(QChar(0xC9));
    symbols["supseteq"]=MathOperatorSymbolUnicode(QChar(0x2287)).addMathOperatorHtml("&SupersetEqual;").addMathOperatorWinSymbol(QChar(0xCA));
    symbols["supsetnot"]=MathOperatorSymbolUnicode(QChar(0x2285)).addMathOperatorHtml("&nsup;");
    symbols["swarrow"]=UprightSymbolUnicode(QChar(0x2199)).addUprightHtml("&LowerLeftArrow;");
    symbols["therefore"]=MathOperatorSymbolUnicode(QChar(0x2234)).addMathOperatorHtml("&therefore;").addMathOperatorWinSymbol(QChar(0x5C));
    symbols["times"] = MathOperatorSymbol(QChar(0xD7), "&times;").addMathOperatorWinSymbol(QChar(0xB4));
    symbols["to"]=symbols["rightarrow"]=UprightSymbolUnicode(QChar(0x2192)).addUprightHtml("&rarr;").addUprightWinSymbol(QChar(0xAE));
    symbols["top"]=MathOperatorSymbolUnicode(QChar(0x22A4)).addMathOperatorHtml("&top;").addMathOperatorWinSymbol(QChar(0x5E));
    symbols["triangle"]=MathOperatorSymbolUnicode(QChar(0x2206));
    symbols["uparrow"]=UprightSymbolUnicode(QChar(0x2191)).addUprightHtml("&ShortUpArrow;").addUprightWinSymbol(QChar(0xAD));
    symbols["updownarrow"]=UprightSymbolUnicode(QChar(0x2195)).addUprightHtml("&updownarrow;");
    symbols["upharpoonleft"]=UprightSymbolUnicode(QChar(0x21BF)).addUprightHtml("&LeftUpVector;");
    symbols["upharpoonright"]=UprightSymbolUnicode(QChar(0x21BE)).addUprightHtml("&RightUpVector;");
    symbols["vartriangleleft"]=MathOperatorSymbolUnicode(QChar(0x22B2)).addMathOperatorHtml("&LeftTriangle;");
    symbols["vdots"]=MathOperatorSymbolUnicode(QChar(0x22EE)).addMathOperatorHtml("&vellip;");
    symbols["vee"]=MathOperatorSymbolUnicode(QChar(0x2228)).addMathOperatorHtml("&vee;").addMathOperatorWinSymbol(QChar(0xDA));
    symbols["wedge"]=MathOperatorSymbolUnicode(QChar(0x2227)).addMathOperatorHtml("&wedge;").addMathOperatorWinSymbol(QChar(0xD9));



    /**************************************************************************************
     * GREEK letters
     **************************************************************************************/
    symbols["alpha"]=GreekLetter_WinSymbol_Unicode_Html( "a", QChar(0x3B1), "&alpha;");
    symbols["beta"]=GreekLetter_WinSymbol_Unicode_Html("b", QChar(0x3B2), "&beta;");
    symbols["gamma"]=GreekLetter_WinSymbol_Unicode_Html("g", QChar(0x3B3), "&gamma;");
    symbols["delta"]=GreekLetter_WinSymbol_Unicode_Html("d", QChar(0x3B4), "&delta;");
    symbols["epsilon"]=GreekLetter_WinSymbol_Unicode_Html("e", QChar(0x3F5), "&varepsilon;");
    symbols["varepsilon"]=GreekLetter_WinSymbol_Unicode_Html("e", QChar(0x3B5), "&epsi;");
    symbols["zeta"]=GreekLetter_WinSymbol_Unicode_Html("z", QChar(0x3B6),"&zeta;");
    symbols["eta"]=GreekLetter_WinSymbol_Unicode_Html("h", QChar(0x3B7),"&eta;");
    symbols["theta"]=GreekLetter_WinSymbol_Unicode_Html("q", QChar(0x3B8),"&theta;");
    symbols["vartheta"]=GreekLetter_WinSymbol_Unicode_Html("J", QChar(0x3D1),"&thetasym;");
    symbols["iota"]=GreekLetter_WinSymbol_Unicode_Html("i", QChar(0x3B9),"&iota;");
    symbols["kappa"]=GreekLetter_WinSymbol_Unicode_Html("k", QChar(0x3BA),"&kappa;");
    symbols["lambda"]=GreekLetter_WinSymbol_Unicode_Html("l", QChar(0x3BB),"&lambda;");
    symbols["mu"]=GreekLetter_WinSymbol_Unicode_Html("m", QChar(0x3BC),"&mu;");
    symbols["nu"]=GreekLetter_WinSymbol_Unicode_Html("n", QChar(0x3BD),"&nu;");
    symbols["xi"]=GreekLetter_WinSymbol_Unicode_Html("x", QChar(0x3BE),"&xi;");
    symbols["pi"]=GreekLetter_WinSymbol_Unicode_Html("p", QChar(0x3C0),"&pi;");
    symbols["varpi"]=GreekLetter_WinSymbol_Unicode_Html("v", QChar(0x3D6),"&piv;");
    symbols["rho"]=GreekLetter_WinSymbol_Unicode_Html("r", QChar(0x3C1),"&rho;");
    symbols["varrho"]=GreekLetter_WinSymbol_Unicode_Html("r", QChar(0x3F1),"&varrho;");
    symbols["sigma"]=GreekLetter_WinSymbol_Unicode_Html("s", QChar(0x3C3),"&sigma;");
    symbols["varsigma"]=GreekLetter_WinSymbol_Unicode_Html("V", QChar(0x3C2),"&varsigma;");
    symbols["tau"]=GreekLetter_WinSymbol_Unicode_Html("t", QChar(0x3C4),"&tau;");
    symbols["upsilon"]=GreekLetter_WinSymbol_Unicode_Html("u", QChar(0x3C5),"&upsilon;");
    symbols["phi"]=GreekLetter_WinSymbol_Unicode_Html("f", QChar(0x3D5),"&straightphi;");
    symbols["varphi"]=GreekLetter_WinSymbol_Unicode_Html("j", QChar(0x3C6),"&phi;");
    symbols["chi"]=GreekLetter_WinSymbol_Unicode_Html("c", QChar(0x3C7),"&chi;");
    symbols["psi"]=GreekLetter_WinSymbol_Unicode_Html("y", QChar(0x3C8),"&psi;");
    symbols["omega"]=GreekLetter_WinSymbol_Unicode_Html("w", QChar(0x3C9),"&omega;");
    symbols["Gamma"]=GreekLetter_WinSymbol_Unicode_Html("G", QChar(0x3A9),"&Gamma;");
    symbols["Delta"]=GreekLetter_WinSymbol_Unicode_Html("D", QChar(0x394),"&Delta;");
    symbols["Theta"]=GreekLetter_WinSymbol_Unicode_Html("Q", QChar(0x398),"&Theta;");
    symbols["Lambda"]=GreekLetter_WinSymbol_Unicode_Html("L", QChar(0x39B),"&Lambda;");
    symbols["Omega"]=GreekLetter_WinSymbol_Unicode_Html("W", QChar(0x3A9),"&Omega;");
    symbols["Xi"]=GreekLetter_WinSymbol_Unicode_Html("X", QChar(0x39E),"&Xi;");
    symbols["Pi"]=GreekLetter_WinSymbol_Unicode_Html("P", QChar(0x3A0),"&Pi;");
    symbols["Sigma"]=GreekLetter_WinSymbol_Unicode_Html("S", QChar(0x3A3),"&Sigma;");
    symbols["Upsilon"]=GreekLetter_WinSymbol_Unicode_Html("U", QChar(0x3C6),"&Upsilon;");
    symbols["Phi"]=GreekLetter_WinSymbol_Unicode_Html("F", QChar(0x3A6),"&Phi;");
    symbols["Psi"]=GreekLetter_WinSymbol_Unicode_Html("Y", QChar(0x3A8),"&Psi;");



}


JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps():
    customFontFamily(),
    html(),
    fontType(MTECurrentFont),
    globalFlags(NoGLobalSymbolFlags)
{

}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(const QString &_font, const SymbolProps &props, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    customFontFamily(_font),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    fontType(MTECustomFont),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[MTFEStandard]=props;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(const QString &_font, const QString &symbol, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    customFontFamily(_font),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    fontType(MTECustomFont),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[MTFEStandard]=SymbolProps(symbol);
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(const SymbolProps &props, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    fontType(MTECurrentFont),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[MTFEStandard]=props;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, const SymbolProps &props, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    fontType(_fontType),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[MTFEStandard]=props;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, const QString &symbol, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    fontType(_fontType),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[MTFEStandard]=SymbolProps(symbol);
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextFontEncoding enc0, const SymbolProps &props0, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    fontType(MTECurrentFont),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[enc0]=props0;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, JKQTMathTextFontEncoding enc0, const SymbolProps &props0, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    fontType(_fontType),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[enc0]=props0;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextFontEncoding enc0, const SymbolProps &props0, JKQTMathTextFontEncoding enc1, const SymbolProps &props1, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    fontType(MTECurrentFont),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[enc0]=props0;
    this->props[enc1]=props1;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, JKQTMathTextFontEncoding enc0, const SymbolProps &props0, JKQTMathTextFontEncoding enc1, const SymbolProps &props1, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    fontType(_fontType),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[enc0]=props0;
    this->props[enc1]=props1;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextFontEncoding enc0, const SymbolProps &props0, JKQTMathTextFontEncoding enc1, const SymbolProps &props1, JKQTMathTextFontEncoding enc2, const SymbolProps &props2, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    fontType(MTECurrentFont),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[enc0]=props0;
    this->props[enc1]=props1;
    this->props[enc2]=props2;
}

JKQTMathTextSymbolNode::SymbolFullProps::SymbolFullProps(JKQTMathTextEnvironmentFont _fontType, JKQTMathTextFontEncoding enc0, const SymbolProps &props0, JKQTMathTextFontEncoding enc1, const SymbolProps &props1, JKQTMathTextFontEncoding enc2, const SymbolProps &props2, const QString &_html, SymbolFlags _htmlflags, double _htmlfontScalingFactor, double _htmlyShiftFactor):
    customFontFamily(),
    html(_html, _htmlflags, _htmlfontScalingFactor, _htmlyShiftFactor),
    fontType(_fontType),
    globalFlags(NoGLobalSymbolFlags)
{
    this->props[enc0]=props0;
    this->props[enc1]=props1;
    this->props[enc2]=props2;
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

QPair<QFont, JKQTMathTextSymbolNode::SymbolProps> JKQTMathTextSymbolNode::SymbolFullProps::getDrawingData(JKQTMathTextEnvironment currentEv, JKQTMathText* parent) const
{
    SymbolProps outProps;
    QFont outFont;
    const JKQTMathTextFontEncoding currentEnc=currentEv.getFontEncoding(parent);
    QFont currentFont=currentEv.getFont(parent);
    currentFont.setStyleStrategy(QFont::NoFontMerging);
    const QFontMetricsF currentFM(currentFont);
    QFont fallbackGreekFont=parent->getFallbackFontGreek();
    fallbackGreekFont.setStyleStrategy(QFont::NoFontMerging);
    const QFontMetricsF fallbackGreekFM(fallbackGreekFont);
    QFont fallbackSymbolsFont=parent->getFallbackFontSymbols();
    fallbackSymbolsFont.setStyleStrategy(QFont::NoFontMerging);
    const QFontMetricsF fallbackSymbolsFM(fallbackSymbolsFont);

    if (fontType==MTEFallbackGreek) {
        if (currentEnc!=MTFEStandard && props.contains(currentEnc)) {
            outProps=props[currentEnc];
            const QChar ch=props[currentEnc].getSymbolSingleChar();
            if (!ch.isNull() && !currentFM.inFont(ch)) {
                currentEv.font=MTEFallbackGreek;
                outProps=props.value(currentEv.getFontEncoding(parent), outProps);
            }
        } else if (currentEnc==MTFEStandard && props.contains(currentEnc)){
            outProps=props[currentEnc];
            const QChar ch=props[currentEnc].getSymbolSingleChar();
            if (!ch.isNull() && fallbackGreekFM.inFont(ch)) {
                currentEv.font=MTEFallbackGreek;
                outProps=props.value(currentEv.getFontEncoding(parent), outProps);
            }
        } else {
            currentEv.font=MTEFallbackGreek;
        }
    } else if (fontType==MTEFallbackSymbols) {
        if (currentEnc!=MTFEStandard && props.contains(currentEnc)) {
            outProps=props[currentEnc];
            const QChar ch=props[currentEnc].getSymbolSingleChar();
            if (!ch.isNull() && !currentFM.inFont(ch)) {
                currentEv.font=MTEFallbackGreek;
                outProps=props.value(currentEv.getFontEncoding(parent), outProps);
            }
        } else if (currentEnc==MTFEStandard && props.contains(currentEnc)){
            outProps=props[currentEnc];
            const QChar ch=props[currentEnc].getSymbolSingleChar();
            if (!ch.isNull() && fallbackSymbolsFM.inFont(ch)) {
                currentEv.font=MTEFallbackSymbols;
                outProps=props.value(currentEv.getFontEncoding(parent), outProps);
            }
        } else {
            currentEv.font=MTEFallbackGreek;
        }
    } else if (fontType!=MTEEnvironmentFontCount) {
        if (fontType!=MTECurrentFont) {
            currentEv.font=fontType;
        }
        if (fontType==MTECustomFont) {
            currentEv.customFontName=customFontFamily;
            outProps=props.value(MTFEStandard, outProps);
        } else {
            QFont lcurrentFont=currentEv.getFont(parent);
            lcurrentFont.setStyleStrategy(QFont::NoFontMerging);
            const QFontMetricsF lcurrentFM(lcurrentFont);
            auto lcurrentEnc=currentEv.getFontEncoding(parent);
            outProps=props.value(lcurrentEnc, props.value(MTFEStandard, outProps));
            const QChar ch=outProps.getSymbolSingleChar();
            if (!ch.isNull() && !lcurrentFM.inFont(ch) && fallbackSymbolsFM.inFont(ch)) {
                currentEv.font=MTEFallbackSymbols;
                outProps=props.value(currentEv.getFontEncoding(parent), outProps);
            }
        }
        if (props.contains(currentEv.getFontEncoding(parent))) outProps=props[currentEv.getFontEncoding(parent)];
    }
    outFont=currentEv.getFont(parent);
    if (outProps.fontScalingFactor!=1) outFont.setPointSizeF(outFont.pointSizeF()*outProps.fontScalingFactor);
    if (has(outProps.flags, ItalicOn)) outFont.setItalic(true);
    if (has(outProps.flags, ItalicOff)) outFont.setItalic(false);
    if (has(outProps.flags, BoldOn)) outFont.setBold(true);
    if (has(outProps.flags, BoldOff)) outFont.setBold(false);
    return QPair<QFont, SymbolProps>(outFont, outProps);
}
