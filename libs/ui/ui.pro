#-------------------------------------------------
#
# Project created by QtCreator 2011-04-05T14:36:49
#
#-------------------------------------------------

TARGET = ui
TEMPLATE = lib
CONFIG += staticlib

HEADERS += AbstractWidgetGenerator.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
