#include "dfatable.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSaveFile>
#include <QMessageBox>
#include <QMenu>

DfaTable::DfaTable(DFA* dfa, QWidget *parent)
    : QWidget(parent), myDfa(dfa)
{
    myTable = new QTableWidget(this);

    myTable->setMinimumSize(QSize(480, 320));
    QPushButton *minimizeButton = new QPushButton(tr("Minimize"), this);
    minimizeButton->setDisabled(true);

    QPushButton *saveButton = new QPushButton(tr("Save"), this);
    connect(saveButton, &QPushButton::clicked, this, &DfaTable::saveTable);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(minimizeButton);
    hlayout->addWidget(saveButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(hlayout);
    mainLayout->addWidget(myTable);
    setLayout(mainLayout);
    fillTable();
}

void DfaTable::saveTable()
{
    QFileDialog dialog(this, tr("Save table"), "",
                       tr( "Text table (*.csv)"));
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return;
    QString fileName = dialog.selectedFiles().first();
    QSaveFile file(fileName);
    QString errorMessage;
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);

        QString textData;
        int rows = myTable->rowCount();
        int columns = myTable->columnCount();

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {

                    textData += myTable->item(i,j)->text();
                    textData += ", ";
            }
            textData += "\n";
        }

        out << textData;

        if (!file.commit())
        {
            errorMessage = tr("Cannot write file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    } else {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                       .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }

    if (!errorMessage.isEmpty())
    {
        QMessageBox::warning(this, tr("Application"), errorMessage);
    }
    else
    {
        QMessageBox::information(this, tr("Application"), "File saved");
    }
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
