QT += testlib

include(../VibratoNotes-Desktop.pro)

TARGET = UnitTests
TEMPLATE = app
DEFINES += UNIT_TEST

# Remove the app's entry file
SOURCES -= $$VIBRATO_ENTRY_POINT

# Sources
SOURCES += $$PWD/unit-tests.cpp

RESOURCES += \
    resources/testresources.qrc
