#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

#include "ui_finddialog.h"

class FindDialog : public QDialog, private Ui::FindDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = 0);
    ~FindDialog();

private:

signals:
    void findNext(QString text, Qt::CaseSensitivity cs);
    void findPrev(QString text, Qt::CaseSensitivity cs);

private slots:
    void on_findButton_clicked();
    void on_findLineEdit_textChanged(QString );
};

#endif // FINDDIALOG_H
