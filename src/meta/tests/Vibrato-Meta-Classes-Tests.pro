QT += core gui widgets sql testlib
TARGET = VibratoMetaTests

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

INCLUDEPATH += $$PWD/../../helpers

SOURCES += $$PWD/main.cpp \
           $$PWD/../tag.cpp \
           $$PWD/../notebook.cpp \
           $$PWD/../note.cpp \
           $$PWD/../vibratoobject.cpp \
           $$PWD/../../sql/sqlmanager.cpp

HEADERS += $$PWD/../tag.h \
           $$PWD/../notebook.h \
           $$PWD/../note.h \
           $$PWD/../vibratoobject.h \
           $$PWD/../../sql/sqlmanager.h
