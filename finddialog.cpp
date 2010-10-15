#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    setWindowTitle("Find");
    setMaximumHeight(sizeHint().height());
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

FindDialog::~FindDialog()
{

}

void FindDialog::on_findLineEdit_textChanged(QString text)
{
    findButton->setEnabled(!text.isEmpty());
}

void FindDialog::on_findButton_clicked()
{
    QString text = findButton->text();
    Qt::CaseSensitivity cs = (caseCheckBox->isChecked() ? Qt::CaseSensitive :
                              Qt::CaseInsensitive);
    if(backwardChechBox->isChecked()) {
        emit findPrev(text, cs);
    } else {
        emit findNext(text, cs);
    }
}
