include(../xqeval.pri)

TEMPLATE = lib
CONFIG += staticlib

TARGET = TextEditing
DESTDIR = $$BIN_BASE

INCLUDEPATH += $$PWD

HEADERS += \
    XQueryHighlighter.h \
    XQEditor.h \
    XQEdit.h \
    XmlHighlighter.h \
    TextEditMetaBorder.h

SOURCES += \
    XQueryHighlighter.cpp \
    XQEditor.cpp \
    XQEdit.cpp \
    XmlHighlighter.cpp \
    TextEditMetaBorder.cpp
