message ("Building $$TARGET with Qt Version $$[QT_VERSION]")

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = \
    #3rdparty \
    ui \
    Query \
    TextEditing
