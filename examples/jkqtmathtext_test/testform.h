#ifndef TESTFORM_H
#define TESTFORM_H

#include <QWidget>
#include <QStringList>
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtcommon/jkqtphighrestimer.h"
#include <QPainter>
#include <QTreeWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class TestForm;
}
QT_END_NAMESPACE

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
        double draw(QPainter& painter, double X, double YY, JKQTMathText& mt, QString name, double &durationSizingMS, double &durationTimingMS, QStringList *lstErrors=nullptr);
        double drawAligned(QPainter& painter, double X, double YY, JKQTMathText& mt, QString name);
        QString getFonts(const JKQTMathText& mt) const;
        QTreeWidgetItem* createTree(JKQTMathTextNode* node, QTreeWidgetItem *parent=NULL);
};

#endif // TESTFORM_H
