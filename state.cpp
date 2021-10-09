#include "state.h"

State::State(QString name, QString description, QString state_variables, bool final)
    : _name(name), _description(description), _state_variables(state_variables),
      _final(final), _state_transitions(QSet<Transition>())
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

Transition* State::addTransition(QString signal)
{
    if (getTransition(signal) == nullptr)
    {
        _state_transitions.insert(Transition(signal));
         getTransition(signal)->setSource(this);

        return getTransition(signal);
    }
    return nullptr;
}

Transition* State::connectTo(State *state, QString signal)
{
    Transition *t = getTransition(signal);
    t->setDestination(state);
    return t;
}

void State::removeTransition(QString signal)
{
    Transition *t = getTransition(signal);
    _state_transitions.remove(*t);
}

Transition* State::getTransition(QString signal)
{
    QSet<Transition>::iterator i = std::find_if(_state_transitions.begin(), _state_transitions.end(),
                                                 [signal](const Transition value) { return value.getSignal() == signal; });
    if (i != _state_transitions.end())
    {
        Transition* s = const_cast<Transition*>(&*i);
        return s;
    }
    return nullptr;
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
