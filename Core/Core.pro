include(../xqeval.pri)

TEMPLATE = app

TARGET = XQueryEvaluator

VERSION = $$XQEVAL_VERSION
message("XQE Version: $$VERSION")

macx:ICON = resources/logo.icns

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

RESOURCES += \
    resources/xqe_resource.qrc
