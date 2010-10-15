#include <QtGui>

#include "spreadsheet.h"

Spreadsheet::Spreadsheet(QWidget *parent) :
    QTableWidget(parent)
{
    setRowCount(20);
    setColumnCount(26);
    setCurrentCell(0, 0);

    for(int i = 0; i < columnCount(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(QString(QChar('A' + i)));
        setHorizontalHeaderItem(i, item);
        setColumnWidth(i, 100);
    }
}

QString Spreadsheet::currentLocation()
{
    return QChar('A' + currentColumn()) + QString::number(currentRow() + 1);
}

QString Spreadsheet::currentFormula()
{
    QTableWidgetItem *item = currentItem();
    if(item != NULL)
    {
        return item->text();
    }
    return "";
}

QString Spreadsheet::formulaString(int r, int c)
{
    QTableWidgetItem *cell = item(r, c);
    if(cell != NULL) {
        return cell->text();
    }
    return "";
}

void Spreadsheet::setFormula(int r, int c, const QString &str)
{
    QTableWidgetItem *cell = item(r, c);
    if(cell == NULL)
    {
        cell = new QTableWidgetItem();
        setItem(r, c, cell);
    }
    cell->setText(str);
}

bool Spreadsheet::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox msg;
        msg.setWindowTitle(tr("QSpreadsheet"));
        msg.setIcon(QMessageBox::Critical);
        msg.setText(tr("Unable to save spreadsheet."));
        msg.setInformativeText(tr("Cannot write file %1: %2").
                               arg(file.fileName(), file.errorString()));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();

        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint32(MagicNumber);

    QString formula;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    for(int row = 0; row < rowCount(); ++row)
    {
        for(int col = 0; col < columnCount(); ++col)
        {
            formula = formulaString(row, col);
            if(!formula.isEmpty())
            {
                out << quint16(row) << quint16(col) << formula;
            }
        }
    }
    QApplication::restoreOverrideCursor();

    return true;
}

bool Spreadsheet::readFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox msg;
        msg.setWindowTitle(tr("QSpreadsheet"));
        msg.setIcon(QMessageBox::Critical);
        msg.setText(tr("Unable to load spreadsheet."));
        msg.setInformativeText(tr("Cannot read file %1: %2").
                               arg(file.fileName(), file.errorString()));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();

        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_7);

    quint32 magic;
    in >> magic;
    if(magic != MagicNumber)
    {
        QMessageBox msg;
        msg.setWindowTitle(tr("QSpreadsheet"));
        msg.setIcon(QMessageBox::Critical);
        msg.setText(tr("Unknown type of file."));
        msg.setInformativeText(tr("Unable to recognize format of file."));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();

        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    clear();

    QString formula;
    quint16 row, col;
    while(!in.atEnd())
    {
        in >> row >> col >> formula;
        setFormula(row, col, formula);
    }

    QApplication::restoreOverrideCursor();

    return true;
}

void Spreadsheet::clear()
{
    clearContents();
    setCurrentCell(0, 0);

    for(int i = 0; i < columnCount(); ++i)
    {
        setColumnWidth(i, 100);
    }
}
