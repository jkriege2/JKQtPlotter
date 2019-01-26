#ifndef TEST_USER_INTERACTION_H
#define TEST_USER_INTERACTION_H

#include <QWidget>
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplotter/jkqtpgraphs.h"
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

    signals:

    public slots:
        void setLeftMouseAction(int index);
        void setPlotMagnification(int index);

        void plotMouseMove(double x, double y);
        void plotMouseClicked(double x, double y, Qt::KeyboardModifiers modifiers, Qt::MouseButton button);
        void plotMouseDoubleClicked(double x, double y, Qt::KeyboardModifiers modifiers, Qt::MouseButton button);
        void plotNewZoomRectangle(double mouseDragRectXStart, double mouseDragRectXEnd, double mouseDragRectYStart, double mouseDragRectYEnd, Qt::KeyboardModifiers modifiers);
        void contextMenuOpened(double x, double y, QMenu* contextMenu);
        void zoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* sender);
        void userClickFinished(double x, double y, Qt::KeyboardModifiers modifiers);
        void userScribbleClick(double x, double y, Qt::KeyboardModifiers modifiers, bool first, bool last);
        void userRectangleFinished(double x, double y, double width, double height, Qt::KeyboardModifiers modifiers);
        void userLineFinished(double x1, double y1, double x2, double y2, Qt::KeyboardModifiers modifiers);
        void userCircleFinished(double x, double y, double radius, Qt::KeyboardModifiers modifiers);
        void userEllipseFinished(double x, double y, double radiusX, double radiusY, Qt::KeyboardModifiers modifiers);
    protected:
        void initPlot();
        JKQTPlotter* plot;
        QGridLayout* layout;
        QHBoxLayout* layChk;
        QHBoxLayout* layChk2;
        QFormLayout* layForm;
        QCheckBox* chkPositionDisplay;
        QCheckBox* chkShowToolbar;
        QCheckBox* chkToolbarAlwaysOn;
        QCheckBox* chkGrid;
        QComboBox* cmbMouseAction;
        QCheckBox* chkLogX;
        QCheckBox* chkLogY;
        QCheckBox* chkZoomByMouseWheel;
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
