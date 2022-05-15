#include "jkqtplotterstyle.h"
#include <QApplication>
#include <QDebug>
#include "jkqtcommon/jkqttools.h"
#include "jkqtplotter/jkqtptools.h"

JKQTPlotterStyle::JKQTPlotterStyle():
    maxTooltipDistance(16),
    userActionCatchSensitivity(1),
    toolbarIconSize(16),
    userActionOverlayPen(QColor("darkblue").darker(), 1.5, Qt::DashLine),
    userActionOverlayBrush(QColorWithAlphaF(QColor("steelblue"), 0.2)),
    userActionOpaquePen(QColor("darkblue").darker(), 1.5, Qt::DashLine),
    userActionOpaqueBrush(QColor("aliceblue")),
    userActionMarkerPen(QColor("red").darker(), 1, Qt::DotLine),
    userActionMarkerBrush(QColorWithAlphaF(QColor("red"), 0.2)),
    userActionMarkerType(jkqtpuamtCircleAndCrossHair),
    userActionFontName(QApplication::font().family()+"+XITS"),
    userActionFontSize(QApplication::font().pointSizeF()*1.1),
    userActionMarkerDiameter(12),
    mousePositionTemplate("(%1; %2)"),
    displayMousePosition(true),
    toolbarEnabled(true),
    toolbarAlwaysOn(false),
    usePaletteColors(true),
    registeredMouseDragActionModes(),
    registeredMouseWheelActions(),
    registeredMouseMoveActions(),
    registeredMouseDoubleClickActions()
{
    // default user-actions:
    registeredMouseDragActionModes[qMakePair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::NoModifier)]=JKQTPMouseDragActions::jkqtpmdaZoomByRectangle;
    registeredMouseDragActionModes[qMakePair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ControlModifier)]=JKQTPMouseDragActions::jkqtpmdaPanPlotOnMove;
    registeredMouseDoubleClickActions[qMakePair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::NoModifier)]=JKQTPMouseDoubleClickActions::jkqtpdcaClickMovesViewport;
    registeredMouseWheelActions[Qt::NoModifier]=JKQTPMouseWheelActions::jkqtpmwaZoomByWheel;
    //qDebug()<<"JKQTPlotterStyle(): registeredMouseWheelActions="<<registeredMouseWheelActions;
}

void JKQTPlotterStyle::loadSettings(const QSettings &settings, const QString &group, const JKQTPlotterStyle &defaultStyle)
{
    toolbarIconSize=settings.value(group+"toolbar_icon_size", defaultStyle.toolbarIconSize).toInt();
    mousePositionTemplate=settings.value(group+"mouse_position_template", defaultStyle.mousePositionTemplate).toString();


    userActionOverlayPen=defaultStyle.userActionOverlayPen;
    userActionOverlayPen.setColor(jkqtp_String2QColor(settings.value(group+"useraction_overlay_color", jkqtp_QColor2String(defaultStyle.userActionOverlayPen.color())).toString()));
    userActionOverlayPen.setStyle(jkqtp_String2QPenStyle(settings.value(group+"useraction_overlay_penstyle", jkqtp_QPenStyle2String(defaultStyle.userActionOverlayPen.style())).toString()));
    userActionOverlayPen.setWidthF(settings.value(group+"useraction_overlay_linewidth", defaultStyle.userActionOverlayPen.widthF()).toDouble());
    userActionOverlayBrush=defaultStyle.userActionOverlayBrush;
    userActionOverlayBrush.setColor(jkqtp_String2QColor(settings.value(group+"useraction_overlay_brushcolor", jkqtp_QColor2String(defaultStyle.userActionOverlayBrush.color())).toString()));
    userActionOverlayBrush.setStyle(jkqtp_String2QBrushStyle(settings.value(group+"useraction_overlay_brushstyle", jkqtp_QBrushStyle2String(defaultStyle.userActionOverlayBrush.style())).toString()));

    userActionOpaquePen=defaultStyle.userActionOpaquePen;
    userActionOpaquePen.setColor(jkqtp_String2QColor(settings.value(group+"useraction_opaque_color", jkqtp_QColor2String(defaultStyle.userActionOpaquePen.color())).toString()));
    userActionOpaquePen.setStyle(jkqtp_String2QPenStyle(settings.value(group+"useraction_opaque_penstyle", jkqtp_QPenStyle2String(defaultStyle.userActionOpaquePen.style())).toString()));
    userActionOpaquePen.setWidthF(settings.value(group+"useraction_opaque_linewidth", defaultStyle.userActionOpaquePen.widthF()).toDouble());
    userActionOpaqueBrush=defaultStyle.userActionOpaqueBrush;
    userActionOpaqueBrush.setColor(jkqtp_String2QColor(settings.value(group+"useraction_opaque_brushcolor", jkqtp_QColor2String(defaultStyle.userActionOpaqueBrush.color())).toString()));
    userActionOpaqueBrush.setStyle(jkqtp_String2QBrushStyle(settings.value(group+"useraction_opaque_brushstyle", jkqtp_QBrushStyle2String(defaultStyle.userActionOpaqueBrush.style())).toString()));

    userActionMarkerPen=defaultStyle.userActionMarkerPen;
    userActionMarkerPen.setColor(jkqtp_String2QColor(settings.value(group+"useraction_marker_color", jkqtp_QColor2String(defaultStyle.userActionMarkerPen.color())).toString()));
    userActionMarkerPen.setStyle(jkqtp_String2QPenStyle(settings.value(group+"useraction_marker_penstyle", jkqtp_QPenStyle2String(defaultStyle.userActionMarkerPen.style())).toString()));
    userActionMarkerPen.setWidthF(settings.value(group+"useraction_marker_linewidth", defaultStyle.userActionMarkerPen.widthF()).toDouble());
    userActionMarkerBrush=defaultStyle.userActionMarkerBrush;
    userActionMarkerBrush.setColor(jkqtp_String2QColor(settings.value(group+"useraction_marker_brushcolor", jkqtp_QColor2String(defaultStyle.userActionMarkerBrush.color())).toString()));
    userActionMarkerBrush.setStyle(jkqtp_String2QBrushStyle(settings.value(group+"useraction_marker_brushstyle", jkqtp_QBrushStyle2String(defaultStyle.userActionMarkerBrush.style())).toString()));

    userActionMarkerDiameter=settings.value(group+"useraction_marker_diameter", defaultStyle.userActionMarkerDiameter).toInt();
    userActionMarkerType=String2JKQTPUserActionMarkerType(settings.value(group+"useraction_marker_type",JKQTPUserActionMarkerType2String(defaultStyle.userActionMarkerType)).toString());

    maxTooltipDistance=settings.value(group+"max_tooltip_distance", defaultStyle.maxTooltipDistance).toInt();
    userActionCatchSensitivity=settings.value(group+"useraction_catch_sensitivity", defaultStyle.userActionCatchSensitivity).toInt();
    userActionFontName=settings.value(group+"useraction_font_name", defaultStyle.userActionFontName).toString();
    userActionFontSize=settings.value(group+"useraction_font_size", defaultStyle.userActionFontSize).toDouble();

    toolbarEnabled=settings.value(group+"toolbar_enabled", defaultStyle.toolbarEnabled).toBool();
    toolbarAlwaysOn=settings.value(group+"toolbar_always_visible", defaultStyle.toolbarAlwaysOn).toBool();
    displayMousePosition=settings.value(group+"display_mouse_position", defaultStyle.displayMousePosition).toBool();
    usePaletteColors=settings.value(group+"use_palette_colors", defaultStyle.usePaletteColors).toBool();


    auto readID=[](const QString& k, const QString& start) {
        if (k.startsWith(start)) {
            QString kk=k;
            kk.remove(0, start.size());
            QString num="";
            while (!kk.isEmpty() && kk.at(0).isDigit()) {
                num+=kk.at(0);
                kk.remove(0, 1);
            }
            bool ok=false;
            int id=num.toUInt(&ok);
            if (ok) {
                return id;
            }
        }
        return -1;
    };

    registeredMouseMoveActions.clear();
    registeredMouseWheelActions.clear();
    registeredMouseDragActionModes.clear();
    registeredMouseDoubleClickActions.clear();
    QStringList allk=settings.allKeys();
    for (auto& k: allk) {
        if (k.endsWith("/action")) {
            int id=readID(k, group+"actions/mouse_drag");
            if (id>=0) {
                auto button=jkqtp_String2MouseButton(settings.value(group+"actions/mouse_drag"+QString::number(id)+"/button", Qt::NoButton).toString());
                auto modifiers=jkqtp_String2KeyboardModifiers(settings.value(group+"actions/mouse_drag"+QString::number(id)+"/modifiers", Qt::NoModifier).toString());
                auto action=String2JKQTPMouseDragActions(settings.value(group+"actions/mouse_drag"+QString::number(id)+"/action", jkqtpmdaZoomByRectangle).toString());
                registeredMouseDragActionModes[qMakePair(button, modifiers)]=action;
            }

            id=readID(k, group+"actions/mouse_doubleclick");
            if (id>=0) {
                auto button=jkqtp_String2MouseButton(settings.value(group+"actions/mouse_doubleclick"+QString::number(id)+"/button", Qt::NoButton).toString());
                auto modifiers=jkqtp_String2KeyboardModifiers(settings.value(group+"actions/mouse_doubleclick"+QString::number(id)+"/modifiers", Qt::NoModifier).toString());
                auto action=String2JKQTPMouseDoubleClickActions(settings.value(group+"actions/mouse_doubleclick"+QString::number(id)+"/action", jkqtpdcaClickZoomsIn).toString());
                registeredMouseDoubleClickActions[qMakePair(button, modifiers)]=action;
            }

            id=readID(k, group+"actions/mouse_wheel");
            if (id>=0) {
                auto modifiers=jkqtp_String2KeyboardModifiers(settings.value(group+"actions/mouse_wheel"+QString::number(id)+"/modifiers", Qt::NoModifier).toString());
                auto action=String2JKQTPMouseWheelActions(settings.value(group+"actions/mouse_wheel"+QString::number(id)+"/action", jkqtpmwaZoomByWheel).toString());
                registeredMouseWheelActions[modifiers]=action;
            }
            id=readID(k, group+"actions/mouse_move");
            if (id>=0) {
                auto modifiers=jkqtp_String2KeyboardModifiers(settings.value(group+"actions/mouse_move"+QString::number(id)+"/modifiers", Qt::NoModifier).toString());
                auto action=String2JKQTPMouseMoveActions(settings.value(group+"actions/mouse_move"+QString::number(id)+"/action", jkqtpmwaZoomByWheel).toString());
                registeredMouseMoveActions[modifiers]=action;
            }
        }
    }
}

void JKQTPlotterStyle::saveSettings(QSettings &settings, const QString &group) const
{
    int cnt=0;

    settings.setValue(group+"max_tooltip_distance", maxTooltipDistance);
    settings.setValue(group+"useraction_catch_sensitivity", userActionCatchSensitivity);

    settings.setValue(group+"useraction_overlay_color", jkqtp_QColor2String(userActionOverlayPen.color()));
    settings.setValue(group+"useraction_overlay_penstyle", jkqtp_QPenStyle2String(userActionOverlayPen.style()));
    settings.setValue(group+"useraction_overlay_linewidth", userActionOverlayPen.widthF());
    settings.setValue(group+"useraction_overlay_brushcolor", jkqtp_QColor2String(userActionOverlayBrush.color()));
    settings.setValue(group+"useraction_overlay_brushstyle", jkqtp_QBrushStyle2String(userActionOverlayBrush.style()));

    settings.setValue(group+"useraction_opaque_color", jkqtp_QColor2String(userActionOpaquePen.color()));
    settings.setValue(group+"useraction_opaque_penstyle", jkqtp_QPenStyle2String(userActionOpaquePen.style()));
    settings.setValue(group+"useraction_opaque_linewidth", userActionOpaquePen.widthF());
    settings.setValue(group+"useraction_opaque_brushcolor", jkqtp_QColor2String(userActionOpaqueBrush.color()));
    settings.setValue(group+"useraction_opaque_brushstyle", jkqtp_QBrushStyle2String(userActionOpaqueBrush.style()));

    settings.setValue(group+"useraction_marker_color", jkqtp_QColor2String(userActionMarkerPen.color()));
    settings.setValue(group+"useraction_marker_penstyle", jkqtp_QPenStyle2String(userActionMarkerPen.style()));
    settings.setValue(group+"useraction_marker_linewidth", userActionMarkerPen.widthF());
    settings.setValue(group+"useraction_marker_brushcolor", jkqtp_QColor2String(userActionMarkerBrush.color()));
    settings.setValue(group+"useraction_marker_brushstyle", jkqtp_QBrushStyle2String(userActionMarkerBrush.style()));
    settings.setValue(group+"useraction_marker_diameter", userActionMarkerDiameter);
    settings.setValue(group+"useraction_marker_type", JKQTPUserActionMarkerType2String(userActionMarkerType));

    settings.setValue(group+"useraction_font_name", userActionFontName);
    settings.setValue(group+"useraction_font_size", userActionFontSize);


    settings.setValue(group+"toolbar_enabled", toolbarEnabled);
    settings.setValue(group+"toolbar_always_visible", toolbarAlwaysOn);
    settings.setValue(group+"toolbar_icon_size", toolbarIconSize);
    settings.setValue(group+"mouse_position_template", mousePositionTemplate);
    settings.setValue(group+"display_mouse_position", displayMousePosition);
    settings.setValue(group+"use_palette_colors", usePaletteColors);


    cnt=0;
    for (auto it=registeredMouseDragActionModes.begin(); it!=registeredMouseDragActionModes.end(); ++it) {
        settings.setValue(group+"actions/mouse_drag"+QString::number(cnt)+"/button", jkqtp_MouseButton2String(it.key().first));
        settings.setValue(group+"actions/mouse_drag"+QString::number(cnt)+"/modifiers", jkqtp_KeyboardModifiers2String(it.key().second));
        settings.setValue(group+"actions/mouse_drag"+QString::number(cnt)+"/action", JKQTPMouseDragActions2String(it.value()));
        cnt++;
    }

    cnt=0;
    for (auto it=registeredMouseDoubleClickActions.begin(); it!=registeredMouseDoubleClickActions.end(); ++it) {
        settings.setValue(group+"actions/mouse_doubleclick"+QString::number(cnt)+"/button", jkqtp_MouseButton2String(it.key().first));
        settings.setValue(group+"actions/mouse_doubleclick"+QString::number(cnt)+"/modifiers", jkqtp_KeyboardModifiers2String(it.key().second));
        settings.setValue(group+"actions/mouse_doubleclick"+QString::number(cnt)+"/action", JKQTPMouseDoubleClickActions2String(it.value()));
        cnt++;
    }

    cnt=0;
    for (auto it=registeredMouseWheelActions.begin(); it!=registeredMouseWheelActions.end(); ++it) {
        settings.setValue(group+"actions/mouse_wheel"+QString::number(cnt)+"/modifiers", jkqtp_KeyboardModifiers2String(it.key()));
        settings.setValue(group+"actions/mouse_wheel"+QString::number(cnt)+"/action", JKQTPMouseWheelActions2String(it.value()));
        cnt++;
    }

    cnt=0;
    for (auto it=registeredMouseMoveActions.begin(); it!=registeredMouseMoveActions.end(); ++it) {
        settings.setValue(group+"actions/mouse_move"+QString::number(cnt)+"/modifiers", jkqtp_KeyboardModifiers2String(it.key()));
        settings.setValue(group+"actions/mouse_move"+QString::number(cnt)+"/action", JKQTPMouseMoveActions2String(it.value()));
        cnt++;
    }
}


JKQTPlotterStyle &JKQTPGetSystemDefaultStyle()
{
    static JKQTPlotterStyle instance=JKQTPlotterStyle();
    return instance;
}

void JKQTPSetSystemDefaultStyle(JKQTPlotterStyle &newStyle)
{
    JKQTPGetSystemDefaultStyle().operator=(newStyle);
}
