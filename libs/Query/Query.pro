include(../../xqeval.pri)

TEMPLATE = lib
CONFIG += staticlib

TARGET = Query
DESTDIR = $$BIN_BASE

VERSION = $$XQEVAL_VERSION

HEADERS += \
    XQEMessageHandler.h \
    XQEvaluator.h

SOURCES += \
    XQEMessageHandler.cpp \
    XQEvaluator.cpp
