GENERATED_SOURCES += \
    entity_wrap.cxx

SWIG_FILES += \
    entity.i

SWIG_USE_FILE += \
    -I/usr/include/aarch64-linux-gnu/qt6 \
    -I/usr/include/aarch64-linux-gnu/qt6/QtGui \
    -I/usr/include/aarch64-linux-gnu/qt6/QtCore \
    -I/usr/include/aarch64-linux-gnu/qt6/QtWidgets

swig.input = SWIG_FILES
swig.variable_out = GENERATED_SOURCES
swig.commands = /usr/bin/swig -c++ -python -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
#swig.commands = /usr/bin/swig -c++ -v $$SWIG_USE_FILE -DQT_CORE_LIB -DQT_GUI_LIB -DQT_WIDGETS_LIB -DQT_VERSION_MAJOR=6 -python -o $${INSTALLDIR} ${QMAKE_FILE_NAME}

swig.output = entity_wrap.cxx
QMAKE_EXTRA_COMPILERS += swig
