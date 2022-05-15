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
        case JKQTPCADMTickLabelsAxisLabel: return "ticks+axislabel";
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
        case JKQTPFFCMFullyTransparentColor: return "transparent";
        case JKQTPFFCMBlack: return "black";
        case JKQTPFFCMWhite: return "white";
        case JKQTPFFCMGrey25: return "grey25";
        case JKQTPFFCMGrey50: return "grey50";
        case JKQTPFFCMGrey75: return "grey75";
        case JKQTPFFCMBlackTransparent: return "black_transparent";
        case JKQTPFFCMWhiteTransparent: return "white_transparent";
        case JKQTPFFCMGrey25Transparent: return "grey25_transparent";
        case JKQTPFFCMGrey50Transparent: return "grey50_transparent";
        case JKQTPFFCMGrey75Transparent: return "grey75_transparent";
        case JKQTPFFCMSameColor: return "same";
        case JKQTPFFCMInvertedColor: return "inverted";
        case JKQTPFFCMInvertedTransparentColor: return "inverted_transparent";
        case JKQTPFFCMInvertedNonTransparentColor: return "inverted_non_transparent";
        case JKQTPFFCMLighterColor: return "lighter";
        case JKQTPFFCMEvenLighterColor: return "even_lighter";
        case JKQTPFFCMDarkerColor: return "darker";
        case JKQTPFFCMEvenDarkerColor: return "even_darker";
        case JKQTPFFCMLighterAndTransparentColor: return "lighter_transparent";
        case JKQTPFFCMEvenLighterAndTransparentColor: return "even_lighter_transparent";
        case JKQTPFFCMDarkerAndTransparentColor: return "darker_transparent";
        case JKQTPFFCMEvenDarkerAndTransparentColor: return "even_darker_transparent";
        case JKQTPFFCMLighterAndNonTransparentColor: return "lighter_non_transparent";
        case JKQTPFFCMEvenLighterAndNonTransparentColor: return "even_lighter_non_transparent";
        case JKQTPFFCMDarkerAndNonTransparentColor: return "darker_non_transparent";
        case JKQTPFFCMEvenDarkerAndNonTransparentColor: return "even_darker_non_transparent";
        case JKQTPFFCMSameMoreTransparentColor: return "same_more_transparent";
        case JKQTPFFCMSameEvenMoreTransparentColor: return "same_even_more_transparent";
        case JKQTPFFCMSameLessTransparentColor: return "same_less_transparent";
        case JKQTPFFCMSameEvenLessTransparentColor: return "same_even_less_transparent";
        case JKQTPFFCMSameNonTransparentColor: return "same_non_transparent";
    }
    return "same";
}

JKQTPColorDerivationMode String2JKQTPColorDerivationMode(const QString &mode)
{
    const QString m=mode.trimmed().toLower();
    if (m=="transparent" || m=="no_color" || m=="none") return JKQTPFFCMFullyTransparentColor;
    if (m=="same"||m=="same_color") return JKQTPFFCMSameColor;
    if (m=="black") return JKQTPFFCMBlack;
    if (m=="white") return JKQTPFFCMWhite;
    if (m=="grey25") return JKQTPFFCMGrey25;
    if (m=="grey50") return JKQTPFFCMGrey50;
    if (m=="grey75") return JKQTPFFCMGrey75;
    if (m=="transparent_black" || m=="black_transparent") return JKQTPFFCMBlackTransparent;
    if (m=="transparent_white" || m=="white_transparent") return JKQTPFFCMWhiteTransparent;
    if (m=="transparent_grey25" || m=="grey25_transparent") return JKQTPFFCMGrey25Transparent;
    if (m=="transparent_grey50" || m=="grey50_transparent") return JKQTPFFCMGrey50Transparent;
    if (m=="transparent_grey75" || m=="grey75_transparent") return JKQTPFFCMGrey75Transparent;
    if (m=="inverted") return JKQTPFFCMInvertedColor;
    if (m=="transparent_inverted" || m=="inverted_transparent") return JKQTPFFCMInvertedTransparentColor;
    if (m=="non_transparent_inverted" || m=="inverted_non_transparent") return JKQTPFFCMInvertedNonTransparentColor;
    if (m=="lighter") return JKQTPFFCMLighterColor;
    if (m=="even_lighter") return JKQTPFFCMEvenLighterColor;
    if (m=="darker") return JKQTPFFCMDarkerColor;
    if (m=="even_darker") return JKQTPFFCMEvenDarkerColor;
    if (m=="lighter_transparent"||m=="lighter_and_transparent") return JKQTPFFCMLighterAndTransparentColor;
    if (m=="even_lighter_transparent"||m=="even_lighter_and_transparent") return JKQTPFFCMEvenLighterAndTransparentColor;
    if (m=="darker_transparent"||m=="darker_and_transparent") return JKQTPFFCMDarkerAndTransparentColor;
    if (m=="even_darker_transparent"||m=="even_darker_and_transparent") return JKQTPFFCMEvenDarkerAndTransparentColor;
    if (m=="lighter_non_transparent"||m=="lighter_and_non_transparent") return JKQTPFFCMLighterAndNonTransparentColor;
    if (m=="even_lighter_non_transparent"||m=="even_lighter_and_non_transparent") return JKQTPFFCMEvenLighterAndNonTransparentColor;
    if (m=="darker_non_transparent"||m=="darker_and_non_transparent") return JKQTPFFCMDarkerAndNonTransparentColor;
    if (m=="even_darker_non_transparent"||m=="even_darker_and_non_transparent") return JKQTPFFCMEvenDarkerAndNonTransparentColor;
    if (m=="same_more_transparent"||m=="more_transparent") return JKQTPFFCMSameMoreTransparentColor;
    if (m=="same_even_more_transparent"||m=="even_more_transparent") return JKQTPFFCMSameEvenMoreTransparentColor;
    if (m=="same_less_transparent"||m=="less_transparent") return JKQTPFFCMSameLessTransparentColor;
    if (m=="same_even_less_transparent"||m=="even_less_transparent") return JKQTPFFCMSameEvenLessTransparentColor;
    if (m=="same_non_transparent" || m=="non_transparent") return JKQTPFFCMSameNonTransparentColor;
    return JKQTPFFCMSameColor;
}

QColor JKQTPGetDerivedColor(JKQTPColorDerivationMode mode, const QColor &basecolor)
{
    switch(mode) {
        case JKQTPFFCMFullyTransparentColor: return QColor(Qt::transparent);
        case JKQTPFFCMBlack: return QColor(Qt::black);
        case JKQTPFFCMWhite: return QColor(Qt::white);
        case JKQTPFFCMGrey25: return QColor(64,64,64);
        case JKQTPFFCMGrey50: return QColor(127,127,127);
        case JKQTPFFCMGrey75: return QColor(191,191,191);
        case JKQTPFFCMBlackTransparent: return QColor(0,0,0,175);
        case JKQTPFFCMWhiteTransparent: return QColor(255,255,255,175);
        case JKQTPFFCMGrey25Transparent: return QColor(64,64,64,175);
        case JKQTPFFCMGrey50Transparent: return QColor(127,127,127,175);
        case JKQTPFFCMGrey75Transparent: return QColor(191,191,191,175);
        case JKQTPFFCMSameColor:
        case JKQTPFFCMSameNonTransparentColor: {
            QColor c=basecolor;
            if (mode==JKQTPFFCMSameNonTransparentColor) c.setAlphaF(1.0);
            return c;
        }
        case JKQTPFFCMSameMoreTransparentColor: { QColor c=basecolor; c.setAlphaF(0.66*c.alphaF()); return c; }
        case JKQTPFFCMSameEvenMoreTransparentColor: { QColor c=basecolor; c.setAlphaF(0.33*c.alphaF()); return c; }
        case JKQTPFFCMSameLessTransparentColor: { QColor c=basecolor; c.setAlphaF(c.alphaF()+(1.0-c.alphaF())*0.33); return c; }
        case JKQTPFFCMSameEvenLessTransparentColor: { QColor c=basecolor; c.setAlphaF(c.alphaF()+(1.0-c.alphaF())*0.66); return c; }
        case JKQTPFFCMInvertedColor:
        case JKQTPFFCMInvertedTransparentColor:
        case JKQTPFFCMInvertedNonTransparentColor: {
            QColor c(255-basecolor.red(), 255-basecolor.green(), 255-basecolor.blue(), basecolor.alpha());
            if (mode==JKQTPFFCMInvertedTransparentColor) c.setAlphaF(0.66*c.alphaF());
            if (mode==JKQTPFFCMInvertedNonTransparentColor) c.setAlphaF(1.0);
            return c;
        }
        case JKQTPFFCMLighterColor:
        case JKQTPFFCMLighterAndTransparentColor:
        case JKQTPFFCMLighterAndNonTransparentColor: {
            QColor c=basecolor.lighter();
            if (basecolor==QColor("black")) c=QColor(64,64,64);
            if (mode==JKQTPFFCMLighterAndTransparentColor) c.setAlphaF(0.66*c.alphaF());
            if (mode==JKQTPFFCMLighterAndNonTransparentColor) c.setAlphaF(1.0);
            return c;
        }
        case JKQTPFFCMEvenLighterColor:
        case JKQTPFFCMEvenLighterAndTransparentColor:
        case JKQTPFFCMEvenLighterAndNonTransparentColor: {
            QColor c=basecolor.lighter().lighter();
            if (basecolor==QColor("black")) c=QColor(127,127,127);
            if (mode==JKQTPFFCMEvenLighterAndTransparentColor) c.setAlphaF(0.66*c.alphaF());
            if (mode==JKQTPFFCMEvenLighterAndNonTransparentColor) c.setAlphaF(1.0);
            return c;
        }
        case JKQTPFFCMDarkerColor:
        case JKQTPFFCMDarkerAndTransparentColor:
        case JKQTPFFCMDarkerAndNonTransparentColor: {
            QColor c=basecolor.darker();
            if (basecolor==QColor("white")) c= QColor(191,191,191);
            if (mode==JKQTPFFCMDarkerAndTransparentColor) c.setAlphaF(0.66*c.alphaF());
            if (mode==JKQTPFFCMDarkerAndNonTransparentColor) c.setAlphaF(1.0);
            return c;
        }
        case JKQTPFFCMEvenDarkerColor:
        case JKQTPFFCMEvenDarkerAndTransparentColor:
        case JKQTPFFCMEvenDarkerAndNonTransparentColor: {
            QColor c=basecolor.darker().darker();
            if (basecolor==QColor("white")) c= QColor(127,127,127);
            if (mode==JKQTPFFCMEvenDarkerAndTransparentColor) c.setAlphaF(0.66*c.alphaF());
            if (mode==JKQTPFFCMEvenDarkerAndNonTransparentColor) c.setAlphaF(1.0);
            return c;
        }
    }
    return basecolor;
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
