#include "dfa.h"

DFA::DFA(QString name)
    : _name(name), _initial_state(nullptr), _states(QSet<State>()),
      _transitions(QSet<Transition>())
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

Transition* DFA::getTransition(QString signal)
{
    QSet<Transition>::iterator i = std::find_if(_transitions.begin(), _transitions.end(),
                                                 [signal](const Transition value) { return value.getSignal() == signal; });
    if (i != _transitions.end())
    {
        Transition* s = const_cast<Transition*>(&*i);
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
    return _states.remove(*getState(name));
}

Transition* DFA::addTransition(QString signal)
{
    if (getTransition(signal) == nullptr)
    {
        _transitions.insert(Transition(signal));
        return getTransition(signal);
    }
    return nullptr;
}

bool DFA::removeTransition(QString signal)
{
    _transitions.remove(*getTransition(signal));
}

bool DFA::connectStateTransition(QString state, QString signal, bool source)
{
    Transition *t = getTransition(signal);
    State *s = getState(state);
    if (!s)
    {
        return false;
    }
    if (!source)
    {
        t->setDestination(getState(state));
    }
    else if (s->State::addTransition(t))
    {
        t->setSource(s);
    }
    return false;
}

void DFA::unconnectStateTransition(QString state, QString signal, bool source)
{
    Transition *t = getTransition(signal);
    State *s = getState(state);
    if (!source)
    {
        t->setDestination(nullptr);
    }
    else
    {
        t->setSource(nullptr);
        s->State::removeTransition(t);
    }
}

void DFA::clear()
{
    _name = QObject::tr("Untitled");
    _initial_state = nullptr;
    _states.clear();
    _transitions.clear();
}
