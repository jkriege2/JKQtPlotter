/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "jkqtmathtext/jkqtmathtext.h"
#include <QFontMetricsF>
#include <QDebug>
#include <QFontDatabase>
#include <typeinfo>
#include <QApplication>
//#define JKQTP_QColor2String(color) QString(jkqtp_rgbtostring((color).red(), (color).green(), (color).blue(), (color).alpha()).c_str())


QPainterPath makeHBracePath(double x, double ybrace, double width, double bw, double cubicshrink=0.5, double cubiccontrolfac=0.3) {
    double xl1=x-(width)*cubicshrink+bw*cubicshrink;
    double xr2=x+(width)*cubicshrink-bw*cubicshrink;
    double xl2=x-bw*cubicshrink;
    double xr1=x+bw*cubicshrink;

    QPainterPath path;
    path.moveTo(xl1-bw*cubicshrink, ybrace-bw*cubicshrink);
    path.cubicTo(xl1-bw*cubicshrink, ybrace-bw*cubicshrink+bw*cubiccontrolfac, xl1-bw*cubiccontrolfac, ybrace, xl1, ybrace);
    path.lineTo(xl2, ybrace);
    path.cubicTo(xl2+bw*cubiccontrolfac, ybrace, (xl2+xr1)/2.0, ybrace+bw*cubicshrink-bw*cubiccontrolfac, (xl2+xr1)/2.0, ybrace+bw*cubicshrink);
    path.cubicTo((xl2+xr1)/2.0, ybrace+bw*cubicshrink-bw*cubiccontrolfac, xr1-bw*cubiccontrolfac, ybrace, xr1, ybrace);
    path.lineTo(xr2, ybrace);
    path.cubicTo(xr2+bw*cubiccontrolfac, ybrace, xr2+bw*cubicshrink, ybrace-bw*cubicshrink+bw*cubiccontrolfac, xr2+bw*cubicshrink, ybrace-bw*cubicshrink);
    return path;
}


QPainterPath makeArrow(double x, double y, double width, double arrowW, bool left=false, bool right=true) {
    double x1=x;
    double x2=x+width;
    double x3=x2-arrowW/2.0;
    double y3u=y-arrowW/2.0;
    double y3d=y+arrowW/2.0;
    double x3l=x+arrowW/2.0;

    QPainterPath path;
    path.moveTo(x1, y);
    path.lineTo(x2, y);
    if (right) {
        path.moveTo(x3, y3u);
        path.lineTo(x2, y);
        path.lineTo(x3, y3d);
    }
    if (left) {
        path.moveTo(x3l, y3u);
        path.lineTo(x1, y);
        path.lineTo(x3l, y3d);
    }
    return path;
}


QPainterPath makeDArrow(double x, double y, double width, double arrowW, bool left=false, bool right=true) {
    double x1=x;
    double x2=x+width;
    double dx=arrowW/4.0;
    double y1=y-dx;
    double y2=y+dx;
    double x3=x2-arrowW/2.0;
    double y3u=y-arrowW/2.0;
    double y3d=y+arrowW/2.0;
    double x3l=x+arrowW/2.0;

    QPainterPath path;
    path.moveTo(x1+dx, y1);
    path.lineTo(x2-dx, y1);
    path.moveTo(x1+dx, y2);
    path.lineTo(x2-dx, y2);
    if (right) {
        path.moveTo(x3, y3u);
        path.lineTo(x2, y);
        path.lineTo(x3, y3d);
    }
    if (left) {
        path.moveTo(x3l, y3u);
        path.lineTo(x1, y);
        path.lineTo(x3l, y3d);
    }
    return path;
}



// --------------------------------------------------------------------------------------------------
// -- implementation of the MTnode's methods
// --------------------------------------------------------------------------------------------------

JKQTmathText::MTenvironment::MTenvironment() {
    color=QColor("black");
    font=MTEroman;
    fontSize=10;
    bold=false;
    italic=false;
    smallCaps=false;
    underlined=false;
    overline=false;
    strike=false;
    insideMath=false;
}

QFont JKQTmathText::MTenvironment::getFont(JKQTmathText* parent) const {
    QFont f;
    switch (font) {
        case MTEsans:
                if (insideMath) f.setFamily(parent->get_fontMathSans());
                else f.setFamily(parent->get_fontSans());
                break;
        case MTEtypewriter: f.setFamily(parent->get_fontTypewriter()); break;
        case MTEscript: f.setFamily(parent->get_fontScript()); break;
        case MTEroman:
                if (insideMath) f.setFamily(parent->get_fontMathRoman());
                else f.setFamily(parent->get_fontRoman());
                break;
        case MTEcaligraphic: f.setFamily(parent->get_fontCaligraphic()); break;
        case MTEblackboard: f.setFamily(parent->get_fontBlackboard()); break;
    };
    f.setBold(bold);
    f.setItalic(italic);
    f.setUnderline(underlined);
    f.setOverline(overline);
    f.setStrikeOut(strike);
    f.setCapitalization(QFont::MixedCase);
    if (smallCaps) f.setCapitalization(QFont::SmallCaps);
    f.setPointSizeF(fontSize);
    f.setStyleStrategy(QFont::NoFontMerging);
    return f;
}

QString JKQTmathText::MTenvironment::toHtmlStart(MTenvironment defaultEv) const {
    QString s;
    s=s+"font-size: "+QLocale::c().toString(fontSize)+"pt; ";
    if (insideMath) {
        if (defaultEv.italic) {
            if (!italic) s=s+"font-style: italic; ";
            if (italic) s=s+"font-style: normal; ";
        } else {
            if (!italic) s=s+"font-style: italic; ";
        }
    } else {
        if (!defaultEv.italic && italic) s=s+"font-style: italic; ";
    }
    if (bold && !defaultEv.bold) s=s+"font-weight: bold";

    QStringList td;
    if (underlined && !defaultEv.underlined) td<<"underline";
    if (overline && !defaultEv.overline) td<<"overline";
    if (strike && !defaultEv.strike) td<<"line-through";
    if (td.size()>0) s=s+"text-decoration: "+td.join(", ");
    return "<span style=\""+s+"\">";
}

QString JKQTmathText::MTenvironment::toHtmlAfter(JKQTmathText::MTenvironment /*defaultEv*/) const {
    return "</span>";
}

JKQTmathText::MTnode::MTnode(JKQTmathText* parent) {
    this->parent=parent;
    drawBoxes=false;
}

void JKQTmathText::MTnode::getSize(QPainter &painter, JKQTmathText::MTenvironment currentEv, double &width, double &baselineHeight, double &overallHeight, double &strikeoutPos)
{
    double w=width, b=baselineHeight, o=overallHeight, s=strikeoutPos;
    getSizeInternal(painter, currentEv, w, b, o, s);

    if (w<1e5) width=w;
    if (b<1e5) baselineHeight=b;
    if (o<1e5) overallHeight=o;
    if (s<1e5) strikeoutPos=s;
}

bool JKQTmathText::MTnode::toHtml(QString &/*html*/, JKQTmathText::MTenvironment /*currentEv*/, JKQTmathText::MTenvironment /*defaultEv*/) {
    return false;
}


void JKQTmathText::MTnode::doDrawBoxes(QPainter& painter, double x, double y, JKQTmathText::MTenvironment currentEv) {
    if (drawBoxes) {
        painter.save();
        double w, oh, bh, sp;
        getSize(painter, currentEv, w, bh, oh, sp);
        QPen p=painter.pen();
        p.setColor("lightcoral");
        p.setWidthF(0.5);
        painter.setPen(p);
        QRectF r(x, y-bh, w, oh);
        painter.drawRect(r);
        p.setColor("lightblue");
        painter.setPen(p);
        if (w>0) painter.drawLine(QLineF(x, y, x+w, y));
        p.setColor("green");
        painter.setPen(p);
        painter.drawEllipse(x-3.0,y-3.0,6.0,6.0);
        p.setColor("lightgreen");
        painter.setPen(p);
        painter.drawLine(x-2.0, y, x+2.0, y);
        painter.drawLine(x, y-2, x, y+2.0);
        painter.restore();
    }
}


JKQTmathText::MTtextNode::MTtextNode(JKQTmathText* parent, QString textIn, bool addWhitespace, bool stripInnerWhitepace):
    JKQTmathText::MTnode(parent)
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
    //qDebug()<<"MTtextNode( text="<<text<<" addWhitespace="<<addWhitespace<<")   [=> this->text="<<this->text<<"]";
}

JKQTmathText::MTtextNode::~MTtextNode() {
}

void JKQTmathText::MTtextNode::getSizeInternal(QPainter& painter, JKQTmathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos) {
    QFont f=currentEv.getFont(parent);
    if (currentEv.insideMath && (text=="(" || text=="[" || text=="|" || text=="]" || text==")" || text=="<" || text==">" ||
                                 text==QString(QChar(0x2329)) || text==QString(QChar(0x232A)) || text==QString(QChar(0x2308)) ||
                                 text==QString(QChar(0x2309)) || text==QString(QChar(0x230A)) || text==QString(QChar(0x230B)))) {
            f.setItalic(false);
            f.setFamily(parent->get_fontBraces());
        }
    QString txt=textTransform(text, currentEv, true);
    QFontMetricsF fm(f, painter.device());
    QRectF br=fm.boundingRect(txt);
    QRectF tbr=parent->getTBR(f, txt, painter.device()); //fm.tightBoundingRect(txt);
    if (txt=="|") {
        br=fm.boundingRect("X");
        tbr=QRect(0,0,fm.width("X"), fm.ascent());//fm.boundingRect("X");
        br.setWidth(0.7*br.width());
    }
    width=br.width();//width(text);

    if (txt.size()>0) {
        if (txt[0].isSpace() /*&& br.width()<=0*/) width=width+fm.boundingRect("I").width();
        if (txt.size()>1 && txt[txt.size()-1].isSpace() /*&& (fm.boundingRect("a ").width()==fm.boundingRect("a").width())*/) width=width+fm.width("I");
    }

    //qDebug()<<"text: "<<text<<"   "<<tbr.height()<<tbr.top()<<tbr.bottom();
    overallHeight=tbr.height()*1.1; //fm.height();
    baselineHeight=1.1*(tbr.height()-(tbr.height()+tbr.top()));//fm.ascent();
    strikeoutPos=fm.strikeOutPos()*1.1;
}

double JKQTmathText::MTtextNode::draw(QPainter& painter, double x, double y, JKQTmathText::MTenvironment currentEv) {
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
    QFont f=currentEv.getFont(parent);
    if (currentEv.insideMath && (text=="(" || text=="[" || text=="|" || text=="]" || text==")" || text=="<" || text==">" ||
                                 text==QString(QChar(0x2329)) || text==QString(QChar(0x232A)) || text==QString(QChar(0x2308)) ||
                                 text==QString(QChar(0x2309)) || text==QString(QChar(0x230A)) || text==QString(QChar(0x230B)))) {
            f.setItalic(false);
            f.setFamily(parent->get_fontBraces());
        }


    if (onlyDigits && currentEv.insideMath) {
        f.setItalic(false);
    }

    painter.setFont(f);

    QPen p=painter.pen();
    p.setColor(currentEv.color);
    painter.setPen(p);
    double dx=0;
    QFontMetricsF fm(f, painter.device());
    /*if (txt.size()>1 && txt[txt.size()-1].isSpace()) {
        QFontMetricsF fm(f, painter.device());
        //if ((fm.width("a ")==fm.width("a"))) dx=fm.boundingRect("I").width();
    }*/

    if (!hasDigits || !f.italic()) {
        painter.drawText(QPointF(x+dx, y), txt);//.simplified());
    } else {
        int i=0;
        double xx=x+dx;
        QFont ff=f;
        QFontMetricsF fmff(ff, painter.device());
        ff.setItalic(false);
        while (i<txt.size()) {
            if (txt[i].isDigit()) {
                painter.setFont(ff);
                painter.drawText(QPointF(xx, y), QString(txt[i]));
                xx=xx+fmff.width(txt[i]);
            } else {
                painter.setFont(f);
                painter.drawText(QPointF(xx, y), QString(txt[i]));
                xx=xx+fm.width(txt[i]);
            }
            i++;
        }
    }
    painter.setPen(pold);
    painter.setFont(fold);

    return x+width;
}

bool JKQTmathText::MTtextNode::toHtml(QString &html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv) {
    html=html+currentEv.toHtmlStart(defaultEv)+text+currentEv.toHtmlAfter(defaultEv);
    return true;
}

QString JKQTmathText::MTtextNode::getTypeName() const
{
    return QLatin1String("MTtextNode(")+text+")";
}

QString JKQTmathText::MTtextNode::textTransform(const QString &text, JKQTmathText::MTenvironment currentEv, bool /*forSize*/)
{
    QString txt=text;
    if (parent->get_fontEncoding()==MTFEunicode) {
        if (currentEv.insideMath) {
            txt="";
            for (int i=0; i<text.size(); i++) {
                QChar c=text[i];
                switch(c.unicode()) {
                    case '-': txt+=QString(QString(" ")+QChar(0x2212)); break;
                    case '+': txt+=QString(QString(" +")); break;
                    case '/': txt+=QString(QString(" / ")); break;
                    case '<': txt+=QString(QString(" < ")); break;
                    case '>': txt+=QString(QString(" > ")); break;
                    case '=': txt+=QString(QString(" = ")); break;
                    case ';': txt+=QString(QString("; ")); break;
                    default: txt+=c; break;
                }
            }
            txt=txt.replace("  ", " ");
        }
    }
    if (currentEv.font==MTEblackboard && parent->get_fontEncoding()==MTFEunicode) {
        txt="";
        for (int i=0; i<text.size(); i++) {
            QChar c=text[i];
            switch(c.unicode()) {
                case 'C': txt+=QChar(0x2102); break;
                case 'H': txt+=QChar(0x210D); break;
                case 'N': txt+=QChar(0x2115); break;
                case 'P': txt+=QChar(0x2119); break;
                case 'Q': txt+=QChar(0x211A); break;
                case 'R': txt+=QChar(0x211D); break;
                case 'Z': txt+=QChar(0x2124); break;
                case 'd': txt+=QChar(0x2146); break;
                case 'e': txt+=QChar(0x2147); break;
                case 'i': txt+=QChar(0x2148); break;
                case 'j': txt+=QChar(0x2149); break;
                default: txt+=c; break;
            }

        }
    }
    if (currentEv.font==MTEcaligraphic && parent->get_fontEncoding()==MTFEunicode) {
        txt="";
        for (int i=0; i<text.size(); i++) {
            QChar c=text[i];
            switch(c.unicode()) {
                case 'I': txt+=QChar(0x2110); break;
                case 'g': txt+=QChar(0x210A); break;
                case 'H': txt+=QChar(0x210B); break;
                case 'L': txt+=QChar(0x2112); break;
                case 'l': txt+=QChar(0x2113); break;
                case 'p': txt+=QChar(0x2118); break;
                case 'R': txt+=QChar(0x211B); break;
                case 'B': txt+=QChar(0x212C); break;
                case 'e': txt+=QChar(0x212F); break;
                case 'E': txt+=QChar(0x2130); break;
                case 'F': txt+=QChar(0x2131); break;
                case 'M': txt+=QChar(0x2133); break;
                case 'o': txt+=QChar(0x2134); break;



                default: txt+=c; break;
            }

        }
    }
    return txt;
}







JKQTmathText::MTinstruction1Node::MTinstruction1Node(JKQTmathText* parent, QString name, MTnode* child, const QStringList& parameters):
    JKQTmathText::MTnode(parent)
{
    this->name=name;
    this->child=child;
    this->parameters=parameters;
}

JKQTmathText::MTinstruction1Node::~MTinstruction1Node() {
    if (child!=nullptr) delete child;
}

QString JKQTmathText::MTinstruction1Node::getTypeName() const
{
    return QLatin1String("MTinstruction1Node(")+name+")";
}

void JKQTmathText::MTinstruction1Node::getSizeInternal(QPainter& painter, JKQTmathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos) {
    JKQTmathText::MTenvironment ev=currentEv;
    if (name=="bf" || name=="textbf" || name=="mathbf") ev.bold=true;
    else if (name=="em") ev.italic=!ev.italic;
    else if (name=="it" || name=="textit" || name=="mathit") ev.italic=true;
    else if (name=="textcolor" || name=="mathcolor" || name=="color") ev.color=QColor(parameters.value(0, ev.color.name()));
    else if (name=="equation") { ev.italic=true; ev.insideMath=true; }
    else if (name=="sc" || name=="textsc" || name=="mathsc") ev.smallCaps=true;
    else if (name=="ul" || name=="underline" || name=="underlined") ev.underlined=true;
    else if (name=="ol" || name=="overline" || name=="overlined") ev.overline=true;
    else if (name=="strike") ev.strike=true;
    else if (name=="rm" || name=="textrm") { ev.font=JKQTmathText::MTEroman; }
    else if (name=="mathrm" || name=="text" || name=="mbox" || name=="operatorname") { ev.font=JKQTmathText::MTEroman; ev.italic=false; }
    else if (name=="mat") { ev.font=JKQTmathText::MTEroman; ev.italic=false; ev.bold=true; }
    else if (name=="cal" || name=="textcal" || name=="mathcal") { ev.font=JKQTmathText::MTEcaligraphic; }
    else if (name=="bb" || name=="textbb" || name=="mathbb") { ev.font=JKQTmathText::MTEblackboard; }
    else if (name=="tt" || name=="texttt" || name=="mathtt") { ev.font=JKQTmathText::MTEtypewriter; }
    else if (name=="sf" || name=="textsf" || name=="mathsf") { ev.font=JKQTmathText::MTEsans; }
    else if (name=="script" || name=="textscript" || name=="mathscript") { ev.font=JKQTmathText::MTEscript; }
    else if (name=="displaystyle") { ev.fontSize=ev.fontSize/0.8; }
    else if (name=="scriptstyle") { ev.fontSize=ev.fontSize*0.8; }
    else if (name=="scriptscriptstyle") { ev.fontSize=ev.fontSize*0.8*0.8; }

    child->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);
    if (name=="colorbox" || name=="fbox" || name=="boxed") {
        QFontMetricsF fm(ev.getFont(parent));
        double xw=fm.width("x");
        width+=xw;
        overallHeight+=xw;
        baselineHeight+=xw/2.0;
    }
}

double JKQTmathText::MTinstruction1Node::draw(QPainter& painter, double x, double y, JKQTmathText::MTenvironment currentEv) {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTmathText::MTenvironment ev=currentEv;    
    if (name=="bf" || name=="textbf" || name=="mathbf") ev.bold=true;
    else if (name=="em") ev.italic=!ev.italic;
    else if (name=="it" || name=="textit" || name=="mathit") ev.italic=true;
    else if (name=="textcolor" || name=="mathcolor" || name=="color") ev.color=QColor(parameters.value(0, ev.color.name()));
    else if (name=="equation") { ev.italic=true; ev.insideMath=true; }
    else if (name=="sc" || name=="textsc" || name=="mathsc") ev.smallCaps=true;
    else if (name=="ul" || name=="underline" || name=="underlined") ev.underlined=true;
    else if (name=="ol" || name=="overline" || name=="overlined") ev.overline=true;
    else if (name=="strike") ev.strike=true;
    else if (name=="rm" || name=="textrm") { ev.font=JKQTmathText::MTEroman; }
    else if (name=="mathrm" || name=="text" || name=="mbox" || name=="operatorname") { ev.font=JKQTmathText::MTEroman; ev.italic=false; }
    else if (name=="mat") { ev.font=JKQTmathText::MTEroman; ev.italic=false; ev.bold=true; }
    else if (name=="cal" || name=="textcal" || name=="mathcal") { ev.font=JKQTmathText::MTEcaligraphic; }
    else if (name=="bb" || name=="textbb" || name=="mathbb") { ev.font=JKQTmathText::MTEblackboard; }
    else if (name=="tt" || name=="texttt" || name=="mathtt") { ev.font=JKQTmathText::MTEtypewriter; }
    else if (name=="sf" || name=="textsf" || name=="mathsf") { ev.font=JKQTmathText::MTEsans; }
    else if (name=="script" || name=="textscript" || name=="mathscript") { ev.font=JKQTmathText::MTEscript; }
    else if (name=="displaystyle") { ev.fontSize=ev.fontSize/0.8; }
    else if (name=="scriptstyle") { ev.fontSize=ev.fontSize*0.8; }
    else if (name=="scriptscriptstyle") { ev.fontSize=ev.fontSize*0.8*0.8; }

    QPen oldPen=painter.pen();
    double shiftX=0;
    if (name=="colorbox" || name=="fbox" || name=="boxed") {
        QColor fcol=currentEv.color;
        if (name=="colorbox") fcol=QColor(parameters.value(0, ev.color.name()));
        //qDebug()<<"COLOR="<<fcol;
        double width, baselineHeight, overallHeight, strikeoutPos;
        child->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);
        QPen p=painter.pen();
        QFontMetricsF fm(currentEv.getFont(parent));
        double xw=fm.width("x");
        p.setColor(fcol);
        painter.setPen(p);
        painter.drawRect(x,y-baselineHeight-xw/2,width+xw,overallHeight+xw);
        shiftX=xw/2.0;
    }

    double xnew= child->draw(painter, x+shiftX, y, ev);
    painter.setPen(oldPen);
    return xnew;
}

bool JKQTmathText::MTinstruction1Node::toHtml(QString &html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv) {
    JKQTmathText::MTenvironment ev=currentEv;
    if (name=="bf" || name=="textbf" || name=="mathbf") ev.bold=true;
    else if (name=="em") ev.italic=!ev.italic;
    else if (name=="it" || name=="textit" || name=="mathit") ev.italic=true;
    else if (name=="textcolor" || name=="mathcolor" || name=="color") ev.color=QColor(parameters.value(0, ev.color.name()));
    else if (name=="equation") { ev.italic=true; ev.insideMath=true; }
    else if (name=="sc" || name=="textsc" || name=="mathsc") ev.smallCaps=true;
    else if (name=="ul" || name=="underline" || name=="underlined") ev.underlined=true;
    else if (name=="ol" || name=="overline" || name=="overlined") ev.overline=true;
    else if (name=="strike") ev.strike=true;
    else if (name=="rm" || name=="textrm") { ev.font=JKQTmathText::MTEroman; }
    else if (name=="mathrm" || name=="text" || name=="mbox" || name=="operatorname") { ev.font=JKQTmathText::MTEroman; ev.italic=false; }
    else if (name=="mat") { ev.font=JKQTmathText::MTEroman; ev.italic=false; ev.bold=true; }
    else if (name=="cal" || name=="textcal" || name=="mathcal") { ev.font=JKQTmathText::MTEcaligraphic; }
    else if (name=="bb" || name=="textbb" || name=="mathbb") { ev.font=JKQTmathText::MTEblackboard; }
    else if (name=="tt" || name=="texttt" || name=="mathtt") { ev.font=JKQTmathText::MTEtypewriter; }
    else if (name=="sf" || name=="textsf" || name=="mathsf") { ev.font=JKQTmathText::MTEsans; }
    else if (name=="script" || name=="textscript" || name=="mathscript") { ev.font=JKQTmathText::MTEscript; }
    else if (name=="displaystyle") { ev.fontSize=ev.fontSize/0.8; }
    else if (name=="scriptstyle") { ev.fontSize=ev.fontSize*0.8; }
    else if (name=="scriptscriptstyle") { ev.fontSize=ev.fontSize*0.8*0.8; }

    return child->toHtml(html, ev, defaultEv);
}

void JKQTmathText::MTinstruction1Node::set_drawBoxes(bool draw)
{
    drawBoxes=draw;
    child->set_drawBoxes(draw);
}






JKQTmathText::MTsubscriptNode::MTsubscriptNode(JKQTmathText* parent, MTnode* child):
    JKQTmathText::MTnode(parent)
{
    this->child=child;
}

JKQTmathText::MTsubscriptNode::~MTsubscriptNode() {
    if (child!=nullptr) delete child;
}

void JKQTmathText::MTsubscriptNode::getSizeInternal(QPainter& painter, JKQTmathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos) {
    JKQTmathText::MTenvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parent->get_subsuper_size_factor();

    QFontMetricsF fm(ev.getFont(parent), painter.device());
    double shift=parent->get_sub_shift_factor()*parent->getTBR(ev.getFont(parent), "M", painter.device()).height();

    child->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);

    QFontMetricsF fmouter(currentEv.getFont(parent), painter.device());
    QRectF tbr=parent->getTBR(currentEv.getFont(parent), "M", painter.device());
    overallHeight=tbr.height()+shift+(overallHeight-baselineHeight);
    baselineHeight=tbr.height();
    strikeoutPos=fmouter.strikeOutPos();
    if (currentEv.italic) width=width-double(fm.width(' '))*parent->get_italic_correction_factor();
}

double JKQTmathText::MTsubscriptNode::draw(QPainter& painter, double x, double y, JKQTmathText::MTenvironment currentEv) {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTmathText::MTenvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parent->get_subsuper_size_factor();

    QFontMetricsF fm(ev.getFont(parent), painter.device());

    double shift=parent->get_sub_shift_factor()*parent->getTBR(ev.getFont(parent), "M", painter.device()).height();
    double xx=x;
    if (currentEv.italic) xx=xx-double(fm.width(' '))*parent->get_italic_correction_factor();
    return child->draw(painter, xx, y+shift, ev);//+0.5*fm.boundingRect("A").width();
}

bool JKQTmathText::MTsubscriptNode::isSubOrSuper()
{
    return true;
}

QString JKQTmathText::MTsubscriptNode::getTypeName() const
{
    return "MTsubscriptNode";
}


bool JKQTmathText::MTsubscriptNode::toHtml(QString &html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv) {
    html=html+"<sub>";
    bool ok=child->toHtml(html, currentEv, defaultEv);
    html=html+"</sub>";
    return ok;
}

void JKQTmathText::MTsubscriptNode::set_drawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child->set_drawBoxes(draw);

}





JKQTmathText::MTsqrtNode::MTsqrtNode(JKQTmathText* parent, MTnode* child, int degree):
    JKQTmathText::MTnode(parent)
{
    this->child=child;
    this->degree=degree;
}

JKQTmathText::MTsqrtNode::~MTsqrtNode() {
    if (child!=nullptr) delete child;
}

void JKQTmathText::MTsqrtNode::getSizeInternal(QPainter& painter, JKQTmathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos) {
    QFontMetricsF fm(currentEv.getFont(parent), painter.device());

    child->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);

    overallHeight=overallHeight*1.2;//+fm.ascent()*0.1;
    baselineHeight=baselineHeight*1.2;//+fm.ascent()*0.1;
    width=width+fm.boundingRect("A").width()*2; // 1.53
}

double JKQTmathText::MTsqrtNode::draw(QPainter& painter, double x, double y, JKQTmathText::MTenvironment currentEv) {
    doDrawBoxes(painter, x, y, currentEv);
    double width=0, baselineHeight=0, overallHeight=0, sp=0;
    child->getSize(painter, currentEv, width, baselineHeight, overallHeight, sp);
    QFont f=currentEv.getFont(parent);
    QFont fsmall=f;
    QFontMetricsF fm(f, painter.device());
    double w=fm.boundingRect("A").width();
    double a=baselineHeight*1.15;
    double d=overallHeight-baselineHeight;
    //painter.save();
    QPen p=painter.pen();
    p.setColor(currentEv.color);
    p.setWidthF(qMax(JKQTMATHTEXT_ABS_MIN_LINEWIDTH,ceil(currentEv.fontSize/16.0)));
    //painter.setPen(p);
    QPainterPath path;
    if (w>0) {
        path.moveTo(x+0.1*w, y-0.4*a);
        path.lineTo(x+0.33*w, y-0.4*a);
        path.lineTo( x+0.66*w, y+0.5*d);
        path.lineTo(x+w, y-a);
    }
    if (degree!=2) {
        fsmall.setPointSizeF(fsmall.pointSizeF()/2.0);
        fsmall.setItalic(false);
        painter.setFont(fsmall);
        painter.drawText(QPointF(x+0.33*w, y-0.55*a), QLocale::c().toString(degree));
    }
    //painter.restore();
    double xnew=child->draw(painter, x+1.2*w, y, currentEv);
    painter.save();
    painter.setPen(p);
    if (w>0) {
        path.lineTo( xnew+0.2*w, y-a);
        path.lineTo(xnew+0.2*w, y-0.8*a);
        painter.drawPath(path);
    }
    painter.restore();
    return xnew+0.33*w;
}

bool JKQTmathText::MTsqrtNode::toHtml(QString &html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv) {
    html=html+"&radic;<span style=\"text-decoration:overline\">";
    bool ok=child->toHtml(html, currentEv, defaultEv);
    html=html+"&nbsp;</span>";
    return ok;
}

void JKQTmathText::MTsqrtNode::set_drawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child->set_drawBoxes(draw);

}

QString JKQTmathText::MTsqrtNode::getTypeName() const
{
    return "MTsqrtNode";
}






JKQTmathText::MTfracNode::MTfracNode(JKQTmathText* parent, MTnode* child_top, MTnode* child_bottom, MTfracMode mode):
    JKQTmathText::MTnode(parent)
{
    this->child1=child_top;
    this->child2=child_bottom;
    this->mode=mode;
}

JKQTmathText::MTfracNode::~MTfracNode() {
    if (child1!=nullptr) delete child1;
    if (child2!=nullptr) delete child2;
}

QString JKQTmathText::MTfracNode::getTypeName() const
{
    return "MTfracNode";
}

void JKQTmathText::MTfracNode::getSizeInternal(QPainter& painter, JKQTmathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos) {
    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    JKQTmathText::MTenvironment ev1=currentEv;
    JKQTmathText::MTenvironment ev2=currentEv;

    double xh=fm.xHeight(); //tightBoundingRect("x").height();
    double sp=xh;
    double Ah=parent->getTBR(currentEv.getFont(parent), "M", painter.device()).height();//fm.ascent();
    double xw=fm.boundingRect("x").width();

    if (mode==MTFMunderbrace || mode==MTFMoverbrace) {
        ev2.fontSize=ev2.fontSize*parent->get_underbrace_factor();
    } else if (mode==MTFMunderset || mode==MTFMoverset) {
        ev2.fontSize=ev2.fontSize*parent->get_underset_factor();
    } else  if (mode==MTFMfrac) {
        ev1.fontSize=ev1.fontSize*parent->get_frac_factor();
        ev2.fontSize=ev2.fontSize*parent->get_frac_factor();
    } else  if (mode==MTFMtfrac) {
        ev1.fontSize=ev1.fontSize*parent->get_frac_factor()*0.7;
        ev2.fontSize=ev2.fontSize*parent->get_frac_factor()*0.7;
    }


    double width1=0, baselineHeight1=0, overallHeight1=0, strikeoutPos1=0;
    double width2=0, baselineHeight2=0, overallHeight2=0, strikeoutPos2=0;
    child1->getSize(painter, ev1, width1, baselineHeight1, overallHeight1, strikeoutPos1);
    child2->getSize(painter, ev2, width2, baselineHeight2, overallHeight2, strikeoutPos2);

    overallHeight=0;
    baselineHeight=0;
    width=0;
    if (mode==MTFMfrac || mode==MTFMdfrac || mode==MTFMtfrac) {
        //std::cout<<"\nxh="<<xh;
        //std::cout<<"\n  baselineHeight1="<<baselineHeight1<<",  overallHeight1="<<overallHeight1;
        //std::cout<<"\n  baselineHeight2="<<baselineHeight2<<",  overallHeight2="<<overallHeight2<<std::endl;
        //overallHeight=overallHeight1+overallHeight2+sp*(2.0*parent->get_frac_shift_factor());
        //baselineHeight=overallHeight1+xh*(2.0*parent->get_frac_shift_factor());
        overallHeight=2.0*qMax(overallHeight1, overallHeight2)+sp*(2.0*parent->get_frac_shift_factor());
        baselineHeight=qMax(overallHeight1, overallHeight2)+xh*(2.0*parent->get_frac_shift_factor());
        //std::cout<<"=>  baselineHeight="<<baselineHeight<<",  overallHeight="<<overallHeight<<std::endl;
        width=qMax(width1, width2)+ xw;
        strikeoutPos=sp;
    } else if (mode==MTFMstackrel) {
        //overallHeight=overallHeight1+overallHeight2+sp*(2.0*parent->get_frac_shift_factor());
        //baselineHeight=overallHeight1+xh*(2.0*parent->get_frac_shift_factor());
        overallHeight=2.0*qMax(overallHeight1, overallHeight2)+sp*(2.0*parent->get_frac_shift_factor());
        baselineHeight=qMax(overallHeight1, overallHeight2)+xh*(2.0*parent->get_frac_shift_factor());
        width=qMax(width1, width2)+ xw;
        strikeoutPos=sp;
    } else if (mode==MTFMunderbrace) {
        overallHeight=overallHeight1+overallHeight2+Ah/2.0;
        baselineHeight=baselineHeight1;
        width=qMax(width1, width2)+xw;
        strikeoutPos=sp;
    } else if (mode==MTFMoverbrace) {
        overallHeight=overallHeight1+overallHeight2+Ah/2.0;
        baselineHeight=baselineHeight1+overallHeight2+Ah/2.0;
        width=qMax(width1, width2)+xw;
        strikeoutPos=sp;
    } else if (mode==MTFMunderset) {
        overallHeight=overallHeight1+overallHeight2+xh/6.0;
        baselineHeight=baselineHeight1;
        width=qMax(width1, width2)+xw;
        strikeoutPos=sp;
    } else if (mode==MTFMoverset) {
        overallHeight=overallHeight1+overallHeight2+xh/6.0;
        baselineHeight=baselineHeight1+overallHeight2+xh/6.0;
        width=qMax(width1, width2)+xw;
        strikeoutPos=sp;
    }
}

double JKQTmathText::MTfracNode::draw(QPainter& painter, double x, double y, JKQTmathText::MTenvironment currentEv) {
    doDrawBoxes(painter, x, y, currentEv);
    QFont f=currentEv.getFont(parent);
    QFontMetricsF fm(f, painter.device());
    JKQTmathText::MTenvironment ev1=currentEv;
    JKQTmathText::MTenvironment ev2=currentEv;


    double xh=parent->getTBR(f, "x", painter.device()).height(); //fm.xHeight();
    double xw=fm.boundingRect("x").width();
    double lw=qMax(0.0,ceil(currentEv.fontSize/16.0));//fm.lineWidth();
    double Ah=parent->getTBR(f, "M", painter.device()).height();//fm.ascent();
    double bw=Ah/2.0;

    if (mode==MTFMunderbrace || mode==MTFMoverbrace) {
        ev2.fontSize=ev2.fontSize*parent->get_underbrace_factor();
    } else if (mode==MTFMunderset || mode==MTFMoverset) {
        ev2.fontSize=ev2.fontSize*parent->get_underset_factor();
    } else  if (mode==MTFMfrac) {
        ev1.fontSize=ev1.fontSize*parent->get_frac_factor();
        ev2.fontSize=ev2.fontSize*parent->get_frac_factor();
    } else  if (mode==MTFMtfrac) {
        ev1.fontSize=ev1.fontSize*parent->get_frac_factor()*0.7;
        ev2.fontSize=ev2.fontSize*parent->get_frac_factor()*0.7;
    }

    double width1=0, baselineHeight1=0, overallHeight1=0;//, strikeoutPos1=0;
    double width2=0, baselineHeight2=0, overallHeight2=0, strikeoutPos=0;
    child1->getSize(painter, ev1, width1, baselineHeight1, overallHeight1, strikeoutPos);
    child2->getSize(painter, ev2, width2, baselineHeight2, overallHeight2, strikeoutPos);
    double ascent1=baselineHeight1;
    double descent1=overallHeight1-baselineHeight1;
    double ascent2=baselineHeight2;
    double descent2=overallHeight2-baselineHeight2;

    double yline=y-xh*0.5;


    //double overallHeight=overallHeight1+overallHeight2+xh;
    //double baselineHeight=3.0*xh/2.0+overallHeight1;
    double width=qMax(width1, width2);

    QPen p=painter.pen();
    p.setColor(ev1.color);
    p.setStyle(Qt::SolidLine);
    p.setWidthF(qMax(JKQTMATHTEXT_ABS_MIN_LINEWIDTH, lw));
    painter.save();
    painter.setPen(p);
    if (mode==MTFMfrac || mode==MTFMdfrac || mode==MTFMtfrac) {
        QLineF l(x+xw/4.0, yline, x+width+xw/2.0, yline);
        if (l.length()>0) painter.drawLine(l);
        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, yline-xh*(parent->get_frac_shift_factor())-descent1, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, yline+xh*(parent->get_frac_shift_factor())+ascent2, ev2);
    } else if (mode==MTFMstackrel) {
        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, yline-xh*(parent->get_frac_shift_factor())-descent1, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, yline+xh*(parent->get_frac_shift_factor())+ascent2, ev2);
    } else if (mode==MTFMunderset) {
        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, y, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, y+descent1+xh/6.0+ascent2, ev2);
    } else if (mode==MTFMunderbrace) {
        double ybrace=y+descent1+bw/2.0;
        QPainterPath path=makeHBracePath(x+xw/2.0+(width1)/2.0, ybrace, width, bw);
        painter.drawPath(path);

        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, y, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, y+descent1+bw+ascent2, ev2);
    } else if (mode==MTFMoverset) {
        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, y, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, y-ascent1-xh/6.0-descent2, ev2);
    } else if (mode==MTFMoverbrace) {
        double ybrace=y-ascent1-bw/2.0;

        painter.save();
        painter.translate(x+xw/2.0+(width1)/2.0, ybrace);
        painter.rotate(180);
        QPainterPath path=makeHBracePath(0,0, width, bw);
        painter.drawPath(path);
        painter.restore();

        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, y, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, y-ascent1-bw-descent2, ev2);
    }
    painter.restore();

    if (mode==MTFMstackrel) return x+width+ xw;
    return x+width+xw;

}

bool JKQTmathText::MTfracNode::toHtml(QString &/*html*/, JKQTmathText::MTenvironment /*currentEv*/, JKQTmathText::MTenvironment /*defaultEv*/) {
    bool ok=false;



    return ok;
}

void JKQTmathText::MTfracNode::set_drawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child1->set_drawBoxes(draw);
    child2->set_drawBoxes(draw);

}









JKQTmathText::MTmatrixNode::MTmatrixNode(JKQTmathText* parent, QVector<QVector<MTnode*> > children):
    JKQTmathText::MTnode(parent)
{
    this->lines=children.size();
    this->columns=0;
    for (int i=0; i<children.size(); i++) {
        if (children[i].size()>this->columns) this->columns=children[i].size();
    }
    this->children=children;
}

JKQTmathText::MTmatrixNode::~MTmatrixNode() {
    for (int i=0; i<children.size(); i++) {
        for (int j=0; j<children[i].size(); j++) {
            delete children[i].at(j);
        }
    }
    children.clear();
}

QString JKQTmathText::MTmatrixNode::getTypeName() const
{
    return "MTmatrixNode";
}

void JKQTmathText::MTmatrixNode::getSizeInternal(QPainter& painter, JKQTmathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos) {
    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    JKQTmathText::MTenvironment ev1=currentEv;

    double xh=fm.strikeOutPos();//fm.xHeight();
    //double Ah=fm.ascent();
    double xw=fm.boundingRect("x").width();

    //ev1.fontSize=ev1.fontSize*parent->get_frac_factor();


    QVector<double> colwidth, rowheight;
    //QVector<QVector<double> > widths, heights, baselines;

    double width1=0, baselineHeight1=0, overallHeight1=0, strikeoutPos1=0;
    //widths.resize(lines);
    colwidth.resize(columns); for (int i=0; i<columns; i++) colwidth[i]=0;
    rowheight.resize(lines);
    //heights=baselines=widths;
    for (int i=0; i<lines; i++) {
        rowheight[i]=0;
        //widths[i].resize(columns);
        //baselines[i]=heights[i]=widths[i];
        for (int j=0; j<children[i].size(); j++) {
            children[i].at(j)->getSize(painter, ev1, width1, baselineHeight1, overallHeight1, strikeoutPos1);
            /*widths[i].operator[](j)=width1;
            baselines[i].operator[](j)=baselineHeight;
            heights[i].operator[](j)=overallHeight1;*/
            if (overallHeight1>rowheight[i]) rowheight[i]=overallHeight1;
            if (width1>colwidth[j]) colwidth[j]=width1;
        }
    }


    overallHeight=(lines-1)*xw/2.0;
    width=columns*xw;
    for (int i=0; i<columns; i++) width=width+colwidth[i];
    for (int i=0; i<lines; i++) overallHeight=overallHeight+rowheight[i];
    baselineHeight=overallHeight/2.0+xh;
    strikeoutPos=xh;
}

double JKQTmathText::MTmatrixNode::draw(QPainter& painter, double x, double y, JKQTmathText::MTenvironment currentEv) {
    doDrawBoxes(painter, x, y, currentEv);

    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    JKQTmathText::MTenvironment ev1=currentEv;

    double xh=fm.strikeOutPos();//fm.xHeight();
    //double Ah=fm.ascent();
    double xw=fm.boundingRect("x").width();

    //ev1.fontSize=ev1.fontSize;*parent->get_frac_factor();


    QVector<double> colwidth, rowheight, rowascent;
    //QVector<QVector<double> > widths, heights, baselines;

    double width1=0, baselineHeight1=0, overallHeight1=0, strikeoutPos=0;
    //widths.resize(lines);
    colwidth.resize(columns); for (int i=0; i<columns; i++) colwidth[i]=0;
    rowheight.resize(lines);
    rowascent.resize(lines);
    //heights=baselines=widths;
    for (int i=0; i<lines; i++) {
        rowheight[i]=0;
        rowascent[i]=0;
        //widths[i].resize(columns);
        //baselines[i]=heights[i]=widths[i];
        for (int j=0; j<children[i].size(); j++) {
            if (children[i].at(j)!=nullptr) children[i].at(j)->getSize(painter, ev1, width1, baselineHeight1, overallHeight1, strikeoutPos);
            /*widths[i].operator[](j)=width1;
            baselines[i].operator[](j)=baselineHeight;
            heights[i].operator[](j)=overallHeight1;*/
            if (overallHeight1>rowheight[i]) rowheight[i]=overallHeight1;
            if (baselineHeight1>rowascent[i]) rowascent[i]=baselineHeight1;
            if (width1>colwidth[j]) colwidth[j]=width1;
        }
    }

    double overallHeight=(lines-1)*xw/2.0;
    double width=(columns)*xw;
    for (int i=0; i<columns; i++) width=width+colwidth[i];
    for (int i=0; i<lines; i++) overallHeight=overallHeight+rowheight[i];
    double baselineHeight=overallHeight/2.0+xh;

    double xx=x;
    double yy=y-baselineHeight;
    if (lines>0) yy=yy+rowascent[0];
    for (int i=0; i<lines; i++) {
        xx=x;
        for (int j=0; j<children[i].size(); j++) {
            children[i].at(j)->draw(painter, xx, yy, ev1);
            xx=xx+colwidth[j]+xw;
        }

        if (i<lines-1) yy=yy+(rowheight[i]-rowascent[i])+xw/2.0+rowascent[i+1];
    }


    return x+width;
}

bool JKQTmathText::MTmatrixNode::toHtml(QString &/*html*/, JKQTmathText::MTenvironment /*currentEv*/, JKQTmathText::MTenvironment /*defaultEv*/)
{
    return false;
}

void JKQTmathText::MTmatrixNode::set_drawBoxes(bool draw)
{
    this->drawBoxes=draw;
    for (int i=0; i<lines; i++) {
        for (int j=0; j<children[i].size(); j++) {
            children[i].at(j)->set_drawBoxes(draw);
        }
    }

}








JKQTmathText::MTdecoratedNode::MTdecoratedNode(JKQTmathText* parent, MTdecoration decoration, MTnode* child):
    JKQTmathText::MTnode(parent)
{
    this->child=child;
    this->decoration=decoration;
}

JKQTmathText::MTdecoratedNode::~MTdecoratedNode() {
    if (child!=nullptr) delete child;
}

void JKQTmathText::MTdecoratedNode::getSizeInternal(QPainter& painter, JKQTmathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos) {
    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    double wc=fm.boundingRect("A").width();
    double dheightfactor=1.0+parent->get_decoration_height_factor()*2.0;

    child->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);

    overallHeight=overallHeight*dheightfactor;
    baselineHeight=baselineHeight*dheightfactor;
    width=width+0.3*wc;
}

double JKQTmathText::MTdecoratedNode::draw(QPainter& painter, double x, double y, JKQTmathText::MTenvironment currentEv) {
    doDrawBoxes(painter, x, y, currentEv);
    MTenvironment ev=currentEv;
    double width=0, baselineHeight=0, overallHeight=0, strikeoutPos=0;
    child->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);
    QFontMetricsF fm(ev.getFont(parent), painter.device());
    double w=width;
    double wc=fm.boundingRect("A").width();
    //double ll=wc*0.8;
    double a=baselineHeight;
    //double d=overallHeight-baselineHeight;
    double dheightfactor=parent->get_decoration_height_factor();
    double dpos=y-a*(1.0+dheightfactor);
    double dposb=y+qMax((overallHeight-baselineHeight)*(1.0+dheightfactor), fm.xHeight()*dheightfactor);
    double deltax=0;//(wc-ll)/2.0;
    double dheight=dheightfactor*a;


    if (ev.italic) deltax+=0.1*fm.boundingRect("A").width();
    QPen pold=painter.pen();
    QPen p=pold;
    p.setColor(ev.color);
    p.setWidthF(qMax(JKQTMATHTEXT_ABS_MIN_LINEWIDTH, fm.lineWidth()));//ceil(currentEv.fontSize/16.0));
    if (decoration==MTDbar) ev.overline=true;
    double xnew=child->draw(painter, x, y, ev);
    if (decoration==MTDvec) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(x+deltax, dpos)<<QPointF(xnew+deltax-0.2*wc, dpos)<<QPointF(xnew-0.4*wc+deltax, dpos-dheight/2.0);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    } else if (decoration==MTDoverline) {
        painter.setPen(p);
        QLineF l(x+deltax, dpos, xnew+deltax-0.2*wc, dpos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDdoubleoverline) {
        painter.setPen(p);
        QLineF l(x+deltax, dpos, xnew+deltax-0.2*wc, dpos);
        if (l.length()>0) painter.drawLine(l);
        l=QLineF(x+deltax, dpos-2.0*p.widthF(), xnew+deltax-0.2*wc, dpos-2.0*p.widthF());
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDunderline) {
        painter.setPen(p);
        QLineF l(x+deltax, dposb, xnew+deltax-0.2*wc, dposb);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDdoubleunderline) {
        painter.setPen(p);
        QLineF l(x+deltax, dposb, xnew+deltax-0.2*wc, dposb);
        if (l.length()>0) painter.drawLine(l);
        l=QLineF(x+deltax, dposb+2.0*p.widthF(), xnew+deltax-0.2*wc, dposb+2.0*p.widthF());
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDarrow) {
        painter.setPen(p);
        QLineF l(x+deltax, dpos+dheight/2.0, xnew+deltax-0.2*wc, dpos+dheight/2.0);
        if (l.length()>0) painter.drawLine(l);
        QPolygonF poly;
        poly<<QPointF(xnew+deltax-0.4*wc, dpos)<<QPointF(xnew+deltax-0.2*wc, dpos+dheight/2.0)<<QPointF(xnew+deltax-0.4*wc, dpos+dheight);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    /*} else if (decoration==MTDbar) {
        painter.setPen(p);
        painter.drawLine(QLineF(x+0.2*wc+deltax, dpos, xnew+0.2*wc+deltax, dpos);
        painter.setPen(pold);*/
    } else if (decoration==MTDhat) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(x+w/2.0-0.35*wc+deltax, dpos+dheight/2.0)<<QPointF(x+w/2.0+deltax, dpos)<<QPointF(x+w/2.0+0.35*wc+deltax, dpos+dheight/2.0);
        painter.drawPolyline(poly);
        painter.setPen(pold);

    } else if (decoration==MTDtilde) {
        painter.setPen(p);
        QPainterPath path;
        double x0=x+deltax;
        double x1=xnew+deltax-0.2*wc;
        path.moveTo(x0, dpos);
        path.cubicTo((x0+x1)/2.0, dpos+dheight,  (x0+x1)/2.0, dpos-dheight    , x1, dpos);
        painter.drawPath(path);
        painter.setPen(pold);

    } else if (decoration==MTDdot) {
        p.setWidthF(0.01);
        painter.setPen(p);
        QBrush b=painter.brush();
        QBrush bold=b;
        b.setColor(ev.color);
        b.setStyle(Qt::SolidPattern);
        painter.setBrush(b);
        painter.drawEllipse(QPointF(x+w/2.0+deltax, dpos), wc/10.0, wc/10.0);
        painter.setBrush(bold);
        painter.setPen(pold);
    } else if (decoration==MTDddot) {
        p.setWidthF(0.01);
        painter.setPen(p);
        QBrush b=painter.brush();
        QBrush bold=b;
        b.setColor(ev.color);
        b.setStyle(Qt::SolidPattern);
        painter.setBrush(b);
        painter.drawEllipse(QPointF(x+w/2.0-0.2*wc+deltax, dpos), wc/10.0, wc/10.0);
        painter.drawEllipse(QPointF(x+w/2.0+0.2*wc+deltax, dpos), wc/10.0, wc/10.0);
        painter.setBrush(bold);
        painter.setPen(pold);
        painter.setBrush(bold);
    }
    return xnew+0.3*wc;
}

bool JKQTmathText::MTdecoratedNode::toHtml(QString &/*html*/, JKQTmathText::MTenvironment /*currentEv*/, JKQTmathText::MTenvironment /*defaultEv*/) {
    //QString f;
    //JKQTmathText::MTenvironment ev=currentEv;

    //bool ok=child->toHtml(html, ev, defaultEv);

    return false;
}

void JKQTmathText::MTdecoratedNode::set_drawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child->set_drawBoxes(draw);

}

QString JKQTmathText::MTdecoratedNode::getTypeName() const
{
    return "MTdecoratedNode";
}






JKQTmathText::MTsuperscriptNode::MTsuperscriptNode(JKQTmathText* parent, MTnode* child):
    JKQTmathText::MTnode(parent)
{
    this->child=child;
}

JKQTmathText::MTsuperscriptNode::~MTsuperscriptNode() {
    if (child!=nullptr) delete child;
}

void JKQTmathText::MTsuperscriptNode::getSizeInternal(QPainter& painter, JKQTmathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos) {
    JKQTmathText::MTenvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parent->get_subsuper_size_factor();
    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    child->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);
    double shift=0;//parent->get_super_shift_factor()*fm.ascent();
    overallHeight=overallHeight+shift;
    strikeoutPos=fm.strikeOutPos();
    if (currentEv.italic) width=width+double(fm.width(' '))*parent->get_italic_correction_factor();
}

double JKQTmathText::MTsuperscriptNode::draw(QPainter& painter, double x, double y, JKQTmathText::MTenvironment currentEv) {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTmathText::MTenvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parent->get_subsuper_size_factor();

    double cwidth, cbaselineHeight, coverallheight, cStrikeoutPos;
    child->getSize(painter, ev, cwidth, cbaselineHeight, coverallheight, cStrikeoutPos);

    QFontMetricsF fm(currentEv.getFont(parent), painter.device());

    double shift=0;//parent->get_super_shift_factor()*fm.ascent();
    //double shift=ev.;
    double xx=x;
    if (currentEv.italic) xx=xx+double(fm.width(' '))*parent->get_italic_correction_factor();

    return child->draw(painter, xx, y-shift, ev);//+0.5*fm.boundingRect("A").width();
}

bool JKQTmathText::MTsuperscriptNode::isSubOrSuper()
{
    return true;
}

QString JKQTmathText::MTsuperscriptNode::getTypeName() const
{
    return "MTsuperscriptNode";
}


bool JKQTmathText::MTsuperscriptNode::toHtml(QString &html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv)
{
    html=html+"<sup>";
    bool ok=child->toHtml(html, currentEv, defaultEv);
    html=html+"</sup>";
    return ok;
}

void JKQTmathText::MTsuperscriptNode::set_drawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child->set_drawBoxes(draw);

}






JKQTmathText::MTbraceNode::MTbraceNode(JKQTmathText* parent, QString openbrace, QString closebrace, MTnode* child, bool showRightBrace):
    JKQTmathText::MTnode(parent)
{
    this->child=child;
    this->openbrace=openbrace;
    this->closebrace=closebrace;
    this->showRightBrace=showRightBrace;
}

JKQTmathText::MTbraceNode::~MTbraceNode() {
    if (child!=nullptr) delete child;
}

void JKQTmathText::MTbraceNode::getSizeInternal(QPainter& painter, JKQTmathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos) {

    JKQTmathText::MTenvironment ev=currentEv;
    child->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);

    double bracewidth=0, braceheight=0;
    getBraceWidth(painter, ev, baselineHeight, overallHeight, bracewidth, braceheight);

    bracewidth=bracewidth/parent->get_brace_shrink_factor();

    baselineHeight=/*qMin(baselineHeight, braceheight)*/ baselineHeight*parent->get_brace_factor();
    overallHeight=qMax(overallHeight, braceheight)*parent->get_brace_factor(); //fm.height();

    width=width+bracewidth*2.0;


}

double JKQTmathText::MTbraceNode::draw(QPainter& painter, double x, double y, JKQTmathText::MTenvironment currentEv) {
    //std::cout<<"drawing brace-node: '"<<openbrace.toStdString()<<"' ... '"<<closebrace.toStdString()<<"'\n";
    doDrawBoxes(painter, x, y, currentEv);
    JKQTmathText::MTenvironment ev=currentEv;

    double width=0;
    double baselineHeight=0;
    double overallHeight=0, strikeoutPos=0;

    child->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);

    double bracewidth=0, braceheight=0;
    getBraceWidth(painter, ev, baselineHeight, overallHeight, bracewidth, braceheight);

    double cwidth=0;
    double cbaselineHeight=0;
    double coverallHeight=0, cstrikeoutPos=0;

    getSize(painter, currentEv, cwidth, cbaselineHeight, coverallHeight, cstrikeoutPos);

    double lw=qMax(0.25,ceil(currentEv.fontSize/12.0));//fm.lineWidth();

    double xnew=x+lw;

    QPen pold=painter.pen();
    QPen p=pold;
    p.setWidthF(lw);
    p.setColor(currentEv.color);
    painter.setPen(p);
    double brace_fraction=0.85;
    if (openbrace=="(") {
        QPainterPath path;
        double y1=y+(coverallHeight-cbaselineHeight);
        double y2=y-cbaselineHeight;
        path.moveTo(xnew+brace_fraction*bracewidth, y1);
        path.cubicTo(xnew, (y1+y2)/2.0+fabs(y1-y2)/6.0, xnew, (y1+y2)/2.0-fabs(y1-y2)/6.0   , xnew+brace_fraction*bracewidth, y2);
        painter.drawPath(path);
    } else if (openbrace=="[") {
        QPainterPath path;
        double y1=y+(coverallHeight-cbaselineHeight);
        double y2=y-cbaselineHeight;
        path.moveTo(xnew+brace_fraction*bracewidth, y1);
        path.lineTo(xnew+lw/2.0, y1);
        path.lineTo(xnew+lw/2.0, y2);
        path.lineTo(xnew+brace_fraction*bracewidth, y2);
        painter.drawPath(path);
    } else if (openbrace=="{") {
        QPainterPath path=makeHBracePath(0,0,coverallHeight, bracewidth*brace_fraction);
        painter.save();
        painter.translate(xnew+bracewidth*(1.0-brace_fraction), y-cbaselineHeight+coverallHeight/2.0);
        painter.rotate(90);
        painter.drawPath(path);
        painter.restore();
    } else if (openbrace=="_") {
        QPainterPath path;
        double y1=y+(coverallHeight-cbaselineHeight);
        double y2=y-cbaselineHeight;
        path.moveTo(xnew+brace_fraction*bracewidth, y1);
        path.lineTo(xnew, y1);
        path.lineTo(xnew, y2);
        painter.drawPath(path);
    } else if (openbrace=="~") {
        QPainterPath path;
        double y1=y+(coverallHeight-cbaselineHeight);
        double y2=y-cbaselineHeight;
        path.moveTo(xnew, y1);
        path.lineTo(xnew, y2);
        path.lineTo(xnew+brace_fraction*bracewidth, y2);
        painter.drawPath(path);
    } else if (openbrace=="|") {
        QPainterPath path;
        double y1=y+(coverallHeight-cbaselineHeight);
        double y2=y-cbaselineHeight;
        QLineF l(xnew+brace_fraction*bracewidth, y1, xnew+brace_fraction*bracewidth, y2);
        if (l.length()>0) painter.drawLine(l);
        painter.drawPath(path);
    } else if (openbrace=="#" || openbrace=="||") {
        QPainterPath path;
        double y1=y+(coverallHeight-cbaselineHeight);
        double y2=y-cbaselineHeight;
        QLineF l(xnew+brace_fraction*bracewidth, y1, xnew+brace_fraction*bracewidth, y2);
        if (l.length()>0) painter.drawLine(l);
        l=QLineF(xnew+brace_fraction*bracewidth-1.5*lw, y1, xnew+brace_fraction*bracewidth-1.5*lw, y2);
        if (l.length()>0) painter.drawLine(l);
    } else if (openbrace=="<") {
        QPainterPath path;
        double y1=y+(coverallHeight-cbaselineHeight);
        double y2=y-cbaselineHeight;
        path.moveTo(xnew+brace_fraction*bracewidth, y1);
        path.lineTo(xnew, (y2+y1)/2.0);
        path.lineTo(xnew+brace_fraction*bracewidth, y2);
        painter.drawPath(path);
    }

    painter.setPen(pold);

    xnew= child->draw(painter, xnew+bracewidth/parent->get_brace_shrink_factor()-lw, y, currentEv)+lw;

    if (showRightBrace) {
        painter.setPen(p);
        if (closebrace==")") {
            QPainterPath path;
            double y1=y+(coverallHeight-cbaselineHeight);
            double y2=y-cbaselineHeight;
            path.moveTo(xnew+(1.0-brace_fraction)*bracewidth, y1);
            path.cubicTo(xnew+bracewidth, (y1+y2)/2.0+fabs(y1-y2)/6.0, xnew+bracewidth, (y1+y2)/2.0-fabs(y1-y2)/6.0   , xnew+(1.0-brace_fraction)*bracewidth, y2);
            painter.drawPath(path);
        } else if (closebrace=="]") {
            QPainterPath path;
            double y1=y+(coverallHeight-cbaselineHeight);
            double y2=y-cbaselineHeight;
            path.moveTo(xnew+(1.0-brace_fraction)*bracewidth, y1);
            path.lineTo(xnew+bracewidth-lw/2.0, y1);
            path.lineTo(xnew+bracewidth-lw/2.0, y2);
            path.lineTo(xnew+(1.0-brace_fraction)*bracewidth, y2);
            painter.drawPath(path);
        } else if (closebrace=="}") {
            QPainterPath path=makeHBracePath(0,0,coverallHeight, bracewidth*brace_fraction);
            painter.save();
            painter.translate(xnew+bracewidth*brace_fraction, y-cbaselineHeight+coverallHeight/2.0);
            painter.rotate(270);
            painter.drawPath(path);
            painter.restore();
        } else if (closebrace=="_") {
            QPainterPath path;
            double y1=y+(coverallHeight-cbaselineHeight);
            double y2=y-cbaselineHeight;
            path.moveTo(xnew+(1.0-brace_fraction)*bracewidth, y1);
            path.lineTo(xnew+bracewidth, y1);
            path.lineTo(xnew+bracewidth, y2);
            painter.drawPath(path);
        } else if (closebrace=="~") {
            QPainterPath path;
            double y1=y+(coverallHeight-cbaselineHeight);
            double y2=y-cbaselineHeight;
            path.moveTo(xnew+bracewidth, y1);
            path.lineTo(xnew+bracewidth, y2);
            path.lineTo(xnew+(1.0-brace_fraction)*bracewidth, y2);
            painter.drawPath(path);
        } else if (closebrace=="|") {
            QPainterPath path;
            double y1=y+(coverallHeight-cbaselineHeight);
            double y2=y-cbaselineHeight;
            QLineF l(xnew+(1.0-brace_fraction)*bracewidth, y1, xnew+(1.0-brace_fraction)*bracewidth, y2);
            if (l.length()>0) painter.drawLine(l);
            painter.drawPath(path);
        } else if (closebrace=="#" || closebrace=="||") {
            QPainterPath path;
            double y1=y+(coverallHeight-cbaselineHeight);
            double y2=y-cbaselineHeight;
            QLineF l(xnew+(1.0-brace_fraction)*bracewidth, y1, xnew+(1.0-brace_fraction)*bracewidth, y2);
            if (l.length()>0) painter.drawLine(l);
            l=QLineF(xnew+(1.0-brace_fraction)*bracewidth+1.5*lw, y1, xnew+(1.0-brace_fraction)*bracewidth+1.5*lw, y2);
            if (l.length()>0) painter.drawLine(l);
        } else if (closebrace==">") {
            QPainterPath path;
            double y1=y+(coverallHeight-cbaselineHeight);
            double y2=y-cbaselineHeight;
            path.moveTo(xnew+(1.0-brace_fraction)*bracewidth, y1);
            path.lineTo(xnew+bracewidth, (y2+y1)/2.0);
            path.lineTo(xnew+(1.0-brace_fraction)*bracewidth, y2);
            painter.drawPath(path);
        }
        painter.setPen(pold);
    }

    //qDebug()<<" ==> "<<bc<<fm.boundingRect(bc).width();
    return xnew+bracewidth/parent->get_brace_shrink_factor()-lw;
}

bool JKQTmathText::MTbraceNode::toHtml(QString &html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv) {
    QString ob=openbrace;
    QString cb=closebrace;
    if (ob=="<") ob="&lang;";
    else if (ob=="_") ob="&lfloor;";
    else if (ob=="~") ob="&lceil;";
    else if (ob=="||" || ob=="#") ob="||";
    if (cb=="<") cb="&rang;";
    else if (cb=="_") cb="&rfloor;";
    else if (cb=="~") cb="&rceil;";
    else if (cb=="||" || cb=="#") cb="||";


    html=html+ob;

    bool ok=child->toHtml(html, currentEv, defaultEv);

    html=html+cb;

    return ok;
}

void JKQTmathText::MTbraceNode::set_drawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child->set_drawBoxes(draw);

}

QString JKQTmathText::MTbraceNode::getTypeName() const
{
    return QLatin1String("MTbraceNode(")+openbrace+" "+closebrace+")";
}

void JKQTmathText::MTbraceNode::getBraceWidth(QPainter &/*painter*/, JKQTmathText::MTenvironment ev, double /*baselineHeight*/, double overallHeight, double &bracewidth, double &braceheight)
{
    /*QFont evf=ev.getFont(parent);
    if (ev.insideMath) evf.setItalic(false);
    ev.italic=false;
    while (ev.fontSize<10*parent->get_fontSize()) {
        const QFontMetricsF fme(evf, painter.device());
        if (fme.ascent()>overallHeight) break;
        ev.fontSize+=0.5;
        evf.setPointSizeF(ev.fontSize);
    }
    ev.fontSize=ev.fontSize*parent->get_brace_factor();
    evf.setPointSizeF(ev.fontSize);
    QFontMetricsF fm(evf, painter.device());
    QString bc="_X";
    bracewidth=fm.width("I")*parent->get_brace_shrink_factor();
    braceheight=parent->getTBR(evf, bc, painter.device()).height();*/
    double lw=qMax(0.25,ceil(ev.fontSize/12.0));
    braceheight=overallHeight*parent->get_brace_factor();
    bracewidth=0.6*pow(braceheight, 0.6);
    if (openbrace=="{" || closebrace=="}")  bracewidth=qMax(bracewidth, lw*3.5);

}





JKQTmathText::MTlistNode::MTlistNode(JKQTmathText* parent):
    JKQTmathText::MTnode(parent)
{
    nodes.clear();
    // these operations cause sub/sup script to be typeset over/under the operator, not right besides!
    subsupOperations<<"sum"<<"prod"<<"coprod"
               <<"bigcap"<<"bigcup"<<"bigvee"<<"bighat"
               <<"int"<<"iint"<<"iiint"<<"oint"<<"oiint"<<"oiiint"
               <<"max"<<"min"<<"argmax"<<"argmin"<<"sup"<<"inf"
               <<"liminf"<<"limsup"<<"lim"<<"max"<<"min";
}

JKQTmathText::MTlistNode::~MTlistNode() {
    for (int i=0; i<nodes.size(); i++) {
        delete nodes[i];
    }
    nodes.clear();
}

QString JKQTmathText::MTlistNode::getTypeName() const
{
    return "MTlistNode";
}

void JKQTmathText::MTlistNode::getSizeInternal(QPainter& painter, JKQTmathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos) {
    width=0;
    overallHeight=0;
    baselineHeight=0;
    strikeoutPos=0;
    QFontMetricsF fm(currentEv.getFont(parent));
    QRectF tbr=parent->getTBR(currentEv.getFont(parent), "M", painter.device());


    double xnew=0;
    bool wasBrace=false;
    for (int i=0; i<nodes.size(); i++) {
        {
            double w1, oh, bh, sp;
            nodes[i]->getSize(painter, currentEv, w1, bh, oh, sp);
            //qDebug()<<"i="<<i<<" width="<<width<<" baselineHeight="<<baselineHeight<<" overallHeight="<<overallHeight<<" strikeoutPos="<<strikeoutPos<<"\n"<<"### nodes["<<i<<"] = "<<nodes[i]->getTypeName()<< "   w="<<w1<<" bh="<<bh<<" oh="<<oh<<" sp="<<sp;
        }
        bool doDraw=true;
        MTsymbolNode* smb=dynamic_cast<MTsymbolNode*>(nodes[i]);
        // if we find a subscript/superscript node we check whether the next node is super/subscript
        // if so, we typeset them at the same x-psotion, so sub/superscripts appear correctly
        if (dynamic_cast<MTsuperscriptNode*>(nodes[i])) {
            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<MTsubscriptNode*>(nodes[i+1])) { // is this subscript?
                    double w1, w2, oh, bh, sp;
                    nodes[i]->getSize(painter, currentEv, w1, bh, oh, sp);                    
                    double shift=parent->get_super_shift_factor()*fm.xHeight()+(oh-bh);//(overallHeight-baselineHeight)+(oh-bh);
                    if (wasBrace) {
                        shift=baselineHeight-parent->get_super_shift_factor()*tbr.height()+(oh-bh);
                    }


                    //qDebug()<<"super_sub: super: "<<nodes[i]->getTypeName()<<"  w1="<<w1<<" bh"<<bh<<" oh="<<oh<<" sp="<<sp;
                    if (shift+bh>baselineHeight) {
                        double lheight=overallHeight-baselineHeight;
                        baselineHeight=shift+bh;
                        overallHeight=baselineHeight+lheight;
                    }
                    i++;
                    nodes[i]->getSize(painter, currentEv, w2, bh, oh, sp);
                    //qDebug()<<"super_sub:   sub: "<<nodes[i]->getTypeName()<<"  w2="<<w2<<" bh"<<bh<<" oh="<<oh<<" sp="<<sp;
                    if (shift+oh-bh>overallHeight-baselineHeight) {
                        overallHeight=baselineHeight+shift+(oh-bh);
                    }
                    doDraw=false;
                    xnew+=qMax(w1+fm.width(" "), w2);

                    //qDebug()<<"### super+sub";
                    //qDebug()<<"### subsupop: super+sub   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            }
        } else if (dynamic_cast<MTsubscriptNode*>(nodes[i])) {
            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<MTsuperscriptNode*>(nodes[i+1])) { // is this subscript?
                    double w1, w2, oh, bh, sp;
                    nodes[i]->getSize(painter, currentEv, w1, bh, oh, sp);
                    double shift=parent->get_super_shift_factor()*fm.xHeight()+(oh-bh);//(overallHeight-baselineHeight)+(oh-bh);
                    if (wasBrace) {
                        shift=baselineHeight-parent->get_super_shift_factor()*parent->getTBR(currentEv.getFont(parent), "M", painter.device()).height()+(oh-bh);
                    }

                    //qDebug()<<"sub_super:   sub: "<<nodes[i]->getTypeName()<<"  w1="<<w1<<" bh"<<bh<<" oh="<<oh<<" sp="<<sp;
                    if (shift+oh-bh>overallHeight-baselineHeight) {
                        overallHeight=baselineHeight+shift+(oh-bh);
                    }
                    i++;
                    nodes[i]->getSize(painter, currentEv, w2, bh, oh, sp);
                    //qDebug()<<"sub_super: super: "<<nodes[i]->getTypeName()<<"  w2="<<w2<<" bh"<<bh<<" oh="<<oh<<" sp="<<sp;
                    if (shift+bh>baselineHeight) {
                        double lheight=overallHeight-baselineHeight;
                        baselineHeight=shift+bh;
                        overallHeight=baselineHeight+lheight;
                    }
                    doDraw=false;
                    xnew+=qMax(w1, w2+fm.width(" "));
                    //qDebug()<<"### sub+super";
                    //qDebug()<<"### subsupop: sub+super1   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            }
        } else if (smb) {
            QString s=smb->get_symbolName();
            if (subsupOperations.contains(s)) {
                MTsubscriptNode* subn=nullptr;
                if (i+1<nodes.size()) subn=dynamic_cast<MTsubscriptNode*>(nodes[i+1]);
                MTsuperscriptNode* supn=nullptr;
                if (i+2<nodes.size()) supn=dynamic_cast<MTsuperscriptNode*>(nodes[i+2]);
                //std::cout<<"symbol ='"<<s.toStdString()<<"'   subn="<<subn<<"   supn="<<supn<<"\n";
                if (subn && supn) { // is this subscript and superscript?
                    MTenvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parent->get_operatorsubsuper_size_factor();
                    double w1=0, w2=0, w3=0;
                    double oh1=0, oh2=0, oh3=0;
                    double bh1=0, bh2=0, bh3=0;
                    double sp1=0, sp2=0, sp3=0;
                    nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp1);
                    //qDebug()<<"sub_super:   node: "<<nodes[i]->getTypeName()<<"  w1="<<w1<<" bh"<<bh1<<" oh="<<oh1<<" sp="<<sp1;
                    subn->get_child()->getSize(painter, ev, w2, bh2, oh2, sp2);
                    //qDebug()<<"sub_super:   node: "<<subn->getTypeName()<<"  w2="<<w2<<" bh2"<<bh2<<" oh2="<<oh2<<" sp2="<<sp2;
                    supn->get_child()->getSize(painter, ev, w3, bh3, oh3, sp3);
                    //qDebug()<<"sub_super:   node: "<<supn->getTypeName()<<"  w3="<<w3<<" bh3"<<bh3<<" oh3="<<oh3<<" sp3="<<sp3;
                    //double d1=oh1-bh1;
                    //double d2=oh2-bh2;
                    //double d3=oh3-bh3;
                    double w=qMax(qMax(w1, w2), w3)+fm.width(" ");

                    double oh=oh1+oh2+oh3;
                    double bh=bh1+oh3;
                    if (oh>overallHeight) overallHeight=oh;
                    if (bh>baselineHeight) { baselineHeight=bh; strikeoutPos=sp1; }
                    if (oh-bh>overallHeight-baselineHeight) {
                        overallHeight=baselineHeight+oh-bh;
                    }

                    i++;
                    i++;
                    doDraw=false;
                    xnew+=w;
                    //qDebug()<<"### subsupop: sub+super2   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                /*} else if (subn && supn) { // is this subscript and superscript?
                    MTenvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parent->get_operatorsubsuper_size_factor();
                    double w1=0, w2=0, w3=0;
                    double oh1=0, oh2=0, oh3=0;
                    double bh1=0, bh2=0, bh3=0;
                    double sp1=0, sp2=0, sp3=0;
                    nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp1);
                    supn->get_child()->getSize(painter, ev, w3, bh3, oh3, sp3);
                    //double d1=oh1-bh1;
                    //double d2=oh2-bh2;
                    //double d3=oh3-bh3;
                    double w=qMax(w1, w3)+fm.width(" ");

                    double oh=oh1+oh3;
                    double bh=bh1+oh3;
                    if (oh>overallHeight) overallHeight=oh;
                    if (bh>baselineHeight) { baselineHeight=bh; strikeoutPos=sp1; }
                    if (oh-bh>overallHeight-baselineHeight) {
                        overallHeight=baselineHeight+oh-bh;
                    }

                    i++;
                    i++;
                    doDraw=false;
                    xnew+=w;
                    //qDebug()<<"### subsupop: sub+super";*/
                } else if (subn) { // is this subscript?
                    MTenvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parent->get_operatorsubsuper_size_factor();
                    double w1=0, w2=0;
                    double oh1=0, oh2=0;
                    double bh1=0, bh2=0;
                    double sp1=0, sp2=0;//, sp3=0;
                    nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp1);
                    subn->get_child()->getSize(painter, ev, w2, bh2, oh2, sp2);
                    //double d1=oh1-bh1;
                    //double d2=oh2-bh2;

                    double oh=oh1+oh2;
                    double sh=oh1-bh1+oh2*1.1;
                    if (oh>overallHeight) overallHeight=oh;
                    if (bh1>baselineHeight) baselineHeight=bh1;
                    if (sh>overallHeight-baselineHeight) {
                        overallHeight=baselineHeight+sh;
                    }
                    double w=qMax(w1, w2)+fm.width(" ");
                    i++;
                    doDraw=false;
                    xnew+=w;
                    //qDebug()<<"### subsupop: sub   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            }
        }

        if (i<nodes.size() && doDraw) {
            double w, oh, bh, sp;
            nodes[i]->getSize(painter, currentEv, w, bh, oh, sp);
            double shift=0;

            if (dynamic_cast<MTsuperscriptNode*>(nodes[i])) {
                //QFontMetricsF fm(currentEv.getFont(parent));
                //QRectF tbr=fm.tightBoundingRect("M");
                shift=parent->get_super_shift_factor()*tbr.height()+(oh-bh);//((overallHeight-baselineHeight)+(oh-bh));
                if (wasBrace) {
                    shift=baselineHeight-parent->get_super_shift_factor()*tbr.height()+(oh-bh);
                }
                //qDebug()<<"+++ super: bh="<<bh<<" oh="<<oh<<" shift="<<shift<<" wasBrace="<<wasBrace<<" baselineHeight="<<baselineHeight<<" overallHeight="<<overallHeight<<" fm.ascent="<<fm.ascent();

                w+=fm.width(" ");
                if (shift+bh>baselineHeight) {
                    double lheight=overallHeight-baselineHeight;
                    baselineHeight=shift+bh;
                    overallHeight=baselineHeight+lheight;
                }
                //qDebug()<<"### super";
                //qDebug()<<"### subsupop: super   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
            } else if (dynamic_cast<MTsubscriptNode*>(nodes[i])) {
                //QFontMetricsF fm(currentEv.getFont(parent));
                shift=0;//parent->get_super_shift_factor()*fm.ascent();//((overallHeight-baselineHeight)+(oh-bh));
                w+=fm.width(" ");
                if (shift+oh-bh>overallHeight-baselineHeight) {
                    overallHeight=baselineHeight+shift+(oh-bh);
                }
                //qDebug()<<"### sub";
                //qDebug()<<"### subsupop: sub   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
            } else {
                //qDebug()<<"### else:  bh="<<bh<<" baselineHeight="<<baselineHeight<<"   oh="<<oh<<" overallHeight="<<overallHeight;
                if (bh>baselineHeight) {
                    overallHeight=overallHeight+bh-baselineHeight;
                    baselineHeight=bh;
                    strikeoutPos=sp;
                }
                if (baselineHeight+oh-bh>overallHeight) {
                    overallHeight=baselineHeight+oh-bh;
                    strikeoutPos=sp;
                }
                //qDebug()<<"### subsupop: else   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
            }
            xnew+=w;
            //qDebug()<<i<<xnew;
            //if (baselineHeight+oh+shift>overallHeight) overallHeight=baselineHeight+oh+shift;
            /*if (oh+shift-bh>overallHeight-baselineHeight) {
                overallHeight=baselineHeight+oh+shift-bh;
            }*/
        }
         wasBrace=dynamic_cast<MTbraceNode*>(nodes[i]);
    }
    width=xnew;
}

double JKQTmathText::MTlistNode::draw(QPainter& painter, double x, double y, JKQTmathText::MTenvironment currentEv) {
    doDrawBoxes(painter, x, y, currentEv);
    double ynew=y;
    double xnew=x;
    double cwidth=0, cbaselineHeight=0, coverallHeight=0, cstrieoutPos=0;
    //qDebug()<<"listNode: "<<currentEv.fontSize;
    QFontMetricsF fm(currentEv.getFont(parent));
    QRectF tbr=parent->getTBR(currentEv.getFont(parent), "M", painter.device());
    bool wasBrace=false;
    for (int i=0; i<nodes.size(); i++) {
        bool doDraw=true;
        MTsymbolNode* smb=dynamic_cast<MTsymbolNode*>(nodes[i]);
        // if we find a subscript/superscript node we check whether the next node is super/subscript
        // if so, we typeset them at the same x-psotion, so sub/superscripts appear correctly
        if (dynamic_cast<MTsuperscriptNode*>(nodes[i])) {
            double ccwidth=0, ccbaselineHeight=0, ccoverallHeight=0, ccstrieoutPos=0;
            nodes[i]->getSize(painter, currentEv, ccwidth, ccbaselineHeight, ccoverallHeight, ccstrieoutPos);

            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<MTsubscriptNode*>(nodes[i+1])) { // is this subscript?

                    double shift=-parent->get_super_shift_factor()*tbr.height();
                    if (wasBrace) {
                        shift=-cbaselineHeight+parent->get_super_shift_factor()*tbr.height();
                    }
                    //painter.setPen(QPen("red"));
                    //painter.drawEllipse(xnew-4,ynew+shift-(ccoverallHeight-ccbaselineHeight)-4,8,8);
                    double xnew1=nodes[i]->draw(painter, xnew, ynew+shift-(ccoverallHeight-ccbaselineHeight), currentEv);
                    i++;
                    //painter.setPen(QPen("magenta"));
                    //painter.drawEllipse(xnew-4,ynew-4,8,8);
                    double xnew2=nodes[i]->draw(painter, xnew, ynew, currentEv);
                    //i++;
                    xnew=qMax(xnew1, xnew2);
                    doDraw=false;
                }
            }
        } else if (dynamic_cast<MTsubscriptNode*>(nodes[i])) {
            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<MTsuperscriptNode*>(nodes[i+1])) { // is this subscript?
                    //painter.setPen(QPen("magenta"));
                    //painter.drawEllipse(xnew-4,ynew-4,8,8);
                    double xnew1=nodes[i]->draw(painter, xnew, ynew, currentEv);
                    i++;
                    double ccwidth=0, ccbaselineHeight=0, ccoverallHeight=0, ccstrieoutPos=0;
                    nodes[i]->getSize(painter, currentEv, ccwidth, ccbaselineHeight, ccoverallHeight, ccstrieoutPos);
                    //QRectF tbr=fm.tightBoundingRect("M");
                    double shift=-parent->get_super_shift_factor()*tbr.height();
                    if (wasBrace) {
                        shift=-cbaselineHeight+parent->get_super_shift_factor()*tbr.height();
                    }
                    //painter.setPen(QPen("red"));
                    //painter.drawEllipse(xnew-4,ynew+shift-(ccoverallHeight-ccbaselineHeight)-4,8,8);
                    double xnew2=nodes[i]->draw(painter, xnew, ynew+shift-(ccoverallHeight-ccbaselineHeight), currentEv);
                    //i++;
                    xnew=qMax(xnew1, xnew2);
                    doDraw=false;
                }
            }
        } else {
            if (smb) {
                QString s=smb->get_symbolName();
                if (subsupOperations.contains(s)) {
                    MTsubscriptNode* subn=nullptr;
                    if (i+1<nodes.size()) subn=dynamic_cast<MTsubscriptNode*>(nodes[i+1]);
                    MTsuperscriptNode* supn=nullptr;
                    if (i+2<nodes.size()) supn=dynamic_cast<MTsuperscriptNode*>(nodes[i+2]);
                    //std::cout<<"symbol ='"<<s.toStdString()<<"'   subn="<<subn<<"   supn="<<supn<<"\n";
                    if (subn && supn) { // is this subscript and superscript?
                        MTenvironment ev=currentEv;
                        ev.fontSize=ev.fontSize*parent->get_operatorsubsuper_size_factor();
                        double w1=0, w2=0, w3=0;
                        double oh1=0, oh2=0, oh3=0;
                        double bh1=0, bh2=0, bh3=0, sp;
                        nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp);
                        subn->get_child()->getSize(painter, ev, w2, bh2, oh2, sp);
                        supn->get_child()->getSize(painter, ev, w3, bh3, oh3, sp);
                        double d1=oh1-bh1;
                        //double d2=oh2-bh2;
                        double d3=oh3-bh3;

                        double w=qMax(qMax(w1, w2), w3);
                        //double xnew1=
                        double xn1=nodes[i]->draw(painter, xnew+(w-w1)/2.0, ynew, currentEv);
                        i++;
                        //double xnew2=
                        double xn2=subn->get_child()->draw(painter, xnew+(w-w2)/2.0, ynew+bh2+d1, ev);
                        i++;
                        //double xnew3=
                        double xn3=supn->get_child()->draw(painter, xnew+(w-w3)/2.0, ynew-bh1-d3-fm.xHeight()/4.0, ev);
                        doDraw=false;
                        xnew=qMax(qMax(xn1, xn2), xn3)+fm.width(" ");
                    } else if (subn) { // is this subscript and not superscript?
                        MTenvironment ev=currentEv;
                        ev.fontSize=ev.fontSize*parent->get_operatorsubsuper_size_factor();
                        double w1=0, w2=0;
                        double oh1=0, oh2=0;
                        double bh1=0, bh2=0, sp=0;
                        nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp);
                        subn->get_child()->getSize(painter, ev, w2, bh2, oh2, sp);
                        double d1=oh1-bh1;
                        //double d2=oh2-bh2;

                        double w=qMax(w1, w2);
                        //double xnew1=
                        double xn2=nodes[i]->draw(painter, xnew+(w-w1)/2.0, ynew, currentEv);
                        i++;
                        //double xnew2=
                        double xn1=subn->get_child()->draw(painter, xnew+(w-w2)/2.0, ynew+bh2+d1, ev)+fm.width(" ");
                        doDraw=false;
                        //xnew+=w;
                        xnew=qMax(xn1, xn2);
                    } else if (supn) { // is this subscript and superscript?
                        MTenvironment ev=currentEv;
                        ev.fontSize=ev.fontSize*parent->get_operatorsubsuper_size_factor();
                        double w1=0,  w3=0;
                        double oh1=0,  oh3=0;
                        double bh1=0,  bh3=0, sp;
                        nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp);
                        supn->get_child()->getSize(painter, ev, w3, bh3, oh3, sp);
                        //double d1=oh1-bh1;
                        //double d2=oh2-bh2;
                        double d3=oh3-bh3;

                        double w=qMax(w1, w3);
                        //double xnew1=
                        double xn1=nodes[i]->draw(painter, xnew+(w-w1)/2.0, ynew, currentEv);
                        i++;
                        //double xnew3=
                        double xn3=supn->get_child()->draw(painter, xnew+(w-w3)/2.0, ynew-bh1-d3-fm.xHeight()/4.0, ev);
                        doDraw=false;
                        xnew=qMax(xn1, xn3)+fm.width(" ");
                    }
                }
            }

            nodes[i]->getSize(painter, currentEv, cwidth, cbaselineHeight, coverallHeight, cstrieoutPos);
        }

        if (i<nodes.size() && doDraw) {
            if (dynamic_cast<MTsuperscriptNode*>(nodes[i])) { // is this superscript?
                double ccwidth=0, ccbaselineHeight=0, ccoverallHeight=0, ccstrieoutPos=0;
                nodes[i]->getSize(painter, currentEv, ccwidth, ccbaselineHeight, ccoverallHeight, ccstrieoutPos);
                //QRectF tbr=fm.tightBoundingRect("M");
                double shift=-parent->get_super_shift_factor()*tbr.height();
                if (wasBrace) {
                    shift=-cbaselineHeight+parent->get_super_shift_factor()*tbr.height();
                }
                //painter.setPen(QPen("red"));
                //painter.drawEllipse(xnew-4,ynew+shift-(ccoverallHeight-ccbaselineHeight)-4,8,8);
                xnew=nodes[i]->draw(painter, xnew, ynew+shift-(ccoverallHeight-ccbaselineHeight), currentEv);
            } else {
                xnew=nodes[i]->draw(painter, xnew, ynew, currentEv);
            }
        }
        wasBrace=dynamic_cast<MTbraceNode*>(nodes[i]);
    }
    return xnew;
}

bool JKQTmathText::MTlistNode::toHtml(QString &html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv) {
    bool ok=true;
    for (int i=0; i<nodes.size(); i++) {
        QString h="";
        ok = ok && nodes[i]->toHtml(h, currentEv, defaultEv);
        html=html+h;
    }
    return ok;
}

void JKQTmathText::MTlistNode::set_drawBoxes(bool draw)
{
    this->drawBoxes=draw;
    for (int i=0; i<nodes.size(); i++) {
        nodes[i]->set_drawBoxes(draw);
    }
}





JKQTmathText::MTsymbolNode::MTsymbolNode(JKQTmathText* parent, QString name, bool addWhitespace):
    JKQTmathText::MTnode(parent)
{
    double mathFontFactor=1.8;
    symbol = "";
    symbolName=name;
    fontFactor=1.0;
    bold=0;
    italic=-1;
    yfactor=0;
    QString n=name;
    drawBar=false;
    font=MTSFdefault;
    heightIsAscent=false;
    exactAscent=false;
    extendWidthInMathmode=false;

    
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
                    <<"subset"<<"subseteq"<<"in"<<"notin"<<"cdot"<<"wedge"<<"vee"<<"cong"<<"bot";
    }
    
    if (extendWInMM.contains(n)) {
        extendWidthInMathmode=true;
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
      simpleTranslations.insert("min", "min");
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
      simpleTranslations_heightIsAscent.insert("min", "min");
      simpleTranslations_heightIsAscent.insert("max", "max");
      simpleTranslations_heightIsAscent.insert("inf", "inf");
      simpleTranslations_heightIsAscent.insert("sup", "sup");
      simpleTranslations_heightIsAscent.insert("liminf", "liminf");
      simpleTranslations_heightIsAscent.insert("limsup", "limsup");
      simpleTranslations_heightIsAscent.insert("lim", "lim");
    }

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
      winSymbolSymbol.insert("trademark", QChar(0xD4));
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
      winSymbolSymbol.insert("bot", QChar(0x5E));
      winSymbolSymbol.insert("cdots", QString(3, QChar(0xD7)));
      winSymbolSymbol.insert("dots", QChar(0xDC));
      winSymbolSymbol.insert("ldots", QChar(0xDC));
      winSymbolSymbol.insert("cent", "c");
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
      unicodeGreek.insert("Sigma", QChar(0x3A3));
      unicodeGreek.insert("Upsilon", QChar(0x3A5));
      unicodeGreek.insert("Phi", QChar(0x3A6));
      unicodeGreek.insert("Psi", QChar(0x3A8));
    }
    
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
      unicodeSymbol.insert("pm", QChar(0x00B1));
      unicodeSymbol.insert("mp", QChar(0x2213));
      unicodeSymbol.insert("leq", QChar(0x2264));
      unicodeSymbol.insert("geq", QChar(0x2265));
      unicodeSymbol.insert("ll", QChar(0x226A));
      unicodeSymbol.insert("gg", QChar(0x226B));
      unicodeSymbol.insert("hbar", QChar(0x210F));
      unicodeSymbol.insert("euro", QChar(0x20AC));
      unicodeSymbol.insert("Angstrom", QChar(0x212B));
      unicodeSymbol.insert("Alef", QChar(0x2135));
      unicodeSymbol.insert("Bet", QChar(0x2136));
      unicodeSymbol.insert("Gimel", QChar(0x2137));
      unicodeSymbol.insert("Dalet", QChar(0x2138));
      unicodeSymbol.insert("alef", QChar(0x2135));
      unicodeSymbol.insert("bet", QChar(0x2136));
      unicodeSymbol.insert("gimel", QChar(0x2137));
      unicodeSymbol.insert("dalet", QChar(0x2138));
      unicodeSymbol.insert("nexists", QChar(0x2204));
      unicodeSymbol.insert("ni", QChar(0x220B));
      unicodeSymbol.insert("notni", QChar(0x220C));
      unicodeSymbol.insert("circ", QChar(0x2218));
      unicodeSymbol.insert("tilde", QChar(0x223C));
      unicodeSymbol.insert("emptyset", QChar(0x2300));
      unicodeSymbol.insert("varnothing", QChar(0x2300));
      unicodeSymbol.insert("odot", QChar(0x2299));
      unicodeSymbol.insert("ominus", QChar(0x2296));
      unicodeSymbol.insert("lceil", QChar(0x2308));
      unicodeSymbol.insert("rceil", QChar(0x2309));
      unicodeSymbol.insert("lfloor", QChar(0x230A));
      unicodeSymbol.insert("rfloor", QChar(0x230B));
      unicodeSymbol.insert("subsetnot", QChar(0x2284));
      unicodeSymbol.insert("DC", QChar(0x2393));
      unicodeSymbol.insert("bot", QChar(0x22A4));
      unicodeSymbol.insert("cdots", QString(QChar(0x00B7)) + QString(QChar(0x00B7)) + QString(QChar(0x00B7)));
      unicodeSymbol.insert("vdots", QChar(0x22EE));
      unicodeSymbol.insert("iddots", QChar(0x22F0));
      unicodeSymbol.insert("ddots", QChar(0x22F1));
      unicodeSymbol.insert("dots", "...");
      unicodeSymbol.insert("ldots", "...");
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
      unicodeSymbol.insert("cent", QChar(0x00A2));
      unicodeSymbol.insert("pound", QChar(0x00A3));
      unicodeSymbol.insert("yen", QChar(0x00A5));
      unicodeSymbol.insert("div", QChar(0x00F7));
      unicodeSymbol.insert("multimap", QChar(0x22B8));
      unicodeSymbol.insert("maporiginal", QChar(0x22B6));
      unicodeSymbol.insert("mapimage", QChar(0x22B7));
      unicodeSymbol.insert("benzene", QChar(0x232C));
      unicodeSymbol.insert("times", QChar(0x2A2F));
      unicodeSymbol.insert("propto", QChar(0x221D));
      unicodeSymbol.insert("partial", QChar(0x2202));
      unicodeSymbol.insert("bullet", QChar(0x2219));
      unicodeSymbol.insert("neq", QChar(0x2260));
      unicodeSymbol.insert("ne", QChar(0x2260));
      unicodeSymbol.insert("equiv", QChar(0x2261));
      unicodeSymbol.insert("approx", QChar(0x2245));
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
      unicodeSymbol.insert("angle", QChar(0x2221));
      unicodeSymbol.insert("nabla", QChar(0x2207));
      unicodeSymbol.insert("copyright", QChar(0x00A9));
      unicodeSymbol.insert("registered", QChar(0x00AE));
      unicodeSymbol.insert("trademark", QChar(0x2122));
      unicodeSymbol.insert("cdot", QChar(0x00B7));
      unicodeSymbol.insert("neg", QChar(0x00AC));
      unicodeSymbol.insert("wedge", QChar(0x2227));
      unicodeSymbol.insert("vee", QChar(0x2228));
      unicodeSymbol.insert("diamond", QChar(0xE0));
      unicodeSymbol.insert("langle", QChar(0x2329));
      unicodeSymbol.insert("rangle", QChar(0x232A));
      unicodeSymbol.insert("infty", QChar(0x221E));
      unicodeSymbol.insert("forall", QChar(0x2200));
      unicodeSymbol.insert("exists", QChar(0x2203));
      unicodeSymbol.insert("cong", QChar(0x2245));
      unicodeSymbol.insert("bot", QChar(0x22A5));

    }
    
    static QHash<QString, QString> latexSimpleSymbol;
    if (latexSimpleSymbol.isEmpty()) {
      latexSimpleSymbol.insert("_", "_");
      latexSimpleSymbol.insert("}", "}");
      latexSimpleSymbol.insert("{", "{");
      latexSimpleSymbol.insert("$", "$");
      latexSimpleSymbol.insert("%", "%");
      latexSimpleSymbol.insert("&", "&");
      latexSimpleSymbol.insert("#", "#");
      latexSimpleSymbol.insert("ast", "*");
      latexSimpleSymbol.insert("glq", QChar(0x27));
      latexSimpleSymbol.insert("grq", QChar(0x60));
      latexSimpleSymbol.insert("glqq", QChar(0x5C));
      latexSimpleSymbol.insert("grqq", "\"");
    }
    
    static QHash<QString, QString> latexGreek;
    if (latexGreek.isEmpty()) {
      latexGreek.insert("alpha", QChar(0xAE));
      latexGreek.insert("beta", QChar(0xAF));
      latexGreek.insert("gamma", QChar(0xB0));
      latexGreek.insert("delta", QChar(0xB1));
      latexGreek.insert("epsilon", QChar(0x22));
      latexGreek.insert("varepsilon", QChar(0xB2));
      latexGreek.insert("zeta", QChar(0xB3));
      latexGreek.insert("eta", QChar(0xB4));
      latexGreek.insert("theta", QChar(0xB5));
      latexGreek.insert("vartheta", QChar(0x23));
      latexGreek.insert("iota", QChar(0xB6));
      latexGreek.insert("kappa", QChar(0xB7));
      latexGreek.insert("lambda", QChar(0xB8));
      latexGreek.insert("mu", QChar(0xB9));
      latexGreek.insert("nu", QChar(0xBA));
      latexGreek.insert("xi", QChar(0xBB));
      latexGreek.insert("pi", QChar(0xBC));
      latexGreek.insert("varpi", QChar(0x24));
      latexGreek.insert("rho", QChar(0xBD));
      latexGreek.insert("varrho", QChar(0x25));
      latexGreek.insert("sigma", QChar(0xBE));
      latexGreek.insert("varsigma", QChar(0x26));
      latexGreek.insert("tau", QChar(0xBF));
      latexGreek.insert("upsilon", QChar(0xC0));
      latexGreek.insert("phi", QChar(0xC1));
      latexGreek.insert("varphi", QChar(0x27));
      latexGreek.insert("chi", QChar(0xC2));
      latexGreek.insert("psi", QChar(0xC3));
      latexGreek.insert("omega", QChar(0x21));
      latexGreek.insert("Gamma", QChar(0xA1));
      latexGreek.insert("Delta", QChar(0xA2));
      latexGreek.insert("Theta", QChar(0xA3));
      latexGreek.insert("Lambda", QChar(0xA4));
      latexGreek.insert("Xi", QChar(0xA5));
      latexGreek.insert("Pi", QChar(0xA6));
      latexGreek.insert("Sigma", QChar(0xA7));
      latexGreek.insert("Upsilon", QChar(0xA8));
      latexGreek.insert("Phi", QChar(0xA9));
      latexGreek.insert("Psi", QChar(0xAA));
      latexGreek.insert("leftharpoonup", QChar(0x28));
      latexGreek.insert("rightharpoonup", QChar(0x2A));
      latexGreek.insert("leftharpoondown", QChar(0x29));
      latexGreek.insert("rightharpoondown", QChar(0x2B));
      latexGreek.insert("neg", QChar(0xAC));
      latexGreek.insert("star", QChar(0x3F));
    }
    
    static QHash<QString, QString> latexSymbol;
    if (latexSymbol.isEmpty()) {
      latexSymbol.insert("leftrightarrow", QChar(0x24));
      latexSymbol.insert("leftarrow", QChar(0xc3));
      latexSymbol.insert("rightarrow", QChar(0x21));
      latexSymbol.insert("to", QChar(0x21));
      latexSymbol.insert("uparrow", QChar(0x22));
      latexSymbol.insert("downarrow", QChar(0x23));
      latexSymbol.insert("updownarrow", QChar(0x6c));
      latexSymbol.insert("Leftrightarrow", QChar(0x2c));
      latexSymbol.insert("iff", QChar(0x2c));
      latexSymbol.insert("Leftarrow", QChar(0x28));
      latexSymbol.insert("Rightarrow", QChar(0x29));
      latexSymbol.insert("Uparrow", QChar(0x2a));
      latexSymbol.insert("Downarrow", QChar(0x2b));
      latexSymbol.insert("Updownarrow", QChar(0x6d));
      latexSymbol.insert("pm", QChar(0xa7));
      latexSymbol.insert("mp", QChar(0xa8));
      latexSymbol.insert("leq", QChar(0xb7));
      latexSymbol.insert("geq", QChar(0xb8));
      latexSymbol.insert("ll", QChar(0xbf));
      latexSymbol.insert("gg", QChar(0xc0));
      latexSymbol.insert("Alef", QChar(0x40));
      latexSymbol.insert("alef", QChar(0x40));
      latexSymbol.insert("ni", QChar(0x33));
      latexSymbol.insert("circ", QChar(0xb1));
      latexSymbol.insert("tilde", QChar(0xbb));
      latexSymbol.insert("emptyset", QChar(0x3b));
      latexSymbol.insert("odot", QChar(0xaf));
      latexSymbol.insert("ominus", QChar(0xaa));
      latexSymbol.insert("odiv", QChar(0xae));
      latexSymbol.insert("oplus", QChar(0xa9));
      latexSymbol.insert("lceil", QChar(0x64));
      latexSymbol.insert("rceil", QChar(0x65));
      latexSymbol.insert("lfloor", QChar(0x62));
      latexSymbol.insert("rfloor", QChar(0x63));
      latexSymbol.insert("bot", QChar(0x3f));
      latexSymbol.insert("cdots", QString(3, QChar(0xA2)));
      latexSymbol.insert("dots", "...");
      latexSymbol.insert("nwarrow", QChar(0x2d));
      latexSymbol.insert("nearrow", QChar(0x25));
      latexSymbol.insert("searrow", QChar(0x26));
      latexSymbol.insert("swarrow", QChar(0x2e));
      latexSymbol.insert("div", QChar(0xa5));
      latexSymbol.insert("times", QChar(0xa3));
      latexSymbol.insert("propto", QChar(0x2f));
      latexSymbol.insert("bullet", QChar(0x2b));
      latexSymbol.insert("equiv", QChar(0xb4));
      latexSymbol.insert("approx", QChar(0xbc));
      latexSymbol.insert("ellipsis", "...");
      latexSymbol.insert("Im", QChar(0x3D));
      latexSymbol.insert("Re", QChar(0x3C));
      latexSymbol.insert("oplus", QChar(0xa9));
      latexSymbol.insert("oslash", QChar(0xae));
      latexSymbol.insert("cap", QChar(0x5c));
      latexSymbol.insert("cup", QChar(0x5b));
      latexSymbol.insert("land", QChar(0x5c));
      latexSymbol.insert("lor", QChar(0x5b));
      latexSymbol.insert("supset", QChar(0xbe));
      latexSymbol.insert("supseteq", QChar(0xb6));
      latexSymbol.insert("subset", QChar(0xbd));
      latexSymbol.insert("subseteq", QChar(0xb5));
      latexSymbol.insert("in", QChar(0x32));
      latexSymbol.insert("nabla", QChar(0x35));
      latexSymbol.insert("cdot", QChar(0xa2));
      latexSymbol.insert("wedge", QChar(0x5e));
      latexSymbol.insert("vee", QChar(0x5f));
      latexSymbol.insert("diamond", QChar(0xE0));
      latexSymbol.insert("langle", QChar(0x68));
      latexSymbol.insert("rangle", QChar(0x69));
      latexSymbol.insert("infty", QChar(0x31));
      latexSymbol.insert("forall", QChar(0x38));
      latexSymbol.insert("exists", QChar(0x39));
      latexSymbol.insert("cong", QChar(0xbb));
      latexSymbol.insert("bot", QChar(0x3f));

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QHash<QString, QString>::iterator itsimple = simpleTranslations.find(n);
    if (itsimple!= simpleTranslations.end()) { 
      symbol=itsimple.value(); 
    } else {
      QHash<QString, QString>::iterator itsimplehia = simpleTranslations_heightIsAscent.find(n);
      if (itsimplehia != simpleTranslations_heightIsAscent.end()) {
        symbol = itsimplehia.value();
        heightIsAscent = true; 
      } else {
        if (parent->get_fontEncoding() == MTFEwinSymbol) {
          // first we start with greek characters
          font = MTSFgreek;
          italic = -1;
          QHash<QString, QString>::iterator itgreek = winSymbolGreek.find(n);
          if (itgreek!=winSymbolGreek.end()) {
              symbol = itgreek.value();
          } else if (n == "sum") { symbol = "S"; fontFactor = mathFontFactor; heightIsAscent = true; exactAscent = true; }
          else if (n == "prod") { symbol = "P"; fontFactor = mathFontFactor; heightIsAscent = true; exactAscent = true; }
          else {
            // now we set the symbols from the Symbol font
            font = MTSFsymbol;
            QHash<QString, QString>::iterator itsymbol = winSymbolSymbol.find(n);
            if (itsymbol!=winSymbolSymbol.end()) {
              symbol = itsymbol.value();
            } else if (n == "int") { symbol = QChar(0xF2); fontFactor = mathFontFactor; yfactor = +0.1; }
            else if (n == "bbC") { symbol = "C"; bold = +1; italic = -1; }
            else if (n == "bbH") { symbol = "H"; bold = +1; italic = -1; }
            else if (n == "bbN") { symbol = "N"; bold = +1; italic = -1; }
            else if (n == "bbP") { symbol = "P"; bold = +1; italic = -1; }
            else if (n == "bbQ") { symbol = "Q"; bold = +1; italic = -1; }
            else if (n == "bbR") { symbol = "R"; bold = +1; italic = -1; }
            else if (n == "bbZ") { symbol = "Z"; bold = +1; italic = -1; }
            else if (n == "iint") { symbol = QString(2, QChar(0xF2)); fontFactor = mathFontFactor; yfactor = +0.1; }
            else if (n == "iiint") { symbol = QString(3, QChar(0xF2)); fontFactor = mathFontFactor; yfactor = +0.1; }
            else if (n == "bigcap") { symbol = QChar(0xC7); fontFactor = 2; }
            else if (n == "bigcup") { symbol = QChar(0xC8); fontFactor = 2; }
            else if (n == "bigvee") { symbol = QChar(0xDA); fontFactor = 2; }
            else if (n == "bighat") { symbol = QChar(0xD9); fontFactor = 2; }

            else { // here are text mode symbols, i.e. bold and italic won't be touched
              bold = -1;
              italic = -1;
              font = MTSFdefault;
              if (n == "_") { symbol = "_"; bold = 0; italic = 0; }
              else if (n == "}") { symbol = "}"; }
              else if (n == "{") { symbol = "{"; }
              else if (n == "hbar") { symbol = "h"; bold = 0; italic = 0; drawBar = true; }
              else if (n == "euro") { symbol = ""; bold = 0; italic = 0; }
              else if (n == "cent") { symbol = QChar(0xA2); bold = 0; italic = 0; }
              else if (n == "pound") { symbol = QChar(0xA3); bold = 0; italic = 0; }
              else if (n == "yen") { symbol = QChar(0xA5); bold = 0; italic = 0; }
              else if (n == "div") { symbol = QChar(0xF7); bold = 0; italic = 0; }
              else if (n == "backslash") { symbol = "\\"; bold = 0; italic = 0; }
              //else if (n=="|") { symbol="||"; bold=0; italic=0; }
              else if (n == "$") { symbol = "$"; bold = 0; italic = 0; }
              else if (n == "%") { symbol = "%"; bold = 0; italic = 0; }
              else if (n == "&") { symbol = "&"; bold = 0; italic = 0; }
              else if (n == "#") { symbol = "#"; bold = 0; italic = 0; }
              else if (n == "ast") { symbol = "*"; bold = 0; italic = 0; }
              else if (n == "glq") { symbol = "'"; bold = 0; italic = 0; }
              else if (n == "grq") { symbol = "'"; bold = 0; italic = 0; }
              else if (n == "glqq") { symbol = "\""; bold = 0; italic = 0; }
              else if (n == "grqq") { symbol = "\""; bold = 0; italic = 0; }
              else if (n == "flq") { symbol = "<"; bold = 0; italic = 0; }
              else if (n == "frq") { symbol = ">"; bold = 0; italic = 0; }
              else if (n == "flqq") { symbol = ""; bold = 0; italic = 0; }
              else if (n == "frqq") { symbol = ""; bold = 0; italic = 0; }
            }
            //else if (n=="") { symbol=QChar(); font=MTSFdefault; }
            //else if (n=="") symbol=QChar(0x);
          }
        }
        else if (parent->get_fontEncoding() == MTFEunicode) { // use UNICODE encoding for special characters
         // first we start with greek characters
          font = MTSFdefault; //MTSFgreek;
          //std::cout<<"encoding unicode\n";
          QHash<QString, QString>::iterator itgreek = unicodeGreek.find(n);
          if (itgreek!=unicodeGreek.end()) {
              symbol = itgreek.value();
          } else {
            // now we set the symbols from the Symbol font
            font = MTSFsymbol;
            QHash<QString, QString>::iterator itsymbol = unicodeSymbol.find(n);
            if (itsymbol!=unicodeSymbol.end()) {
              symbol = itsymbol.value();
            } if (n == "sum") { symbol = QChar(0x2211); heightIsAscent = true; exactAscent = true; }
            else if (n == "prod") { symbol = QChar(0x220F); heightIsAscent = true; exactAscent = true; }
            else if (n == "bbC") { symbol = QChar(0x2102); italic = -1; }
            else if (n == "bbH") { symbol = QChar(0x210D); italic = -1; }
            else if (n == "bbN") { symbol = QChar(0x2115); italic = -1; }
            else if (n == "bbP") { symbol = QChar(0x2119); italic = -1; }
            else if (n == "bbQ") { symbol = QChar(0x211A); italic = -1; }
            else if (n == "bbR") { symbol = QChar(0x211D); italic = -1; }
            else if (n == "bbZ") { symbol = QChar(0x2124); italic = -1; }
            else if (n == "iint") { symbol = QChar(0x222C); fontFactor = mathFontFactor; /*yfactor=+0.1;;*/ heightIsAscent = true; exactAscent = true; }
            else if (n == "iiint") { symbol = QChar(0x222D); fontFactor = mathFontFactor; /*yfactor=+0.1;;*/ heightIsAscent = true; exactAscent = true; }
            else if (n == "oint") { symbol = QChar(0x222E); fontFactor = mathFontFactor; /*yfactor=+0.1;;*/ heightIsAscent = true; exactAscent = true; }
            else if (n == "oiint") { symbol = QChar(0x222F); fontFactor = mathFontFactor; /*yfactor=+0.1;;*/ heightIsAscent = true; exactAscent = true; }
            else if (n == "oiiint") { symbol = QChar(0x2230); fontFactor = mathFontFactor; /*yfactor=+0.1;;*/ heightIsAscent = true; exactAscent = true; }
            else if (n == "coprod") { symbol = QChar(0x2210); heightIsAscent = true; exactAscent = true; }
            else if (n == "bigcap") { symbol = QChar(0x22C2); heightIsAscent = true; exactAscent = true;; heightIsAscent = true; exactAscent = true; }
            else if (n == "bigcup") { symbol = QChar(0x22C3); heightIsAscent = true; exactAscent = true;; heightIsAscent = true; exactAscent = true; }
            else if (n == "bigvee") { symbol = QChar(0x22C1); heightIsAscent = true; exactAscent = true;; heightIsAscent = true; exactAscent = true; }
            else if (n == "bighat") { symbol = QChar(0x22C0); heightIsAscent = true; exactAscent = true;; heightIsAscent = true; exactAscent = true; }
            else if (n == "int") { symbol = QChar(0x222B); fontFactor = mathFontFactor; /*yfactor=+0.1;;*/ heightIsAscent = true; exactAscent = true; }

            else { // here are text mode symbols, i.e. bold and italic won't be touched
              bold = -1;
              italic = -1;
              font = MTSFdefault;
              if (n == "_") { symbol = "_"; }
              else if (n == "}") { symbol = "}"; }
              else if (n == "{") { symbol = "{"; }
              else if (n == "backslash") { symbol = "\\"; bold = 0; italic = 0; }
              //else if (n=="|") { symbol=QChar(0x2016); }
              else if (n == "$") { symbol = "$"; }
              else if (n == "%") { symbol = "%"; }
              else if (n == "&") { symbol = "&"; }
              else if (n == "#") { symbol = "#"; }
              else if (n == "ast") { symbol = "*"; }
              else if (n == "glq") { symbol = QChar(0x2018); bold = 0; italic = 0; }
              else if (n == "grq") { symbol = QChar(0x2019); bold = 0; italic = 0; }
              else if (n == "glqq") { symbol = QChar(0x201C); bold = 0; italic = 0; }
              else if (n == "grqq") { symbol = QChar(0x201D); bold = 0; italic = 0; }
              else if (n == "flq") { symbol = QChar(0x2039); bold = 0; italic = 0; }
              else if (n == "frq") { symbol = QChar(0x203A); bold = 0; italic = 0; }
              else if (n == "flqq") { symbol = ""; bold = 0; italic = 0; }
              else if (n == "frqq") { symbol = ""; bold = 0; italic = 0; }
            }
            //else if (n=="") { symbol=QChar(); font=MTSFdefault; }
            //else if (n=="") symbol=QChar(0x);
          }
        }
        else if (parent->get_fontEncoding() == MTFElatex) { // use UNICODE encoding for special characters
         // first we start with greek characters
          font = MTSFdefault; //MTSFgreek;
          //std::cout<<"encoding unicode\n";
          QHash<QString, QString>::iterator itssymbol = latexSimpleSymbol.find(n);
          if (itssymbol!=latexSimpleSymbol.end()) {
            symbol = itssymbol.value();
          } else if (n == "backslash") { symbol = "\\"; bold = 0; italic = 0; }

          else {
            font = MTSFgreek;
            QHash<QString, QString>::iterator itgreek = latexGreek.find(n);
            if (itgreek!=latexGreek.end()) {
              symbol = itgreek.value();
            } else {
              // now we set the symbols from the Symbol font
              font = MTSFsymbol;
              QHash<QString, QString>::iterator itsymbol = latexSymbol.find(n);
              if (itsymbol!=latexSymbol.end()) {
                symbol = itsymbol.value();
              } else if (n == "int") { symbol = QChar(0x73); fontFactor = mathFontFactor; yfactor = +0.1; }

              else { // here are text mode symbols, i.e. bold and italic won't be touched
                font = MTSFbraces;
                if (n == "bigcap") { symbol = QChar(0x5c); heightIsAscent = true; exactAscent = true; }
                else if (n == "bigcup") { symbol = QChar(0x5b); heightIsAscent = true; exactAscent = true; }
                else if (n == "bigvee") { symbol = QChar(0x5F); heightIsAscent = true; exactAscent = true; }
                else if (n == "bighat") { symbol = QChar(0x5E); heightIsAscent = true; exactAscent = true; }
                else if (n == "oint") { symbol = QChar(0x49); yfactor = +0.1; }
                else if (n == "coprod") { symbol = QChar(0x61); heightIsAscent = true; exactAscent = true; }
                else if (n == "iint") { symbol = QString(2, QChar(0x5A)); yfactor = +0.1; }
                else if (n == "iiint") { symbol = QString(3, QChar(0x5A)); yfactor = +0.1; }
                else if (n == "sum") { symbol = QChar(0x58); heightIsAscent = true; exactAscent = true; }
                else if (n == "prod") { symbol = QChar(0x59); heightIsAscent = true; exactAscent = true; }



              }
            }
          }
        }
      }
    }
    if (addWhitespace) symbol=symbol+" ";
    //std::cout<<"symbol node '"<<symbolName.toStdString()<<"': symbol='"<<symbol.toStdString()<<"'\n";
}

JKQTmathText::MTsymbolNode::~MTsymbolNode() {
}

QString JKQTmathText::MTsymbolNode::getTypeName() const
{
    return QLatin1String("MTsymbolNode(")+symbolName+QLatin1String(")");
}

QFont JKQTmathText::MTsymbolNode::getFontName(symbolFont f, QFont& fi) {
    QFont fr=fi;
    switch(f) {
        case MTSFgreek: fr.setFamily(parent->get_fontGreek()); break;
        case MTSFsymbol: fr.setFamily(parent->get_fontSymbol()); break;
        case MTSFbraces: fr.setFamily(parent->get_fontBraces()); break;
        case MTSFintegrals: fr.setFamily(parent->get_fontIntegrals()); break;
        case MTSFcaligraphic: fr.setFamily(parent->get_fontCaligraphic()); break;
        case MTSFblackboard: fr.setFamily(parent->get_fontBlackboard()); break;
        default: break;
    }
    return fr;
}

void JKQTmathText::MTsymbolNode::getSizeInternal(QPainter& painter, JKQTmathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos) {
    QFont f=currentEv.getFont(parent);
    f=getFontName(font, f);
    f.setPointSizeF(f.pointSizeF()*fontFactor);
    if (italic<0) f.setItalic(false);
    if (italic>0) f.setItalic(true);
    if (bold<0) f.setBold(false);
    if (bold>0) f.setBold(true);
    QFontMetricsF fm(f, painter.device());
    QString symb=symbol;
    width=0;
    if (currentEv.insideMath) width=qMax(parent->getTBR(f, symb, painter.device()).width(),parent->getTBR(f, "i", painter.device()).width());//fm.width(symbol);
    else width=fm.boundingRect(symb).width();//fm.width(symbol);

    width=qMax(fm.width("j"), width);
    if (symb.isEmpty()) {
        width=fm.width("a");
        if (symbolName=="|") width=fm.width("1")*0.8;
        else if (symbolName=="infty") width=fm.width("M");
        else if (symbolName==" ") width=parent->getTBR(f, "x", painter.device()).width();
        else if (symbolName==";") width=parent->getTBR(f, "x", painter.device()).width()*0.75;
        else if (symbolName==":") width=parent->getTBR(f, "x", painter.device()).width()*0.5;
        else if (symbolName==",") width=parent->getTBR(f, "x", painter.device()).width()*0.25;
        else if (symbolName=="!") width=-parent->getTBR(f, "x", painter.device()).width()*0.25;
        else if (symbolName=="longleftarrow") { width=parent->getTBR(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="longrightarrow") {  width=parent->getTBR(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="Longleftarrow") { width=parent->getTBR(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="Longrightarrow") { width=parent->getTBR(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="longleftrightarrow") { width=parent->getTBR(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="Longleftrightarrow") { width=parent->getTBR(f, "X", painter.device()).width()*3.5; symb="x"; }
    }
    QRectF tbr=parent->getTBR(f, symb, painter.device());
    overallHeight=tbr.height();// fm.height();
    baselineHeight=tbr.height()-tbr.bottom();
    if (exactAscent) {
        //baselineHeight=fm.ascent()*0.8;
    }
    if (heightIsAscent) {
        overallHeight=baselineHeight*1.1;
    }
    if (exactAscent && heightIsAscent) {
        //qDebug()<<symbolName<<symbol<<tbr.top()<<tbr.bottom()<<tbr.height();
        baselineHeight=tbr.height()-tbr.bottom();//fm.ascent()*0.8;
        overallHeight=tbr.height();//baselineHeight*1.1;
    }
    strikeoutPos=fm.strikeOutPos();

    if (extendWidthInMathmode && currentEv.insideMath) width=width*parent->get_mathoperator_width_factor();

}

double JKQTmathText::MTsymbolNode::draw(QPainter& painter, double x, double y, JKQTmathText::MTenvironment currentEv) {
    doDrawBoxes(painter, x, y, currentEv);
    double width=0;
    double baselineHeight=0;
    double overallHeight=0, strikeoutPos=0;
    getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);
    QPen pold=painter.pen();
    QFont fold=painter.font();
    QFont f=currentEv.getFont(parent);
    QFont f1=f;
    f=getFontName(font, f);
    f.setPointSizeF(f.pointSizeF()*fontFactor);
    if (italic<0) f.setItalic(false);
    if (italic>0) f.setItalic(true);
    if (bold<0) f.setBold(false);
    if (bold>0) f.setBold(true);
    QFontMetricsF fm(f, painter.device());
    QFontMetricsF fm1(f1, painter.device());
    painter.setFont(f);

    if (extendWidthInMathmode && currentEv.insideMath) {
        x=x+0.5*width*parent->get_mathoperator_width_factor();
    }

    //std::cout<<"symbol '"<<symbolName.toStdString()<<"' = "<<std::hex<<symbol.at(0).digitValue()<<" in font '"<<f.family().toStdString()<<"' ("<<QFontInfo(f).family().toStdString()<<"): "<<fm.inFont(symbol.at(0))<<std::endl;

    QPen p=painter.pen();
    p.setColor(currentEv.color);
    p.setWidthF(qMax(JKQTMATHTEXT_ABS_MIN_LINEWIDTH, fm.lineWidth()));
    p.setStyle(Qt::SolidLine);
    painter.setPen(p);
    double xwi=fm.width("x");
    if (!symbol.isEmpty()) {
        // if the symbol has been recognized in the constructor: draw the symbol
        painter.drawText(QPointF(x, y+yfactor*overallHeight), symbol);
        double xx=x;
        double yy=y-fm.xHeight()-(parent->getTBR(f, "M", painter.device()).height()-fm.xHeight())/3.0;
        QLineF l(xx, yy, xx+xwi/3.0+((currentEv.italic)?(xwi/3.0):0), yy);
        if (drawBar&&l.length()>0) painter.drawLine(l);

    // try to draw some often used special symbols, by synthesizing them from
    // standard characters in the current drawing font
    } else if (symbolName=="infty") {
        //std::cout<<"draw infty\n";
        f1.setItalic(false);
        painter.setFont(f1);
        painter.save();
        painter.translate(x+fm1.width("8")/3.0, y-fm1.xHeight());
        painter.rotate(90);
        painter.drawText(QPointF(0,0), "8");
        painter.restore();
    } else if (symbolName=="|") {
        //std::cout<<"draw infty\n";
        f1.setItalic(false);
        painter.setFont(f1);

        painter.save();
        painter.translate(x, y);
        painter.drawText(QPointF(0,0), "|");
        painter.translate(fm1.width("8")/3.0, 0);
        painter.drawText(QPointF(0,0), "|");
        painter.restore();

    // here are some spaces
    } else if (symbolName==" ") { // full space
    } else if (symbolName==";") { // 75% space
    } else if (symbolName==":") { // 50% space
    } else if (symbolName==",") { // 25% space
    } else if (symbolName=="!") { // -25% space
    } else if (symbolName=="longleftarrow") {
        double width=parent->getTBR(f, "X", painter.device()).width()*3.0;
        double dx=parent->getTBR(f, "X", painter.device()).width()*0.25;
        double ypos=y-parent->getTBR(f, "x", painter.device()).height()/2.0;
        QPainterPath path=makeArrow(x+dx, ypos, width, parent->getTBR(f, "M", painter.device()).height()*0.5, true, false);
        painter.drawPath(path);
    } else if (symbolName=="longrightarrow"){
        double width=parent->getTBR(f, "X", painter.device()).width()*3.0;
        double dx=parent->getTBR(f, "X", painter.device()).width()*0.25;
        double ypos=y-parent->getTBR(f, "x", painter.device()).height()/2.0;
        QPainterPath path=makeArrow(x+dx, ypos, width, parent->getTBR(f, "M", painter.device()).height()*0.5, false, true);
        painter.drawPath(path);
    } else if (symbolName=="Longleftarrow") {
        double width=parent->getTBR(f, "X", painter.device()).width()*3.0;
        double dx=parent->getTBR(f, "X", painter.device()).width()*0.25;
        double ypos=y-parent->getTBR(f, "x", painter.device()).height()/2.0;
        QPainterPath path=makeDArrow(x+dx, ypos, width, parent->getTBR(f, "M", painter.device()).height()*0.5, true, false);
        painter.drawPath(path);
    } else if (symbolName=="Longrightarrow") {
        double width=parent->getTBR(f, "X", painter.device()).width()*3.0;
        double dx=parent->getTBR(f, "X", painter.device()).width()*0.25;
        double ypos=y-parent->getTBR(f, "x", painter.device()).height()/2.0;
        QPainterPath path=makeDArrow(x+dx, ypos, width, parent->getTBR(f, "M", painter.device()).height()*0.5, false, true);
        painter.drawPath(path);
    } else if (symbolName=="longleftrightarrow") {
        double width=parent->getTBR(f, "X", painter.device()).width()*3.0;
        double dx=parent->getTBR(f, "X", painter.device()).width()*0.25;
        double ypos=y-parent->getTBR(f, "x", painter.device()).height()/2.0;
        QPainterPath path=makeArrow(x+dx, ypos, width, parent->getTBR(f, "M", painter.device()).height()*0.5, true, true);
        painter.drawPath(path);
    } else if (symbolName=="Longleftrightarrow") {
        double width=parent->getTBR(f, "X", painter.device()).width()*3.0;
        double dx=parent->getTBR(f, "X", painter.device()).width()*0.25;
        double ypos=y-parent->getTBR(f, "x", painter.device()).height()/2.0;
        QPainterPath path=makeDArrow(x+dx, ypos, width, parent->getTBR(f, "M", painter.device()).height()*0.5, true, true);
        painter.drawPath(path);
    } else { // draw a box to indicate an unavailable symbol
        QRectF tbr=parent->getTBR(f, "M", painter.device());
        painter.drawRect(QRectF(x,y-tbr.height(), xwi, tbr.height()*0.8));
    }
    painter.setPen(pold);
    painter.setFont(fold);
    return x+width;
}

bool JKQTmathText::MTsymbolNode::toHtml(QString &html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv) {
    bool ok=true;
    QString s;
    JKQTmathText::MTenvironment ev=currentEv;

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
      entitylut.insert("min", "min");
      entitylut.insert("max", "max");
      entitylut.insert("argmin", "argmin");
      entitylut.insert("argmax", "argmax");
      entitylut.insert("inf", "inf");
      entitylut.insert("sup", "sup");
      entitylut.insert("liminf", "liminf");
      entitylut.insert("limsup", "limsup");
      entitylut.insert("lim", "lim");
      entitylut.insert("max", "max");
      entitylut.insert("min", "min");
      entitylut.insert("sec", "sec");
      entitylut.insert("gcd", "gcd");
      entitylut.insert("hom", "hom");
      entitylut.insert("ker", "ker");
      entitylut.insert("dim", "dim");
      entitylut.insert("cot", "cot");
      entitylut.insert("arg", "arg");
      entitylut.insert("det", "det");
      entitylut.insert("deg", "deg");
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
      entitylut.insert("ast", "*");
      entitylut.insert("glq", "'");
      entitylut.insert("grq", "'");
      entitylut.insert("glqq", "\"");
      entitylut.insert("grqq", "\"");
      entitylut.insert("flq", "&lt;");
      entitylut.insert("frq", "&gt;");
      entitylut.insert("flqq", "");
      entitylut.insert("frqq", "");
    }


    QMap<QString, QString>::iterator itS = entitylut.find(symbolName);
    if (itS!=entitylut.end()) { s=itS.value(); }
    else if (symbolName == "sum") { ev.fontSize*=1.7; s="&sum;"; }
    else if (symbolName == "prod") { ev.fontSize*=1.7; s="&prod;"; }
    else if (symbolName == "bigcap") { ev.fontSize*=1.7; s="&cap;"; }
    else if (symbolName == "bigcup") { ev.fontSize*=1.7; s="&cup;"; }
    else if (symbolName == "bigvee") { ev.fontSize*=1.7; s="&or;"; }
    else if (symbolName == "bighat") { ev.fontSize*=1.7; s="&and;"; }
    else ok=false;

    if (ok) html=html+ev.toHtmlStart(defaultEv)+s+ev.toHtmlAfter(defaultEv);
    return ok;
}


// --------------------------------------------------------------------------------------------------
// -- implementation of the JKQTmathText methods
// --------------------------------------------------------------------------------------------------
JKQTmathText::JKQTmathText(QObject* parent):
    QObject(parent)
{
    Q_INIT_RESOURCE(xits);
    QFontDatabase fontdb;

    QString serifFont="Serif";
    QString sansFont="Sans";
    QString symbolFont="Symbol";
    QString scriptFont="Script";
    QString typewriterFont="typewriter";
    QString decorativeFont="decorative";

     QStringList fonts=fontdb.families();
    //qDebug()<<"fonts:\n"<<fonts;

    if (fonts.contains("Times New Roman")) {
        serifFont="Times New Roman";
    } else if (fonts.contains("Times")) {
        serifFont="Times";
    } else if (fonts.contains("FreeSerif")) {
        serifFont="FreeSerif";
    }
    if (fonts.contains("Arial")) {
        sansFont="Arial";
    } else if (fonts.contains("Helvetica")) {
        sansFont="Helvetica";
    } else if (fonts.contains("FreeSans")) {
        sansFont="FreeSans";
    }
    if (fonts.contains("Symbol")) {
        symbolFont="Symbol";
    } else if (fonts.contains("Standard Symbols L")) {
        symbolFont="Standard Symbols L";
    }
    if (fonts.contains("Courier")) {
        typewriterFont="Courier";
    } else if (fonts.contains("Courier New")) {
        typewriterFont="Courier New";
    } else if (fonts.contains("Courier Std")) {
        typewriterFont="Courier Std";
    } else if (fonts.contains("FreeMono")) {
        typewriterFont="FreeMono";
    }



    def_fontSize=fontSize=10;
    def_fontRoman=fontRoman=serifFont;
    def_fontSans=fontSans=sansFont;
    def_fontTypewriter=fontTypewriter=typewriterFont;
    def_fontScript=fontScript=scriptFont;
    def_fontGreek=fontGreek=symbolFont;
    def_fontSymbol=fontSymbol=symbolFont;
    def_fontCaligraphic=fontCaligraphic=decorativeFont;
    def_fontBlackboard=fontBlackboard=serifFont;
    def_fontMathRoman=fontMathRoman=serifFont;
    def_fontMathSans=fontMathSans=sansFont;
    def_fontBraces=fontBraces=fontMathRoman;
    def_fontIntegrals=fontIntegrals=fontMathRoman;
    def_fontEncoding=fontEncoding=MTFEwinSymbol;
    def_fontLatexPostfix=fontLatexPostfix="10";
    def_fontLatexPrefix=fontLatexPrefix="jlm_";
    def_brace_factor=brace_factor=1.04;
    def_subsuper_size_factor=subsuper_size_factor=0.7;
    def_italic_correction_factor=italic_correction_factor=0.4;
    def_sub_shift_factor=sub_shift_factor=0.6;
    def_super_shift_factor=super_shift_factor=0.5;
    def_brace_shrink_factor=brace_shrink_factor=0.6;
    def_fontColor=fontColor=QColor("black");
    def_useSTIXfonts=useSTIXfonts=false;
    def_useXITSfonts=useXITSfonts=false;
    def_useASANAfonts=useASANAfonts=false;
    def_frac_factor=frac_factor=0.9;
    def_frac_shift_factor=frac_shift_factor=0.5;
    def_underbrace_factor=underbrace_factor=0.75;
    def_underset_factor=underset_factor=0.7;
    def_decoration_height_factor=decoration_height_factor=0.2;
    def_brace_y_shift_factor=brace_y_shift_factor=0.7;//-1;
    def_operatorsubsuper_size_factor=operatorsubsuper_size_factor=0.65;
    def_mathoperator_width_factor=mathoperator_width_factor=1.5;

    def_expensiveRendering=expensiveRendering=true;
    useUnparsed=false;

    parsedNode=nullptr;
    unparsedNode=nullptr;

    currentToken=MTTnone;
    currentTokenName="";
    currentTokenID=0;
    parseString="";
    parsingMathEnvironment=false;
}

JKQTmathText::~JKQTmathText() {
    if (parsedNode!=nullptr) delete parsedNode;
    parsedNode=nullptr;
    if (unparsedNode!=nullptr) delete unparsedNode;
    unparsedNode=nullptr;
}

void JKQTmathText::loadSettings(QSettings& settings, QString group){
    fontSize=settings.value(group+"font_size", fontSize).toDouble();
    fontColor=QColor(settings.value(group+"font_color", JKQTP_QColor2String(fontColor)).toString());
    fontRoman=settings.value(group+"font_roman", fontRoman).toString();
    fontSans=settings.value(group+"font_sans", fontSans).toString();
    fontTypewriter=settings.value(group+"font_typewriter", fontTypewriter).toString();
    fontScript=settings.value(group+"font_script", fontScript).toString();
    fontGreek=settings.value(group+"font_greek", fontGreek).toString();
    fontSymbol=settings.value(group+"font_symbol", fontSymbol).toString();
    fontMathRoman=settings.value(group+"font_math_roman", fontRoman).toString();
    fontMathSans=settings.value(group+"font_math_sans", fontSans).toString();
    fontBraces=settings.value(group+"font_braces", fontRoman).toString();
    fontIntegrals=settings.value(group+"font_integrals", fontSans).toString();
    fontBlackboard=settings.value(group+"font_blackboard", fontSans).toString();
    fontCaligraphic=settings.value(group+"font_caligraphics", fontSans).toString();
    brace_factor=settings.value(group+"brace_factor", brace_factor).toDouble();
    brace_shrink_factor=settings.value(group+"brace_shrink_factor", brace_shrink_factor).toDouble();
    subsuper_size_factor=settings.value(group+"subsuper_size_factor", subsuper_size_factor).toDouble();
    italic_correction_factor=settings.value(group+"italic_correction_factor", italic_correction_factor).toDouble();
    super_shift_factor=settings.value(group+"super_shift_factor", super_shift_factor).toDouble();
    sub_shift_factor=settings.value(group+"sub_shift_factor", sub_shift_factor).toDouble();
    frac_factor=settings.value(group+"frac_factor", frac_factor).toDouble();
    frac_shift_factor=settings.value(group+"frac_shift_factor", frac_shift_factor).toDouble();
    underbrace_factor=settings.value(group+"underbrace_factor", underbrace_factor).toDouble();
    underset_factor=settings.value(group+"underset_factor", underset_factor).toDouble();
    brace_y_shift_factor=settings.value(group+"brace_y_shift_factor", brace_y_shift_factor).toDouble();
    decoration_height_factor=settings.value(group+"decoration_height_factor", decoration_height_factor).toDouble();
    operatorsubsuper_size_factor=settings.value(group+"operatorsubsuper_size_factor", operatorsubsuper_size_factor).toDouble();
    mathoperator_width_factor=settings.value(group+"mathoperator_width_factor", mathoperator_width_factor).toDouble();

    QString v=settings.value(group+"font_encoding", "win_symbol").toString().toLower();
    fontEncoding=MTFEwinSymbol;
    if (v=="unicode") fontEncoding=MTFEunicode;
    useSTIXfonts=settings.value(group+"use_stix_fonts", false).toBool();
    useXITSfonts=settings.value(group+"use_xits_fonts", false).toBool();
    useASANAfonts=settings.value(group+"use_asana_fonts", false).toBool();

    if (useSTIXfonts) useSTIX();
    if (useXITSfonts) useXITS();
    if (useASANAfonts) useASANA();
}


void JKQTmathText::saveSettings(QSettings& settings, QString group){
    JKQTMTPROPERTYsave(settings, group, fontSize,"font_size");
    if (fontColor!=def_fontColor) settings.setValue(group+"font_color", JKQTP_QColor2String(fontColor));
    JKQTMTPROPERTYsave(settings, group, fontRoman, "font_roman");
    JKQTMTPROPERTYsave(settings, group, fontSans, "font_sans");
    JKQTMTPROPERTYsave(settings, group, fontTypewriter, "font_typewriter");
    JKQTMTPROPERTYsave(settings, group, fontScript, "font_script");
    JKQTMTPROPERTYsave(settings, group, fontGreek, "font_greek");
    JKQTMTPROPERTYsave(settings, group, fontSymbol, "font_symbol");
    JKQTMTPROPERTYsave(settings, group, fontMathRoman, "font_math_roman");
    JKQTMTPROPERTYsave(settings, group, fontMathSans, "font_math_sans");
    JKQTMTPROPERTYsave(settings, group, fontBraces, "font_braces");
    JKQTMTPROPERTYsave(settings, group, fontIntegrals, "font_integrals");
    JKQTMTPROPERTYsave(settings, group, fontBlackboard, "font_blackboard");
    JKQTMTPROPERTYsave(settings, group, fontCaligraphic, "font_caligraphic");
    JKQTMTPROPERTYsave(settings, group, brace_factor, "brace_factor");
    JKQTMTPROPERTYsave(settings, group, brace_shrink_factor, "brace_shrink_factor");
    JKQTMTPROPERTYsave(settings, group, subsuper_size_factor, "subsuper_size_factor");
    JKQTMTPROPERTYsave(settings, group, italic_correction_factor, "italic_correction_factor");
    JKQTMTPROPERTYsave(settings, group, sub_shift_factor, "sub_shift_factor");
    JKQTMTPROPERTYsave(settings, group, super_shift_factor, "super_shift_factor");
    JKQTMTPROPERTYsave(settings, group, useSTIXfonts, "use_stix_fonts");
    JKQTMTPROPERTYsave(settings, group, useXITSfonts, "use_xits_fonts");
    JKQTMTPROPERTYsave(settings, group, useASANAfonts, "use_asana_fonts");
    JKQTMTPROPERTYsave(settings, group, frac_factor, "frac_factor");
    JKQTMTPROPERTYsave(settings, group, frac_shift_factor, "frac_shift_factor");
    JKQTMTPROPERTYsave(settings, group, underbrace_factor, "underbrace_factor");
    JKQTMTPROPERTYsave(settings, group, underset_factor, "underset_factor");
    JKQTMTPROPERTYsave(settings, group, operatorsubsuper_size_factor, "operatorsubsuper_size_factor");
    JKQTMTPROPERTYsave(settings, group, mathoperator_width_factor, "mathoperator_width_factor");
    JKQTMTPROPERTYsave(settings, group, brace_y_shift_factor, "brace_y_shift_factor");
    JKQTMTPROPERTYsave(settings, group, decoration_height_factor, "decoration_height_factor");
    if (fontEncoding!=def_fontEncoding) {
        if (fontEncoding==MTFEunicode) settings.setValue(group+"font_encoding", "unicode");
        else if (fontEncoding==MTFEwinSymbol) settings.setValue(group+"font_encoding", "win_symbol");
    }
}

void JKQTmathText::useSTIX() {
    useSTIXfonts=true;
    fontGreek="STIXGeneral";
    fontSymbol="STIXGeneral";
    fontBraces="STIXGeneral";
    fontIntegrals="STIXGeneral";
    //fontRoman="STIXGeneral";
    fontMathRoman="STIXGeneral";
    fontEncoding=MTFEunicode;
    brace_shrink_factor=0.6;
}

void JKQTmathText::useXITS()
{
    QFontDatabase fdb;
#ifdef AUTOLOAD_XITS_FONTS
    //qDebug()<<"has XITS: "<<fdb.families().contains("XITS");
    if (!fdb.families().contains("XITS")) {
        int i=0;
        if (QFile::exists(":/JKQTmathText/fonts/xits-bold.otf")) { i=QFontDatabase::addApplicationFont(":/JKQTmathText/fonts/xits-bold.otf"); }
        //qDebug()<<QFontDatabase::applicationFontFamilies(i);
        if (QFile::exists(":/JKQTmathText/fonts/xits-bolditalic.otf")) { i=QFontDatabase::addApplicationFont(":/JKQTmathText/fonts/xits-bolditalic.otf"); }
        //qDebug()<<QFontDatabase::applicationFontFamilies(i);
        if (QFile::exists(":/JKQTmathText/fonts/xits-italic.otf")) { i=QFontDatabase::addApplicationFont(":/JKQTmathText/fonts/xits-italic.otf"); }
        //qDebug()<<QFontDatabase::applicationFontFamilies(i);
        if (QFile::exists(":/JKQTmathText/fonts/xits-math.otf")) { i=QFontDatabase::addApplicationFont(":/JKQTmathText/fonts/xits-math.otf"); }
        //qDebug()<<QFontDatabase::applicationFontFamilies(i);
        if (QFile::exists(":/JKQTmathText/fonts/xits-mathbold.otf")) { i=QFontDatabase::addApplicationFont(":/JKQTmathText/fonts/xits-mathbold.otf"); }
        //qDebug()<<QFontDatabase::applicationFontFamilies(i);
        if (QFile::exists(":/JKQTmathText/fonts/xits-regular.otf")) { i=QFontDatabase::addApplicationFont(":/JKQTmathText/fonts/xits-regular.otf"); }
        //qDebug()<<QFontDatabase::applicationFontFamilies(i);
        (void)i;
    }
#endif
    //qDebug()<<"has XITS: "<<fdb.families().contains("XITS")<<"\n"<<fdb.families();
    bool hasXITS=false;
    QString XITSfam;
    //bool hasXITSMath=false;
    QString XITSMathfam;
    for (int i=0; i<fdb.families().size(); i++) {
        //::cout<<fdb.families().at(i).simplified().toStdString()<<"\n";
        //qDebug()<<fdb.families().at(i).simplified();
        if (fdb.families().at(i).contains("XITS Math")) {
            //hasXITSMath=true;
            XITSMathfam=fdb.families().at(i);
        } else if (fdb.families().at(i).contains("XITS")) {
            hasXITS=true;
            XITSfam=fdb.families().at(i);
        }
    }
    QString fam="XITS";
    if (hasXITS) fam=XITSfam;
    //if (hasXITSMath) fam=XITSMathfam;
    useXITSfonts=true;
    //fontRoman=fam;
    //fontSans=fam;
    fontMathRoman=fam;
    fontMathSans=fam;
    fontTypewriter=fam;
    fontBlackboard=fam;
    fontCaligraphic=fam;
    fontScript=fam;
    fontBraces=fam;
    fontSymbol=fam;
    fontGreek=fam;
    fontEncoding=MTFEunicode;
    brace_shrink_factor=0.6;
}

void JKQTmathText::useASANA()
{
    QFontDatabase fdb;
#ifdef AUTOLOAD_XITS_FONTS
    //qDebug()<<"has XITS: "<<fdb.families().contains("XITS");
    if (!fdb.families().contains("Asana")) {
        //int i;//=-1;
        if (QFile::exists(":/JKQTmathText/fonts/asana-math.otf")) { /*i=*/QFontDatabase::addApplicationFont(":/JKQTmathText/fonts/asana-math.otf"); }
        //qDebug()<<QFontDatabase::applicationFontFamilies(i);
    }
#endif
    //qDebug()<<"has ASANA: "<<fdb.families().contains("Asana Math");
    bool hasXITS=false;
    QString XITSfam;
    //bool hasXITSMath=false;
    QString XITSMathfam;
    for (int i=0; i<fdb.families().size(); i++) {
        if (fdb.families().at(i).contains("Asana Math")) {
            //hasXITSMath=true;
            XITSMathfam=fdb.families().at(i);
        } else if (fdb.families().at(i).contains("Asana")) {
            hasXITS=true;
            XITSfam=fdb.families().at(i);
        }
    }
    QString fam="Asana Math";
    if (hasXITS) fam=XITSfam;
    useXITSfonts=true;
    //fontRoman=fam;
    //fontSans=fam;
    fontMathRoman=fam;
    fontMathSans=fam;
    fontTypewriter=fam;
    fontBlackboard=fam;
    fontCaligraphic=fam;
    fontScript=fam;
    fontBraces=fam;
    fontSymbol=fam;
    fontGreek=fam;
    fontEncoding=MTFEunicode;
    brace_shrink_factor=0.6;
}

void JKQTmathText::useLatexFonts(QString prefix, QString postfix) {
    fontLatexPostfix=postfix;
    fontLatexPrefix=prefix;
    fontRoman=prefix+"cmr"+postfix;
    fontSans=prefix+"cmss"+postfix;
    fontMathRoman=prefix+"cmr"+postfix;
    fontMathSans=prefix+"cmss"+postfix;
    fontTypewriter=prefix+"cmtt"+postfix;
    fontBlackboard=prefix+"dsrom"+postfix;
    fontCaligraphic=prefix+"csmy"+postfix;
    fontScript=prefix+"rsfs"+postfix;
    fontBraces=prefix+"cmex"+postfix;
    fontSymbol=prefix+"cmsy"+postfix;
    fontGreek=prefix+"cmmi"+postfix;
    fontEncoding=MTFElatex;
    brace_shrink_factor=0.6;
}

QString JKQTmathText::toHtml(bool *ok, double fontPointSize) {
    QString s;
    bool okk=false;
    if (getTree()!=nullptr) {
        MTenvironment ev;
        ev.color=fontColor;
        ev.fontSize=fontPointSize;

        MTenvironment defaultev;
        defaultev.fontSize=fontPointSize;

        okk=getTree()->toHtml(s, ev, defaultev);
    }
    if (ok) *ok=okk;
    return s;
}

void JKQTmathText::useAnyUnicode(QString timesFont, QString sansFont) {
    if (!timesFont.isEmpty()) { fontMathRoman=timesFont; fontRoman=timesFont; }
    if (!sansFont.isEmpty()) { fontMathSans=sansFont; fontSans=sansFont; }
    useSTIXfonts=false;
    useXITSfonts=false;
    useASANAfonts=false;
    fontGreek=fontMathRoman;
    fontSymbol=fontMathRoman;
    fontBraces=fontMathRoman;
    fontIntegrals=fontMathRoman;
    fontEncoding=MTFEunicode;
    brace_shrink_factor=0.6;
}


JKQTmathText::tokenType JKQTmathText::getToken() {
    currentTokenID++;
    if (currentTokenID>parseString.size()-1) return currentToken=MTTnone;
    QChar c=parseString[currentTokenID];
    currentTokenName="";
    if (c=='\\') { // read an instruction name
        currentTokenID++;
        if (currentTokenID>=parseString.size()-1) return currentToken=MTTnone;
        c=parseString[currentTokenID];
        /*if (c=='_' || c=='\\' || c=='$' || c=='%' || c=='&' || c=='#' || c=='}' || c=='{') {
            currentTokenName=c; // parse special one-symbol instructions like \\, \& ...
                                // that may be directly converted to text
            return currentToken=MTTtext;
        } else*/ if (c=='|' || c==';' || c==':' || c=='!' || c==',' || c=='_' || c=='\\' || c=='$' ||
                     c=='%' || c=='&' || c=='#' || c=='}' || c=='{' || c==' ') {
            currentTokenName=c; // parse one-symbol instructions like \\, \& ...
            //std::cout<<"found text node '"<<currentTokenName.toStdString()<<"'\n";
            return currentToken=MTTinstruction;
        } else { // parse letter instructions
            while (c.isLetter()&& (currentTokenID<parseString.size())) {
                currentTokenName+=c;
                currentTokenID++;
                if (currentTokenID<parseString.size()) c=parseString[currentTokenID];
            }
            if (!c.isLetter()) currentTokenID--;
            currentTokenName=currentTokenName.trimmed();
        }
        //std::cout<<"found instruction node '"<<currentTokenName.toStdString()<<"'\n";
        return currentToken=MTTinstruction;
    } else if (c=='$') {
        //std::cout<<"found dollar\n";
        return currentToken=MTTdollar;
    } else if (c=='&') {
        //std::cout<<"found ampersand\n";
        return currentToken=MTTampersand;
    } else if (c=='{') {
        //std::cout<<"found openbrace\n";
        return currentToken=MTTopenbrace;
    } else if (c=='}') {
        //std::cout<<"found closebrace\n";
        return currentToken=MTTclosebrace;
    } else if (c=='_') {
        //std::cout<<"found underscore\n";
        return currentToken=MTTunderscore;
    } else if (c=='^') {
        //std::cout<<"found hat\n";
        return currentToken=MTThat;
    } else if (c.isSpace()) {
        while (c.isSpace() &&(currentTokenID<parseString.size())) { // eat up whitespace
            currentTokenID++;
            if (currentTokenID<parseString.size())c=parseString[currentTokenID];
        }
        if (!c.isSpace()) currentTokenID--;
        //std::cout<<"found whitespace\n";
        return currentToken=MTTwhitespace;
    } else {
        if (parsingMathEnvironment) {
            // inside math environments we split texts at every brace {[(|)]} so that
            // braces form their own MTtextNode and may be formated accordingly
            if (c=='(' || c=='[' || c=='|' || c==')' || c==']') {
                currentTokenName=c;
                //std::cout<<"found text node '"<<currentTokenName.toStdString()<<"'\n";
                return currentToken=MTTtext;
            }
            while (c!='(' && c!='&' && c!='[' && c!='|' && c!=')' && c!=']' && c!='\\' && c!='$' && c!='{' && c!='}' && c!='_' && c!='^' && (currentTokenID<parseString.size())) {
                // add whitespaces only once
                if (c.isSpace()) {
                    if (!currentTokenName.isEmpty()) {
                        if (!currentTokenName[currentTokenName.size()-1].isSpace())
                            currentTokenName+=c;
                    }
                } else currentTokenName+=c;
                currentTokenID++;
                if (currentTokenID<parseString.size())c=parseString[currentTokenID];
            }
            if (c=='&' || c=='(' || c=='[' || c=='|' || c==')' || c==']' || c=='\\' || c=='$' || c=='{' || c=='}' || c=='_' || c=='^' || c.isSpace()) currentTokenID--;
            //currentTokenName=currentTokenName.trimmed();
            //std::cout<<"found text node '"<<currentTokenName.toStdString()<<"'\n";
            return currentToken=MTTtext;
        } else {
            while ((!c.isSpace()) && c!='&' && c!='\\' && c!='$' && c!='{' && c!='}' && c!='_' && c!='^' && (currentTokenID<parseString.size())) {
                // add whitespaces only once
                if (c.isSpace()) {
                    if (!currentTokenName.isEmpty()) {
                        if (!currentTokenName[currentTokenName.size()-1].isSpace())
                            currentTokenName+=c;
                    }
                } else currentTokenName+=c;
                currentTokenID++;
                if (currentTokenID<parseString.size())c=parseString[currentTokenID];
            }
            if (c=='&' || c=='\\' || c=='$' || c=='{' || c=='}' || c=='_' || c=='^' || c.isSpace()) currentTokenID--;
            //currentTokenName=currentTokenName.trimmed();
            //std::cout<<"found text node '"<<currentTokenName.toStdString()<<"'\n";
            return currentToken=MTTtext;
        }
    }
    return currentToken=MTTnone;
}

JKQTmathText::MTnode* JKQTmathText::parseLatexString(bool get, QString quitOnClosingBrace, QString quitOnEnvironmentEnd) {
    //std::cout<<"    entering parseLatexString()\n";
    MTlistNode* nl=new MTlistNode(this);
    if (get) getToken();
    bool getNew=true;
    while (currentToken!=MTTnone) {
        getNew=true;
        if (currentToken==MTTtext) {
            QString text=currentTokenName;
            bool addWhite=(getToken()==MTTwhitespace) && (!parsingMathEnvironment);
            getNew=addWhite;
            nl->addNode(new MTtextNode(this, text, addWhite, parsingMathEnvironment));
        } else if (currentToken==MTTinstruction) {
            QString name=currentTokenName;
            if (name=="\\") break; // break on linebrak character
            getToken(); // look at next token
            if (currentToken==MTTopenbrace) {
                //std::cout<<"found '{' after '"<<name.toStdString()<<"'\n";
                if (name=="sqrt") {
                    nl->addNode(new MTsqrtNode(this, parseLatexString(true)));
                } else if (name=="cbrt") {
                    nl->addNode(new MTsqrtNode(this, parseLatexString(true), 3));
                } else if (name=="verb") {
                    QString text="";
                    currentTokenID++;
                    if (currentTokenID<=parseString.size()-1) {
                        QChar c=parseString[currentTokenID];
                        while (c!='}' && (currentTokenID<parseString.size())) {
                            text=text+c;
                            currentTokenID++;
                            if (currentTokenID<parseString.size()) c=parseString[currentTokenID];
                        }
                        if (c!='}')  error_list.append(tr("error @ ch. %1: \verb{...} not closed by '}'").arg(currentTokenID).arg(name));
                        nl->addNode(new MTtextNode(this, text, false));
                    }
                } else if (name=="frac") {
                    MTnode* n1=parseLatexString(true);
                    MTnode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addNode(new MTfracNode(this, n1, n2, MTFMfrac));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(name));
                } else if (name=="dfrac") {
                    MTnode* n1=parseLatexString(true);
                    MTnode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addNode(new MTfracNode(this, n1, n2, MTFMdfrac));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(name));
                } else if (name=="tfrac") {
                    MTnode* n1=parseLatexString(true);
                    MTnode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addNode(new MTfracNode(this, n1, n2, MTFMtfrac));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(name));
                } else if (name=="stackrel") {
                    MTnode* n1=parseLatexString(true);
                    MTnode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addNode(new MTfracNode(this, n1, n2, MTFMstackrel));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(name));
                } else if (name=="binom") {
                    MTnode* n1=parseLatexString(true);
                    MTnode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addNode(new MTbraceNode(this, "(", ")", new MTfracNode(this, n1, n2, MTFMstackrel)));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(name));
                } else if (name=="underbrace") {
                    MTnode* n1=parseLatexString(true);
                    MTnode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addNode(new MTfracNode(this, n1, n2, MTFMunderbrace));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(name));
                } else if (name=="underset") {
                    MTnode* n1=parseLatexString(true);
                    MTnode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addNode(new MTfracNode(this, n1, n2, MTFMunderset));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(name));
                } else if (name=="overbrace") {
                    MTnode* n1=parseLatexString(true);
                    MTnode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addNode(new MTfracNode(this, n1, n2, MTFMoverbrace));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(name));
                } else if (name=="overset") {
                    MTnode* n1=parseLatexString(true);
                    MTnode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addNode(new MTfracNode(this, n1, n2, MTFMoverset));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(name));
                } else if (name=="begin") {
                    if (getToken()==MTTtext) {
                        QString envname=currentTokenName;
                        while (currentToken!=MTTclosebrace) getToken(); // find closing brace '}' after '\\begin{name'
                        if (envname=="matrix" || envname=="array" || envname=="aligned" || envname=="align" || envname=="cases" || envname=="pmatrix"|| envname=="bmatrix"|| envname=="Bmatrix"|| envname=="vmatrix"|| envname=="Vmatrix") {
                            QVector< QVector<MTnode*> > items;
                            //int lines=0;
                            //int cols=0;
                            bool first=true;
                            QVector<MTnode*> line;
                            //std::cout<<"found \\begin{matrix}\n";
                            while (first || currentToken==MTTampersand || (currentToken==MTTinstruction && currentTokenName=="\\")) {
                                MTnode* it=parseLatexString(true, "", envname);
                                if (currentToken==MTTampersand) {
                                    //std::cout<<"  appending item\n";
                                    line.append(it);
                                } else {
                                    line.append(it);
                                    //std::cout<<"  appending item and line with "<<line.size()<<" items.\n";
                                    items.append(line);
                                    line.clear();
                                }
                                first=false;
                            }
                            //std::cout<<"  creating matrix-node with "<<items.size()<<" items.\n";
                            if (envname=="pmatrix") nl->addNode(new MTbraceNode(this, "(", ")", new MTmatrixNode(this, items)));
                            else if (envname=="cases") nl->addNode(new MTbraceNode(this, "{", "", new MTmatrixNode(this, items)));
                            else if (envname=="bmatrix") nl->addNode(new MTbraceNode(this, "[", "]", new MTmatrixNode(this, items)));
                            else if (envname=="Bmatrix") nl->addNode(new MTbraceNode(this, "{", "}", new MTmatrixNode(this, items)));
                            else if (envname=="vmatrix") nl->addNode(new MTbraceNode(this, "|", "|", new MTmatrixNode(this, items)));
                            else if (envname=="Vmatrix") nl->addNode(new MTbraceNode(this, "||", "||", new MTmatrixNode(this, items)));
                            else nl->addNode(new MTmatrixNode(this, items));
                            //std::cout<<"  creating matrix-node ... done!\n";
                        } else {
                            error_list.append(tr("error @ ch. %1: unknown environment '%2'").arg(currentTokenID).arg(envname));
                        }
                    } else { // find next '}'
                        error_list.append(tr("error @ ch. %1: text after '\\begin{' expected!").arg(currentTokenID));
                        while (currentToken!=MTTclosebrace) getToken();
                        getNew=true;
                    }
                } else if (name=="end") {
                    if (getToken()==MTTtext) {
                        QString envname=currentTokenName;
                        while (currentToken!=MTTclosebrace) getToken(); // find closing brace '}' after '\\begin{name'
                        if (envname==quitOnEnvironmentEnd) {
                            break;
                        } else {
                            error_list.append(tr("error @ ch. %1: '\\end{%2}' widthout preceding '\\begin{%3}'").arg(currentTokenID).arg(envname).arg(envname));
                        }
                    } else { // find next '}'
                        error_list.append(tr("error @ ch. %1: text after '\\begin{' expected!").arg(currentTokenID));
                        while (currentToken!=MTTclosebrace) getToken();
                        getNew=true;
                    }
                } else if (name=="vec") {
                    nl->addNode(new MTdecoratedNode(this, MTDvec, parseLatexString(true)));
                } else if (name=="overline"||name=="oline"||name=="ol") {
                    nl->addNode(new MTdecoratedNode(this, MTDoverline, parseLatexString(true)));
                } else if (name=="underline"||name=="uline"||name=="ul") {
                    nl->addNode(new MTdecoratedNode(this, MTDunderline, parseLatexString(true)));
                } else if (name=="uuline"||name=="uul") {
                    nl->addNode(new MTdecoratedNode(this, MTDdoubleunderline, parseLatexString(true)));
                } else if (name=="ooline"||name=="ool") {
                    nl->addNode(new MTdecoratedNode(this, MTDdoubleoverline, parseLatexString(true)));
                } else if (name=="arrow") {
                    nl->addNode(new MTdecoratedNode(this, MTDarrow, parseLatexString(true)));
                } else if (name=="hat") {
                    nl->addNode(new MTdecoratedNode(this, MTDhat, parseLatexString(true)));
                } else if (name=="bar") {
                    nl->addNode(new MTdecoratedNode(this, MTDbar, parseLatexString(true)));
                } else if (name=="dot") {
                    nl->addNode(new MTdecoratedNode(this, MTDdot, parseLatexString(true)));
                } else if (name=="tilde") {
                    nl->addNode(new MTdecoratedNode(this, MTDtilde, parseLatexString(true)));
                } else if (name=="ddot") {
                    nl->addNode(new MTdecoratedNode(this, MTDddot, parseLatexString(true)));
                } else {
                    if (name=="textcolor" || name=="mathcolor" || name=="color" || name=="colorbox") {
                        bool foundError=true;
                        QString col="";
                        if (getToken()==MTTtext) {
                            col=currentTokenName;
                            if (getToken()==MTTclosebrace) {
                                if (getToken()==MTTopenbrace) {
                                    foundError=false;
                                }
                            }
                        }
                        if (foundError) error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(name));
                        else nl->addNode(new MTinstruction1Node(this, name, parseLatexString(true), QStringList(col)));


                    } else {
                        nl->addNode(new MTinstruction1Node(this, name, parseLatexString(true)));
                    }
                }
            } else {
                //std::cout<<"did not find '{' after '"<<name.toStdString()<<"'\n";
                if (name=="right") {
                    if (currentToken==MTTtext) {
                        if (currentTokenName.size()>0) {
                            if (QString(currentTokenName[0])==quitOnClosingBrace || quitOnClosingBrace=="any" || QString(currentTokenName[0])==".") {
                                //std::cout<<"found \\right '"<<currentTokenName.toStdString()<<"'\n";
                                showRightBrace=(QString(currentTokenName[0])!=".");
                                //if (!showRightBrace) std::cout<<"don't show right brace '"<<quitOnClosingBrace.toStdString()<<"' !!!\n";
                                if (quitOnClosingBrace!="any") currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                break;
                            } else {
                                getNew=false;
                            }
                        }
                    } else if (currentToken==MTTinstruction) {
                        if (quitOnClosingBrace=="~" && (currentTokenName=="rceil" || QString(currentTokenName[0])==".")) {
                                showRightBrace=(QString(currentTokenName[0])!=".");
                                currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                break;
                        } else if (quitOnClosingBrace=="}" && (currentTokenName=="}" || QString(currentTokenName[0])==".")) {
                                showRightBrace=(QString(currentTokenName[0])!=".");
                                currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                break;
                        } else if (quitOnClosingBrace=="_" && (currentTokenName=="rfloor" || QString(currentTokenName[0])==".")) {
                                showRightBrace=(QString(currentTokenName[0])!=".");
                                currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                break;
                        } else if (quitOnClosingBrace=="#" && (currentTokenName=="|" || QString(currentTokenName[0])==".")) {
                                showRightBrace=(QString(currentTokenName[0])!=".");
                                currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                break;
                        } else if (quitOnClosingBrace==">" && (currentTokenName=="rangle" || QString(currentTokenName[0])==".")) {
                                showRightBrace=(QString(currentTokenName[0])!=".");
                                currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                break;
                        } else if (quitOnClosingBrace=="any") {
                                showRightBrace=(QString(currentTokenName[0])!=".");
                                //currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                break;
                        }
                    } else {
                        getNew=false;
                    }
                } else if (name=="left") {
                    if (currentToken==MTTtext) {
                        if (currentTokenName.size()>0) {
                            if (currentTokenName[0]=='(') {
                                currentTokenName=currentTokenName.right(currentTokenName.size()-1); // we already used the first character from the text token!
                                nl->addNode(new MTbraceNode(this, "(", ")", parseLatexString(currentTokenName.size()<=0, ")"), showRightBrace));
                            } else if (currentTokenName[0]=='[') {
                                currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                nl->addNode(new MTbraceNode(this, "[", "]", parseLatexString(currentTokenName.size()<=0, "]"), showRightBrace));
                            } else if (currentTokenName[0]=='{') {
                                currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                nl->addNode(new MTbraceNode(this, "{", "}", parseLatexString(currentTokenName.size()<=0, "}"), showRightBrace));
                            } else if (currentTokenName[0]=='<') {
                                currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                nl->addNode(new MTbraceNode(this, "<", ">", parseLatexString(currentTokenName.size()<=0, ">"), showRightBrace));
                            } else if (currentTokenName[0]=='|') {
                                currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                nl->addNode(new MTbraceNode(this, "|", "|", parseLatexString(currentTokenName.size()<=0, "|"), showRightBrace));
                            } else if (currentTokenName[0]=='~') {
                                currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                nl->addNode(new MTbraceNode(this, "~", "~", parseLatexString(currentTokenName.size()<=0, "~"), showRightBrace));
                            } else if (currentTokenName[0]=='_') {
                                currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                nl->addNode(new MTbraceNode(this, "_", "_", parseLatexString(currentTokenName.size()<=0, "_"), showRightBrace));
                            } else if (currentTokenName[0]=='#') {
                                currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                nl->addNode(new MTbraceNode(this, "#", "#", parseLatexString(currentTokenName.size()<=0, "#"), showRightBrace));
                            } else if (currentTokenName[0]=='.') {
                                currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                                JKQTmathText::MTnode* cn=parseLatexString(currentTokenName.size()<=0, "any");
                                nl->addNode(new MTbraceNode(this, ".", currentTokenName, cn, showRightBrace));
                            } else {
                                getNew=false;
                            }
                        }
                    } else if (currentToken==MTTinstruction && currentTokenName=="langle") {
                        currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                        nl->addNode(new MTbraceNode(this, "<", ">", parseLatexString(true, ">"), showRightBrace));
                    } else if (currentToken==MTTinstruction && currentTokenName=="{") {
                        currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                        nl->addNode(new MTbraceNode(this, "{", "}", parseLatexString(currentTokenName.size()<=0, "}"), showRightBrace));
                    } else if (currentToken==MTTinstruction && currentTokenName=="lfloor") {
                        currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                        nl->addNode(new MTbraceNode(this, "_", "_", parseLatexString(true, "_"), showRightBrace));
                    } else if (currentToken==MTTinstruction && currentTokenName=="lceil") {
                        currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                        nl->addNode(new MTbraceNode(this, "~", "~", parseLatexString(true, "~"), showRightBrace));
                    } else if (currentToken==MTTinstruction && currentTokenName=="|") {
                        currentTokenName=currentTokenName.right(currentTokenName.size()-1);
                        nl->addNode(new MTbraceNode(this, "#", "#", parseLatexString(currentTokenName.size()<=0, "#"), showRightBrace));
                    } else if (currentToken==MTTinstruction && currentTokenName==quitOnClosingBrace) {
                        break;
                    }
                } else {
                    //bool addWhite=(currentToken==MTTwhitespace);
                    //getNew=addWhite;
                    getNew=false;
                    bool done=false;
                    if (name.size()==2) {
                        QChar n0=name[0];
                        QChar n1=name[1];
                        if (n0=='v' && n1.isLetter()) {
                            done=true;
                            //std::cout<<"found \\v... command\n";
                            nl->addNode(new MTdecoratedNode(this, MTDvec, new MTtextNode(this, QString(n1), false, parsingMathEnvironment)));
                        } else if (n0=='c' && n1.isLetter()) {
                            done=true;
                            //std::cout<<"found \\v... command\n";
                            nl->addNode(new MTinstruction1Node(this, "mathcal", new MTtextNode(this, QString(n1), false, parsingMathEnvironment)));
                        }
                    } else if (name.size()==3) {
                        QString n0=name.left(2);
                        QChar n1=name[name.size()-1];
                        if (n0=="bb" && n1.isLetter()) {
                            done=true;
                            //std::cout<<"found \\v... command\n";
                            nl->addNode(new MTinstruction1Node(this, "mathbb", new MTtextNode(this, QString(n1), false, parsingMathEnvironment)));
                        }
                    }
                    if (!done) nl->addNode(new MTsymbolNode(this, name, false));//, addWhite));

                }
            }
        } else if (currentToken==MTTwhitespace) {
            if (!parsingMathEnvironment) nl->addNode(new MTwhitespaceNode(this));
        } else if (currentToken==MTTunderscore) {
            getToken();
            MTnode* child=nullptr;
            MTnode* child2=nullptr;
            if (currentToken==MTTinstruction) {
                QString name=currentTokenName;
                getToken(); // look at next token
                if (currentToken==MTTopenbrace) {
                    child=new MTinstruction1Node(this, name, parseLatexString(true));
                } else {
                    //bool addWhite=(currentToken==MTTwhitespace);
                    //getNew=addWhite;
                    //child=new MTsymbolNode(this, name, addWhite);
                    getNew=false;
                    child=new MTsymbolNode(this, name, false);
                }
            } else if (currentToken==MTTopenbrace) {
                child=parseLatexString(true);
            } else if (currentToken==MTTtext) {
                if (currentTokenName.size()<=1) {
                    child=new MTtextNode(this, currentTokenName, false, parsingMathEnvironment);
                } else {
                    child=new MTtextNode(this, QString(currentTokenName[0]), false, parsingMathEnvironment);
                    child2=new MTtextNode(this, currentTokenName.right(currentTokenName.size()-1), false, parsingMathEnvironment);
                }
            } else {
                getNew=false;
            }
            if (child!=nullptr) nl->addNode(new MTsubscriptNode(this, child));
            if (child2!=nullptr) nl->addNode(child2);
        } else if (currentToken==MTThat) {
            getToken();
            MTnode* child=nullptr;
            MTnode* child2=nullptr;
            if (currentToken==MTTinstruction) {
                QString name=currentTokenName;
                getToken(); // look at next token
                if (currentToken==MTTopenbrace) {
                    child=new MTinstruction1Node(this, name, parseLatexString(true));
                } else {
                    //bool addWhite=(currentToken==MTTwhitespace);
                    //getNew=addWhite;
                    //child=new MTsymbolNode(this, name, addWhite);
                    getNew=false;
                    child=new MTsymbolNode(this, name, false);
                }
            } else if (currentToken==MTTopenbrace) {
                child=parseLatexString(true);
            } else if (currentToken==MTTtext) {
                if (currentTokenName.size()<=1) {
                    child=new MTtextNode(this, currentTokenName, false, parsingMathEnvironment);
                } else {
                    child=new MTtextNode(this, QString(currentTokenName[0]), false, parsingMathEnvironment);
                    child2=new MTtextNode(this, currentTokenName.right(currentTokenName.size()-1), false, parsingMathEnvironment);
                }
            } else {
                getNew=false;
            }
            if (child!=nullptr) nl->addNode(new MTsuperscriptNode(this, child));
            if (child2!=nullptr) nl->addNode(child2);
        } else if (currentToken==MTTopenbrace) {
            nl->addNode(parseLatexString(true));
        } else if (currentToken==MTTclosebrace) {
            break;
        } else if (currentToken==MTTampersand) {
            break;
        } else if (currentToken==MTTdollar) {
            if (parsingMathEnvironment) { // reached end of math environment
                parsingMathEnvironment=false;
                break;
            } else { // starting math environment
                parsingMathEnvironment=true;
                nl->addNode(new MTinstruction1Node(this, "equation", parseLatexString(true)));
            }
        }
        if (getNew) getToken();
    }
    //std::cout<<"    leaving parseLatexString()\n";
    return nl;
}


QList<JKQTmathText::tbrData> JKQTmathText::tbrs=QList<JKQTmathText::tbrData>();
QHash<JKQTmathText::tbrDataH, QRectF> JKQTmathText::tbrh=QHash<JKQTmathText::tbrDataH, QRectF>();

QRectF JKQTmathText::getTBR(const QFont &fm, const QString &text, QPaintDevice *pd)
{
    JKQTmathText::tbrDataH  dh(fm, text, pd);
    if (pd) {        
        if (tbrh.contains(dh)) return tbrh[dh];
        /*for (int i=0; i<tbrs.size(); i++) {
            if (tbrs[i].f==fm && tbrs[i].text==text && (tbrs[i].ldpiX==pd->logicalDpiX() && tbrs[i].ldpiY==pd->logicalDpiY() && tbrs[i].pdpiX==pd->physicalDpiX() && tbrs[i].pdpiY==pd->physicalDpiY())) {
                //qDebug()<<"   ### "<<fm<<pd<<tbrs[i].text<<tbrs[i].tbr;
                return tbrs[i].tbr;
            }
        }*/
    } else {
        //qDebug()<<"warning no pd";
    }
    tbrData d(fm, text, pd);
    tbrs.append(d);
    tbrh[dh]=d.tbr;
    //qDebug()<<"TBRs lits: "<<tbrs.size();
    //qDebug()<<"+++ "<<fm<<pd<<d.text<<d.tbr;
    return d.tbr;
}





bool JKQTmathText::parse(QString text){
    QString ntext=QString("\\;")+text+QString("\\;");
    ntext=ntext.remove("\\limits");
    if (parsedNode && parseString==ntext) return true;


    if (parsedNode!=nullptr) delete parsedNode;
    if (unparsedNode!=nullptr) delete unparsedNode;
    parseString=ntext;

    currentTokenID=-1;
    currentToken=MTTnone;
    currentTokenName="";
    parsingMathEnvironment=false;
    error_list.clear();
    parsedNode=parseLatexString(true);
    unparsedNode=new MTplainTextNode(this, text, false);
    return (parsedNode!=nullptr);
}


QSizeF JKQTmathText::getSize(QPainter& painter){
    if (getTree()!=nullptr) {
        double w=0, a=0, d=0, s=0;
        getSizeDetail(painter, w, a, d, s);
        return QSizeF(w, a+d);
    }
    return QSizeF(0,0);
}

double JKQTmathText::getDescent(QPainter& painter) {
    double w=0, a=0, d=0, s=0;
    getSizeDetail(painter, w, a, d, s);
    return d;
}

double JKQTmathText::getAscent(QPainter& painter) {
    double w=0, a=0, d=0, s=0;
    getSizeDetail(painter, w, a, d, s);
    return a;
}

void JKQTmathText::getSizeDetail(QPainter& painter, double& width, double& ascent, double& descent, double& strikeoutPos) {
    width=0;
    ascent=0;
    descent=0;
    strikeoutPos=0;
    if (getTree()!=nullptr) {
        MTenvironment ev;
        ev.color=fontColor;
        ev.fontSize=fontSize;

        double overallHeight=0;        
        getTree()->getSize(painter, ev, width, ascent, overallHeight, strikeoutPos);
        descent=overallHeight-ascent;
        ascent=ascent*1.1;
        descent=qMax(ascent*0.1, descent*1.1);
        strikeoutPos=strikeoutPos*1.1;
    }
}

void JKQTmathText::draw(QPainter& painter, double x, double y, bool drawBoxes){
    if (getTree()!=nullptr) {
        MTenvironment ev;
        ev.color=fontColor;
        ev.fontSize=fontSize;
        getTree()->set_drawBoxes(drawBoxes);
        getTree()->draw(painter, x, y, ev);
    }
}

void JKQTmathText::draw(QPainter& painter, int flags, QRectF rect, bool drawBoxes) {
    if (getTree()!=nullptr) {
        MTenvironment ev;
        ev.color=fontColor;
        ev.fontSize=fontSize;
        getTree()->set_drawBoxes(drawBoxes);

        double width=0;
        double baselineHeight=0;
        double overallHeight=0, strikeoutPos=0;
        getTree()->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);

        // align left top
        double x=rect.left();
        double y=rect.top()+baselineHeight;

        // care for horizontal align
        if ((flags & Qt::AlignRight) != 0) x=x+rect.width()-width;
        else if ((flags & Qt::AlignHCenter) != 0) x=x+(rect.width()-width)/2.0;

        // care for vertical align
        if ((flags & Qt::AlignBottom) != 0) y=y+rect.height()-overallHeight;
        else if ((flags & Qt::AlignVCenter) != 0) y=y+(rect.height()-overallHeight)/2.0;

        // finally draw
        getTree()->draw(painter, x, y, ev);
    }
}

JKQTmathText::MTwhitespaceNode::MTwhitespaceNode(JKQTmathText *parent):
    MTtextNode(parent, " ", false, false)
{

}

JKQTmathText::MTwhitespaceNode::~MTwhitespaceNode()
{

}

QString JKQTmathText::MTwhitespaceNode::getTypeName() const
{
    return QLatin1String("MTwhitespaceNode(")+text+")";
}

bool JKQTmathText::MTwhitespaceNode::toHtml(QString &html, JKQTmathText::MTenvironment /*currentEv*/, JKQTmathText::MTenvironment /*defaultEv*/) {
    html=html+"&nbsp;";
    return true;
}

void JKQTmathText::MTnode::set_drawBoxes(bool draw)
{
    this->drawBoxes=draw;
}

QString JKQTmathText::MTnode::getTypeName() const
{
    return "MTnode";
}

QString JKQTmathText::fracModeToString(JKQTmathText::MTfracMode mode)
{
    switch(mode) {
        case MTFMfrac:
            return "frac";
        case MTFMdfrac:
            return "dfrac";
        case MTFMtfrac:
            return "tfrac";
        case MTFMunderbrace:
            return "underbrace";
        case MTFMoverbrace:
            return "overbrace";
        case MTFMunderset:
            return "underset";
        case MTFMoverset:
            return "overset";
        case MTFMstackrel:
            return "stackrel";
    }
    return "unknown";
}


QString JKQTmathText::decorationToString(JKQTmathText::MTdecoration mode)
{
    switch(mode) {
        case MTDvec:
            return "vec";
        case MTDtilde:
            return "tilde";
        case MTDhat:
            return "hat";
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
    }
    return "unknown";
}

JKQTMathTextLabel::JKQTMathTextLabel(QWidget *parent):
    QLabel(parent)
{
    m_mathText=new JKQTmathText(this);
    m_mathText->useXITS();
    m_mathText->set_fontSize(font().pointSizeF()*1.3);
    lastText="";
    repaintDo=true;
    buffer=QPixmap();
}

JKQTMathTextLabel::~JKQTMathTextLabel()
{
}

JKQTmathText *JKQTMathTextLabel::getMathText() const
{
    return m_mathText;
}

void JKQTMathTextLabel::setMath(const QString &text, bool doRepaint)
{
    if (text!=lastText || doRepaint){
        lastText=text;
        repaintDo=true;
        internalPaint();
        update();
    }
}


void JKQTMathTextLabel::internalPaint()
{
    //return;
    //qDebug()<<"internalPaint "<<lastText<<repaintDo;
    //if (repaintDo) {
        QSizeF size;
        {
            repaintDo=false;
            //qDebug()<<"internalPaint(): parse "<<m_mathText->parse(lastText)<<"\n  "<<m_mathText->get_error_list().join("\n")<<"\n\n";
            if (!m_mathText->parse(lastText)) {
                qDebug()<<"JKQTMathTextLabel::internalPaint(): parse '"<<lastText<<"': "<<m_mathText->parse(lastText)<<"\n  "<<m_mathText->get_error_list().join("\n")<<"\n\n";
            }

            if (buffer.width()<=0 || buffer.height()<=0) buffer=QPixmap(1000,100);
            //qDebug()<<"internalPaint(): buffer "<<buffer.size();
            QPainter p;
            //qDebug()<<"internalPaint(): "<<p.begin(&buffer);
            p.begin(&buffer);
            p.setRenderHint(QPainter::Antialiasing);
            p.setRenderHint(QPainter::HighQualityAntialiasing);
            p.setRenderHint(QPainter::TextAntialiasing);
            size=m_mathText->getSize(p);
            p.end();
        }
        buffer=QPixmap(qMax(32.0,size.width()*1.2), qMax(10.0,size.height()*1.1));
        buffer.fill(Qt::transparent);
        {
            //qDebug()<<"internalPaint(): "<<buffer.size()<<size;
            QPainter p;
            //qDebug()<<"internalPaint(): "<<p.begin(&buffer);
            p.begin(&buffer);
            p.setRenderHint(QPainter::Antialiasing);
            p.setRenderHint(QPainter::HighQualityAntialiasing);
            p.setRenderHint(QPainter::TextAntialiasing);
            m_mathText->draw(p,alignment(), QRectF(QPointF(0,0), size));
            p.end();
        }
        setPixmap(buffer);
    //}
    //qDebug()<<"internalPaint(): setPixmap";
    QApplication::processEvents();
    //qDebug()<<"internalPaint(): DONE";
}

void JKQTMathTextLabel::paintEvent(QPaintEvent *event)
{
    //QLabel::paintEvent(event);
    //return;

    //qDebug()<<"paintEvent: "<<buffer.size();

    //QApplication::processEvents();
    QLabel::paintEvent(event);
    //qDebug()<<"paintEvent DONE: "<<size();
}



JKQTmathText::tbrData::tbrData(const QFont &f, const QString &text,  QPaintDevice *pd):
    fm(f, pd)
{
    this->text=text;
    this->tbr=this->fm.tightBoundingRect(text);
    this->f=f;
    //this->pd=pd;
    if (pd) {
        ldpiX=pd->logicalDpiX();
        ldpiY=pd->logicalDpiY();
        pdpiX=pd->physicalDpiX();
        pdpiY=pd->physicalDpiY();
    } else {
        ldpiX=0;
        ldpiY=0;
        pdpiX=0;
        pdpiY=0;
    }
}

bool JKQTmathText::tbrData::operator==(const JKQTmathText::tbrData &other) const
{
    return ldpiX==other.ldpiX &&  ldpiY==other.ldpiY && text==other.text && f==other.f;
}


JKQTmathText::tbrDataH::tbrDataH(const QFont &f, const QString &text, QPaintDevice *pd)
{
    this->text=text;
    this->f=f;
    if (pd) {
        ldpiX=pd->logicalDpiX();
        ldpiY=pd->logicalDpiY();
        pdpiX=pd->physicalDpiX();
        pdpiY=pd->physicalDpiY();
    } else {
        ldpiX=0;
        ldpiY=0;
        pdpiX=0;
        pdpiY=0;
    }
}

bool JKQTmathText::tbrDataH::operator==(const JKQTmathText::tbrDataH &other) const
{
    return ldpiX==other.ldpiX &&  ldpiY==other.ldpiY && text==other.text && f==other.f;

}


JKQTmathText::MTplainTextNode::MTplainTextNode(JKQTmathText *parent, QString text, bool addWhitespace, bool stripInnerWhitepace):
    JKQTmathText::MTtextNode(parent, text, addWhitespace, stripInnerWhitepace)
{

}

QString JKQTmathText::MTplainTextNode::getTypeName() const
{return QLatin1String("MTplainTextNode(")+text+")";

}


QString JKQTmathText::MTplainTextNode::textTransform(const QString &text, JKQTmathText::MTenvironment /*currentEv*/, bool /*forSize*/)
{
    return text;
}

void initJKQtMathTextResources()
{
    Q_INIT_RESOURCE(xits);
}
