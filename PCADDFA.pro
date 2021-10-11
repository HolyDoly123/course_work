QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    codeeditor.cpp \
    dfa.cpp \
    dfatable.cpp \
    grapharrow.cpp \
    graphscene.cpp \
    graphvertex.cpp \
    main.cpp \
    mainwindow.cpp \
    state.cpp \
    transition.cpp \

HEADERS += \
    codeeditor.h \
    dfa.h \
    dfatable.h \
    grapharrow.h \
    graphscene.h \
    graphvertex.h \
    mainwindow.h \
    state.h \
    transition.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
