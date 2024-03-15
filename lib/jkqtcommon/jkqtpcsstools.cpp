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



#include "jkqtcommon/jkqtpcsstools.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include "jkqtcommon/jkqtpcodestructuring.h"
#include <stdexcept>
#include <QMetaEnum>
#include <QSet>
#include <QVector>


JKQTPCSSParser::RawErrorTag_t JKQTPCSSParser::RawErrorTag={};



JKQTPCSSParser::Token JKQTPCSSParser::peekNextToken(int *endPos)
{
    // on "RETURN" fromm this function: reset to current status and store the new pos in oldPos (if !=nullptr)
    auto _final=JKQTPFinally(std::bind([&](JKQTPCSSParser* p,     const Token oldToken, const int oldPos){
        if (endPos) *endPos=p->pos;
        p->CurrentToken=oldToken;
        p->pos=oldPos;
    }, this, CurrentToken, pos));

    QChar ch('\0');
    while(getCh(ch) && ch.isSpace()) {

    }

    static QString hexDigits="0123456789abcdefABCDEF";

    if (ch==QChar('\0')) return Token(Token::END);
    else if (ch==QChar(',')) return Token(Token::COMMA);
    else if (ch==QChar('(')) return Token(Token::LBRACKET);
    else if (ch==QChar(')')) return Token(Token::RBRACKET);
    else if (ch==QChar('/')) return Token(Token::SLASH);
    else if (ch==QChar('#')) {
        QString hex;
        while (peekCh(ch) && hexDigits.contains(ch)) {
            getCh(ch);
            hex+=ch;
        }
        if (hex.size()==0) throw std::runtime_error(QString("did not find HEX digits after a HASH '#' at pos.%1!").arg(pos).toStdString());
        return Token(hex,Token::HEXSTRING);
    }  else if (ch.isDigit() || ch=='+' || ch=='-' || ch=='.') {
        QString num=ch;
        QString unit;
        while (peekCh(ch) && (ch.isDigit() || ch=='.')) {
            getCh(ch);
            num+=ch;
        }
        if (ch.isLetter() || ch=='\xB0' || ch=='%') {
            unit=ch;
            getCh(ch);
            while (peekCh(ch) && (ch.isLetter() || ch=='\xB0' || ch=='%')) {
                getCh(ch);
                unit+=ch;
            }
        }
        return Token(num.toDouble(), unit);
    } else if (ch.isLetter()) {
        QString name=ch;
        while (peekCh(ch) && (ch.isLetterOrNumber() ||ch=='-' || ch=='_'|| ch=='.')) {
            getCh(ch);
            name+=ch;
        }
        return Token(name);
    } else {
        throw std::runtime_error(QString("found unexpected character '%1' at pos.%2").arg(ch).arg(pos).toStdString());
    }
    return Token::END;
}

JKQTPCSSParser::Token JKQTPCSSParser::getToken()
{
    int newpos=pos;
    const auto newT=peekNextToken(&newpos);
    pos=newpos;
    return CurrentToken=newT;
}

JKQTPExpected<JKQTPCSSParser::NumberWithUnit, JKQTPCSSParser::GeneralError> JKQTPCSSParser::parseNumberWithUnit(bool get)
{
    NumberWithUnit num = NumberWithUnit(0.0,"");

    if (get) getToken();
    if (CurrentToken.is(Token::TokenType::NUMBER)) {
        num=NumberWithUnit(CurrentToken.NumberValue, CurrentToken.StringValue);
    } else {
        return {JKQTPUnexpected, UnexpectedTokenError(Token::NUMBER, CurrentToken, pos) };
    }
    return { num };
}

JKQTPExpected<QColor, JKQTPCSSParser::GeneralError> JKQTPCSSParser::parseColor(bool get)
{
    QColor col=Qt::black;

    if (get) getToken();

    if (CurrentToken.is(Token::TokenType::HEXSTRING)) {
        const QString hex=CurrentToken.StringValue;
        if (hex.size()==3) {
            const int r=QString(hex[0]).toInt(nullptr, 16);
            const int g=QString(hex[1]).toInt(nullptr, 16);
            const int b=QString(hex[2]).toInt(nullptr, 16);
            col=QColor::fromRgb(r*16+r,g*16+g,b*16+b);
        } else if (hex.size()==4) {
            const int r=QString(hex[0]).toInt(nullptr, 16);
            const int g=QString(hex[1]).toInt(nullptr, 16);
            const int b=QString(hex[2]).toInt(nullptr, 16);
            const int a=QString(hex[3]).toInt(nullptr, 16);
            col=QColor::fromRgb(r*16+r,g*16+g,b*16+b,a*16+a);
        } else if (hex.size()==6) {
            col=QColor::fromRgb(hex.mid(0,2).toInt(nullptr, 16),hex.mid(2,2).toInt(nullptr, 16),hex.mid(4,2).toInt(nullptr, 16));
        } else if (hex.size()==8) {
            col=QColor::fromRgb(hex.mid(0,2).toInt(nullptr, 16),hex.mid(2,2).toInt(nullptr, 16),hex.mid(4,2).toInt(nullptr, 16),hex.mid(6,2).toInt(nullptr, 16));
        } else {
            return {JKQTPUnexpected, UnconvertobleError("#"+hex, "HEX-RGB value", pos) };
        }
    } else if (CurrentToken.is(Token::TokenType::NAME)) {
        static QMap<QString,QPair<int,int> > rgbFuncs={
            {"rgb", QPair<int,int>(3,4)},
            {"rgba",QPair<int,int>(4,4)},
            {"hsl",QPair<int,int>(3,4)},
            {"hsv",QPair<int,int>(3,4)},
            {"gray",QPair<int,int>(1,2)},
            {"grey",QPair<int,int>(1,2)},
            {"red",QPair<int,int>(1,2)},
            {"green",QPair<int,int>(1,2)},
            {"blue",QPair<int,int>(1,2)}
        };
        const QString func=CurrentToken.getNormString();
        if (rgbFuncs.contains(func)) {
            int nextPos=pos;
            const auto nextT=peekNextToken(&nextPos);
            if (nextT.is(Token::END)) {
                getToken();
                return jkqtp_lookupQColorName(func);
            } else if (!nextT.is(Token::LBRACKET)) {
                bool found=false;
                col=jkqtp_lookupQColorName(func, false, &found);
                if (!found) {
                    getToken();
                    return {JKQTPUnexpected, UnexpectedTokenError(func, Token::LBRACKET, CurrentToken, pos) };
                } else return { col };
            }
            getToken();
            const int minParams=rgbFuncs[func].first;
            const int maxParams=rgbFuncs[func].second;
            QList<double> params;
            getToken();
            while (CurrentToken.is(Token::NUMBER)) {
                const auto num=parseNumberWithUnit(false);
                if (!num.has_value()) return {JKQTPUnexpected, UnexpectedTermError(func, "number with unit", CurrentToken, pos) };
                if (params.size()==0 && (func=="hsl" || func=="hsv")) params.append(num.value().normHueNumber());
                else {
                    if ((minParams!=maxParams && params.size()==maxParams-1) || minParams==4) params.append(jkqtp_bounded(0.0,num.value().normNumber(),1.0));
                    else params.append(num.value().normRGBNumber());
                }
                getToken();
                if (CurrentToken.is(Token::COMMA) || CurrentToken.is(Token::SLASH)) getToken();
            }
            if (!CurrentToken.is(Token::RBRACKET) && !CurrentToken.is(Token::END)) return {JKQTPUnexpected, UnexpectedTokenError(func, Token::RBRACKET, CurrentToken, pos) };
            if (params.size()>=minParams && params.size()<=maxParams) {
                if (func=="rgb" || func=="rgba") {
                    col=QColor::fromRgbF(params[0],params[1],params[2],params.value(3,1));
                } else if (func=="hsl") {
                    col=QColor::fromHslF(params[0],params[1],params[2],params.value(3,1));
                } else if (func=="hsv") {
                    col=QColor::fromHsvF(params[0],params[1],params[2],params.value(3,1));
                } else if (func=="gray" || func=="grey") {
                    col=QColor::fromRgbF(params[0],params[0],params[0],params.value(1,1));
                } else if (func=="red") {
                    col=QColor::fromRgbF(params[0],0.0,0.0,params.value(1,1));
                } else if (func=="green") {
                    col=QColor::fromRgbF(0.0,params[0],0.0,params.value(1,1));
                } else if (func=="blue") {
                    col=QColor::fromRgbF(0.0,0.0,params[0],params.value(1,1));
                }
            } else return {JKQTPUnexpected, WrongNumberOfArgumentError(func, params.size(), minParams, maxParams, pos) };
        } else {
            col=jkqtp_lookupQColorName(CurrentToken.StringValue);
        }
    } else {
        return {JKQTPUnexpected, UnexpectedTokenError(Token::NUMBER, CurrentToken, pos) };
    }
    return { col };
}

JKQTPExpected<QGradient, JKQTPCSSParser::GeneralError> JKQTPCSSParser::parseGradient(bool get)
{
    QGradient grad=QGradient();

    if (get) getToken();

    static QMap<QString,QGradient::Preset> s_GradientPresets = []() {
        QMap<QString,QGradient::Preset> m;
        for (int i=1; i<QMetaEnum::fromType<QGradient::Preset>().keyCount(); i++) {
            const QString id=QString(QMetaEnum::fromType<QGradient::Preset>().valueToKey(static_cast<QGradient::Preset>(i))).toLower().trimmed().simplified();
            if (id.size()>0) m[id]=static_cast<QGradient::Preset>(i);
        }
        return m;
    }();

    const QString func=CurrentToken.StringValue.trimmed().simplified().toLower();
    if (CurrentToken.is(Token::TokenType::NAME) && func=="linear-gradient") {
        QGradientStops colorStops;
        double alphaDeg=180;    // 0   = bottom->top    angle like clock-pointer
                                // 90  = left->right
                                // 180 = top->bottom
                                // 270 = right->left

        getToken();

        if (!CurrentToken.is(Token::LBRACKET)) return {JKQTPUnexpected, UnexpectedTokenError(func, Token::LBRACKET, CurrentToken, pos) };
        getToken();

        if (CurrentToken.isNormString("to")) {
            QStringList capDir;
            static QSet<QString> allowedDirs={"left","right","top","bottom"};
            while (getToken().is(Token::NAME)) {
                const QString ns=CurrentToken.getNormString();
                if (allowedDirs.contains(ns)) {
                    capDir.append(ns);
                } else {
                    return {JKQTPUnexpected, UnexpectedTermError(func, "direction specifier [left|right|top|bottom] after 'to'", CurrentToken, pos) };
                }
            }
            if (capDir.size()==1 || (capDir.size()==2 && capDir[1].isEmpty())) {
                if (capDir[0]=="left") alphaDeg=270;
                else if (capDir[0]=="right") alphaDeg=90;
                else if (capDir[0]=="top") alphaDeg=0;
                else if (capDir[0]=="bottom") alphaDeg=180;
            } else if (capDir.size()==2) {
                if (capDir[0]=="top"||capDir[0]=="bottom") qSwap(capDir[0],capDir[1]);
                if (capDir[0]=="left" && capDir[1]=="top") alphaDeg=270+45;
                else if (capDir[0]=="left" && capDir[1]=="bottom") alphaDeg=270-45;
                else if (capDir[0]=="right" && capDir[1]=="top") alphaDeg=90-45;
                else if (capDir[0]=="right" && capDir[1]=="bottom") alphaDeg=90+45;
            } else {
                return {JKQTPUnexpected, GeneralError(func, "at most two directions allowed after 'to', but found "+QString::number(capDir.size()), pos) };
            }
            if (!CurrentToken.is(Token::COMMA)) return {JKQTPUnexpected, UnexpectedTokenError(func, Token::COMMA, CurrentToken, pos) };
            //getToken();
        } if (CurrentToken.is(Token::NUMBER)) {
            auto angle=parseNumberWithUnit(false);
            alphaDeg=angle.value().normNumber();
            getToken();
        }

        //  alphaDeg  0    90   180   270
        //  sin       0     1     0    -1
        //  cos       1     0    -1     0
        //           up   right down  left
        const double dx=jkqtp_roundToDigits(0.5*sin(alphaDeg/180.0*M_PI),10);
        const double dy=jkqtp_roundToDigits(-0.5*cos(alphaDeg/180.0*M_PI),10);

        QLinearGradient lgrad(0.5-dx,0.5-dy,0.5+dx,0.5+dy);
        lgrad.setCoordinateMode(QGradient::ObjectBoundingMode);

        bool done=false;
        while (!done) {
            QGradientStop stop;
            const auto col=parseColor(true);
            if (!col.has_value()) return {JKQTPUnexpected, UnexpectedTermError(func, "color specifier", CurrentToken, pos) };
            stop.second=col.value();
            stop.first=-1;
            getToken();

            if (CurrentToken.is(Token::NUMBER)) {
                if (CurrentToken.StringValue!="%") return {JKQTPUnexpected, UnexpectedTermError(func, "percentage value", CurrentToken, pos) };
                stop.first=CurrentToken.NumberValue/100.0;
                getToken();
            }
            colorStops<<stop;
            if (CurrentToken.is(Token::RBRACKET) || CurrentToken.is(Token::END)) {
                done=true;
            }
        }
        if (colorStops.size()>0) {
            // normalize colorStop positions
            if (colorStops.first().first<0) colorStops.first().first=0;
            if (colorStops.size()>1) {
                if (colorStops.last().first<0) colorStops.last().first=1.0;
            }
            int i=1;
            while (i<colorStops.size()) {
                if(colorStops[i].first<0) {
                    const double lastStop=colorStops[i-1].first;
                    const int lastI=i-1;
                    while (colorStops[i].first<0) i++;
                    const double endStop=colorStops[i].first;
                    const int cnt=i-lastI-1;
                    const double delta=(endStop-lastStop)/static_cast<double>(cnt+1);
                    for (int j=lastI+1; j<i; j++) {
                        colorStops[j].first=lastStop+static_cast<double>(j-lastI)*delta;
                    }
                }
                i++;
            }

            lgrad.setStops(colorStops);
            grad=lgrad;
        }
    } else if (CurrentToken.isNormStringAnyOf(s_GradientPresets.keys())) {
        grad=QGradient(s_GradientPresets[CurrentToken.getNormString()]);
        grad.setCoordinateMode(QGradient::ObjectBoundingMode);

    } else {
        return {JKQTPUnexpected, UnexpectedTermError("supported gradient-function [linear-gradient|] or predefined gradient name", CurrentToken, pos) };
    }
    return { grad };
}



JKQTPCSSParser::JKQTPCSSParser(const QString &text_):
    CurrentToken(Token::TokenType::END), text(text_), pos(0)
{

}


QString JKQTPCSSParser::Token::toString(TokenType type)
{
    switch(type) {
    case END: return "END";
    case NAME: return "NAME";
    case NUMBER: return "NUMBER";
    case HEXSTRING: return "HEXSTRING";
    case LBRACKET: return "LBRACKET";
    case RBRACKET: return "RBRACKET";
    case COMMA: return "COMMA";
    case SLASH: return "SLASH";
    }
    return "???";
}

JKQTPCSSParser::Token::Token():
    type(TokenType::END), StringValue(""), NumberValue(0.0)
{

}

JKQTPCSSParser::Token::Token(TokenType type):
    type(type), StringValue(""), NumberValue(0.0)
{
    switch(type) {
    case LBRACKET: StringValue="("; break;
    case RBRACKET: StringValue=")"; break;
    case COMMA: StringValue=","; break;
    default: break;
    }

}

JKQTPCSSParser::Token::Token(double num, const QString &unit_):
    type(NUMBER), StringValue(unit_), NumberValue(num)
{

}

JKQTPCSSParser::Token::Token(const QString &str, TokenType type_):
    type(type_), StringValue(str), NumberValue(0.0)
{
    if (type_==NUMBER) NumberValue=str.toDouble();
    if (type_==HEXSTRING) NumberValue=str.toInt(nullptr,16);
}

QString JKQTPCSSParser::Token::toString() const
{
    QString s=toString(type);
    if (type==NUMBER) s+= " ["+QString::number(NumberValue, 'f')+"]";
    else if (StringValue.size()>0) s+= " ["+StringValue+"]";
    return s;
}


JKQTPCSSParser::NumberWithUnit JKQTPCSSParser::readNumberWithUnit(const QString &prog)
{
    JKQTPCSSParser parser(prog);
    const auto res= parser.parseNumberWithUnit(true);
    if (res.has_value()) return res.value();
    else throw std::runtime_error(res.error().error.toStdString());
}

QColor JKQTPCSSParser::readColor(const QString &prog)
{
    JKQTPCSSParser parser(prog);
    const auto res= parser.parseColor(true);
    if (res.has_value()) return res.value();
    else throw std::runtime_error(res.error().error.toStdString());
}

QGradient JKQTPCSSParser::readGradient(const QString &prog)
{
    JKQTPCSSParser parser(prog);
    const auto res= parser.parseGradient(true);
    if (res.has_value()) return res.value();
    else throw std::runtime_error(res.error().error.toStdString());
}
