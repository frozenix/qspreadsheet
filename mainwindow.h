#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class QLabel;
class QToolBar;
class QCloseEvent;
class FindDialog;
class Spreadsheet;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

protected:
    void closeEvent(QCloseEvent *event);

public slots:

private slots:
    void newFile();
    bool openFile();
    bool save();
    bool saveAs();
    void find();
    void about();
    void modified();
    void updateStatusBar();

private:
    QString curFile;
    FindDialog *findDialog;

    // Actions
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;

    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *findAction;

    QAction *aboutAction;
    QAction *aboutQtAction;

    // Menus
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    // ToolBars
    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    // Status Bar
    QLabel *locationLabel;
    QLabel *formulaLabel;

    // Central Widget
    Spreadsheet *spreadsheet;

    // Methods
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createCentralWidget();

    bool saveFile(const QString &fileName);
    bool loadFile(const QString &fileName);

    void saveSettings();
    void readSettings();

    bool okToContinue();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fileName);
};

#endif // MAINWINDOW_H
