#include <QtGui>

#include "mainwindow.h"
#include "finddialog.h"
#include "spreadsheet.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    findDialog = 0;

    readSettings();

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createCentralWidget();
    updateStatusBar();

    setCurrentFile("");
    setWindowIcon(QIcon::fromTheme("gnome-gnumeric"));
}

// Slots
void MainWindow::newFile()
{
    if(okToContinue()) {
        spreadsheet->clear();
        setCurrentFile("");
    }
}

bool MainWindow::openFile()
{
    if(okToContinue()) {
        QString fileName = QFileDialog::
                           getOpenFileName(this, tr("QSpreadsheet"), ".",
                                           tr("QSpreadsheet files (*.qsp)"));
        if(!fileName.isEmpty()) {
            return loadFile(fileName);
        }
    }
    return false;
}

bool MainWindow::save()
{
    if(!curFile.isEmpty()) {
        return saveFile(curFile);
    } else {
        return saveAs();
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::
                       getSaveFileName(this, tr("QSpreadsheet"), ".",
                                       tr("QSpreadsheet files (*.qsp)"));
    if(!fileName.isEmpty())
    {
        saveFile(fileName);
    }

    return false;
}

void MainWindow::find()
{
    if(!findDialog)
    {
        findDialog = new FindDialog(this);
    }
    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("QSpreasheet"),
                       tr("<h2>QSpreadsheet</h2>"
                          "<p>Copyright (C) 2010, Andrew Kazancev</p>"
                          "<p>QSpreadsheet is a simple application writed with "
                          "Qt</p>"));
}

void MainWindow::modified()
{
    setWindowModified(true);
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    locationLabel->setText(spreadsheet->currentLocation());
    formulaLabel->setText(spreadsheet->currentFormula());
}

// Methods
void MainWindow::createActions()
{
    // File actions
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon::fromTheme("document-new"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new spreadsheet"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open"), this);
    openAction->setIcon(QIcon::fromTheme("document-open"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a spreadsheet"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon::fromTheme("document-save"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save a spreadsheet"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save &As"), this);
    saveAsAction->setIcon(QIcon::fromTheme("document-save-as"));
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save a spreadsheet in new file"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setIcon(QIcon::fromTheme("application-exit"));
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    // Edit actions
    cutAction = new QAction(tr("&Cut"), this);
    cutAction->setIcon(QIcon::fromTheme("edit-cut"));
    cutAction->setShortcut(QKeySequence::Cut);

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon::fromTheme("edit-copy"));
    copyAction->setShortcut(QKeySequence::Copy);

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon::fromTheme("edit-paste"));
    pasteAction->setShortcut(QKeySequence::Paste);

    findAction = new QAction(tr("&Find"), this);
    findAction->setIcon(QIcon::fromTheme("edit-find"));
    findAction->setShortcut(QKeySequence::Find);
    connect(findAction, SIGNAL(triggered()), this, SLOT(find()));

    // Help actions
    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setIcon(QIcon::fromTheme("help-about"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(findAction);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);
    fileToolBar->addAction(saveAsAction);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addAction(findAction);
}

void MainWindow::createStatusBar()
{
    locationLabel = new QLabel(" W999 ");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());
    statusBar()->addWidget(locationLabel);

    formulaLabel = new QLabel("Some mathematic formula...");
    formulaLabel->setIndent(3);
    statusBar()->addWidget(formulaLabel);
}

void MainWindow::createCentralWidget()
{
    spreadsheet = new Spreadsheet();
    setCentralWidget(spreadsheet);
    connect(spreadsheet, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(modified()));
    connect(spreadsheet, SIGNAL(currentCellChanged(int,int,int,int)), this,
            SLOT(updateStatusBar()));
}

void MainWindow::saveSettings()
{
    QSettings config("akazancev", "QSpreadsheet");

    config.setValue("geometry", saveGeometry());
}

void MainWindow::readSettings()
{
    QSettings config("akazancev", "QSpreadsheet");

    restoreGeometry(config.value("geometry").toByteArray());
}

bool MainWindow::okToContinue()
{
    if(isWindowModified()) {
        QMessageBox msg;
        msg.setWindowTitle(tr("QSpreadsheet"));
        msg.setIcon(QMessageBox::Warning);
        msg.setText(tr("The document was modified."));
        msg.setInformativeText(tr("Do you want to save your changes?"));
        msg.setStandardButtons(QMessageBox::Cancel | QMessageBox::No |
                               QMessageBox::Yes);
        msg.setDefaultButton(QMessageBox::Yes);

        int responde = msg.exec();
        if(responde == QMessageBox::Yes) {
            save();
        } else if(responde == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    QString shownName = tr("Untitled");
    if(!fileName.isEmpty()) {
        curFile = fileName;
        shownName = strippedName(fileName);
    }
    setWindowTitle(tr("%1[*] - %2").arg(shownName, tr("QSpreadsheet")));
    setWindowModified(false);
}

QString MainWindow::strippedName(const QString &fileName)
{
    return QFileInfo(fileName).fileName();
}

bool MainWindow::saveFile(const QString &fileName)
{
    if(spreadsheet->writeFile(fileName))
    {
        setCurrentFile(fileName);
        return true;
    }
    return false;
}

bool MainWindow::loadFile(const QString &fileName)
{
    if(spreadsheet->readFile(fileName))
    {
        setCurrentFile(fileName);
        return true;
    }
    return false;
}

// Events
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(okToContinue()) {
        saveSettings();
        event->accept();
    } else {
        event->ignore();
    }
}
