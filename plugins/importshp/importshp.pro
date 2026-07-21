QT       += gui widgets
TEMPLATE = lib
CONFIG += plugin c++17
VERSION = 1.0.0
TARGET = $$qtLibraryTarget(importshp)

GENERATED_DIR = ../../generated/plugin/importshp
include(../../common.pri)

INCLUDEPATH    += ../../librecad/src/plugins \
                  ../../libraries/shapelib/src

SOURCES += importshp.cpp \
           ../../libraries/shapelib/src/shpopen.cpp \
           ../../libraries/shapelib/src/safileio.cpp \
           ../../libraries/shapelib/src/dbfopen.cpp

HEADERS += importshp.h \
           ../../libraries/shapelib/src/shapefil.h

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
