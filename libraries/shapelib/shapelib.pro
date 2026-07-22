#-------------------------------------------------
#
# shapelib 1.6.3 (vendored, compiled as C++17)
#
# Backs the native ESRI Shapefile import filter (RS_FilterSHP).
# See libraries/shapelib/README.librecad for vendoring notes and the
# resync procedure.
#
#-------------------------------------------------

QT       -= core gui
TEMPLATE = lib

CONFIG += static warn_on

DESTDIR = ../../generated/lib

VERSION = 1.6.3

DLL_NAME = shapelib
TARGET = $$DLL_NAME

GENERATED_DIR = ../../generated/lib/shapelib
# Use common project definitions.
include(../../common.pri)

INCLUDEPATH += src

SOURCES += \
    src/shpopen.cpp \
    src/dbfopen.cpp \
    src/safileio.cpp

HEADERS += \
    src/shapefil.h \
    src/shapefil_private.h
