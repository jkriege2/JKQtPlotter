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



#include "jkqtplotter/jkqtptools.h"
#include <cmath>
#include <QDebug>
#include <QSet>
#include <QApplication>
#include <QClipboard>
#include <QKeyEvent>
#include <QAction>
#include <QLocale>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctype.h>
#include <locale>





QString JKQTPCADrawMode2String(JKQTPCADrawMode pos) {
    if (pos==JKQTPCADMnone) return "none";
    if (pos==JKQTPCADMcomplete) return "complete";
    if (pos==JKQTPCADMcompleteMaxArrow) return "complete+max_filled_arrow";
    if (pos==JKQTPCADMcompleteMinMaxArrow) return "complete+max_filled_arrow+min_filled_arrow";

    QString s="";
    if (pos.testFlag(JKQTPCADMLine)) JKQTPExtendString(s, "+", "line");
    if (pos.testFlag(JKQTPCADMMaxArrow)) JKQTPExtendString(s, "+", "max_arrow");
    if (pos.testFlag(JKQTPCADMMaxFilledArrow)) JKQTPExtendString(s, "+", "max_filled_arrow");
    if (pos.testFlag(JKQTPCADMMinArrow)) JKQTPExtendString(s, "+", "min_arrow");
    if (pos.testFlag(JKQTPCADMMinFilledArrow)) JKQTPExtendString(s, "+", "min_filled_arrow");
    if (pos.testFlag(JKQTPCADMTicks)) JKQTPExtendString(s, "+", "ticks");
    if (pos.testFlag(JKQTPCADMTickLabels)) JKQTPExtendString(s, "+", "labels");
    if (pos.testFlag(JKQTPCADMAxisLabel)) JKQTPExtendString(s, "+", "axislabel");

    return s;
}



JKQTPCADrawMode String2JKQTPCADrawMode(const QString& pos) {
    QStringList slist=pos.trimmed().toLower().split('+');

    JKQTPCADrawMode res=JKQTPCADMnone;
    for (const QString& s: slist) {
        if (s=="all" || s=="complete") res |= JKQTPCADMcomplete;
        if (s=="labels" || s=="ticklabels") res |= JKQTPCADMTickLabels;
        if (s=="axislabel") res |= JKQTPCADMAxisLabel;
        if (s=="ticks") res |= JKQTPCADMTicks;
        if (s=="line") res |= JKQTPCADMLine;
        if (s=="max_arrow"||s=="maxarrow") res |= JKQTPCADMMaxArrow;
        if (s=="max_filled_arrow"||s=="maxfilledarrow") res |= JKQTPCADMMaxFilledArrow;
        if (s=="min_arrow"||s=="minarrow") res |= JKQTPCADMMinArrow;
        if (s=="min_filled_arrow"||s=="minfilledarrow") res |= JKQTPCADMMinFilledArrow;
    }
    return res;
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
        case JKQTPCALTscientific: return "scientific";
        case JKQTPCALTintfrac: return "intfrac";
        case JKQTPCALTintsfrac: return "intsfrac";
        case JKQTPCALTintslashfrac: return "intslashfrac";
        case JKQTPCALTfrac: return "frac";
        case JKQTPCALTsfrac: return "sfrac";
        case JKQTPCALTslashfrac: return "slashfrac";
        case JKQTPCALTprintf: return "printf";
        case JKQTPCALTcount: return "";
    }
    return "";
}

JKQTPCALabelType String2JKQTPCALabelType(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="default" || s=="f") return JKQTPCALTdefault;
    if (s=="scientifc" || s=="g") return JKQTPCALTscientific;
    if (s=="exponent_character") return JKQTPCALTexponentCharacter;
    if (s=="exponent") return JKQTPCALTexponent;
    if (s=="time") return JKQTPCALTtime;
    if (s=="date") return JKQTPCALTdate;
    if (s=="datetime") return JKQTPCALTdatetime;
    if (s=="frac") return JKQTPCALTfrac;
    if (s=="sfrac") return JKQTPCALTsfrac;
    if (s=="slashfrac") return JKQTPCALTslashfrac;
    if (s=="intfrac") return JKQTPCALTintfrac;
    if (s=="intsfrac") return JKQTPCALTintsfrac;
    if (s=="intslashfrac") return JKQTPCALTintslashfrac;
    if (s=="printf" || s=="sprintf") return JKQTPCALTprintf;
    return JKQTPCALTdefault;
}

QString JKQTPLabelPosition2String(JKQTPLabelPosition pos) {
    switch(pos) {
    case JKQTPLabelMin: return "min";
    case JKQTPLabelMax: return "max";
    case JKQTPLabelMinBesides: return "minBesides";
    case JKQTPLabelMaxBesides: return "maxBesides";
    case JKQTPLabelCenter: return "center";
    }
    return "";
}

JKQTPLabelPosition String2JKQTPLabelPosition(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="min") return JKQTPLabelMin;
    if (s=="max") return JKQTPLabelMax;
    if (s=="minbesides") return JKQTPLabelMinBesides;
    if (s=="maxbesides") return JKQTPLabelMaxBesides;
    if (s=="center") return JKQTPLabelCenter;
    return JKQTPLabelCenter;
}

QString JKQTPKeyPosition2String(JKQTPKeyPosition pos) {
    switch(pos) {
        case JKQTPKeyOutsideLeftBottom: return "outside_leftbottom";
        case JKQTPKeyOutsideLeftTop: return "outside_lefttop";
        case JKQTPKeyOutsideLeftCenter: return "outside_leftcenter";

        case JKQTPKeyOutsideTopLeft: return "outside_topleft";
        case JKQTPKeyOutsideTopRight: return "outside_topright";
        case JKQTPKeyOutsideTopCenter: return "outside_topcenter";

        case JKQTPKeyOutsideRightBottom: return "outside_rightbottom";
        case JKQTPKeyOutsideRightTop: return "outside_righttop";
        case JKQTPKeyOutsideRightCenter: return "outside_rightcenter";

        case JKQTPKeyOutsideBottomLeft: return "outside_bottomleft";
        case JKQTPKeyOutsideBottomRight: return "outside_bottomright";
        case JKQTPKeyOutsideBottomCenter: return "outside_bottomcenter";

        case JKQTPKeyInsideBottomRight: return "inside_bottomright";
        case JKQTPKeyInsideTopLeft: return "inside_topleft";
        case JKQTPKeyInsideTopRight: return "inside_topright";
        case JKQTPKeyInsideBottomLeft: return "inside_bottomleft";
        case JKQTPKeyInsideTop: return "inside_top";
        case JKQTPKeyInsideBottom: return "inside_bottom";
        case JKQTPKeyInsideLeft: return "inside_left";
        case JKQTPKeyInsideRight: return "inside_right";
    }

    QString res;
    if (pos.testFlag(JKQTPKeyInside)) res+=std::string(res.size()>0?"|":"")+"inside";
    else if (pos.testFlag(JKQTPKeyOutsideTop)) res+=std::string(res.size()>0?"|":"")+"outside_top";
    else if (pos.testFlag(JKQTPKeyOutsideBottom)) res+=std::string(res.size()>0?"|":"")+"outside_bottom";
    else if (pos.testFlag(JKQTPKeyOutsideLeft)) res+=std::string(res.size()>0?"|":"")+"outside_left";
    else if (pos.testFlag(JKQTPKeyOutsideRight)) res+=std::string(res.size()>0?"|":"")+"outside_right";

    if (pos.testFlag(JKQTPKeyLeft)) res+=std::string(res.size()>0?"|":"")+"left";
    else if (pos.testFlag(JKQTPKeyHCenter)) res+=std::string(res.size()>0?"|":"")+"hcenter";
    else if (pos.testFlag(JKQTPKeyRight)) res+=std::string(res.size()>0?"|":"")+"right";

    if (pos.testFlag(JKQTPKeyTop)) res+=std::string(res.size()>0?"|":"")+"top";
    else if (pos.testFlag(JKQTPKeyVCenter)) res+=std::string(res.size()>0?"|":"")+"vcenter";
    else if (pos.testFlag(JKQTPKeyBottom)) res+=std::string(res.size()>0?"|":"")+"bottom";

    return res;
}



JKQTPKeyPosition String2JKQTPKeyPosition(const QString& pos) {
    const QString s=pos.trimmed().toLower();
    if (s=="outside_bottom" || s=="outsidebottom" || s=="outside_leftbottom" || s=="outsideleftbottom" || s=="olb") return JKQTPKeyOutsideLeftBottom;
    if (s=="outside_left" || s=="outsideleft" || s=="outside_topleft" || s=="outsidetopleft" || s=="otl") return JKQTPKeyOutsideTopLeft;
    if (s=="outside_right" || s=="outsideright" || s=="outside_topright" || s=="outsidetopright" || s=="otr") return JKQTPKeyOutsideTopRight;
    if (s=="outside_top" || s=="outsidetop" || s=="outside_lefttop" || s=="outsidelefttop" || s=="olt") return JKQTPKeyOutsideLeftTop;

    if (s=="outside_leftcenter" || s=="outsideleftcenter" || s=="olc") return JKQTPKeyOutsideLeftCenter;
    if (s=="outside_rightcenter" || s=="outsiderightcenter" || s=="orc") return JKQTPKeyOutsideRightCenter;
    if (s=="outside_topcenter" || s=="outsidetopcenter" || s=="otc") return JKQTPKeyOutsideTopCenter;
    if (s=="outside_bottomcenter" || s=="outsidebottomcenter" || s=="obc") return JKQTPKeyOutsideBottomCenter;

    if (s=="outside_rightbottom" || s=="outsiderightbottom" || s=="orb" ) return JKQTPKeyOutsideRightBottom;
    if (s=="outside_bottomleft" || s=="outsidebottomleft" || s=="obl" ) return JKQTPKeyOutsideBottomLeft;
    if (s=="outside_bottomright" || s=="outsidebottomright" || s=="obr" ) return JKQTPKeyOutsideBottomRight;
    if (s=="outside_righttop" || s=="outsiderighttop" || s=="ort" ) return JKQTPKeyOutsideRightTop;

    if (s=="inside_bottom" || s=="insidebottom" || s=="inside_bottomright" || s=="insidebottomright" || s=="ibr") return JKQTPKeyInsideBottomRight;
    if (s=="inside_top" || s=="insidetop" || s=="inside_left" || s=="insideleft" || s=="inside_topleft" || s=="insidetopleft" || s=="itl") return JKQTPKeyInsideTopLeft;
    if (s=="inside_right" || s=="insideright" || s=="inside_topright" || s=="insidetopright" || s=="itr") return JKQTPKeyInsideTopRight;
    if (s=="inside_bottomleft" || s=="insidebottomleft"  || s=="ibl") return JKQTPKeyInsideBottomLeft;

    if (s.contains("|")) {
        JKQTPKeyPosition pos;
        const QStringList ss=s.split("|");
        for (const auto& i: ss) {
            if (i=="inside") pos|=JKQTPKeyInside;
            if (i=="outside_top") pos|=JKQTPKeyOutsideTop;
            if (i=="outside_bottom") pos|=JKQTPKeyOutsideBottom;
            if (i=="outside_left") pos|=JKQTPKeyOutsideLeft;
            if (i=="outside_right") pos|=JKQTPKeyOutsideRight;

            if (i=="left") pos|=JKQTPKeyLeft;
            if (i=="hcenter") pos|=JKQTPKeyHCenter;
            if (i=="right") pos|=JKQTPKeyRight;

            if (i=="top") pos|=JKQTPKeyTop;
            if (i=="vcenter") pos|=JKQTPKeyVCenter;
            if (i=="bottom") pos|=JKQTPKeyBottom;
        }
        return pos;
    }

    return JKQTPKeyInsideTopRight;
}


QString JKQTPKeyLayout2String(JKQTPKeyLayout pos) {
    switch(pos) {
        case JKQTPKeyLayoutOneColumn: return "one_column";
        case JKQTPKeyLayoutOneRow: return "one_row";
        case JKQTPKeyLayoutMultiColumn: return "multi_column";
        case JKQTPKeyLayoutMultiRow: return "multi_row";
    }
    return "";
}

JKQTPKeyLayout String2JKQTPKeyLayout(const QString& pos) {
    QString s=pos.trimmed().toLower();
    if (s=="one_column" || s=="onecolumn" || s=="one") return JKQTPKeyLayoutOneColumn;
    if (s=="one_row" || s=="onerow") return JKQTPKeyLayoutOneRow;
    if (s=="multi_column" || s=="multicolumn" || s=="multi_columns" || s=="multicolumns" || s=="multi" || s=="columns") return JKQTPKeyLayoutMultiColumn;
    if (s=="multi_row" || s=="multirow" || s=="multi_rows" || s=="multirows" || s=="rows") return JKQTPKeyLayoutMultiRow;
    return JKQTPKeyLayoutOneColumn;
}


QString JKQTPErrorPlotstyle2String(JKQTPErrorPlotstyle pos) {
    QString s="";
    if (pos==JKQTPNoError) return "error_none";
    if (pos.testFlag(JKQTPErrorSimpleBars)&&pos.testFlag(JKQTPErrorIndicatorBar)) JKQTPExtendString(s, "+", "error_bars");
    else if (pos.testFlag(JKQTPErrorSimpleBars)&&pos.testFlag(JKQTPErrorIndicatorArrows)) JKQTPExtendString(s, "+", "error_arrows");
    else if (pos.testFlag(JKQTPErrorSimpleBars)&&pos.testFlag(JKQTPErrorIndicatorInwardArrows)) JKQTPExtendString(s, "+", "error_inwardarrows");
    else if (pos.testFlag(JKQTPErrorSimpleBars)) JKQTPExtendString(s, "+", "error_simplebars");

    if (pos.testFlag(JKQTPErrorLines)) JKQTPExtendString(s, "+", "error_lines");
    if (pos.testFlag(JKQTPErrorPolygons)) JKQTPExtendString(s, "+", "error_polygons");
    if (pos.testFlag(JKQTPErrorEllipses)) JKQTPExtendString(s, "+", "error_ell");
    if (pos.testFlag(JKQTPErrorBoxes)) JKQTPExtendString(s, "+", "error_box");

    if (pos.testFlag(JKQTPErrorDirectionOutwards)) JKQTPExtendString(s, "+", "outwards");
    if (pos.testFlag(JKQTPErrorDirectionInwards)) JKQTPExtendString(s, "+", "inwards");
    if (pos.testFlag(JKQTPErrorDirectionAbove)) JKQTPExtendString(s, "+", "above");
    if (pos.testFlag(JKQTPErrorDirectionBelow)) JKQTPExtendString(s, "+", "below");

    if (!pos.testFlag(JKQTPErrorSimpleBars)&&pos.testFlag(JKQTPErrorIndicatorBar)) JKQTPExtendString(s, "+", "bars");
    if (!pos.testFlag(JKQTPErrorSimpleBars)&&pos.testFlag(JKQTPErrorIndicatorArrows)) JKQTPExtendString(s, "+", "arrows");
    if (!pos.testFlag(JKQTPErrorSimpleBars)&&pos.testFlag(JKQTPErrorIndicatorInwardArrows)) JKQTPExtendString(s, "+", "inwardarrows");

    return s;
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
    if (s=="error_bars_half_above") return JKQTPErrorHalfBarsAbove;
    if (s=="error_bars_half_below") return JKQTPErrorHalfBarsBelow;
    if (s=="error_bars_half_inwards") return JKQTPErrorHalfBarsInwards;
    if (s=="error_bars_half_outwards") return JKQTPErrorHalfBarsOutwards;
    const QStringList sl=s.split("+");
    JKQTPErrorPlotstyle es=JKQTPNoError;
    for (const QString& s: sl) {
        if (s=="error_arrows") es|=JKQTPErrorArrows;
        if (s=="error_inwardarrows") es|=JKQTPErrorInwardArrows;
        if (s=="error_bars") es|=JKQTPErrorBars;
        if (s=="error_simplebars") es|=JKQTPErrorSimpleBars;
        if (s=="error_lines") es|=JKQTPErrorLines;
        if (s=="error_polygons") es|=JKQTPErrorPolygons;
        if (s=="error_ell" || s=="error_ellipses") es|=JKQTPErrorEllipses;
        if (s=="error_box" || s=="error_boxes") es|=JKQTPErrorBoxes;
        if (s=="outwards") es|=JKQTPErrorDirectionOutwards;
        if (s=="inwards") es|=JKQTPErrorDirectionInwards;
        if (s=="above") es|=JKQTPErrorDirectionAbove;
        if (s=="below") es|=JKQTPErrorDirectionBelow;
        if (s=="bars"||s=="bar") es|=JKQTPErrorIndicatorBar;
        if (s=="arrow" || s=="arrows") es|=JKQTPErrorIndicatorArrows;
        if (  s=="inwardarrows" || s=="inwardsarrows" || s=="inwardarrow" || s=="inwardsarrow"
            ||s=="inward_arrows" || s=="inwards_arrows" || s=="inward_arrow" || s=="inwards_arrow") es|=JKQTPErrorIndicatorInwardArrows;
    }
    return es;
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
    if (s=="jkqtpmwazoombywheelandtrackpadpan"||s=="zoombywheelortrackpan"||s=="zoomortrackpan") return jkqtpmwaZoomByWheelAndTrackpadPan;
    return jkqtpmwaZoomByWheel;

}

QString JKQTPMouseWheelActions2String(JKQTPMouseWheelActions act)
{
    if (act==jkqtpmwaZoomByWheel) return "zoom";
    if (act==jkqtpmwaPanByWheel) return "pan";
    if (act==jkqtpmwaZoomByWheelAndTrackpadPan) return "zoomortrackpan";
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
    if (s=="jkqtpmdadrawcircleforevent"||s=="drawcircle"||s=="circle") return jkqtpmdaDrawCircleForEvent;
    if (s=="jkqtpmdadrawellipseforevent"||s=="drawellipse"||s=="ellipse") return jkqtpmdaDrawEllipseForEvent;
    if (s=="jkqtpmdadrawlineforevent"||s=="drawline"||s=="line") return jkqtpmdaDrawLineForEvent;
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



JKQTPColorDerivationMode::JKQTPColorDerivationMode(PredefinedModes mode):
    colorModification(ColorChangeMode::SameColor),
    targetColor("black"),
    colorModificationStrength(200),
    transparencyModification(TransparencyChangeMode::SameTransparency),
    targetTransparency(0.5f),
    transparencyModficationStrength(0.66f)
{
    switch(mode) {
    case JKQTPFFCMFullyTransparentColor:
        targetColor=QColor(Qt::transparent);
        colorModification=ColorChangeMode::ReplaceColorAndTransparency;
        break;
    case JKQTPFFCMBlack:
        targetColor=QColor(Qt::black);
        colorModification=ColorChangeMode::ReplaceColorAndTransparency;
        break;
    case JKQTPFFCMWhite:
        targetColor=QColor(Qt::white);
        colorModification=ColorChangeMode::ReplaceColorAndTransparency;
        break;
    case JKQTPFFCMGrey25:
        targetColor=QColor(64,64,64);
        colorModification=ColorChangeMode::ReplaceColorAndTransparency;
        break;
    case JKQTPFFCMGrey50:
        targetColor=QColor(127,127,127);
        colorModification=ColorChangeMode::ReplaceColorAndTransparency;
        break;
    case JKQTPFFCMGrey75:
        targetColor=QColor(191,191,191);
        colorModification=ColorChangeMode::ReplaceColorAndTransparency;
        break;
    case JKQTPFFCMBlackTransparent:
        targetColor=QColor(0,0,0,175);
        colorModification=ColorChangeMode::ReplaceColorAndTransparency;
        break;
    case JKQTPFFCMWhiteTransparent:
        targetColor=QColor(255,255,255,175);
        colorModification=ColorChangeMode::ReplaceColorAndTransparency;
        break;
    case JKQTPFFCMGrey25Transparent:
        targetColor=QColor(64,64,64,175);
        colorModification=ColorChangeMode::ReplaceColorAndTransparency;
        break;
    case JKQTPFFCMGrey50Transparent:
        targetColor=QColor(127,127,127,175);
        colorModification=ColorChangeMode::ReplaceColorAndTransparency;
        break;
    case JKQTPFFCMGrey75Transparent:
        targetColor=QColor(191,191,191,175);
        colorModification=ColorChangeMode::ReplaceColorAndTransparency;
        break;
    case JKQTPFFCMSameColor:
        break;
    case JKQTPFFCMSameNonTransparentColor:
        transparencyModification=TransparencyChangeMode::ReplaceTransparency;
        targetTransparency=0.0;
        break;
    case JKQTPFFCMSameMoreTransparentColor:
        transparencyModification=TransparencyChangeMode::MoreTransparent;
        transparencyModficationStrength=0.33f;
        break;
    case JKQTPFFCMSameEvenMoreTransparentColor:
        transparencyModification=TransparencyChangeMode::MoreTransparent;
        transparencyModficationStrength=0.66f;
        break;
    case JKQTPFFCMSameLessTransparentColor:
        transparencyModification=TransparencyChangeMode::LessTransparent;
        transparencyModficationStrength=0.33f;
        break;
    case JKQTPFFCMSameEvenLessTransparentColor:
        transparencyModification=TransparencyChangeMode::LessTransparent;
        transparencyModficationStrength=0.66f;
        break;
    case JKQTPFFCMInvertedColor:
        colorModification=ColorChangeMode::InvertColor;
        break;
    case JKQTPFFCMInvertedTransparentColor:
        colorModification=ColorChangeMode::InvertColor;
        transparencyModification=TransparencyChangeMode::MoreTransparent;
        transparencyModficationStrength=0.33f;
        break;
    case JKQTPFFCMInvertedNonTransparentColor:
        colorModification=ColorChangeMode::InvertColor;
        transparencyModification=TransparencyChangeMode::ReplaceTransparency;
        targetTransparency=0.0;
        break;

    case JKQTPFFCMLighterColor:
        colorModification=ColorChangeMode::LighterColor;
        colorModificationStrength=150;
        break;
    case JKQTPFFCMLighterAndTransparentColor:
        colorModification=ColorChangeMode::LighterColor;
        colorModificationStrength=150;
        transparencyModification=TransparencyChangeMode::MoreTransparent;
        transparencyModficationStrength=0.33f;
        break;
    case JKQTPFFCMLighterAndNonTransparentColor:
        colorModification=ColorChangeMode::LighterColor;
        colorModificationStrength=150;
        transparencyModification=TransparencyChangeMode::ReplaceTransparency;
        targetTransparency=0.0;
        break;

    case JKQTPFFCMEvenLighterColor:
        colorModification=ColorChangeMode::LighterColor;
        colorModificationStrength=200;
        break;
    case JKQTPFFCMEvenLighterAndTransparentColor:
        colorModification=ColorChangeMode::LighterColor;
        colorModificationStrength=200;
        transparencyModification=TransparencyChangeMode::MoreTransparent;
        transparencyModficationStrength=0.33f;
        break;
    case JKQTPFFCMEvenLighterAndNonTransparentColor:
        colorModification=ColorChangeMode::LighterColor;
        colorModificationStrength=200;
        transparencyModification=TransparencyChangeMode::ReplaceTransparency;
        targetTransparency=0.0f;
        break;


    case JKQTPFFCMDarkerColor:
        colorModification=ColorChangeMode::DarkerColor;
        colorModificationStrength=200;
        break;
    case JKQTPFFCMDarkerAndTransparentColor:
        colorModification=ColorChangeMode::DarkerColor;
        colorModificationStrength=200;
        transparencyModification=TransparencyChangeMode::MoreTransparent;
        transparencyModficationStrength=0.33f;
        break;
    case JKQTPFFCMDarkerAndNonTransparentColor:
        colorModification=ColorChangeMode::DarkerColor;
        colorModificationStrength=200;
        transparencyModification=TransparencyChangeMode::ReplaceTransparency;
        targetTransparency=0.0f;
        break;

    case JKQTPFFCMEvenDarkerColor:
        colorModification=ColorChangeMode::DarkerColor;
        colorModificationStrength=300;
        break;
    case JKQTPFFCMEvenDarkerAndTransparentColor:
        colorModification=ColorChangeMode::DarkerColor;
        colorModificationStrength=300;
        transparencyModification=TransparencyChangeMode::MoreTransparent;
        transparencyModficationStrength=0.33f;
        break;
    case JKQTPFFCMEvenDarkerAndNonTransparentColor:
        colorModification=ColorChangeMode::DarkerColor;
        colorModificationStrength=300;
        transparencyModification=TransparencyChangeMode::ReplaceTransparency;
        targetTransparency=0.0f;
        break;
    }
}

JKQTPColorDerivationMode::JKQTPColorDerivationMode(const QString &mode):
    JKQTPColorDerivationMode()
{
    *this=fromString(mode);
}

QString JKQTPColorDerivationMode::toString() const
{
    // some shortcut strings
    if (*this == JKQTPFFCMFullyTransparentColor) return "transparent";
    if (*this == JKQTPFFCMSameColor) return "same";
    if (*this == JKQTPFFCMInvertedColor) return "inverted";
    if (*this == JKQTPFFCMWhite) return "white";
    if (*this == JKQTPFFCMSameNonTransparentColor) return "same_non_transparent";
    if (*this == JKQTPFFCMLighterColor) return "lighter";
    if (*this == JKQTPFFCMDarkerColor) return "darker";
    if (*this == JKQTPFFCMLighterAndTransparentColor) return "lighter_transparent";
    if (*this == JKQTPFFCMDarkerAndTransparentColor) return "darker_transparent";

    QString name;
    switch (colorModification) {
    case ColorChangeMode::SameColor:
        name="same_color";
        break;
    case ColorChangeMode::ReplaceColorNotTransparency:
        name="replace_color_not_transparency("+jkqtp_QColor2String(targetColor)+")";
        break;
    case ColorChangeMode::ReplaceColorAndTransparency:
        name="replace_color_and_transparency("+jkqtp_QColor2String(targetColor)+")";
        break;
    case ColorChangeMode::DarkerColor:
        name="darker_color("+QString::number(colorModificationStrength)+")";
        break;
    case ColorChangeMode::LighterColor:
        name="lighter_color("+QString::number(colorModificationStrength)+")";
        break;
    case ColorChangeMode::InvertColor:
        name="inverted_color";
        break;
    default:
        qDebug()<<"unknown colorModification="<<colorModification<<" in JKQTPColorDerivationMode::toString()";
        name="same_color";
        break;
    }

    // alpha=0: transparent, alpha=1: ppaque
    switch (transparencyModification) {
    case TransparencyChangeMode::SameTransparency:
        break;
    case TransparencyChangeMode::ReplaceTransparency:
        name+=",transparency("+QString::number(targetTransparency*100,'f',1)+")";
        break;
    case TransparencyChangeMode::MoreTransparent:
        name+=",more_transparent("+QString::number(transparencyModficationStrength,'f',3)+")";
        break;
    case TransparencyChangeMode::LessTransparent:
        name+=",less_transparent("+QString::number(transparencyModficationStrength,'f',3)+")";
        break;
    default:
        qDebug()<<"unknown transparencyModification="<<transparencyModification<<" in JKQTPColorDerivationMode::toString()";
        break;
    }
    return name;
}

JKQTPColorDerivationMode JKQTPColorDerivationMode::fromString(const QString &mode)
{
    const QString m=mode.trimmed().toLower();
    //qDebug()<<"JKQTPColorDerivationMode::fromString("<<m<<")";

    // legacy strings
    if (m=="transparent" || m=="no_color" || m=="none") return JKQTPColorDerivationMode(JKQTPFFCMFullyTransparentColor);
    if (m=="same"||m=="same_color"||m.isEmpty()) return JKQTPColorDerivationMode(JKQTPFFCMSameColor);
    if (m=="black") return JKQTPColorDerivationMode(JKQTPFFCMBlack);
    if (m=="white") return JKQTPColorDerivationMode(JKQTPFFCMWhite);
    if (m=="grey25") return JKQTPColorDerivationMode(JKQTPFFCMGrey25);
    if (m=="grey50") return JKQTPColorDerivationMode(JKQTPFFCMGrey50);
    if (m=="grey75") return JKQTPColorDerivationMode(JKQTPFFCMGrey75);
    if (m=="transparent_black" || m=="black_transparent") return JKQTPColorDerivationMode(JKQTPFFCMBlackTransparent);
    if (m=="transparent_white" || m=="white_transparent") return JKQTPColorDerivationMode(JKQTPFFCMWhiteTransparent);
    if (m=="transparent_grey25" || m=="grey25_transparent") return JKQTPColorDerivationMode(JKQTPFFCMGrey25Transparent);
    if (m=="transparent_grey50" || m=="grey50_transparent") return JKQTPColorDerivationMode(JKQTPFFCMGrey50Transparent);
    if (m=="transparent_grey75" || m=="grey75_transparent") return JKQTPColorDerivationMode(JKQTPFFCMGrey75Transparent);
    if (m=="inverted") return JKQTPColorDerivationMode(JKQTPFFCMInvertedColor);
    if (m=="transparent_inverted" || m=="inverted_transparent") return JKQTPColorDerivationMode(JKQTPFFCMInvertedTransparentColor);
    if (m=="non_transparent_inverted" || m=="inverted_non_transparent") return JKQTPColorDerivationMode(JKQTPFFCMInvertedNonTransparentColor);
    if (m=="lighter") return JKQTPColorDerivationMode(JKQTPFFCMLighterColor);
    if (m=="even_lighter") return JKQTPColorDerivationMode(JKQTPFFCMEvenLighterColor);
    if (m=="darker") return JKQTPColorDerivationMode(JKQTPFFCMDarkerColor);
    if (m=="even_darker") return JKQTPColorDerivationMode(JKQTPFFCMEvenDarkerColor);
    if (m=="lighter_transparent"||m=="lighter_and_transparent") return JKQTPColorDerivationMode(JKQTPFFCMLighterAndTransparentColor);
    if (m=="even_lighter_transparent"||m=="even_lighter_and_transparent") return JKQTPColorDerivationMode(JKQTPFFCMEvenLighterAndTransparentColor);
    if (m=="darker_transparent"||m=="darker_and_transparent") return JKQTPColorDerivationMode(JKQTPFFCMDarkerAndTransparentColor);
    if (m=="even_darker_transparent"||m=="even_darker_and_transparent") return JKQTPColorDerivationMode(JKQTPFFCMEvenDarkerAndTransparentColor);
    if (m=="lighter_non_transparent"||m=="lighter_and_non_transparent") return JKQTPColorDerivationMode(JKQTPFFCMLighterAndNonTransparentColor);
    if (m=="even_lighter_non_transparent"||m=="even_lighter_and_non_transparent") return JKQTPColorDerivationMode(JKQTPFFCMEvenLighterAndNonTransparentColor);
    if (m=="darker_non_transparent"||m=="darker_and_non_transparent") return JKQTPColorDerivationMode(JKQTPFFCMDarkerAndNonTransparentColor);
    if (m=="even_darker_non_transparent"||m=="even_darker_and_non_transparent") return JKQTPColorDerivationMode(JKQTPFFCMEvenDarkerAndNonTransparentColor);
    if (m=="same_more_transparent"||m=="more_transparent") return JKQTPColorDerivationMode(JKQTPFFCMSameMoreTransparentColor);
    if (m=="same_even_more_transparent"||m=="even_more_transparent") return JKQTPColorDerivationMode(JKQTPFFCMSameEvenMoreTransparentColor);
    if (m=="same_less_transparent"||m=="less_transparent") return JKQTPColorDerivationMode(JKQTPFFCMSameLessTransparentColor);
    if (m=="same_even_less_transparent"||m=="even_less_transparent") return JKQTPColorDerivationMode(JKQTPFFCMSameEvenLessTransparentColor);
    if (m=="same_non_transparent" || m=="non_transparent") return JKQTPColorDerivationMode(JKQTPFFCMSameNonTransparentColor);

    // cleanly analyze string
    JKQTPColorDerivationMode res;
    const QStringList parts=mode.split(',');
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
            static QRegularExpression rx1arg("(transparency|more_transparent|less_transparent|replace_color_not_transparency|replace_color_and_transparency|darker_color|lighter_color)\\((.*)\\)", QRegularExpression::CaseInsensitiveOption|QRegularExpression::InvertedGreedinessOption);
#else
            const QRegExp rx1arg=[](){ QRegExp rx4("(transparency|more_transparent|less_transparent|replace_color_not_transparency|replace_color_and_transparency|darker_color|lighter_color)\\((.*)\\)", Qt::CaseInsensitive);
                                 rx4.setMinimal(false);
                                 return rx4;
            }();
#endif
    for (QString pin: parts) {
        QString p;
        p.reserve(pin.size());
        for (auto c: pin) {
            if (!c.isSpace()&&c!='\t') p.append(c);
        }
        p=p.trimmed();
        if (p=="same_color") res.colorModification=ColorChangeMode::SameColor;
        else if (p=="invert"||p=="inverted"||p=="inverted_color") res.colorModification=ColorChangeMode::InvertColor;
        else if (p=="invert"||p=="inverted"||p=="inverted_color") res.colorModification=ColorChangeMode::InvertColor;
        else if (p=="more_transparent") { res.transparencyModification=TransparencyChangeMode::MoreTransparent; res.transparencyModficationStrength=0.3; }
        else if (p=="non_transparent" || p=="opaque") { res.transparencyModification=TransparencyChangeMode::ReplaceTransparency; res.targetTransparency=0; }
        else if (p=="transparent") { res.transparencyModification=TransparencyChangeMode::ReplaceTransparency; res.targetTransparency=1; }
        else if (p=="less_transpatransparentrent") { res.transparencyModification=TransparencyChangeMode::LessTransparent; res.transparencyModficationStrength=0.3; }
        else if (p=="same_transparency") { res.transparencyModification=TransparencyChangeMode::SameTransparency; }
        else {
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
            const auto m1arg=rx1arg.match(p);
            if (m1arg.hasMatch()) {
                const QString instr=m1arg.captured(1).trimmed();
                const QString arg1=m1arg.captured(2).trimmed();
#else
            if (rx1arg.indexIn(p)>=0) {
                const QString instr=rx1arg.cap(1).trimmed();
                const QString arg1=rx1arg.cap(2).trimmed();
#endif
                bool arg1f_ok=false;
                const float arg1f=arg1.toFloat(&arg1f_ok);
                if (instr=="transparency" && arg1f_ok) {
                    res.transparencyModification=TransparencyChangeMode::ReplaceTransparency;
                    res.targetTransparency=arg1f;
                } else if (instr=="more_transparent" && arg1f_ok) {
                    res.transparencyModification=TransparencyChangeMode::MoreTransparent;
                    res.transparencyModficationStrength=arg1f;
                } else if (instr=="less_transparent" && arg1f_ok) {
                    res.transparencyModification=TransparencyChangeMode::LessTransparent;
                    res.transparencyModficationStrength=arg1f;
                } else if (instr=="darker_color" && arg1f_ok) {
                    res.colorModification=ColorChangeMode::DarkerColor;
                    res.colorModificationStrength=arg1f;
                } else if (instr=="lighter_color" && arg1f_ok) {
                    res.colorModification=ColorChangeMode::LighterColor;
                    res.colorModificationStrength=arg1f;
                } else if (instr=="replace_color_not_transparency") {
                    const QColor arg1c=jkqtp_String2QColor(arg1);
                    res.colorModification=ColorChangeMode::ReplaceColorNotTransparency;
                    res.targetColor=arg1c;
                } else if (instr=="replace_color_and_transparency") {
                    const QColor arg1c=jkqtp_String2QColor(arg1);
                    res.colorModification=ColorChangeMode::ReplaceColorAndTransparency;
                    res.targetColor=arg1c;
                }
            }
        }
    }

    return res;
}

QColor JKQTPColorDerivationMode::apply(const QColor& basecolor) const
{
    QColor c=basecolor;
    switch (colorModification) {
    case ColorChangeMode::SameColor:
        c=basecolor;
        break;
    case ColorChangeMode::ReplaceColorNotTransparency:
        c=targetColor;
        c.setAlphaF(basecolor.alphaF());
        break;
    case ColorChangeMode::ReplaceColorAndTransparency:
        c=targetColor;
        break;
    case ColorChangeMode::DarkerColor:
        c=c.darker(static_cast<int>(colorModificationStrength));
        break;
    case ColorChangeMode::LighterColor:
        c=c.lighter(static_cast<int>(colorModificationStrength));
        break;
    case ColorChangeMode::InvertColor:
        c=QColor(255-c.red(), 255-c.green(), 255-c.blue(), c.alpha());
        break;
    }

    // alpha=0: transparent, alpha=1: ppaque
    switch (transparencyModification) {
    case TransparencyChangeMode::SameTransparency:
        c.setAlphaF(c.alphaF());
        break;
    case TransparencyChangeMode::ReplaceTransparency:
        c.setAlphaF(1.0-targetTransparency);
        break;
    case TransparencyChangeMode::MoreTransparent:
        c.setAlphaF(c.alphaF()-transparencyModficationStrength*c.alphaF());
        break;
    case TransparencyChangeMode::LessTransparent:
        c.setAlphaF(c.alphaF()+(1.0-c.alphaF())*transparencyModficationStrength);
        break;
    }
    return c;
}

bool JKQTPColorDerivationMode::operator==(const JKQTPColorDerivationMode &other) const
{
    if (colorModification!=other.colorModification) return false;
    else {
        if (colorModification==ColorChangeMode::ReplaceColorNotTransparency || colorModification==ColorChangeMode::ReplaceColorAndTransparency) {
            if (targetColor!=other.targetColor) return false;
        } else if (colorModification==ColorChangeMode::DarkerColor || colorModification==ColorChangeMode::LighterColor) {
            if (colorModificationStrength!=other.colorModificationStrength) return false;
        }
    }
    if (transparencyModification!=other.transparencyModification) return false;
    else {
        if (transparencyModification==TransparencyChangeMode::ReplaceTransparency) {
            if (targetTransparency!=other.targetTransparency) return false;
        } else if (transparencyModification==TransparencyChangeMode::MoreTransparent || transparencyModification==TransparencyChangeMode::LessTransparent) {
            if (transparencyModficationStrength!=other.transparencyModficationStrength) return false;
        }
    }
    return true;
}

bool JKQTPColorDerivationMode::operator==(const JKQTPColorDerivationMode::PredefinedModes &other) const
{
    return operator==(JKQTPColorDerivationMode(other));
}


QString JKQTPColorDerivationMode2String(JKQTPColorDerivationMode mode)
{
    return mode.toString();
}

JKQTPColorDerivationMode String2JKQTPColorDerivationMode(const QString &mode)
{
    return JKQTPColorDerivationMode::fromString(mode);
}

QColor JKQTPGetDerivedColor(JKQTPColorDerivationMode mode, const QColor &basecolor)
{
    return mode.apply(basecolor);
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

QString JKQTPMouseMoveActions2String(JKQTPMouseMoveActions act)
{
    if (act==jkqtpmmaToolTipForClosestDataPoint) return "ToolTipForClosestDataPoint";
    return "ToolTipForClosestDataPoint";
}

JKQTPMouseMoveActions String2JKQTPMouseMoveActions(const QString &s)
{
    if (s=="jkqtpmmatooltipforclosestdatapoint"||s=="closestdatapointtooltip"||s=="tooltipforclosestdatapoint"||s=="tooltip") return jkqtpmmaToolTipForClosestDataPoint;
    return jkqtpmmaToolTipForClosestDataPoint;
}

QList<JKQTPKeyPosition> JKQTPGetTypicalKeyPositions()
{
    static QList<JKQTPKeyPosition> pos = {
        JKQTPKeyInsideTopRight,
        JKQTPKeyInsideTopLeft,
        JKQTPKeyInsideBottomLeft,
        JKQTPKeyInsideBottomRight,
        JKQTPKeyInsideTop,
        JKQTPKeyInsideBottom,
        JKQTPKeyInsideLeft,
        JKQTPKeyInsideRight,
        JKQTPKeyOutsideTopLeft,
        JKQTPKeyOutsideTopCenter,
        JKQTPKeyOutsideTopRight,
        JKQTPKeyOutsideLeftTop,
        JKQTPKeyOutsideLeftCenter,
        JKQTPKeyOutsideLeftBottom,
        JKQTPKeyOutsideRightTop,
        JKQTPKeyOutsideRightCenter,
        JKQTPKeyOutsideRightBottom,
        JKQTPKeyOutsideBottomLeft,
        JKQTPKeyOutsideBottomCenter,
        JKQTPKeyOutsideBottomRight
     };
    return pos;
}
