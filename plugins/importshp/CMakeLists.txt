set(PLUGIN_NAME importshp)

qt_add_plugin(
    ${PLUGIN_NAME}
    SHARED
    importshp.cpp
    importshp.h
    shapelib/shpopen.c
    shapelib/dbfopen.c
    shapelib/safileio.c
)

set_target_properties(
    ${PLUGIN_NAME}
    PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
)

target_include_directories(${PLUGIN_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/shapelib
    ../../librecad/src/plugins
)

target_compile_definitions(${PLUGIN_NAME} PRIVATE
    MUPARSER_STATIC
)

target_link_libraries(
    ${PLUGIN_NAME}
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION bin/resources/plugins)
