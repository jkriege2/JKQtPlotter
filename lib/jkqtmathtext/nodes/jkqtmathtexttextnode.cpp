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
#include <cmath>
#include <QFontMetricsF>
#include <QDebug>
#include <QFontDatabase>
#include <QFontInfo>
#include <QApplication>
#include <QFont>


JKQTMathTextTextNode::JKQTMathTextTextNode(JKQTMathText* _parent, const QString& textIn, bool addWhitespace, bool stripInnerWhitepace):
    JKQTMathTextNode(_parent)
{
    QString text=textIn;

    if (stripInnerWhitepace) {
        text="";
        for (int i=0; i<textIn.size(); i++) {
            if (!textIn[i].isSpace()) text+=textIn[i];
        }
    }

    this->text=text;
    // strip all whitespace from left
    while (this->text.size()>1 && this->text[0].isSpace()) {
        this->text=this->text.right(this->text.size()-1);
    }
    if (addWhitespace && (this->text.size()>0) && (!this->text[this->text.size()-1].isSpace())) this->text=this->text+" ";
    //qDebug()<<"JKQTMathTextTextNode( text="<<text<<" addWhitespace="<<addWhitespace<<")   [=> this->text="<<this->text<<"]";
}

JKQTMathTextTextNode::~JKQTMathTextTextNode() = default;

void JKQTMathTextTextNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    QStringList textpart;
    QList<bool> fontForcedUpright;
    QList<double> textpartXPos;
    getSizeInternalAndData(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos,textpart, fontForcedUpright, textpartXPos);
}

void JKQTMathTextTextNode::getSizeInternalAndData(QPainter &painter, JKQTMathTextEnvironment currentEv, double &width, double &baselineHeight, double &overallHeight, double &strikeoutPos, QStringList &textpart, QList<bool> &fontForcedUpright, QList<double> &textpartXPos)
{
    textpart.clear();
    fontForcedUpright.clear();
    const QString txt=textTransform(text, currentEv, true);
    if (currentEv.insideMath && currentEv.insideMathForceDigitsUpright) {
        splitTextForMathMode(txt, textpart, fontForcedUpright);
    } else {
        textpart.append(text);
        fontForcedUpright.append(false);
    }


    const QFont f=currentEv.getFont(parentMathText);
    const QFont fnonItalic=JKQTMathTextGetNonItalic(f);
    const QFontMetricsF fmNonItalic(fnonItalic, painter.device());
    const QFontMetricsF fm(f, painter.device());

    width=0;
    double ascent=0;
    double descent=0;
    for (int i=0; i<textpart.size(); i++) {
        const QRectF br=(fontForcedUpright[i]) ? fmNonItalic.boundingRect(textpart[i]) : fm.boundingRect(textpart[i]);
        const QRectF tbr=(fontForcedUpright[i]) ? JKQTMathTextGetTightBoundingRect(fnonItalic, textpart[i], painter.device()) : JKQTMathTextGetTightBoundingRect(f, textpart[i], painter.device());
        textpartXPos.append(width);
        width+=br.width();
        const double thisAscent=-tbr.top();
        const double thisDescent=tbr.bottom();
        ascent=qMax(ascent, thisAscent);
        descent=qMax(descent, thisDescent);
    }
    overallHeight=(ascent+descent); //fm.height();
    baselineHeight=ascent;
    strikeoutPos=fm.strikeOutPos();
}

void JKQTMathTextTextNode::splitTextForMathMode(const QString &txt, QStringList &textpart, QList<bool> &fontForcedUpright)
{
    auto isForcedUprightChar=[](const QChar& c) {
        return c.isDigit()
                || c=='(' || c=='[' || c=='|' || c==']' || c==')' || c=='<' || c=='>'|| c=='{' || c=='}' || c=='|'
                || c==QChar(0x2329) || c==QChar(0x232A) || c==QChar(0x2308) || c==QChar(0x2309) || c==QChar(0x230A) || c==QChar(0x230B);
    };

    textpart.clear();
    fontForcedUpright.clear();
    QString currentSection="";
    bool currentSectionForcedUpright=false;
    int i=0;
    while (i<txt.size()) {
        const QChar c=txt[i];
        const bool CisForcedUprightChar=isForcedUprightChar(c);
        const bool CisForcedUprightCharExt=CisForcedUprightChar||(c=='.')||(c==',');
        if (currentSection.size()==0) {
            // start new section
            currentSectionForcedUpright=CisForcedUprightChar;
            currentSection+=c;
        } else {
            // existing section
            if (CisForcedUprightCharExt==currentSectionForcedUpright) {
                // continue current section
                currentSection+=c;
            } else {
                // start new section
                textpart.append(currentSection);
                fontForcedUpright.append(currentSectionForcedUpright);
                currentSection=c;
                currentSectionForcedUpright=CisForcedUprightChar;
            }
        }
        i++;
    }
    if (currentSection.size()>0) {
        textpart.append(currentSection);
        fontForcedUpright.append(currentSectionForcedUpright);
    }
}

double JKQTMathTextTextNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    double width=0;
    double baselineHeight=0;
    double overallHeight=0;
    double sp=0;
    QStringList textpart;
    QList<bool> fontForcedUpright;
    QList<double> textpartXPos;
    getSizeInternalAndData(painter, currentEv, width, baselineHeight, overallHeight, sp, textpart, fontForcedUpright, textpartXPos);


    const QFont f=currentEv.getFont(parentMathText);
    const QFont fnonItalic=JKQTMathTextGetNonItalic(f);
    const QFontMetricsF fm(f, painter.device());
    const QFontMetricsF fmNonItalic(fnonItalic, painter.device());

    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setFont(f);

    //qDebug()<<"JKQTMathTextTextNode: text="<<text<<" font="<<f;

    const QPen p(currentEv.color, fm.lineWidth(), Qt::SolidLine);
    painter.setPen(p);

    auto drawString=[&](QPainter& painter, const QFont& f, double x, double y, const QString& txt)  {
        if (currentEv.font==MTEblackboard && parentMathText->isFontBlackboardSimulated()) {
            QPainterPath path;
            path.addText(QPointF(x, y), f, txt);
            painter.drawPath(path);
        } else {
            painter.setFont(f);
            painter.drawText(QPointF(x, y), txt);
        }
    };

    for (int i=0; i<textpart.size(); i++) {
        if (fontForcedUpright[i]) drawString(painter, fnonItalic, x+textpartXPos[i], y, textpart[i]);
        else drawString(painter, f, x+textpartXPos[i], y, textpart[i]);
    }

    return x+width;
}

bool JKQTMathTextTextNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
    html=html+currentEv.toHtmlStart(defaultEv, parentMathText)+text+currentEv.toHtmlAfter(defaultEv, parentMathText);
    return true;
}

QString JKQTMathTextTextNode::getText() const {
    return this->text;
}

QString JKQTMathTextTextNode::getTypeName() const
{
    return QLatin1String("JKQTMathTextTextNode(")+text+")";
}

QString JKQTMathTextTextNode::textTransform(const QString &text, JKQTMathTextEnvironment currentEv, bool /*forSize*/)
{
    QString txt=text;
    auto fnt=parentMathText->getFontData(currentEv.font, currentEv.insideMath);
    if (fnt.second==MTFEUnicode || fnt.second==MTFEUnicode) {
        if (currentEv.insideMath) {
            txt="";
            for (int i=0; i<text.size(); i++) {
                QChar c=text[i];
                switch(c.unicode()) {
                    case '-': txt+=QString(QString(" ")+QChar(0x2212)); break;
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



MTplainTextNode::MTplainTextNode(JKQTMathText *_parent, const QString& _text, bool addWhitespace, bool stripInnerWhitepace):
    JKQTMathTextTextNode(_parent, _text, addWhitespace, stripInnerWhitepace)
{

}

QString MTplainTextNode::getTypeName() const
{
    return QLatin1String("MTplainTextNode(")+text+")";
}


QString MTplainTextNode::textTransform(const QString &_text, JKQTMathTextEnvironment /*currentEv*/, bool /*forSize*/)
{
    return _text;
}


