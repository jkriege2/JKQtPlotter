#ifndef FORMWITHJKQTPLOTTER_H
#define FORMWITHJKQTPLOTTER_H

#include <QWidget>
#include "jkqtplotter/jkqtpgraphsparsedfunction.h"

namespace Ui {
    class FormWithJKQTPlotter;
}

class FormWithJKQTPlotter : public QWidget
{
        Q_OBJECT

    public:
        explicit FormWithJKQTPlotter(QWidget *parent = nullptr);
        ~FormWithJKQTPlotter();
    protected slots:
        void on_chkLogX_toggled(bool checked);
        void on_chkLogY_toggled(bool checked);
        void on_btnReplot_clicked();

    private:
        Ui::FormWithJKQTPlotter *ui;

        JKQTPXParsedFunctionLineGraph* graph;
};

#endif // FORMWITHJKQTPLOTTER_H
