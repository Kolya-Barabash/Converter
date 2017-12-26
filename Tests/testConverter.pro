QT += testlib sql widgets
QT -= gui

TARGET = tst_tester
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES +=  tst_tester.cpp \
        ../processing.cpp \
    ../contractor.cpp \
    ../convertclass.cpp \
    ../tablemodel.cpp


#DEFINES += SRCDIR=\"$$PWD../\"

HEADERS += \
    ../processing.h \
    ../contractor.h \
    ../convertclass.h \
    ../tablemodel.h

DISTFILES += \
    input.csv

