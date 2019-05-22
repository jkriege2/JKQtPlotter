/*
Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

    

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



#include "jkqtcommon/jkqtptools.h"
#include "jkqtplottertools/jkqtpenhancedpainter.h"
#include <cmath>
#include <QDebug>
#include <QSet>
#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QAction>
#include <QLocale>
#include <QDialog>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QPrinter>
#include <QHeaderView>
#include <QPrintDialog>
#include <QDialogButtonBox>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctype.h>
#include <locale>





QString JKQTPCADrawMode2String(JKQTPCADrawMode pos) {
    switch(pos) {
        case JKQTPCADMcomplete: return "all";
        case JKQTPCADMTicksTickLabelsAxisLabel: return "ticks+labels+axislabel";
        case JKQTPCADMTicksTickLabels: return "ticks+labels";
        case JKQTPCADMTicks: return "ticks";
        case JKQTPCADMLineTicksTickLabels: return "line+ticks+labels";
        case JKQTPCADMTickLabelsAxisLabel: return "ticks+labels";
        case JKQTPCADMTickLabels: return "labels";
        case JKQTPCADMLineTicks: return "line+ticks";
        case JKQTPCADMLine: return "line";
        case JKQTPCADMnone: return "none";
    }
    return "";
}



JKQTPCADrawMode String2JKQTPCADrawMode(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="all" || s=="complete" || s=="line+ticks+labels+axislabel") return JKQTPCADMcomplete;
    if (s=="ticks+labels") return JKQTPCADMLineTicksTickLabels;
    if (s=="ticks+labels+axislabel") return JKQTPCADMTicksTickLabelsAxisLabel;
    if (s=="labels+axislabel") return JKQTPCADMTickLabelsAxisLabel;
    if (s=="ticks+labels") return JKQTPCADMTicksTickLabels;
    if (s=="line+ticks+labels") return JKQTPCADMLineTicksTickLabels;
    if (s=="labels") return JKQTPCADMTickLabels;
    if (s=="line+ticks") return JKQTPCADMLineTicks;
    if (s=="ticks") return JKQTPCADMTicks;
    if (s=="line") return JKQTPCADMLine;
    if (s=="none") return JKQTPCADMnone;
    return JKQTPCADMnone;
}




QString JKQTPLabelTickMode2String(JKQTPLabelTickMode pos) {
    switch(pos) {
        case JKQTPLTMLinOrPower: return "lin_or_power";
        case JKQTPLTMLin: return "lin";
        case JKQTPLTMPower: return "power";
    }
    return "";
}

JKQTPLabelTickMode String2JKQTPLabelTickMode(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="lin_or_power") return JKQTPLTMLinOrPower;
    if (s=="lin") return JKQTPLTMLin;
    if (s=="power") return JKQTPLTMPower;
    return JKQTPLTMLinOrPower;
}



QString JKQTPCALabelType2String(JKQTPCALabelType pos) {
    switch(pos) {
        case JKQTPCALTdefault: return "default";
        case JKQTPCALTexponentCharacter: return "exponent_character";
        case JKQTPCALTexponent: return "exponent";
        case JKQTPCALTtime: return "time";
        case JKQTPCALTdate: return "date";
        case JKQTPCALTdatetime: return "datetime";
    }
    return "";
}

JKQTPCALabelType String2JKQTPCALabelType(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="default") return JKQTPCALTdefault;
    if (s=="exponent_character") return JKQTPCALTexponentCharacter;
    if (s=="exponent") return JKQTPCALTexponent;
    if (s=="time") return JKQTPCALTtime;
    if (s=="date") return JKQTPCALTdate;
    if (s=="datetime") return JKQTPCALTdatetime;
    return JKQTPCALTdefault;
}

QString JKQTPLabelPosition2String(JKQTPLabelPosition pos) {
    switch(pos) {
        case JKQTPLabelMin: return "min";
        case JKQTPLabelMax: return "max";
        case JKQTPLabelCenter: return "center";
    }
    return "";
}

JKQTPLabelPosition String2JKQTPLabelPosition(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="min") return JKQTPLabelMin;
    if (s=="max") return JKQTPLabelMax;
    if (s=="center") return JKQTPLabelCenter;
    return JKQTPLabelCenter;
}

QString JKQTPKeyPosition2String(JKQTPKeyPosition pos) {
    switch(pos) {
        case JKQTPKeyOutsideLeftBottom: return "outside_leftbottom";
        case JKQTPKeyOutsideTopLeft: return "outside_topleft";
        case JKQTPKeyOutsideTopRight: return "outside_topright";
        case JKQTPKeyOutsideLeftTop: return "outside_lefttop";

        case JKQTPKeyOutsideRightBottom: return "outside_rightbottom";
        case JKQTPKeyOutsideBottomLeft: return "outside_bottomleft";
        case JKQTPKeyOutsideBottomRight: return "outside_bottomright";
        case JKQTPKeyOutsideRightTop: return "outside_righttop";

        case JKQTPKeyInsideBottomRight: return "inside_bottomright";
        case JKQTPKeyInsideTopLeft: return "inside_topleft";
        case JKQTPKeyInsideTopRight: return "inside_topright";
        case JKQTPKeyInsideBottomLeft: return "inside_bottomleft";
    }
    return "";
}

JKQTPKeyPosition String2JKQTPKeyPosition(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="outside_bottom" || s=="outsidebottom" || s=="outside_leftbottom" || s=="outsideleftbottom" || s=="olb") return JKQTPKeyOutsideLeftBottom;
    if (s=="outside_left" || s=="outsideleft" || s=="outside_topleft" || s=="outsidetopleft" || s=="otl") return JKQTPKeyOutsideTopLeft;
    if (s=="outside_right" || s=="outsideright" || s=="outside_topright" || s=="outsidetopright" || s=="otr") return JKQTPKeyOutsideTopRight;
    if (s=="outside_top" || s=="outsidetop" || s=="outside_lefttop" || s=="outsidelefttop" || s=="olt") return JKQTPKeyOutsideLeftTop;

    if (s=="outside_rightbottom" || s=="outsiderightbottom" || s=="orb" ) return JKQTPKeyOutsideRightBottom;
    if (s=="outside_bottomleft" || s=="outsidebottomleft" || s=="obl" ) return JKQTPKeyOutsideBottomLeft;
    if (s=="outside_bottomright" || s=="outsidebottomright" || s=="obr" ) return JKQTPKeyOutsideBottomRight;
    if (s=="outside_righttop" || s=="outsiderighttop" || s=="ort" ) return JKQTPKeyOutsideRightTop;

    if (s=="inside_bottom" || s=="insidebottom" || s=="inside_bottomright" || s=="insidebottomright" || s=="ibr") return JKQTPKeyInsideBottomRight;
    if (s=="inside_top" || s=="insidetop" || s=="inside_left" || s=="insideleft" || s=="inside_topleft" || s=="insidetopleft" || s=="itl") return JKQTPKeyInsideTopLeft;
    if (s=="inside_right" || s=="insideright" || s=="inside_topright" || s=="insidetopright" || s=="itr") return JKQTPKeyInsideTopRight;
    if (s=="inside_bottomleft" || s=="insidebottomleft"  || s=="ibl") return JKQTPKeyInsideBottomLeft;
    return JKQTPKeyInsideTopRight;
}


QString JKQTPKeyLayout2String(JKQTPKeyLayout pos) {
    switch(pos) {
        case JKQTPKeyLayoutOneColumn: return "one_column";
        case JKQTPKeyLayoutOneRow: return "one_row";
        case JKQTPKeyLayoutMultiColumn: return "multi_column";
    }
    return "";
}

JKQTPKeyLayout String2JKQTPKeyLayout(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="one_column" || s=="onecolumn" || s=="one") return JKQTPKeyLayoutOneColumn;
    if (s=="one_row" || s=="onerow") return JKQTPKeyLayoutOneRow;
    if (s=="multi_column" || s=="multicolumn" || s=="multi") return JKQTPKeyLayoutMultiColumn;
    return JKQTPKeyLayoutOneColumn;
}


QString JKQTPErrorPlotstyle2String(JKQTPErrorPlotstyle pos) {
    switch(pos) {
        case JKQTPErrorBoxes: return "error_box";
        case JKQTPErrorEllipses: return "error_ell";
        case JKQTPErrorLines: return "error_lines";
        case JKQTPErrorBars: return "error_bars";
        case JKQTPErrorSimpleBars: return "error_simplebars";
        case JKQTPErrorPolygons: return "error_polygons";
        case JKQTPErrorBarsLines: return "error_bars_lines";
        case JKQTPErrorBarsPolygons: return "error_bars_polygons";
        case JKQTPErrorSimpleBarsLines: return "error_simplebars_lines";
        case JKQTPErrorSimpleBarsPolygons: return "error_simplebars_polygons";
        case JKQTPNoError: return "error_none";
    }
    return "";
}

JKQTPErrorPlotstyle String2JKQTPErrorPlotstyle(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="error_lines") return JKQTPErrorLines;
    if (s=="error_box") return JKQTPErrorBoxes;
    if (s=="error_ell") return JKQTPErrorEllipses;
    if (s=="error_bars") return JKQTPErrorBars;
    if (s=="error_simplebars") return JKQTPErrorSimpleBars;
    if (s=="error_polygons") return JKQTPErrorPolygons;
    if (s=="error_bars_lines") return JKQTPErrorBarsLines;
    if (s=="error_bars_polygons") return JKQTPErrorBarsPolygons;
    if (s=="error_simplebars_lines") return JKQTPErrorSimpleBarsLines;
    if (s=="error_simplebars_polygons") return JKQTPErrorSimpleBarsPolygons;
    return JKQTPNoError;
}





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
    }
    return "";
}
JKQTPGraphSymbols String2JKQTPGraphSymbols(const QString& pos)  {
    QString s=pos.trimmed().toLower();
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

    return JKQTPNoSymbol;
}






QString JKQTPSpecialLineType2String(JKQTPSpecialLineType pos)
{
    switch(pos) {
        case JKQTPStepLeft: return "step_left";
        case JKQTPStepCenter: return "step_center";
        case JKQTPStepRight: return "step_right";
        case JKQTPStepAverage: return "step_average";
        case JKQTPDirectLine: return "direct_line";
    }
    return "";
}

JKQTPSpecialLineType String2JKQTPSpecialLineType(const QString& pos)
{
    QString s=pos.trimmed().toLower();
    if (s=="step_left"||s=="left"||s=="l") return JKQTPStepLeft;
    if (s=="step_center"||s=="center"||s=="c") return JKQTPStepCenter;
    if (s=="step_right"||s=="right"||s=="r") return JKQTPStepRight;
    if (s=="step_average"||s=="step_avg"||s=="average"||s=="avg"||s=="a") return JKQTPStepAverage;
    if (s=="direct_line"||s=="line"||s=="d") return JKQTPDirectLine;

    return JKQTPStepLeft;
}


QString JKQTPContextMenuModes2String(JKQTPContextMenuModes act)
{
    if (act==jkqtpcmmStandardContextMenu) return "standard";
    if (act==jkqtpcmmSpecialContextMenu) return "special";
    if (act==jkqtpcmmStandardAndSpecialContextMenu) return "both";
    if (act==jkqtpcmmNoContextMenu) return "none";
    return "none";
}

JKQTPContextMenuModes String2JKQTPContextMenuModes(const QString &act)
{
    QString s=act.trimmed().toLower();
    if (s=="jkqtpcmmstandardcontextmenu"||s=="standardcontextmenu" ||s=="standard") return jkqtpcmmStandardContextMenu;
    if (s=="jkqtpcmmspecialcontextmenu"||s=="specialcontextmenu"||s=="special") return jkqtpcmmSpecialContextMenu;
    if (s=="jkqtpcmmstandardandspecialcontextmenu"||s=="standardandspecialcontextmenu"||s=="standardandspecial"||s=="standard+special"||s=="both") return jkqtpcmmStandardAndSpecialContextMenu;

    return jkqtpcmmNoContextMenu;
}

JKQTPMouseWheelActions String2JKQTPMouseWheelActions(const QString &act)
{
    QString s=act.trimmed().toLower();
    if (s=="jkqtpmwazoombywheel"||s=="zoombywheel" ||s=="zoom") return jkqtpmwaZoomByWheel;
    if (s=="jkqtpmwapanbywheel"||s=="panbywheel"||s=="pan") return jkqtpmwaPanByWheel;
    return jkqtpmwaZoomByWheel;

}

QString JKQTPMouseWheelActions2String(JKQTPMouseWheelActions act)
{
    if (act==jkqtpmwaZoomByWheel) return "zoom";
    if (act==jkqtpmwaPanByWheel) return "pan";
    return "unknown";
}

JKQTPMouseDoubleClickActions String2JKQTPMouseDoubleClickActions(const QString &act)
{
    QString s=act.trimmed().toLower();
    if (s=="jkqtpdcaclickzoomsin"||s=="clickzoomsin" ||s=="zoomsin" ||s=="zoomin") return jkqtpdcaClickZoomsIn;
    if (s=="jkqtpdcaclickzoomsout"||s=="clickzoomsout" ||s=="zoomsout" ||s=="zoomout") return jkqtpdcaClickZoomsOut;
    if (s=="jkqtpdcaclickopenscontextmenu"||s=="openscontextmenu"||s=="opencontextmenu"||s=="contextmenu") return jkqtpdcaClickOpensContextMenu;
    if (s=="jkqtpdcaclickopensspecialcontextmenu"||s=="opensspecialcontextmenu"||s=="openspecialcontextmenu"||s=="specialcontextmenu") return jkqtpdcaClickOpensSpecialContextMenu;
    if (s=="jkqtpdcaclickmovesviewport"||s=="clickmovesviewport"||s=="movesviewport"||s=="moveviewport"||s=="moveview") return jkqtpdcaClickMovesViewport;

    return jkqtpdcaClickZoomsIn;
}

QString JKQTPMouseDoubleClickActions2String(JKQTPMouseDoubleClickActions act)
{
    if (act==jkqtpdcaClickZoomsIn) return "zoomin";
    if (act==jkqtpdcaClickZoomsOut) return "zoomout";
    if (act==jkqtpdcaClickOpensContextMenu) return "contextmenu";
    if (act==jkqtpdcaClickOpensSpecialContextMenu) return "specialcontextmenu";
    if (act==jkqtpdcaClickMovesViewport) return "moveviewport";
    return "unknown";

}

JKQTPMouseDragActions String2JKQTPMouseDragActions(const QString &act)
{
    QString s=act.trimmed().toLower();
    if (s=="jkqtpmdapanonmove"||s=="panonmove" ||s=="panmove") return jkqtpmdaPanPlotOnMove;
    if (s=="jkqtpmdapanonrelease"||s=="panonrelease" ||s=="panrelease") return jkqtpmdaPanPlotOnRelease;
    if (s=="jkqtpmdazoombyrectangle"||s=="zoomrectangle"||s=="zoomrect"||s=="zoombyrectangle"||s=="zoombyrect") return jkqtpmdaZoomByRectangle;
    if (s=="jkqtpmdadrawrectforevent"||s=="drawrectangle"||s=="drawrect"||s=="rectangle"||s=="rect") return jkqtpmdaDrawRectangleForEvent;
    if (s=="jkqtpmdadrawrectforevent"||s=="drawcircle"||s=="circle") return jkqtpmdaDrawCircleForEvent;
    if (s=="jkqtpmdadrawrectforevent"||s=="drawellipse"||s=="ellipse") return jkqtpmdaDrawEllipseForEvent;
    if (s=="jkqtpmdadrawrectforevent"||s=="drawline"||s=="line") return jkqtpmdaDrawLineForEvent;
    if (s=="jkqtpmdascribbleforevents"||s=="scribble") return jkqtpmdaScribbleForEvents;
    if (s=="jkqtpmdatooltipforclosestdatapoint"||s=="closestdatapointtooltip"||s=="tooltipforclosestdatapoint"||s=="tooltip") return jkqtpmdaToolTipForClosestDataPoint;
    if (s=="jkqtpmdaruler"||s=="ruler") return jkqtpmdaRuler;


    return jkqtpmdaZoomByRectangle;

}

QString JKQTPMouseDragActions2String(JKQTPMouseDragActions act)
{
    if (act==jkqtpmdaPanPlotOnMove) return "PanOnMove";
    if (act==jkqtpmdaPanPlotOnRelease) return "PanOnRelease";
    if (act==jkqtpmdaZoomByRectangle) return "ZoomRectangle";
    if (act==jkqtpmdaDrawRectangleForEvent) return "DrawRectangle";
    if (act==jkqtpmdaDrawCircleForEvent) return "DrawCircle";
    if (act==jkqtpmdaDrawEllipseForEvent) return "DrawEllipse";
    if (act==jkqtpmdaDrawLineForEvent) return "DrawLine";
    if (act==jkqtpmdaScribbleForEvents) return "Scribble";
    if (act==jkqtpmdaToolTipForClosestDataPoint) return "ToolTipForClosestDataPoint";
    if (act==jkqtpmdaRuler) return "Ruler";
    return "unknown";
}


bool JKQTPCADrawModeHasLine(JKQTPCADrawMode pos)
{
    return (pos==JKQTPCADMcomplete) || (pos==JKQTPCADMLineTicksTickLabels) || (pos==JKQTPCADMLineTicks) || (pos==JKQTPCADMLine);
}

bool JKQTPCADrawModeHasTicks(JKQTPCADrawMode pos)
{
    return (pos==JKQTPCADMcomplete) || (pos==JKQTPCADMTicksTickLabelsAxisLabel) || (pos==JKQTPCADMLineTicks) || (pos==JKQTPCADMLineTicksTickLabels) || (pos==JKQTPCADMTicksTickLabels)|| (pos==JKQTPCADMTicks);
}

bool JKQTPCADrawModeHasTickLabels(JKQTPCADrawMode pos)
{
    return (pos==JKQTPCADMcomplete) || (pos==JKQTPCADMTicksTickLabelsAxisLabel) || (pos==JKQTPCADMLineTicksTickLabels) || (pos==JKQTPCADMTicksTickLabels) || (pos==JKQTPCADMTickLabels) || (pos==JKQTPCADMTickLabelsAxisLabel);
}

bool JKQTPCADrawModeHasAxisLabel(JKQTPCADrawMode pos)
{
    return (pos==JKQTPCADMcomplete) || (pos==JKQTPCADMTicksTickLabelsAxisLabel) || (pos==JKQTPCADMTickLabelsAxisLabel);
}

QString JKQTPColorDerivationMode2String(JKQTPColorDerivationMode mode)
{
    switch(mode) {
        case JKQTPFFCMSameColor: return "same";
        case JKQTPFFCMInvertedColor: return "inverted";
        case JKQTPFFCMLighterColor: return "lighter";
        case JKQTPFFCMEvenLighterColor: return "even_lighter";
        case JKQTPFFCMDarkerColor: return "darker";
        case JKQTPFFCMEvenDarkerColor: return "even_darker";
        case JKQTPFFCMMoreTransparentColor: return "more_transparent";
        case JKQTPFFCMEvenMoreTransparentColor: return "even_more_transparent";
        case JKQTPFFCMLessTransparentColor: return "less_transparent";
        case JKQTPFFCMEvenLessTransparentColor: return "even_less_transparent";
    }
    return "same";
}

JKQTPColorDerivationMode String2JKQTPColorDerivationMode(const QString &mode)
{
    QString m=mode.trimmed().toLower();
    if (m=="same") return JKQTPFFCMSameColor;
    if (m=="inverted") return JKQTPFFCMInvertedColor;
    if (m=="lighter") return JKQTPFFCMLighterColor;
    if (m=="even_lighter") return JKQTPFFCMEvenLighterColor;
    if (m=="darker") return JKQTPFFCMDarkerColor;
    if (m=="even_darker") return JKQTPFFCMEvenDarkerColor;
    if (m=="more_transparent") return JKQTPFFCMMoreTransparentColor;
    if (m=="even_more_transparent") return JKQTPFFCMEvenMoreTransparentColor;
    if (m=="less_transparent") return JKQTPFFCMLessTransparentColor;
    if (m=="even_less_transparent") return JKQTPFFCMEvenLessTransparentColor;
    return JKQTPFFCMSameColor;
}

QColor JKQTPGetDerivedColor(JKQTPColorDerivationMode mode, const QColor &col)
{
    switch(mode) {
        case JKQTPFFCMSameColor: return col;
        case JKQTPFFCMInvertedColor: return QColor(255-col.red(), 255-col.green(), 255-col.blue(), col.alpha());
        case JKQTPFFCMLighterColor: return col.lighter();
        case JKQTPFFCMEvenLighterColor: return col.lighter().lighter();
        case JKQTPFFCMDarkerColor: return col.darker();
        case JKQTPFFCMEvenDarkerColor: return col.darker().darker();
        case JKQTPFFCMMoreTransparentColor: { QColor c=col; c.setAlphaF(0.66*c.alphaF()); return c; }
        case JKQTPFFCMEvenMoreTransparentColor: { QColor c=col; c.setAlphaF(0.33*c.alphaF()); return c; }
        case JKQTPFFCMLessTransparentColor: { QColor c=col; c.setAlphaF(c.alphaF()+(1.0-c.alphaF())*0.33); return c; }
        case JKQTPFFCMEvenLessTransparentColor: { QColor c=col; c.setAlphaF(c.alphaF()+(1.0-c.alphaF())*0.66); return c; }
    }
    return col;
}

QString JKQTPUserActionMarkerType2String(JKQTPUserActionMarkerType act)
{
    switch(act) {
        case jkqtpuamtCircle: return "circle";
        case jkqtpuamtCrossHair: return "crosshair";
        case jkqtpuamtCircleAndCrossHair: return "circle+crosshair";
    }
    return "circle";
}

JKQTPUserActionMarkerType String2JKQTPUserActionMarkerType(const QString &act)
{
    QString m=act.trimmed().toLower();
    if (m=="circle") return jkqtpuamtCircle;
    if (m=="crosshair" || m=="cross") return jkqtpuamtCrossHair;
    if (m=="circle+crosshair" || m=="circle+cross") return jkqtpuamtCircleAndCrossHair;
    return jkqtpuamtCircle;
}
