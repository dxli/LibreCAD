set(PLUGIN_NAME importshp)

qt_add_plugin(
    ${PLUGIN_NAME}
    SHARED
    importshp.cpp
    importshp.h
    # Shapelib now lives under libraries/shapelib/src (Phase 1a) and is
    # compiled as C++17 (.cpp; Phase 1b).  The plugin still lists the sources
    # explicitly here — retired in Phase 5 with the plugin itself.
    ../../libraries/shapelib/src/shpopen.cpp
    ../../libraries/shapelib/src/dbfopen.cpp
    ../../libraries/shapelib/src/safileio.cpp
)

set_target_properties(
    ${PLUGIN_NAME}
    PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
)

target_include_directories(${PLUGIN_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/shapelib/src
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
