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
    QSet<Transition *> _state_transitions;

public:
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

    bool addTransition(Transition *transition);
    void removeTransition(Transition *transition);

    void clear();
};

#endif // STATE_H
