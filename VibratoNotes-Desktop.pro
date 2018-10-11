#-------------------------------------------------
#
# Project created by QtCreator 2018-09-19T15:41:41
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = VibratoNotes-Desktop
TEMPLATE = app

include(src/text-editor/Escriba.pro)

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    src/meta/info/appconfig.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/userwindow.cpp \
    src/ui-managers/notelistmanager.cpp \
    src/components/notelistitem.cpp \
    src/meta/note.cpp \
    src/meta/notebook.cpp \
    src/meta/tag.cpp \
    src/meta/db/notedatabase.cpp \
    src/meta/db/notebookdatabase.cpp \
    src/meta/db/tagdatabase.cpp \
    src/models/treemodel.cpp \
    src/models/items/basictreeitem.cpp \
    src/ui-managers/old-treemanager.cpp \
    src/ui-managers/treemanager.cpp \
    src/meta/filter/notefilter.cpp \
    src/meta/db/database.cpp

HEADERS += \
    src/meta/info/appconfig.h \
    src/meta/info/appinfo.h \
    src/mainwindow.h \
    src/userwindow.h \
    src/ui-managers/notelistmanager.h \
    src/components/notelistitem.h \
    src/meta/note.h \
    src/meta/notebook.h \
    src/meta/tag.h \
    src/meta/db/notedatabase.h \
    src/meta/db/notebookdatabase.h \
    src/meta/db/tagdatabase.h \
    include/helper-io.hpp \
    src/models/treemodel.h \
    src/models/items/basictreeitem.h \
    src/ui-managers/old-treemanager.h \
    src/ui-managers/treemanager.h \
    src/meta/filter/notefilter.h \
    src/meta/db/database.h

INCLUDEPATH += $$PWD/include

FORMS += \
    ui/mainwindow.ui \
    ui/userwindow.ui \
    ui/notelistitem.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Mac-specific things
macx {

}

# Mac & Linux
unix {
    #RESOURCES += # elementary-icons.qrc # Add Elementary icons as fallback icons
}

# Linux / BSD (NOT Mac) specfic things
unix:!macx {

}

# Windows-Specific things
win32 {
    # TODO: Add Colibre icon theme as fallback
}


RESOURCES += \
    resources/dummy-data.qrc \
    resources/icons.qrc
