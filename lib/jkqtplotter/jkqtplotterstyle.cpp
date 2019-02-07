#include "jkqtplotterstyle.h"
#include "jkqtplottertools/jkqttools.h"
#include "jkqtplottertools/jkqtptools.h"

JKQTPlotterStyle::JKQTPlotterStyle():
    toolbarIconSize(16),
    userActionColor("steelblue"),
    userActionCompositionMode(QPainter::CompositionMode_SourceOver),
    mousePositionTemplate("(%1; %2)"),
    displayMousePosition(true),
    toolbarEnabled(true),
    toolbarAlwaysOn(false),
    usePaletteColors(true)
{
    // default user-actions:
    registeredMouseDragActionModes[qMakePair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::NoModifier)]=JKQTPMouseDragActions::jkqtpmdaZoomByRectangle;
    registeredMouseDragActionModes[qMakePair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ControlModifier)]=JKQTPMouseDragActions::jkqtpmdaPanPlotOnMove;
    registeredMouseDoubleClickActions[qMakePair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::NoModifier)]=JKQTPMouseDoubleClickActions::jkqtpdcaClickMovesViewport;
    registeredMouseWheelActions[Qt::NoModifier]=JKQTPMouseWheelActions::jkqtpmwaZoomByWheel;
}

void JKQTPlotterStyle::loadSettings(const QSettings &settings, const QString &group, const JKQTPlotterStyle &defaultStyle)
{
    toolbarIconSize=settings.value(group+"toolbar_icon_size", defaultStyle.toolbarIconSize).toInt();
    mousePositionTemplate=settings.value(group+"mouse_position_template", defaultStyle.mousePositionTemplate).toString();
    userActionColor=jkqtp_String2QColor(settings.value(group+"useraction_color", jkqtp_QColor2String(defaultStyle.userActionColor)).toString());
    toolbarEnabled=settings.value(group+"toolbar_enabled", defaultStyle.toolbarEnabled).toBool();
    toolbarAlwaysOn=settings.value(group+"toolbar_always_visible", defaultStyle.toolbarAlwaysOn).toBool();
    displayMousePosition=settings.value(group+"display_mouse_position", defaultStyle.displayMousePosition).toBool();
    usePaletteColors=settings.value(group+"use_palette_colors", defaultStyle.usePaletteColors).toBool();


    auto readID=[](const QString& k, const QString& start) {
        if (k.startsWith(start)) {
            QString kk=k;
            kk.remove(0, start.size());
            QString num="";
            while (kk.front().isDigit()) {
                num+=kk.front();
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
        }
    }
}

void JKQTPlotterStyle::saveSettings(QSettings &settings, const QString &group) const
{
    int cnt=0;

    settings.setValue(group+"useraction_color", jkqtp_QColor2String(userActionColor));
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
