#ifndef FORMWITHJKQTPLOTTER_H
#define FORMWITHJKQTPLOTTER_H

#include <QWidget>
#include "jkqtplotter/jkqtpgraphsparsedfunction.h"

namespace Ui {
    class FormWithJKQTPLotter;
}

class FormWithJKQTPLotter : public QWidget
{
        Q_OBJECT

    public:
        explicit FormWithJKQTPLotter(QWidget *parent = nullptr);
        ~FormWithJKQTPLotter();
    protected slots:
        void on_chkLogX_toggled(bool checked);
        void on_chkLogY_toggled(bool checked);
        void on_btnReplot_clicked();

    private:
        Ui::FormWithJKQTPLotter *ui;

        JKQTPXParsedFunctionLineGraph* graph;
};

#endif // FORMWITHJKQTPLOTTER_H
