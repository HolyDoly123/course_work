#ifndef DFATABLE_H
#define DFATABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QPlainTextEdit>

#include "dfa.h"

class DfaTable : public QWidget
{
    Q_OBJECT
public:
    DfaTable(DFA *dfa, QWidget *parent = nullptr);

private slots:
    void saveTable();

private:
    void fillTable();
    void minimizeTable();

    QPlainTextEdit *minimization_information;
    DFA *myDfa;
    QTableWidget *myTable;
};

#endif // DFATABLE_H
