#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QTableWidget>

class QWidget;

class Spreadsheet : public QTableWidget
{
    Q_OBJECT

public:
    Spreadsheet(QWidget *parent = 0);

    QString currentLocation();
    QString currentFormula();
    bool writeFile(const QString &fileName);
    bool readFile(const QString &fileName);
    QString formulaString(int r, int c);
    void setFormula(int r, int c, const QString &str);
signals:

public slots:
    void clear();

private:
    enum { MagicNumber = 0xDEADBEAF };
};

#endif // SPREADSHEET_H
