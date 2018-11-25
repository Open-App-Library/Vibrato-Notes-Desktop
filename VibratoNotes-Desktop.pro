#-------------------------------------------------
#
# Project created by QtCreator 2018-09-19T15:41:41
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = VibratoNotesDesktop
TEMPLATE = app

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
    $$PWD/src/main.cpp \
    $$PWD/src/meta/info/appconfig.cpp \
    $$PWD/src/mainwindow.cpp \
    $$PWD/src/userwindow.cpp \
    $$PWD/src/ui-managers/notelistmanager.cpp \
    $$PWD/src/meta/note.cpp \
    $$PWD/src/meta/notebook.cpp \
    $$PWD/src/meta/tag.cpp \
    $$PWD/src/meta/db/notedatabase.cpp \
    $$PWD/src/meta/db/notebookdatabase.cpp \
    $$PWD/src/meta/db/tagdatabase.cpp \
    $$PWD/src/models/treemodel.cpp \
    $$PWD/src/models/items/basictreeitem.cpp \
    $$PWD/src/ui-managers/treemanager.cpp \
    $$PWD/src/ui-managers/escribamanager.cpp \
    $$PWD/src/meta/filter/notefilter.cpp \
    $$PWD/src/meta/db/database.cpp \
    $$PWD/src/models/items/notelistitem.cpp \
    $$PWD/src/models/notelistmodel.cpp \
    $$PWD/src/models/views/customlistview.cpp \
    $$PWD/ui/note_editnotebook.cpp \
    $$PWD/src/models/items/treeitemwithid.cpp \
    $$PWD/src/iconutils.cpp \
    ui/note_edittags.cpp \
    src/models/items/listitemwithid.cpp

HEADERS += \
    $$PWD/src/meta/info/appconfig.h \
    $$PWD/src/meta/info/appinfo.h \
    $$PWD/src/mainwindow.h \
    $$PWD/src/userwindow.h \
    $$PWD/src/ui-managers/notelistmanager.h \
    $$PWD/src/meta/note.h \
    $$PWD/src/meta/notebook.h \
    $$PWD/src/meta/tag.h \
    $$PWD/src/meta/db/notedatabase.h \
    $$PWD/src/meta/db/notebookdatabase.h \
    $$PWD/src/meta/db/tagdatabase.h \
    $$PWD/include/helper-io.hpp \
    $$PWD/src/models/treemodel.h \
    $$PWD/src/models/items/basictreeitem.h \
    $$PWD/src/ui-managers/treemanager.h \
    $$PWD/src/ui-managers/escribamanager.h \
    $$PWD/src/meta/filter/notefilter.h \
    $$PWD/src/meta/db/database.h \
    $$PWD/src/models/items/notelistitem.h \
    $$PWD/src/models/notelistmodel.h \
    $$PWD/src/models/views/customlistview.h \
    $$PWD/ui/note_editnotebook.h \
    $$PWD/src/models/items/treeitemwithid.h \
    $$PWD/src/iconutils.h \
    $$PWD/include/helper-io.hpp \
    ui/note_edittags.h \
    src/models/items/listitemwithid.h

INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/src/models/views # Location of customlistview

FORMS += \
    $$PWD/ui/mainwindow.ui \
    $$PWD/ui/userwindow.ui \
    $$PWD/ui/notelistitem.ui \
    $$PWD/ui/escribaaddons.ui \
    $$PWD/ui/tagitem.ui \
    $$PWD/ui/note_editnotebook.ui \
    ui/note_edittags.ui

include($$PWD/src/text-editor/Escriba.pro)

RESOURCES += \
    $$PWD/resources/dummy-data.qrc \
    $$PWD/resources/icons.qrc \
    $$PWD/resources/fonts.qrc \
    $$PWD/resources/styles.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
