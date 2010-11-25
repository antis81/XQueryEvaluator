#-------------------------------------------------
#
# Project created by QtCreator 2010-04-30T22:43:53
#
#-------------------------------------------------

#qt version check

message ("Building with Qt Version $${QT_VERSION}")

contains(QT_VERSION, ^4\\.[0-4]\\..*) {
    message("Cannot build XQueryEvaluator with Qt version $${QT_VERSION}.")
    error("Use at least Qt 4.5.")
}

VERSION = 0.5.1.0

TARGET = XQueryEvaluator
TEMPLATE = app

QT       += xmlpatterns

DEPENDPATH += .

CONFIG += qt

MOC_DIR = GeneratedFiles
UI_DIR  = GeneratedFiles
RCC_DIR = GeneratedFiles
OBJECTS_DIR = obj


include(XQueryEvaluator.pri)

RESOURCES += \
    resources/xqe_resource.qrc
