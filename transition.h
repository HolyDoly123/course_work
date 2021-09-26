#ifndef TRANSITION_H
#define TRANSITION_H
#include <QtCore>

QT_FORWARD_DECLARE_CLASS(State);

class Transition
{
    Q_OBJECT
private:
    State *_source;
    State *_destination;
    QString _signal;
    QString _description;
    QTextStream _output;

public:
    Transition() = delete;
    Transition(QString signal = "", State *source = nullptr, State *destination = nullptr,
               QString description = "", QTextStream output = QTextStream());
    Transition(const State& other); // II. copy constructor
    Transition& operator=(const State& other); // III. copy assignment
    ~Transition();

    void setSource(State *source);
    void setDestination(State *destination);
    void setSignal(QString signal);
    void setDescription(QString description);
    void setOutput(QTextStream output);

    State *getSource() const;
    State *getDestination() const;
    QString getSignal() const;
    QString getDescription() const;
    QTextStream getOutput() const;

    void clear();
};

#endif // TRANSITION_H
