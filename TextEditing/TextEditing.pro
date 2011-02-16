include(../xqeval.pri)

TEMPLATE = lib
CONFIG += staticlib

TARGET = TextEditing
DESTDIR = $$BIN_BASE

VERSION = $$XQEVAL_VERSION

DEPENDPATH += .

HEADERS += \
    XQueryHighlighter.h \
    XQEditor.h \
    XQEdit.h \
    XmlHighlighter.h \
    TextEditMetaBorder.h \
    XmlEditor.h \
    AutoIndent.h

SOURCES += \
    XQueryHighlighter.cpp \
    XQEditor.cpp \
    XQEdit.cpp \
    XmlHighlighter.cpp \
    TextEditMetaBorder.cpp \
    XmlEditor.cpp \
    AutoIndent.cpp

RESOURCES += \
    resources/TextEditing.qrc
