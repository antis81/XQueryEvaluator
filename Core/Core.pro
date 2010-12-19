include(../xqeval.pri)

TEMPLATE = app

TARGET = XQueryEvaluator
VERSION = $$XQEVAL_VERSION
message("XQE Version: $$VERSION")

DESTDIR = $$BIN_BASE

unix:LIBS += -L$$BIN_BASE \
    -lTextEditing \
    -lQuery

QT *= xmlpatterns

FORMS    += XQEMainWindow.ui \
    XQEOutput.ui

HEADERS  += \
    XQEMainWindow.h \
    XQEOutput.h

SOURCES += main.cpp\
    XQEMainWindow.cpp \
    XQEOutput.cpp
