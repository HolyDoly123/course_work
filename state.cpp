#include "state.h"

State::State(QString name, QString description, QString state_variables, bool final)
    : _name(name), _description(description), _state_variables(state_variables),
      _final(final), _state_transitions(QSet<Transition*>())
{}

void State::setName(QString name)
{
    _name = name;
}

void State::setDescription(QString description)
{
    _description = description;
}

void State::setFinal(bool final)
{
    _final = final;
}

void State::setStateVariables(QString state_variables)
{
    _state_variables = state_variables;
}

QString State::getName() const
{
    return _name;
}

QString State::getDescription() const
{
    return _description;
}

bool State::isFinal() const
{
    return _final;
}

QString State::getStateVariables() const
{
    return _state_variables;
}

bool State::addTransition(Transition *transition)
{
    QString signal = transition->getSignal();
    if (std::find_if(_state_transitions.begin(), _state_transitions.end(),
                     [signal](const Transition* value) { return value->getSignal() == signal; }) != _state_transitions.end())
    {
        return false;
    }
    _state_transitions.insert(transition);
    return true;
}

void State::removeTransition(Transition *transition)
{
    _state_transitions.remove(transition);
}

bool operator==(const State& left, const State& right)
{
    return left._name == right._name;
}

void State::clear()
{
    _description = QObject::tr("No description");
    _state_variables = nullptr;
    _final = false;
    _state_transitions.clear();
}
