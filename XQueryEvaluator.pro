message ("Building $$TARGET with Qt Version $$[QT_VERSION]")

contains(QT_VERSION, ^4\\.[0-4]\\..*) {
    message("Cannot build $$TARGET with Qt version $${QT_VERSION}.")
    error("Use at least Qt 4.5.")
}

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = \
    Query \
    TextEditing \
    Core \
    #libs

Core.depends = Query TextEditing

TRANSLATIONS += \
    translations/xqeval_cs.ts \
    translations/xqeval_de.ts
