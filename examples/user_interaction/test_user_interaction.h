#ifndef TEST_USER_INTERACTION_H
#define TEST_USER_INTERACTION_H

#include <QWidget>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/graphs/jkqtpscatter.h"
#include <QFormLayout>
#include <QCheckBox>
#include <QComboBox>
#include <cmath>
#include <QToolBar>
#include <QMainWindow>
#include <QHBoxLayout>

class TestUserInteraction : public QMainWindow
{
        Q_OBJECT
    public:
        explicit TestUserInteraction(QWidget *parent = nullptr);

    Q_SIGNALS:

    public Q_SLOTS:
        void setMouseMoveAction(int index);
        void setLeftMouseAction(int index);
        void setLeftCtrlMouseAction(int index);
        void setRightMouseAction(int index);
        void setPlotMagnification(int index);
        void setContextMenuMode(int index);
        void setLeftDoubleClickMouseAction(int index);
        void setRightDoubleClickMouseAction(int index);
        void setMouseWheelNoModAction(int index);
        void setUserActionMarkerType(int index);
        void setUserActionMarkerColor(int index);
        void setUserActionOverlayColor(int index);
        void setUserActionOverlayFillColor(int index);
        void setUserActionOpaqueColor(int index);
        void setUserActionOpaqueFillColor(int index);

        void plotMouseMove(double x, double y);
        void plotMouseClicked(double x, double y, Qt::KeyboardModifiers modifiers, Qt::MouseButton button);
        void plotMouseDoubleClicked(double x, double y, Qt::KeyboardModifiers modifiers, Qt::MouseButton button);
        void plotNewZoomRectangle(double mouseDragRectXStart, double mouseDragRectXEnd, double mouseDragRectYStart, double mouseDragRectYEnd, Qt::KeyboardModifiers modifiers);
        void plotMouseWheelOperated(double x, double y, Qt::KeyboardModifiers modifiers, int deltaAngleX, int deltaAngleY);
        void contextMenuOpened(double x, double y, QMenu* contextMenu);
        void zoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* sender);
        void userScribbleClick(double x, double y, Qt::KeyboardModifiers modifiers, bool first, bool last);
        void userRectangleFinished(double x, double y, double width, double height, Qt::KeyboardModifiers modifiers);
        void userLineFinished(double x1, double y1, double x2, double y2, Qt::KeyboardModifiers modifiers);
        void userCircleFinished(double x, double y, double radius, Qt::KeyboardModifiers modifiers);
        void userEllipseFinished(double x, double y, double radiusX, double radiusY, Qt::KeyboardModifiers modifiers);
        void tooltipDisplayed(double x, double y, const QStringList& entries, const QList<JKQTPPlotElement*>& graphs);
        void rulerDisplayed(double x1, double y1, double x2, double y2, Qt::KeyboardModifiers modifiers);
    protected:
        void initPlot();
        JKQTPlotter* plot;
        QGridLayout* layout;
        QHBoxLayout* layChk;
        QHBoxLayout* layChk2;
        QFormLayout* layForm;
        QFormLayout* layForm2;
        QFormLayout* layLab;
        QCheckBox* chkPositionDisplay;
        QCheckBox* chkShowToolbar;
        QCheckBox* chkToolbarAlwaysOn;
        QCheckBox* chkGrid;
        QComboBox* cmbMouseMoveAction;
        QComboBox* cmbLeftNoModMouseAction;
        QComboBox* cmbLeftCtrlModMouseAction;
        QComboBox* cmbRightNoModMouseAction;
        QComboBox* cmbRightClickContextMenu;
        QComboBox* cmbRightDoubleClickMouseAction;
        QComboBox* cmbLeftDoubleClickMouseAction;
        QComboBox* cmbMouseWheelAction;
        QComboBox* cmbUserActionMarkerType;
        QComboBox* cmbUserActionMarkerColor;
        QComboBox* cmbUserActionOverlayColor;
        QComboBox* cmbUserActionOverlayFillColor;
        QComboBox* cmbUserActionOpaqueColor;
        QComboBox* cmbUserActionOpaqueFillColor;
        QCheckBox* chkLogX;
        QCheckBox* chkLogY;
        QLabel* labMouseAction;
        QLabel* labMouseMoved;
        QLabel* labMouseClicked;
        QComboBox* cmbMagnification;
        JKQTPXYLineGraph* graph1;
        JKQTPXYLineGraph* graph2;

        static QString MouseButton2String(Qt::MouseButton button);
        static QString KeyboradMod2String(Qt::KeyboardModifiers modifiers);
};

#endif // TEST_USER_INTERACTION_H
