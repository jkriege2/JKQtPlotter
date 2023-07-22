#ifndef TEST_MULTIPLOT_UI_H
#define TEST_MULTIPLOT_UI_H

#include <QDialog>
#include "jkqtplotter/jkqtplotter.h"
#include <QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
    class TestMultiplotUI;
}
QT_END_NAMESPACE

class TestMultiplotUI : public QDialog
{
        Q_OBJECT

    public:
        explicit TestMultiplotUI(JKQTPlotter* plotTop_, JKQTPlotter* plotBot_, JKQTPlotter* plotRight_, QGridLayout* plotLayout_, QWidget *parent = nullptr);
        ~TestMultiplotUI();

    protected Q_SLOTS:
        void updatePlot();
    private:
        JKQTPlotter* plotTop;
        JKQTPlotter* plotBot;
        JKQTPlotter* plotRight;
        QGridLayout* plotLayout;

        Ui::TestMultiplotUI *ui;
};

#endif // TEST_MULTIPLOT_UI_H
