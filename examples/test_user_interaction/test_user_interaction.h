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

    protected:
        void initPlot();
        JKQTPlotter* plot;
        QGridLayout* layout;
        QHBoxLayout* layChk;
        QFormLayout* layForm;
        QCheckBox* chkPositionDisplay;
        QCheckBox* chkShowToolbar;
        QCheckBox* chkToolbarAlwaysOn;
        QCheckBox* chkGrid;
        JKQTPXYLineGraph* graph1;
        JKQTPXYLineGraph* graph2;
};

#endif // TEST_USER_INTERACTION_H
