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

    QString getDestinationPartition(State destination, QList<QSet<State>> partitions);

public:
    static const State ERROR_STATE;

    DFA() = delete;
    DFA(QString name = QObject::tr("Untitled"));
    virtual ~DFA() {};

    QString isDFAValid();

    void setName(QString name);
    void setInitial(QString state);
    void setInitial(State* state) { _initial_state = state; }

    QString getName() const;
    State* getInitial() const;
    QSet<State> getStates() const {return _states;}

    QString validate(QString input) const;
    QVector<QVector<QString>> buildTransitionTable() const;
    QVector<QVector<QString>> minimizeTransitionTable();
    QString buildCode() const;
    void readTransitionTable(const QVector<QVector<QString>> &transition_table);

    QList<QSet<State>> getPartitions();
    QSet<State> getReachableStates();

    State* addState(QString name);
    bool removeState(QString name);

    State* getState(QString name);

    void clear();
};
#endif // DFA_H
