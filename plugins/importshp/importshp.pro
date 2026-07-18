QT       += gui widgets
TEMPLATE = lib
CONFIG += plugin c++17
VERSION = 1.0.0
TARGET = $$qtLibraryTarget(importshp)

GENERATED_DIR = ../../generated/plugin/importshp
include(../../common.pri)

INCLUDEPATH    += ../../librecad/src/plugins \
                  shapelib

SOURCES += importshp.cpp \
           shapelib/shpopen.c \
           shapelib/safileio.c \
           shapelib/dbfopen.c

HEADERS += importshp.h \
           shapelib/shapefil.h

win32 {
    DESTDIR = ../../windows/resources/plugins
}
unix {
    macx {
        DESTDIR = ../../LibreCAD.app/Contents/Resources/plugins
    }
    else {
        DESTDIR = ../../unix/resources/plugins
    }
}

TRANSLATIONS += ./ts/plugins_shp.ts
