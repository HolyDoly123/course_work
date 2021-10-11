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

QString DFA::isDFAValid()
{
    if(!_initial_state)
    {
        return "No initial state";
    }
    for(const auto& s: _states)
    {
        if(s.getStateTransitions().isEmpty() && !s.isFinal())
            return "Transition " + s.getName() + " isnt final and has no transitions!";
    }
    return "OK";
}

QString DFA::validate(QString input) const
{
    QStringList list = input.split(QLatin1Char('|'), Qt::SkipEmptyParts);
    State *current_state = _initial_state;
    for (auto & s : list)
    {
        bool flag = false;
        for (auto & t : current_state->getStateTransitions())
        {
            if(t.getSignal() == s)
            {
                current_state = t.getDestination();
                flag = true;
                break;
            }
        }
        if(flag) continue;;
        return QObject::tr("Invalid: no ") + s + QObject::tr(" transition in ") + current_state->getName();
    }
    qDebug() << current_state->getName() << current_state->isFinal();
    if (current_state->isFinal())
    {
        return QObject::tr("Valid");
    }
    return QObject::tr("Invalid: state ") + current_state->getName() + QObject::tr(" isnt final!");
}

QVector<QVector<QString>> DFA::buildTransitionTable() const
{
    QSet<Transition> header;
    for(const auto& s: _states)
    {
        for(const auto& t : s.getStateTransitions())
        {
            header.insert(t);
        }
    }

    QVector<QVector<QString>> table;
    QVector<QString> row;
    row.push_back("Name | Signal/(Output)");
    for (const auto& h : header)
    {
        row.push_back(h.getSignal());
    }
    table.push_back(row);
    row = QVector<QString>(header.count()+1, "Error");
    for(const auto& s: _states)
    {
        row[0] = s.getName();
        for(const auto& t : s.getStateTransitions())
        {
            if(t.getOutput().isEmpty())
                row[table[0].indexOf(t.getSignal())] = t.getDestination()->getName();
            else
                row[table[0].indexOf(t.getSignal())] = t.getDestination()->getName() + "/" + t.getOutput();
        }
        table.push_back(row);
        row = QVector<QString>(header.count()+1, "Error");
    }
    return table;
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
        return const_cast<State*>(&*_states.insert(State(name)));
    }
    return nullptr;
}

bool DFA::removeState(QString name)
{
    State *to_delete = getState(name);
    for(const State& e : _states)
    {
        for(const Transition& t : e.getStateTransitions())
        {
            if(t.getDestination() == to_delete)
            {
                State *s = getState(e.getName());
                s->removeTransition(t.getSignal());
            }
        }
    }
    return _states.remove(*to_delete);
}

void DFA::clear()
{
    _name = QObject::tr("Untitled");
    _initial_state = nullptr;
    _states.clear();
}
