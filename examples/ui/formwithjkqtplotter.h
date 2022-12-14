#ifndef FORMWITHJKQTPLOTTER_H
#define FORMWITHJKQTPLOTTER_H

#include <QWidget>
#include "jkqtplotter/graphs/jkqtpparsedfunction.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class FormWithJKQTPlotter;
}
QT_END_NAMESPACE

class FormWithJKQTPlotter : public QWidget
{
        Q_OBJECT

    public:
        explicit FormWithJKQTPlotter(QWidget *parent = nullptr);
        ~FormWithJKQTPlotter();
    protected Q_SLOTS:
        void on_chkLogX_toggled(bool checked);
        void on_chkLogY_toggled(bool checked);
        void on_btnReplot_clicked();

    private:
        Ui::FormWithJKQTPlotter *ui;

        JKQTPXParsedFunctionLineGraph* graph;
};

#endif // FORMWITHJKQTPLOTTER_H
