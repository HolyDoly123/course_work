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
        if(flag) continue;
        return QObject::tr("Invalid: no ") + s + QObject::tr(" transition in ") + current_state->getName();
    }
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
    row.push_back("Name / Signal-Output");
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

void DFA::readTransitionTable(const QVector<QVector<QString>> &transition_table)
{
}

QList<QSet<State>> DFA::getPartitions()
{
    QSet<State> states = _states;
    QList<QSet<State>> partitions;
    QSet<State> final_states;
    QSet<QString> transition_signals;
    for(const auto &s: states)
    {
        if(s.isFinal())
        {
            final_states.insert(s);
        }
        for(const auto& t: s.getStateTransitions())
        {
            transition_signals.insert(t.getSignal());
        }
    }
    partitions.push_back(final_states);
    partitions.push_back(states - final_states);

    QList<QPair<QSet<State>, QString>> S;
    for(auto c : transition_signals)
    {
        S.push_back(QPair<QSet<State>, QString>(final_states, c));
        S.push_back(QPair<QSet<State>, QString>(states - final_states, c));
    }

    QSet<State> R1, R2;
    while(!S.empty())
    {
        R1.clear();
        R2.clear();
        QString splitter = S.first().second;
        QSet<State> splitter_set = S.first().first;
        S.pop_front();
        for(QSet<State> partition : partitions)
        {
            if(partition.size() == 1) continue;
            for(auto state : partition)
            {
                if(!state.getTransition(splitter))
                {
                    continue;
                }
                if(splitter_set.contains(*(state.getTransition(splitter)->getDestination())))
                {
                    R1.insert(state);
                }
            }
            R2 = partition - R1;
            if(!R1.empty() && !R2.empty())
            {
                partitions.replace(partitions.indexOf(partition), R1);
                partitions.push_back(R2);
                for(auto c : transition_signals)
                {
                    S.push_back(QPair<QSet<State>, QString>(R1, c));
                    S.push_back(QPair<QSet<State>, QString>(R2, c));
                }
                break;
            }
        }
    }
    QMutableListIterator<QSet<State>> i(partitions);
    while (i.hasNext()) {
        if (i.next().empty())
            i.remove();
    }
    return partitions;
}

QSet<State> DFA::getReachableStates()
{
    QSet<State> visited;
    QSet<State> to_visit;

    to_visit.insert(*getInitial());

    while(!to_visit.empty())
    {
        State current_state = *to_visit.begin();
        if(visited.contains(current_state))
        {
            to_visit.remove(current_state);
            continue;
        }
        for(auto t: current_state.getStateTransitions())
        {
            to_visit.insert(*t.getDestination());
        }
        visited.insert(current_state);
        to_visit.remove(current_state);
    }
    return visited;
}

QString DFA::getDestinationPartition(State destination, QList<QSet<State>> partitions)
{
    QString result;
    QTextStream partition_stream(&result);
    partition_stream << "{";
    for(const auto& p: partitions)
    {
        if(p.contains(destination))
        {
            for(const auto& s: p)
            {
                partition_stream << s.getName() << ", ";
            }
            break;
        }
    }
    result.chop(2);
    partition_stream << "}";
    return result;
}

QVector<QVector<QString>> DFA::minimizeTransitionTable()
{
    QList<QSet<State>> partitions = getPartitions();
    QSet<State> reachable_states = getReachableStates();

    QSet<Transition> header;
    for(const auto& s: reachable_states)
    {
        for(const auto& t : s.getStateTransitions())
        {
            header.insert(t);
        }
    }

    QVector<QVector<QString>> table;
    QVector<QString> row;
    row.push_back("Name / Signal");
    for (const auto& h : header)
    {
        row.push_back(h.getSignal());
    }
    table.push_back(row);
    row = QVector<QString>(header.count()+1, "Error");
    for(const auto& p: partitions)
    {
        QString name;
        QTextStream name_stream(&name);
        name_stream << "{";
        for(const auto& s: p)
        {
            if(!reachable_states.contains(s))
            {
                continue;
            }
            name_stream << s.getName() << ", ";
            for(const auto& t : s.getStateTransitions())
            {
                row[table[0].indexOf(t.getSignal())] = getDestinationPartition(*t.getDestination(), partitions);
            }
        }
        if(!(name == "{"))
        {
            name.chop(2);
            name_stream << "}";
            row[0] = name;
            table.push_back(row);
            row = QVector<QString>(header.count()+1, "Error");
        }
    }
    return table;
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
    State::num = 1;
    Transition::num = 1;
}

QString DFA::buildCode() const
{
    QString result;
    QTextStream code(&result);

    QString str = getInitial()->getName();

    code << "#include <iostream>\n";
    code << "#include <map>\n";
    code << "#include <set>\n";
    code << "#include <stringstream>\n";
    code << "#include <string>\n\n";

    code << "using namespace std\n\n";

    code << "map<string, map<string, string>> state_transitions;\n\n";
    for(const auto& s: _states)
    {
        str = s.getName();
        for(const auto& t : s.getStateTransitions())
        {
            code << "state_transitions[\"" << s.getName() << "\"][\"" << t.getSignal() << "\"] = \"" << t.getDestination()->getName() << "\";\n";
        }
    }
    code << "\n";

    code << "int main()\n";
    code << "{\n";
    code << "   string init_state = \"" + getInitial()->getName() << "\"\n";
    code << "   set<string> final_states = {";
    for(const auto& s: _states)
    {
        if(s.isFinal())
        {
            code << "\"" << s.getName() << "\", ";
        }
    }
    result.chop(2);
    code << "}\n\n";

    code << "   stringstream input(\"\"); \\\\place signals here with | as delimiter\n\n";
    code << "   string s;\n";
    code << "   string current_state = init_state;\n";
    code << "   while (getline(input, s, ','))\n";
    code << "   {\n";
    code << "       if(state_transitions[current_state].count != 0)\n";
    code << "           if(state_transitions[current_state][s].count != 0)\n";
    code << "               current_state = state_transitions[current_state][s];\n";
    code << "           else\n";
    code << "           {\n";
    code << "               cout << \"No \" << s << \" signal in \" << current_state << endl;\n";
    code << "               return 1;\n";
    code << "           }\n";
    code << "       else\n";
    code << "       {\n";
    code << "           cout << \"No \" << current_state << \" state exist\" << endl;\n";
    code << "           return 1;\n";
    code << "       }\n";
    code << "   }\n\n";

    code << "   if(final_states[current_state].count == 0)\n";
    code << "       cout << current_state << \" inst final state\" << endl;\n";
    code << "   else\n";
    code << "       cout << \"Input is correct!\" << endl;\n\n";
    code << "   return 0;\n";
    code << "}\n";
    return result;
}
