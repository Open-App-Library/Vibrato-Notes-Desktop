QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase
DEFINES += "UNIT_TEST"

TEMPLATE = app

SOURCES += \  
    unit-tests.cpp \
    ../src/meta/note.cpp

HEADERS += \
    ../src/meta/note.h
