#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "dfa.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void loadFile(const QString &fileName);

    ~MainWindow();
/*
protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU
*/
protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void open();
    bool saveAs();
    void preferences();
    void checkInput();
    void buildTable();
    void buildCode();
    void manual();
    void about();

private:
    void createActions();
    void createMenus();
    void createStatusBar();
    void createDockWindows();

    bool maybeSave();
    void readSettings();
    void writeSettings();
    bool saveFile(const QString &fileName);
    bool isModified();
    void setCurrentFile(const QString &fileName);

    QMenu *fileMenu;
    QMenu *checkMenu;
    QMenu *buildMenu;
    QMenu *helpMenu;

    QStatusBar *statusbar;

    QDockWidget *graphTreeDock;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAsAct;
    QAction *preferencesAct;
    QAction *closeAct;
    QAction *checkInputAct;
    QAction *buildTableAct;
    QAction *buildCodeAct;
    QAction *manualAct;
    QAction *aboutAct;

    QString curFile;
};
#endif // MAINWINDOW_H
