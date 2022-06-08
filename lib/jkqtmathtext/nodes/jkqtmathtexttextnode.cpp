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
    if (addWhitespace /*&& (this->text.size()>0)*/ && (!this->text[this->text.size()-1].isSpace())) this->text=this->text+" ";
    //qDebug()<<"JKQTMathTextTextNode( text="<<text<<" addWhitespace="<<addWhitespace<<")   [=> this->text="<<this->text<<"]";
}

JKQTMathTextTextNode::~JKQTMathTextTextNode() = default;

void JKQTMathTextTextNode::getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    QFont f=currentEv.getFont(parentMathText);
    if (currentEv.insideMath && (text=="(" || text=="[" || text=="|" || text=="]" || text==")" || text=="<" || text==">" ||
                                 text==QString(QChar(0x2329)) || text==QString(QChar(0x232A)) || text==QString(QChar(0x2308)) ||
                                 text==QString(QChar(0x2309)) || text==QString(QChar(0x230A)) || text==QString(QChar(0x230B)))) {
            f.setItalic(false);
            //f.setFamily(parent->getFontData(currentEv.font, currentEv.insideMath, FontSubclass::Text).first);
        }
    QString txt=textTransform(text, currentEv, true);
    QFontMetricsF fm(f, painter.device());
    QRectF br=fm.boundingRect(txt);
    QRectF tbr=JKQTMathTextGetTightBoundingRect(f, txt, painter.device()); //fm.tightBoundingRect(txt);
    if (txt=="|") {
        br=fm.boundingRect("X");
        tbr=QRectF(0,0,fm.boundingRect("X").width(), fm.ascent());//fm.boundingRect("X");
        br.setWidth(0.7*br.width());
    }
    width=br.width();//width(text);

    if (txt.size()>0) {
        if (txt[0].isSpace() /*&& br.width()<=0*/) width=width+fm.boundingRect("I").width();
        if (txt.size()>1 && txt[txt.size()-1].isSpace() /*&& (fm.boundingRect("a ").width()==fm.boundingRect("a").width())*/) width=width+fm.boundingRect("I").width();
    }

    //qDebug()<<"text: "<<text<<"   "<<tbr.height()<<tbr.top()<<tbr.bottom();
    overallHeight=tbr.height()*1.1; //fm.height();
    baselineHeight=1.1*(tbr.height()-(tbr.height()+tbr.top()));//fm.ascent();
    strikeoutPos=fm.strikeOutPos()*1.1;
}

double JKQTMathTextTextNode::draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv, const JKQTMathTextNodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    double width=0;
    double baselineHeight=0;
    double overallHeight=0;
    double sp=0;
    getSize(painter, currentEv, width, baselineHeight, overallHeight, sp);

    QString txt=textTransform(text, currentEv);

    bool hasDigits=false;
    bool onlyDigits=true;
    for (int i=0; i<txt.size(); i++) {
        if (txt[i].isDigit()) {
            hasDigits=true;
        }
        if (!txt[i].isDigit() && !txt[i].isSpace()) {
            onlyDigits=false;
        }
    }


    QPen pold=painter.pen();
    QFont fold=painter.font();
    QFont f=currentEv.getFont(parentMathText);
    if (currentEv.insideMath && (text=="(" || text=="[" || text=="|" || text=="]" || text==")" || text=="<" || text==">" ||
                                 text==QString(QChar(0x2329)) || text==QString(QChar(0x232A)) || text==QString(QChar(0x2308)) ||
                                 text==QString(QChar(0x2309)) || text==QString(QChar(0x230A)) || text==QString(QChar(0x230B)))) {
        f.setItalic(false);
    }


    if (onlyDigits && currentEv.insideMath) {
        f.setItalic(false);
    }

    painter.setFont(f);

    //qDebug()<<"JKQTMathTextTextNode: text="<<text<<" font="<<f;

    QPen p=painter.pen();
    p.setColor(currentEv.color);
    painter.setPen(p);
    double dx=0;
    QFontMetricsF fm(f, painter.device());
    /*if (txt.size()>1 && txt[txt.size()-1].isSpace()) {
        QFontMetricsF fm(f, painter.device());
        //if ((fm.QFMF_WIDTH("a ")==fm.QFMF_WIDTH("a"))) dx=fm.boundingRect("I").QFMF_WIDTH();
    }*/

    if (!hasDigits || !f.italic()) {
        if (currentEv.font==MTEblackboard && parentMathText->isFontBlackboardSimulated()) {
            QPainterPath path;
            path.addText(QPointF(x+dx, y), f, txt);
            painter.drawPath(path);
        } else {
            painter.drawText(QPointF(x+dx, y), txt);//.simplified());
        }
    } else {
        int i=0;
        double xx=x+dx;
        QFont ff=f;
        QFontMetricsF fmff(ff, painter.device());
        ff.setItalic(false);
        while (i<txt.size()) {
            if (txt[i].isDigit()) {
                if (currentEv.font==MTEblackboard && parentMathText->isFontBlackboardSimulated()) {
                    QPainterPath path;
                    path.addText(QPointF(xx, y), ff, QString(txt[i]));
                    painter.drawPath(path);
                } else {
                    painter.setFont(ff);
                    painter.drawText(QPointF(xx, y), QString(txt[i]));
                }
                xx=xx+fmff.boundingRect(txt[i]).width();
            } else {
                if (currentEv.font==MTEblackboard && parentMathText->isFontBlackboardSimulated()) {
                    QPainterPath path;
                    path.addText(QPointF(xx, y), f, QString(txt[i]));
                    painter.drawPath(path);
                } else {
                    painter.setFont(f);
                    painter.drawText(QPointF(xx, y), QString(txt[i]));
                }
                xx=xx+fm.boundingRect(txt[i]).width();
            }
            i++;
        }
    }
    painter.setPen(pold);
    painter.setFont(fold);

    return x+width;
}

bool JKQTMathTextTextNode::toHtml(QString &html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) {
    html=html+currentEv.toHtmlStart(defaultEv)+text+currentEv.toHtmlAfter(defaultEv);
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
    if (fnt.second==MTFEunicode || fnt.second==MTFEunicodeLimited) {
        if (currentEv.insideMath) {
            txt="";
            for (int i=0; i<text.size(); i++) {
                QChar c=text[i];
                switch(c.unicode()) {
                    case '-': txt+=QString(QString(" ")+QChar(0x2212)); break;
                    case '+': txt+=QString(QString(" +")); break;
                    case '/': txt+=QString(QString(" /")); break;
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


JKQTMathTextWhitespaceNode::JKQTMathTextWhitespaceNode(JKQTMathText *_parent):
    JKQTMathTextTextNode(_parent, " ", false, false)
{

}

JKQTMathTextWhitespaceNode::~JKQTMathTextWhitespaceNode()
= default;

QString JKQTMathTextWhitespaceNode::getTypeName() const
{
    return QLatin1String("JKQTMathTextWhitespaceNode(")+text+")";
}

bool JKQTMathTextWhitespaceNode::toHtml(QString &html, JKQTMathTextEnvironment /*currentEv*/, JKQTMathTextEnvironment /*defaultEv*/) {
    html=html+"&nbsp;";
    return true;
}
