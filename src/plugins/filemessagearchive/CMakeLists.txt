project(filemessagearchive)

set(QT_USE_QTSQL YES) 

set(PLUGIN_NAME "filemessagearchive")
set(PLUGIN_DISPLAY_NAME "File Message Archive")
set(PLUGIN_DEPENDENCIES messagearchiver) # used only in CPack

include("filemessagearchive.cmake")
include("${CMAKE_SOURCE_DIR}/src/plugins/plugins.cmake")

if (IS_ENABLED)
	find_package(Qt5Sql REQUIRED)
	target_link_libraries(${PLUGIN_NAME} Qt5::Sql)
endif (IS_ENABLED)
