#ifndef TESTFORM_H
#define TESTFORM_H

#include <QWidget>
#include <QStringList>
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtplottertools/jkqtphighrestimer.h"
#include <QPainter>
#include <QTreeWidget>


namespace Ui {
    class TestForm;
}

class TestForm : public QWidget
{
        Q_OBJECT
        
    public:
        explicit TestForm(QWidget *parent = 0);
        ~TestForm();
        
    public slots:
        void updateMath();
    private:
        Ui::TestForm *ui;
        JKQTPHighResTimer ht;
        double draw(QPainter& painter, double X, double YY, JKQTmathText& mt, QString name, double &durationSizingMS, double &durationTimingMS);

        QTreeWidgetItem* createTree(JKQTmathText::MTnode* node, QTreeWidgetItem *parent=NULL);
};

#endif // TESTFORM_H
