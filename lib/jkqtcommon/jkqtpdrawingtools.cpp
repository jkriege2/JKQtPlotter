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

#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtcommon/jkqtpenhancedpainter.h"
#include <QDebug>

const double JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH= 0.02;



QString JKQTPGraphSymbols2String(JKQTPGraphSymbols pos) {
    switch(pos) {
        case JKQTPDot: return "symbol_dot";
        case JKQTPCross: return "symbol_cross";
        case JKQTPPlus: return "symbol_plus";
        case JKQTPCircle: return "symbol_circle";
        case JKQTPFilledCircle: return "symbol_filled_circle";
        case JKQTPRect: return "symbol_rect";
        case JKQTPFilledRect: return "symbol_filled_rect";
        case JKQTPTriangle: return "symbol_triangle";
        case JKQTPFilledTriangle: return "symbol_filled_triangle";
        case JKQTPDownTriangle: return "symbol_down_triangle";
        case JKQTPFilledDownTriangle: return "symbol_filled_down_triangle";
        case JKQTPTarget: return "symbol_target";
        case JKQTPstar: return "symbol_star";
        case JKQTPFilledStar: return "symbol_filled_star";
        case JKQTPDiamond: return "symbol_diamond";
        case JKQTPFilledDiamond: return "symbol_filled_diamond";
        case JKQTPPentagon: return "symbol_pentagon";
        case JKQTPAsterisc: return "symbol_asterisc";
        case JKQTPAsterisc6: return "symbol_asterisc6";
        case JKQTPAsterisc8: return "symbol_asterisc8";
        case JKQTPFilledPentagon: return "symbol_filled_pentagon";
        case JKQTPHexagon: return "symbol_hexagon";
        case JKQTPFilledHexagon: return "symbol_filled_hexagon";
        case JKQTPOctagon: return "symbol_octagon";
        case JKQTPFilledOctagon: return "symbol_filled_octagon";
        case JKQTPRectCross: return "symbol_rect_cross";
        case JKQTPRectPlus: return "symbol_rect_plus";
        case JKQTPNoSymbol: return "none";
        case JKQTPDiamondPlus: return "symbol_diamond_plus";
        case JKQTPDiamondCross: return "symbol_diamond_cross";
        case JKQTPCircleCross: return "symbol_circle_cross";
        case JKQTPCirclePlus: return "symbol_circle_plus";
        case JKQTPUpDownTriangle: return "symbol_updowntriangle";
        case JKQTPFilledUpDownTriangle: return "symbol_filled_updowntriangle";
        case JKQTPSantaClauseHouse: return "symbol_santaclause";
        case JKQTPFilledSantaClauseHouse: return "symbol_filled_santaclause";
        case JKQTPHourglass: return "symbol_hourglass";
        case JKQTPFilledHourglass: return "symbol_filled_hourglass";
        case JKQTPHorizontalHourglass: return "symbol_horizontal_hourglass";
        case JKQTPFilledHorizontalHourglass: return "symbol_filled_horizontal_hourglass";
        case JKQTPRectTriangle: return "symbol_rect_triangle";
        case JKQTPRectDownTriangle: return "symbol_rect_downtriangle";
        case JKQTPRectLeftTriangle: return "symbol_rect_lefttriangle";
        case JKQTPRectRightTriangle: return "symbol_rect_righttriangle";
        case JKQTPLeftTriangle: return "symbol_left_triangle";
        case JKQTPFilledLeftTriangle: return "symbol_filled_left_triangle";
        case JKQTPRightTriangle: return "symbol_right_triangle";
        case JKQTPFilledRightTriangle: return "symbol_filled_right_triangle";
        case JKQTPTripod: return "symbol_tripod";
        case JKQTPDownTripod: return "symbol_down_tripod";
        case JKQTPLeftTripod: return "symbol_left_tripod";
        case JKQTPRightTripod: return "symbol_right_tripod";
        case JKQTPFilledCurvedTriangle: return "symbol_filled_curved_triangle";
        case JKQTPFilledDownCurvedTriangle: return "symbol_filled_down_curved_triangle";
        case JKQTPFilledLeftCurvedTriangle: return "symbol_filled_left_curved_triangle";
        case JKQTPFilledRightCurvedTriangle: return "symbol_filled_right_curved_triangle";
        case JKQTPCurvedTriangle: return "symbol_curved_triangle";
        case JKQTPDownCurvedTriangle: return "symbol_down_curved_triangle";
        case JKQTPLeftCurvedTriangle: return "symbol_left_curved_triangle";
        case JKQTPRightCurvedTriangle: return "symbol_right_curved_triangle";
        case JKQTPPeace: return "symbol_peace";
        case JKQTPFemale: return "symbol_female";
        case JKQTPMale: return "symbol_male";
        case JKQTPCirclePeace: return "symbol_circle_peace";
        case JKQTPSymbolCount: JKQTPGraphSymbols2String(JKQTPMaxSymbolID);
        case JKQTPCharacterSymbol:
        case JKQTPFilledCharacterSymbol:
        case JKQTPFirstCustomSymbol:
            break;
    }
    if (pos>=JKQTPCharacterSymbol && pos<=JKQTPCharacterSymbol+0xFFFF) {
        QString s=QString::number(pos-JKQTPCharacterSymbol,16);
        while (s.size()<4) s="0"+s;
        return "symbol_char"+s;
    }
    if (pos>=JKQTPFilledCharacterSymbol && pos<=JKQTPFilledCharacterSymbol+0xFFFF) {
        QString s=QString::number(pos-JKQTPFilledCharacterSymbol,16);
        while (s.size()<4) s="0"+s;
        return "symbol_filled_char"+s;
    }
    if (pos>=JKQTPFirstCustomSymbol) {
        QString s=QString::number(pos-JKQTPFirstCustomSymbol);
        return "symbol_custom"+s;
    }
    return "";
}

QString JKQTPGraphSymbols2NameString(JKQTPGraphSymbols pos) {
    switch(pos) {
        case JKQTPDot: return QObject::tr("dot");
        case JKQTPCross: return QObject::tr("cross");
        case JKQTPPlus: return QObject::tr("plus");
        case JKQTPCircle: return QObject::tr("circle");
        case JKQTPFilledCircle: return QObject::tr("filled circle");
        case JKQTPRect: return QObject::tr("rectangle");
        case JKQTPFilledRect: return QObject::tr("filled rectangle");
        case JKQTPTriangle: return QObject::tr("triangle");
        case JKQTPFilledTriangle: return QObject::tr("filled triangle");
        case JKQTPDownTriangle: return QObject::tr("down triangle");
        case JKQTPFilledDownTriangle: return QObject::tr("filled down triangle");
        case JKQTPstar: return QObject::tr("star");
        case JKQTPFilledStar: return QObject::tr("filled star");
        case JKQTPDiamond: return QObject::tr("diamond");
        case JKQTPFilledDiamond: return QObject::tr("filled diamond");
        case JKQTPPentagon: return QObject::tr("pentagon");
        case JKQTPFilledPentagon: return QObject::tr("filled pentagon");
        case JKQTPHexagon: return QObject::tr("hexagon");
        case JKQTPFilledHexagon: return QObject::tr("filled hexagon");
        case JKQTPOctagon: return QObject::tr("octagon");
        case JKQTPFilledOctagon: return QObject::tr("filled octagon");
        case JKQTPTarget: return QObject::tr("target");
        case JKQTPAsterisc: return QObject::tr("asterisc, 5-arm");
        case JKQTPAsterisc6: return QObject::tr("asterisc, 6-arm");
        case JKQTPAsterisc8: return QObject::tr("asterisc, 8-arm");
        case JKQTPRectCross: return QObject::tr("square with cross");
        case JKQTPRectPlus: return QObject::tr("square with plus");
        case JKQTPNoSymbol: return QObject::tr("none");
        case JKQTPDiamondPlus: return QObject::tr("diamond with plus");
        case JKQTPDiamondCross: return QObject::tr("diamond with cross");
        case JKQTPCircleCross: return QObject::tr("circled cross");
        case JKQTPCirclePlus: return QObject::tr("circled plus");
        case JKQTPUpDownTriangle: return QObject::tr("up/down triangle");
        case JKQTPFilledUpDownTriangle: return QObject::tr("filled up/down triangle");
        case JKQTPSantaClauseHouse: return QObject::tr("santa clause");
        case JKQTPFilledSantaClauseHouse: return QObject::tr("filled santa clause");
        case JKQTPHourglass: return QObject::tr("hourglass");
        case JKQTPFilledHourglass: return QObject::tr("filled hourglass");
        case JKQTPHorizontalHourglass: return QObject::tr("horizontal hourglass");
        case JKQTPFilledHorizontalHourglass: return QObject::tr("filled horizontal hourglass");
        case JKQTPRectTriangle: return QObject::tr("rectangle with triangle");
        case JKQTPRectDownTriangle: return QObject::tr("rectangle with downwards triangle");
        case JKQTPRectLeftTriangle: return QObject::tr("rectangle with leftwards triangle");
        case JKQTPRectRightTriangle: return QObject::tr("rectangle with rightwards triangle");
        case JKQTPLeftTriangle: return QObject::tr("left triangle");
        case JKQTPFilledLeftTriangle: return QObject::tr("filled left triangle");
        case JKQTPRightTriangle: return QObject::tr("right triangle");
        case JKQTPFilledRightTriangle: return QObject::tr("filled right triangle");
        case JKQTPTripod: return QObject::tr("tripod");
        case JKQTPDownTripod: return QObject::tr("down tripod");
        case JKQTPLeftTripod: return QObject::tr("left tripod");
        case JKQTPRightTripod: return QObject::tr("right tripod");
        case JKQTPFilledCurvedTriangle: return QObject::tr("filled curved triangle");
        case JKQTPFilledDownCurvedTriangle: return QObject::tr("filled down curved triangle");
        case JKQTPFilledLeftCurvedTriangle: return QObject::tr("filled left curved triangle");
        case JKQTPFilledRightCurvedTriangle: return QObject::tr("filled right curved triangle");
        case JKQTPCurvedTriangle: return QObject::tr("curved triangle");
        case JKQTPDownCurvedTriangle: return QObject::tr("down curved triangle");
        case JKQTPLeftCurvedTriangle: return QObject::tr("left curved triangle");
        case JKQTPRightCurvedTriangle: return QObject::tr("right curved triangle");
        case JKQTPPeace: return QObject::tr("peace");
        case JKQTPFemale: return QObject::tr("female");
        case JKQTPMale: return QObject::tr("male");
        case JKQTPCirclePeace: return QObject::tr("circled peace");
        case JKQTPSymbolCount: JKQTPGraphSymbols2NameString(JKQTPMaxSymbolID);
        case JKQTPCharacterSymbol:
        case JKQTPFilledCharacterSymbol:
        case JKQTPFirstCustomSymbol:
            break;
    }
    if (pos>=JKQTPCharacterSymbol && pos<=JKQTPCharacterSymbol+0xFFFF) {
        return QObject::tr("character")+" '"+QChar(static_cast<uint16_t>(pos-JKQTPCharacterSymbol))+"'";
    }
    if (pos>=JKQTPFilledCharacterSymbol && pos<=JKQTPFilledCharacterSymbol+0xFFFF) {
        return QObject::tr("filled character")+" '"+QChar(static_cast<uint16_t>(pos-JKQTPFilledCharacterSymbol))+"'";
    }
    if (pos>=JKQTPFirstCustomSymbol) {
        return QObject::tr("custom symbol %1").arg(static_cast<uint64_t>(pos-JKQTPFirstCustomSymbol));
    }
    return "";
}
JKQTPGraphSymbols String2JKQTPGraphSymbols(const QString& pos)  {
    const QString posT=pos.trimmed();
    const QString s=posT.toLower();
    if (s=="symbol_dot"||s=="dot"||s==".") return JKQTPDot;
    if (s=="symbol_cross"||s=="cross"||s=="x") return JKQTPCross;
    if (s=="symbol_plus"||s=="plus"||s=="+") return JKQTPPlus;
    if (s=="symbol_circle"||s=="circle"||s=="o") return JKQTPCircle;
    if (s=="symbol_filled_circle"||s=="filled_circle"||s=="fo") return JKQTPFilledCircle;
    if (s=="symbol_rect"||s=="rect"||s=="r") return JKQTPRect;
    if (s=="symbol_filled_rect"||s=="filled_rect"||s=="fr") return JKQTPFilledRect;
    if (s=="symbol_triangle"||s=="triangle"||s=="^") return JKQTPTriangle;
    if (s=="symbol_filled_triangle"||s=="filled_triangle"||s=="f^") return JKQTPFilledTriangle;
    if (s=="symbol_down_triangle"||s=="down_triangle"||s=="v") return JKQTPDownTriangle;
    if (s=="symbol_filles_down_triangle"||s=="filles_down_triangle"||s=="symbol_filled_down_triangle"||s=="filled_down_triangle"||s=="fv") return JKQTPFilledDownTriangle;
    if (s=="symbol_target"||s=="target"||s=="t") return JKQTPTarget;
    if (s=="symbol_star"||s=="star"||s=="s") return JKQTPstar;
    if (s=="symbol_filled_star"||s=="filled_star"||s=="fs") return JKQTPFilledStar;
    if (s=="symbol_diamond"||s=="diamond"||s=="d") return JKQTPDiamond;
    if (s=="symbol_filled_diamond"||s=="filled_diamond"||s=="fd") return JKQTPFilledDiamond;
    if (s=="symbol_pentagon"||s=="pentagon"||s=="p") return JKQTPPentagon;
    if (s=="symbol_filled_pentagon"||s=="filled_pentagon"||s=="fp") return JKQTPFilledPentagon;
    if (s=="symbol_hexagon"||s=="hexagon"||s=="h") return JKQTPHexagon;
    if (s=="symbol_filled_hexagon"||s=="filled_hexagon"||s=="fh") return JKQTPFilledHexagon;
    if (s=="symbol_octagon"||s=="octagon"||s=="h") return JKQTPOctagon;
    if (s=="symbol_filled_octagon"||s=="filled_octagon"||s=="fh") return JKQTPFilledOctagon;
    if (s=="symbol_asterisc"||s=="asterisc"||s=="*") return JKQTPAsterisc;
    if (s=="symbol_asterisc6"||s=="asterisc6"||s=="*6") return JKQTPAsterisc6;
    if (s=="symbol_asterisc8"||s=="asterisc8"||s=="*8") return JKQTPAsterisc8;
    if (s=="symbol_rect_cross"||s=="rect_cross"||s=="rx") return JKQTPRectCross;
    if (s=="symbol_rect_plus"||s=="rect_plus"||s=="r+") return JKQTPRectPlus;
    if (s=="symbol_diamond_plus" || s=="diamond_plus") return JKQTPDiamondPlus;
    if (s=="symbol_diamond_cross" || s=="diamond_cross") return JKQTPDiamondCross;
    if (s=="symbol_circle_cross" || s=="circle_cross") return JKQTPCircleCross;
    if (s=="symbol_circle_plus" || s=="circle_plus") return JKQTPCirclePlus;
    if (s=="symbol_updowntriangle" || s=="updowntriangle") return JKQTPUpDownTriangle;
    if (s=="symbol_filled_updowntriangle" || s=="filled_updowntriangle") return JKQTPFilledUpDownTriangle;
    if (s=="symbol_santaclause" || s=="santaclause") return JKQTPSantaClauseHouse;
    if (s=="symbol_filled_santaclause" || s=="filled_santaclause") return JKQTPFilledSantaClauseHouse;
    if (s=="symbol_hourglass" || s=="hourglass") return JKQTPHourglass;
    if (s=="symbol_filled_hourglass" || s=="filled_hourglass") return JKQTPFilledHourglass;
    if (s=="symbol_horizontal_hourglass" || s=="horizontal_hourglass") return JKQTPHorizontalHourglass;
    if (s=="symbol_filled_horizontal_hourglass" || s=="filled_horizontal_hourglass") return JKQTPFilledHorizontalHourglass;
    if (s=="symbol_rect_triangle" || s=="rect_triangle") return JKQTPRectTriangle;
    if (s=="symbol_rect_downtriangle" || s=="rect_downtriangle") return JKQTPRectDownTriangle;
    if (s=="symbol_rect_lefttriangle" || s=="rect_lefttriangle") return JKQTPRectLeftTriangle;
    if (s=="symbol_rect_righttriangle" || s=="rect_righttriangle") return JKQTPRectRightTriangle;
    if (s=="symbol_left_triangle" || s=="left_triangle") return JKQTPLeftTriangle;
    if (s=="symbol_filled_left_triangle" || s=="filled_left_triangle") return JKQTPFilledLeftTriangle;
    if (s=="symbol_right_triangle" || s=="right_triangle") return JKQTPRightTriangle;
    if (s=="symbol_filled_right_triangle" || s=="filled_right_triangle") return JKQTPFilledRightTriangle;
    if (s=="symbol_tripod" || s=="tripod") return JKQTPTripod;
    if (s=="symbol_down_tripod" || s=="down_tripod") return JKQTPDownTripod;
    if (s=="symbol_left_tripod" || s=="left_tripod") return JKQTPLeftTripod;
    if (s=="symbol_right_tripod" || s=="right_tripod") return JKQTPRightTripod;
    if (s=="symbol_filled_curved_triangle" || s=="filled_curved_triangle") return JKQTPFilledCurvedTriangle;
    if (s=="symbol_filled_down_curved_triangle" || s=="filled_down_curved_triangle") return JKQTPFilledDownCurvedTriangle;
    if (s=="symbol_filled_left_curved_triangle" || s=="filled_left_curved_triangle") return JKQTPFilledLeftCurvedTriangle;
    if (s=="symbol_filled_right_curved_triangle" || s=="filled_right_curved_triangle") return JKQTPFilledRightCurvedTriangle;
    if (s=="symbol_curved_triangle" || s=="curved_triangle") return JKQTPCurvedTriangle;
    if (s=="symbol_down_curved_triangle" || s=="down_curved_triangle") return JKQTPDownCurvedTriangle;
    if (s=="symbol_left_curved_triangle" || s=="left_curved_triangle") return JKQTPLeftCurvedTriangle;
    if (s=="symbol_right_curved_triangle" || s=="right_curved_triangle") return JKQTPRightCurvedTriangle;
    if (s=="symbol_peace" || s=="peace") return JKQTPPeace;
    if (s=="symbol_circle_peace" || s=="circle_peace") return JKQTPCirclePeace;
    if (s=="symbol_female" || s=="female") return JKQTPFemale;
    if (s=="symbol_male" || s=="male") return JKQTPMale;
    if (posT.startsWith("symbol_charsym") && posT.size()>14) {
        //qDebug()<<posT<<" --> "<<posT[14]<<" "<<posT[14].unicode();
        return JKQTPCharacterSymbol+posT[14].unicode();
    }
    if (posT.startsWith("charsym") && posT.size()>7) {
        //qDebug()<<posT<<" --> "<<posT[7]<<" "<<posT[7].unicode();
        return JKQTPCharacterSymbol+posT[7].unicode();
    }
    if (posT.startsWith("char")) {
        return JKQTPCharacterSymbol+posT.mid(4).toUInt(nullptr,16);
    }
    if (posT.startsWith("symbol_char")) {
        return JKQTPCharacterSymbol+posT.mid(11).toUInt(nullptr,16);
    }
    if (posT.startsWith("symbol_fillcharsym") && posT.size()>18) {
        return JKQTPFilledCharacterSymbol+posT[18].unicode();
    }
    if (posT.startsWith("fillcharsym") && posT.size()>11) {
        return JKQTPFilledCharacterSymbol+posT[11].unicode();
    }
    if (posT.startsWith("filled_char")) {
        return JKQTPFilledCharacterSymbol+posT.mid(11).toUInt(nullptr,16);
    }
    if (posT.startsWith("symbol_filled_char")) {
        return JKQTPFilledCharacterSymbol+posT.mid(18).toUInt(nullptr,16);
    }
    if (posT.startsWith("symbol_custom")) {
        return JKQTPFirstCustomSymbol+posT.mid(13).toUInt();
    }
    return JKQTPNoSymbol;
}





void JKQTPPlotSymbol(QPaintDevice &paintDevice, double x, double y, JKQTPGraphSymbols symbol, double size, double symbolLineWidth, QColor color, QColor fillColor, const  QFont& symbolFont) {
    JKQTPEnhancedPainter p(&paintDevice);
    JKQTPPlotSymbol(p, x, y, symbol, size, symbolLineWidth, color, fillColor, symbolFont);
}

QString JKQTPLineDecoratorStyle2String(JKQTPLineDecoratorStyle pos)
{
    switch(pos) {
    case JKQTPNoDecorator: return "none";
    case JKQTPArrow: return "arrow";
    case JKQTPFilledArrow: return "filled_arrow";
    case JKQTPTriangleDecorator: return "triangle";
    case JKQTPFilledTriangleDecorator: return "filled_triangle";
    case JKQTPTriangleDecoratorAndBar: return "triangle_bar";
    case JKQTPFilledTriangleDecoratorAndBar: return "filled_triangle_bar";
    case JKQTPDoubleArrow: return "double_arrow";
    case JKQTPFilledDoubleArrow: return "filled_double_arrow";
    case JKQTPCircleDecorator: return "circle";
    case JKQTPFilledCircleDecorator: return "filled_circle";
    case JKQTPRectangleDecorator: return "rectangle";
    case JKQTPFilledRectangleDecorator: return "filled_rectangle";
    case JKQTPArrowAndBar: return "arrow_bar";
    case JKQTPDoubleArrowAndBar: return "double_arrow_bar";
    case JKQTPBarDecorator: return "bar";
    case JKQTPBracketDecorator: return "bracket";
    case JKQTPHalfBarDecorator: return "half_bar";
    case JKQTPSkewedBarDecorator: return "skewed_bar";
    case JKQTPHarpoonDecorator: return "harpoon";
    case JKQTPHarpoonDecoratorAndBar: return "harpoon_bar";
    case JKQTPDiamondDecorator: return "diamond";
    case JKQTPFilledDiamondDecorator: return "filled_diamond";
    case JKQTPDiamondDecoratorAndBar: return "diamond_bar";
    case JKQTPFilledDiamondDecoratorAndBar: return "filled_diamond_bar";
    case JKQTPLineDecoratorCount: JKQTPLineDecoratorStyle2String(JKQTPMaxLineDecoratorID);
    }
    return "";
}

QString JKQTPLineDecoratorStyle2NameString(JKQTPLineDecoratorStyle pos)
{
    switch(pos) {
    case JKQTPNoDecorator: return QObject::tr("no decorator");
    case JKQTPArrow: return QObject::tr("arrow");
    case JKQTPFilledArrow: return QObject::tr("filled arrow");
    case JKQTPTriangleDecorator: return QObject::tr("triangle");
    case JKQTPFilledTriangleDecorator: return QObject::tr("filled triangle");
    case JKQTPTriangleDecoratorAndBar: return QObject::tr("triangle, with bar");
    case JKQTPFilledTriangleDecoratorAndBar: return QObject::tr("filled triangle, with bar");
    case JKQTPDoubleArrow: return QObject::tr("double arrow");
    case JKQTPFilledDoubleArrow: return QObject::tr("filled double arrow");
    case JKQTPCircleDecorator: return QObject::tr("circle");
    case JKQTPFilledCircleDecorator: return QObject::tr("filled circle");
    case JKQTPRectangleDecorator: return QObject::tr("rectangle");
    case JKQTPFilledRectangleDecorator: return QObject::tr("filled rectangle");
    case JKQTPArrowAndBar: return QObject::tr("arrow, with bar");
    case JKQTPDoubleArrowAndBar: return QObject::tr("double arrow, with bar");
    case JKQTPBarDecorator: return QObject::tr("full bar");
    case JKQTPHalfBarDecorator: return QObject::tr("half bar");
    case JKQTPSkewedBarDecorator: return QObject::tr("skewed bar");
    case JKQTPHarpoonDecorator: return QObject::tr("harpoon");
    case JKQTPHarpoonDecoratorAndBar: return QObject::tr("harpoon, with bar");
    case JKQTPDiamondDecorator: return QObject::tr("diamond");
    case JKQTPFilledDiamondDecorator: return QObject::tr("filled diamond");
    case JKQTPDiamondDecoratorAndBar: return QObject::tr("diamond, with bar");
    case JKQTPFilledDiamondDecoratorAndBar: return QObject::tr("filled diamond, with bar");
    case JKQTPBracketDecorator: return QObject::tr("bracket");
    case JKQTPLineDecoratorCount: JKQTPLineDecoratorStyle2NameString(JKQTPMaxLineDecoratorID);
    }
    return "";
}

JKQTPLineDecoratorStyle String2JKQTPLineDecoratorStyle(const QString &pos)
{
    QString s=pos.trimmed().toLower();
    if (s=="line_decorator_none"||s=="decorator_none"||s=="no_line_decorator"||s=="no_decorator"||s=="none"||s=="-") return JKQTPNoDecorator;
    if (s=="arrow"||s=="simple_arrow"||s=="arr"||s=="->"||s=="<-"||s==">"||s=="<") return JKQTPArrow;
    if (s=="filled_arrow") return JKQTPFilledArrow;
    if (s=="triangle") return JKQTPTriangleDecorator;
    if (s=="filled_triangle") return JKQTPFilledTriangleDecorator;
    if (s=="triangle_bar") return JKQTPTriangleDecoratorAndBar;
    if (s=="filled_triangle_bar") return JKQTPFilledTriangleDecoratorAndBar;
    if (s=="double_arrow") return JKQTPDoubleArrow;
    if (s=="filled_double_arrow") return JKQTPFilledDoubleArrow;
    if (s=="circle") return JKQTPCircleDecorator;
    if (s=="filled_circle") return JKQTPFilledCircleDecorator;
    if (s=="rectangle") return JKQTPRectangleDecorator;
    if (s=="filled_rectangle") return JKQTPFilledRectangleDecorator;
    if (s=="arrow_bar") return JKQTPArrowAndBar;
    if (s=="double_arrow_bar") return JKQTPDoubleArrowAndBar;
    if (s=="bar" || s=="vertical_line") return JKQTPBarDecorator;
    if (s=="half_bar") return JKQTPHalfBarDecorator;
    if (s=="skewed_bar") return JKQTPSkewedBarDecorator;
    if (s=="harpoon") return JKQTPHarpoonDecorator;
    if (s=="harpoon_bar") return JKQTPHarpoonDecoratorAndBar;
    if (s=="diamond") return JKQTPDiamondDecorator;
    if (s=="filled_diamond") return JKQTPFilledDiamondDecorator;
    if (s=="diamond_bar") return JKQTPDiamondDecoratorAndBar;
    if (s=="filled_diamond_bar") return JKQTPFilledDiamondDecoratorAndBar;
    if (s=="bracket") return JKQTPBracketDecorator;
    return JKQTPNoDecorator;
}

double JKQTPLineDecoratorStyleCalcDecoratorSize(double line_width, double decoratorSizeFactor)
{
    if (line_width<=0.75) return 3.0+(decoratorSizeFactor*0.75-3.0)/(0.75*0.75)*line_width*line_width;
    if (line_width<=1.0) return decoratorSizeFactor*line_width;
    return decoratorSizeFactor*pow(line_width, 0.7);
}


QPolygonF JKQTPSimplifyPolyLines(const QPolygonF &lines_in, double maxDeltaXY)
{
    if (lines_in.size()<=1) return QPolygonF();
    if (lines_in.size()<=10) return lines_in;
    QPolygonF l;
    l.reserve(lines_in.size());

    int groupStart=0;
    int groupCount=0;
    QRectF groupSize(lines_in[0], QSizeF(0,0));

    auto writeGroup=[](QPolygonF& l, const int& groupStart, const int& groupCount, const QRectF& groupSize, const QPolygonF& lines_in, double maxDeltaXY) {
        // group ends
        if (groupCount>4) {
            // we can optimize the group away
            // we take the first and the last point and in between one at the top and one at the bottom of the group
            // unless we have a group that is smaller than maxDeltaXY in both directions, the the first and last point suffice
            const QPointF& x0=lines_in[groupStart];
            l<<x0; // first point
            if (groupSize.width()<=maxDeltaXY && groupSize.height()>maxDeltaXY) {
                // x-group, i.e. small on x-axis
                l<<QPointF(x0.x()+groupSize.width()/3.0, groupSize.bottom());
                l<<QPointF(x0.x()+groupSize.width()*2.0/3.0, groupSize.top());
            } else  if (groupSize.width()>maxDeltaXY && groupSize.height()<=maxDeltaXY) {
                // y-group, i.e. small on y-axis
                l<<QPointF(groupSize.left(), x0.y()+groupSize.height()/3.0);
                l<<QPointF(groupSize.right(), x0.y()+groupSize.height()*2.0/3.0);
            }
            l<<lines_in[groupStart+groupCount-1]; // last point
        } else {
            // small groups cannot be optimized away
            // so we simply copy it to the output
            for (int j=groupStart; j<groupStart+groupCount; j++) l<<lines_in[j];
        }

    };

    for (int i=0; i<lines_in.size(); i++) {
        const QPointF& x=lines_in[i];
        QRectF newGroupSize=groupSize;
        newGroupSize.setLeft(qMin(newGroupSize.left(), x.x()));
        newGroupSize.setRight(qMax(newGroupSize.right(), x.x()));
        newGroupSize.setTop(qMin(newGroupSize.top(), x.y()));
        newGroupSize.setBottom(qMax(newGroupSize.bottom(), x.y()));

        if (newGroupSize.width()<=maxDeltaXY || newGroupSize.height()<=maxDeltaXY) {
            // points still in group, so extend the group
            groupCount++;
            groupSize=newGroupSize;
        } else {
            // group ends, because adding a new point would increase it's size too much
            writeGroup(l, groupStart, groupCount, groupSize, lines_in, maxDeltaXY);
            // start new group with current point
            groupStart=i;
            groupCount=1;
            groupSize=QRectF(x, QSizeF(0,0));
        }
    }
    writeGroup(l, groupStart, groupCount, groupSize, lines_in, maxDeltaXY);
    //qDebug()<<"JKQTPSimplifyPolyLines("<<lines_in.size()<<", maxDeltaXY="<<maxDeltaXY<<") -> "<<l.size();
    return l;
}

QList<QPolygonF > JKQTPClipPolyLine(const QPolygonF &polyline_in, const QRectF &clipRect)
{
    QList<QPolygonF> l;
    l<<polyline_in;
    return JKQTPClipPolyLines(l, clipRect);
}

QList<QPolygonF > JKQTPClipPolyLines(const QList<QPolygonF > &polylines_in, const QRectF &clipRect)
{
    const double xmin=qMin(clipRect.left(), clipRect.right());
    const double xmax=qMax(clipRect.left(), clipRect.right());
    const double ymin=qMin(clipRect.top(), clipRect.bottom());
    const double ymax=qMax(clipRect.top(), clipRect.bottom());
    QList<QPolygonF> out;
    for (const QPolygonF& pl: polylines_in) {
        if (pl.size()>1) {
            if (out.size()==0 || out.last().size()>0) {
                out<<QPolygonF();
            }
            for (int i=1; i<pl.size(); i++) {
                const QLineF l(pl[i-1], pl[i]);
                const QLineF lclipped=JKQTPClipLine(l, xmin, xmax, ymin, ymax);
                //qDebug()<<"  "<<l<<"  -->  "<<lclipped<<"  clip: x="<<xmin<<".."<<xmax<<", y="<<ymin<<".."<<ymax;
                // 0-length: line remove ==> dont's add anything, start new segment if necessary
                // l=lclipped: no point was clipped: add second point, or if list empty both points (first line segment)
                // only p1 clipped, i.e. second point clipped: add second point, end segmnt (i.e. start a new one)
                // only p2 clipped, i.e. first point clipped: start new segment, add first point to it
                // p1 and p2 clipped, i.e. segment clipped on both ends: add clipped line as separate segment
                if (lclipped.length()==0) {
                    if (out.last().size()>0) {
                        out<<QPolygonF();
                    }
                } else if (l==lclipped) {
                    if (out.last().size()==0) {
                        out.last()<<lclipped.p1();
                    } else if (out.last().last()!=lclipped.p1()) {
                        out<<QPolygonF();
                        out.last()<<lclipped.p1();
                    }
                    out.last()<<lclipped.p2();
                } else if (l.p1()==lclipped.p1() && l.p2()!=lclipped.p2()) {
                    if (out.last().size()==0) {
                        out.last()<<lclipped.p1();
                    } else if (out.last().last()!=lclipped.p1()) {
                        out<<QPolygonF();
                        out.last()<<lclipped.p1();
                    }
                    out.last()<<lclipped.p2();
                    out<<QPolygonF();
                } else if (l.p1()!=lclipped.p1() && l.p2()==lclipped.p2()) {
                    if (out.last().size()==0) {
                        out.last()<<lclipped.p1();
                    } else if (out.last().last()!=lclipped.p1()) {
                        out<<QPolygonF();
                        out.last()<<lclipped.p1();
                    }
                    out.last()<<lclipped.p2();
                } else if (l.p1()!=lclipped.p1() && l.p2()!=lclipped.p2()) {
                    if (out.last().size()>0) {
                        out<<QPolygonF();
                    }
                    out.last()<<lclipped.p1();
                    out.last()<<lclipped.p2();
                    out<<QPolygonF();
                }
            }
        }
    }
    return out;
}

QList<QPolygonF> JKQTPSimplifyPolyLines(const QList<QPolygonF> &lines_in, double maxDeltaXY)
{
    QList<QPolygonF> l;
    for (const QPolygonF& p: lines_in) {
        l<<JKQTPSimplifyPolyLines(p, maxDeltaXY);
    }
    return l;
}


QVector<JKQTPCustomGraphSymbolFunctor> JKQTPlotterDrawingTools::JKQTPCustomGraphSymbolStore=QVector<JKQTPCustomGraphSymbolFunctor>();

JKQTPGraphSymbols JKQTPRegisterCustomGraphSymbol(JKQTPCustomGraphSymbolFunctor&& f)
{
    JKQTPlotterDrawingTools::JKQTPCustomGraphSymbolStore.push_back(std::move(f));
    return JKQTPFirstCustomSymbol+(JKQTPlotterDrawingTools::JKQTPCustomGraphSymbolStore.size()-1);
}

JKQTPGraphSymbols JKQTPRegisterCustomGraphSymbol(const JKQTPCustomGraphSymbolFunctor& f)
{
    JKQTPlotterDrawingTools::JKQTPCustomGraphSymbolStore.push_back(f);
    return JKQTPFirstCustomSymbol+(JKQTPlotterDrawingTools::JKQTPCustomGraphSymbolStore.size()-1);
}
