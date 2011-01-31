include(../xqeval.pri)

TEMPLATE = lib
CONFIG += staticlib

TARGET = Query
DESTDIR = $$BIN_BASE

INCLUDEPATH += $$PWD

HEADERS += \
    XQEMessageHandler.h \
    XQEvaluator.h

SOURCES += \
    XQEMessageHandler.cpp \
    XQEvaluator.cpp
