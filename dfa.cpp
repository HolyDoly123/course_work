#include "dfa.h"

DFA::DFA(QString name)
    : _name(name), _initial_state(nullptr), _states(QSet<State>())
{
}

void DFA::setName(QString name)
{
    _name = name;
}

void DFA::setInitial(QString state)
{
    _initial_state = getState(state);
}

QString DFA::getName() const
{
    return _name;
}

State* DFA::getState(QString name)
{
    QSet<State>::iterator i = std::find_if(_states.begin(), _states.end(),
                                                 [name](const State value) { return value.getName() == name; });
    if (i != _states.end())
    {
        State* s = const_cast<State*>(&*i);
        return s;
    }
    return nullptr;
}

State* DFA::getInitial() const
{
    return _initial_state;
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

State* DFA::addState(QString name)
{
    if (getState(name) == nullptr)
    {
        _states.insert(State(name));
        return getState(name);
    }
    return nullptr;
}

bool DFA::removeState(QString name)
{
    for(const State& e : _states)
    {
        for(const Transition& t : e.getStateTransitions())
        {
            if(*t.getDestination() == e)
            {
                State *s = getState(e.getName());
                s->removeTransition(t.getSignal());
            }
        }
    }
    return _states.remove(*getState(name));
}

void DFA::clear()
{
    _name = QObject::tr("Untitled");
    _initial_state = nullptr;
    _states.clear();
}
