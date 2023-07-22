#ifndef JKQTPSTYLEPLAINTEXTEDIT_H
#define JKQTPSTYLEPLAINTEXTEDIT_H

#include <QPlainTextEdit>

class JKQTPStylePlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    JKQTPStylePlainTextEdit(QWidget *parent = nullptr);
    virtual ~JKQTPStylePlainTextEdit();
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
protected Q_SLOTS:
    void changeCurrentLineValueTo(const QString &targetText);
    void addTransparencyToCurrentLineValue(const QString &targetText);
    void addMathFontSpecifier(const QString &newMath);
private:
    QTextCursor curContextMenu;
};

#endif // JKQTPSTYLEPLAINTEXTEDIT_H
