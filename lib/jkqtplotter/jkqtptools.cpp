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
#include "jkqtcommon/jkqtpenhancedpainter.h"
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
    switch(pos) {
        case JKQTPCADMcomplete: return "all";
        case JKQTPCADMTicksTickLabelsAxisLabel: return "ticks+labels+axislabel";
        case JKQTPCADMTicksTickLabels: return "ticks+labels";
        case JKQTPCADMTicks: return "ticks";
        case JKQTPCADMLineTicksTickLabels: return "line+ticks+labels";
        case JKQTPCADMTickLabelsAxisLabel: return "labels+axislabel";
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
        case JKQTPErrorHalfBarsAbove: return "error_bars_half_above";
        case JKQTPErrorHalfBarsBelow: return "error_bars_half_below";
        case JKQTPErrorHalfBarsInwards: return "error_bars_half_inwards";
        case JKQTPErrorHalfBarsOutwards: return "error_bars_half_outwards";
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
    if (s=="error_bars_half_above") return JKQTPErrorHalfBarsAbove;
    if (s=="error_bars_half_below") return JKQTPErrorHalfBarsBelow;
    if (s=="error_bars_half_inwards") return JKQTPErrorHalfBarsInwards;
    if (s=="error_bars_half_outwards") return JKQTPErrorHalfBarsOutwards;
    return JKQTPNoError;
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
    }
    return name;
}

JKQTPColorDerivationMode JKQTPColorDerivationMode::fromString(const QString &mode)
{
    const QString m=mode.trimmed().toLower();

    // legacy strings
    if (m=="transparent" || m=="no_color" || m=="none") return JKQTPColorDerivationMode(JKQTPFFCMFullyTransparentColor);
    if (m=="same"||m=="same_color") return JKQTPColorDerivationMode(JKQTPFFCMSameColor);
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


    return JKQTPColorDerivationMode();
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
