#ifndef DFA_H
#define DFA_H

#include "state.h"
#include "transition.h"

class DFA
{
    Q_OBJECT
private:
    QString _name;
    QSet<State*> _states;
    QSet<Transition*> _transitions;

public:
    DFA() = delete;
    DFA(QString name = tr("Untitled"));
    //DFA(QDataStream / SVG);
    DFA(const DFA& other); // II. copy constructor
    DFA& operator=(const DFA& other); // III. copy assignment
    ~DFA();

    void setName(QString name);
    void setTransitions(QSet<Transition *> transitions);
    void setStates(QSet<State *> states);

    QString getName() const;
    QSet<State*> getStates() const;
    QSet<Transition *> getTransitions() const;

    bool validate(QString input) const;
    QVector<QDataStream> buildTransitionTable() const;
    QTextStream buildCode() const;
    void readTransitionTable(const QVector<QDataStream> &transition_table);

    bool addState(State *state);
    bool removeState(State *state);

    bool addTransition(Transition *transition);
    bool removeTransition(Transition *transition);

    bool connect(State *state, Transition *transition, bool source = true);
    void unconnect(State *state, Transition *transition, bool source = true);

    void clear();
};
#endif // DFA_H
