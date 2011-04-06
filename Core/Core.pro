include(../xqeval.pri)

TEMPLATE = app

TARGET = XQueryEvaluator

unix:VERSION = $$XQEVAL_VERSION
DEFINES += \
    APP_VERSION=\\\"$$XQEVAL_VERSION\\\" \
    APP_NAME=\\\"$$TARGET\\\"

CONFIG += qt

macx {
    ICON = resources/logo.icns
    QMAKE_INFO_PLIST = '''resources/InfoTemplate.plist'''
}

DESTDIR = $$BIN_BASE

LIBS += -L$$BIN_BASE \
    -lui \
    -lTextEditing \
    -lQuery

QT += xmlpatterns svg

FORMS    += XQEMainWindow.ui \
    XQEOutput.ui

HEADERS  += \
    XQEMainWindow.h \
    XQEOutput.h \
    MainApplication.h \
    XmlSource.h

SOURCES += main.cpp\
    XQEMainWindow.cpp \
    XQEOutput.cpp \
    MainApplication.cpp \
    XmlSource.cpp

RESOURCES += \
    resources/xqe_resource.qrc
