#ifndef TEST_STYLING_H
#define TEST_STYLING_H

#include <QMainWindow>
#include "jkqtplotter/jkqtplotter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class TestStyling;
}
QT_END_NAMESPACE

class TestStyling : public QMainWindow
{
        Q_OBJECT

    public:
        explicit TestStyling(QWidget *parent = nullptr);
        ~TestStyling();

    protected slots:
        void on_btnUpdate_clicked();
        void on_btnLoad_clicked(const QString &filename=QString());
        void on_btnSave_clicked();

    private:
        Ui::TestStyling *ui;
        void initPlot();
        QPointer<JKQTPlotter> plotExtra;
};

#endif // TEST_STYLING_H
