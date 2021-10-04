#ifndef TST_TESTCREATING_H
#define TST_TESTCREATING_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "dfa.h"
#include "transition.h"
#include "state.h"

using namespace testing;

TEST(TestDFA, TestCreating)
{
    DFA fa("as");
    EXPECT_EQ(fa.getName(), "as");

    EXPECT_TRUE(fa.getState("Hello") == nullptr);
    fa.addState("Hello");
    fa.addState("as");
    State* sp = fa.getState("HElll");

    EXPECT_FALSE(sp == fa.getState("Hello"));

    QSet<State> _states;
    QString name = "DS";
    State s(name);
    _states.insert(s);
    EXPECT_TRUE(_states.contains(s));
    s.setName("cx");
    EXPECT_FALSE(_states.contains(s));

    QSet<State>::iterator i = std::find_if(_states.begin(), _states.end(),
                                                 [name](const State value) { return value.getName() == name; });
    EXPECT_TRUE(i != _states.end());
    EXPECT_TRUE((*i).getName() == "DS");

    State* new_s = const_cast<State*>(&*i);
    EXPECT_FALSE(new_s == nullptr);

    sp = fa.addState("Hello");
    EXPECT_TRUE(sp == nullptr);

    Transition* tp = fa.addTransition("0");
    Transition* tp2 = fa.addTransition("0");
}

#endif // TST_TESTCREATING_H
