#ifndef DFA_H
#define DFA_H

#include "state.h"
#include "transition.h"

class DFA
{

private:
    QString _name;
    State *_initial_state;
    QSet<State> _states;
   // QSet<Transition> _transitions;

    bool isDFAValid();

public:
    static const State ERROR_STATE;

    DFA() = delete;
    DFA(QString name = QObject::tr("Untitled"));
    //DFA(QDataStream / SVG);
    //DFA(const DFA& other); // II. copy constructor
    //DFA& operator=(const DFA& other); // III. copy assignment
    virtual ~DFA() {};


    void setName(QString name);
    void setInitial(QString state);

    QString getName() const;
    State* getInitial() const;

    bool validate(QString input) const;
    QVector<QVector<QString>> buildTransitionTable() const;
    //QVector<QDataStream> minimizeTransitionTable();
    QTextStream buildCode() const;
    void readTransitionTable(const QVector<QVector<QString>> &transition_table);

    State* addState(QString name);
    bool removeState(QString name);

    State* getState(QString name);
/*
    Transition* getTransition(QString name);

    Transition* addTransition(QString name);
    bool removeTransition(QString name);

    bool connectStateTransition(QString state_name, QString signal_name, bool source = true);
    void unconnectStateTransition(QString state_name, QString signal_name, bool source = true);
*/
    void clear();
};
#endif // DFA_H
