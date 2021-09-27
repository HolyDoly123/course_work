#ifndef DFA_H
#define DFA_H

#include "state.h"
#include "transition.h"

class DFA
{

private:
    QString _name;
    State* _initial_state;
    QSet<State*> _states;
    QSet<Transition*> _transitions;

    bool isDFAValid();

public:
    static const State *ERROR_STATE;

    DFA() = delete;
    DFA(QString name = QObject::tr("Untitled"), State *initial_state = nullptr);
    //DFA(QDataStream / SVG);
    //DFA(const DFA& other); // II. copy constructor
    //DFA& operator=(const DFA& other); // III. copy assignment
    virtual ~DFA() {};


    void setName(QString name);
    void setInitial(State* state);

    QString getName() const;
    QSet<State*> getStates() const;
    QSet<Transition *> getTransitions() const;
    State* getInitial() const;
    bool isInitial(State* state) const;

    bool validate(QString input) const;
    QVector<QVector<QString>> buildTransitionTable() const;
    //QVector<QDataStream> minimizeTransitionTable() const;
    QTextStream buildCode() const;
    void readTransitionTable(const QVector<QVector<QString>> &transition_table);

    void addState(State *state);
    void removeState(State *state);

    void addTransition(Transition *transition);
    void removeTransition(Transition *transition);

    bool STConnect(State *state, Transition *transition, bool source = true);
    void unconnect(State *state, Transition *transition, bool source = true);

    void clear();
};
#endif // DFA_H
