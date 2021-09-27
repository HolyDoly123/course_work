#include "dfa.h"

DFA::DFA(QString name, State *initial_state)
    : _name(name), _initial_state(initial_state), _states(QSet<State*>()),
      _transitions(QSet<Transition*>())
{}

void DFA::setName(QString name)
{
    _name = name;
}

void DFA::setInitial(State* state)
{
    _initial_state = state;
}

QString DFA::getName() const
{
    return _name;
}

QSet<State*> DFA::getStates() const
{
    return _states;
}

QSet<Transition *> DFA::getTransitions() const
{
    return _transitions;
}

State* DFA::getInitial() const
{
    return _initial_state;
}

bool DFA::isInitial(State* state) const
{
    if (_initial_state == state)
    {
        return true;
    }
    return false;
}

bool DFA::isDFAValid()
{
    return true;
}

bool DFA::validate(QString input) const
{
    return true;
}

QVector<QVector<QString>> DFA::buildTransitionTable() const
{
    return QVector<QVector<QString>>();
}

QTextStream DFA::buildCode() const
{
    return QTextStream();
}

void DFA::readTransitionTable(const QVector<QVector<QString>> &transition_table)
{
}

void DFA::addState(State *state)
{
    _states.insert(state);
}

void DFA::removeState(State *state)
{
    _states.remove(state);
}

void DFA::addTransition(Transition *transition)
{
    _transitions.insert(transition);
}

void DFA::removeTransition(Transition *transition)
{
    _transitions.remove(transition);
}

bool DFA::STConnect(State *state, Transition *transition, bool source)
{
    if (!source)
    {
        transition->setDestination(state);
    }
    else if (state->State::addTransition(transition))
    {
        transition->setSource(state);
    }
    return false;
}

void DFA::unconnect(State *state, Transition *transition, bool source)
{
    if (!source)
    {
        transition->setDestination(nullptr);
    }
    else
    {
        transition->setSource(nullptr);
        state->State::removeTransition(transition);
    }
}

void DFA::clear()
{
    _name = QObject::tr("Untitled");
    _initial_state = nullptr;
    _states.clear();
    _transitions.clear();
}
