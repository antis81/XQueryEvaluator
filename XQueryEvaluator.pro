message ("Building $$TARGET with Qt Version $$[QT_VERSION]")

contains(QT_VERSION, ^4\\.[0-9][0-9]?\\..*) {
    message("Cannot build $$TARGET with Qt version $${QT_VERSION}.")
    error("Requires Qt 5.0 or later.")
}

TEMPLATE = subdirs
CONFIG += ordered

QT += widgets

SUBDIRS = \
    libs \
    Core

TRANSLATIONS += \
    translations/xqeval_cs.ts \
    translations/xqeval_de.ts
