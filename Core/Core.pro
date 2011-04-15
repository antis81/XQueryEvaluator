include(../xqeval.pri)

TEMPLATE = app

TARGET = XQueryEvaluator
DESTDIR = $$BIN_BASE

VERSION = $$XQEVAL_VERSION
DEFINES += \
    APP_VERSION=\\\"$$XQEVAL_VERSION\\\" \
    APP_NAME=\\\"$$TARGET\\\"

CONFIG += qt

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

OTHER_FILES = xqe.rc

win32 {
    RC_FILE = xqe.rc
    #INSTALLS += target
} else:macx {
    ICON = resources/logo.icns
    QMAKE_INFO_PLIST = '''resources/InfoTemplate.plist'''
        QMAKE_BUNDLE_DATA += FILETYPES
}
