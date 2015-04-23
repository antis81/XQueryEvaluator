include(../../xqeval.pri)

TEMPLATE = lib
CONFIG += staticlib

TARGET = TextEditing
DESTDIR = $$BIN_BASE

VERSION = $$XQEVAL_VERSION

QT += widgets

DEPENDPATH += .

HEADERS += \
    XQueryHighlighter.h \
    XmlHighlighter.h \
    TextEditMetaBorder.h \
    AutoIndent.h \
    AbstractHighlighter.h \
    CppHighlighter.h \
    TextSearch.h \
    TextEditBase.h \
    XmlEditor.h \
    XQEditor.h

SOURCES += \
    XQueryHighlighter.cpp \
    XmlHighlighter.cpp \
    TextEditMetaBorder.cpp \
    AutoIndent.cpp \
    AbstractHighlighter.cpp \
    CppHighlighter.cpp \
    TextSearch.cpp \
    TextEditBase.cpp \
    XmlEditor.cpp \
    XQEditor.cpp

RESOURCES += \
    resources/TextEditing.qrc

FORMS += \
    TextSearch.ui
