#ifndef TRANSITION_H
#define TRANSITION_H
#include <QtCore>

QT_FORWARD_DECLARE_CLASS(State);

class Transition
{
private:
    QString _name;
    QString _signal;
    QString _description;
    State *_source;
    State *_destination;
    QString _output;

public:
    Transition() = delete;
    Transition(QString _name, QString signal = "1", QString description = QObject::tr("No description"),
               State *source = nullptr, State *destination = nullptr, QString output = nullptr);
 //   Transition(const Transition& other); // II. copy constructor
 //   Transition& operator=(const Transition& other); // III. copy assignment
    virtual ~Transition() {};

    void setSource(State *source);
    void setDestination(State *destination);
    void setSignal(QString signal);
    void setDescription(QString description);
    void setOutput(QString output);

    QString getName() const;
    State *getSource() const;
    State *getDestination() const;
    QString getSignal() const;
    QString getDescription() const;
    QString getOutput() const;

    friend bool operator==(const Transition& left, const Transition& right);

    void clear();
};

namespace std
{
    template<> struct hash<Transition>
    {
        std::size_t operator()(Transition const& s) const noexcept
        {
            std::size_t h1 = std::hash<std::string>{}(s.getName().toStdString());
            return h1;
        }
    };
}

#endif // TRANSITION_H
