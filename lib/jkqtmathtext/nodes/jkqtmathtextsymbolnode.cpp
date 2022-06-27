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
}

JKQTMathTextSymbolNode::~JKQTMathTextSymbolNode() = default;

QString JKQTMathTextSymbolNode::getTypeName() const
{
    return QLatin1String("JKQTMathTextSymbolNode(")+symbolName+QLatin1String(")");
}

bool JKQTMathTextSymbolNode::getWinSymbolProp(JKQTMathTextSymbolNode::SymbolProps& props, const QString &n, const JKQTMathTextEnvironment& currentEv, double mathFontFactor) const
{
    auto fnt=parentMathText->getFontData(currentEv.font, currentEv.insideMath, JKQTMathText::FontSubclass::Text);
    auto fntSym=parentMathText->getFontData(currentEv.font, currentEv.insideMath, JKQTMathText::FontSubclass::Symbols);

    //qDebug()<<"  +--- getWinSymbolProp("<<n<<"): fnt: "<<fnt.first<<", "<<fnt.second<<" / sym: "<<fntSym.first<<", "<<fntSym.second;

    static QHash<QString, QString> winSymbolSymbol;
    if (winSymbolSymbol.isEmpty()) {
      winSymbolSymbol.insert("leftrightarrow", QChar(0xAB));
      winSymbolSymbol.insert("leftarrow", QChar(0xAC));
      winSymbolSymbol.insert("rightarrow", QChar(0xAE));
      winSymbolSymbol.insert("to", QChar(0xAE));
      winSymbolSymbol.insert("uparrow", QChar(0xAD));
      winSymbolSymbol.insert("downarrow", QChar(0xAF));
      winSymbolSymbol.insert("Leftrightarrow", QChar(0xDB));
      winSymbolSymbol.insert("iff", QChar(0xDB));
      winSymbolSymbol.insert("Leftarrow", QChar(0xDC));
      winSymbolSymbol.insert("Rightarrow", QChar(0xDE));
      winSymbolSymbol.insert("Uparrow", QChar(0xDD));
      winSymbolSymbol.insert("Downarrow", QChar(0xFF));
      winSymbolSymbol.insert("pm", QChar(0xB1));
      winSymbolSymbol.insert("leq", QChar(0xA3));
      winSymbolSymbol.insert("geq", QChar(0xB3));
      winSymbolSymbol.insert("le", QChar(0xA3));
      winSymbolSymbol.insert("ge", QChar(0xB3));
      winSymbolSymbol.insert("times", QChar(0xB4));
      winSymbolSymbol.insert("propto", QChar(0xB5));
      winSymbolSymbol.insert("partial", QChar(0xB6));
      winSymbolSymbol.insert("bullet", QChar(0xB7));
      winSymbolSymbol.insert("neq", QChar(0xB9));
      winSymbolSymbol.insert("ne", QChar(0xB9));
      winSymbolSymbol.insert("equiv", QChar(0xBA));
      winSymbolSymbol.insert("approx", QChar(0xBB));
      winSymbolSymbol.insert("ellipsis", QChar(0xBC));
      winSymbolSymbol.insert("Im", QChar(0xC1));
      winSymbolSymbol.insert("Re", QChar(0xC2));
      winSymbolSymbol.insert("otimes", QChar(0xC4));
      winSymbolSymbol.insert("oplus", QChar(0xC5));
      winSymbolSymbol.insert("oslash", QChar(0xC6));
      winSymbolSymbol.insert("cap", QChar(0xC7));
      winSymbolSymbol.insert("land", QChar(0xC7));
      winSymbolSymbol.insert("cup", QChar(0xC8));
      winSymbolSymbol.insert("lor", QChar(0xC8));
      winSymbolSymbol.insert("supset", QChar(0xC9));
      winSymbolSymbol.insert("supseteq", QChar(0xCA));
      winSymbolSymbol.insert("supsetnot", QChar(0xCB));
      winSymbolSymbol.insert("subset", QChar(0xCC));
      winSymbolSymbol.insert("subseteq", QChar(0xCD));
      winSymbolSymbol.insert("in", QChar(0xCE));
      winSymbolSymbol.insert("notin", QChar(0xCF));
      winSymbolSymbol.insert("angle", QChar(0xD0));
      winSymbolSymbol.insert("nabla", QChar(0xD1));
      winSymbolSymbol.insert("copyright", QChar(0xD3));
      winSymbolSymbol.insert("registered", QChar(0xD2));
      winSymbolSymbol.insert("circledR", QChar(0xD2));
      winSymbolSymbol.insert("trademark", QChar(0xD4));
      winSymbolSymbol.insert("textregistered", QChar(0xD4));
      winSymbolSymbol.insert("cdot", QChar(0xD7));
      winSymbolSymbol.insert("neg", QChar(0xD8));
      winSymbolSymbol.insert("wedge", QChar(0xD9));
      winSymbolSymbol.insert("vee", QChar(0xDA));
      winSymbolSymbol.insert("diamond", QChar(0xE0));
      winSymbolSymbol.insert("langle", QChar(0xE1));
      winSymbolSymbol.insert("rangle", QChar(0xF1));
      winSymbolSymbol.insert("forall", QChar(0x22));
      winSymbolSymbol.insert("exists", QChar(0x24));
      winSymbolSymbol.insert("cong", QChar(0x40));
      winSymbolSymbol.insert("bot", QChar(0x5E));
      winSymbolSymbol.insert("ll", "<<");
      winSymbolSymbol.insert("gg", ">>");
      winSymbolSymbol.insert("Alef", QChar(0xC0));
      winSymbolSymbol.insert("alef", QChar(0xC0));
      winSymbolSymbol.insert("tilde", "~");
      winSymbolSymbol.insert("emptyset", QChar(0xC6));
      winSymbolSymbol.insert("varnothing", QChar(0xC6));
      winSymbolSymbol.insert("lceil", QChar(0xE9));
      winSymbolSymbol.insert("rceil", QChar(0xF9));
      winSymbolSymbol.insert("lfloor", QChar(0xEB));
      winSymbolSymbol.insert("rfloor", QChar(0xFB));
      winSymbolSymbol.insert("subsetnot", QChar(0xCB));
      winSymbolSymbol.insert("DC", "=");
      winSymbolSymbol.insert("mid", "|");
      winSymbolSymbol.insert("cdots", QString(3, QChar(0xD7)));
      winSymbolSymbol.insert("dots", QChar(0xDC));
      winSymbolSymbol.insert("ldots", QChar(0xDC));
      winSymbolSymbol.insert("cent", "c");
      winSymbolSymbol.insert("sim", QChar(0x7E));
      winSymbolSymbol.insert("infty", QChar(0xA5));
    }

    QHash<QString, QString>::iterator itsymbol = winSymbolSymbol.find(n);
    if (itsymbol!=winSymbolSymbol.end()) {
      props.symbol = itsymbol.value();
    } else if (n == "int") { props.symbol = QChar(0xF2); props.fontFactor = mathFontFactor; props.yfactor = +0.1; }
    else if (n == "bbC") { props.symbol = "C"; props.bold = +1; props.italic = -1; }
    else if (n == "bbH") { props.symbol = "H"; props.bold = +1; props.italic = -1; }
    else if (n == "bbN") { props.symbol = "N"; props.bold = +1; props.italic = -1; }
    else if (n == "bbP") { props.symbol = "P"; props.bold = +1; props.italic = -1; }
    else if (n == "bbQ") { props.symbol = "Q"; props.bold = +1; props.italic = -1; }
    else if (n == "bbR") { props.symbol = "R"; props.bold = +1; props.italic = -1; }
    else if (n == "bbZ") { props.symbol = "Z"; props.bold = +1; props.italic = -1; }
    else if (n == "iint") { props.symbol = QString(2, QChar(0xF2)); props.fontFactor = mathFontFactor; props.yfactor = +0.1; }
    else if (n == "iiint") { props.symbol = QString(3, QChar(0xF2)); props.fontFactor = mathFontFactor; props.yfactor = +0.1; }
    else if (n == "bigcap") { props.symbol = QChar(0xC7); props.fontFactor = 2; }
    else if (n == "bigcup") { props.symbol = QChar(0xC8); props.fontFactor = 2; }
    else if (n == "bigvee") { props.symbol = QChar(0xDA); props.fontFactor = 2; }
    else if (n == "bighat") { props.symbol = QChar(0xD9); props.fontFactor = 2; }

    else { // here are text mode symbols, i.e. bold and italic won't be touched
      props.bold = -1;
      props.italic = -1;
      props.font = fnt.first;
      if (n == "_") { props.symbol = "_"; props.bold = 0; props.italic = 0; }
      else if (n == "}") { props.symbol = "}"; props.italic=-1; }
      else if (n == "{") { props.symbol = "{"; props.italic=-1; }
      else if (n == "]") { props.symbol = "]"; props.italic=-1; }
      else if (n == "[") { props.symbol = "["; props.italic=-1; }
      else if (n == "(") { props.symbol = "("; props.italic=-1; }
      else if (n == ")") { props.symbol = ")"; props.italic=-1; }
      else if (n == "|") { props.symbol = "|"; props.italic=-1; }
      else if (n == "hbar") { props.symbol = "h"; props.bold = 0; props.italic = 0; props.drawBar = true; }
      else if (n == "euro") { props.symbol = ""; props.bold = 0; props.italic = 0; }
      else if (n == "cent") { props.symbol = QChar(0xA2); props.bold = 0; props.italic = 0; }
      else if (n == "pound") { props.symbol = QChar(0xA3); props.bold = 0; props.italic = 0; }
      else if (n == "yen") { props.symbol = QChar(0xA5); props.bold = 0; props.italic = 0; }
      else if (n == "div") { props.symbol = QChar(0xF7); props.bold = 0; props.italic = 0; }
      else if (n == "backslash") { props.symbol = "\\"; props.bold = 0; props.italic = 0; }
      //else if (n=="|") { symbol="||"; bold=0; italic=0; }
      else if (n == "/") { props.symbol = "/"; props.bold = 0; props.italic = 0; }
      else if (n == "+") { props.symbol = "+"; props.bold = 0; props.italic = 0; }
      else if (n == "-") { props.symbol = "-"; props.bold = 0; props.italic = 0; }
      else if (n == "*") { props.symbol = "*"; props.bold = 0; props.italic = 0; }
      else if (n == "=") { props.symbol = "= "; props.bold = 0; props.italic = 0; }
      else if (n == ">") { props.symbol = ">"; props.bold = 0; props.italic = 0; }
      else if (n == "<") { props.symbol = "<"; props.bold = 0; props.italic = 0; }
      else if (n == "$") { props.symbol = "$"; props.bold = 0; props.italic = 0; }
      else if (n == "%") { props.symbol = "%"; props.bold = 0; props.italic = 0; }
      else if (n == "&") { props.symbol = "&"; props.bold = 0; props.italic = 0; }
      else if (n == "#") { props.symbol = "#"; props.bold = 0; props.italic = 0; }
      else if (n == "ast") { props.symbol = "*"; props.bold = 0; props.italic = 0; }
      else if (n == "asterisk") { props.symbol = "*"; props.bold = 0; props.italic = 0; }
      else if (n == "glq") { props.symbol = "'"; props.bold = 0; props.italic = 0; }
      else if (n == "grq") { props.symbol = "'"; props.bold = 0; props.italic = 0; }
      else if (n == "glqq") { props.symbol = "\""; props.bold = 0; props.italic = 0; }
      else if (n == "grqq") { props.symbol = "\""; props.bold = 0; props.italic = 0; }
      else if (n == "flq") { props.symbol = "<"; props.bold = 0; props.italic = 0; }
      else if (n == "frq") { props.symbol = ">"; props.bold = 0; props.italic = 0; }
      else if (n == "flqq") { props.symbol = QChar(0xAB); props.bold = 0; props.italic = 0; }
      else if (n == "frqq") { props.symbol = QChar(0xBB); props.bold = 0; props.italic = 0; }
      else if (n == "prime") { props.symbol = "'"; props.bold = 0; props.italic = 0; }
      else if (n == "dprime") { props.symbol = "''"; props.bold = 0; props.italic = 0; }
      else if (n == "trprime") { props.symbol = "'''"; props.bold = 0; props.italic = 0; }
      else { return false; }
    }

    return true;
}

bool JKQTMathTextSymbolNode::getGreekSymbolProp(JKQTMathTextSymbolNode::SymbolProps& props, const QString &n, const JKQTMathTextEnvironment& currentEv, double mathFontFactor) const
{
    auto fnt=parentMathText->getFontData(currentEv.font, currentEv.insideMath, JKQTMathText::FontSubclass::Text);
    auto fntGreek=parentMathText->getFontData(currentEv.font, currentEv.insideMath, JKQTMathText::FontSubclass::Greek);

    //qDebug()<<"  +--- getGreekSymbolProp("<<n<<"): fnt: "<<fnt.first<<", "<<fnt.second<<" / greek: "<<fntGreek.first<<", "<<fntGreek.second;

    static QHash<QString, QString> winSymbolGreek;
    if (winSymbolGreek.isEmpty()) {
      winSymbolGreek.insert("alpha", "a");
      winSymbolGreek.insert("beta", "b");
      winSymbolGreek.insert("gamma", "g");
      winSymbolGreek.insert("delta", "d");
      winSymbolGreek.insert("epsilon", "e");
      winSymbolGreek.insert("varepsilon", "e");
      winSymbolGreek.insert("zeta", "z");
      winSymbolGreek.insert("eta", "h");
      winSymbolGreek.insert("theta", "q");
      winSymbolGreek.insert("vartheta", "J");
      winSymbolGreek.insert("iota", "i");
      winSymbolGreek.insert("kappa", "k");
      winSymbolGreek.insert("lambda", "l");
      winSymbolGreek.insert("mu", "m");
      winSymbolGreek.insert("nu", "n");
      winSymbolGreek.insert("xi", "x");
      winSymbolGreek.insert("pi", "p");
      winSymbolGreek.insert("varpi", "v");
      winSymbolGreek.insert("rho", "r");
      winSymbolGreek.insert("sigma", "s");
      winSymbolGreek.insert("varsigma", "V");
      winSymbolGreek.insert("tau", "t");
      winSymbolGreek.insert("upsilon", "u");
      winSymbolGreek.insert("phi", "f");
      winSymbolGreek.insert("varphi", "j");
      winSymbolGreek.insert("chi", "c");
      winSymbolGreek.insert("psi", "y");
      winSymbolGreek.insert("omega", "w");
      winSymbolGreek.insert("Gamma", "G");
      winSymbolGreek.insert("Delta", "D");
      winSymbolGreek.insert("Theta", "Q");
      winSymbolGreek.insert("Lambda", "L");
      winSymbolGreek.insert("Omega", "W");
      winSymbolGreek.insert("Xi", "X");
      winSymbolGreek.insert("Pi", "P");
      winSymbolGreek.insert("Sigma", "S");
      winSymbolGreek.insert("Upsilon", "");
      winSymbolGreek.insert("Phi", "F");
      winSymbolGreek.insert("Psi", "Y");
    }

    static QHash<QString, QString> unicodeGreek;
    if (unicodeGreek.isEmpty()) {
      unicodeGreek.insert("alpha", QChar(0x3B1));
      unicodeGreek.insert("beta", QChar(0x3B2));
      unicodeGreek.insert("gamma", QChar(0x3B3));
      unicodeGreek.insert("delta", QChar(0x3B4));
      unicodeGreek.insert("epsilon", QChar(0x3B5));
      unicodeGreek.insert("varepsilon", QChar(0x3B5));
      unicodeGreek.insert("zeta", QChar(0x3B6));
      unicodeGreek.insert("eta", QChar(0x3B7));
      unicodeGreek.insert("theta", QChar(0x3B8));
      unicodeGreek.insert("vartheta", QChar(0x3D1));
      unicodeGreek.insert("iota", QChar(0x3B9));
      unicodeGreek.insert("kappa", QChar(0x3BA));
      unicodeGreek.insert("lambda", QChar(0x3BB));
      unicodeGreek.insert("mu", QChar(0x3BC));
      unicodeGreek.insert("nu", QChar(0x3BD));
      unicodeGreek.insert("xi", QChar(0x3BE));
      unicodeGreek.insert("pi", QChar(0x3C0));
      unicodeGreek.insert("varpi", QChar(0x3D6));
      unicodeGreek.insert("rho", QChar(0x3C1));
      unicodeGreek.insert("varrho", QChar(0x3F1));
      unicodeGreek.insert("sigma", QChar(0x3C3));
      unicodeGreek.insert("varsigma", QChar(0x3C2));
      unicodeGreek.insert("tau", QChar(0x3C4));
      unicodeGreek.insert("upsilon", QChar(0x3C5));
      unicodeGreek.insert("phi", QChar(0x3D5));
      unicodeGreek.insert("varphi", QChar(0x3C6));
      unicodeGreek.insert("chi", QChar(0x3C7));
      unicodeGreek.insert("psi", QChar(0x3C8));
      unicodeGreek.insert("omega", QChar(0x3C9));
      unicodeGreek.insert("Omega", QChar(0x3A9));
      unicodeGreek.insert("Gamma", QChar(0x393));
      unicodeGreek.insert("Delta", QChar(0x394));
      unicodeGreek.insert("Theta", QChar(0x398));
      unicodeGreek.insert("Lambda", QChar(0x39B));
      unicodeGreek.insert("Xi", QChar(0x39E));
      unicodeGreek.insert("Pi", QChar(0x3A0));
      unicodeGreek.insert("prod", QChar(0x3A0));
      unicodeGreek.insert("Sigma", QChar(0x3A3));
      unicodeGreek.insert("sum", QChar(0x3A3));
      unicodeGreek.insert("Upsilon", QChar(0x3A5));
      unicodeGreek.insert("Phi", QChar(0x3A6));
      unicodeGreek.insert("Psi", QChar(0x3A8));
    }

    //qDebug()<<"##SEARCHING "<<n;
    props.italic = -1;
    if(fnt.second==MTFEunicodeLimited || fnt.second==MTFEunicode) {
        props.font = fnt.first;
        //qDebug()<<"##SEARCHING "<<n<<": SEARCHING IN "<<props.font<<" [unicode]";
        //std::cout<<"encoding unicode\n";
        QHash<QString, QString>::iterator itgreek = unicodeGreek.find(n);
        if (itgreek!=unicodeGreek.end()) {
            //qDebug()<<"##SEARCHING "<<n<<": FOUND IN "<<props.font<<" [unicode]";
            props.symbol = itgreek.value();
            return true;
        }
    }

    if(fntGreek.second==MTFEunicodeLimited || fntGreek.second==MTFEunicode) {
        props.font = fntGreek.first;
        //qDebug()<<"##SEARCHING "<<n<<": SEARCHING IN "<<props.font<<" [unicode]";
        //std::cout<<"encoding unicode\n";
        QHash<QString, QString>::iterator itgreek = unicodeGreek.find(n);
        if (itgreek!=unicodeGreek.end()) {
            //qDebug()<<"##SEARCHING "<<n<<": FOUND IN "<<props.font<<" [unicode]";
            props.symbol = itgreek.value();
            return true;
        }
    }

    props.font = fntGreek.first;
    //qDebug()<<"##SEARCHING "<<n<<": SEARCHING IN "<<props.font<<" [winSym]";
    QHash<QString, QString>::iterator itgreek = winSymbolGreek.find(n);
    if (itgreek!=winSymbolGreek.end()) {
        props.symbol = itgreek.value();
        //qDebug()<<"##SEARCHING "<<n<<": FOUND IN "<<props.font<<" [unicode]";
    } else if (n == "sum") { props.symbol = "S"; props.fontFactor = mathFontFactor; props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "prod") { props.symbol = "P"; props.fontFactor = mathFontFactor; props.heightIsAscent = true; props.exactAscent = true; }
    else {
        //qDebug()<<"##SEARCHING "<<n<<": DIDN'T FIND!";
        return false;
    }

    //qDebug()<<"##SEARCHING "<<n<<": FOUND!";
    return true;
}


bool JKQTMathTextSymbolNode::getStandardTextSymbolProp(JKQTMathTextSymbolNode::SymbolProps& props, const QString &n) const
{
    props.bold = -1;
    props.italic = -1;

    //qDebug()<<"  +--- getStandardTextSymbolProp("<<n<<")";

    if (n == "_") { props.symbol = "_"; }
    else if (n == "backslash") { props.symbol = "\\"; props.bold = 0; }
    else if (n == "+") { props.symbol = "+"; props.bold = 0; }
    else if (n == "-") { props.symbol = "-"; props.bold = 0; }
    else if (n == "/") { props.symbol = "/"; props.bold = 0; }
    else if (n == "=") { props.symbol = "= "; props.bold = 0; }
    else if (n == "<") { props.symbol = "<"; props.bold = 0; }
    else if (n == ">") { props.symbol = ">"; props.bold = 0; }
    else if (n == "|") { props.symbol = "|"; props.bold = 0; }
    else if (n == "}") { props.symbol = "}"; }
    else if (n == "{") { props.symbol = "{"; }
    else if (n == "]") { props.symbol = "]"; }
    else if (n == "[") { props.symbol = "["; }
    else if (n == "(") { props.symbol = "("; }
    else if (n == ")") { props.symbol = ")"; }
    else if (n == "|") { props.symbol = "|"; }
    else if (n == "$") { props.symbol = "$"; }
    else if (n == "%") { props.symbol = "%"; }
    else if (n == "&") { props.symbol = "&"; }
    else if (n == "#") { props.symbol = "#"; }
    else if (n == "ast") { props.symbol = "*"; }
    else if (n == "dots") { props.symbol = "..."; }
    else if (n == "ldots") { props.symbol = "..."; }
    else if (n == "colon") { props.symbol = ":"; }

    else {return false;}
    return true;
}

bool JKQTMathTextSymbolNode::getUnicodeBaseSymbolProp(JKQTMathTextSymbolNode::SymbolProps& props, const QString &n) const
{

    //qDebug()<<"--- getUnicodeBaseSymbolProp("<<n<<"): ";
    static QHash<QString, QString> unicodeBaseSymbol;
    if (unicodeBaseSymbol.isEmpty()) {

        unicodeBaseSymbol.insert("diamond", QChar(0xE0));
        unicodeBaseSymbol.insert("infty", QChar(0x221E));
        unicodeBaseSymbol.insert("partial", QChar(0x2202));
        unicodeBaseSymbol.insert("times", QChar(0x2A2F));
        unicodeBaseSymbol.insert("*", QChar(0x2217));
        unicodeBaseSymbol.insert("ast", QChar(0x2217));
        unicodeBaseSymbol.insert("asterisk", QChar(0x2217));
        unicodeBaseSymbol.insert("star", QChar(0x22C6));
        unicodeBaseSymbol.insert("bullet", QChar(0x2219));
        unicodeBaseSymbol.insert("copyright", QChar(0x00A9));
        unicodeBaseSymbol.insert("registered", QChar(0x00AE));
        unicodeBaseSymbol.insert("circledR", QChar(0x00AE));
        unicodeBaseSymbol.insert("trademark", QChar(0x2122));
        unicodeBaseSymbol.insert("textregistered", QChar(0x2122));
        unicodeBaseSymbol.insert("cdot", QChar(0x00B7));
        unicodeBaseSymbol.insert("pm", QChar(0x00B1));
        unicodeBaseSymbol.insert("leq", QChar(0x2264));
        unicodeBaseSymbol.insert("geq", QChar(0x2265));
        unicodeBaseSymbol.insert("le", QChar(0x2264));
        unicodeBaseSymbol.insert("ge", QChar(0x2265));
        unicodeBaseSymbol.insert("hbar", QChar(0x210F));
        unicodeBaseSymbol.insert("EUR", QChar(0x20AC));
        unicodeBaseSymbol.insert("euro", QChar(0x20AC));
        unicodeBaseSymbol.insert("circ", QChar(0x2218));
        unicodeBaseSymbol.insert("cent", QChar(0x00A2));
        unicodeBaseSymbol.insert("pound", QChar(0x00A3));
        unicodeBaseSymbol.insert("yen", QChar(0x00A5));
        unicodeBaseSymbol.insert("dollar", QChar(0x0024));
        unicodeBaseSymbol.insert("neq", QChar(0x2260));
        unicodeBaseSymbol.insert("lnot", QChar(0x2260));
        unicodeBaseSymbol.insert("Angstrom", QChar(0x00AC));
        unicodeBaseSymbol.insert("co", QChar(0x2105));
        unicodeBaseSymbol.insert("No", QChar(0x2116));
        unicodeBaseSymbol.insert("Ohm", QChar(0x2126));
        unicodeBaseSymbol.insert("ohm", QChar(0x2126));
        unicodeBaseSymbol.insert("tcohm", QChar(0x2126));
        unicodeBaseSymbol.insert("partial", QChar(0x2202));
        unicodeBaseSymbol.insert("cdots", QString(QChar(0x00B7)) + QString(QChar(0x00B7)) + QString(QChar(0x00B7)));
        unicodeBaseSymbol.insert("approx", QChar(0x2248));
        unicodeBaseSymbol.insert("Angstroem", QChar(0x212B));
        unicodeBaseSymbol.insert("-", QChar(0x2212));
        unicodeBaseSymbol.insert("dots", QChar(0x2026));
        unicodeBaseSymbol.insert("ldots", QChar(0x2026));
    }

    QHash<QString, QString>::iterator itbasesymbol = unicodeBaseSymbol.find(n);
    if (itbasesymbol!=unicodeBaseSymbol.end()) {
      props.symbol = itbasesymbol.value();
      //qDebug()<<"### found "<<n<<" in unicodeBaseSymbol";
    } else {
        props.bold = -1;
        props.italic = -1;


        if (n == "glq") { props.symbol = QChar(0x2018); props.bold = 0; props.italic = 0; }
        else if (n == "grq") { props.symbol = QChar(0x2019); props.bold = 0; props.italic = 0; }
        else if (n == "glqq") { props.symbol = QChar(0x201C); props.bold = 0; props.italic = 0; }
        else if (n == "grqq") { props.symbol = QChar(0x201D); props.bold = 0; props.italic = 0; }
        else if (n == "flq") { props.symbol = QChar(0x2039); props.bold = 0; props.italic = 0; }
        else if (n == "frq") { props.symbol = QChar(0x203A); props.bold = 0; props.italic = 0; }
        else if (n == "flqq") { props.symbol = QChar(0x00AB); props.bold = 0; props.italic = 0; }
        else if (n == "frqq") { props.symbol = QChar(0x00BB); props.bold = 0; props.italic = 0; }
        else if (n == "prime") { props.symbol = QChar(0x2032); props.bold = 0; props.italic = 0; }
        else if (n == "dprime") { props.symbol = QChar(0x2033); props.bold = 0; props.italic = 0; }
        else if (n == "trprime") { props.symbol = QChar(0x2034); props.bold = 0; props.italic = 0; }
        else { return false; }
    }
    //qDebug()<<"### found "<<n<<" in unicodeBaseSymbol";
    return true;
}

bool JKQTMathTextSymbolNode::getUnicodeFullSymbolProp(JKQTMathTextSymbolNode::SymbolProps& props, const QString &n, double mathFontFactor) const
{

    //qDebug()<<"  +--- getUnicodeFullSymbolProp("<<n<<")";

    static QHash<QString, QString> unicodeSymbol;
    if (unicodeSymbol.isEmpty()) {
      unicodeSymbol.insert("leftrightarrow", QChar(0x2194));
      unicodeSymbol.insert("leftarrow", QChar(0x2190));
      unicodeSymbol.insert("rightarrow", QChar(0x2192));
      unicodeSymbol.insert("to", QChar(0x2192));
      unicodeSymbol.insert("uparrow", QChar(0x2191));
      unicodeSymbol.insert("downarrow", QChar(0x2193));
      unicodeSymbol.insert("updownarrow", QChar(0x2195));
      unicodeSymbol.insert("Leftrightarrow", QChar(0x21D4));
      unicodeSymbol.insert("iff", QChar(0x21D4));
      unicodeSymbol.insert("Leftarrow", QChar(0x21D0));
      unicodeSymbol.insert("Rightarrow", QChar(0x21D2));
      unicodeSymbol.insert("Uparrow", QChar(0x21D1));
      unicodeSymbol.insert("Downarrow", QChar(0x21D3));
      unicodeSymbol.insert("Updownarrow", QChar(0x21D5));
      unicodeSymbol.insert("mp", QChar(0x2213));
      unicodeSymbol.insert("ll", QChar(0x226A));
      unicodeSymbol.insert("gg", QChar(0x226B));
      unicodeSymbol.insert("Alef", QChar(0x2135));
      unicodeSymbol.insert("Aleph", QChar(0x2135));
      unicodeSymbol.insert("Bet", QChar(0x2136));
      unicodeSymbol.insert("Beth", QChar(0x2136));
      unicodeSymbol.insert("Gimel", QChar(0x2137));
      unicodeSymbol.insert("Dalet", QChar(0x2138));
      unicodeSymbol.insert("alef", QChar(0x2135));
      unicodeSymbol.insert("aleph", QChar(0x2135));
      unicodeSymbol.insert("bet", QChar(0x2136));
      unicodeSymbol.insert("beth", QChar(0x2136));
      unicodeSymbol.insert("gimel", QChar(0x2137));
      unicodeSymbol.insert("dalet", QChar(0x2138));
      unicodeSymbol.insert("nexists", QChar(0x2204));
      unicodeSymbol.insert("ni", QChar(0x220B));
      unicodeSymbol.insert("notni", QChar(0x220C));
      unicodeSymbol.insert("tilde", QChar(0x223C));
      unicodeSymbol.insert("sim", QChar(0x223C));
      unicodeSymbol.insert("emptyset", QChar(0x2300));
      unicodeSymbol.insert("varnothing", QChar(0x2300));
      unicodeSymbol.insert("odot", QChar(0x2299));
      unicodeSymbol.insert("ominus", QChar(0x2296));
      unicodeSymbol.insert("lceil", QChar(0x2308));
      unicodeSymbol.insert("rceil", QChar(0x2309));
      unicodeSymbol.insert("lfloor", QChar(0x230A));
      unicodeSymbol.insert("rfloor", QChar(0x230B));
      unicodeSymbol.insert("llcorner", QChar(0x231E));
      unicodeSymbol.insert("lrcorner", QChar(0x231F));
      unicodeSymbol.insert("ulcorner", QChar(0x231C));
      unicodeSymbol.insert("urcorner", QChar(0x231D));
      unicodeSymbol.insert("blcorner", QChar(0x231E));
      unicodeSymbol.insert("brcorner", QChar(0x231F));
      unicodeSymbol.insert("tlcorner", QChar(0x231C));
      unicodeSymbol.insert("trcorner", QChar(0x231D));
      unicodeSymbol.insert("subsetnot", QChar(0x2284));
      unicodeSymbol.insert("DC", QChar(0x2393));
      unicodeSymbol.insert("bot", QChar(0x22A4));
      unicodeSymbol.insert("perp", QChar(0x22A5));
      unicodeSymbol.insert("sqcap", QChar(0x2293));
      unicodeSymbol.insert("sqcup", QChar(0x2294));
      unicodeSymbol.insert("triangle", QChar(0x2206));
      unicodeSymbol.insert("square", QChar(0x25A1));
      unicodeSymbol.insert("setminus", QChar(0x2216));
      unicodeSymbol.insert("mid", QChar(0x2223));
      unicodeSymbol.insert("nmid", QChar(0x2224));
      unicodeSymbol.insert("vdots", QChar(0x22EE));
      unicodeSymbol.insert("iddots", QChar(0x22F0));
      unicodeSymbol.insert("ddots", QChar(0x22F1));
      unicodeSymbol.insert("perthousand", QChar(0x2030));
      unicodeSymbol.insert("leftharpoonup", QChar(0x21BC));
      unicodeSymbol.insert("rightharpoonup", QChar(0x21C0));
      unicodeSymbol.insert("upharpoonleft", QChar(0x21BF));
      unicodeSymbol.insert("downharpoonleft", QChar(0x21C3));
      unicodeSymbol.insert("leftrightharpoon", QChar(0x21CB));
      unicodeSymbol.insert("rightleftharpoon", QChar(0x21CC));
      unicodeSymbol.insert("leftharpoondown", QChar(0x21BD));
      unicodeSymbol.insert("rightharpoondown", QChar(0x21C1));
      unicodeSymbol.insert("upharpoonright", QChar(0x21BE));
      unicodeSymbol.insert("downharpoonright", QChar(0x21C2));
      unicodeSymbol.insert("nwarrow", QChar(0x2196));
      unicodeSymbol.insert("nearrow", QChar(0x2197));
      unicodeSymbol.insert("searrow", QChar(0x2198));
      unicodeSymbol.insert("swarrow", QChar(0x2199));
      unicodeSymbol.insert("mapsto", QChar(0x21A6));
      unicodeSymbol.insert("div", QChar(0x00F7));
      unicodeSymbol.insert("multimap", QChar(0x22B8));
      unicodeSymbol.insert("maporiginal", QChar(0x22B6));
      unicodeSymbol.insert("mapimage", QChar(0x22B7));
      unicodeSymbol.insert("benzene", QChar(0x232C));
      unicodeSymbol.insert("propto", QChar(0x221D));
      unicodeSymbol.insert("ne", QChar(0x2260));
      unicodeSymbol.insert("equiv", QChar(0x2261));
      unicodeSymbol.insert("ellipsis", QChar(0x2026));
      unicodeSymbol.insert("Im", QChar(0x2111));
      unicodeSymbol.insert("Re", QChar(0x211C));
      unicodeSymbol.insert("otimes", QChar(0x2297));
      unicodeSymbol.insert("oplus", QChar(0x2295));
      unicodeSymbol.insert("oslash", QChar(0x2298));
      unicodeSymbol.insert("cap", QChar(0x2229));
      unicodeSymbol.insert("land", QChar(0x2229));
      unicodeSymbol.insert("cup", QChar(0x222A));
      unicodeSymbol.insert("lor", QChar(0x222A));
      unicodeSymbol.insert("supset", QChar(0x2283));
      unicodeSymbol.insert("supseteq", QChar(0x2286));
      unicodeSymbol.insert("supsetnot", QChar(0x2285));
      unicodeSymbol.insert("subset", QChar(0x2282));
      unicodeSymbol.insert("subseteq", QChar(0x2286));
      unicodeSymbol.insert("in", QChar(0x2208));
      unicodeSymbol.insert("notin", QChar(0x2209));
      unicodeSymbol.insert("angle", QChar(0x2220));
      unicodeSymbol.insert("measuredangle", QChar(0x2221));
      unicodeSymbol.insert("sphericalangle", QChar(0x2222));
      unicodeSymbol.insert("rightangle", QChar(0x221F));
      unicodeSymbol.insert("nabla", QChar(0x2207));
      unicodeSymbol.insert("parallel", QChar(0x2225));
      unicodeSymbol.insert("nparallel", QChar(0x2226));
      unicodeSymbol.insert("neg", QChar(0x00AC));
      unicodeSymbol.insert("wedge", QChar(0x2227));
      unicodeSymbol.insert("vee", QChar(0x2228));
      unicodeSymbol.insert("langle", QChar(0x2329));
      unicodeSymbol.insert("rangle", QChar(0x232A));
      unicodeSymbol.insert("forall", QChar(0x2200));
      unicodeSymbol.insert("exists", QChar(0x2203));
      unicodeSymbol.insert("bot", QChar(0x22A5));
      unicodeSymbol.insert("geqq", QChar(0x2267));
      unicodeSymbol.insert("leqq", QChar(0x2266));
      unicodeSymbol.insert("prec", QChar(0x227A));
      unicodeSymbol.insert("succ", QChar(0x227B));
      unicodeSymbol.insert("vartriangleleft", QChar(0x22B2));
      unicodeSymbol.insert("cong", QChar(0x2245));
      unicodeSymbol.insert("simeq", QChar(0x2243));
      unicodeSymbol.insert("therefore", QChar(0x2234));
      unicodeSymbol.insert("because", QChar(0x2235));
      unicodeSymbol.insert("lightning", QChar(0x21AF));
      unicodeSymbol.insert("blacksquare", QChar(0x220E));
      unicodeSymbol.insert("Box", QChar(0x25A1));
      unicodeSymbol.insert("celsius", QChar(0x2103));
      unicodeSymbol.insert("AC", QChar(0x223F));
      unicodeSymbol.insert("frown", QChar(0x2322));
      unicodeSymbol.insert("smile", QChar(0x2323));
      unicodeSymbol.insert("smiley", QChar(0x233A));
      unicodeSymbol.insert("blacksmiley", QChar(0x233B));
      unicodeSymbol.insert("frownie", QChar(0x2639));
      unicodeSymbol.insert("varhexagonlrbonds", QChar(0x232C));
      unicodeSymbol.insert("hexagon", QChar(0x2394));
      unicodeSymbol.insert("varcarriagereturn", QChar(0x23CE));
      unicodeSymbol.insert("benzenr", QChar(0x23E3));
      unicodeSymbol.insert("trapezium", QChar(0x23E2));
      unicodeSymbol.insert("female", QChar(0x2640));
      unicodeSymbol.insert("male", QChar(0x2642));

      unicodeSymbol.insert("accurrent", QChar(0x23E6));
    }

    QHash<QString, QString>::iterator itsymbol = unicodeSymbol.find(n);

    if (itsymbol!=unicodeSymbol.end()) {
      props.symbol = itsymbol.value();
    }
    else if (n == "sum") { props.symbol = QChar(0x2211); props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "prod") { props.symbol = QChar(0x220F); props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "bbC") { props.symbol = QChar(0x2102); props.italic = -1; }
    else if (n == "bbH") { props.symbol = QChar(0x210D); props.italic = -1; }
    else if (n == "bbN") { props.symbol = QChar(0x2115); props.italic = -1; }
    else if (n == "bbP") { props.symbol = QChar(0x2119); props.italic = -1; }
    else if (n == "bbQ") { props.symbol = QChar(0x211A); props.italic = -1; }
    else if (n == "bbR") { props.symbol = QChar(0x211D); props.italic = -1; }
    else if (n == "bbZ") { props.symbol = QChar(0x2124); props.italic = -1; }
    else if (n == "iint") { props.symbol = QChar(0x222C); props.fontFactor = mathFontFactor; /*yfactor=+0.1;*/ props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "iiint") { props.symbol = QChar(0x222D); props.fontFactor = mathFontFactor; /*yfactor=+0.1;*/ props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "oint") { props.symbol = QChar(0x222E); props.fontFactor = mathFontFactor; /*yfactor=+0.1;*/ props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "oiint") { props.symbol = QChar(0x222F); props.fontFactor = mathFontFactor; /*yfactor=+0.1;*/ props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "oiiint") { props.symbol = QChar(0x2230); props.fontFactor = mathFontFactor; /*yfactor=+0.1;*/ props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "coprod") { props.symbol = QChar(0x2210); props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "bigcap") { props.symbol = QChar(0x22C2); props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "bigcup") { props.symbol = QChar(0x22C3); props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "bigvee") { props.symbol = QChar(0x22C1); props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "bighat") { props.symbol = QChar(0x22C0); props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "int") { props.symbol = QChar(0x222B); props.fontFactor = mathFontFactor; /*yfactor=+0.1;*/ props.heightIsAscent = true; props.exactAscent = true; }
    else {return false;}
    return true;
}

bool JKQTMathTextSymbolNode::getSymbolProp(JKQTMathTextSymbolNode::SymbolProps& props, const QString &n, const JKQTMathTextEnvironment& currentEv, double mathFontFactor) const
{
    auto fnt=parentMathText->getFontData(currentEv.font, currentEv.insideMath, JKQTMathText::FontSubclass::Text);
    auto fntGreek=parentMathText->getFontData(currentEv.font, currentEv.insideMath, JKQTMathText::FontSubclass::Greek);
    auto fntSym=parentMathText->getFontData(currentEv.font, currentEv.insideMath, JKQTMathText::FontSubclass::Symbols);

    //qDebug()<<"--- getSymbolProp("<<n<<"): "<<fnt.first<<", "<<fnt.second;

    props.font = fnt.first;
    if (fnt.second==MTFEunicode) {
        props.font = fnt.first;
        if (!getUnicodeBaseSymbolProp(props, n) && !getUnicodeFullSymbolProp(props,n,mathFontFactor) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
            return false;
        }
    } else if (fnt.second==MTFEunicodeLimited) {
        props.font = fnt.first;
        if (!getUnicodeBaseSymbolProp(props, n) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
            if (fntSym.second==MTFEunicode) {
                props.font = fntSym.first;
                if (!getUnicodeBaseSymbolProp(props, n) && !getUnicodeFullSymbolProp(props,n,mathFontFactor) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
                    return false;
                }
            } else if (fntSym.second==MTFEunicodeLimited) {
                props.font = fntSym.first;
                if (!getUnicodeBaseSymbolProp(props, n) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
                    return false;
                }
            } else if (fntSym.second==MTFEwinSymbol) {
                props.font = fntSym.first;
                if (!getWinSymbolProp(props, n, currentEv, mathFontFactor) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
                    return false;
                }
            } else {
                return false;
            }
        }

    } else if (fnt.second==MTFEwinSymbol) {
        props.font = fnt.first;
        if (!getWinSymbolProp(props, n, currentEv, mathFontFactor)) {
            if (fntSym.second==MTFEunicode) {
                props.font = fntSym.first;
                if (!getUnicodeBaseSymbolProp(props, n) && !getUnicodeFullSymbolProp(props,n,mathFontFactor) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
                    return false;
                }
            } else if (fntSym.second==MTFEunicodeLimited) {
                props.font = fntSym.first;
                if (!getUnicodeBaseSymbolProp(props, n) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
                    return false;
                }
            } else if (fntSym.second==MTFEwinSymbol) {
                props.font = fntSym.first;
                if (!getWinSymbolProp(props, n, currentEv, mathFontFactor) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
                    return false;
                }
            } else {
                return false;
            }
        }

    } else if (fnt.second==MTFEStandard) {
        props.font = fnt.first;
        if (!getStandardTextSymbolProp(props, n)) {
            if (fntSym.second==MTFEunicode) {
                props.font = fntSym.first;
                if (!getUnicodeBaseSymbolProp(props, n) && !getUnicodeFullSymbolProp(props,n,mathFontFactor) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
                    return false;
                }
            } else if (fntSym.second==MTFEunicodeLimited) {
                props.font = fntSym.first;
                if (!getUnicodeBaseSymbolProp(props, n) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
                    return false;
                }
            } else if (fntSym.second==MTFEwinSymbol) {
                props.font = fntSym.first;
                if (!getWinSymbolProp(props, n, currentEv, mathFontFactor) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
                    return false;
                }
            } else {
                return false;
            }
        }
    } else {
        return false;
    }


    return true;
}

JKQTMathTextSymbolNode::SymbolProps JKQTMathTextSymbolNode::getSymbolProp(const QString &symName, const JKQTMathTextEnvironment& currentEv) const
{

    auto fnt=parentMathText->getFontData(currentEv.font, currentEv.insideMath, JKQTMathText::FontSubclass::Text);
    auto fntSym=parentMathText->getFontData(currentEv.font, currentEv.insideMath, JKQTMathText::FontSubclass::Symbols);
    auto fntGreek=parentMathText->getFontData(currentEv.font, currentEv.insideMath, JKQTMathText::FontSubclass::Greek);

    JKQTMathTextSymbolNode::SymbolProps props;
    double mathFontFactor=1.8;
    props.symbol="";
    props.fontFactor=1.0;
    props.bold=0;
    props.italic=-1;
    props.yfactor=0;
    props.drawBar=false;
    props.font=fnt.first;
    props.heightIsAscent=false;
    props.exactAscent=false;
    props.extendWidthInMathmode=false;

    QString n=symName;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // statische Lookup-Tabellen vorbereiten
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static QStringList extendWInMM;
    if (extendWInMM.isEmpty()) {
        extendWInMM <<"ll"<<"gg"<<"leq"<<"geq"<<"leftrightarrow"<<"leftarrow"<<"rightarrow"<<"to"<<"uparrow"<<"downarrow"<<"updownarrow"<<"Leftrightarrow"
                    <<"iff"<<"Leftarrow"<<"Rightarrow"<<"Uparrow"<<"Downarrow"<<"Updownarrow"<<"pm"<<"mp"<<"nexists"<<"ni"<<"notni"<<"circ"<<"sim"<<"emptyset"<<"odot"<<"ominus"
                    <<"subsetnot"<<"bot"<<"leftharpoonup"<<"rightharpoonup"<<"upharpoonleft"<<"downharpoonleft"<<"leftrightharpoon"<<"rightleftharpoon"<<"coprod"<<"leftharpoondown"
                    <<"rightharpoondown"<<"upharpoonright"<<"downharpoonright"<<"nwarrow"<<"nearrow"<<"searrow"<<"swarrow"<<"mapsto"<<"div"<<"multimap"<<"maporiginal"<<"mapimage"
                    <<"times"<<"propto"<<"bullet"<<"neq"<<"ne"<<"equiv"<<"approx"<<"otimes"<<"oplus"<<"oslash"<<"cap"<<"land"<<"cup"<<"lor"<<"supset"<<"supseteq"<<"supsetnot"
                    <<"subset"<<"subseteq"<<"in"<<"notin"<<"cdot"<<"wedge"<<"vee"<<"cong"<<"bot"<<"mid"<<"+"<<"-"<<"|"<<"*"<<"/"<<"<"<<">"<<"=";
    }

    if (extendWInMM.contains(n)) {
        props.extendWidthInMathmode=true;
    }


    static QHash<QString, QString> simpleTranslations;
    if (simpleTranslations.isEmpty()) {
      simpleTranslations.insert("", " ");
      simpleTranslations.insert("sin", "sin");
      simpleTranslations.insert("cos", "cos");
      simpleTranslations.insert("tan", "tan");
      simpleTranslations.insert("sinh", "sinh");
      simpleTranslations.insert("cosh", "cosh");
      simpleTranslations.insert("tanh", "tanh");
      simpleTranslations.insert("atan", "atan");
      simpleTranslations.insert("acos", "acos");
      simpleTranslations.insert("asin", "asin");
      simpleTranslations.insert("arcsin", "arcsin");
      simpleTranslations.insert("arccos", "arccos");
      simpleTranslations.insert("arctan", "arctan");
      simpleTranslations.insert("degree", QLatin1String("\xB0"));
      simpleTranslations.insert("textdegree ", QLatin1String("\xB0"));
      simpleTranslations.insert("ii", "i");
      simpleTranslations.insert("dd", "d");
      simpleTranslations.insert("exp", "exp");
      simpleTranslations.insert("log", "log");
      simpleTranslations.insert("ln", "ln");
      simpleTranslations.insert("ld", "ld");
      simpleTranslations.insert("lb", "lb");
      simpleTranslations.insert("argmin", "argmin");
      simpleTranslations.insert("argmax", "argmax");
      simpleTranslations.insert("max", "max");
      simpleTranslations.insert("mod", "mod");
      simpleTranslations.insert("min", "min");
      simpleTranslations.insert("median", "median");
      simpleTranslations.insert("sign", "sign");
      simpleTranslations.insert("sgn", "sgn");
      simpleTranslations.insert("sec", "sec");
      simpleTranslations.insert("gcd", "gcd");
      simpleTranslations.insert("hom", "hom");
      simpleTranslations.insert("ker", "ker");
      simpleTranslations.insert("dim", "dim");
      simpleTranslations.insert("cot", "cot");
      simpleTranslations.insert("arg", "arg");
      simpleTranslations.insert("det", "det");
      simpleTranslations.insert("deg", "deg");
      simpleTranslations.insert("Pr", "Pr");
      simpleTranslations.insert("coth", "coth");
    }


    static QHash<QString, QString> simpleTranslations_heightIsAscent;
    if (simpleTranslations_heightIsAscent.isEmpty()) {
      simpleTranslations_heightIsAscent.insert("erf", "erf");
      simpleTranslations_heightIsAscent.insert("median", "median");
      simpleTranslations_heightIsAscent.insert("min", "min");
      simpleTranslations_heightIsAscent.insert("max", "max");
      simpleTranslations_heightIsAscent.insert("inf", "inf");
      simpleTranslations_heightIsAscent.insert("sup", "sup");
      simpleTranslations_heightIsAscent.insert("liminf", "liminf");
      simpleTranslations_heightIsAscent.insert("limsup", "limsup");
      simpleTranslations_heightIsAscent.insert("lim", "lim");
    }



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    props.font=fnt.first;
    QString errorExplanation="";
    QHash<QString, QString>::iterator itsimple = simpleTranslations.find(n);
    if (itsimple!= simpleTranslations.end()) {
      props.symbol=itsimple.value();
    } else {
      QHash<QString, QString>::iterator itsimplehia = simpleTranslations_heightIsAscent.find(n);
      if (itsimplehia != simpleTranslations_heightIsAscent.end()) {
        props.symbol = itsimplehia.value();
        props.heightIsAscent = true;
      } else {
        props.font=fnt.first;
        if (!getSymbolProp(props, n, currentEv, mathFontFactor)) {
            errorExplanation="didn't find symbol given font def:"+fnt.first+"["+JKQTMathTextFontEncoding2String(fnt.second)+"] / sym:"+fntSym.first+"["+JKQTMathTextFontEncoding2String(fntSym.second)+"] / grk:"+fntGreek.first+"["+JKQTMathTextFontEncoding2String(fntGreek.second)+"]";
        }
      }
    }
    if (addWhitespace) props.symbol+=" ";

    static QSet<QString> extraSymbolName = {
        "infty",
        "|", " ", "quad", "qquad", "space", ";", ":", ",", "!","%"
        "longleftarrow",  "longrightarrow",
        "Longleftarrow", "Longrightarrow",
        "longleftrightarrow", "Longleftrightarrow"
    };


    static QSet<QString> extraSymbolNonItalic = {
        "+", "-", "*", "/", "<", ">", "=", "|", "{", "(", "[", "]", ")", "}", "\\"
    };

    if (props.symbol.simplified().isEmpty() && extraSymbolNonItalic.contains(n)) {
        props.symbol=n;
        if (n=="=") props.symbol="= ";
        props.italic=-1;
    }

    if (props.symbol.simplified().isEmpty() && !extraSymbolName.contains(n)) {
        parentMathText->addToErrorList(QObject::tr("unknown symbol '%1' found (%2)!").arg(n).arg(errorExplanation));
    }

    //qDebug()<<n<<": '"<<props.symbol<<"' / "<<props.font<<"    {{{ def:"+fnt.first+"["+encoding2String(fnt.second)+"] / sym:"+fntSym.first+"["+encoding2String(fntSym.second)+"] / grk:"+fntGreek.first+"["+encoding2String(fntGreek.second)+"] }}}";
    return props;

}

void JKQTMathTextSymbolNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* prevNodeSize) {
    double dummy1, dummy2;
    getSymbolSizeInternal(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos, dummy1, dummy2, prevNodeSize);
}

void JKQTMathTextSymbolNode::getSymbolSizeInternal(QPainter &painter, JKQTMathTextEnvironment currentEv, double &width, double &baselineHeight, double &overallHeight, double &strikeoutPos, double &subSuperXCorrection, double &subBesidesXCorrection, const JKQTMathTextNodeSize *prevNodeSize)
{
    QFont f=currentEv.getFont(parentMathText);
    auto props=getSymbolProp(symbolName, currentEv);
    f.setFamily(props.font);
    f.setPointSizeF(f.pointSizeF()*props.fontFactor);
    if (props.italic<0) f.setItalic(false);
    if (props.italic>0) f.setItalic(true);
    if (props.bold<0) f.setBold(false);
    if (props.bold>0) f.setBold(true);
    const QFontMetricsF fm(f, painter.device());
    QString symb=props.symbol;
    width=0;
    if (currentEv.insideMath) {
        width=qMax(JKQTMathTextGetTightBoundingRect(f, symb, painter.device()).width(), JKQTMathTextGetTightBoundingRect(f, "(", painter.device()).width());
    } else width=fm.boundingRect(symb).width();//fm.width(symbol);

    if (symb.isEmpty()) {
        width=fm.boundingRect("a").width();
        if (symbolName=="|") width=fm.boundingRect("1").width()*0.8;
        else if (symbolName=="infty") width=JKQTMathTextGetTightBoundingRect(f, "8", painter.device()).height();
        else if (symbolName=="quad" || symbolName=="qquad") width=JKQTMathTextGetTightBoundingRect(f, "M", painter.device()).width();
        else if (symbolName==" " || symbolName=="space") width=JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).width();
        else if (symbolName==";") width=JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).width()*0.75;
        else if (symbolName==":") width=JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).width()*0.5;
        else if (symbolName==",") width=JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).width()*0.25;
        else if (symbolName=="!") width=-JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).width()*0.25;
        else if (symbolName=="longleftarrow") { width=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="longrightarrow") {  width=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="Longleftarrow") { width=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="Longrightarrow") { width=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="longleftrightarrow") { width=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="Longleftrightarrow") { width=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*3.5; symb="x"; }
    }
    const QRectF tbr=(symb.isEmpty())?JKQTMathTextGetTightBoundingRect(f, "X", painter.device()):JKQTMathTextGetTightBoundingRect(f, symb, painter.device());
    overallHeight=tbr.height();// fm.height();
    baselineHeight=tbr.height()-tbr.bottom();
    if (props.exactAscent) {
        //baselineHeight=fm.ascent()*0.8;
    }
    if (props.heightIsAscent) {
        overallHeight=baselineHeight*1.1;
    }
    if (props.exactAscent && props.heightIsAscent) {
        //qDebug()<<symbolName<<symbol<<tbr.top()<<tbr.bottom()<<tbr.height();
        baselineHeight=tbr.height()-tbr.bottom();//fm.ascent()*0.8;
        overallHeight=tbr.height();//baselineHeight*1.1;
    }
    strikeoutPos=fm.strikeOutPos();

    if (props.extendWidthInMathmode && currentEv.insideMath) width=width*parentMathText->getMathoperatorWidthFactor();

    static QSet<QString> intCorrectionSymbolNames=QSet<QString>()<<"int"<<"iint"<<"iiint"<<"oint"<<"oiint"<<"oiiint";
    if (intCorrectionSymbolNames.contains(symbolName)) {
        subSuperXCorrection=parentMathText->getIntSubSuperXCorrectionFactor()*tbr.width();
        subBesidesXCorrection=parentMathText->getIntSubBesidesXCorrectionXFactor()*JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width();
    }
}

double JKQTMathTextSymbolNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    double width=0;
    double baselineHeight=0;
    double overallHeight=0, strikeoutPos=0;
    getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);
    QPen pold=painter.pen();
    QFont fold=painter.font();
    QFont f=currentEv.getFont(parentMathText);
    QFont f1=f;
    const auto props=getSymbolProp(symbolName, currentEv);
    f.setFamily(props.font);
    f.setPointSizeF(f.pointSizeF()*props.fontFactor);
    if (props.italic<0) f.setItalic(false);
    if (props.italic>0) f.setItalic(true);
    if (props.bold<0) f.setBold(false);
    if (props.bold>0) f.setBold(true);
    const QFontMetricsF fm(f, painter.device());
    painter.setFont(f);

    double shift=0;
    if (props.extendWidthInMathmode && currentEv.insideMath) {
        const QRectF tbr=JKQTMathTextGetTightBoundingRect(f, props.symbol, painter.device());
        shift=0.5*width-tbr.width()/2.0-tbr.x();
    }

    //std::cout<<"symbol '"<<symbolName.toStdString()<<"' = "<<std::hex<<symbol.at(0).digitValue()<<" in font '"<<f.family().toStdString()<<"' ("<<QFontInfo(f).family().toStdString()<<"): "<<fm.inFont(symbol.at(0))<<std::endl;

    QPen p=painter.pen();
    p.setColor(currentEv.color);
    p.setWidthF(fm.lineWidth());
    p.setStyle(Qt::SolidLine);
    painter.setPen(p);
    const double xwi=fm.boundingRect("x").width();
    if (!props.symbol.isEmpty()) {
        // if the symbol has been recognized in the constructor: draw the symbol
        const QPointF x0(x+shift, y+props.yfactor*overallHeight);
        painter.drawText(x0, props.symbol);
        if (props.drawBar) {
            const double xx=x+shift;
            const double yy=y-fm.xHeight()-(JKQTMathTextGetTightBoundingRect(f, "M", painter.device()).height()-fm.xHeight())/3.0;
            const QLineF l(xx, yy, xx+xwi/3.0+((currentEv.italic)?(xwi/3.0):0), yy);
            if (l.length()>0) painter.drawLine(l);
        }

    // try to draw some often used special symbols, by synthesizing them from
    // standard characters in the current drawing font
    } else if (symbolName=="infty") {
        //std::cout<<"draw infty\n";
        f1.setItalic(false);
        const QFontMetricsF fm1(f1, painter.device());
        painter.setFont(f1);
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(x+shift+fm1.boundingRect("8").width()/3.0, y-fm1.xHeight());
        painter.rotate(90);
        painter.drawText(QPointF(0,0), "8");

    } else if (symbolName=="|") {
        //std::cout<<"draw ||\n";
        f1.setItalic(false);
        const QFontMetricsF fm1(f1, painter.device());
        painter.setFont(f1);
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(x+shift, y);
        painter.drawText(QPointF(0,0), "|");
        painter.translate(fm1.boundingRect("8").width()/3.0, 0);
        painter.drawText(QPointF(0,0), "|");


    // here are some spaces
    } else if (symbolName==" ") { // full space (width of x)
    } else if (symbolName=="space") { // full space (width of x)
    } else if (symbolName=="qquad") { // full space(width of M)
    } else if (symbolName=="quad") { // full space(width of M)
    } else if (symbolName==";") { // 75% space
    } else if (symbolName==":") { // 50% space
    } else if (symbolName==",") { // 25% space
    } else if (symbolName=="!") { // -25% space
    } else if (symbolName=="longleftarrow") {
        const double width=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*3.0;
        const double dx=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*0.25;
        const double ypos=y-JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).height()/2.0;
        const QPainterPath path=JKQTMathTextMakeArrow(x+shift+dx, ypos, width, JKQTMathTextGetTightBoundingRect(f, "M", painter.device()).height()*0.5, true, false);
    } else if (symbolName=="longrightarrow"){
        const double width=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*3.0;
        const double dx=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*0.25;
        const double ypos=y-JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).height()/2.0;
        const QPainterPath path=JKQTMathTextMakeArrow(x+shift+dx, ypos, width, JKQTMathTextGetTightBoundingRect(f, "M", painter.device()).height()*0.5, false, true);
        painter.drawPath(path);
    } else if (symbolName=="Longleftarrow") {
        const double width=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*3.0;
        const double dx=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*0.25;
        const double ypos=y-JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).height()/2.0;
        const QPainterPath path=JKQTMathTextMakeDArrow(x+shift+dx, ypos, width, JKQTMathTextGetTightBoundingRect(f, "M", painter.device()).height()*0.5, true, false);
    } else if (symbolName=="Longrightarrow") {
        const double width=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*3.0;
        const double dx=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*0.25;
        const double ypos=y-JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).height()/2.0;
        const QPainterPath path=JKQTMathTextMakeDArrow(x+shift+dx, ypos, width, JKQTMathTextGetTightBoundingRect(f, "M", painter.device()).height()*0.5, false, true);
        painter.drawPath(path);
    } else if (symbolName=="longleftrightarrow") {
        const double width=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*3.0;
        const double dx=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*0.25;
        const double ypos=y-JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).height()/2.0;
        const QPainterPath path=JKQTMathTextMakeArrow(x+shift+dx, ypos, width, JKQTMathTextGetTightBoundingRect(f, "M", painter.device()).height()*0.5, true, true);
        painter.drawPath(path);
    } else if (symbolName=="Longleftrightarrow") {
        const double width=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*3.0;
        const double dx=JKQTMathTextGetTightBoundingRect(f, "X", painter.device()).width()*0.25;
        const double ypos=y-JKQTMathTextGetTightBoundingRect(f, "x", painter.device()).height()/2.0;
        const QPainterPath path=JKQTMathTextMakeDArrow(x+shift+dx, ypos, width, JKQTMathTextGetTightBoundingRect(f, "M", painter.device()).height()*0.5, true, true);
        painter.drawPath(path);
    } else { // draw a box to indicate an unavailable symbol
        const QRectF tbr=JKQTMathTextGetTightBoundingRect(f, "M", painter.device());
        painter.drawRect(QRectF(x+shift,y-tbr.height(), xwi, tbr.height()*0.8));
    }
    painter.setPen(pold);
    painter.setFont(fold);
    return x+width;
}

bool JKQTMathTextSymbolNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
    bool ok=true;
    QString s;
    JKQTMathTextEnvironment ev=currentEv;

    static QMap<QString, QString> entitylut;
    if (entitylut.isEmpty()) {
      entitylut.insert("sin", "sin");
      entitylut.insert("cos", "cos");
      entitylut.insert("tan", "tan");
      entitylut.insert("sinh", "sinh");
      entitylut.insert("cosh", "cosh");
      entitylut.insert("tanh", "tanh");
      entitylut.insert("atan", "atan");
      entitylut.insert("acos", "acos");
      entitylut.insert("asin", "asin");
      entitylut.insert("arcsin", "arcsin");
      entitylut.insert("arccos", "arccos");
      entitylut.insert("arctan", "arctan");
      entitylut.insert("ii", "i");
      entitylut.insert("dd", "d");
      entitylut.insert("exp", "exp");
      entitylut.insert("log", "log");
      entitylut.insert("ln", "ln");
      entitylut.insert("ld", "ld");
      entitylut.insert("lb", "lb");
      entitylut.insert("erf", "erf");
      entitylut.insert("mod", "mod");
      entitylut.insert("median", "median");
      entitylut.insert("min", "min");
      entitylut.insert("max", "max");
      entitylut.insert("argmin", "argmin");
      entitylut.insert("argmax", "argmax");
      entitylut.insert("inf", "inf");
      entitylut.insert("sup", "sup");
      entitylut.insert("liminf", "liminf");
      entitylut.insert("limsup", "limsup");
      entitylut.insert("lim", "lim");
      entitylut.insert("sec", "sec");
      entitylut.insert("gcd", "gcd");
      entitylut.insert("hom", "hom");
      entitylut.insert("ker", "ker");
      entitylut.insert("dim", "dim");
      entitylut.insert("cot", "cot");
      entitylut.insert("arg", "arg");
      entitylut.insert("det", "det");
      entitylut.insert("deg", "deg");
      entitylut.insert("sign", "sign");
      entitylut.insert("sgn", "sgn");
      entitylut.insert("Pr", "Pr");
      entitylut.insert("coth", "coth");
      entitylut.insert("alpha",  "&alpha;");
      entitylut.insert("beta",  "&beta;");
      entitylut.insert("gamma",  "&gamma;");
      entitylut.insert("delta",  "&delta;");
      entitylut.insert("epsilon",  "&epsilon;");
      entitylut.insert("varepsilon",  "&epsilon;");
      entitylut.insert("zeta",  "&zeta;");
      entitylut.insert("eta",  "&eta;");
      entitylut.insert("theta",  "&theta;");
      entitylut.insert("vartheta",  "&thetasym;");
      entitylut.insert("iota",  "&iota;");
      entitylut.insert("kappa",  "&kappa;");
      entitylut.insert("lambda",  "&lambda;");
      entitylut.insert("mu",  "&mu;");
      entitylut.insert("nu",  "&nu;");
      entitylut.insert("xi",  "&xi;");
      entitylut.insert("pi",  "&pi;");
      entitylut.insert("varpi",  "&piv;");
      entitylut.insert("rho",  "&rho;");
      entitylut.insert("sigma",  "&sigma;");
      entitylut.insert("varsigma",  "&sigmaf;");
      entitylut.insert("tau",  "&tau;");
      entitylut.insert("upsilon",  "&upsilon;");
      entitylut.insert("phi",  "&phi;");
      entitylut.insert("varphi",  "&phi;");
      entitylut.insert("chi",  "&chi;");
      entitylut.insert("psi",  "&psi;");
      entitylut.insert("omega",  "&omega;");
      entitylut.insert("Gamma",  "&Gamma;");
      entitylut.insert("Delta",  "&Delta;");
      entitylut.insert("Theta",  "&Theta;");
      entitylut.insert("Lambda",  "&Lambda;");
      entitylut.insert("Omega",  "&Omega;");
      entitylut.insert("Xi",  "&Xi;");
      entitylut.insert("Pi",  "&Pi;");
      entitylut.insert("Sigma",  "&Sigma;");
      entitylut.insert("Upsilon",  "&Upsilon;");
      entitylut.insert("Phi",  "&Phi;");
      entitylut.insert("Psi",  "&Psi;");
      entitylut.insert("leftrightarrow",  "&harr;");
      entitylut.insert("leftarrow",  "&larr;");
      entitylut.insert("rightarrow", "&rarr;");
      entitylut.insert("to", "&rarr;");
      entitylut.insert("uparrow",  "&uarr;");
      entitylut.insert("downarrow",  "&darr;");
      entitylut.insert("Leftrightarrow", "&hArr;");
      entitylut.insert("iff", "&hArr;");
      entitylut.insert("Leftarrow",  "&lArr;");
      entitylut.insert("Rightarrow",  "&rArr;");
      entitylut.insert("Uparrow",  "&uArr;");
      entitylut.insert("Downarrow",  "&dArr;");
      entitylut.insert("pm",  "&plusmn;");
      entitylut.insert("leq",  "&le;");
      entitylut.insert("geq",  "&ge;");
      entitylut.insert("times",  "&times;");
      entitylut.insert("propto",  "&prop;");
      entitylut.insert("partial",  "&part;");
      entitylut.insert("bullet",  "&bull;");
      entitylut.insert("star",  "&Star;");
      entitylut.insert("ast",  "&lowast;");
      entitylut.insert("asterisk",  "&lowast;");
      entitylut.insert("neq", "&ne;");
      entitylut.insert("ne", "&ne;");
      entitylut.insert("equiv",  "&equiv;");
      entitylut.insert("approx",  "&asymp;");
      entitylut.insert("ellipsis",  "...");
      entitylut.insert("Im",  "&image;");
      entitylut.insert("Re",  "&real;");
      entitylut.insert("otimes",  "&otimes;");
      entitylut.insert("oplus",  "&oplus;");
      entitylut.insert("oslash",  "/");
      entitylut.insert("cap", "&cap;");
      entitylut.insert("cup", "&cup;");
      entitylut.insert("land", "&cap;");
      entitylut.insert("lor", "&cup;");
      entitylut.insert("supset",  "&sup;");
      entitylut.insert("supseteq",  "&supe;");
      entitylut.insert("supsetnot",  "&nsup;");
      entitylut.insert("subset",  "&sub;");
      entitylut.insert("subseteq",  "&sube;");
      entitylut.insert("in",  "&isin;");
      entitylut.insert("notin",  "&notin;");
      entitylut.insert("angle",  "&ang;");
      entitylut.insert("nabla",  "&nabla;");
      entitylut.insert("copyright",  "&copy;");
      entitylut.insert("registered",  "&reg;");
      entitylut.insert("trademark",  "&trade;");
      entitylut.insert("textregistered",  "&trade;");
      entitylut.insert("cdot",  "&sdot;");
      entitylut.insert("neg",  "&not;");
      entitylut.insert("wedge",  "&and;");
      entitylut.insert("vee",  "&vee;");
      entitylut.insert("diamond",  "&loz;");
      entitylut.insert("langle",  "&lang;");
      entitylut.insert("rangle",  "&rang;");
      entitylut.insert("int", "&int;");
      entitylut.insert("forall", "&forall;");
      entitylut.insert("exists", "&exist;");
      entitylut.insert("cong", "&sim;");
      entitylut.insert("bot", "&perp;");
      entitylut.insert("llcorner", "&ulcorner;");
      entitylut.insert("lrcorner", "&urcorner;");
      entitylut.insert("ulcorner", "&ulcorner;");
      entitylut.insert("urcorner", "&urcorner;");
      entitylut.insert("blcorner", "&llcorner;");
      entitylut.insert("brcorner", "&llcorner;");
      entitylut.insert("tlcorner", "&lrcorner;");
      entitylut.insert("trcorner", "&lrcorner;");


      entitylut.insert("ll", "<<");
      entitylut.insert("gg", ">>");
      entitylut.insert("bbC", "<b>C</b>");
      entitylut.insert("bbH", "<b>H</b>");
      entitylut.insert("bbN", "<b>N</b>");
      entitylut.insert("bbP", "<b>P</b>");
      entitylut.insert("bbQ", "<b>Q</b>");
      entitylut.insert("bbR", "<b>R</b>");
      entitylut.insert("bbZ", "<b>Z</b>");
      entitylut.insert("Alef", "&alefsym;");
      entitylut.insert("alef", "&alefsym;");
      entitylut.insert("tilde",  "~");
      entitylut.insert("iint", "&int;&int;");
      entitylut.insert("iiint", "&int;&int;&int;");
      entitylut.insert("emptyset",  "&empty;");
      entitylut.insert("varnothing",  "&empty;");
      entitylut.insert("lceil",  "&lceil;");
      entitylut.insert("rceil",  "&rceil;");
      entitylut.insert("lfloor",  "&lfloor;");
      entitylut.insert("rfloor",  "&rfloor;");
      entitylut.insert("subsetnot",  "&nsub;");
      entitylut.insert("DC",  "=");
      entitylut.insert("cdots", "&sdot;&sdot;&sdot;");
      entitylut.insert("dots", "...");
      entitylut.insert("ldots", "...");

      entitylut.insert("cent", "&cent;");

      entitylut.insert("_", "_");
      entitylut.insert("}", "}");
      entitylut.insert("{", "{");
      entitylut.insert("hbar", "&#8463;");
      entitylut.insert("euro", "&euro;");
      entitylut.insert("pound", "&pound;");
      entitylut.insert("yen", "&yen;");
      entitylut.insert("div", "&divide;");
      entitylut.insert("backslash", "\\");
      entitylut.insert("$", "$");
      entitylut.insert("%", "%");
      entitylut.insert("&", "&");
      entitylut.insert("#", "#");
      entitylut.insert("|", "||");
      entitylut.insert("<", "&lt;");
      entitylut.insert(">", "&gt;");
      entitylut.insert("ast", "*");
      entitylut.insert("glq", "&OpenCurlyQuote;");
      entitylut.insert("grq", "&CloseCurlyQuote;");
      entitylut.insert("glqq", "&OpenCurlyDoubleQuote;");
      entitylut.insert("grqq", "&CloseCurlyDoubleQuote;");
      entitylut.insert("flq", "&lsaquo;");
      entitylut.insert("frq", "&rsaquo;");
      entitylut.insert("flqq", "&laquo;");
      entitylut.insert("frqq", "&raquo;");
      entitylut.insert("prime", "&prime;");
      entitylut.insert("dprime", "&Prime;");
      entitylut.insert("trprime", "&tprime;");
    }


    QMap<QString, QString>::iterator itS = entitylut.find(symbolName);
    if (itS!=entitylut.end()) { s=itS.value(); }
    else if (symbolName == "sum") {
        ev.fontSize*=parentMathText->getBigMathoperatorFontFactor(); s="&sum;"; }
    else if (symbolName == "prod") {
        ev.fontSize*=parentMathText->getBigMathoperatorFontFactor(); s="&prod;"; }
    else if (symbolName == "bigcap") { ev.fontSize*=parentMathText->getBigMathoperatorFontFactor(); s="&cap;"; }
    else if (symbolName == "bigcup") { ev.fontSize*=parentMathText->getBigMathoperatorFontFactor(); s="&cup;"; }
    else if (symbolName == "bigvee") { ev.fontSize*=parentMathText->getBigMathoperatorFontFactor(); s="&or;"; }
    else if (symbolName == "bighat") { ev.fontSize*=parentMathText->getBigMathoperatorFontFactor(); s="&and;"; }
    else ok=false;

    if (ok) html=html+ev.toHtmlStart(defaultEv)+s+ev.toHtmlAfter(defaultEv);
    return ok;
}

QString JKQTMathTextSymbolNode::getSymbolName() const {
    return this->symbolName;
}

QString JKQTMathTextSymbolNode::getSymbolfontName() const {
    JKQTMathTextEnvironment currentEv;
    auto props=getSymbolProp(symbolName, currentEv);
    return props.font;
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


