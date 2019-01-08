#ifndef FORMWITHJKQTPLOTTER_H
#define FORMWITHJKQTPLOTTER_H

#include <QWidget>
#include "jkqtplotter/jkqtpgraphsparsedfunction.h"

namespace Ui {
    class FormWithJKQtPlotter;
}

class FormWithJKQtPlotter : public QWidget
{
        Q_OBJECT

    public:
        explicit FormWithJKQtPlotter(QWidget *parent = nullptr);
        ~FormWithJKQtPlotter();
    protected slots:
        void on_chkLogX_toggled(bool checked);
        void on_chkLogY_toggled(bool checked);
        void on_btnReplot_clicked();

    private:
        Ui::FormWithJKQtPlotter *ui;

        JKQTPxParsedFunctionLineGraph* graph;
};

#endif // FORMWITHJKQTPLOTTER_H
