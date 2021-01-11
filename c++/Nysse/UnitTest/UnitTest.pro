QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_teststatistics.cpp \
    ../Game/core/statistics.cpp

HEADERS += \
    ../Game/core/statistics.hh \
    ../Game/core/statistics.hh


INCLUDEPATH += ../Game
DEPENDPATH += ../Game
