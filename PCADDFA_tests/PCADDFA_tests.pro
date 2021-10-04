include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG += qt

HEADERS += \
        tst_testcreating.h \
        ../dfa.h \
        ../state.h \
        ../transition.h \

SOURCES += \
        main.cpp \
        ../dfa.cpp \
        ../state.cpp \
        ../transition.cpp \

INCLUDEPATH += "/home/rudakov/pg/course_work/PCADDFA"
