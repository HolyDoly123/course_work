#include "transition.h"

Transition::Transition(QString signal, QString description,
                       State *source, State *destination, QString output)
    : _signal(signal), _description(description), _source(source), _destination(destination), _output(output)
{}
/*
Transition::Transition(const Transition& other)
    :_signal(other._signal), _description(other._description),
      _source(other._source), _destination(other._destination)
{}

Transition& Transition::operator=(const Transition& other)
{
    if (this != &other) // not a self-assignment
    {
        //do copy
    }
    return *this;
}
*/
void Transition::setSource(State *source)
{
    _source = source;
}

void Transition::setDestination(State *destination)
{
    _destination = destination;
}

void Transition::setSignal(QString signal)
{
    _signal = signal;
}

void Transition::setDescription(QString description)
{
    _description = description;
}

void Transition::setOutput(QString output)
{
    _output = output;
}

State* Transition::getSource() const
{
    return _source;
}

State* Transition::getDestination() const
{
    return _destination;
}

QString Transition::getSignal() const
{
    return _signal;
}

QString Transition::getDescription() const
{
    return _description;
}

QString Transition::getOutput() const
{
    return _output;
}

bool operator==(const Transition& left, const Transition& right)
{
    return left._signal == right._signal;
}

void Transition::clear()
{
    _signal = "";
    _description = QObject::tr("No description");
    _source = nullptr;
    _destination = nullptr;
    _output = nullptr;
}
