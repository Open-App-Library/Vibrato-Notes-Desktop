QT += core gui widgets sql

TARGET = vibrato
TEMPLATE = app

LIBS += -lsodium
LIBS += -lchicken

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

VIBRATO_ENTRY_POINT=$$PWD/src/main.cpp

SOURCES += $$PWD/src/main.cpp \
           $$PWD/src/mainwindow.cpp \
           $$PWD/src/components/componentmanager.cpp \
           $$PWD/src/meta/note.cpp \
           $$PWD/src/sql/sqlmanager.cpp


HEADERS += $$PWD/src/mainwindow.h \
           $$PWD/src/components/componentmanager.h \
           $$PWD/src/meta/note.h \
           $$PWD/src/sql/sqlmanager.h

INCLUDEPATH += $$PWD/src/helpers

FORMS += $$PWD/mainwindow.ui

include($$PWD/src/components/main-components/text-editor/escriba/Escriba.pro)
include($$PWD/src/cloud/api/Qt-Vibrato-Cloud-API-Library.pro)


RESOURCES += \
    $$PWD/resources/dummy-data.qrc \
    $$PWD/resources/icons.qrc \
    $$PWD/resources/fonts.qrc \
    $$PWD/resources/qdarkstyle.qrc \
    $$PWD/resources/sqlfiles.qrc \
    $$PWD/resources/tutorial.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
