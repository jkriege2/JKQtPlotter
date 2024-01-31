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



#include "jkqtmathtext/parsers/jkqtmathtextlatexparser.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtcommon/jkqtpcodestructuring.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnodetools.h"
#include "jkqtmathtext/nodes/jkqtmathtexttextnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextbracenode.h"
#include "jkqtmathtext/nodes/jkqtmathtextdecoratednode.h"
#include "jkqtmathtext/nodes/jkqtmathtextfracnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextinstructionnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextlistnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextmatrixnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextsqrtnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextsubsupernode.h"
#include "jkqtmathtext/nodes/jkqtmathtextsymbolnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextwhitespacenode.h"
#include "jkqtmathtext/nodes/jkqtmathtextboxinstructionnode.h"
#include "jkqtmathtext/nodes/jkqtmathtexthorizontallistnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextmodifyenvironmentnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextnoopnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextverbatimnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextverticallistnode.h"
#include <QDebug>
#include <typeinfo>


QSet<QChar> JKQTMathTextLatexParser::TokenCharacters=QSet<QChar>()<<'-'<<'_'<<'^'<<'\\'<<'$'<<'&'<<'}'<<'{'<<'['<<']'<<'~';
QSet<QChar> JKQTMathTextLatexParser::mathEnvironmentSpecialChars=QSet<QChar>()<<'(' << '[' << '|' << ')' << ']' << '+' << '-' << '*' << '/' << '<' << '>' << '=';
QSet<QChar> JKQTMathTextLatexParser::mathEnvironmentSpecialEndChars=QSet<QChar>()<<'(' << '&' << '[' << '|' << ')' << ']' << '\\' << '$' << '{' << '}' << '_' << '^' << '+' << '-' << '/' << '*' << '=' << '<' << '>' << '~';
QSet<QChar> JKQTMathTextLatexParser::SingleCharInstructions=QSet<QChar>()<<'|'<<';'<<':'<<'!'<<','<<'_'<<'\\'<<'$'<<'%'<<'&'<<'#'<<'}'<<'{'<<' '<<'['<<']';
QSet<QString> JKQTMathTextLatexParser::mathEnvironmentSpecialText=QSet<QString>()<<"+"<<"-"<<"="<<"*"<<"<"<<">";
QHash<QString, QChar> JKQTMathTextLatexParser::accentLetters=QHash<QString, QChar>();
QSet<int> JKQTMathTextLatexParser::accentLetters_LenBackslash=QSet<int>();
QSet<int> JKQTMathTextLatexParser::accentLetters_LenCurly=QSet<int>();
QHash<QString,double> JKQTMathTextLatexParser::big_instructions_family;

//----------------------------------------------------------
// define some static sets for easy character lookup/identificattion
void JKQTMathTextLatexParser::initStaticStructures()
{
    static std::mutex sMutex;
    std::lock_guard<std::mutex> lock(sMutex);
    if (accentLetters.size()==0) {
        auto fAddUml=[](const QString& cmd, const QChar& letter, const QChar& ch) {
            QString i;
            if (cmd.size()>0 && !letter.isNull()) {
                if (cmd.size()==1 && !cmd[0].isLetter()) {
                    i="\\"+cmd+letter;
                    accentLetters[i]=ch; accentLetters_LenBackslash.insert(i.size());
                }
                i="{\\"+cmd+letter+"}";
                accentLetters[i]=ch; accentLetters_LenCurly.insert(i.size());
                i="\\"+cmd+"{"+letter+"}";
                accentLetters[i]=ch;  accentLetters_LenBackslash.insert(i.size());
            } else if  (cmd.size()>0 && letter.isNull()) {
                i="\\"+cmd+" ";
                accentLetters[i]=ch; accentLetters_LenBackslash.insert(i.size());
                i="\\"+cmd+"\t";
                accentLetters[i]=ch; accentLetters_LenBackslash.insert(i.size());
                i="\\"+cmd+"\n";
                accentLetters[i]=ch; accentLetters_LenBackslash.insert(i.size());
                i="\\"+cmd+"\\";
                accentLetters[i]=ch; accentLetters_LenBackslash.insert(i.size());
                i="{\\"+cmd+"}";
                accentLetters[i]=ch; accentLetters_LenCurly.insert(i.size());
            }
        };
        // instructions like \"{a}
        fAddUml("\"", 'A', QChar(0xC4));
        fAddUml("\"", 'E', QChar(0xCB));
        fAddUml("\"", 'I', QChar(0xCF));
        fAddUml("\"", 'O', QChar(0xD6));
        fAddUml("\"", 'U', QChar(0xDC));
        fAddUml("\"", 'Y', QChar(0x178));
        fAddUml("\"", 'a', QChar(0xE4));
        fAddUml("\"", 'e', QChar(0xEB));
        fAddUml("\"", 'i', QChar(0xEF));
        fAddUml("\"", 'o', QChar(0xF6));
        fAddUml("\"", 'u', QChar(0xFC));
        fAddUml("\"", 'y', QChar(0xFF));

        fAddUml("'", 'A', QChar(0xC1));
        fAddUml("'", 'E', QChar(0xC9));
        fAddUml("'", 'I', QChar(0xCD));
        fAddUml("'", 'O', QChar(0xD3));
        fAddUml("'", 'U', QChar(0xDA));
        fAddUml("'", 'Y', QChar(0xDD));
        fAddUml("'", 'a', QChar(0xE1));
        fAddUml("'", 'e', QChar(0xE9));
        fAddUml("'", 'i', QChar(0xED));
        fAddUml("'", 'o', QChar(0xF3));
        fAddUml("'", 'u', QChar(0xFA));
        fAddUml("'", 'y', QChar(0xFD));
        fAddUml("'", 'C', QChar(0x106));
        fAddUml("'", 'c', QChar(0x107));
        fAddUml("'", 'L', QChar(0x139));
        fAddUml("'", 'l', QChar(0x13A));
        fAddUml("'", 'N', QChar(0x143));
        fAddUml("'", 'n', QChar(0x144));
        fAddUml("'", 'R', QChar(0x154));
        fAddUml("'", 'r', QChar(0x155));
        fAddUml("'", 'S', QChar(0x15A));
        fAddUml("'", 's', QChar(0x15B));
        fAddUml("'", 'Z', QChar(0x179));
        fAddUml("'", 'z', QChar(0x17A));
        fAddUml("'", 'G', QChar(0x1F4));
        fAddUml("'", 'g', QChar(0x1F5));

        fAddUml("`", 'A', QChar(0xC0));
        fAddUml("`", 'E', QChar(0xC8));
        fAddUml("`", 'I', QChar(0xCC));
        fAddUml("`", 'O', QChar(0xD2));
        fAddUml("`", 'U', QChar(0xD9));
        fAddUml("`", 'a', QChar(0xE0));
        fAddUml("`", 'e', QChar(0xE8));
        fAddUml("`", 'i', QChar(0xEC));
        fAddUml("`", 'o', QChar(0xF2));
        fAddUml("`", 'u', QChar(0xF9));
        fAddUml("`", 'N', QChar(0x1F8));
        fAddUml("`", 'n', QChar(0x1F9));

        fAddUml("^", 'A', QChar(0xC2));
        fAddUml("^", 'E', QChar(0xCA));
        fAddUml("^", 'I', QChar(0xCE));
        fAddUml("^", 'O', QChar(0xD4));
        fAddUml("^", 'U', QChar(0xDB));
        fAddUml("^", 'a', QChar(0xE2));
        fAddUml("^", 'e', QChar(0xEA));
        fAddUml("^", 'i', QChar(0xEE));
        fAddUml("^", 'o', QChar(0xF4));
        fAddUml("^", 'u', QChar(0xFB));
        fAddUml("^", 'C', QChar(0x108));
        fAddUml("^", 'c', QChar(0x109));
        fAddUml("^", 'G', QChar(0x11C));
        fAddUml("^", 'g', QChar(0x11D));
        fAddUml("^", 'H', QChar(0x124));
        fAddUml("^", 'h', QChar(0x125));
        fAddUml("^", 'J', QChar(0x134));
        fAddUml("^", 'j', QChar(0x135));
        fAddUml("^", 'S', QChar(0x15C));
        fAddUml("^", 's', QChar(0x15D));
        fAddUml("^", 'W', QChar(0x174));
        fAddUml("^", 'w', QChar(0x175));
        fAddUml("^", 'Y', QChar(0x176));
        fAddUml("^", 'y', QChar(0x177));

        fAddUml("v", 'C', QChar(0x10C));
        fAddUml("v", 'c', QChar(0x10D));
        fAddUml("v", 'D', QChar(0x10E));
        fAddUml("v", 'd', QChar(0x10F));
        fAddUml("v", 'E', QChar(0x11A));
        fAddUml("v", 'e', QChar(0x11B));
        fAddUml("v", 'L', QChar(0x13D));
        fAddUml("v", 'l', QChar(0x13E));
        fAddUml("v", 'N', QChar(0x147));
        fAddUml("v", 'n', QChar(0x148));
        fAddUml("v", 'R', QChar(0x158));
        fAddUml("v", 'r', QChar(0x159));
        fAddUml("v", 'S', QChar(0x160));
        fAddUml("v", 's', QChar(0x161));
        fAddUml("v", 'T', QChar(0x164));
        fAddUml("v", 't', QChar(0x165));
        fAddUml("v", 'Z', QChar(0x17D));
        fAddUml("v", 'z', QChar(0x17E));
        fAddUml("v", 'A', QChar(0x1CD));
        fAddUml("v", 'a', QChar(0x1CE));
        fAddUml("v", 'I', QChar(0x1CF));
        fAddUml("v", 'i', QChar(0x1D0));
        fAddUml("v", 'O', QChar(0x1D1));
        fAddUml("v", 'o', QChar(0x1D2));
        fAddUml("v", 'U', QChar(0x1D3));
        fAddUml("v", 'u', QChar(0x1D4));
        fAddUml("v", 'G', QChar(0x1E6));
        fAddUml("v", 'g', QChar(0x1E7));
        fAddUml("v", 'K', QChar(0x1E8));
        fAddUml("v", 'k', QChar(0x1E9));
        fAddUml("v", 'j', QChar(0x1F0));
        fAddUml("v", 'H', QChar(0x21E));
        fAddUml("v", 'h', QChar(0x21F));

        fAddUml("~", 'A', QChar(0xC3));
        fAddUml("~", 'N', QChar(0xD1));
        fAddUml("~", 'O', QChar(0xD5));
        fAddUml("~", 'a', QChar(0xE3));
        fAddUml("~", 'n', QChar(0xF1));
        fAddUml("~", 'o', QChar(0xF5));
        fAddUml("~", 'I', QChar(0x128));
        fAddUml("~", 'i', QChar(0x129));
        fAddUml("~", 'U', QChar(0x168));
        fAddUml("~", 'u', QChar(0x169));

        fAddUml("r", 'A', QChar(0xC5));
        fAddUml("r", 'a', QChar(0xE5));
        fAddUml("r", 'U', QChar(0x16E));
        fAddUml("r", 'u', QChar(0x16F));


        fAddUml("=", 'A', QChar(0xC2));
        fAddUml("=", 'E', QChar(0xCA));
        fAddUml("=", 'I', QChar(0xCE));
        fAddUml("=", 'O', QChar(0xD4));
        fAddUml("=", 'U', QChar(0xDB));
        fAddUml("=", 'a', QChar(0xE2));
        fAddUml("=", 'e', QChar(0xEA));
        fAddUml("=", 'i', QChar(0xEE));
        fAddUml("=", 'o', QChar(0xF4));
        fAddUml("=", 'u', QChar(0xFB));
        fAddUml("=", 'Y', QChar(0x108));
        fAddUml("=", 'y', QChar(0x109));

        fAddUml(".", 'C', QChar(0x10A));
        fAddUml(".", 'c', QChar(0x10B));
        fAddUml(".", 'E', QChar(0x116));
        fAddUml(".", 'e', QChar(0x117));
        fAddUml(".", 'G', QChar(0x120));
        fAddUml(".", 'g', QChar(0x121));
        fAddUml(".", 'I', QChar(0x130));
        fAddUml(".", 'Z', QChar(0x17B));
        fAddUml(".", 'z', QChar(0x17C));
        fAddUml(".", 'A', QChar(0x226));
        fAddUml(".", 'a', QChar(0x227));
        fAddUml(".", 'O', QChar(0x22E));
        fAddUml(".", 'o', QChar(0x22F));
        fAddUml(".", 'B', QChar(0x1E02));
        fAddUml(".", 'b', QChar(0x1E03));
        fAddUml(".", 'D', QChar(0x1E0A));
        fAddUml(".", 'd', QChar(0x1E0B));
        fAddUml(".", 'F', QChar(0x1E1E));
        fAddUml(".", 'f', QChar(0x1E1F));
        fAddUml(".", 'H', QChar(0x1E22));
        fAddUml(".", 'h', QChar(0x1E23));
        fAddUml(".", 'M', QChar(0x1E40));
        fAddUml(".", 'm', QChar(0x1E41));
        fAddUml(".", 'N', QChar(0x1E44));
        fAddUml(".", 'n', QChar(0x1E45));
        fAddUml(".", 'P', QChar(0x1E56));
        fAddUml(".", 'p', QChar(0x1E57));
        fAddUml(".", 'R', QChar(0x1E58));
        fAddUml(".", 'r', QChar(0x1E59));
        fAddUml(".", 'S', QChar(0x1E60));
        fAddUml(".", 's', QChar(0x1E61));
        fAddUml(".", 'T', QChar(0x1E6A));
        fAddUml(".", 't', QChar(0x1E6B));
        fAddUml(".", 'W', QChar(0x1E86));
        fAddUml(".", 'w', QChar(0x1E87));
        fAddUml(".", 'X', QChar(0x1E8A));
        fAddUml(".", 'x', QChar(0x1E8B));
        fAddUml(".", 'Y', QChar(0x1E8E));
        fAddUml(".", 'y', QChar(0x1E8F));

        fAddUml("u", 'A', QChar(0x102));
        fAddUml("u", 'a', QChar(0x103));
        fAddUml("u", 'E', QChar(0x114));
        fAddUml("u", 'e', QChar(0x115));
        fAddUml("u", 'G', QChar(0x11E));
        fAddUml("u", 'g', QChar(0x11F));
        fAddUml("u", 'I', QChar(0x12C));
        fAddUml("u", 'i', QChar(0x12D));
        fAddUml("u", 'O', QChar(0x14E));
        fAddUml("u", 'o', QChar(0x14F));
        fAddUml("u", 'U', QChar(0x16C));
        fAddUml("u", 'u', QChar(0x16D));


        fAddUml("c", 'C', QChar(0xC7));
        fAddUml("c", 'c', QChar(0xE7));
        fAddUml("c", 'G', QChar(0x122));
        fAddUml("c", 'g', QChar(0x123));
        fAddUml("c", 'K', QChar(0x136));
        fAddUml("c", 'k', QChar(0x137));
        fAddUml("c", 'L', QChar(0x13B));
        fAddUml("c", 'l', QChar(0x13C));
        fAddUml("c", 'N', QChar(0x145));
        fAddUml("c", 'n', QChar(0x146));
        fAddUml("c", 'R', QChar(0x156));
        fAddUml("c", 'r', QChar(0x157));
        fAddUml("c", 'S', QChar(0x15E));
        fAddUml("c", 's', QChar(0x15F));
        fAddUml("c", 'T', QChar(0x162));
        fAddUml("c", 't', QChar(0x163));
        fAddUml("c", 'E', QChar(0x228));
        fAddUml("c", 'e', QChar(0x229));
        fAddUml("c", 'D', QChar(0x1E10));
        fAddUml("c", 'd', QChar(0x1E11));
        fAddUml("c", 'H', QChar(0x1E28));
        fAddUml("c", 'h', QChar(0x1E29));

        fAddUml("H", 'O', QChar(0x150));
        fAddUml("H", 'o', QChar(0x151));
        fAddUml("H", 'U', QChar(0x170));
        fAddUml("H", 'u', QChar(0x171));
        fAddUml("H", 'Y', QChar(0x4F2));
        fAddUml("H", 'y', QChar(0x4F3));

        fAddUml("k", 'A', QChar(0x104));
        fAddUml("k", 'a', QChar(0x105));
        fAddUml("k", 'E', QChar(0x118));
        fAddUml("k", 'e', QChar(0x119));
        fAddUml("k", 'I', QChar(0x12E));
        fAddUml("k", 'i', QChar(0x12F));
        fAddUml("k", 'U', QChar(0x172));
        fAddUml("k", 'u', QChar(0x173));
        fAddUml("k", 'O', QChar(0x1EA));
        fAddUml("k", 'u', QChar(0x1EB));

        // ligatures, instructions without {letter}
        fAddUml("ss", QChar(), QChar(0xDF));
        fAddUml("ae", QChar(), QChar(0xE6));
        fAddUml("AE", QChar(), QChar(0xC6));
        fAddUml("oe", QChar(), QChar(0x153));
        fAddUml("OE", QChar(), QChar(0x152));
        fAddUml("o", QChar(), QChar(0xF8));
        fAddUml("O", QChar(), QChar(0xD8));
        fAddUml("S", QChar(), QChar(0xA7));
        fAddUml("l", QChar(), QChar(0x142));
        fAddUml("L", QChar(), QChar(0x141));
        fAddUml("aa", QChar(), QChar(0xE5));
        fAddUml("AA", QChar(), QChar(0xC5));




        big_instructions_family["big"]=0.85;//1.2;
        big_instructions_family["bigl"]=0.85;//1.2;
        big_instructions_family["bigm"]=0.85;//1.2;
        big_instructions_family["bigr"]=0.85;//1.2;

        big_instructions_family["Big"]=1.15;//1.85;
        big_instructions_family["Bigl"]=1.15;//1.85;
        big_instructions_family["Bigm"]=1.15;//1.85;
        big_instructions_family["Bigr"]=1.15;//1.85;

        big_instructions_family["bigg"]=1.45;//2.4;
        big_instructions_family["biggl"]=1.45;//2.4;
        big_instructions_family["biggm"]=1.45;//2.4;
        big_instructions_family["biggr"]=1.45;//2.4;

        big_instructions_family["Bigg"]=1.75;//3.1;
        big_instructions_family["Biggl"]=1.75;//3.1;
        big_instructions_family["Biggm"]=1.75;//3.1;
        big_instructions_family["Biggr"]=1.75;//3.1;
    }

}


JKQTMathTextLatexParser::JKQTMathTextLatexParser(JKQTMathText* parent):
    JKQTMathTextParser(parent),
    currentToken(MTTnone),
    currentTokenName(""),
    currentTokenID(0),
    parseString(""),
    parsingMathEnvironment(false),
    parsinginMathTextStyle(false)
{
    initStaticStructures();
}

JKQTMathTextLatexParser::~JKQTMathTextLatexParser() {
}




JKQTMathTextNode *JKQTMathTextLatexParser::parse(const QString &text, JKQTMathText::ParseOptions options)
{
    initStaticStructures();
    JKQTMathTextNode* parsedNode=nullptr;
    QString ntext=text;
    if (options.testFlag(JKQTMathText::StartWithMathMode)) ntext=QString("$")+ntext+QString("$");
    if (options.testFlag(JKQTMathText::AddSpaceBeforeAndAfter)) ntext=QString("\\;")+ntext+QString("\\;");


    parseString=ntext;
    currentTokenID=-1;
    currentToken=MTTnone;
    currentTokenName="";
    parsingMathEnvironment=false;
    lastMatrixLineCommandCount.clear();

    clearErrorList();
    if (options.testFlag(JKQTMathText::AllowLinebreaks)) {
        parsedNode=parseMultilineLatexString(true, QString(""), MTHALeft, 1.0, MTSMDefaultSpacing, MTVOFirstLine);
    } else {
        parsedNode=parseLatexString(true);
    }
    parsedNode=simplifyJKQTMathTextNode(parsedNode);
    return parsedNode;
}


JKQTMathTextLatexParser::tokenType JKQTMathTextLatexParser::getToken() {
    currentTokenID++;
    if (currentTokenID>parseString.size()-1) return currentToken=MTTnone;
    QChar c=parseString[currentTokenID];
    currentTokenName="";

    //----------------------------------------------------------
    // check for emdash "---" or endash "--"
    if (c=='-' && !parsingMathEnvironment) {
        if (parseString.mid(currentTokenID, 3)=="---") {
            currentTokenID+=2;
            currentTokenName="---";
            return currentToken=MTTemdash;
        } else if (parseString.mid(currentTokenID, 2)=="--") {
            currentTokenID+=1;
            currentTokenName="--";
            return currentToken=MTTendash;
        } else {
            currentTokenName="-";
            return currentToken=MTThyphen;
        }
    }
    //----------------------------------------------------------
    // read an instruction name
    if (c=='\\') {
        //----------------------------------------------------------
        // parsing accent instructions like \ss \"{a} ...
        const QString next5=parseString.mid(currentTokenID, 5);
        if (!parsingMathEnvironment && next5!="\\char"){
            for (int len: accentLetters_LenBackslash) {
                const QString acc=parseString.mid(currentTokenID, len);
                if (acc.size()==len && accentLetters.contains(acc)) {
                    currentTokenName=accentLetters[acc];
                    currentTokenID+=acc.trimmed().size()-1; // forward the instruction, omit trailing whitespace in instruction
                    if (acc.endsWith('\\')) currentTokenID--;
                    return currentToken=MTTtext;
                }
            }
        }
        currentTokenID++;
        if (currentTokenID>=parseString.size()-1) return currentToken=MTTnone;
        c=parseString[currentTokenID];
        //----------------------------------------------------------
        // recognize linebreak "\\"
        if (c=='\\') {
            currentTokenName=c; // parse one-symbol instructions like \\, \& ...
            //std::cout<<"found text node '"<<currentTokenName.toStdString()<<"'\n";
            return currentToken=MTTinstructionNewline;
        }
        //----------------------------------------------------------
        // parsing single-character instruction
        if (SingleCharInstructions.contains(c)) {
            currentTokenName=c; // parse one-symbol instructions like \\, \& ...
            //std::cout<<"found text node '"<<currentTokenName.toStdString()<<"'\n";
            return currentToken=MTTinstruction;
        //----------------------------------------------------------
        // letter-only instruction name
        } else {
            while (c.isLetter()&& (currentTokenID<parseString.size())) {
                currentTokenName+=c;
                currentTokenID++;
                if (currentTokenID<parseString.size()) c=parseString[currentTokenID];
            }
            if (!c.isLetter()) currentTokenID--;
            currentTokenName=currentTokenName.trimmed();
        }
        //std::cout<<"found instruction node '"<<currentTokenName.toStdString()<<"'\n";
        if (currentTokenName.size()==0) addToErrorList(tr("error @ ch. %1: parser encountered empty istruction").arg(currentTokenID));
        else if (currentTokenName=="newline") return MTTinstructionNewline;
        else if (currentTokenName=="linebreak") return MTTinstructionNewline;
        else if (currentTokenName=="char") {
            QString num="";
            currentTokenID++;
            c=parseString[currentTokenID];
            if (c=='"') {
                // match '\char"HEXDIGITS'
                currentTokenID++;
                c=parseString[currentTokenID];
                while ((currentTokenID<parseString.size()) && (c.isDigit() || QString("aAbBcCdDeEfF").contains(c))) {
                    num+=c;
                    currentTokenID++;
                    c=parseString[currentTokenID];
                }
                if (currentTokenID<parseString.size()) currentTokenID--;
                currentTokenName=QString::fromStdString(jkqtp_UnicodeToUTF8(num.toLongLong(nullptr, 16)));
                return currentToken=MTTtext;
            } else if (c=='`' || c=='\'') {
                // match '\char"OCTALDIGITS'
                currentTokenID++;
                c=parseString[currentTokenID];
                while ((currentTokenID<parseString.size()) && (QString("01234567").contains(c))) {
                    num+=c;
                    currentTokenID++;
                    if (currentTokenID<parseString.size()) c=parseString[currentTokenID];
                    else c=QChar();
                }
                if (currentTokenID<parseString.size()) currentTokenID--;
                currentTokenName=QString::fromStdString(jkqtp_UnicodeToUTF8(num.toLongLong(nullptr, 8)));
                return currentToken=MTTtext;
            } else if (c.isDigit()) {
                // match '\charDECIMALDIGITS'
                while ((currentTokenID<parseString.size()) && (c.isDigit())) {
                    num+=c;
                    currentTokenID++;
                    c=parseString[currentTokenID];
                }
                if (currentTokenID<parseString.size()) currentTokenID--;
                currentTokenName=QString::fromStdString(jkqtp_UnicodeToUTF8(num.toLongLong(nullptr, 10)));
                return currentToken=MTTtext;
            }

        } else if (currentTokenName.startsWith("verb")) {
            if (currentTokenName.size()>4) currentTokenID-=(currentTokenName.size()-4);
            currentTokenID++;
            const QString verbEndChar=parseString.mid(currentTokenID, 1);
            currentTokenName=readUntil(true, verbEndChar);
            return currentToken=MTTinstructionVerbatim;
        } else if (currentTokenName=="begin") {
            currentTokenID++;
            if (parseString[currentTokenID]!='{') addToErrorList(tr("error @ ch. %1: didn't find '{' after '\\begin'").arg(currentTokenID)); // find closing brace '}' after '\\begin{name');
            currentTokenName=readUntil(true, "}");
            if (currentTokenName=="verbatim") {
                currentTokenName=readUntil(true, "\\end{verbatim}", true);
                return currentToken=MTTinstructionVerbatim;
            } else if (currentTokenName=="verbatim*") {
                currentTokenName=readUntil(true, "\\end{verbatim*}", true);
                return currentToken=MTTinstructionVerbatimVisibleSpace;
            } else if (currentTokenName=="lstlisting") {
                currentTokenName=readUntil(true, "\\end{lstlisting}", true);
                return currentToken=MTTinstructionVerbatim;
            }
            return currentToken=MTTinstructionBegin;
        } else if (currentTokenName=="end") {
            currentTokenID++;
            if (currentTokenID>=parseString.size() || parseString[currentTokenID]!='{') addToErrorList(tr("error @ ch. %1: didn't find '{' after '\\end'").arg(currentTokenID)); // find closing brace '}' after '\\begin{name');
            currentTokenName=readUntil(true, "}");
            return currentToken=MTTinstructionEnd;
        }
        return currentToken=MTTinstruction;
    //----------------------------------------------------------
    // check for $ character
    } else if (c=='$') {
        //std::cout<<"found dollar\n";
        return currentToken=MTTdollar;
    //----------------------------------------------------------
    // check for & character
    } else if (c=='&') {
        //std::cout<<"found ampersand\n";
        return currentToken=MTTampersand;
    //----------------------------------------------------------
    // check for ~ character
    } else if (c=='~') {
        //std::cout<<"found tilde\n";
        return currentToken=MTTtilde;
    //----------------------------------------------------------
    // check for { character
    } else if (c=='{') {
        //----------------------------------------------------------
        // parsing accent instructions like {\ss}
        if (!parsingMathEnvironment){
            for (int len: accentLetters_LenCurly) {
                const QString acc=parseString.mid(currentTokenID, len);
                if (acc.size()==len && accentLetters.contains(acc)) {
                    currentTokenName=accentLetters[acc];
                    currentTokenID+=acc.trimmed().size()-1; // forward the instruction, omit trailing whitespace in instruction
                    if (acc.endsWith('\\')) currentTokenID--;
                    return currentToken=MTTtext;
                }
            }
        }
        return currentToken=MTTopenbrace;
    //----------------------------------------------------------
    // check for } character
    } else if (c=='}') {
        return currentToken=MTTclosebrace;
    //----------------------------------------------------------
    // check for [ character
    } else if (c=='[') {
        return currentToken=MTTopenbracket;
    //----------------------------------------------------------
    // check for ] character
    } else if (c==']') {
        return currentToken=MTTclosebracket;
    //----------------------------------------------------------
    // check for _ character
    } else if (c=='_') {
        return currentToken=MTTunderscore;
    //----------------------------------------------------------
    // check for ^ character
    } else if (c=='^') {
        return currentToken=MTThat;
    //----------------------------------------------------------
    // check for whitespace character
    } else if (c.isSpace()) {
        while (c.isSpace() &&(currentTokenID<parseString.size())) { // eat up whitespace
            currentTokenID++;
            if (currentTokenID<parseString.size()) c=parseString[currentTokenID];
        }
        if (!c.isSpace()) currentTokenID--;
        //std::cout<<"found whitespace\n";
        return currentToken=MTTwhitespace;
    } else {
        //----------------------------------------------------------
        // no instruction or special character => parse text
        if (parsingMathEnvironment) {
            // inside math environments we split texts at every brace {[(|)]} so that
            // braces form their own JKQTMathTextTextNode and may be formated accordingly
            if (mathEnvironmentSpecialChars.contains(c)) {
                currentTokenName=c;
                //std::cout<<"found text node '"<<currentTokenName.toStdString()<<"'\n";
                return currentToken=MTTtext;
            }
            while (!mathEnvironmentSpecialEndChars.contains(c) && (currentTokenID<parseString.size())) {
                // add whitespaces only once
                if (c.isSpace()) {
                    if (!currentTokenName.isEmpty()) {
                        if (!currentTokenName[currentTokenName.size()-1].isSpace())
                            currentTokenName+=c;
                    }
                } else currentTokenName+=c;
                currentTokenID++;
                if (currentTokenID<parseString.size()) c=parseString[currentTokenID];
            }
            if (mathEnvironmentSpecialEndChars.contains(c) || c.isSpace()) currentTokenID--;
            //currentTokenName=currentTokenName.trimmed();
            //std::cout<<"found text node '"<<currentTokenName.toStdString()<<"'\n";
            return currentToken=MTTtext;
        } else {
            while ((!c.isSpace()) && !TokenCharacters.contains(c) && (currentTokenID<parseString.size())) {
                // add whitespaces only once
                if (c.isSpace()) {
                    if (!currentTokenName.isEmpty()) {
                        if (!currentTokenName[currentTokenName.size()-1].isSpace())
                            currentTokenName+=c;
                    }
                } else currentTokenName+=c;
                currentTokenID++;
                if (currentTokenID<parseString.size()) c=parseString[currentTokenID];
            }
            if (TokenCharacters.contains(c) || c.isSpace()) currentTokenID--;
            //currentTokenName=currentTokenName.trimmed();
            //std::cout<<"found text node '"<<currentTokenName.toStdString()<<"'\n";
            return currentToken=MTTtext;
        }
    }
    return currentToken=MTTnone;
}

void JKQTMathTextLatexParser::giveBackToTokenizer(size_t count)
{
    currentTokenID-=count;
}

JKQTMathTextNode* JKQTMathTextLatexParser::parseLatexString(bool get, JKQTMathTextBraceType quitOnClosingBrace, const QString& quitOnEnvironmentEnd, bool quitOnClosingBracket) {
    QMap<QString,size_t> countLine;
    //std::cout<<"    entering parseLatexString()\n";
    JKQTMathTextHorizontalListNode* nl=new JKQTMathTextHorizontalListNode(parentMathText);
    if (get) getToken();

    bool getNew=true;
    while (currentToken!=MTTnone) {
        getNew=true;
        if (currentToken==MTTtext) {
            QString text=currentTokenName;
            bool addWhite=false;
            if (!parsingMathEnvironment) {
                getToken();
                getNew=false;
                while (currentToken==MTTtext || currentToken==MTTwhitespace) {
                    if (currentToken==MTTtext) {
                        text+=currentTokenName;
                        getNew=true;
                    } else if (currentToken==MTTwhitespace) {
                        text+=" ";
                        getNew=true;
                    }
                    getToken();
                    getNew=false;
                }
                if (text.size()>0 && text[text.size()-1].isSpace()) {
                    addWhite=true;
                    text=text.left(text.size()-1);
                }
            }
            if (parsingMathEnvironment) {
                if (mathEnvironmentSpecialText.contains(text.trimmed()) && JKQTMathTextSymbolNode::hasSymbol(text.trimmed())) {
                    nl->addChild(new JKQTMathTextSymbolNode(parentMathText, text.trimmed()));
                } else {
                    nl->addChild(new JKQTMathTextTextNode(parentMathText, text, addWhite, parsingMathEnvironment));
                }
            } else {
                nl->addChild(new JKQTMathTextTextNode(parentMathText, text, addWhite, parsingMathEnvironment));
            }
        } else if (currentToken==MTTinstructionNewline) {
            break;
        } else if (currentToken==MTTwhitespace) {
            if (!parsingMathEnvironment) nl->addChild(new JKQTMathTextWhitespaceNode(parentMathText));
        } else if (currentToken==MTTtilde) {
            nl->addChild(new JKQTMathTextWhitespaceNode(JKQTMathTextWhitespaceNode::WSTNonbreaking, parentMathText));
        } else if (currentToken==MTTendash) {
            nl->addChild(new JKQTMathTextSymbolNode(parentMathText, "endash"));
        } else if (currentToken==MTTemdash) {
            nl->addChild(new JKQTMathTextSymbolNode(parentMathText, "emdash"));
        } else if (currentToken==MTThyphen) {
            nl->addChild(new JKQTMathTextSymbolNode(parentMathText, "hyphen"));
        } else if (currentToken==MTTinstruction) {
            const QString currentInstructionName=currentTokenName;
            if (currentInstructionName=="\\") break; // break on linebrak character
            if (currentInstructionName=="limits") {
                if (nl->hasChildren()) nl->getLastChild()->setSubSuperscriptAboveBelowNode(true);
            } else  if (currentInstructionName=="nolimits") {
                if (nl->hasChildren()) nl->getLastChild()->setSubSuperscriptAboveBelowNode(false);
            } else  if (currentInstructionName=="hline" || currentInstructionName=="midrule") {
                countLine["hline"]=countLine.value("hline",0)+1;
            } else  if (currentInstructionName=="hdashline") {
                countLine["hdashline"]=countLine.value("hdashline",0)+1;
            } else  if (currentInstructionName=="toprule" || currentInstructionName=="bottomrule") {
                countLine["heavyline"]=countLine.value("heavyline",0)+1;
            } else if (currentInstructionName=="right") {
                getToken();
                if (currentToken==MTTtext) {
                    if (currentTokenName.size()>0) {
                        bool tokenWasNoBrace=false;
                        const QString firstTokenChar(currentTokenName[0]);
                        if (TokenNameMatchesJKQTMathTextBraceType(firstTokenChar, quitOnClosingBrace, true, &tokenWasNoBrace)) {
                            lastRightBraceType=TokenName2JKQTMathTextBraceType(firstTokenChar);
                            if (quitOnClosingBrace!=MTBTAny) currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                            break;
                        } else {
                            getNew=false;
                        }
                    }
                } else if (currentToken==MTTinstruction) {
                    if (InstructionNameMatchesJKQTMathTextBraceType(currentTokenName, quitOnClosingBrace, true)) {
                        lastRightBraceType=InstructionName2JKQTMathTextBraceType(currentTokenName);
                        break;
                    }
                } else if (currentToken==MTTclosebracket) {
                    if (quitOnClosingBrace==MTBTSquareBracket || quitOnClosingBrace==MTBTAny) {
                        lastRightBraceType=MTBTSquareBracket;
                        break;
                    }
                } else {
                    addToErrorList(tr("error @ ch. %1: unexpected token after \\left").arg(currentTokenID));
                }
            } else if (currentInstructionName=="left") {
                getToken();
                if (currentToken==MTTtext) {
                    if (currentTokenName.size()>0) {
                        const QString firstTokenChar(currentTokenName[0]);
                        const JKQTMathTextBraceType bracetype=TokenName2JKQTMathTextBraceType(firstTokenChar);
                        if (bracetype==MTBTNone) {
                            currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                            JKQTMathTextNode* cn=parseLatexString(currentTokenName.size()<=0, MTBTAny);
                            nl->addChild(new JKQTMathTextBraceNode(parentMathText, MTBTNone, lastRightBraceType, cn));
                        } else if (isPrintableJKQTMathTextBraceType(bracetype)) {
                            currentTokenName=currentTokenName.right(currentTokenName.size()-1); // we already used the first character from the text token!
                            JKQTMathTextNode* c=parseLatexString(currentTokenName.size()<=0, bracetype);
                            nl->addChild(new JKQTMathTextBraceNode(parentMathText, bracetype, lastRightBraceType, c));
                        } else {
                            getNew=false;
                        }
                    }
                } else if (currentToken==MTTinstruction) {
                    const JKQTMathTextBraceType bracetypeopening=InstructionName2OpeningJKQTMathTextBraceType(currentTokenName);
                    if (bracetypeopening!=MTBTUnknown) {
                        JKQTMathTextNode* c=parseLatexString(true, bracetypeopening);
                        nl->addChild(new JKQTMathTextBraceNode(parentMathText, bracetypeopening, lastRightBraceType, c));
                    } else if (currentToken==MTTinstruction && TokenNameMatchesJKQTMathTextBraceType(currentTokenName, quitOnClosingBrace, true)) {
                        break;
                    }
                } else if (currentToken==MTTopenbracket) {
                    JKQTMathTextNode* c=parseLatexString(true, MTBTSquareBracket);
                    nl->addChild(new JKQTMathTextBraceNode(parentMathText, MTBTSquareBracket, lastRightBraceType, c));
                } else {
                    addToErrorList(tr("error @ ch. %1: unexpected token after \\left").arg(currentTokenID));
                }
            } else {
                bool foundError=false;
                JKQTMathTextNode* node=parseInstruction(&foundError, &getNew);
                if (node) {
                    if (foundError) {
                        delete node;
                        node=nullptr;
                    } else {
                        nl->addChild(node);
                    }
                }
            }
        } else if (currentToken==MTTunderscore) {
            getToken();
            JKQTMathTextNode* child=nullptr;
            JKQTMathTextNode* child2=nullptr;
            if (currentToken==MTTinstruction) {
                child=parseInstruction(nullptr, &getNew);
            } else if (currentToken==MTTopenbrace) {
                child=parseLatexString(true);
            } else if (currentToken==MTTtext) {
                if (currentTokenName.size()<=1) {
                    child=new JKQTMathTextTextNode(parentMathText, currentTokenName, false, parsingMathEnvironment);
                } else {
                    child=new JKQTMathTextTextNode(parentMathText, QString(currentTokenName[0]), false, parsingMathEnvironment);
                    child2=new JKQTMathTextTextNode(parentMathText, currentTokenName.right(currentTokenName.size()-1), false, parsingMathEnvironment);
                }
            } else {
                getNew=false;
            }
            if (child!=nullptr) nl->addChild(new JKQTMathTextSubscriptNode(parentMathText, child));
            if (child2!=nullptr) nl->addChild(child2);
        } else if (currentToken==MTThat) {
            getToken();
            JKQTMathTextNode* child=nullptr;
            JKQTMathTextNode* child2=nullptr;
            if (currentToken==MTTinstruction) {
                child=parseInstruction(nullptr, &getNew);
            } else if (currentToken==MTTopenbrace) {
                child=parseLatexString(true);
            } else if (currentToken==MTTtext) {
                if (currentTokenName.size()<=1) {
                    child=new JKQTMathTextTextNode(parentMathText, currentTokenName, false, parsingMathEnvironment);
                } else {
                    child=new JKQTMathTextTextNode(parentMathText, QString(currentTokenName[0]), false, parsingMathEnvironment);
                    child2=new JKQTMathTextTextNode(parentMathText, currentTokenName.right(currentTokenName.size()-1), false, parsingMathEnvironment);
                }
            } else {
                getNew=false;
            }
            if (child!=nullptr) nl->addChild(new JKQTMathTextSuperscriptNode(parentMathText, child));
            if (child2!=nullptr) nl->addChild(child2);
        } else if (currentToken==MTTopenbrace) {
            nl->addChild(new JKQTMathTextBlockNode(parseLatexString(true), parentMathText));
        } else if (currentToken==MTTclosebrace) {
            break;
        } else if (currentToken==MTTopenbracket) {
            nl->addChild(new JKQTMathTextTextNode(parentMathText, "[", false));
        } else if (currentToken==MTTinstructionVerbatim) {
            nl->addChild(new JKQTMathTextVerbatimNode(parentMathText, currentTokenName, false));
        } else if (currentToken==MTTinstructionVerbatimVisibleSpace) {
            nl->addChild(new JKQTMathTextVerbatimNode(parentMathText, currentTokenName, true));

        } else if (currentToken==MTTinstructionBegin) {
            const QString envname=currentTokenName;
            if (envname=="matrix" || envname=="array" || envname=="aligned" || envname=="align" || envname=="cases" || envname=="pmatrix"|| envname=="bmatrix"|| envname=="Bmatrix"|| envname=="vmatrix"|| envname=="Vmatrix" || envname=="tabular") {
                QString colspec="";
                if (envname=="tabular" || envname=="array") {
                    if (getToken()==MTTopenbrace) {
                        colspec=readUntil(true, "}");
                    } else {
                        addToErrorList(tr("error @ ch. %1: expected {COLUMNSPEC} after '\\begin{%2}'").arg(currentTokenID).arg(envname));
                    }
                }
                JKQTMathTextMatrixNode* matrixNode=new JKQTMathTextMatrixNode(parentMathText, colspec);
                QVector< QVector<JKQTMathTextNode*> > items;
                //int lines=0;
                //int cols=0;
                bool first=true;
                bool firstLine=true;
                QVector<JKQTMathTextNode*> line;
                qsizetype colCount=0;
                //std::cout<<"found \\begin{matrix}\n";
                while (first || currentToken==MTTampersand || currentToken==MTTinstructionNewline) {
                    while (getToken()==MTTwhitespace) ; // eat whitespace
                    JKQTMathTextNode* it=simplifyAndTrimJKQTMathTextNode(parseLatexString(false, MTBTAny, envname));
                    if (firstLine) {
                        if (lastMatrixLineCommandCount.value("hline",0)==1) {
                            matrixNode->setTopLine(JKQTMathTextMatrixNode::LTline);
                        } else if (lastMatrixLineCommandCount.value("hline",0)>1) {
                            matrixNode->setTopLine(JKQTMathTextMatrixNode::LTdoubleline);
                        } else if (lastMatrixLineCommandCount.value("heavyline",0)>0) {
                            matrixNode->setTopLine(JKQTMathTextMatrixNode::LTheavyline);
                        } else if (lastMatrixLineCommandCount.value("hdashline",0)==1) {
                            matrixNode->setTopLine(JKQTMathTextMatrixNode::LTdashed);
                        } else if (lastMatrixLineCommandCount.value("hdashline",0)>1) {
                            matrixNode->setTopLine(JKQTMathTextMatrixNode::LTdoubleDashed);
                        }
                    } else {
                        if (lastMatrixLineCommandCount.value("hline",0)==1) {
                            matrixNode->setRowBottomLine(items.size()-1, JKQTMathTextMatrixNode::LTline);
                        } else if (lastMatrixLineCommandCount.value("hline",0)>1) {
                            matrixNode->setRowBottomLine(items.size()-1, JKQTMathTextMatrixNode::LTdoubleline);
                        } else if (lastMatrixLineCommandCount.value("heavyline",0)>0) {
                            matrixNode->setRowBottomLine(items.size()-1, JKQTMathTextMatrixNode::LTheavyline);
                        } else if (lastMatrixLineCommandCount.value("hdashline",0)==1) {
                            matrixNode->setRowBottomLine(items.size()-1, JKQTMathTextMatrixNode::LTdashed);
                        } else if (lastMatrixLineCommandCount.value("hdashline",0)>1) {
                            matrixNode->setRowBottomLine(items.size()-1, JKQTMathTextMatrixNode::LTdoubleDashed);
                        }
                    }
                    if (currentToken==MTTampersand) {
                        //std::cout<<"  appending item\n";
                        line.append(it);
                    } else {
                        //std::cout<<"  appending item and line with "<<line.size()<<" items.\n";
                        if (currentToken==MTTinstructionEnd) {
                            JKQTMathTextMultiChildNode* mnc=dynamic_cast<JKQTMathTextMultiChildNode*>(it);
                            if (mnc && mnc->childCount()>0) {
                                line.append(it);
                            } else {
                                line.append(it);
                            }
                        } else {
                            line.append(it);
                        }
                        if (currentToken==MTTinstructionNewline || line.size()>0) {
                            colCount=qMax(colCount, static_cast<qsizetype>(line.size()));
                            if (line.size()==0 || (line.size()>1 && static_cast<qsizetype>(line.size())==colCount)) {
                                items.append(line);
                            } else if (line.size()>1 && static_cast<qsizetype>(line.size())!=colCount) {
                                addToErrorList(tr("error @ ch. %1: wrong number of entries widthin '\\begin{%2}...\\end{%2}'").arg(currentTokenID).arg(envname));
                            }
                        }
                        line.clear();
                        firstLine=false;
                    }
                    first=false;
                }
                //std::cout<<"  creating matrix-node with "<<items.size()<<" items.\n";
                matrixNode->setChildren(items);
                if (envname=="pmatrix") nl->addChild(new JKQTMathTextBraceNode(parentMathText, MTBTParenthesis, MTBTParenthesis, matrixNode));
                else if (envname=="cases") nl->addChild(new JKQTMathTextBraceNode(parentMathText, MTBTCurlyBracket, MTBTNone, matrixNode));
                else if (envname=="bmatrix") nl->addChild(new JKQTMathTextBraceNode(parentMathText, MTBTSquareBracket, MTBTSquareBracket, matrixNode));
                else if (envname=="Bmatrix") nl->addChild(new JKQTMathTextBraceNode(parentMathText, MTBTCurlyBracket, MTBTCurlyBracket, matrixNode));
                else if (envname=="vmatrix") nl->addChild(new JKQTMathTextBraceNode(parentMathText, MTBTSingleLine, MTBTSingleLine, matrixNode));
                else if (envname=="Vmatrix") nl->addChild(new JKQTMathTextBraceNode(parentMathText, MTBTDoubleLine, MTBTDoubleLine, matrixNode));
                else nl->addChild(matrixNode);
                //std::cout<<"  creating matrix-node ... done!\n";
            } else if (envname=="center" || envname=="document" || envname=="flushleft" || envname=="flushright") {
                JKQTMathTextHorizontalAlignment alignment=MTHALeft;
                if (envname=="document") alignment=MTHALeft;
                else alignment=String2JKQTMathTextHorizontalAlignment(envname);
                nl->addChild(parseMultilineLatexString(true, envname, alignment, 1.0, MTSMDefaultSpacing, MTVOFirstLine));

            } else if (envname=="framed" || envname=="shaded" || envname=="snugshade") {
                JKQTMathTextHorizontalAlignment alignment=MTHALeft;
                JKQTMathTextVerticalListNode* vlist = parseMultilineLatexString(true, envname, alignment, 1.0, MTSMDefaultSpacing, MTVOFirstLine);
                QStringList color;
                color<<jkqtp_QColor2String(Qt::lightGray);
                nl->addChild(new JKQTMathTextBoxInstructionNode(parentMathText, envname, vlist, color));
            } else {
                addToErrorList(tr("error @ ch. %1: unknown environment '%2'").arg(currentTokenID).arg(envname));
            }
        } else if (currentToken==MTTinstructionEnd) {
            QString envname=currentTokenName;
            if (envname==quitOnEnvironmentEnd) {
                break;
            } else {
                addToErrorList(tr("error @ ch. %1: '\\end{%2}' widthout preceding '\\begin{%3}'").arg(currentTokenID).arg(envname).arg(envname));
            }
        } else if (currentToken==MTTclosebracket) {
            if (quitOnClosingBracket) break;
            else nl->addChild(new JKQTMathTextTextNode(parentMathText, "]", false));
        } else if (currentToken==MTTampersand) {
            break;
        } else if (currentToken==MTTdollar) {
            if (parsingMathEnvironment) { // reached end of math environment
                parsingMathEnvironment=false;
                break;
            } else { // starting math environment
                parsingMathEnvironment=true;
                nl->addChild(new JKQTMathTextModifiedTextPropsInstructionNode(parentMathText, "equation", parseLatexString(true)));
            }
        }
        if (getNew) getToken();
    }
    //std::cout<<"    leaving parseLatexString()\n";
    lastMatrixLineCommandCount=countLine;

    return simplifyJKQTMathTextNode(nl);
}

JKQTMathTextVerticalListNode *JKQTMathTextLatexParser::parseMultilineLatexString(bool /*get*/, const QString &quitOnEnvironmentEnd, JKQTMathTextHorizontalAlignment _alignment, double _linespacingFactor, JKQTMathTextLineSpacingMode spacingMode_, JKQTMathTextVerticalOrientation _verticalOrientation)
{
    JKQTMathTextVerticalListNode* vlist = new JKQTMathTextVerticalListNode(parentMathText, _alignment, _linespacingFactor, spacingMode_, _verticalOrientation );
    bool first=true;
    while (first || currentToken==MTTinstructionNewline) {
        vlist->addChild(simplifyAndTrimJKQTMathTextNode(parseLatexString(true, MTBTAny, quitOnEnvironmentEnd)));
        first=false;
    }
    return vlist;

}

JKQTMathTextNode* JKQTMathTextLatexParser::parseInstruction(bool *_foundError, bool* getNew) {

    if (currentToken!=MTTinstruction) {
        if (_foundError) *_foundError=true;
        if (getNew) *getNew=false;
        addToErrorList(tr("error @ ch. %1: expected instruction token").arg(currentTokenID));
        return nullptr;
    }
    bool foundError=false;
    const QString currentInstructionName=currentTokenName;
    JKQTMathTextNode* child=nullptr;
    if (getNew) *getNew=true;
    if (JKQTMathTextWhitespaceNode::supportsInstructionName(currentInstructionName)) {
        if (getNew) *getNew=true;
        child= new JKQTMathTextWhitespaceNode(currentInstructionName, parentMathText);
    } else if (JKQTMathTextSymbolNode::hasSymbol(currentInstructionName)) {
        child=new JKQTMathTextSymbolNode(parentMathText, currentInstructionName);
        if (JKQTMathTextSymbolNode::isSubSuperscriptBelowAboveSymbol(currentInstructionName)) {
            if (parsingMathEnvironment) {
                if (parsinginMathTextStyle) {
                    child->setSubSuperscriptAboveBelowNode(false);
                } else {
                    child->setSubSuperscriptAboveBelowNode(true);
                }
            } else {
                child->setSubSuperscriptAboveBelowNode(false);
            }
        }
        if (getNew) *getNew=true;
    } else if (big_instructions_family.contains(currentInstructionName)) {
        // after \big,\bigl... we expect a symbol-instruction or at least one character of text
        while (getToken()==MTTwhitespace);// eat whitespace

        JKQTMathTextBraceType bracetype=MTBTUnknown;
        bool openbrace=true;
        if (currentToken==MTTinstruction) {
            bracetype=TokenName2JKQTMathTextBraceType(currentTokenName, &openbrace);
        } else if (currentToken==MTTtext) {
            const QString firstTokenChar(currentTokenName[0]);
            bracetype=TokenName2JKQTMathTextBraceType(firstTokenChar, &openbrace);
            if (bracetype!=MTBTUnknown) {
                giveBackToTokenizer(currentTokenName.size()-1);
            } else {
                giveBackToTokenizer(currentTokenName.size());
            }
        } else if (currentToken==MTTopenbracket) {
            bracetype=MTBTSquareBracket;
            openbrace=true;
        } else if (currentToken==MTTclosebracket) {
            bracetype=MTBTSquareBracket;
            openbrace=false;
        } else {
            addToErrorList(tr("error @ ch. %1: expected symbol-encoding instruction or a character after '\\%2' command, but found token type %3").arg(currentTokenID).arg(currentInstructionName).arg(tokenType2String(currentToken)));
        }
        if (bracetype!=MTBTUnknown) {
            JKQTMathTextEmptyBoxNode* sizeChild=new JKQTMathTextEmptyBoxNode(parentMathText, 0, JKQTMathTextEmptyBoxNode::EBUem, big_instructions_family[currentInstructionName], JKQTMathTextEmptyBoxNode::EBUem);
            if (openbrace) {
                child=new JKQTMathTextBraceNode(parentMathText, bracetype, MTBTNone, sizeChild);
            } else {
                child=new JKQTMathTextBraceNode(parentMathText, MTBTNone, bracetype, sizeChild);
            }
        } else {
            addToErrorList(tr("error @ ch. %1: expected symbol-encoding instruction or character after '\\%2' command").arg(currentTokenID).arg(currentInstructionName));
        }
        if (getNew) *getNew=true;
    } else if (JKQTMathTextModifiedEnvironmentInstructionNode::supportsInstructionName(currentInstructionName)) {
        const size_t Nparams=JKQTMathTextModifiedEnvironmentInstructionNode::countParametersOfInstruction(currentInstructionName);
        bool foundError=false;
        QStringList params;
        if (Nparams>0) {
            params=parseStringParams(true, Nparams, &foundError);
        }
        if (getToken()!=MTTwhitespace) {
            if (getNew) *getNew=false;
        }
        if (!foundError) {
            child=new JKQTMathTextModifiedEnvironmentInstructionNode(parentMathText, currentInstructionName, params);
            JKQTMathTextModifiedEnvironmentInstructionNode::modifyInMathTextStyleEnvironment(currentInstructionName, parsinginMathTextStyle, parentMathText, params);
        } else {
            addToErrorList(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(Nparams));
        }
    } else if (JKQTMathTextModifiedTextPropsInstructionNode::supportsInstructionName(currentInstructionName)) {
        const size_t Nparams=JKQTMathTextModifiedTextPropsInstructionNode::countParametersOfInstruction(currentInstructionName);
        bool foundError=false;
        const QStringList params=parseStringParams(true, Nparams, &foundError);
        if (!foundError) {
            if (getToken()==MTTopenbrace) {
                const bool oldParseMath=parsingMathEnvironment;
                auto __finalpaint=JKQTPFinally(std::bind([&oldParseMath](bool& parsingMathEnvironment) { parsingMathEnvironment=oldParseMath; }, std::ref(parsingMathEnvironment)));
                JKQTMathTextModifiedTextPropsInstructionNode::modifyInMathEnvironment(currentInstructionName, parsingMathEnvironment, parsinginMathTextStyle, params);
                child=new JKQTMathTextModifiedTextPropsInstructionNode(parentMathText, currentInstructionName, parseLatexString(true), params);

            } else {
                foundError=true;
            }
        }
        if (foundError){
            addToErrorList(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(Nparams+1));
        }
    } else if (JKQTMathTextSimpleInstructionNode::supportsInstructionName(currentInstructionName)) {
        const size_t Nparams=JKQTMathTextSimpleInstructionNode::countParametersOfInstruction(currentInstructionName);
        bool foundError=false;
        const QStringList params=parseStringParams(true, Nparams, &foundError);
        if (!foundError) {
            child=new JKQTMathTextSimpleInstructionNode(parentMathText, currentInstructionName, params);
        } else {
            addToErrorList(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(Nparams));
        }
    } else if (JKQTMathTextBoxInstructionNode::supportsInstructionName(currentInstructionName)) {
        const size_t Nparams=JKQTMathTextBoxInstructionNode::countParametersOfInstruction(currentInstructionName);
        bool foundError=false;
        const QStringList params=parseStringParams(true, Nparams, &foundError);
        if (!foundError) {
            if (getNew) *getNew=true;
            if (getToken()==MTTopenbrace) {
                const bool oldParseMath=parsingMathEnvironment;
                auto __finalpaint=JKQTPFinally(std::bind([&oldParseMath](bool& parsingMathEnvironment) { parsingMathEnvironment=oldParseMath; }, std::ref(parsingMathEnvironment)));
                JKQTMathTextBoxInstructionNode::modifyInMathEnvironment(currentInstructionName, parsingMathEnvironment, parsinginMathTextStyle, params);
                child=new JKQTMathTextBoxInstructionNode(parentMathText, currentInstructionName, parseLatexString(true), params);
            } else {
                foundError=true;
            }
        }
        if (foundError){
            addToErrorList(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(Nparams+1));
        }
    } else if (JKQTMathTextPhantomNode::supportsInstructionName(currentInstructionName)) {
        if (getNew) *getNew=true;
        if (getToken()==MTTopenbrace) {
            child=new JKQTMathTextPhantomNode(parentMathText, currentInstructionName, parseLatexString(true));
        } else {
            addToErrorList(tr("error @ ch. %1: expected one argument in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
        }
    } else if (currentInstructionName=="substack" || currentInstructionName=="rsubstack" || currentInstructionName=="lsubstack") {
        getToken();
        JKQTMathTextHorizontalAlignment alignment=MTHACentered;
        if (currentInstructionName=="rsubstack") alignment=MTHARight;
        if (currentInstructionName=="lsubstack") alignment=MTHALeft;

        if (currentToken==MTTopenbracket) {
            alignment=String2JKQTMathTextHorizontalAlignment(parseSingleString(true));
            if (currentToken!=MTTclosebracket) {
                addToErrorList(tr("error @ ch. %1: didn't find closing brace ']' after '\\%2[]{}' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
            }
            getToken();
        }
        if (currentToken==MTTopenbrace) {
            child=parseMultilineLatexString(true, "", alignment, 1.0, MTSMMinimalSpacing, MTVOFirstLine);
            if (currentToken!=MTTclosebrace) addToErrorList(tr("error @ ch. %1: didn't find closing brace '}' after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
        } else {
            addToErrorList(tr("error @ ch. %1: expected one argument in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
        }
    } else if (currentInstructionName=="sqrt") {
        getToken();
        if (currentToken==MTTopenbrace) {
            child=new JKQTMathTextSqrtNode(parentMathText, parseLatexString(true));
        } else if (currentToken==MTTopenbracket) {
            JKQTMathTextNode* n1=parseLatexString(true, MTBTAny, "", true);
            JKQTMathTextNode* n2=nullptr;
            if (getToken()==MTTopenbrace) n2=parseLatexString(true);
            else addToErrorList(tr("error @ ch. %1: expected one argument in '{' braces after '%2' command with an optional argument in []").arg(currentTokenID).arg(currentInstructionName));

            if (n1 && n2) {
                child=new JKQTMathTextSqrtNode(parentMathText, n2, n1);
            } else {
                if (n1) delete n1;
                if (n2) delete n2;
                addToErrorList(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
            }
        } else {
            addToErrorList(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
        }
    } else if (currentInstructionName=="cbrt") {
        if (getToken()==MTTopenbrace) {
            child=new JKQTMathTextSqrtNode(parentMathText, parseLatexString(true), new JKQTMathTextTextNode(parentMathText, "3", false));
        } else {
            addToErrorList(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
        }
    } else if (currentInstructionName=="verb") {
        if (getToken()==MTTopenbrace) {
            QString text="";
            currentTokenID++;
            if (currentTokenID<=parseString.size()-1) {
                QChar c=parseString[currentTokenID];
                while (c!='}' && (currentTokenID<parseString.size())) {
                    text=text+c;
                    currentTokenID++;
                    if (currentTokenID<parseString.size()) c=parseString[currentTokenID];
                }
                if (c!='}')  addToErrorList(tr("error @ ch. %1: \verb{...} not closed by '}'").arg(currentTokenID).arg(currentInstructionName));
                child=new JKQTMathTextTextNode(parentMathText, text, false);
            }
        } else {
            addToErrorList(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
        }
    } else if (JKQTMathTextDecoratedNode::supportsInstructionName(currentInstructionName)) {
        if (getToken()==MTTopenbrace) {
            child=new JKQTMathTextDecoratedNode(parentMathText, JKQTMathTextDecoratedNode::InstructionName2DecorationType(currentInstructionName), parseLatexString(true));
        } else {
            addToErrorList(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
        }
    } else if (JKQTMathTextFracNode::supportsInstructionName(currentInstructionName)) {
        if (getToken()==MTTopenbrace) {
            JKQTMathTextNode* n1=parseLatexString(true);
            JKQTMathTextNode* n2=nullptr;
            if (getToken()==MTTopenbrace) n2=parseLatexString(true);
            if (n1 && n2) child=new JKQTMathTextFracNode(parentMathText, n1, n2, JKQTMathTextFracNode::InstructionName2FracType(currentInstructionName));
            else addToErrorList(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));

        } else {
            addToErrorList(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
        }
    } else if (currentInstructionName=="binom") {
        if (getToken()==MTTopenbrace) {
            JKQTMathTextNode* n1=parseLatexString(true);
            JKQTMathTextNode* n2=nullptr;
            if (getToken()==MTTopenbrace) n2=parseLatexString(true);
            if (n1 && n2) child=new JKQTMathTextBraceNode(parentMathText, MTBTParenthesis, MTBTParenthesis, new JKQTMathTextFracNode(parentMathText, n1, n2, JKQTMathTextFracNode::MTFMstackrel));
            else addToErrorList(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName));
        } else {
            addToErrorList(tr("error @ ch. %1: expected %3 arguments in '{...}' braces after '%2' command").arg(currentTokenID).arg(currentInstructionName).arg(1));
        }
    } else {
        if (getNew) *getNew=true;
        foundError=true;
        addToErrorList(tr("error @ ch. %1: unknown instruction \\%2 or unsupported instruction at this location").arg(currentTokenID).arg(currentInstructionName));
    }
    if (_foundError) *_foundError=foundError;
    return child;
}


QStringList JKQTMathTextLatexParser::parseStringParams(bool get, size_t Nparams, bool *foundError) {
    const bool old_parsingMathEnvironment=parsingMathEnvironment;
    auto reset_parsingMathEnvironment=JKQTPFinally([&]() { parsingMathEnvironment=old_parsingMathEnvironment; });
    if (*foundError) *foundError=false;
    if (Nparams<=0) return QStringList();
    else {
        QStringList params;
        for (size_t n=0; n<Nparams; n++) {
            if (n>0 || (n==0 && get)) getToken();
            if (currentToken==MTTopenbrace) {
                bool ok=true;
                QString thisparam="";
                while (ok) {
                    getToken();
                    if (currentToken==MTTtext) {
                        thisparam+=currentTokenName;
                    } else if (currentToken==MTTwhitespace) {
                        thisparam+=" ";
                    } else if (currentToken==MTTclosebrace) {
                        params.append(thisparam);
                        ok=false;
                    } else {
                        if (*foundError) *foundError=true;
                        return params;
                    }
                }
            } else {
                if (*foundError) *foundError=true;
                return params;
            }
        }
        return params;
    }
}

QString JKQTMathTextLatexParser::parseSingleString(bool get) {
    const bool old_parsingMathEnvironment=parsingMathEnvironment;
    auto reset_parsingMathEnvironment=JKQTPFinally([&]() { parsingMathEnvironment=old_parsingMathEnvironment; });
    bool ok=true;
    QString thisparam="";
    while (ok) {
        if (get) getToken();
        get=true;
        if (currentToken==MTTtext) {
            thisparam+=currentTokenName;
        } else if (currentToken==MTTwhitespace) {
            thisparam+=" ";
        } else {
            ok=false;
        }
    }
    return thisparam;
}

QString JKQTMathTextLatexParser::readUntil(bool get, const QString &endsequence, bool removeFirstlineWhitespace)
{
    if (get) currentTokenID++;
    QString seq;
    while (currentTokenID<parseString.size() && !seq.endsWith(endsequence)) {
        seq+=parseString[currentTokenID];
        currentTokenID++;
    }
    currentTokenID--;
    if (seq.endsWith(endsequence)) {
        seq=seq.left(seq.size()-endsequence.size());
    }
    if (removeFirstlineWhitespace) {
        QStringList sl=seq.split('\n');
        if (sl.size()>0) {
            bool allWS=true;
            for (const QChar& ch: sl.first()) {
                if (!ch.isSpace()) {
                    allWS=false;
                    break;
                }
            }
            if (allWS) sl.removeFirst();
        }
        if (sl.size()>0) {
            bool allWS=true;
            for (const QChar& ch: sl.last()) {
                if (!ch.isSpace()) {
                    allWS=false;
                    break;
                }
            }
            if (allWS) sl.removeLast();
        }
        seq=sl.join("\n");
    }

    return seq;
}



QString JKQTMathTextLatexParser::tokenType2String(tokenType type)
{
    switch(type) {
      case MTTnone: return "MTTnone";
      case MTTtext: return "MTTtext";
      case MTTinstruction: return "MTTinstruction";
      case MTTinstructionNewline: return "MTTinstructionNewline";
      case MTTunderscore: return "MTTunderscore";
      case MTThat: return "MTThat";
      case MTTdollar: return "MTTdollar";
      case MTTopenbrace: return "MTTopenbrace";
      case MTTclosebrace: return "MTTclosebrace";
      case MTTopenbracket: return "MTTopenbracket";
      case MTTclosebracket: return "MTTclosebracket";
      case MTTwhitespace: return "MTTwhitespace";
      case MTTampersand: return "MTTampersand";
      case MTThyphen: return "MTThyphen";
      case MTTendash: return "MTTendash";
      case MTTemdash: return "MTTemdash";
      case MTTinstructionVerbatim: return "MTTinstructionVerbatim";
      case MTTinstructionVerbatimVisibleSpace: return "MTTinstructionVerbatimVisibleSpace";
      case MTTinstructionBegin: return "MTTinstructionBegin";
      case MTTinstructionEnd: return "MTTinstructionEnd";
      case MTTtilde: return "MTTtilde";
    }
    return "???";
}





