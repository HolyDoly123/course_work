#include "dfatable.h"

#include <QPushButton>
#include <QVBoxLayout>

DfaTable::DfaTable(DFA* dfa, QWidget *parent)
    : QWidget(parent), myDfa(dfa)
{
    myTable = new QTableWidget(this);

    myTable->setMinimumSize(QSize(480, 320));
    QPushButton *minimizeButton = new QPushButton(tr("Minimize"), this);
    minimizeButton->setDisabled(true);

    QPushButton *saveButton = new QPushButton(tr("Save"), this);
    saveButton->setDisabled(true);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(minimizeButton);
    hlayout->addWidget(saveButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(hlayout);
    mainLayout->addWidget(myTable);
    setLayout(mainLayout);
    fillTable();
}

void DfaTable::fillTable()
{
    QVector<QVector<QString>> data = myDfa->buildTransitionTable();
    myTable->setRowCount(data.size());
    myTable->setColumnCount(data[0].size());

    for (uint i = 0; i < data.size(); i++)
    {
        for (uint j = 0; j < data[0].size(); j++)
        {
            QTableWidgetItem *newItem = new QTableWidgetItem(data[i][j]);
            myTable->setItem(i, j, newItem);
        }
    }
}
