QT += core gui widgets sql

TARGET = vibrato
TEMPLATE = app

LIBS += -lsodium
LIBS += -lchicken

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

VIBRATO_ENTRY_POINT=$$PWD/src/main.cpp

SOURCES += $$PWD/main.cpp \
           $$PWD/mainwindow.cpp

HEADERS += $$PWD/mainwindow.cpp

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/src/models/views # Location of customlistview

FORMS += $$PWD/mainwindow.ui

include($$PWD/src/text-editor/Escriba.pro)
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
