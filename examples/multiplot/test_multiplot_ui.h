#ifndef TEST_MULTIPLOT_UI_H
#define TEST_MULTIPLOT_UI_H

#include <QDialog>
#include "jkqtplotter/jkqtplotter.h"
#include <QGridLayout>

namespace Ui {
    class TestMultiplotUI;
}

class TestMultiplotUI : public QDialog
{
        Q_OBJECT

    public:
        explicit TestMultiplotUI(JKQTPlotter* plotTop_, JKQTPlotter* plotBot_, JKQTPlotter* plotRight_, QGridLayout* plotLayout_, QWidget *parent = nullptr);
        ~TestMultiplotUI();

    protected slots:
        void updatePlot();
    private:
        JKQTPlotter* plotTop;
        JKQTPlotter* plotBot;
        JKQTPlotter* plotRight;
        QGridLayout* plotLayout;

        Ui::TestMultiplotUI *ui;
};

#endif // TEST_MULTIPLOT_UI_H
