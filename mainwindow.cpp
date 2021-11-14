#include "grapharrow.h"
#include "graphvertex.h"
#include "graphscene.h"

#include "dfa.h"
#include "state.h"
#include "transition.h"
#include "mainwindow.h"
//Mainwindow
//TODO: save/open file (DFA), preferencies
//Other
//TODO:  Clear bugs (Arrow painting)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createMenus();
    createStatusBar();
    createDockWindows();

    scene = new GraphScene(&myDfa ,this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));

    view = new QGraphicsView(scene);
    setCentralWidget(view);

    readSettings();
    setCurrentFile(QString());
    setWindowTitle(tr("PCADDFA"));
    setMinimumSize(480, 320);
    resize(640, 480);
}

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
    graphTreeDock->setVisible(false);
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        //clear();
        setCurrentFile(QString());
    }
    setCurrentFile(QString());
    scene->clear();
    myDfa.clear();
    view->update();
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
                       tr( "Image .svg(*.svg);;Application .dfa(*.dfa)"));
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    QFileInfo fi(dialog.selectedFiles().first());
    QString extension = fi.completeSuffix();
    if(extension.isEmpty())
    {
        QMessageBox::warning(this, tr("Save Error"),
                                      "No extension given");
    }
    else if (extension == "svg")
    {
        return saveSvgFile(dialog.selectedFiles().first());
    }
    else if (extension == "dfa")
    {
        foreach(QGraphicsItem* item, scene->items())
        {
            qDebug() << item;
        }
    }
}

void MainWindow::preferences()
{
    swidget = new QWidget(this);
    swidget->setMinimumSize(480, 320);
    swidget->setWindowFlag(Qt::Window);
    swidget->show();
}

void MainWindow::checkInput()
{
    QString info = myDfa.isDFAValid();
    if(info == "OK")
    {
        bool ok;
        QString text = QInputDialog::getText(this, tr("Check input"),
                                             tr("Input signals in string with | separator"), QLineEdit::Normal,
                                             "", &ok);
        if (ok && !text.isEmpty())
        {
            QMessageBox::information(this, tr("Validation results"),
                                              myDfa.validate(text));
        }
    }
    else
    {
        QMessageBox::warning(this, tr("DFA Error"),
                                      info);
    }
}

void MainWindow::buildTable()
{
    QString info = myDfa.isDFAValid();
    if(info == "OK")
    {
        tableWindow = new DfaTable(&myDfa, this);
        tableWindow->setMinimumSize(480, 320);
        tableWindow->setWindowFlag(Qt::Window);
        tableWindow->show();
    }
    else
    {
        QMessageBox::warning(this, tr("DFA Error"),
                                      info);
    }
}

void MainWindow::buildCode()
{
    QString info = myDfa.isDFAValid();
    if(info == "OK")
    {
        codeEditor = new CodeEditor(this);
        codeEditor->setMinimumSize(640, 320);
        codeEditor->setWindowFlag(Qt::Window);
        codeEditor->setPlainText(myDfa.buildCode());
        codeEditor->show();
    }
    else
    {
        QMessageBox::warning(this, tr("DFA Error"),
                                      info);
    }
}

void MainWindow::manual()
{
    QFile file("/home/rudakov/pg/course_work/PCADDFA/manual.html");

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
        return;
    }

    QTextStream in(&file);

    QTextBrowser *tb = new QTextBrowser(this);
    tb->setReadOnly(true);
    tb->setWindowFlag(Qt::Window);
    tb->setWindowTitle("User manual");
    tb->setOpenExternalLinks(true);
    tb->setHtml(in.readAll());
    tb->setMinimumSize(720, 480);
    tb->show();
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
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Close | QMessageBox::Cancel);
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

bool MainWindow::saveSvgFile(const QString &fileName)
{
    QSvgGenerator generator;
    generator.setFileName(fileName);
    generator.setSize(QSize(scene->width(), scene->height()));
    generator.setViewBox(QRect(0, 0, scene->width(), scene->height()));
    generator.setTitle(tr("SVG DFA"));
    generator.setDescription(tr("File created by SVG generator"));

    QPainter painter;
    painter.begin(&generator);
    scene->render(&painter);
    painter.end();
    QMessageBox::information(this, tr("Application"), "File saved");
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
        event->accept();
    } else {
        event->ignore();
    }
}

MainWindow::~MainWindow()
{
}

