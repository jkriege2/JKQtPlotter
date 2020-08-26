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
