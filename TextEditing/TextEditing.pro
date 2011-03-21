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
    AutoIndent.h \
    AbstractHighlighter.h \
    CppHighlighter.h \
    TextSearch.h

SOURCES += \
    XQueryHighlighter.cpp \
    XQEditor.cpp \
    XQEdit.cpp \
    XmlHighlighter.cpp \
    TextEditMetaBorder.cpp \
    XmlEditor.cpp \
    AutoIndent.cpp \
    AbstractHighlighter.cpp \
    CppHighlighter.cpp \
    TextSearch.cpp

RESOURCES += \
    resources/TextEditing.qrc

FORMS += \
    TextSearch.ui
