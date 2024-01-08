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



#include "jkqtmathtext/nodes/jkqtmathtexttextnode.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
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





JKQTMathTextTextBaseNode::JKQTMathTextTextBaseNode(JKQTMathText *parent, const QString &text_):
    JKQTMathTextNode(parent),
    text(text_)
{

}

JKQTMathTextTextBaseNode::~JKQTMathTextTextBaseNode()
{

}

QString JKQTMathTextTextBaseNode::getText() const
{
    return text;
}




QString JKQTMathTextTextBaseNode::textTransform(const QString &text, const JKQTMathTextEnvironment &/*currentEv*/) const
{
    return  text;
}


bool JKQTMathTextTextBaseNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const {
    html=html
            +currentEv.toHtmlStart(defaultEv, parentMathText)
            +textTransform(text, currentEv).toHtmlEscaped()
            +currentEv.toHtmlAfter(defaultEv, parentMathText);
    return true;
}



const QHash<QChar, uint32_t>& JKQTMathTextTextNode::blackboardUnicodeTable(){
    static QHash<QChar, uint32_t> table=[]() {
        QHash<QChar, uint32_t> blackboardUnicodeTable;

        const QString ALPHA="ABDEFGIJKLMOSTUVWXYZ";
        for (const QChar ch: ALPHA) {
            blackboardUnicodeTable[ch]=0x1D538+(ch.unicode()-QChar('A').unicode());
        }
        const QString alpha="abcdefghijklmnopqrstuvwxyz";
        for (const QChar ch: alpha) {
            blackboardUnicodeTable[ch]=0x1D552+(ch.unicode()-QChar('a').unicode());
        }
        const QString nums="0123456789";
        for (const QChar ch: nums) {
            blackboardUnicodeTable[ch]=0x1D7D8+(ch.unicode()-QChar('0').unicode());
        }

        blackboardUnicodeTable['C']=0x2102;
        blackboardUnicodeTable['H']=0x210D;
        blackboardUnicodeTable['N']=0x2115;
        blackboardUnicodeTable['P']=0x2119;
        blackboardUnicodeTable['Q']=0x211A;
        blackboardUnicodeTable['R']=0x211D;
        blackboardUnicodeTable['Z']=0x2124;

        return blackboardUnicodeTable;
    }();
    return table;
}

JKQTMathTextTextNode::JKQTMathTextTextNode(JKQTMathText* _parent, const QString& textIn, bool addWhitespace, bool stripInnerWhitepace):
    JKQTMathTextTextBaseNode(_parent, "")
{
    QString textTransformed=textIn;

    if (stripInnerWhitepace) {
        textTransformed="";
        for (int i=0; i<textIn.size(); i++) {
            if (!textIn[i].isSpace()) textTransformed+=textIn[i];
        }
    }

    text=textTransformed;
    // strip all whitespace from left
    while (text.size()>1 && text[0].isSpace()) {
        text=text.right(text.size()-1);
    }
    if (addWhitespace && (text.size()>0) && (!text[text.size()-1].isSpace())) text=text+" ";
    //qDebug()<<"JKQTMathTextTextNode( text="<<text<<" addWhitespace="<<addWhitespace<<")   [=> this->text="<<this->text<<"]";
}

JKQTMathTextTextNode::~JKQTMathTextTextNode() = default;

JKQTMathTextNodeSize JKQTMathTextTextNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const {
    return calcLayout(painter, currentEv);
}

JKQTMathTextTextNode::LayoutInfo JKQTMathTextTextNode::calcLayout(QPainter &painter, JKQTMathTextEnvironment currentEv) const
{
    LayoutInfo l;

    const QString txt=textTransform(text, currentEv);
    splitTextForLayout(painter, currentEv, txt, l.textpart, l.fontMode);

    const QFont f=currentEv.getFont(parentMathText);
    const QFont fUpright=JKQTMathTextGetNonItalic(f);
    const QFont fFallbackSym=currentEv.exchangedFontFor(MTEFallbackSymbols).getFont(parentMathText);
    const QFont fRoman=currentEv.exchangedFontForRoman().getFont(parentMathText);
    const double sp=JKQTMathTextGetHorAdvance(f, " ", painter.device());
    l.width=0;
    double ascent=0;
    double descent=0;
    for (int i=0; i<l.textpart.size(); i++) {
        l.baselineXCorrection=0;
        l.topXCorrection=0;
        QRectF br, tbr;
        switch(l.fontMode[i]) {
            case FMasDefined:
            case FMasDefinedOutline:
                br=JKQTMathTextGetBoundingRect(f, l.textpart[i], painter.device());
                tbr=JKQTMathTextGetTightBoundingRect(f, l.textpart[i], painter.device());
                if (f.italic() && l.textpart[i].size()>0) l.baselineXCorrection=JKQTMathTextGetRightBearing(f,l.textpart[i].operator[](l.textpart[i].size()-1),painter.device());
                break;
            case FMasDefinedForceUpright:
                br=JKQTMathTextGetBoundingRect(fUpright, l.textpart[i], painter.device());
                tbr=JKQTMathTextGetTightBoundingRect(fUpright, l.textpart[i], painter.device());
                break;
            case FMroman:
                br=JKQTMathTextGetBoundingRect(fRoman, l.textpart[i], painter.device());
                tbr=JKQTMathTextGetTightBoundingRect(fRoman, l.textpart[i], painter.device());
                if (fRoman.italic() && l.textpart[i].size()>0) l.baselineXCorrection=JKQTMathTextGetRightBearing(fRoman,l.textpart[i].operator[](l.textpart[i].size()-1),painter.device());
                break;
            case FMfallbackSymbol:
                br=JKQTMathTextGetBoundingRect(fFallbackSym, l.textpart[i], painter.device());
                tbr=JKQTMathTextGetTightBoundingRect(fFallbackSym, l.textpart[i], painter.device());
                if (fFallbackSym.italic() && l.textpart[i].size()>0) l.baselineXCorrection=JKQTMathTextGetRightBearing(fFallbackSym,l.textpart[i].operator[](l.textpart[i].size()-1),painter.device());
                break;
        }
        l.textpartXPos.append(l.width);
        if (i==l.textpart.size()-1) l.width+=tbr.width();
        else l.width+=br.width();
        if (l.textpart[i].size()>0 && l.textpart[i].at(l.textpart[i].size()-1).isSpace()) {
            // this correction is necessary, because it seems that QFontMetricsF::boundingRect() ignores trailing spaces
            l.width+=sp;
        }
        const double thisAscent=-tbr.top();
        const double thisDescent=tbr.bottom();
        ascent=qMax(ascent, thisAscent);
        descent=qMax(descent, thisDescent);
    }
    l.overallHeight=(ascent+descent); //fm.height();
    l.baselineHeight=ascent;
    l.strikeoutPos=JKQTMathTextGetFontStrikoutPos(f, painter.device());
    return l;
}

void JKQTMathTextTextNode::splitTextForLayout(QPainter &painter, JKQTMathTextEnvironment currentEv, const QString &txt, QStringList &textpart, QList<FontMode> &fontMode) const
{
    auto isForcedUprightChar=[](const QChar& c) {
        return c.isDigit()
                || c=='(' || c=='[' || c=='|' || c==']' || c==')' || c=='<' || c=='>'|| c=='{' || c=='}' || c=='|'
                || c==QChar(0x2329) || c==QChar(0x232A) || c==QChar(0x2308) || c==QChar(0x2309) || c==QChar(0x230A) || c==QChar(0x230B);
    };

    //const QFont f=currentEv.getFont(parentMathText);
    //const QFont fUpright=JKQTMathTextGetNonItalic(f);
    const QFont fFallbackSym=currentEv.exchangedFontFor(MTEFallbackSymbols).getFont(parentMathText);
    const QFont fRoman=currentEv.exchangedFontForRoman().getFont(parentMathText);
    //const QFontMetricsF fm(f, painter.device());
    //const QFontMetricsF fmUpright(fUpright, painter.device());
    const QFontMetricsF fmFallbackSym(fFallbackSym, painter.device());
    const QFontMetricsF fmRoman(fRoman, painter.device());
    const JKQTMathTextBlackboradDrawingMode bbMode=parentMathText->getFontBlackboradMode();

    textpart.clear();
    fontMode.clear();
    QString currentSection="";
    FontMode currentSectionFontMode=FMasDefined;
    int i=0;
    while (i<txt.size()) {
        const QChar c=txt[i];
        QString cs=c;
        FontMode CFontMode=FMasDefined;
        const bool CisForcedUprightChar=isForcedUprightChar(c);
        const bool CisForcedUprightCharExt=CisForcedUprightChar||(c=='.')||(c==',');
        if (currentEv.insideMath && currentEv.insideMathForceDigitsUpright && (currentEv.font==MTEroman || currentEv.font==MTEmathRoman)) {
            if (currentSection.size()==0) {
                if (CisForcedUprightChar) {
                    CFontMode=FMasDefinedForceUpright;
                }
            } else {
                if (CisForcedUprightCharExt) {
                    CFontMode=FMasDefinedForceUpright;
                }
            }
        } else if (currentEv.font==MTEblackboard) {
            if (bbMode==MTBBDMfontDirectly) {
                CFontMode=FMasDefined;
            } else if (bbMode==MTBBDMsimulate) {
                CFontMode=FMasDefinedOutline;
            } else if (bbMode==MTBBDMunicodeCharactersOrSimulate || bbMode==MTBBDMunicodeCharactersOrFontDirectly) {
                if (blackboardUnicodeTable().contains(c) && fmRoman.inFontUcs4(blackboardUnicodeTable().operator[](c))) {
                    cs=jkqtp_UnicodeToUTF8Q(blackboardUnicodeTable().operator[](c));
                    CFontMode=FMroman;
                } else if (blackboardUnicodeTable().contains(c) && fmFallbackSym.inFontUcs4(blackboardUnicodeTable().operator[](c))) {
                    cs=jkqtp_UnicodeToUTF8Q(blackboardUnicodeTable().operator[](c));
                    CFontMode=FMfallbackSymbol;
                } else {
                    if (bbMode==MTBBDMunicodeCharactersOrSimulate) {
                        CFontMode=FMasDefinedOutline;
                    } else if (bbMode==MTBBDMunicodeCharactersOrFontDirectly) {
                        CFontMode=FMasDefined;
                    }
                }
            }
        }

        if (currentSection.size()==0) {
            // start new section
            currentSectionFontMode=CFontMode;
            currentSection+=cs;
        } else {
            // existing section
            if (CFontMode==currentSectionFontMode) {
                // continue current section
                currentSection+=cs;
            } else {
                // start new section
                textpart.append(currentSection);
                fontMode.append(currentSectionFontMode);
                currentSection=cs;
                currentSectionFontMode=CFontMode;
            }
        }

        i++;
    }
    if (currentSection.size()>0) {
        textpart.append(currentSection);
        fontMode.append(currentSectionFontMode);
    }
}

double JKQTMathTextTextNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const {
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTMathTextTextNode[]::draw()"));
#endif
    const LayoutInfo l=calcLayout(painter, currentEv);
    doDrawBoxes(painter, x, y, l);


    const QFont f=currentEv.getFont(parentMathText);
    const QFont fUpright=JKQTMathTextGetNonItalic(f);
    const QFont fFallbackSym=currentEv.exchangedFontFor(MTEFallbackSymbols).getFont(parentMathText);
    const QFont fRoman=currentEv.exchangedFontForRoman().getFont(parentMathText);
    //const QFontMetricsF fm(f, painter.device());
    //const QFontMetricsF fmUpright(fUpright, painter.device());
    //const QFontMetricsF fmFallbackSym(fFallbackSym, painter.device());
    //const QFontMetricsF fmRoman(fRoman, painter.device());

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setFont(f);
    painter.setPen(currentEv.color);

    //qDebug()<<"JKQTMathTextTextNode: text="<<text<<" font="<<f;

    //std::cout<<"  TEXT: currentEv.mathMode="<<currentEv.insideMath<<", currentEv.forceUpright="<<currentEv.insideMathForceDigitsUpright<<"\n";
    for (int i=0; i<l.textpart.size(); i++) {
        //std::cout<<"  TEXT: mode="<<fontMode[i]<<", text='"<<textpart[i].toStdString()<<"'\n";
        switch(l.fontMode[i]) {
            case FMasDefined:
                painter.setFont(f);
                painter.drawText(QPointF(x+l.textpartXPos[i], y), l.textpart[i]);
                break;
            case FMasDefinedOutline:
                JKQTMathTextDrawStringSimBlackboard(painter, f, currentEv.color, x+l.textpartXPos[i], y, l.textpart[i]);
                break;
            case FMasDefinedForceUpright:
                painter.setFont(fUpright);
                painter.drawText(QPointF(x+l.textpartXPos[i], y), l.textpart[i]);
                break;
            case FMroman:
                painter.setFont(fRoman);
                painter.drawText(QPointF(x+l.textpartXPos[i], y), l.textpart[i]);
                break;
            case FMfallbackSymbol:
                painter.setFont(fFallbackSym);
                painter.drawText(QPointF(x+l.textpartXPos[i], y), l.textpart[i]);
                break;
        }
    }

    return x+l.width;
}



QString JKQTMathTextTextNode::getTypeName() const
{
    return QLatin1String("JKQTMathTextTextNode(")+text+")";
}

void JKQTMathTextTextNode::removeTrailingWhitespace()
{
    while (text.size()>0 && text[text.size()-1].isSpace()) text=text.left(text.size()-1);
}

void JKQTMathTextTextNode::removeLeadingWhitespace()
{
    while (text.size()>0 && text[0].isSpace()) text=text.right(text.size()-1);
}

QString JKQTMathTextTextNode::textTransform(const QString &text, const JKQTMathTextEnvironment &currentEv) const
{
    QString txt=text;
    auto fnt=parentMathText->getFontData(currentEv.font, currentEv.insideMath);
    const QFontMetricsF fm(currentEv.getFont(parentMathText));
    if (fnt.second==MTFELatin1 || fnt.second==MTFEUnicode) {
        if (currentEv.insideMath) {
            txt="";
            for (int i=0; i<text.size(); i++) {
                QChar c=text[i];
                switch(c.unicode()) {
                    case '-':
                    if (fm.inFont(QChar(0x2212))) {
                        txt+=QString(QString(" ")+QChar(0x2212));
                    } else {
                        txt+=QString(QString(" -"));
                    }
                    break;
                    case '+': txt+=QString(QString(" +")); break;
                    case '<': txt+=QString(QString(" <")); break;
                    case '>': txt+=QString(QString(" >")); break;
                    case '=': txt+=QString(QString(" =")); break;
                    case ';': txt+=QString(QString("; ")); break;
                    case ',': txt+=QString(QString(", ")); break;
                    default: txt+=c; break;
                }
            }
            txt=txt.replace("  ", " ");
        }
    }

    return txt;
}




JKQTMathTextTextNode::LayoutInfo::LayoutInfo():
    JKQTMathTextNodeSize(),
    textpart(), fontMode(), textpartXPos()
{

}

JKQTMathTextTextNode::LayoutInfo::LayoutInfo(const LayoutInfo &other):
    LayoutInfo()
{
    operator=(other);
}

JKQTMathTextTextNode::LayoutInfo::LayoutInfo(const JKQTMathTextNodeSize &other):
    LayoutInfo()
{
    operator=(other);
}

JKQTMathTextTextNode::LayoutInfo &JKQTMathTextTextNode::LayoutInfo::operator=(const LayoutInfo &other) {
    JKQTMathTextNodeSize::operator=(other);
    textpart=other.textpart;
    fontMode=other.fontMode;
    textpartXPos=other.textpartXPos;
    return *this;
}

JKQTMathTextTextNode::LayoutInfo &JKQTMathTextTextNode::LayoutInfo::operator=(const JKQTMathTextNodeSize &other) {
    JKQTMathTextNodeSize::operator=(other);
    textpart.clear();
    fontMode.clear();
    textpartXPos.clear();
    return *this;
}
