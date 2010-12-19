message ("Building with Qt Version $$[QT_VERSION]")

contains(QT_VERSION, ^4\\.[0-4]\\..*) {
    message("Cannot build XQueryEvaluator with Qt version $${QT_VERSION}.")
    error("Use at least Qt 4.5.")
}

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = \
    Query \
    TextEditing \
    Core
