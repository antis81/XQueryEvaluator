include(../xqeval.pri)

TEMPLATE = app

TARGET = XQueryEvaluator

unix:VERSION = $$XQEVAL_VERSION
DEFINES += \
    APP_VERSION=\\\"$$XQEVAL_VERSION\\\" \
    APP_NAME=\\\"$$TARGET\\\"

macx:ICON = resources/logo.icns

DESTDIR = $$BIN_BASE

LIBS += -L$$BIN_BASE \
    -lTextEditing \
    -lQuery

QT += xmlpatterns

FORMS    += XQEMainWindow.ui \
    XQEOutput.ui \
    XmlEditDialog.ui

HEADERS  += \
    XQEMainWindow.h \
    XQEOutput.h \
    XmlEditDialog.h

SOURCES += main.cpp\
    XQEMainWindow.cpp \
    XQEOutput.cpp \
    XmlEditDialog.cpp

RESOURCES += \
    resources/xqe_resource.qrc
