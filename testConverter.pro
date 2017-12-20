QT += testlib
QT -= gui

TARGET = tst_tester
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES +=  tst_tester.cpp \
        ../processing.cpp


DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../processing.h

