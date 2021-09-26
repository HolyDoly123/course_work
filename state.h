#ifndef STATE_H
#define STATE_H

#include <QtCore>
#include "transition.h"

class State
{
    Q_OBJECT

private:
    bool _initial;
    bool _final;
    QString _name;
    QString _description;
    QTextStream _state_variables;
    QSet<Transition*> _state_transitions;

public:
    static const State *ERROR_STATE;
    static const QString ERROR_STATE_NAME;

    State() = delete;
    State(QString name = tr("US"), QTextStream state_variables = QTextStream(),
          bool initial = false, bool final = false);
    State(const State& other);
    State& operator=(const State& other);
    ~State();

    void setName(QString name);
    void setDescription(QString description);
    void setInitial(bool initial);
    void setFinal(bool final);
    void setStateVariables(QTextStream state_variables);

    QString getName() const;
    QString getDescription() const;
    bool isInitial() const;
    bool isFinal() const;
    QTextStream getStateVariables() const;

    bool addTransition(Transition *transition);
    bool removeTransition(Transition *transition);

    void clear();
};

#endif // STATE_H
