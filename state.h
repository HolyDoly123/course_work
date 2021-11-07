#ifndef STATE_H
#define STATE_H

#include <QtCore>
#include <algorithm>
#include "transition.h"



class State
{

private:

    QString _name;
    QString _description;
    QString _state_variables;
    bool _final;
    QSet<Transition> _state_transitions;

public:
    static quint32 num;

    State() = delete;
    State(QString name, QString description = QObject::tr("No description"),
          QString state_variables = nullptr, bool final = false);
 //  State(const State& other);
 //   State& operator=(const State& other);
    virtual ~State() {};

    void setName(QString name);
    void setDescription(QString description);
    void setFinal(bool final);
    void setStateVariables(QString state_variables);

    QString getName() const;
    QString getDescription() const;
    bool isFinal() const;
    QString getStateVariables() const;
    const QSet<Transition> &getStateTransitions() const { return _state_transitions;};

    Transition* addTransition(QString signal);
    void removeTransition(QString signal);

    Transition* connectTo(State *state, QString signal);

    Transition* getTransition(QString signal);

    friend bool operator==(const State& left, const State& right);

    void clear();
};

namespace std
{
    template<> struct hash<State>
    {
        std::size_t operator()(State const& s) const noexcept
        {
            std::size_t h1 = std::hash<std::string>{}(s.getName().toStdString());
            return h1;
        }
    };
}

#endif // STATE_H
