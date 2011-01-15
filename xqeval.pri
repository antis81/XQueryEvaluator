!isEmpty(XQEVAL_PRI_INCLUDED):error("ignored: xqeval.pri already included")
XQEVAL_PRI_INCLUDED = 1

XQEVAL_VERSION = 0.5.3
message("Building $$TARGET")

PROJECT_BASE = $$PWD

BIN_BASE = $$PWD/build
message("Binaries going to $$BIN_BASE")

message("Meta files going to $$PWD/GeneratedFiles")
MOC_DIR = $$PWD/GeneratedFiles/moc
UI_DIR  = $$PWD/GeneratedFiles/ui
RCC_DIR = $$PWD/GeneratedFiles/rcc
OBJECTS_DIR = $$PWD/GeneratedFiles/obj

INCLUDEPATH += $$PWD

RESOURCES *= \
    $$PWD/resources/xqe_resource.qrc
