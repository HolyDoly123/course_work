#include "mainwindow.h"

//Mainwindow
//TODO: Central widget (Graphic scene)
//TODO: Dock window (graph tree)
//TODO: preferencies, check input, build table/code
//TODO: save/load file, isModified()

//Other
//TODO: Create FA classes

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createMenus();
    createStatusBar();
    createDockWindows();

    QGraphicsView *scene = new QGraphicsView;
    setCentralWidget(scene);

    readSettings();
    setCurrentFile(QString());
    setWindowTitle(tr("PCADDFA"));
    setMinimumSize(480, 320);
    resize(640, 480);
}

/*
#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(newAct);
    menu.addAction(openAct);
    menu.exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU
*/

void MainWindow::createActions()
{
    newAct = new QAction(tr("New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    openAct = new QAction(tr("Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAsAct = new QAction(tr("Save as..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);

    preferencesAct = new QAction(tr("Preferences"), this);
    preferencesAct->setShortcuts(QKeySequence::Preferences);
    preferencesAct->setStatusTip(tr("Look up settigns"));
    connect(preferencesAct, &QAction::triggered, this, &MainWindow::preferences);

    closeAct = new QAction(tr("Exit"), this);
    closeAct->setShortcuts(QKeySequence::Quit);
    closeAct->setStatusTip(tr("Exit the application"));
    connect(closeAct, &QAction::triggered, this, &QWidget::close);

    checkInputAct = new QAction(tr("Input"), this);
    checkInputAct->setStatusTip(tr("Run DFA with custom input"));
    connect(checkInputAct, &QAction::triggered, this, &MainWindow::checkInput);

    buildTableAct = new QAction(tr("Table"), this);
    buildTableAct->setShortcut(QKeySequence(tr("Ctrl+R")));
    buildTableAct->setStatusTip(tr("Build the transition table from graph"));
    connect(buildTableAct, &QAction::triggered, this, &MainWindow::buildTable);

    buildCodeAct = new QAction(tr("Code"), this);
    buildCodeAct->setStatusTip(tr("Build C++ code from graph"));
    connect(buildCodeAct, &QAction::triggered, this, &MainWindow::buildCode);

    manualAct = new QAction(tr("Manual"), this);
    manualAct->setShortcuts(QKeySequence::HelpContents);
    manualAct->setStatusTip(tr("Show the application's manual"));
    connect(manualAct, &QAction::triggered, this, &MainWindow::manual);

    aboutAct = new QAction(tr("About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(preferencesAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);

    checkMenu = menuBar()->addMenu(tr("&Check"));
    checkMenu->addAction(checkInputAct);

    buildMenu = menuBar()->addMenu(tr("&Build"));
    buildMenu->addAction(buildTableAct);
    buildMenu->addSeparator();
    buildMenu->addAction(buildCodeAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(manualAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
}

void MainWindow::createStatusBar()
{
    QLabel *message = new QLabel(tr("A context menu is available by right-clicking"));
    statusbar = statusBar();
    statusbar->addWidget(message);
}

void MainWindow::createDockWindows()
{
    QDockWidget *graphTreeDock = new QDockWidget(tr("Graph tree"), this);
    graphTreeDock->setAllowedAreas(Qt::RightDockWidgetArea);
    graphTreeDock->setStatusTip("Graph in form of tree list");
    QTreeWidget *placeholder = new QTreeWidget();
    graphTreeDock->setWidget(placeholder);
    graphTreeDock->setMaximumWidth(this->width()*0.2);
    addDockWidget(Qt::RightDockWidgetArea, graphTreeDock);
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        //clear();
        setCurrentFile(QString());
    }
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open graph"), QString(),
                                                        tr( "Application (*.dfa)"));
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool MainWindow::saveAs()
{
    QFileDialog dialog(this, tr("Save graph"), curFile,
                       tr( "Image (*.svg);;Application (*.dfa)"));
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}

void MainWindow::preferences()
{

}

void MainWindow::checkInput()
{

}

void MainWindow::buildTable()
{

}

void MainWindow::buildCode()
{

}

void MainWindow::manual()
{
    QMessageBox::information(this, tr("Manual"), tr("See guides"), QMessageBox::Help);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
             tr("The <b>Application</b> is made for people "
                "who write modern compilers and digital systems, "
                "and who needs convenient tool for building DFA."));
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
}

bool MainWindow::maybeSave()
{
    if (!isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return saveAs();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    QString errorMessage;
/*
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << textEdit->toPlainText();
        if (!file.commit()) {
            errorMessage = tr("Cannot write file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    } else {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                       .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("Application"), errorMessage);
        return false;
    }
*/
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    /*
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
*/
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::isModified()
{
    return false;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

MainWindow::~MainWindow()
{
}

