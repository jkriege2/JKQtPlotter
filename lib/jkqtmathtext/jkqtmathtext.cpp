/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

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



#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtcommon/jkqtpcodestructuring.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include <cmath>
#include <QFontMetricsF>
#include <QDebug>
#include <QFontDatabase>
#include <typeinfo>
#include <QApplication>
#include <QPainterPath>


const double JKQTMathText::ABS_MIN_LINEWIDTH=0.02;

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

JKQTMathText::MTenvironment::MTenvironment() {
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

QFont JKQTMathText::MTenvironment::getFont(JKQTMathText* parent) const {
    QFont f;
    switch (font) {
        case MTEsans: if (insideMath) {
                f.setFamily(parent->getFontMathSans());
            } else {
                f.setFamily(parent->getFontSans());
            }
            break;
        case MTEmathSans: f.setFamily(parent->getFontMathSans()); break;
        case MTEtypewriter: f.setFamily(parent->getFontTypewriter()); break;
        case MTEscript: f.setFamily(parent->getFontScript()); break;
        case MTEcaligraphic: f.setFamily(parent->getFontCaligraphic()); break;
        case MTEblackboard: f.setFamily(parent->getFontBlackboard()); break;
        case MTEfraktur: f.setFamily(parent->getFontFraktur()); break;
        case MTEmathRoman: f.setFamily(parent->getFontMathRoman()); break;
        default:
        case MTEroman: if (insideMath) {
                f.setFamily(parent->getFontMathRoman());
            } else {
                f.setFamily(parent->getFontRoman());
            }
            break;
    }
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

QString JKQTMathText::MTenvironment::toHtmlStart(MTenvironment defaultEv) const {
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

QString JKQTMathText::MTenvironment::toHtmlAfter(JKQTMathText::MTenvironment /*defaultEv*/) const {
    return "</span>";
}

JKQTMathText::MTnode::MTnode(JKQTMathText* parent) {
    this->parent=parent;
    drawBoxes=false;
}

JKQTMathText::MTnode::~MTnode()
= default;

void JKQTMathText::MTnode::getSize(QPainter &painter, JKQTMathText::MTenvironment currentEv, double &width, double &baselineHeight, double &overallHeight, double &strikeoutPos, const MTnodeSize* prevNodeSize)
{
    double w=width, b=baselineHeight, o=overallHeight, s=strikeoutPos;
    getSizeInternal(painter, currentEv, w, b, o, s, prevNodeSize);

    if (w<1e5) width=w;
    if (b<1e5) baselineHeight=b;
    if (o<1e5) overallHeight=o;
    if (s<1e5) strikeoutPos=s;
}


double JKQTMathText::MTnode::getNonItalicXCorretion(QPainter &painter, double width_potentiallyitalic, const MTenvironment &ev_potentiallyitalic, JKQTMathText::MTnode* child) const
{
    double italic_xcorrection=0.0;
    if (ev_potentiallyitalic.italic) {
        MTenvironment ev_nonitalic=ev_potentiallyitalic;
        ev_nonitalic.italic=false;
        double width_nonitalic=0, baselineHeight_nonitalic=0, overallHeight_nonitalic=0, strikeoutPos_nonitalic=0;
        child->getSize(painter, ev_nonitalic, width_nonitalic, baselineHeight_nonitalic, overallHeight_nonitalic, strikeoutPos_nonitalic);
        italic_xcorrection=width_potentiallyitalic-width_nonitalic;
    }
    return italic_xcorrection;
}


bool JKQTMathText::MTnode::toHtml(QString &/*html*/, JKQTMathText::MTenvironment /*currentEv*/, JKQTMathText::MTenvironment /*defaultEv*/) {
    return false;
}

bool JKQTMathText::MTnode::getDrawBoxes() const {
    return this->drawBoxes;
}


void JKQTMathText::MTnode::doDrawBoxes(QPainter& painter, double x, double y, JKQTMathText::MTenvironment currentEv) {
    if (drawBoxes) {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
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
        painter.drawEllipse(QRectF(x-3.0,y-3.0,6.0,6.0));
        p.setColor("lightgreen");
        painter.setPen(p);
        painter.drawLine(QLineF(x-2.0, y, x+2.0, y));
        painter.drawLine(QLineF(x, y-2, x, y+2.0));

    }
}


JKQTMathText::MTtextNode::MTtextNode(JKQTMathText* _parent, const QString& textIn, bool addWhitespace, bool stripInnerWhitepace):
    JKQTMathText::MTnode(_parent)
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

JKQTMathText::MTtextNode::~MTtextNode() = default;

void JKQTMathText::MTtextNode::getSizeInternal(QPainter& painter, JKQTMathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* /*prevNodeSize*/) {
    QFont f=currentEv.getFont(parent);
    if (currentEv.insideMath && (text=="(" || text=="[" || text=="|" || text=="]" || text==")" || text=="<" || text==">" ||
                                 text==QString(QChar(0x2329)) || text==QString(QChar(0x232A)) || text==QString(QChar(0x2308)) ||
                                 text==QString(QChar(0x2309)) || text==QString(QChar(0x230A)) || text==QString(QChar(0x230B)))) {
            f.setItalic(false);
            //f.setFamily(parent->getFontData(currentEv.font, currentEv.insideMath, FontSubclass::Text).first);
        }
    QString txt=textTransform(text, currentEv, true);
    QFontMetricsF fm(f, painter.device());
    QRectF br=fm.boundingRect(txt);
    QRectF tbr=parent->getTightBoundingRect(f, txt, painter.device()); //fm.tightBoundingRect(txt);
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

double JKQTMathText::MTtextNode::draw(QPainter& painter, double x, double y, JKQTMathText::MTenvironment currentEv, const MTnodeSize* /*prevNodeSize*/) {
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
    }


    if (onlyDigits && currentEv.insideMath) {
        f.setItalic(false);
    }

    painter.setFont(f);

    //qDebug()<<"MTtextNode: text="<<text<<" font="<<f;

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
        if (currentEv.font==MTEblackboard && parent->blackboardSimulated) {
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
                if (currentEv.font==MTEblackboard && parent->blackboardSimulated) {
                    QPainterPath path;
                    path.addText(QPointF(xx, y), ff, QString(txt[i]));
                    painter.drawPath(path);
                } else {
                    painter.setFont(ff);
                    painter.drawText(QPointF(xx, y), QString(txt[i]));
                }
                xx=xx+fmff.boundingRect(txt[i]).width();
            } else {
                if (currentEv.font==MTEblackboard && parent->blackboardSimulated) {
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

bool JKQTMathText::MTtextNode::toHtml(QString &html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) {
    html=html+currentEv.toHtmlStart(defaultEv)+text+currentEv.toHtmlAfter(defaultEv);
    return true;
}

QString JKQTMathText::MTtextNode::getText() const {
    return this->text;
}

QString JKQTMathText::MTtextNode::getTypeName() const
{
    return QLatin1String("MTtextNode(")+text+")";
}

QString JKQTMathText::MTtextNode::textTransform(const QString &text, JKQTMathText::MTenvironment currentEv, bool /*forSize*/)
{
    QString txt=text;
    auto fnt=parent->getFontData(currentEv.font, currentEv.insideMath);
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







JKQTMathText::MTinstruction1Node::MTinstruction1Node(JKQTMathText* _parent, const QString& name, MTnode* child, const QStringList& parameters):
    JKQTMathText::MTnode(_parent)
{
    this->name=name;
    this->child=child;
    this->parameters=parameters;

    JKQTMathText::MTenvironment ev;
    if (!setupMTenvironment(ev)) {
        parent->error_list.append(tr("unknown instruction '%1' found!").arg(name));
    }
}

JKQTMathText::MTinstruction1Node::~MTinstruction1Node() {
    if (child!=nullptr) delete child;
}

QString JKQTMathText::MTinstruction1Node::getTypeName() const
{
    return QLatin1String("MTinstruction1Node(")+name+")";
}

void JKQTMathText::MTinstruction1Node::getSizeInternal(QPainter& painter, JKQTMathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* /*prevNodeSize*/) {
    JKQTMathText::MTenvironment ev=currentEv;

    setupMTenvironment(ev);

    child->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);
    if (name=="colorbox" || name=="fbox" || name=="boxed") {
        QFontMetricsF fm(ev.getFont(parent));
        double xw=fm.boundingRect("x").width();
        width+=xw;
        overallHeight+=xw;
        baselineHeight+=xw/2.0;
    }
}

double JKQTMathText::MTinstruction1Node::draw(QPainter& painter, double x, double y, JKQTMathText::MTenvironment currentEv, const MTnodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathText::MTenvironment ev=currentEv;

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
        QFontMetricsF fm(currentEv.getFont(parent));
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

bool JKQTMathText::MTinstruction1Node::toHtml(QString &html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) {
    JKQTMathText::MTenvironment ev=currentEv;

    setupMTenvironment(ev);

    return child->toHtml(html, ev, defaultEv);
}

void JKQTMathText::MTinstruction1Node::setDrawBoxes(bool draw)
{
    drawBoxes=draw;
    child->setDrawBoxes(draw);
}

JKQTMathText::MTnode *JKQTMathText::MTinstruction1Node::getChild() const {
    return this->child;
}

QString JKQTMathText::MTinstruction1Node::getName() const {
    return this->name;
}

QStringList JKQTMathText::MTinstruction1Node::getParameters() const {
    return this->parameters;
}

bool JKQTMathText::MTinstruction1Node::setupMTenvironment(JKQTMathText::MTenvironment &ev)
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
    else if (name=="rm" || name=="textrm") { ev.font=JKQTMathText::MTEroman; ev.italic=false; }
    else if (name=="mathrm" || name=="unit" ||  name=="operatorname") { ev.font=JKQTMathText::MTEroman; ev.italic=false; }
    else if (name=="mathbfit" || name=="bfit" || name=="textbfit") { ev.bold=true; ev.italic=true; }
    else if (name=="text" || name=="mbox" || name=="ensuretext") { ev.insideMath=false; ev.font=JKQTMathText::MTEroman; ev.italic=false; }
    else if (name=="mat") { ev.font=JKQTMathText::MTEroman; ev.italic=false; ev.bold=true; }
    else if (name=="cal" || name=="textcal" || name=="mathcal") { ev.font=JKQTMathText::MTEcaligraphic; ev.italic=false; }
    else if (name=="fcal" || name=="textfcal" || name=="mathfcal") { ev.font=JKQTMathText::MTEcaligraphic; ev.bold=true; }
    else if (name=="frak" || name=="textfrak" || name=="mathfrak") { ev.font=JKQTMathText::MTEfraktur; ev.italic=false; }
    else if (name=="ffrak" || name=="textffrak" || name=="mathffrak") { ev.font=JKQTMathText::MTEfraktur; ev.bold=true; }
    else if (name=="bb" || name=="textbb" || name=="mathbb") { ev.font=JKQTMathText::MTEblackboard; ev.italic=false; }
    else if (name=="tt" || name=="texttt" || name=="mathtt") { ev.font=JKQTMathText::MTEtypewriter; ev.italic=false; }
    else if (name=="sf" || name=="textsf" || name=="mathsf") { ev.font=JKQTMathText::MTEsans; ev.italic=false; }
    else if (name=="sfit" || name=="textsfit" || name=="mathsfit") { ev.font=JKQTMathText::MTEsans; ev.italic=true; }
    else if (name=="script" || name=="scr" || name=="textscript" || name=="textscr" || name=="mathscript" || name=="mathscr") { ev.font=JKQTMathText::MTEscript; ev.italic=false; }
    else if (name=="fscript" || name=="fscr" || name=="textfscript" || name=="textfscr" || name=="mathfscript" || name=="mathfscr") { ev.font=JKQTMathText::MTEscript; ev.bold=true; ev.italic=false; }
    else if (name=="displaystyle") { ev.fontSize=ev.fontSize/0.8; }
    else if (name=="scriptstyle") { ev.fontSize=ev.fontSize*0.8; }
    else if (name=="scriptscriptstyle") { ev.fontSize=ev.fontSize*0.8*0.8; }
    else {
        return false;
    }

    return true;
}




JKQTMathText::MTsubscriptNode::MTsubscriptNode(JKQTMathText* _parent, MTnode* child):
    JKQTMathText::MTnode(_parent)
{
    this->child=child;
}

JKQTMathText::MTsubscriptNode::~MTsubscriptNode() {
    if (child!=nullptr) delete child;
}

void JKQTMathText::MTsubscriptNode::getSizeInternal(QPainter& painter, JKQTMathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize) {
    JKQTMathText::MTenvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parent->getSubsuperSizeFactor();

    child->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);

    QFontMetricsF fm(ev.getFont(parent), painter.device());
    QRectF tbr=parent->getTightBoundingRect(currentEv.getFont(parent), "M", painter.device());
    double shift=parent->getSubShiftFactor()*tbr.height();

    if (prevNodeSize!=nullptr && prevNodeSize->overallHeight-prevNodeSize->baselineHeight>shift) {
        shift=-1.0*(prevNodeSize->overallHeight-prevNodeSize->baselineHeight-shift);
    }

    double yshift=baselineHeight-shift;
    baselineHeight=shift;
    strikeoutPos=fm.strikeOutPos()+yshift;
    if (currentEv.italic && prevNodeSize==nullptr) width=width-double(fm.boundingRect(' ').width())*parent->getItalicCorrectionFactor();
}

double JKQTMathText::MTsubscriptNode::draw(QPainter& painter, double x, double y, JKQTMathText::MTenvironment currentEv, const MTnodeSize* prevNodeSize) {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathText::MTenvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parent->getSubsuperSizeFactor();
    QFontMetricsF fm(ev.getFont(parent), painter.device());
    QRectF tbr=parent->getTightBoundingRect(currentEv.getFont(parent), "M", painter.device());

    double width=0, baselineHeight=0, overallHeight=0, strikeoutPos=0;
    child->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);
    double shift=parent->getSubShiftFactor()*tbr.height();

    if (prevNodeSize!=nullptr && prevNodeSize->overallHeight-prevNodeSize->baselineHeight>shift) {
        //qDebug()<<"oldshift="<<shift<<", prevNodeSize->overallHeight="<<prevNodeSize->overallHeight<<", prevNodeSize->baselineHeight="<<prevNodeSize->baselineHeight;
        shift=-1.0*(prevNodeSize->overallHeight-prevNodeSize->baselineHeight-shift);
        //qDebug()<<"newshift="<<shift;
    }

    double yshift=baselineHeight-shift;
    //qDebug()<<"baselineHeight="<<baselineHeight<<", overallHeight="<<overallHeight<<", strikeoutPos="<<strikeoutPos;
    //qDebug()<<"shift="<<shift<<", yshift="<<yshift;
    double xx=x;
    if (currentEv.italic && prevNodeSize==nullptr) xx=xx-double(fm.boundingRect(' ').width())*parent->getItalicCorrectionFactor();
    return child->draw(painter, xx, y+yshift, ev);//+0.5*fm.boundingRect("A").width();
}

QString JKQTMathText::MTsubscriptNode::getTypeName() const
{
    return "MTsubscriptNode";
}

JKQTMathText::MTnode *JKQTMathText::MTsubscriptNode::getChild() const {
    return this->child;
}


bool JKQTMathText::MTsubscriptNode::toHtml(QString &html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) {
    html=html+"<sub>";
    bool ok=child->toHtml(html, currentEv, defaultEv);
    html=html+"</sub>";
    return ok;
}

void JKQTMathText::MTsubscriptNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child->setDrawBoxes(draw);

}





JKQTMathText::MTsqrtNode::MTsqrtNode(JKQTMathText* _parent, MTnode* child, int degree):
    JKQTMathText::MTnode(_parent)
{
    this->child=child;
    this->degree=degree;
}

JKQTMathText::MTsqrtNode::~MTsqrtNode() {
    if (child!=nullptr) delete child;
}

void JKQTMathText::MTsqrtNode::getSizeInternal(QPainter& painter, JKQTMathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* /*prevNodeSize*/) {
    QFontMetricsF fm(currentEv.getFont(parent), painter.device());

    child->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);

    overallHeight=overallHeight*1.2;//+fm.ascent()*0.1;
    baselineHeight=baselineHeight*1.2;//+fm.ascent()*0.1;
    width=width+fm.boundingRect("A").width()*2; // 1.53
}

double JKQTMathText::MTsqrtNode::draw(QPainter& painter, double x, double y, JKQTMathText::MTenvironment currentEv, const MTnodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    double width=0, baselineHeight=0, overallHeight=0, sp=0;
    child->getSize(painter, currentEv, width, baselineHeight, overallHeight, sp);
    QFont f=currentEv.getFont(parent);
    QFont fsmall=f;
    QFontMetricsF fm(f, painter.device());
    double w=fm.boundingRect("A").width();
    double a=baselineHeight*1.15;
    double d=overallHeight-baselineHeight;
    //painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=painter.pen();
    p.setColor(currentEv.color);
    p.setWidthF(fm.lineWidth());
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
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(p);
    if (w>0) {
        path.lineTo( xnew+0.2*w, y-a);
        path.lineTo(xnew+0.2*w, y-0.8*a);
        painter.drawPath(path);
    }

    return xnew+0.33*w;
}

bool JKQTMathText::MTsqrtNode::toHtml(QString &html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) {
    html=html+"&radic;<span style=\"text-decoration:overline\">";
    bool ok=child->toHtml(html, currentEv, defaultEv);
    html=html+"&nbsp;</span>";
    return ok;
}

void JKQTMathText::MTsqrtNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child->setDrawBoxes(draw);

}

QString JKQTMathText::MTsqrtNode::getTypeName() const
{
    return "MTsqrtNode";
}

JKQTMathText::MTnode *JKQTMathText::MTsqrtNode::getChild() const {
    return this->child;
}

int JKQTMathText::MTsqrtNode::getDegree() const {
    return this->degree;
}






JKQTMathText::MTfracNode::MTfracNode(JKQTMathText* _parent, MTnode* child_top, MTnode* child_bottom, MTfracMode mode):
    JKQTMathText::MTnode(_parent)
{
    this->child1=child_top;
    this->child2=child_bottom;
    this->mode=mode;
}

JKQTMathText::MTfracNode::~MTfracNode() {
    if (child1!=nullptr) delete child1;
    if (child2!=nullptr) delete child2;
}

QString JKQTMathText::MTfracNode::getTypeName() const
{
    return "MTfracNode";
}

void JKQTMathText::MTfracNode::getSizeInternal(QPainter& painter, JKQTMathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* /*prevNodeSize*/) {
    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    JKQTMathText::MTenvironment ev1=currentEv;
    JKQTMathText::MTenvironment ev2=currentEv;

    double xh=fm.xHeight(); //tightBoundingRect("x").height();
    double sp=xh;
    double Ah=parent->getTightBoundingRect(currentEv.getFont(parent), "M", painter.device()).height();//fm.ascent();
    double xw=fm.boundingRect("x").width();

    if (mode==MTFMunderbrace || mode==MTFMoverbrace) {
        ev2.fontSize=ev2.fontSize*parent->getUnderbraceFactor();
    } else if (mode==MTFMunderset || mode==MTFMoverset) {
        ev2.fontSize=ev2.fontSize*parent->getUndersetFactor();
    } else  if (mode==MTFMfrac || mode==MTFMsfrac) {
        ev1.fontSize=ev1.fontSize*parent->getFracFactor();
        ev2.fontSize=ev2.fontSize*parent->getFracFactor();
    } else  if (mode==MTFMtfrac || mode==MTFMstfrac) {
        ev1.fontSize=ev1.fontSize*parent->getFracFactor()*0.7;
        ev2.fontSize=ev2.fontSize*parent->getFracFactor()*0.7;
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
        //overallHeight=overallHeight1+overallHeight2+sp*(2.0*parent->getFracShiftFactor());
        //baselineHeight=overallHeight1+xh*(2.0*parent->getFracShiftFactor());
        overallHeight=2.0*qMax(overallHeight1, overallHeight2)+sp*(2.0*parent->getFracShiftFactor());
        baselineHeight=qMax(overallHeight1, overallHeight2)+xh*(2.0*parent->getFracShiftFactor());
        //std::cout<<"=>  baselineHeight="<<baselineHeight<<",  overallHeight="<<overallHeight<<std::endl;
        width=qMax(width1, width2)+ xw;
        strikeoutPos=sp;
    } else if (mode==MTFMstfrac || mode==MTFMsfrac) {
        overallHeight=2.0*qMax(overallHeight1, overallHeight2)+sp*(2.0*parent->getFracShiftFactor());
        baselineHeight=qMax(overallHeight1, overallHeight2)+xh*(2.0*parent->getFracShiftFactor());
        width=width1+width2+xw;
        strikeoutPos=sp;
    } else if (mode==MTFMstackrel) {
        //overallHeight=overallHeight1+overallHeight2+sp*(2.0*parent->getFracShiftFactor());
        //baselineHeight=overallHeight1+xh*(2.0*parent->getFracShiftFactor());
        overallHeight=2.0*qMax(overallHeight1, overallHeight2)+sp*(2.0*parent->getFracShiftFactor());
        baselineHeight=qMax(overallHeight1, overallHeight2)+xh*(2.0*parent->getFracShiftFactor());
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

double JKQTMathText::MTfracNode::draw(QPainter& painter, double x, double y, JKQTMathText::MTenvironment currentEv, const MTnodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    QFont f=currentEv.getFont(parent);
    QFontMetricsF fm(f, painter.device());
    JKQTMathText::MTenvironment ev1=currentEv;
    JKQTMathText::MTenvironment ev2=currentEv;


    double xh=parent->getTightBoundingRect(f, "x", painter.device()).height(); //fm.xHeight();
    double xw=fm.boundingRect("x").width();
    double lw=qMax(0.0,ceil(currentEv.fontSize/16.0));//fm.lineWidth();
    double Ah=parent->getTightBoundingRect(f, "M", painter.device()).height();//fm.ascent();
    double bw=Ah/2.0;

    if (mode==MTFMunderbrace || mode==MTFMoverbrace) {
        ev2.fontSize=ev2.fontSize*parent->getUnderbraceFactor();
    } else if (mode==MTFMunderset || mode==MTFMoverset) {
        ev2.fontSize=ev2.fontSize*parent->getUndersetFactor();
    } else  if (mode==MTFMfrac || mode==MTFMsfrac) {
        ev1.fontSize=ev1.fontSize*parent->getFracFactor();
        ev2.fontSize=ev2.fontSize*parent->getFracFactor();
    } else  if (mode==MTFMtfrac || mode==MTFMstfrac) {
        ev1.fontSize=ev1.fontSize*parent->getFracFactor()*0.7;
        ev2.fontSize=ev2.fontSize*parent->getFracFactor()*0.7;
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
    p.setWidthF(qMax(JKQTMathText::ABS_MIN_LINEWIDTH, lw));
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.setPen(p);
    if (mode==MTFMfrac || mode==MTFMdfrac || mode==MTFMtfrac) {
        QLineF l(x+xw/4.0, yline, x+width+xw/2.0, yline);
        if (l.length()>0) painter.drawLine(l);
        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, yline-xh*(parent->getFracShiftFactor())-descent1, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, yline+xh*(parent->getFracShiftFactor())+ascent2, ev2);
    } else if (mode==MTFMstfrac || mode==MTFMsfrac) {
        child1->draw(painter, x, yline-descent1, ev1);
        child2->draw(painter, x+width+xw, yline+ascent2, ev2);
        QLineF l(x+width+1.2*xw, yline-descent1-ascent1, x+width-0.2*xw, yline+ascent1+descent1);
        if (l.length()>0) painter.drawLine(l);
    } else if (mode==MTFMstackrel) {
        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, yline-xh*(parent->getFracShiftFactor())-descent1, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, yline+xh*(parent->getFracShiftFactor())+ascent2, ev2);
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

        {
            painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate(x+xw/2.0+(width1)/2.0, ybrace);
            painter.rotate(180);
            QPainterPath path=makeHBracePath(0,0, width, bw);
            painter.drawPath(path);
        }

        child1->draw(painter, x+xw/2.0+(width-width1)/2.0, y, ev1);
        child2->draw(painter, x+xw/2.0+(width-width2)/2.0, y-ascent1-bw-descent2, ev2);
    }


    if (mode==MTFMstackrel) return x+width+ xw;
    else if (mode==MTFMstfrac || mode==MTFMsfrac) return x+width+width2+xw;
    else return x+width+xw;

}

bool JKQTMathText::MTfracNode::toHtml(QString &/*html*/, JKQTMathText::MTenvironment /*currentEv*/, JKQTMathText::MTenvironment /*defaultEv*/) {
    bool ok=false;



    return ok;
}

void JKQTMathText::MTfracNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child1->setDrawBoxes(draw);
    child2->setDrawBoxes(draw);

}

JKQTMathText::MTnode *JKQTMathText::MTfracNode::getChild1() const {
    return this->child1;
}

JKQTMathText::MTnode *JKQTMathText::MTfracNode::getChild2() const {
    return this->child2;
}

JKQTMathText::MTfracMode JKQTMathText::MTfracNode::getMode() const {
    return this->mode;
}









JKQTMathText::MTmatrixNode::MTmatrixNode(JKQTMathText* _parent, QVector<QVector<MTnode*> > children):
    JKQTMathText::MTnode(_parent)
{
    this->lines=children.size();
    this->columns=0;
    for (int i=0; i<children.size(); i++) {
        if (children[i].size()>this->columns) this->columns=children[i].size();
    }
    this->children=children;
}

JKQTMathText::MTmatrixNode::~MTmatrixNode() {
    for (int i=0; i<children.size(); i++) {
        for (int j=0; j<children[i].size(); j++) {
            delete children[i].at(j);
        }
    }
    children.clear();
}

QString JKQTMathText::MTmatrixNode::getTypeName() const
{
    return "MTmatrixNode";
}

void JKQTMathText::MTmatrixNode::getSizeInternal(QPainter& painter, JKQTMathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* /*prevNodeSize*/) {
    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    JKQTMathText::MTenvironment ev1=currentEv;

    double xh=fm.strikeOutPos();//fm.xHeight();
    //double Ah=fm.ascent();
    double xw=fm.boundingRect("x").width();

    //ev1.fontSize=ev1.fontSize*parent->getFracFactor();


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

double JKQTMathText::MTmatrixNode::draw(QPainter& painter, double x, double y, JKQTMathText::MTenvironment currentEv, const MTnodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);

    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    JKQTMathText::MTenvironment ev1=currentEv;

    double xh=fm.strikeOutPos();//fm.xHeight();
    //double Ah=fm.ascent();
    double xw=fm.boundingRect("x").width();

    //ev1.fontSize=ev1.fontSize;*parent->getFracFactor();


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

bool JKQTMathText::MTmatrixNode::toHtml(QString &/*html*/, JKQTMathText::MTenvironment /*currentEv*/, JKQTMathText::MTenvironment /*defaultEv*/)
{
    return false;
}

QVector<QVector<JKQTMathText::MTnode *> > JKQTMathText::MTmatrixNode::getChildren() const {
    return this->children;
}

int JKQTMathText::MTmatrixNode::getColumns() const {
    return this->columns;
}

int JKQTMathText::MTmatrixNode::getLines() const {
    return this->lines;
}

void JKQTMathText::MTmatrixNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
    for (int i=0; i<lines; i++) {
        for (int j=0; j<children[i].size(); j++) {
            children[i].at(j)->setDrawBoxes(draw);
        }
    }

}








JKQTMathText::MTdecoratedNode::MTdecoratedNode(JKQTMathText* _parent, MTdecoration decoration, MTnode* child):
    JKQTMathText::MTnode(_parent)
{
    this->child=child;
    this->decoration=decoration;
}

JKQTMathText::MTdecoratedNode::~MTdecoratedNode() {
    if (child!=nullptr) delete child;
}

void JKQTMathText::MTdecoratedNode::getSizeInternal(QPainter& painter, JKQTMathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* /*prevNodeSize*/) {
    const QFontMetricsF fm(currentEv.getFont(parent), painter.device());

    child->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);

    const double italic_xcorrection=getNonItalicXCorretion(painter, width, currentEv, child);
    const double decoheightfactor=parent->getDecorationHeightFactor();
    const double deco_miniwidth=((decoration==MTDtilde||decoration==MTDbreve)?fm.boundingRect("~").width():fm.boundingRect("^").width())-italic_xcorrection;

    const double decoAboveAscent_yposdelta=fm.ascent()*(1.0+2.0*decoheightfactor);
    const double decoAboveBaselineheight_yposdelta=baselineHeight*(1.0+decoheightfactor);


    const double descent=overallHeight-baselineHeight;
    baselineHeight=decoAboveBaselineheight_yposdelta;
    if (decoration==MTDbar) {
        baselineHeight=std::max<double>(baselineHeight, decoAboveAscent_yposdelta);
    }
    overallHeight=baselineHeight+descent;
    width=std::max<double>(deco_miniwidth,width);
}

double JKQTMathText::MTdecoratedNode::draw(QPainter& painter, double x, double y, JKQTMathText::MTenvironment currentEv, const MTnodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    MTenvironment ev=currentEv;
    double width=0, baselineHeight=0, overallHeight=0, strikeoutPos=0;
    child->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);
    const QFont font=ev.getFont(parent);
    const QFontMetricsF fm(font, painter.device());
    const double width_X=fm.boundingRect("X").width();
    const double width_x=fm.boundingRect("x").width();
    const double width_dot=fm.boundingRect(".").width()/2.0;
    const double decoheightfactor=parent->getDecorationHeightFactor();
    const double deco_ypos=y-baselineHeight*(1.0+decoheightfactor);
    const double decoAboveAscent_ypos=y-fm.ascent()*(1.0+decoheightfactor);
    const double strike_ypos=y-baselineHeight/2.0;
    const double decobelow_ypos=y+qMax((overallHeight-baselineHeight)*(1.0+decoheightfactor), fm.xHeight()*decoheightfactor);
    const double deco_height=decoheightfactor*baselineHeight;
    const double italic_xcorrection=getNonItalicXCorretion(painter, width, ev, child);
    const double deco_xoffset=parent->getDecorationWidthReductionXFactor()*width_X/2.0;
    const double deco_width=std::max<double>(width_x*0.5,width-2.0*deco_xoffset-italic_xcorrection);
    const double deco_vecwidth=width_x*0.33;
    const double deco_miniwidth=((decoration==MTDtilde||decoration==MTDbreve)?fm.boundingRect("~").width():fm.boundingRect("^").width())-italic_xcorrection;
    const double decotop_xcenter=x+italic_xcorrection+(width-italic_xcorrection)/2.0;
    const double decotop_xstart=decotop_xcenter-deco_width/2.0;
    const double decotop_xend=decotop_xcenter+deco_width/2.0;
    const double decobot_xstart=x;
    const double decobot_xend=x+width-italic_xcorrection;
    const double decobot_xcenter=(decobot_xstart+decobot_xend)/2.0;



    QPen pold=painter.pen();
    QPen p=pold;
    p.setColor(ev.color);
    p.setWidthF(qMax(JKQTMathText::ABS_MIN_LINEWIDTH, fm.lineWidth()));//ceil(currentEv.fontSize/16.0));

    double xnew=child->draw(painter, x, y, ev);

    if (decoration==MTDvec) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(decotop_xstart, deco_ypos)<<QPointF(decotop_xend, deco_ypos)<<QPointF(decotop_xend-deco_vecwidth, deco_ypos-deco_height*2.0/3.0);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    } else if (decoration==MTDoverline) {
        painter.setPen(p);
        const QLineF l(decotop_xstart, deco_ypos, decotop_xend, deco_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDbar) {
        painter.setPen(p);
        const QLineF l(decotop_xstart, decoAboveAscent_ypos, decotop_xend, decoAboveAscent_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDdoubleoverline) {
        painter.setPen(p);
        const QLineF l(decotop_xstart, deco_ypos, decotop_xend, deco_ypos);
        if (l.length()>0) painter.drawLine(l);
        const QLineF l2(decotop_xstart, deco_ypos-2.0*p.widthF(), decotop_xend, deco_ypos-2.0*p.widthF());
        if (l2.length()>0) painter.drawLine(l2);
        painter.setPen(pold);
    } else if (decoration==MTDunderline) {
        painter.setPen(p);
        const QLineF l(decobot_xstart, decobelow_ypos, decobot_xend, decobelow_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDdoubleunderline) {
        painter.setPen(p);
        const QLineF l(decobot_xstart, decobelow_ypos, decobot_xend, decobelow_ypos);
        if (l.length()>0) painter.drawLine(l);
        QLineF l2(decobot_xstart, decobelow_ypos+2.0*p.widthF(), decobot_xend, decobelow_ypos+2.0*p.widthF());
        if (l2.length()>0) painter.drawLine(l2);
        painter.setPen(pold);
    } else if (decoration==MTDarrow) {
        painter.setPen(p);
        const QLineF l(decotop_xstart, deco_ypos+deco_height/2.0, decotop_xend, deco_ypos+deco_height/2.0);
        if (l.length()>0) painter.drawLine(l);
        QPolygonF poly;
        poly<<QPointF(decotop_xend-deco_vecwidth, deco_ypos)<<QPointF(decotop_xend, deco_ypos+deco_height/2.0)<<QPointF(decotop_xend-deco_vecwidth, deco_ypos+deco_height);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    } else if (decoration==MTDhat) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(decotop_xcenter-0.5*deco_miniwidth, deco_ypos+deco_height/3.0)<<QPointF(decotop_xcenter, deco_ypos)<<QPointF(decotop_xcenter+0.5*deco_miniwidth, deco_ypos+deco_height/3.0);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    } else if (decoration==MTDwidehat) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(decotop_xstart, deco_ypos+deco_height/2.0)<<QPointF(decotop_xcenter, deco_ypos+deco_height/3.0)<<QPointF(decotop_xend, deco_ypos+deco_height/2.0);
        painter.drawPolyline(poly);
        painter.setPen(pold);

    } else if (decoration==MTDcheck) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(decotop_xcenter-0.5*deco_miniwidth, deco_ypos)<<QPointF(decotop_xcenter, deco_ypos+deco_height/3.0)<<QPointF(decotop_xcenter+0.5*deco_miniwidth, deco_ypos);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    } else if (decoration==MTDwidecheck) {
        painter.setPen(p);
        QPolygonF poly;
        poly<<QPointF(decotop_xstart, deco_ypos-deco_height/2.0)<<QPointF(decotop_xcenter, deco_ypos+deco_height/3.0)<<QPointF(decotop_xend, deco_ypos-deco_height/2.0);
        painter.drawPolyline(poly);
        painter.setPen(pold);
    } else if (decoration==MTDbreve) {
        painter.setPen(p);
        QPainterPath path;
        const double x0=decotop_xcenter-deco_miniwidth/2.0;
        const double x1=decotop_xcenter+deco_miniwidth/2.0;
        path.moveTo(x0, deco_ypos);
        path.cubicTo(x0, deco_ypos+deco_height,  x1, deco_ypos+deco_height    , x1, deco_ypos);
        painter.drawPath(path);
        painter.setPen(pold);

    } else if (decoration==MTDtilde) {
        painter.setPen(p);
        QPainterPath path;
        const double x0=decotop_xcenter-deco_miniwidth/2.0;
        const double x1=decotop_xcenter+deco_miniwidth/2.0;
        path.moveTo(x0, deco_ypos);
        path.cubicTo((x0+x1)/2.0, deco_ypos+deco_height,  (x0+x1)/2.0, deco_ypos-deco_height    , x1, deco_ypos);
        painter.drawPath(path);
        painter.setPen(pold);

    } else if (decoration==MTDwidetilde) {
        painter.setPen(p);
        QPainterPath path;
        const double x0=decotop_xstart;
        const double x1=decotop_xend;
        path.moveTo(x0, deco_ypos);
        path.cubicTo((x0+x1)/2.0, deco_ypos+deco_height,  (x0+x1)/2.0, deco_ypos-deco_height    , x1, deco_ypos);
        painter.drawPath(path);
        painter.setPen(pold);

    } else if (decoration==MTDocirc) {
        painter.setPen(p);
        const QBrush bold=painter.brush();
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(QPointF(decotop_xcenter, deco_ypos), width_dot/1.5, width_dot/1.5);
        painter.setBrush(bold);
        painter.setPen(pold);
    } else if (decoration==MTDdot) {
        p.setWidthF(0.01);
        painter.setPen(p);
        QBrush b=painter.brush();
        const QBrush bold=b;
        b.setColor(ev.color);
        b.setStyle(Qt::SolidPattern);
        painter.setBrush(b);
        painter.drawEllipse(QPointF(decotop_xcenter, deco_ypos), width_dot/2.0, width_dot/2.0);
        painter.setBrush(bold);
        painter.setPen(pold);
    } else if (decoration==MTDddot) {
        p.setWidthF(0.01);
        painter.setPen(p);
        QBrush b=painter.brush();
        const QBrush bold=b;
        b.setColor(ev.color);
        b.setStyle(Qt::SolidPattern);
        painter.setBrush(b);
        painter.drawEllipse(QPointF(decotop_xcenter-width_dot, deco_ypos), width_dot/2.0, width_dot/2.0);
        painter.drawEllipse(QPointF(decotop_xcenter+width_dot, deco_ypos), width_dot/2.0, width_dot/2.0);
        painter.setBrush(bold);
        painter.setPen(pold);
        painter.setBrush(bold);
    } else if (decoration==MTDstrike) {
        painter.setPen(p);
        const QLineF l((decotop_xstart+decobot_xstart)/2.0, strike_ypos, (decotop_xend+decobot_xend)/2.0, strike_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDcancel) {
        painter.setPen(p);
        const QLineF l(decobot_xstart, decobelow_ypos, decotop_xend, deco_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDbcancel) {
        painter.setPen(p);
        const QLineF l(decobot_xstart, deco_ypos, decotop_xend, decobelow_ypos);
        if (l.length()>0) painter.drawLine(l);
        painter.setPen(pold);
    } else if (decoration==MTDxcancel) {
        painter.setPen(p);
        const QLineF l(decobot_xstart, deco_ypos, decotop_xend, decobelow_ypos);
        if (l.length()>0) painter.drawLine(l);
        const QLineF l1(decobot_xstart, decobelow_ypos, decotop_xend, deco_ypos);
        if (l1.length()>0) painter.drawLine(l1);
        painter.setPen(pold);
    }

    /*painter.setPen(QPen(Qt::red, 1.5));
    painter.drawLine(QLineF(x, deco_ypos, xnew, deco_ypos));
    painter.setPen(QPen(Qt::green, 1.5));
    painter.drawLine(QLineF(deco_xstart, deco_ypos+2, deco_xend, deco_ypos+2));
    painter.drawEllipse(QPointF(deco_xpos_center, deco_ypos+2), 5, 5);
    painter.setPen(pold);*/
    return xnew;
}

bool JKQTMathText::MTdecoratedNode::toHtml(QString &/*html*/, JKQTMathText::MTenvironment /*currentEv*/, JKQTMathText::MTenvironment /*defaultEv*/) {
    //QString f;
    //JKQTMathText::MTenvironment ev=currentEv;

    //bool ok=child->toHtml(html, ev, defaultEv);

    return false;
}

void JKQTMathText::MTdecoratedNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child->setDrawBoxes(draw);

}

QString JKQTMathText::MTdecoratedNode::getTypeName() const
{
    return "MTdecoratedNode";
}

JKQTMathText::MTnode *JKQTMathText::MTdecoratedNode::getChild() const {
    return this->child;
}

JKQTMathText::MTdecoration JKQTMathText::MTdecoratedNode::getDecoration() const {
    return this->decoration;
}






JKQTMathText::MTsuperscriptNode::MTsuperscriptNode(JKQTMathText* _parent, MTnode* child):
    JKQTMathText::MTnode(_parent)
{
    this->child=child;
}

JKQTMathText::MTsuperscriptNode::~MTsuperscriptNode() {
    if (child!=nullptr) delete child;
}

void JKQTMathText::MTsuperscriptNode::getSizeInternal(QPainter& painter, JKQTMathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* prevNodeSize) {
    JKQTMathText::MTenvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parent->getSubsuperSizeFactor();
    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    QRectF tbr=parent->getTightBoundingRect(currentEv.getFont(parent), "M", painter.device());
    child->getSize(painter, ev, width, baselineHeight, overallHeight, strikeoutPos);
    double shift=parent->getSuperShiftFactor()*tbr.height();

    if (prevNodeSize!=nullptr && prevNodeSize->baselineHeight>tbr.height()) {
        shift=prevNodeSize->baselineHeight-(overallHeight-baselineHeight)-shift;
    }

    double yshift=shift+overallHeight-baselineHeight;
    baselineHeight=overallHeight=overallHeight+shift;
    strikeoutPos=strikeoutPos-yshift;
    if (currentEv.italic && prevNodeSize==nullptr) width=width+double(fm.boundingRect(' ').width())*parent->getItalicCorrectionFactor();
}

double JKQTMathText::MTsuperscriptNode::draw(QPainter& painter, double x, double y, JKQTMathText::MTenvironment currentEv, const MTnodeSize* prevNodeSize) {
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathText::MTenvironment ev=currentEv;
    ev.fontSize=ev.fontSize*parent->getSubsuperSizeFactor();

    double cWidth, cBaselineHeight, cOverallHeight, cStrikeoutPos;
    child->getSize(painter, ev, cWidth, cBaselineHeight, cOverallHeight, cStrikeoutPos);

    QFontMetricsF fm(currentEv.getFont(parent), painter.device());
    QRectF tbr=parent->getTightBoundingRect(currentEv.getFont(parent), "M", painter.device());
    double shift=parent->getSuperShiftFactor()*tbr.height();

    if (prevNodeSize!=nullptr && prevNodeSize->baselineHeight>tbr.height()) {
        shift=prevNodeSize->baselineHeight-(cOverallHeight-cBaselineHeight)-shift;
    }

    double yshift=shift+cOverallHeight-cBaselineHeight;
    double xx=x;
    if (currentEv.italic && prevNodeSize==nullptr) xx=xx+double(fm.boundingRect(' ').width())*parent->getItalicCorrectionFactor();

    return child->draw(painter, xx, y-yshift, ev);//+0.5*fm.boundingRect("A").width();
}

JKQTMathText::MTnode *JKQTMathText::MTsuperscriptNode::getChild() const {
    return this->child;
}


QString JKQTMathText::MTsuperscriptNode::getTypeName() const
{
    return "MTsuperscriptNode";
}


bool JKQTMathText::MTsuperscriptNode::toHtml(QString &html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv)
{
    html=html+"<sup>";
    bool ok=child->toHtml(html, currentEv, defaultEv);
    html=html+"</sup>";
    return ok;
}

void JKQTMathText::MTsuperscriptNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child->setDrawBoxes(draw);

}






JKQTMathText::MTbraceNode::MTbraceNode(JKQTMathText* _parent, const QString& openbrace, const QString& closebrace, MTnode* child, bool showRightBrace):
    JKQTMathText::MTnode(_parent)
{
    this->child=child;
    this->openbrace=openbrace;
    this->closebrace=closebrace;
    this->showRightBrace=showRightBrace;
}

JKQTMathText::MTbraceNode::~MTbraceNode() {
    if (child!=nullptr) delete child;
}

void JKQTMathText::MTbraceNode::getSizeInternal(QPainter& painter, JKQTMathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* /*prevNodeSize*/) {

    JKQTMathText::MTenvironment ev=currentEv;
    child->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);

    double bracewidth=0, braceheight=0;
    getBraceWidth(painter, ev, baselineHeight, overallHeight, bracewidth, braceheight);

    bracewidth=bracewidth/parent->getBraceShrinkFactor();

    baselineHeight=/*qMin(baselineHeight, braceheight)*/ baselineHeight*parent->getBraceFactor();
    overallHeight=qMax(overallHeight, braceheight)*parent->getBraceFactor(); //fm.height();

    width=width+bracewidth*2.0;


}

double JKQTMathText::MTbraceNode::draw(QPainter& painter, double x, double y, JKQTMathText::MTenvironment currentEv, const MTnodeSize* /*prevNodeSize*/) {
    //std::cout<<"drawing brace-node: '"<<openbrace.toStdString()<<"' ... '"<<closebrace.toStdString()<<"'\n";
    doDrawBoxes(painter, x, y, currentEv);
    JKQTMathText::MTenvironment ev=currentEv;

    double width=0;
    double baselineHeight=0;
    double overallHeight=0, strikeoutPos=0;

    child->getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);

    double bracewidth=0, braceheight=0;
    getBraceWidth(painter, ev, baselineHeight, overallHeight, bracewidth, braceheight);

    double cWidth=0;
    double cBaselineHeight=0;
    double cOverallHeight=0, cstrikeoutPos=0;

    getSize(painter, currentEv, cWidth, cBaselineHeight, cOverallHeight, cstrikeoutPos);

    double lw=qMax(0.25,ceil(currentEv.fontSize/16.0));//fm.lineWidth();

    double xnew=x+lw;

    QPen pold=painter.pen();
    QPen p=pold;
    p.setWidthF(lw);
    p.setColor(currentEv.color);
    painter.setPen(p);
    double brace_fraction=0.85;
    if (openbrace=="(") {
        QPainterPath path;
        double y1=y+(cOverallHeight-cBaselineHeight);
        double y2=y-cBaselineHeight;
        path.moveTo(xnew+brace_fraction*bracewidth, y1);
        path.cubicTo(xnew, (y1+y2)/2.0+fabs(y1-y2)/6.0, xnew, (y1+y2)/2.0-fabs(y1-y2)/6.0   , xnew+brace_fraction*bracewidth, y2);
        painter.drawPath(path);
    } else if (openbrace=="[") {
        QPainterPath path;
        double y1=y+(cOverallHeight-cBaselineHeight);
        double y2=y-cBaselineHeight;
        path.moveTo(xnew+brace_fraction*bracewidth, y1);
        path.lineTo(xnew+lw/2.0, y1);
        path.lineTo(xnew+lw/2.0, y2);
        path.lineTo(xnew+brace_fraction*bracewidth, y2);
        painter.drawPath(path);
    } else if (openbrace=="{") {
        QPainterPath path=makeHBracePath(0,0,cOverallHeight, bracewidth*brace_fraction);
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(xnew+bracewidth*(1.0-brace_fraction), y-cBaselineHeight+cOverallHeight/2.0);
        painter.rotate(90);
        painter.drawPath(path);

    } else if (openbrace=="_") {
        QPainterPath path;
        double y1=y+(cOverallHeight-cBaselineHeight);
        double y2=y-cBaselineHeight;
        path.moveTo(xnew+brace_fraction*bracewidth, y1);
        path.lineTo(xnew, y1);
        path.lineTo(xnew, y2);
        painter.drawPath(path);
    } else if (openbrace=="~") {
        QPainterPath path;
        double y1=y+(cOverallHeight-cBaselineHeight);
        double y2=y-cBaselineHeight;
        path.moveTo(xnew, y1);
        path.lineTo(xnew, y2);
        path.lineTo(xnew+brace_fraction*bracewidth, y2);
        painter.drawPath(path);
    } else if (openbrace=="|") {
        QPainterPath path;
        double y1=y+(cOverallHeight-cBaselineHeight);
        double y2=y-cBaselineHeight;
        QLineF l(xnew+brace_fraction*bracewidth, y1, xnew+brace_fraction*bracewidth, y2);
        if (l.length()>0) painter.drawLine(l);
        painter.drawPath(path);
    } else if (openbrace=="#" || openbrace=="||") {
        QPainterPath path;
        double y1=y+(cOverallHeight-cBaselineHeight);
        double y2=y-cBaselineHeight;
        QLineF l(xnew+brace_fraction*bracewidth, y1, xnew+brace_fraction*bracewidth, y2);
        if (l.length()>0) painter.drawLine(l);
        l=QLineF(xnew+brace_fraction*bracewidth-1.5*lw, y1, xnew+brace_fraction*bracewidth-1.5*lw, y2);
        if (l.length()>0) painter.drawLine(l);
    } else if (openbrace=="<") {
        QPainterPath path;
        double y1=y+(cOverallHeight-cBaselineHeight);
        double y2=y-cBaselineHeight;
        path.moveTo(xnew+brace_fraction*bracewidth, y1);
        path.lineTo(xnew, (y2+y1)/2.0);
        path.lineTo(xnew+brace_fraction*bracewidth, y2);
        painter.drawPath(path);
    }

    painter.setPen(pold);

    xnew= child->draw(painter, xnew+bracewidth/parent->getBraceShrinkFactor()-lw, y, currentEv)+lw;

    if (showRightBrace) {
        painter.setPen(p);
        if (closebrace==")") {
            QPainterPath path;
            double y1=y+(cOverallHeight-cBaselineHeight);
            double y2=y-cBaselineHeight;
            path.moveTo(xnew+(1.0-brace_fraction)*bracewidth, y1);
            path.cubicTo(xnew+bracewidth, (y1+y2)/2.0+fabs(y1-y2)/6.0, xnew+bracewidth, (y1+y2)/2.0-fabs(y1-y2)/6.0   , xnew+(1.0-brace_fraction)*bracewidth, y2);
            painter.drawPath(path);
        } else if (closebrace=="]") {
            QPainterPath path;
            double y1=y+(cOverallHeight-cBaselineHeight);
            double y2=y-cBaselineHeight;
            path.moveTo(xnew+(1.0-brace_fraction)*bracewidth, y1);
            path.lineTo(xnew+bracewidth-lw/2.0, y1);
            path.lineTo(xnew+bracewidth-lw/2.0, y2);
            path.lineTo(xnew+(1.0-brace_fraction)*bracewidth, y2);
            painter.drawPath(path);
        } else if (closebrace=="}") {
            QPainterPath path=makeHBracePath(0,0,cOverallHeight, bracewidth*brace_fraction);
            painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.translate(xnew+bracewidth*brace_fraction, y-cBaselineHeight+cOverallHeight/2.0);
            painter.rotate(270);
            painter.drawPath(path);

        } else if (closebrace=="_") {
            QPainterPath path;
            double y1=y+(cOverallHeight-cBaselineHeight);
            double y2=y-cBaselineHeight;
            path.moveTo(xnew+(1.0-brace_fraction)*bracewidth, y1);
            path.lineTo(xnew+bracewidth, y1);
            path.lineTo(xnew+bracewidth, y2);
            painter.drawPath(path);
        } else if (closebrace=="~") {
            QPainterPath path;
            double y1=y+(cOverallHeight-cBaselineHeight);
            double y2=y-cBaselineHeight;
            path.moveTo(xnew+bracewidth, y1);
            path.lineTo(xnew+bracewidth, y2);
            path.lineTo(xnew+(1.0-brace_fraction)*bracewidth, y2);
            painter.drawPath(path);
        } else if (closebrace=="|") {
            QPainterPath path;
            double y1=y+(cOverallHeight-cBaselineHeight);
            double y2=y-cBaselineHeight;
            QLineF l(xnew+(1.0-brace_fraction)*bracewidth, y1, xnew+(1.0-brace_fraction)*bracewidth, y2);
            if (l.length()>0) painter.drawLine(l);
            painter.drawPath(path);
        } else if (closebrace=="#" || closebrace=="||") {
            QPainterPath path;
            double y1=y+(cOverallHeight-cBaselineHeight);
            double y2=y-cBaselineHeight;
            QLineF l(xnew+(1.0-brace_fraction)*bracewidth, y1, xnew+(1.0-brace_fraction)*bracewidth, y2);
            if (l.length()>0) painter.drawLine(l);
            l=QLineF(xnew+(1.0-brace_fraction)*bracewidth+1.5*lw, y1, xnew+(1.0-brace_fraction)*bracewidth+1.5*lw, y2);
            if (l.length()>0) painter.drawLine(l);
        } else if (closebrace==">") {
            QPainterPath path;
            double y1=y+(cOverallHeight-cBaselineHeight);
            double y2=y-cBaselineHeight;
            path.moveTo(xnew+(1.0-brace_fraction)*bracewidth, y1);
            path.lineTo(xnew+bracewidth, (y2+y1)/2.0);
            path.lineTo(xnew+(1.0-brace_fraction)*bracewidth, y2);
            painter.drawPath(path);
        }
        painter.setPen(pold);
    }

    //qDebug()<<" ==> "<<bc<<fm.boundingRect(bc).width();
    return xnew+bracewidth/parent->getBraceShrinkFactor()-lw;
}

bool JKQTMathText::MTbraceNode::toHtml(QString &html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) {
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

void JKQTMathText::MTbraceNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
    child->setDrawBoxes(draw);

}

QString JKQTMathText::MTbraceNode::getTypeName() const
{
    return QLatin1String("MTbraceNode(")+openbrace+" "+closebrace+")";
}

void JKQTMathText::MTbraceNode::getBraceWidth(QPainter &/*painter*/, JKQTMathText::MTenvironment ev, double /*baselineHeight*/, double overallHeight, double &bracewidth, double &braceheight)
{
    /*QFont evf=ev.getFont(parent);
    if (ev.insideMath) evf.setItalic(false);
    ev.italic=false;
    while (ev.fontSize<10*parent->getFontSize()) {
        const QFontMetricsF fme(evf, painter.device());
        if (fme.ascent()>overallHeight) break;
        ev.fontSize+=0.5;
        evf.setPointSizeF(ev.fontSize);
    }
    ev.fontSize=ev.fontSize*parent->getBraceFactor();
    evf.setPointSizeF(ev.fontSize);
    QFontMetricsF fm(evf, painter.device());
    QString bc="_X";
    bracewidth=fm.width("I")*parent->getBraceShrinkFactor();
    braceheight=parent->getTBR(evf, bc, painter.device()).height();*/
    double lw=qMax(0.25,ceil(ev.fontSize/12.0));
    braceheight=overallHeight*parent->getBraceFactor();
    bracewidth=0.6*pow(braceheight, 0.6);
    if (openbrace=="{" || closebrace=="}")  bracewidth=qMax(bracewidth, lw*3.5);

}





JKQTMathText::MTlistNode::MTlistNode(JKQTMathText* _parent):
    JKQTMathText::MTnode(_parent)
{
    nodes.clear();
    // these operations cause sub/sup script to be typeset over/under the operator, not right besides!
    subsupOperations<<"sum"<<"prod"<<"coprod"
               <<"bigcap"<<"bigcup"<<"bigvee"<<"bighat"
               <<"int"<<"iint"<<"iiint"<<"oint"<<"oiint"<<"oiiint"
               <<"mod"<<"median"<<"max"<<"min"<<"argmax"<<"argmin"<<"sup"<<"inf"
               <<"liminf"<<"limsup"<<"lim"<<"max"<<"min";
}

JKQTMathText::MTlistNode::~MTlistNode() {
    for (int i=0; i<nodes.size(); i++) {
        delete nodes[i];
    }
    nodes.clear();
}

QString JKQTMathText::MTlistNode::getTypeName() const
{
    return "MTlistNode";
}

void JKQTMathText::MTlistNode::getSizeInternal(QPainter& painter, JKQTMathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* /*prevNodeSize*/) {
    width=0;
    overallHeight=0;
    baselineHeight=0;
    strikeoutPos=0;
    QFontMetricsF fm(currentEv.getFont(parent));
    //QRectF tbr=parent->getTightBoundingRect(currentEv.getFont(parent), "M", painter.device());


    double xnew=0;
    bool wasBrace=false;
    for (int i=0; i<nodes.size(); i++) {
        MTnodeSize prevNodeSize;
        MTnodeSize* prevNodeSizePtr=nullptr;

        if (i>0 && wasBrace) {
            nodes[i-1]->getSize(painter, currentEv, prevNodeSize.width, prevNodeSize.baselineHeight, prevNodeSize.overallHeight, prevNodeSize.strikeoutPos);
            prevNodeSizePtr=&prevNodeSize;
        }


        bool doDraw=true;
        MTsymbolNode* smb=dynamic_cast<MTsymbolNode*>(nodes[i]);
        // if we find a subscript/superscript node we check whether the next node is super/subscript
        // if so, we typeset them at the same x-psotion, so sub/superscripts appear correctly
        if (dynamic_cast<MTsuperscriptNode*>(nodes[i])) {
            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<MTsubscriptNode*>(nodes[i+1])) { // is this subscript?
                    double w1, w2, oh, bh, sp;
                    nodes[i]->getSize(painter, currentEv, w1, bh, oh, sp, prevNodeSizePtr);

                    if (bh>baselineHeight) {
                        overallHeight=overallHeight+bh-baselineHeight;
                        baselineHeight=bh;
                        strikeoutPos=sp;
                    }
                    if (baselineHeight+oh-bh>overallHeight) {
                        overallHeight=baselineHeight+oh-bh;
                        strikeoutPos=sp;
                    }

                    i++;
                    nodes[i]->getSize(painter, currentEv, w2, bh, oh, sp, prevNodeSizePtr);
                    //qDebug()<<"super_sub:   sub: "<<nodes[i]->getTypeName()<<"  w2="<<w2<<" bh"<<bh<<" oh="<<oh<<" sp="<<sp;
                    if (bh>baselineHeight) {
                        overallHeight=overallHeight+bh-baselineHeight;
                        baselineHeight=bh;
                        strikeoutPos=sp;
                    }
                    if (baselineHeight+oh-bh>overallHeight) {
                        overallHeight=baselineHeight+oh-bh;
                        strikeoutPos=sp;
                    }
                    xnew+=qMax(w1+fm.boundingRect(' ').width(), w2);

                    doDraw=false;
                    //qDebug()<<"### super+sub";
                    //qDebug()<<"### subsupop: super+sub   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            }
        } else if (dynamic_cast<MTsubscriptNode*>(nodes[i])) {
            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<MTsuperscriptNode*>(nodes[i+1])) { // is this subscript?
                    double w1, w2, oh, bh, sp;
                    nodes[i]->getSize(painter, currentEv, w1, bh, oh, sp, prevNodeSizePtr);
                    if (bh>baselineHeight) {
                        overallHeight=overallHeight+bh-baselineHeight;
                        baselineHeight=bh;
                        strikeoutPos=sp;
                    }
                    if (baselineHeight+oh-bh>overallHeight) {
                        overallHeight=baselineHeight+oh-bh;
                        strikeoutPos=sp;
                    }

                    i++;
                    nodes[i]->getSize(painter, currentEv, w2, bh, oh, sp, prevNodeSizePtr);
                    if (bh>baselineHeight) {
                        overallHeight=overallHeight+bh-baselineHeight;
                        baselineHeight=bh;
                        strikeoutPos=sp;
                    }
                    if (baselineHeight+oh-bh>overallHeight) {
                        overallHeight=baselineHeight+oh-bh;
                        strikeoutPos=sp;
                    }
                    xnew+=qMax(w1, w2+fm.boundingRect(' ').width());


                    doDraw=false;
                    //qDebug()<<"### sub+super";
                    //qDebug()<<"### subsupop: sub+super1   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            }
        } else if (smb) {
            QString s=smb->getSymbolName();
            if (subsupOperations.contains(s)) {
                MTsubscriptNode* subn=nullptr;
                if (i+1<nodes.size()) subn=dynamic_cast<MTsubscriptNode*>(nodes[i+1]);
                MTsuperscriptNode* supn=nullptr;
                if (i+2<nodes.size()) supn=dynamic_cast<MTsuperscriptNode*>(nodes[i+2]);
                //std::cout<<"symbol ='"<<s.toStdString()<<"'   subn="<<subn<<"   supn="<<supn<<"\n";
                if (subn && supn) { // is this subscript and superscript?
                    MTenvironment ev=currentEv;
                    ev.fontSize=ev.fontSize*parent->getOperatorsubsuperSizeFactor();
                    double w1=0, w2=0, w3=0;
                    double oh1=0, oh2=0, oh3=0;
                    double bh1=0, bh2=0, bh3=0;
                    double sp1=0, sp2=0, sp3=0;
                    nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp1);
                    //qDebug()<<"sub_super:   node: "<<nodes[i]->getTypeName()<<"  w1="<<w1<<" bh"<<bh1<<" oh="<<oh1<<" sp="<<sp1;
                    subn->getChild()->getSize(painter, ev, w2, bh2, oh2, sp2);
                    //qDebug()<<"sub_super:   node: "<<subn->getTypeName()<<"  w2="<<w2<<" bh2"<<bh2<<" oh2="<<oh2<<" sp2="<<sp2;
                    supn->getChild()->getSize(painter, ev, w3, bh3, oh3, sp3);
                    //qDebug()<<"sub_super:   node: "<<supn->getTypeName()<<"  w3="<<w3<<" bh3"<<bh3<<" oh3="<<oh3<<" sp3="<<sp3;
                    //double d1=oh1-bh1;
                    //double d2=oh2-bh2;
                    //double d3=oh3-bh3;
                    double w=qMax(qMax(w1, w2), w3)+fm.boundingRect(' ').width();

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
                    ev.fontSize=ev.fontSize*parent->getOperatorsubsuperSizeFactor();
                    double w1=0, w2=0, w3=0;
                    double oh1=0, oh2=0, oh3=0;
                    double bh1=0, bh2=0, bh3=0;
                    double sp1=0, sp2=0, sp3=0;
                    nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp1);
                    supn->getChild()->getSize(painter, ev, w3, bh3, oh3, sp3);
                    //double d1=oh1-bh1;
                    //double d2=oh2-bh2;
                    //double d3=oh3-bh3;
                    double w=qMax(w1, w3)+fm.boundingRect(' ').width();

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
                    ev.fontSize=ev.fontSize*parent->getOperatorsubsuperSizeFactor();
                    double w1=0, w2=0;
                    double oh1=0, oh2=0;
                    double bh1=0, bh2=0;
                    double sp1=0, sp2=0;//, sp3=0;
                    nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp1);
                    subn->getChild()->getSize(painter, ev, w2, bh2, oh2, sp2);
                    //double d1=oh1-bh1;
                    //double d2=oh2-bh2;

                    double oh=oh1+oh2;
                    double sh=oh1-bh1+oh2*1.1;
                    if (oh>overallHeight) overallHeight=oh;
                    if (bh1>baselineHeight) baselineHeight=bh1;
                    if (sh>overallHeight-baselineHeight) {
                        overallHeight=baselineHeight+sh;
                    }
                    double w=qMax(w1, w2)+fm.boundingRect(' ').width();
                    i++;
                    doDraw=false;
                    xnew+=w;
                    //qDebug()<<"### subsupop: sub   overallHeight="<<overallHeight<<" baselineHeight="<<baselineHeight;
                }
            }
        }

        if (i<nodes.size() && doDraw) {
            double w, oh, bh, sp;
            nodes[i]->getSize(painter, currentEv, w, bh, oh, sp, prevNodeSizePtr);


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

            xnew+=w;
            //qDebug()<<i<<xnew;
        }
         wasBrace=dynamic_cast<MTbraceNode*>(nodes[i]);
    }
    width=xnew;
}

double JKQTMathText::MTlistNode::draw(QPainter& painter, double x, double y, JKQTMathText::MTenvironment currentEv, const MTnodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    double ynew=y;
    double xnew=x;
    //qDebug()<<"listNode: "<<currentEv.fontSize;
    QFontMetricsF fm(currentEv.getFont(parent));
    bool wasBrace=false;
    for (int i=0; i<nodes.size(); i++) {
        bool doDraw=true;

        MTnodeSize prevNodeSize;
        MTnodeSize* prevNodeSizePtr=nullptr;

        if (i>0 && wasBrace) {
            nodes[i-1]->getSize(painter, currentEv, prevNodeSize.width, prevNodeSize.baselineHeight, prevNodeSize.overallHeight, prevNodeSize.strikeoutPos);
            prevNodeSizePtr=&prevNodeSize;
        }


        MTsymbolNode* smb=dynamic_cast<MTsymbolNode*>(nodes[i]);
        // if we find a subscript/superscript node we check whether the next node is super/subscript
        // if so, we typeset them at the same x-psotion, so sub/superscripts appear correctly
        if (dynamic_cast<MTsuperscriptNode*>(nodes[i])) {

            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<MTsubscriptNode*>(nodes[i+1])) { // is this subscript?

                    //painter.setPen(QPen("red"));
                    //painter.drawEllipse(xnew-4,ynew+shift-(ccOverallHeight-ccBaselineHeight)-4,8,8);
                    double xnew1=nodes[i]->draw(painter, xnew, ynew, currentEv, prevNodeSizePtr);
                    i++;
                    //painter.setPen(QPen("magenta"));
                    //painter.drawEllipse(xnew-4,ynew-4,8,8);
                    double xnew2=nodes[i]->draw(painter, xnew, ynew, currentEv, prevNodeSizePtr);
                    //i++;
                    xnew=qMax(xnew1, xnew2);
                    doDraw=false;
                }
            }
        } else if (dynamic_cast<MTsubscriptNode*>(nodes[i])) {
            if (i+1<nodes.size()) { // is there one mor node behind?
                if (dynamic_cast<MTsuperscriptNode*>(nodes[i+1])) { // is this subscript?
                    //painter.setPen(QPen("red"));
                    //painter.drawEllipse(xnew-4,ynew+shift-(ccOverallHeight-ccBaselineHeight)-4,8,8);
                    double xnew1=nodes[i]->draw(painter, xnew, ynew, currentEv, prevNodeSizePtr);
                    i++;
                    //painter.setPen(QPen("magenta"));
                    //painter.drawEllipse(xnew-4,ynew-4,8,8);
                    double xnew2=nodes[i]->draw(painter, xnew, ynew, currentEv, prevNodeSizePtr);
                    //i++;
                    xnew=qMax(xnew1, xnew2);
                    doDraw=false;
                }
            }
        } else {
            if (smb) {
                QString s=smb->getSymbolName();
                if (subsupOperations.contains(s)) {
                    MTsubscriptNode* subn=nullptr;
                    if (i+1<nodes.size()) subn=dynamic_cast<MTsubscriptNode*>(nodes[i+1]);
                    MTsuperscriptNode* supn=nullptr;
                    if (i+2<nodes.size()) supn=dynamic_cast<MTsuperscriptNode*>(nodes[i+2]);
                    //std::cout<<"symbol ='"<<s.toStdString()<<"'   subn="<<subn<<"   supn="<<supn<<"\n";
                    if (subn && supn) { // is this subscript and superscript?
                        MTenvironment ev=currentEv;
                        ev.fontSize=ev.fontSize*parent->getOperatorsubsuperSizeFactor();
                        double w1=0, w2=0, w3=0;
                        double oh1=0, oh2=0, oh3=0;
                        double bh1=0, bh2=0, bh3=0, sp;
                        nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp);
                        subn->getChild()->getSize(painter, ev, w2, bh2, oh2, sp);
                        supn->getChild()->getSize(painter, ev, w3, bh3, oh3, sp);
                        double d1=oh1-bh1;
                        //double d2=oh2-bh2;
                        double d3=oh3-bh3;

                        double w=qMax(qMax(w1, w2), w3);
                        //double xnew1=
                        double xn1=nodes[i]->draw(painter, xnew+(w-w1)/2.0, ynew, currentEv);
                        i++;
                        //double xnew2=
                        double xn2=subn->getChild()->draw(painter, xnew+(w-w2)/2.0, ynew+bh2+d1, ev);
                        i++;
                        //double xnew3=
                        double xn3=supn->getChild()->draw(painter, xnew+(w-w3)/2.0, ynew-bh1-d3-fm.xHeight()/4.0, ev);
                        doDraw=false;
                        xnew=qMax(qMax(xn1, xn2), xn3)+fm.boundingRect(' ').width();
                    } else if (subn) { // is this subscript and not superscript?
                        MTenvironment ev=currentEv;
                        ev.fontSize=ev.fontSize*parent->getOperatorsubsuperSizeFactor();
                        double w1=0, w2=0;
                        double oh1=0, oh2=0;
                        double bh1=0, bh2=0, sp=0;
                        nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp);
                        subn->getChild()->getSize(painter, ev, w2, bh2, oh2, sp);
                        double d1=oh1-bh1;
                        //double d2=oh2-bh2;

                        double w=qMax(w1, w2);
                        //double xnew1=
                        double xn2=nodes[i]->draw(painter, xnew+(w-w1)/2.0, ynew, currentEv);
                        i++;
                        //double xnew2=
                        double xn1=subn->getChild()->draw(painter, xnew+(w-w2)/2.0, ynew+bh2+d1, ev)+fm.boundingRect(' ').width();
                        doDraw=false;
                        //xnew+=w;
                        xnew=qMax(xn1, xn2);
                    } else if (supn) { // is this subscript and superscript?
                        MTenvironment ev=currentEv;
                        ev.fontSize=ev.fontSize*parent->getOperatorsubsuperSizeFactor();
                        double w1=0,  w3=0;
                        double oh1=0,  oh3=0;
                        double bh1=0,  bh3=0, sp;
                        nodes[i]->getSize(painter, currentEv, w1, bh1, oh1, sp);
                        supn->getChild()->getSize(painter, ev, w3, bh3, oh3, sp);
                        //double d1=oh1-bh1;
                        //double d2=oh2-bh2;
                        double d3=oh3-bh3;

                        double w=qMax(w1, w3);
                        //double xnew1=
                        double xn1=nodes[i]->draw(painter, xnew+(w-w1)/2.0, ynew, currentEv);
                        i++;
                        //double xnew3=
                        double xn3=supn->getChild()->draw(painter, xnew+(w-w3)/2.0, ynew-bh1-d3-fm.xHeight()/4.0, ev);
                        doDraw=false;
                        xnew=qMax(xn1, xn3)+fm.boundingRect(' ').width();
                    }
                }
            }
        }

        if (i<nodes.size() && doDraw) {
            xnew=nodes[i]->draw(painter, xnew, ynew, currentEv, prevNodeSizePtr);
        }
        wasBrace=dynamic_cast<MTbraceNode*>(nodes[i]);
    }
    return xnew;
}

bool JKQTMathText::MTlistNode::toHtml(QString &html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) {
    bool ok=true;
    for (int i=0; i<nodes.size(); i++) {
        QString h="";
        ok = ok && nodes[i]->toHtml(h, currentEv, defaultEv);
        html=html+h;
    }
    return ok;
}

void JKQTMathText::MTlistNode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
    for (int i=0; i<nodes.size(); i++) {
        nodes[i]->setDrawBoxes(draw);
    }
}

QList<JKQTMathText::MTnode *> JKQTMathText::MTlistNode::getNodes() const {
    return this->nodes;
}





JKQTMathText::MTsymbolNode::MTsymbolNode(JKQTMathText* _parent, const QString& name, bool _addWhitespace):
    JKQTMathText::MTnode(_parent), symbolName(name), addWhitespace(_addWhitespace)
{
}

JKQTMathText::MTsymbolNode::~MTsymbolNode() = default;

QString JKQTMathText::MTsymbolNode::getTypeName() const
{
    return QLatin1String("MTsymbolNode(")+symbolName+QLatin1String(")");
}

bool JKQTMathText::MTsymbolNode::getWinSymbolProp(JKQTMathText::MTsymbolNode::SymbolProps& props, const QString &n, const MTenvironment& currentEv, double mathFontFactor) const
{
    auto fnt=parent->getFontData(currentEv.font, currentEv.insideMath, FontSubclass::Text);
    auto fntSym=parent->getFontData(currentEv.font, currentEv.insideMath, FontSubclass::Symbols);

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
      else if (n == "flqq") { props.symbol = ""; props.bold = 0; props.italic = 0; }
      else if (n == "frqq") { props.symbol = ""; props.bold = 0; props.italic = 0; }
      else { return false; }
    }

    return true;
}

bool JKQTMathText::MTsymbolNode::getGreekSymbolProp(JKQTMathText::MTsymbolNode::SymbolProps& props, const QString &n, const MTenvironment& currentEv, double mathFontFactor) const
{
    auto fnt=parent->getFontData(currentEv.font, currentEv.insideMath, FontSubclass::Text);
    auto fntGreek=parent->getFontData(currentEv.font, currentEv.insideMath, FontSubclass::Greek);

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


bool JKQTMathText::MTsymbolNode::getStandardTextSymbolProp(JKQTMathText::MTsymbolNode::SymbolProps& props, const QString &n) const
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

bool JKQTMathText::MTsymbolNode::getUnicodeBaseSymbolProp(JKQTMathText::MTsymbolNode::SymbolProps& props, const QString &n) const
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
        else if (n == "flqq") { props.symbol = ""; props.bold = 0; props.italic = 0; }
        else if (n == "frqq") { props.symbol = ""; props.bold = 0; props.italic = 0; }
        else { return false; }
    }
    //qDebug()<<"### found "<<n<<" in unicodeBaseSymbol";
    return true;
}

bool JKQTMathText::MTsymbolNode::getUnicodeFullSymbolProp(JKQTMathText::MTsymbolNode::SymbolProps& props, const QString &n, double mathFontFactor) const
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
    else if (n == "bigcap") { props.symbol = QChar(0x22C2); props.heightIsAscent = true; props.exactAscent = true; props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "bigcup") { props.symbol = QChar(0x22C3); props.heightIsAscent = true; props.exactAscent = true; props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "bigvee") { props.symbol = QChar(0x22C1); props.heightIsAscent = true; props.exactAscent = true; props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "bighat") { props.symbol = QChar(0x22C0); props.heightIsAscent = true; props.exactAscent = true; props.heightIsAscent = true; props.exactAscent = true; }
    else if (n == "int") { props.symbol = QChar(0x222B); props.fontFactor = mathFontFactor; /*yfactor=+0.1;*/ props.heightIsAscent = true; props.exactAscent = true; }
    else {return false;}
    return true;
}

bool JKQTMathText::MTsymbolNode::getSymbolProp(JKQTMathText::MTsymbolNode::SymbolProps& props, const QString &n, const MTenvironment& currentEv, double mathFontFactor) const
{
    auto fnt=parent->getFontData(currentEv.font, currentEv.insideMath, FontSubclass::Text);
    auto fntGreek=parent->getFontData(currentEv.font, currentEv.insideMath, FontSubclass::Greek);
    auto fntSym=parent->getFontData(currentEv.font, currentEv.insideMath, FontSubclass::Symbols);

    //qDebug()<<"--- getSymbolProp("<<n<<"): "<<fnt.first<<", "<<fnt.second;

    props.font = fnt.first;
    if (fnt.second==MTFEunicode) {
        props.font = fnt.first;
        if (!getUnicodeBaseSymbolProp(props, n) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor) && !getUnicodeFullSymbolProp(props,n,mathFontFactor) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
            return false;
        }
    } else if (fnt.second==MTFEunicodeLimited) {
        props.font = fnt.first;
        if (!getUnicodeBaseSymbolProp(props, n) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
            if (fntSym.second==MTFEunicode) {
                props.font = fntSym.first;
                if (!getUnicodeBaseSymbolProp(props, n) && !getUnicodeFullSymbolProp(props,n,mathFontFactor) && !getUnicodeFullSymbolProp(props,n,mathFontFactor) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
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
                if (!getUnicodeBaseSymbolProp(props, n) && !getUnicodeFullSymbolProp(props,n,mathFontFactor) && !getUnicodeFullSymbolProp(props,n,mathFontFactor) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
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
                if (!getUnicodeBaseSymbolProp(props, n) && !getUnicodeFullSymbolProp(props,n,mathFontFactor) && !getUnicodeFullSymbolProp(props,n,mathFontFactor) && !getGreekSymbolProp(props, n, currentEv, mathFontFactor)) {
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

JKQTMathText::MTsymbolNode::SymbolProps JKQTMathText::MTsymbolNode::getSymbolProp(const QString &symName, const MTenvironment& currentEv) const
{

    auto fnt=parent->getFontData(currentEv.font, currentEv.insideMath, FontSubclass::Text);
    auto fntSym=parent->getFontData(currentEv.font, currentEv.insideMath, FontSubclass::Symbols);
    auto fntGreek=parent->getFontData(currentEv.font, currentEv.insideMath, FontSubclass::Greek);

    JKQTMathText::MTsymbolNode::SymbolProps props;
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
                    <<"subset"<<"subseteq"<<"in"<<"notin"<<"cdot"<<"wedge"<<"vee"<<"cong"<<"bot"<<"mid"<<"+"<<"-"<<"|"<<"*"<<"/"<<"<"<<">";
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
            errorExplanation="didn't find symbol given font def:"+fnt.first+"["+encoding2String(fnt.second)+"] / sym:"+fntSym.first+"["+encoding2String(fntSym.second)+"] / grk:"+fntGreek.first+"["+encoding2String(fntGreek.second)+"]";
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
        parent->error_list.append(tr("unknown symbol '%1' found (%2)!").arg(n).arg(errorExplanation));
    }

    //qDebug()<<n<<": '"<<props.symbol<<"' / "<<props.font<<"    {{{ def:"+fnt.first+"["+encoding2String(fnt.second)+"] / sym:"+fntSym.first+"["+encoding2String(fntSym.second)+"] / grk:"+fntGreek.first+"["+encoding2String(fntGreek.second)+"] }}}";
    return props;

}

void JKQTMathText::MTsymbolNode::getSizeInternal(QPainter& painter, JKQTMathText::MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos, const MTnodeSize* /*prevNodeSize*/) {
    QFont f=currentEv.getFont(parent);
    auto props=getSymbolProp(symbolName, currentEv);
    f.setFamily(props.font);
    f.setPointSizeF(f.pointSizeF()*props.fontFactor);
    if (props.italic<0) f.setItalic(false);
    if (props.italic>0) f.setItalic(true);
    if (props.bold<0) f.setBold(false);
    if (props.bold>0) f.setBold(true);
    QFontMetricsF fm(f, painter.device());
    QString symb=props.symbol;
    width=0;
    if (currentEv.insideMath) width=qMax(parent->getTightBoundingRect(f, symb, painter.device()).width(),parent->getTightBoundingRect(f, "i", painter.device()).width());//fm.width(symbol);
    else width=fm.boundingRect(symb).width();//fm.width(symbol);

    width=qMax(fm.boundingRect("j").width(), width);
    if (symb.isEmpty()) {
        width=fm.boundingRect("a").width();
        if (symbolName=="|") width=fm.boundingRect("1").width()*0.8;
        else if (symbolName=="infty") width=fm.boundingRect("M").width();
        else if (symbolName=="quad" || symbolName=="qquad") width=parent->getTightBoundingRect(f, "M", painter.device()).width();
        else if (symbolName==" " || symbolName=="space") width=parent->getTightBoundingRect(f, "x", painter.device()).width();
        else if (symbolName==";") width=parent->getTightBoundingRect(f, "x", painter.device()).width()*0.75;
        else if (symbolName==":") width=parent->getTightBoundingRect(f, "x", painter.device()).width()*0.5;
        else if (symbolName==",") width=parent->getTightBoundingRect(f, "x", painter.device()).width()*0.25;
        else if (symbolName=="!") width=-parent->getTightBoundingRect(f, "x", painter.device()).width()*0.25;
        else if (symbolName=="longleftarrow") { width=parent->getTightBoundingRect(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="longrightarrow") {  width=parent->getTightBoundingRect(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="Longleftarrow") { width=parent->getTightBoundingRect(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="Longrightarrow") { width=parent->getTightBoundingRect(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="longleftrightarrow") { width=parent->getTightBoundingRect(f, "X", painter.device()).width()*3.5; symb="x"; }
        else if (symbolName=="Longleftrightarrow") { width=parent->getTightBoundingRect(f, "X", painter.device()).width()*3.5; symb="x"; }
    }
    QRectF tbr=parent->getTightBoundingRect(f, symb, painter.device());
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

    if (props.extendWidthInMathmode && currentEv.insideMath) width=width*parent->getMathoperatorWidthFactor();

}

double JKQTMathText::MTsymbolNode::draw(QPainter& painter, double x, double y, JKQTMathText::MTenvironment currentEv, const MTnodeSize* /*prevNodeSize*/) {
    doDrawBoxes(painter, x, y, currentEv);
    double width=0;
    double baselineHeight=0;
    double overallHeight=0, strikeoutPos=0;
    getSize(painter, currentEv, width, baselineHeight, overallHeight, strikeoutPos);
    QPen pold=painter.pen();
    QFont fold=painter.font();
    QFont f=currentEv.getFont(parent);
    QFont f1=f;
    auto props=getSymbolProp(symbolName, currentEv);
    f.setFamily(props.font);
    f.setPointSizeF(f.pointSizeF()*props.fontFactor);
    if (props.italic<0) f.setItalic(false);
    if (props.italic>0) f.setItalic(true);
    if (props.bold<0) f.setBold(false);
    if (props.bold>0) f.setBold(true);
    QFontMetricsF fm(f, painter.device());
    QFontMetricsF fm1(f1, painter.device());
    painter.setFont(f);

    double shift=0;
    if (props.extendWidthInMathmode && currentEv.insideMath) {
        double origwidth=width/parent->getMathoperatorWidthFactor();
        shift=0.5*(width-origwidth);
        //width=width*parent->getMathoperatorWidthFactor();
    }

    //std::cout<<"symbol '"<<symbolName.toStdString()<<"' = "<<std::hex<<symbol.at(0).digitValue()<<" in font '"<<f.family().toStdString()<<"' ("<<QFontInfo(f).family().toStdString()<<"): "<<fm.inFont(symbol.at(0))<<std::endl;

    QPen p=painter.pen();
    p.setColor(currentEv.color);
    p.setWidthF(fm.lineWidth());
    p.setStyle(Qt::SolidLine);
    painter.setPen(p);
    double xwi=fm.boundingRect("x").width();
    if (!props.symbol.isEmpty()) {
        // if the symbol has been recognized in the constructor: draw the symbol
        painter.drawText(QPointF(x+shift, y+props.yfactor*overallHeight), props.symbol);
        double xx=x+shift;
        double yy=y-fm.xHeight()-(parent->getTightBoundingRect(f, "M", painter.device()).height()-fm.xHeight())/3.0;
        QLineF l(xx, yy, xx+xwi/3.0+((currentEv.italic)?(xwi/3.0):0), yy);
        if (props.drawBar&&l.length()>0) painter.drawLine(l);

    // try to draw some often used special symbols, by synthesizing them from
    // standard characters in the current drawing font
    } else if (symbolName=="infty") {
        //std::cout<<"draw infty\n";
        f1.setItalic(false);
        painter.setFont(f1);
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
        painter.translate(x+shift+fm1.boundingRect("8").width()/3.0, y-fm1.xHeight());
        painter.rotate(90);
        painter.drawText(QPointF(0,0), "8");

    } else if (symbolName=="|") {
        //std::cout<<"draw infty\n";
        f1.setItalic(false);
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
        double width=parent->getTightBoundingRect(f, "X", painter.device()).width()*3.0;
        double dx=parent->getTightBoundingRect(f, "X", painter.device()).width()*0.25;
        double ypos=y-parent->getTightBoundingRect(f, "x", painter.device()).height()/2.0;
        QPainterPath path=makeArrow(x+shift+dx, ypos, width, parent->getTightBoundingRect(f, "M", painter.device()).height()*0.5, true, false);
        painter.drawPath(path);
    } else if (symbolName=="longrightarrow"){
        double width=parent->getTightBoundingRect(f, "X", painter.device()).width()*3.0;
        double dx=parent->getTightBoundingRect(f, "X", painter.device()).width()*0.25;
        double ypos=y-parent->getTightBoundingRect(f, "x", painter.device()).height()/2.0;
        QPainterPath path=makeArrow(x+shift+dx, ypos, width, parent->getTightBoundingRect(f, "M", painter.device()).height()*0.5, false, true);
        painter.drawPath(path);
    } else if (symbolName=="Longleftarrow") {
        double width=parent->getTightBoundingRect(f, "X", painter.device()).width()*3.0;
        double dx=parent->getTightBoundingRect(f, "X", painter.device()).width()*0.25;
        double ypos=y-parent->getTightBoundingRect(f, "x", painter.device()).height()/2.0;
        QPainterPath path=makeDArrow(x+shift+dx, ypos, width, parent->getTightBoundingRect(f, "M", painter.device()).height()*0.5, true, false);
        painter.drawPath(path);
    } else if (symbolName=="Longrightarrow") {
        double width=parent->getTightBoundingRect(f, "X", painter.device()).width()*3.0;
        double dx=parent->getTightBoundingRect(f, "X", painter.device()).width()*0.25;
        double ypos=y-parent->getTightBoundingRect(f, "x", painter.device()).height()/2.0;
        QPainterPath path=makeDArrow(x+shift+dx, ypos, width, parent->getTightBoundingRect(f, "M", painter.device()).height()*0.5, false, true);
        painter.drawPath(path);
    } else if (symbolName=="longleftrightarrow") {
        double width=parent->getTightBoundingRect(f, "X", painter.device()).width()*3.0;
        double dx=parent->getTightBoundingRect(f, "X", painter.device()).width()*0.25;
        double ypos=y-parent->getTightBoundingRect(f, "x", painter.device()).height()/2.0;
        QPainterPath path=makeArrow(x+shift+dx, ypos, width, parent->getTightBoundingRect(f, "M", painter.device()).height()*0.5, true, true);
        painter.drawPath(path);
    } else if (symbolName=="Longleftrightarrow") {
        double width=parent->getTightBoundingRect(f, "X", painter.device()).width()*3.0;
        double dx=parent->getTightBoundingRect(f, "X", painter.device()).width()*0.25;
        double ypos=y-parent->getTightBoundingRect(f, "x", painter.device()).height()/2.0;
        QPainterPath path=makeDArrow(x+shift+dx, ypos, width, parent->getTightBoundingRect(f, "M", painter.device()).height()*0.5, true, true);
        painter.drawPath(path);
    } else { // draw a box to indicate an unavailable symbol
        QRectF tbr=parent->getTightBoundingRect(f, "M", painter.device());
        painter.drawRect(QRectF(x+shift,y-tbr.height(), xwi, tbr.height()*0.8));
    }
    painter.setPen(pold);
    painter.setFont(fold);
    return x+width;
}

bool JKQTMathText::MTsymbolNode::toHtml(QString &html, JKQTMathText::MTenvironment currentEv, JKQTMathText::MTenvironment defaultEv) {
    bool ok=true;
    QString s;
    JKQTMathText::MTenvironment ev=currentEv;

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

QString JKQTMathText::MTsymbolNode::getSymbolName() const {
    return this->symbolName;
}

QString JKQTMathText::MTsymbolNode::getSymbolfontName() const {
    MTenvironment currentEv;
    auto props=getSymbolProp(symbolName, currentEv);
    return props.font;
}

bool JKQTMathText::MTsymbolNode::getAddWhitespace() const
{
    return addWhitespace;
}


// --------------------------------------------------------------------------------------------------
// -- implementation of the JKQTMathText methods
// --------------------------------------------------------------------------------------------------
JKQTMathText::JKQTMathText(QObject* parent):
    QObject(parent)
{
    //std::chrono::high_resolution_clock::time_point t0=std::chrono::high_resolution_clock::now();
    initJKQTMathTextResources();
    //qDebug()<<"init_resoucre: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms"; t0=std::chrono::high_resolution_clock::now();

    fontSize=10;
    brace_factor=1.04;
    subsuper_size_factor=0.7;
    italic_correction_factor=0.4;
    sub_shift_factor=0.4;
    super_shift_factor=0.6;
    brace_shrink_factor=0.6;
    fontColor=QColor("black");

    frac_factor=0.9;
    frac_shift_factor=0.4;
    underbrace_factor=0.75;
    undersetFactor=0.7;
    decoration_height_factor=0.2;
    decoration_width_reduction_Xfactor=0.2;
    brace_y_shift_factor=0.7;//-1;
    operatorsubsuper_size_factor=0.65;
    mathoperator_width_factor=1.5;

    expensiveRendering=true;
    blackboardSimulated=true;


    static QString serifFont="serif";
    static QString sansFont="sans";
    static QString symbolFont="symbol";
    static QString scriptFont="script";
    static QString typewriterFont="typewriter";
    static QString decorativeFont="decorative";
    static QString blackboardFont="blackboard";
    static QString fracturFont="fraktur";
    static bool firstStart=true;

    if (firstStart) {
        //t0=std::chrono::high_resolution_clock::now();

        firstStart=false;
#if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
        QFontDatabase fdb;
        const auto fonts=fdb.families();
#else
        const auto fonts=QFontDatabase::families();
#endif
        //qDebug()<<"fonts:\n"<<fonts;

        /*if (SCAN_FONTS_ON_STARTUP) {
            for (const QString& f: fonts) {
                QFont fnt(f);
                QFontInfo fi(fnt);
                if (typewriterFont=="typewriter" && fi.styleHint()==QFont::TypeWriter) {
                    typewriterFont=f;
                }
                if (decorativeFont=="decorative" && fi.styleHint()==QFont::Decorative) {
                    decorativeFont=f;
                }
                if (serifFont=="serif" && fi.styleHint()==QFont::Serif) {
                    serifFont=f;
                }
                if (sansFont=="sans" && fi.styleHint()==QFont::SansSerif) {
                    sansFont=f;
                }
                if (scriptFont=="script" && fi.styleHint()==QFont::Cursive) {
                    scriptFont=f;
                }
            }
        }*/
        //qDebug()<<"iterate "<<fonts.size()<<" fonts: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms"; t0=std::chrono::high_resolution_clock::now();


        auto checkForFonts=[&fonts](QString& targetfont, const QStringList& fontoptions) {
            for (auto& f: fontoptions) {
                if (fonts.contains(f)) {
                    targetfont=f;
                    break;
                }
            }
        };

        checkForFonts(serifFont, QStringList {"Times New Roman", "Times", "FreeSerif", "DejaVu Serif"});
        //qDebug()<<"check 1st font: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms";

        checkForFonts(sansFont, QStringList {"Arial Unicode MS", "Arial Unicode", "Lucida Sans Unicode", "Arial", "Helvetica", "FreeSans", "DejaVu Sans", "Lucida Sans"});
        checkForFonts(symbolFont, QStringList {"SymbolStandard", "Symbol"});
        checkForFonts(typewriterFont, QStringList {"Courier New", "Courier", "Courier Std", "FreeMono", "CMU Typewriter Text", "UM Typewriter"});
        checkForFonts(blackboardFont, QStringList {"Double Stroke", "CloisterOpenFace BT", "GoudyHandtooled BT", "Castellar", "MathJax_AMS", "Castellar Standard", "MathJax_AMS Standard", "Colonna MT"});
        checkForFonts(decorativeFont, QStringList {"Lucida Calligraphy", "Cookie", "Segoe Print", "Comic Sans", "Comic Sans MS", "Gabriola", "Gabriola Standard", "Lucida Handwriting Kursiv", "Lucida Handwriting", "Pristina", "Pristina Standard", "MathJax_Caligraphics"});
        checkForFonts(scriptFont, QStringList {"Lucida Handwriting", "Dancing Script", "Amazone BT", "ScriptS", "ScriptC", "ScriptC Standard", "Script", "Brush Script MT", "Brush Script MT Kursiv", "MathJax_Script"});
        checkForFonts(fracturFont, QStringList {"Old English Text MT", "Old English Text MT Standard", "UnifrakturMaguntia Standard", "UnifrakturMaguntia", "MathJax_Fraktur", "UnifrakturCook Fett"});
        //qDebug()<<"check all font: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms";
    }

    //t0=std::chrono::high_resolution_clock::now();
    if (serifFont!="serif") addReplacementFont("serif", serifFont);
    if (sansFont!="sans") addReplacementFont("sans", sansFont);
    if (symbolFont!="symbol") addReplacementFont("symbol", symbolFont);
    if (scriptFont!="script") addReplacementFont("script", scriptFont);
    if (typewriterFont!="typewriter") addReplacementFont("typewriter", typewriterFont);
    if (decorativeFont!="decorative") addReplacementFont("decorative", decorativeFont);
    if (fracturFont!="fraktur") addReplacementFont("fraktur", fracturFont);
    if (blackboardFont!="blackboard") {
        addReplacementFont("blackboard", blackboardFont);
    }
    //qDebug()<<"add replacement fonts: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms"; t0=std::chrono::high_resolution_clock::now();
    setFontSans(sansFont, MTFEStandard);
    setFontMathSans(sansFont, MTFEStandard);
    setFontTypewriter(typewriterFont, MTFEStandard);
    setFontRoman(serifFont, MTFEStandard);
    setFontMathRoman(serifFont, MTFEStandard);
    setFontCaligraphic(decorativeFont, MTFEStandard);
    setFontBlackboard(blackboardFont, MTFEStandard);
    setFontBlackboardSimulated(blackboardFont=="blackboard");
    setFontScript(scriptFont, MTFEStandard);
    setFontFraktur(fracturFont, MTFEStandard);
    //qDebug()<<"set fonts: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms"; t0=std::chrono::high_resolution_clock::now();
    useXITS();
    //qDebug()<<"useXITS: "<<std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()-t0).count()/1000.0<<"ms"; t0=std::chrono::high_resolution_clock::now();
    useUnparsed=false;

    parsedNode=nullptr;
    unparsedNode=nullptr;

    currentToken=MTTnone;
    currentTokenName="";
    currentTokenID=0;
    parseString="";
    parsingMathEnvironment=false;
}

JKQTMathText::~JKQTMathText() {
    if (parsedNode!=nullptr) delete parsedNode;
    parsedNode=nullptr;
    if (unparsedNode!=nullptr) delete unparsedNode;
    unparsedNode=nullptr;
}

void JKQTMathText::loadSettings(const QSettings& settings, const QString& group){
    fontSize=settings.value(group+"font_size", fontSize).toDouble();
    fontColor=jkqtp_String2QColor(settings.value(group+"font_color", jkqtp_QColor2String(fontColor)).toString());
    brace_factor=settings.value(group+"brace_factor", brace_factor).toDouble();
    brace_shrink_factor=settings.value(group+"brace_shrink_factor", brace_shrink_factor).toDouble();
    subsuper_size_factor=settings.value(group+"subsuper_size_factor", subsuper_size_factor).toDouble();
    italic_correction_factor=settings.value(group+"italic_correction_factor", italic_correction_factor).toDouble();
    super_shift_factor=settings.value(group+"super_shift_factor", super_shift_factor).toDouble();
    sub_shift_factor=settings.value(group+"sub_shift_factor", sub_shift_factor).toDouble();
    frac_factor=settings.value(group+"frac_factor", frac_factor).toDouble();
    frac_shift_factor=settings.value(group+"frac_shift_factor", frac_shift_factor).toDouble();
    underbrace_factor=settings.value(group+"underbrace_factor", underbrace_factor).toDouble();
    undersetFactor=settings.value(group+"undersetFactor", undersetFactor).toDouble();
    brace_y_shift_factor=settings.value(group+"brace_y_shift_factor", brace_y_shift_factor).toDouble();
    decoration_height_factor=settings.value(group+"decoration_height_factor", decoration_height_factor).toDouble();
    decoration_width_reduction_Xfactor=settings.value(group+"decoration_width_reduction_xfactor", decoration_width_reduction_Xfactor).toDouble();
    operatorsubsuper_size_factor=settings.value(group+"operatorsubsuper_size_factor", operatorsubsuper_size_factor).toDouble();
    mathoperator_width_factor=settings.value(group+"mathoperator_width_factor", mathoperator_width_factor).toDouble();


    if (settings.value(group+"use_stix_fonts", false).toBool()) useSTIX();
    if (settings.value(group+"use_xits_fonts", false).toBool()) useXITS();
    if (settings.value(group+"use_asana_fonts", false).toBool()) useASANA();
}


void JKQTMathText::saveSettings(QSettings& settings, const QString& group) const{
    settings.setValue(group+"font_size", fontSize);
    settings.setValue(group+"font_color", jkqtp_QColor2String(fontColor));
    settings.setValue(group+ "brace_factor", brace_factor);
    settings.setValue(group+ "brace_shrink_factor", brace_shrink_factor);
    settings.setValue(group+ "subsuper_size_factor", subsuper_size_factor);
    settings.setValue(group+ "italic_correction_factor", italic_correction_factor);
    settings.setValue(group+ "sub_shift_factor", sub_shift_factor);
    settings.setValue(group+ "super_shift_factor", super_shift_factor);
    settings.setValue(group+ "frac_factor", frac_factor);
    settings.setValue(group+ "frac_shift_factor", frac_shift_factor);
    settings.setValue(group+ "underbrace_factor", underbrace_factor);
    settings.setValue(group+ "undersetFactor", undersetFactor);
    settings.setValue(group+ "operatorsubsuper_size_factor", operatorsubsuper_size_factor);
    settings.setValue(group+ "mathoperator_width_factor", mathoperator_width_factor);
    settings.setValue(group+ "brace_y_shift_factor", brace_y_shift_factor);
    settings.setValue(group+ "decoration_height_factor", decoration_height_factor);
    settings.setValue(group+ "decoration_width_reduction_xfactor", decoration_width_reduction_Xfactor);
}

bool JKQTMathText::useSTIX(bool mathModeOnly) {

    const JKQTMathTextFontSpecifier xits=JKQTMathTextFontSpecifier::getSTIXFamilies();
    bool res=false;

    if (!mathModeOnly && !xits.fontName().isEmpty()) {
        setFontRoman(xits.fontName(), MTFEunicode);
        res=true;
    }
    if (!xits.mathFontName().isEmpty()) {
        setFontMathRoman(xits.mathFontName(), MTFEunicode);
        res=true;
    } else if (!xits.fontName().isEmpty()) {
        setFontMathRoman(xits.fontName(), MTFEunicode);
        res=true;
    }

    brace_shrink_factor=0.6;
    return res;
}

bool JKQTMathText::useXITS(bool mathModeOnly)
{

    const JKQTMathTextFontSpecifier xits=JKQTMathTextFontSpecifier::getXITSFamilies();
    bool res=false;

    if (!mathModeOnly && !xits.fontName().isEmpty()) {
        setFontRoman(xits.fontName(), MTFEunicode);
        setSymbolfontSymbol(xits.fontName(), MTFEunicode);
        setSymbolfontGreek(xits.fontName(), MTFEunicode);
        brace_shrink_factor=0.6;
        res=true;
    }
    if (!xits.mathFontName().isEmpty()) {
        setFontMathRoman(xits.mathFontName(), MTFEunicode);
        setSymbolfontSymbol(xits.fontName(), MTFEunicode);
        setSymbolfontGreek(xits.fontName(), MTFEunicode);
        brace_shrink_factor=0.6;
        res=true;
    }

    return res;
}

bool JKQTMathText::useASANA(bool mathModeOnly)
{

    const JKQTMathTextFontSpecifier asana=JKQTMathTextFontSpecifier::getXITSFamilies();
    bool res=false;

    if (!mathModeOnly && !asana.fontName().isEmpty()) {
        setFontRoman(asana.fontName(), MTFEunicode);
        res=true;
    }
    if (!asana.mathFontName().isEmpty()) {
        setFontMathRoman(asana.mathFontName(), MTFEunicode);
        res=true;
    }

    brace_shrink_factor=0.6;
    return res;
}

void JKQTMathText::useAnyUnicode(QString timesFont, const QString &sansFont, JKQTMathText::MTfontEncoding encodingTimes, JKQTMathText::MTfontEncoding encodingSans)
{
    if (!timesFont.isEmpty()) { setFontRoman(timesFont, encodingTimes); }
    if (!sansFont.isEmpty()) { setFontSans(sansFont, encodingSans); }
    brace_shrink_factor=0.6;
}


QString JKQTMathText::toHtml(bool *ok, double fontPointSize) {
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

QString JKQTMathText::encoding2String(JKQTMathText::MTfontEncoding e)
{
    switch(e) {
        case MTFEunicode: return "MTFEunicode";
        case MTFEStandard: return "MTFEStandard";
        case MTFEunicodeLimited: return "MTFEunicodeLimited";
        case MTFEwinSymbol: return "MTFEwinSymbol";
    }
    return "???";
}

void JKQTMathText::setFontColor(const QColor &__value)
{
    this->fontColor = __value;
}

QColor JKQTMathText::getFontColor() const
{
    return this->fontColor;
}

void JKQTMathText::setFontSize(double __value)
{
    this->fontSize = __value;
}

double JKQTMathText::getFontSize() const
{
    return this->fontSize;
}

void JKQTMathText::addReplacementFont(const QString &nonUseFont, const QString &useFont, MTfontEncoding useFontEncoding) {
    fontReplacements.insert(nonUseFont, useFont);
    fontEncodingReplacements.insert(nonUseFont, useFontEncoding);
}

void JKQTMathText::addReplacementFont(const QString &nonUseFont, const QString &useFont) {
    fontReplacements.insert(nonUseFont, useFont);
    auto it=fontEncodingReplacements.find(nonUseFont);
    if (it!=fontEncodingReplacements.end()) fontEncodingReplacements.erase(it);
}

QPair<QString,JKQTMathText::MTfontEncoding> JKQTMathText::getReplacementFont(const QString &nonUseFont, const QString &defaultFont, JKQTMathText::MTfontEncoding defaultFontEncoding) const {
    QPair<QString,MTfontEncoding> res(defaultFont, defaultFontEncoding);
    bool foundFont=false;
    for (auto it=fontReplacements.begin(); it!=fontReplacements.end(); ++it) {
        if (it.key().toLower()==nonUseFont.toLower()) {
            foundFont=true;
            res.first=it.value();
            res.second=fontEncodingReplacements.value(res.first, res.second);
            return res;
        }
    }
    return res;
}

QPair<QString, JKQTMathText::MTfontEncoding> JKQTMathText::getFontData(JKQTMathText::MTenvironmentFont font, bool in_math_environment, FontSubclass subclass) const
{
    if (in_math_environment) {
        if (font==MTEroman) font=MTEmathRoman;
        if (font==MTEsans) font=MTEmathSans;
    }
    const auto fd=fontDefinitions.value(font);
    if (subclass==FontSubclass::Greek) return QPair<QString, JKQTMathText::MTfontEncoding>(fd.symbolfontGreek, fd.symbolfontGreekEncoding);
    if (subclass==FontSubclass::Symbols) return QPair<QString, JKQTMathText::MTfontEncoding>(fd.symbolfontSymbol, fd.symbolfontSymbolEncoding);
    return QPair<QString, JKQTMathText::MTfontEncoding>(fd.fontName, fd.fontEncoding);
}


void JKQTMathText::setFontRomanOrSpecial(const QString &__value, MTfontEncoding encoding)
{
    setFontRomanOrSpecial(JKQTMathTextFontSpecifier::fromFontSpec(__value), encoding);
}

void JKQTMathText::setFontRomanOrSpecial(const JKQTMathTextFontSpecifier &fontName, MTfontEncoding encoding)
{
    if (!fontName.hasMathFontName()) {
        if (fontName.fontName().toUpper()=="XITS") useXITS(false);
        else if (fontName.fontName().toUpper()=="STIX") useSTIX(false);
        else if (fontName.fontName().toUpper()=="ASANA") useASANA(false);
        else {
            setFontRoman(fontName.fontName(), encoding);
            setFontMathRoman(fontName.fontName(), encoding);
        }
    } else {
        if (fontName.mathFontName().toUpper()=="XITS") useXITS(true);
        else if (fontName.mathFontName().toUpper()=="STIX") useSTIX(true);
        else if (fontName.mathFontName().toUpper()=="ASANA") useASANA(true);
        else setFontMathRoman(fontName.mathFontName(), encoding);
        setFontRoman(fontName.fontName(), encoding);
    }
}

void JKQTMathText::setFontRoman(const QString &__value, MTfontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEroman].fontName = f.first;
    fontDefinitions[MTEroman].fontEncoding = f.second;
}

QString JKQTMathText::getFontRoman() const
{
    return fontDefinitions[MTEroman].fontName;
}

JKQTMathText::MTfontEncoding JKQTMathText::getFontEncodingRoman() const
{
    return fontDefinitions[MTEroman].fontEncoding;
}

void JKQTMathText::setFontSans(const QString &__value, MTfontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEsans].fontName = f.first;
    fontDefinitions[MTEsans].fontEncoding = f.second;
}

QString JKQTMathText::getFontSans() const
{
    return fontDefinitions[MTEsans].fontName;
}

JKQTMathText::MTfontEncoding JKQTMathText::getFontEncodingSans() const
{
    return fontDefinitions[MTEsans].fontEncoding;
}

void JKQTMathText::setFontTypewriter(const QString &__value, MTfontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEtypewriter].fontName = f.first;
    fontDefinitions[MTEtypewriter].fontEncoding = f.second;
}

QString JKQTMathText::getFontTypewriter() const
{
    return fontDefinitions[MTEtypewriter].fontName;
}

JKQTMathText::MTfontEncoding JKQTMathText::getFontEncodingTypewriter() const
{
    return fontDefinitions[MTEtypewriter].fontEncoding;
}

void JKQTMathText::setFontScript(const QString &__value, MTfontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEscript].fontName = f.first;
    fontDefinitions[MTEscript].fontEncoding = f.second;
}

QString JKQTMathText::getFontScript() const
{
    return fontDefinitions[MTEscript].fontName;
}

JKQTMathText::MTfontEncoding JKQTMathText::getFontEncodingScript() const
{
    return fontDefinitions[MTEscript].fontEncoding;
}

void JKQTMathText::setFontFraktur(const QString &__value, MTfontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEfraktur].fontName = f.first;
    fontDefinitions[MTEfraktur].fontEncoding = f.second;
}

QString JKQTMathText::getFontFraktur() const
{
    return fontDefinitions[MTEfraktur].fontName;
}

JKQTMathText::MTfontEncoding JKQTMathText::getFontEncodingFraktur() const
{
    return fontDefinitions[MTEfraktur].fontEncoding;
}
void JKQTMathText::setSymbolfontGreek(MTenvironmentFont font, const QString &__value, MTfontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[font].symbolfontGreek = f.first;
    fontDefinitions[font].symbolfontGreekEncoding = f.second;
}

void JKQTMathText::setSymbolfontGreek(const QString &fontName, JKQTMathText::MTfontEncoding encoding)
{
    for (int f=0; f<static_cast<int>(MTenvironmentFontCount); f++) {
        setSymbolfontGreek(static_cast<MTenvironmentFont>(f), fontName, encoding);
    }
}

QString JKQTMathText::getSymbolfontGreek(MTenvironmentFont font) const
{
    return fontDefinitions[font].symbolfontGreek;
}

JKQTMathText::MTfontEncoding JKQTMathText::getSymbolfontEncodingGreek(MTenvironmentFont font) const
{
    return fontDefinitions[font].symbolfontGreekEncoding;
}

void JKQTMathText::setSymbolfontSymbol(MTenvironmentFont font, const QString &__value, MTfontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[font].symbolfontSymbol = f.first;
    fontDefinitions[font].symbolfontSymbolEncoding = f.second;
}

void JKQTMathText::setSymbolfontSymbol(const QString &fontName, JKQTMathText::MTfontEncoding encoding)
{
    for (int f=0; f<static_cast<int>(MTenvironmentFontCount); f++) {
        setSymbolfontSymbol(static_cast<MTenvironmentFont>(f), fontName, encoding);
    }
}

QString JKQTMathText::getSymbolfontSymbol(MTenvironmentFont font) const
{
    return fontDefinitions[font].symbolfontSymbol;
}

JKQTMathText::MTfontEncoding JKQTMathText::getSymbolfontEncodingSymbol(MTenvironmentFont font) const
{
    return fontDefinitions[font].symbolfontSymbolEncoding;
}

void JKQTMathText::setFontCaligraphic(const QString &__value, MTfontEncoding encoding)
{
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEcaligraphic].fontName = f.first;
    fontDefinitions[MTEcaligraphic].fontEncoding = f.second;
}

QString JKQTMathText::getFontCaligraphic() const
{
    return fontDefinitions[MTEcaligraphic].fontName;
}

JKQTMathText::MTfontEncoding JKQTMathText::getFontEncodingCaligraphic() const
{
    return fontDefinitions[MTEcaligraphic].fontEncoding;
}

void JKQTMathText::setFontMathRoman(const QString &fontName, JKQTMathText::MTfontEncoding encoding)
{
    auto f=getReplacementFont(fontName, fontName, encoding);
    fontDefinitions[MTEmathRoman].fontName = f.first;
    fontDefinitions[MTEmathRoman].fontEncoding = f.second;
}

QString JKQTMathText::getFontMathRoman() const
{
    return fontDefinitions[MTEmathRoman].fontName;
}

void JKQTMathText::setFontMathSans(const QString &fontName, JKQTMathText::MTfontEncoding encoding)
{
    auto f=getReplacementFont(fontName, fontName, encoding);
    fontDefinitions[MTEmathSans].fontName = f.first;
    fontDefinitions[MTEmathSans].fontEncoding = f.second;
}

QString JKQTMathText::getFontMathSans() const
{
    return fontDefinitions[MTEmathSans].fontName;
}

JKQTMathText::MTfontEncoding JKQTMathText::getFontEncodingMathSans() const
{
    return fontDefinitions[MTEmathSans].fontEncoding;
}

JKQTMathText::MTfontEncoding JKQTMathText::getFontEncodingMathRoman() const
{
    return fontDefinitions[MTEmathRoman].fontEncoding;
}

void JKQTMathText::setFontBlackboard(const QString &__value, MTfontEncoding encoding)
{
    blackboardSimulated=false;
    auto f=getReplacementFont(__value, __value, encoding);
    fontDefinitions[MTEblackboard].fontName = f.first;
    fontDefinitions[MTEblackboard].fontEncoding = f.second;
}

void JKQTMathText::setFontBlackboardSimulated(bool doSimulate)
{
    blackboardSimulated=doSimulate;
}

bool JKQTMathText::isFontBlackboardSimulated() const
{
    return blackboardSimulated;
}

QString JKQTMathText::getFontBlackboard() const
{
    return fontDefinitions[MTEblackboard].fontName;
}

JKQTMathText::MTfontEncoding JKQTMathText::getFontEncodingBlackboard() const
{
    return fontDefinitions[MTEblackboard].fontEncoding;
}


void JKQTMathText::setBraceFactor(double __value)
{
    this->brace_factor = __value;
}

double JKQTMathText::getBraceFactor() const
{
    return this->brace_factor;
}

void JKQTMathText::setSubsuperSizeFactor(double __value)
{
    this->subsuper_size_factor = __value;
}

double JKQTMathText::getSubsuperSizeFactor() const
{
    return this->subsuper_size_factor;
}

void JKQTMathText::setItalicCorrectionFactor(double __value)
{
    this->italic_correction_factor = __value;
}

double JKQTMathText::getItalicCorrectionFactor() const
{
    return this->italic_correction_factor;
}

void JKQTMathText::setOperatorsubsuperSizeFactor(double __value)
{
    this->operatorsubsuper_size_factor = __value;
}

double JKQTMathText::getOperatorsubsuperSizeFactor() const
{
    return this->operatorsubsuper_size_factor;
}

void JKQTMathText::setMathoperatorWidthFactor(double __value)
{
    this->mathoperator_width_factor = __value;
}

double JKQTMathText::getMathoperatorWidthFactor() const
{
    return this->mathoperator_width_factor;
}

void JKQTMathText::setSuperShiftFactor(double __value)
{
    this->super_shift_factor = __value;
}

double JKQTMathText::getSuperShiftFactor() const
{
    return this->super_shift_factor;
}

void JKQTMathText::setSubShiftFactor(double __value)
{
    this->sub_shift_factor = __value;
}

double JKQTMathText::getSubShiftFactor() const
{
    return this->sub_shift_factor;
}

void JKQTMathText::setBraceShrinkFactor(double __value)
{
    this->brace_shrink_factor = __value;
}

double JKQTMathText::getBraceShrinkFactor() const
{
    return this->brace_shrink_factor;
}

void JKQTMathText::setUnderbraceFactor(double __value)
{
    this->underbrace_factor = __value;
}

double JKQTMathText::getUnderbraceFactor() const
{
    return this->underbrace_factor;
}

void JKQTMathText::setUndersetFactor(double __value)
{
    this->undersetFactor = __value;
}

double JKQTMathText::getUndersetFactor() const
{
    return this->undersetFactor;
}

void JKQTMathText::setFracFactor(double __value)
{
    this->frac_factor = __value;
}

double JKQTMathText::getFracFactor() const
{
    return this->frac_factor;
}

void JKQTMathText::setFracShiftFactor(double __value)
{
    this->frac_shift_factor = __value;
}

double JKQTMathText::getFracShiftFactor() const
{
    return this->frac_shift_factor;
}

void JKQTMathText::setBraceYShiftFactor(double __value)
{
    this->brace_y_shift_factor = __value;
}

double JKQTMathText::getBraceYShiftFactor() const
{
    return this->brace_y_shift_factor;
}

void JKQTMathText::setDecorationHeightFactor(double __value)
{
    this->decoration_height_factor = __value;
}

double JKQTMathText::getDecorationHeightFactor() const
{
    return this->decoration_height_factor;
}

void JKQTMathText::setDecorationWidthReductionXFactor(double __value)
{
    decoration_width_reduction_Xfactor=__value;
}

double JKQTMathText::getDecorationWidthReductionXFactor() const
{
    return decoration_width_reduction_Xfactor;
}

void JKQTMathText::setExpensiveRendering(bool __value)
{
    this->expensiveRendering = __value;
}

bool JKQTMathText::getExpensiveRendering() const
{
    return this->expensiveRendering;
}

void JKQTMathText::setUseUnparsed(bool __value)
{
    this->useUnparsed = __value;
}

bool JKQTMathText::isUsingUnparsed() const
{
    return this->useUnparsed;
}

QStringList JKQTMathText::getErrorList() const {
    return this->error_list;
}



JKQTMathText::tokenType JKQTMathText::getToken() {
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
            static QSet<QChar> mathEnvironmentSpecialChars, mathEnvironmentSpecialEndChars;
            if (mathEnvironmentSpecialChars.size()==0) {
                mathEnvironmentSpecialChars<<'(' << '[' << '|' << ')' << ']' << '+' << '-' << '*' << '/' << '<' << '>' << '=';
                mathEnvironmentSpecialEndChars<<'(' << '&' << '[' << '|' << ')' << ']' << '\\' << '$' << '{' << '}' << '_' << '^' << '+' << '-' << '/' << '*' << '=' << '<' << '>';
            }
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
                if (currentTokenID<parseString.size())c=parseString[currentTokenID];
            }
            if (mathEnvironmentSpecialEndChars.contains(c) || c.isSpace()) currentTokenID--;
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

JKQTMathText::MTnode* JKQTMathText::parseLatexString(bool get, const QString& quitOnClosingBrace, const QString& quitOnEnvironmentEnd) {
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
            if (parsingMathEnvironment) {
                static QSet<QString> mathEnvironmentSpecialText;
                if (mathEnvironmentSpecialText.size()==0) {
                    mathEnvironmentSpecialText<<"+"<<"-"<<"="<<"*"<<"<"<<">"<<"|"<<"/";
                }
                if (mathEnvironmentSpecialText.contains(text.trimmed())) {
                    nl->addNode(new MTsymbolNode(this, text, addWhite));
                } else {
                    nl->addNode(new MTtextNode(this, text, addWhite, parsingMathEnvironment));
                }
            } else {
                nl->addNode(new MTtextNode(this, text, addWhite, parsingMathEnvironment));
            }
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
                } else if (name=="sfrac" || name=="slantfrac" || name=="xfrac") {
                    MTnode* n1=parseLatexString(true);
                    MTnode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addNode(new MTfracNode(this, n1, n2, MTFMsfrac));
                    else error_list.append(tr("error @ ch. %1: expected two arguments in '{' braces after '%2' command").arg(currentTokenID).arg(name));
                } else if (name=="stfrac" || name=="nicefrac" || name=="slanttextfrac" || name=="xtfrac") {
                    MTnode* n1=parseLatexString(true);
                    MTnode* n2=nullptr;
                    if (getToken()==MTTopenbrace) n2=parseLatexString(true);
                    if (n1 && n2) nl->addNode(new MTfracNode(this, n1, n2, MTFMstfrac));
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
                } else if (name=="arrow"||name=="overrightarrow"||name=="overarrow") {
                    nl->addNode(new MTdecoratedNode(this, MTDarrow, parseLatexString(true)));
                } else if (name=="hat" || name=="^") {
                    nl->addNode(new MTdecoratedNode(this, MTDhat, parseLatexString(true)));
                } else if (name=="widehat") {
                    nl->addNode(new MTdecoratedNode(this, MTDwidehat, parseLatexString(true)));
                } else if (name=="check" || name=="v") {
                    nl->addNode(new MTdecoratedNode(this, MTDcheck, parseLatexString(true)));
                } else if (name=="widecheck") {
                    nl->addNode(new MTdecoratedNode(this, MTDwidecheck, parseLatexString(true)));
                } else if (name=="bar") {
                    nl->addNode(new MTdecoratedNode(this, MTDbar, parseLatexString(true)));
                } else if (name=="dot" || name==".") {
                    nl->addNode(new MTdecoratedNode(this, MTDdot, parseLatexString(true)));
                } else if (name=="ocirc") {
                    nl->addNode(new MTdecoratedNode(this, MTDocirc, parseLatexString(true)));
                } else if (name=="tilde" || name=="~") {
                    nl->addNode(new MTdecoratedNode(this, MTDtilde, parseLatexString(true)));
                } else if (name=="breve" || name=="u") {
                    nl->addNode(new MTdecoratedNode(this, MTDbreve, parseLatexString(true)));
                } else if (name=="widetilde") {
                    nl->addNode(new MTdecoratedNode(this, MTDwidetilde, parseLatexString(true)));
                } else if (name=="ddot") {
                    nl->addNode(new MTdecoratedNode(this, MTDddot, parseLatexString(true)));
                } else if (name=="cancel") {
                    nl->addNode(new MTdecoratedNode(this, MTDcancel, parseLatexString(true)));
                } else if (name=="xcancel") {
                    nl->addNode(new MTdecoratedNode(this, MTDxcancel, parseLatexString(true)));
                } else if (name=="bcancel") {
                    nl->addNode(new MTdecoratedNode(this, MTDbcancel, parseLatexString(true)));
                } else if (name=="strike" || name=="st" || name=="sout") {
                    nl->addNode(new MTdecoratedNode(this, MTDstrike, parseLatexString(true)));
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
                                JKQTMathText::MTnode* cn=parseLatexString(currentTokenName.size()<=0, "any");
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

JKQTMathText::MTnode *JKQTMathText::getParsedNode() const {
    return this->parsedNode;
}


QList<JKQTMathText::tbrData> JKQTMathText::tbrs=QList<JKQTMathText::tbrData>();
QHash<JKQTMathText::tbrDataH, QRectF> JKQTMathText::tbrh=QHash<JKQTMathText::tbrDataH, QRectF>();

QRectF JKQTMathText::getTightBoundingRect(const QFont &fm, const QString &text, QPaintDevice *pd)
{
    JKQTMathText::tbrDataH  dh(fm, text, pd);
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

QFont JKQTMathText::getNonItalic(const QFont &font)
{
    QFont f=font;
    f.setItalic(false);
    return f;
}





bool JKQTMathText::parse(const QString& text, bool addSpaceBeforeAndAfter){
    QString ntext;
    if (addSpaceBeforeAndAfter) ntext=QString("\\;")+text+QString("\\;");
    else ntext=text;
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


QSizeF JKQTMathText::getSize(QPainter& painter){
    if (getTree()!=nullptr) {
        double w=0, a=0, d=0, s=0;
        getSizeDetail(painter, w, a, d, s);
        return QSizeF(w, a+d);
    }
    return QSizeF(0,0);
}

double JKQTMathText::getDescent(QPainter& painter) {
    double w=0, a=0, d=0, s=0;
    getSizeDetail(painter, w, a, d, s);
    return d;
}

double JKQTMathText::getAscent(QPainter& painter) {
    double w=0, a=0, d=0, s=0;
    getSizeDetail(painter, w, a, d, s);
    return a;
}

void JKQTMathText::getSizeDetail(QPainter& painter, double& width, double& ascent, double& descent, double& strikeoutPos) {
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

void JKQTMathText::draw(QPainter& painter, double x, double y, bool drawBoxes){
    if (getTree()!=nullptr) {
        MTenvironment ev;
        ev.color=fontColor;
        ev.fontSize=fontSize;
        QPen pp=painter.pen();
        QPen p=pp;
        p.setStyle(Qt::SolidLine);
        painter.setPen(p);
        getTree()->setDrawBoxes(drawBoxes);
        painter.setPen(p);
        getTree()->draw(painter, x, y, ev);
        painter.setPen(pp);
    }
}

void JKQTMathText::draw(QPainter& painter, unsigned int flags, QRectF rect, bool drawBoxes) {
    if (getTree()!=nullptr) {
        QPen pp=painter.pen();
        QPen p=pp;
        p.setStyle(Qt::SolidLine);
        painter.setPen(p);
        MTenvironment ev;
        ev.color=fontColor;
        ev.fontSize=fontSize;
        getTree()->setDrawBoxes(drawBoxes);
        painter.setPen(p);

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
        painter.setPen(pp);
    }
}

JKQTMathText::MTwhitespaceNode::MTwhitespaceNode(JKQTMathText *_parent):
    MTtextNode(_parent, " ", false, false)
{

}

JKQTMathText::MTwhitespaceNode::~MTwhitespaceNode()
= default;

QString JKQTMathText::MTwhitespaceNode::getTypeName() const
{
    return QLatin1String("MTwhitespaceNode(")+text+")";
}

bool JKQTMathText::MTwhitespaceNode::toHtml(QString &html, JKQTMathText::MTenvironment /*currentEv*/, JKQTMathText::MTenvironment /*defaultEv*/) {
    html=html+"&nbsp;";
    return true;
}

void JKQTMathText::MTnode::setDrawBoxes(bool draw)
{
    this->drawBoxes=draw;
}

QString JKQTMathText::MTnode::getTypeName() const
{
    return "MTnode";
}

QString JKQTMathText::fracModeToString(JKQTMathText::MTfracMode mode)
{
    switch(mode) {
        case MTFMfrac:
            return "frac";
        case MTFMdfrac:
            return "dfrac";
        case MTFMsfrac:
            return "sfrac";
        case MTFMstfrac:
            return "stfrac";
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


QString JKQTMathText::decorationToString(JKQTMathText::MTdecoration mode)
{
    switch(mode) {
        case MTDvec:
            return "vec";
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
    }
    return "unknown";
}

JKQTMathText::MTnode *JKQTMathText::getTree() const {
    if (useUnparsed) return unparsedNode;
    return parsedNode;
}

JKQTMathTextLabel::JKQTMathTextLabel(QWidget *parent):
    QLabel(parent)
{
    m_mathText=new JKQTMathText(this);
    m_mathText->useXITS();
    m_mathText->setFontSize(font().pointSizeF()*1.3);
    lastText="";
    repaintDo=true;
    buffer=QPixmap();
}

JKQTMathTextLabel::~JKQTMathTextLabel()
= default;

JKQTMathText *JKQTMathTextLabel::getMathText() const
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
            //qDebug()<<"internalPaint(): parse "<<m_mathText->parse(lastText)<<"\n  "<<m_mathText->getErrorList().join("\n")<<"\n\n";
            if (!m_mathText->parse(lastText)) {
                //qDebug()<<"JKQTMathTextLabel::internalPaint(): parse '"<<lastText<<"': "<<m_mathText->parse(lastText)<<"\n  "<<m_mathText->getErrorList().join("\n")<<"\n\n";
            }
            if (buffer.width()<=0 || buffer.height()<=0) {
                const qreal dpr = devicePixelRatioF();
                buffer=QPixmap(1000*dpr,100*dpr);
                buffer.setDevicePixelRatio(dpr);
            }
            //qDebug()<<"internalPaint(): buffer "<<buffer.size();
            QPainter p;
            //qDebug()<<"internalPaint(): "<<p.begin(&buffer);
            p.begin(&buffer);
            p.setRenderHint(QPainter::Antialiasing);
#if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
            p.setRenderHint(QPainter::HighQualityAntialiasing);
#endif
            p.setRenderHint(QPainter::TextAntialiasing);
            size=m_mathText->getSize(p);
            p.end();
        }
        const qreal dpr = devicePixelRatioF();
        buffer=QPixmap(static_cast<int>(qMax(32.0,size.width()*1.2))*dpr, static_cast<int>(qMax(10.0,size.height()*1.1))*dpr);
        buffer.setDevicePixelRatio(dpr);
        buffer.fill(Qt::transparent);
        {
            qDebug()<<"internalPaint(): buffer.size()="<<buffer.size()<<" size="<<size<<" dpr="<<dpr;
            QPainter p;
            //qDebug()<<"internalPaint(): "<<p.begin(&buffer);
            p.begin(&buffer);
            p.setRenderHint(QPainter::Antialiasing);
#if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
            p.setRenderHint(QPainter::HighQualityAntialiasing);
#endif
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
    QLabel::paintEvent(event);
}



JKQTMathText::tbrData::tbrData(const QFont &f, const QString &text,  QPaintDevice *pd):
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

bool JKQTMathText::tbrData::operator==(const JKQTMathText::tbrData &other) const
{
    return ldpiX==other.ldpiX &&  ldpiY==other.ldpiY && text==other.text && f==other.f;
}


JKQTMathText::tbrDataH::tbrDataH(const QFont &f, const QString &text, QPaintDevice *pd)
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

bool JKQTMathText::tbrDataH::operator==(const JKQTMathText::tbrDataH &other) const
{
    return ldpiX==other.ldpiX &&  ldpiY==other.ldpiY && text==other.text && f==other.f;

}


JKQTMathText::MTplainTextNode::MTplainTextNode(JKQTMathText *_parent, const QString& _text, bool addWhitespace, bool stripInnerWhitepace):
    JKQTMathText::MTtextNode(_parent, _text, addWhitespace, stripInnerWhitepace)
{

}

QString JKQTMathText::MTplainTextNode::getTypeName() const
{
    return QLatin1String("MTplainTextNode(")+text+")";
}


QString JKQTMathText::MTplainTextNode::textTransform(const QString &_text, JKQTMathText::MTenvironment /*currentEv*/, bool /*forSize*/)
{
    return _text;
}

void initJKQTMathTextResources()
{
    static bool initialized=false;
    if (!initialized) {
#ifdef JKQTMATHTEXT_COMPILED_WITH_XITS
        Q_INIT_RESOURCE(xits);
#endif
        initialized=true;
    }
}

JKQTMathText::MTnodeSize::MTnodeSize():
    width(0), baselineHeight(0),overallHeight(0),strikeoutPos()
{

}

JKQTMathText::FontDefinition::FontDefinition():
    fontName("Times New Roman"), fontEncoding(MTFEStandard),
    symbolfontGreek("Symbol"), symbolfontGreekEncoding(MTFEwinSymbol),
    symbolfontSymbol("Symbol"), symbolfontSymbolEncoding(MTFEwinSymbol)
{

}

JKQTMathTextFontSpecifier::JKQTMathTextFontSpecifier():
    m_fontName(""),
    m_mathFontName("")
{

}

JKQTMathTextFontSpecifier::JKQTMathTextFontSpecifier(const QString &_fontName, const QString &_mathFontName):
    m_fontName(_fontName),
    m_mathFontName(_mathFontName)
{

}

JKQTMathTextFontSpecifier JKQTMathTextFontSpecifier::fromFontSpec(const QString &fontSpec)
{
    JKQTMathTextFontSpecifier s;
    s.setFontSpec(fontSpec);
    return s;
}

void JKQTMathTextFontSpecifier::setFontSpec(const QString &fontSpec)
{
    QStringList splitspec=fontSpec.split('+');
    if (splitspec.size()==0) {
        m_fontName=m_mathFontName="";
    } else if (splitspec.size()==1) {
        m_fontName=splitspec[0];
        m_mathFontName="";
    } else if (splitspec.size()==2) {
        m_fontName=splitspec[0];
        m_mathFontName=splitspec[1];
    } else if (splitspec.size()>2) {
        m_fontName=splitspec.mid(0, splitspec.size()-1).join('+');
        m_mathFontName=splitspec.last();
    }
}

QString JKQTMathTextFontSpecifier::getFontSpec() const
{
    QString res=m_fontName;
    if (m_mathFontName.size()>0) res+="+"+m_mathFontName;
    return res;
}

QString JKQTMathTextFontSpecifier::fontName() const
{
    return transformFontName(m_fontName);
}

QString JKQTMathTextFontSpecifier::mathFontName() const
{
    return transformFontName(m_mathFontName);
}

QString JKQTMathTextFontSpecifier::transformFontName(const QString &fontName)
{
    const QString fnt=fontName.trimmed().toLower();
    QFont testFnt;
    if (fnt=="serif") {
        testFnt.setStyleHint(QFont::StyleHint::Serif);
        return testFnt.defaultFamily();
    }
    if (fnt=="sans-serif" || fnt=="sansserif" || fnt=="sans" || fnt=="sans serif") {
        testFnt.setStyleHint(QFont::StyleHint::SansSerif);
        return testFnt.defaultFamily();
    }
    if (fnt=="cursive") {
        testFnt.setStyleHint(QFont::StyleHint::Cursive);
        return testFnt.defaultFamily();
    }
    if (fnt=="typewriter") {
        testFnt.setStyleHint(QFont::StyleHint::TypeWriter);
        return testFnt.defaultFamily();
    }
    if (fnt=="monospace") {
        testFnt.setStyleHint(QFont::StyleHint::Monospace);
        return testFnt.defaultFamily();
    }
    if (fnt=="fantasy") {
        testFnt.setStyleHint(QFont::StyleHint::Fantasy);
        return testFnt.defaultFamily();
    }
    if (fnt=="system") {
        testFnt.setStyleHint(QFont::StyleHint::System);
        return testFnt.defaultFamily();
    }
    if (fnt=="decorative") {
        testFnt.setStyleHint(QFont::StyleHint::Decorative);
        return testFnt.defaultFamily();
    }
    if (fnt=="default" || fnt=="app" || fnt=="application") {
        return QGuiApplication::font().family();
    }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    if (fnt=="fixed") {
        return QFontDatabase::systemFont(QFontDatabase::SystemFont::FixedFont).family();
    }
    if (fnt=="smallest_readable" || fnt=="smallestreadable" || fnt=="smallest readable" || fnt=="smallest") {
        return QFontDatabase::systemFont(QFontDatabase::SystemFont::SmallestReadableFont).family();
    }
    if (fnt=="title") {
        return QFontDatabase::systemFont(QFontDatabase::SystemFont::TitleFont).family();
    }
    if (fnt=="general") {
        return QFontDatabase::systemFont(QFontDatabase::SystemFont::GeneralFont).family();
    }
#elif QT_VERSION >= QT_VERSION_CHECK(5,2,0)
    QFontDatabase fontDB;
    if (fnt=="fixed") {
        return fontDB.systemFont(QFontDatabase::SystemFont::FixedFont).family();
    }
    if (fnt=="smallest_readable" || fnt=="smallestreadable" || fnt=="smallest readable" || fnt=="smallest") {
        return fontDB.systemFont(QFontDatabase::SystemFont::SmallestReadableFont).family();
    }
    if (fnt=="title") {
        return fontDB.systemFont(QFontDatabase::SystemFont::TitleFont).family();
    }
    if (fnt=="general") {
        return fontDB.systemFont(QFontDatabase::SystemFont::GeneralFont).family();
    }
#endif
    return fontName;
}

QString JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts(const QString &fontName)
{
    const QString fnt=fontName.toLower().trimmed();
    if (fnt=="xits") {
        return getXITSFamilies().fontName();
    } else if (fnt=="asana") {
        return getASANAFamilies().fontName();
    } else if (fnt=="stix") {
        return getSTIXFamilies().fontName();
    }
    return transformFontName(fontName);
}

bool JKQTMathTextFontSpecifier::hasFontName() const
{
    return !m_fontName.isEmpty();
}

bool JKQTMathTextFontSpecifier::hasMathFontName() const
{
    return !m_mathFontName.isEmpty();
}

JKQTMathTextFontSpecifier JKQTMathTextFontSpecifier::getXITSFamilies()
{
#if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
    QFontDatabase fdb;
    const auto fontFamilies=fdb.families();
#else
    const auto fontFamilies=QFontDatabase::families();
#endif
    if (!fontFamilies.contains("XITS")) {
        if (QFile::exists(":/JKQTMathText/fonts/xits-bold.otf")) { QFontDatabase::addApplicationFont(":/JKQTMathText/fonts/xits-bold.otf"); }
        if (QFile::exists(":/JKQTMathText/fonts/xits-bolditalic.otf")) { QFontDatabase::addApplicationFont(":/JKQTMathText/fonts/xits-bolditalic.otf"); }
        if (QFile::exists(":/JKQTMathText/fonts/xits-italic.otf")) { QFontDatabase::addApplicationFont(":/JKQTMathText/fonts/xits-italic.otf"); }
        if (QFile::exists(":/JKQTMathText/fonts/xits-math.otf")) { QFontDatabase::addApplicationFont(":/JKQTMathText/fonts/xits-math.otf"); }
        if (QFile::exists(":/JKQTMathText/fonts/xits-mathbold.otf")) { QFontDatabase::addApplicationFont(":/JKQTMathText/fonts/xits-mathbold.otf"); }
        if (QFile::exists(":/JKQTMathText/fonts/xits-regular.otf")) { QFontDatabase::addApplicationFont(":/JKQTMathText/fonts/xits-regular.otf"); }
    }

    static JKQTMathTextFontSpecifier fontSpec;
    if (fontSpec.m_fontName.isEmpty() && fontSpec.m_mathFontName.isEmpty()) {
        for (int i=0; i<fontFamilies.size(); i++) {
            if (fontFamilies.at(i).contains("XITS Math")) {
                fontSpec.m_mathFontName=fontFamilies.at(i);
            } else if (fontFamilies.at(i).contains("XITS")) {
                fontSpec.m_fontName=fontFamilies.at(i);
            }
            if (fontSpec.m_mathFontName.size()>0 && fontSpec.m_fontName.size()>0) {
                break;
            }
        }
        if (fontSpec.m_mathFontName.isEmpty() && !fontSpec.m_fontName.isEmpty()) {
            fontSpec.m_mathFontName=fontSpec.m_fontName;
        } else if (!fontSpec.m_mathFontName.isEmpty() && fontSpec.m_fontName.isEmpty()) {
            fontSpec.m_fontName=fontSpec.m_mathFontName;
        }
    }

    return fontSpec;
}

JKQTMathTextFontSpecifier JKQTMathTextFontSpecifier::getASANAFamilies()
{
#if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
    QFontDatabase fdb;
    const auto fontFamilies=fdb.families();
#else
    const auto fontFamilies=QFontDatabase::families();
#endif
    if (!fontFamilies.contains("Asana") && !fontFamilies.contains("Asana Math")) {
        if (QFile::exists(":/JKQTMathText/fonts/asana-math.otf")) { /*i=*/QFontDatabase::addApplicationFont(":/JKQTMathText/fonts/asana-math.otf"); }
    }


    static JKQTMathTextFontSpecifier fontSpec;
    if (fontSpec.m_fontName.isEmpty() && fontSpec.m_mathFontName.isEmpty()) {
        for (int i=0; i<fontFamilies.size(); i++) {
            if (fontFamilies.at(i).contains("Asana Math")) {
                fontSpec.m_mathFontName=fontFamilies.at(i);
            } else if (fontFamilies.at(i).contains("Asana")) {
                fontSpec.m_fontName=fontFamilies.at(i);
            }
            if (fontSpec.m_mathFontName.size()>0 && fontSpec.m_fontName.size()>0) {
                break;
            }
        }
        if (fontSpec.m_mathFontName.isEmpty() && !fontSpec.m_fontName.isEmpty()) {
            fontSpec.m_mathFontName=fontSpec.m_fontName;
        } else if (!fontSpec.m_mathFontName.isEmpty() && fontSpec.m_fontName.isEmpty()) {
            fontSpec.m_fontName=fontSpec.m_mathFontName;
        }
    }


    return fontSpec;
}

JKQTMathTextFontSpecifier JKQTMathTextFontSpecifier::getSTIXFamilies()
{
    static QStringList mathNames{"STIX Two Math", "STIX Math", "STIX Two Math Standard", "STIX Math Standard"};
    static QStringList textNames{"STIX", "STIXGeneral", "STIX General"};

    static JKQTMathTextFontSpecifier fontSpec;
    if (fontSpec.m_fontName.isEmpty() && fontSpec.m_mathFontName.isEmpty()) {
#if (QT_VERSION<QT_VERSION_CHECK(6, 0, 0))
        QFontDatabase fdb;
        const auto fontFamilies=fdb.families();
#else
        const auto fontFamilies=QFontDatabase::families();
#endif
        for (const QString& name:mathNames) {
            for (int i=0; i<fontFamilies.size(); i++) {
                if (fontFamilies.at(i).contains(name) ) {
                    fontSpec.m_mathFontName=fontFamilies.at(i);
                }
                if (fontSpec.m_mathFontName.size()>0) {
                    break;
                }
            }
            if (fontSpec.m_mathFontName.size()>0) {
                break;
            }
        }
        for (const QString& name:textNames) {
            for (int i=0; i<fontFamilies.size(); i++) {
                if (fontFamilies.at(i).contains(name) ) {
                    fontSpec.m_fontName=fontFamilies.at(i);
                }
                if (fontSpec.m_fontName.size()>0) {
                    break;
                }
            }
            if (fontSpec.m_fontName.size()>0) {
                break;
            }
        }
        if (fontSpec.m_mathFontName.isEmpty() && !fontSpec.m_fontName.isEmpty()) {
            fontSpec.m_mathFontName=fontSpec.m_fontName;
        } else if (!fontSpec.m_mathFontName.isEmpty() && fontSpec.m_fontName.isEmpty()) {
            fontSpec.m_fontName=fontSpec.m_mathFontName;
        }
    }
    return fontSpec;
}
