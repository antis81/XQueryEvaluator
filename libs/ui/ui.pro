include(../../xqeval.pri)

TARGET = ui
DESTDIR = $$BIN_BASE
TEMPLATE = lib
CONFIG += staticlib

HEADERS += AbstractWidgetGenerator.h \
    DockWidgets.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

SOURCES += \
    DockWidgets.cpp
