include(../xqeval.pri)

TEMPLATE = lib
CONFIG += staticlib

TARGET = Query
DESTDIR = $$BIN_BASE

INCLUDEPATH += $$PWD

QT *= xmlpatterns

HEADERS += \
    XQEMessageHandler.h \
    XQEvaluator.h

SOURCES += \
    XQEMessageHandler.cpp \
    XQEvaluator.cpp
