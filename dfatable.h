#ifndef DFATABLE_H
#define DFATABLE_H

#include <QWidget>
#include <QTableWidget>

#include "dfa.h"

class DfaTable : public QWidget
{
    Q_OBJECT
public:
    DfaTable(DFA *dfa, QWidget *parent = nullptr);

private:
    void fillTable();

    DFA *myDfa;
    QTableWidget *myTable;
};

#endif // DFATABLE_H
