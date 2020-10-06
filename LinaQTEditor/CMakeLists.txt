cmake_minimum_required(VERSION 3.14)

project(LinaQTEditor LANGUAGES CXX)

set(QT_CONFIG_DIR "D:/Softwares/Qt/5.15.1/msvc2019_64/lib/cmake" CACHE FILEPATH "" FORCE)
set(QT_BIN_DIR "D:/Softwares/Qt/5.15.1/msvc2019_64/bin" CACHE FILEPATH "" FORCE)
set (CMAKE_PREFIX_PATH ${QT_CONFIG_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

set(LINAEDITOR_SOURCES
	src/main.cpp
	src/qml.qrc
	src/Levels/DefaultEditorLevel.cpp
)

set(LINAEDITOR_HEADERS
	include/Levels/DefaultEditorLevel.hpp
)

add_executable(${PROJECT_NAME} ${LINAEDITOR_SOURCES} ${LINAEDITOR_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/vendor/glfw/include)


option(LINA_CLIENT_ENABLE_LOGGING "Enables console logging" ON)

if(LINA_CLIENT_ENABLE_LOGGING)
	target_compile_definitions(${PROJECT_NAME} PRIVATE LINA_CLIENT_ENABLE_LOGGING=1)
endif()


target_compile_definitions(LinaQTEditor
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(LinaQTEditor
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick
  PRIVATE Lina::Engine
  PRIVATE ${CMAKE_SOURCE_DIR}/vendor/glfw/lib/${TARGET_ARCHITECTURE}/$<CONFIGURATION>/glfw3.lib
)

#add_custom_command(TARGET ${PROJECT_NAME}
#POST_BUILD
#COMMAND ${CMAKE_COMMAND}
#	-Dsrc_dir="${CMAKE_CURRENT_SOURCE_DIR}/Resources"
#    -Ddst_dir="${CMAKE_BINARY_DIR}/Sandbox/Resources"
#    -P "${CMAKE_SOURCE_DIR}/CMake/UpdateDirectory.cmake"
#)


#Copy PDBs
#add_custom_command(
#TARGET ${PROJECT_NAME}
#PRE_BUILD
#COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/LinaEngine/vendor/glfw/lib/$<CONFIGURATION>/glfw.pdb" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>")

#Copy PDBs
#add_custom_command(
#TARGET ${PROJECT_NAME}
#PRE_BUILD
#COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/LinaEngine/vendor/glad/lib/$<CONFIGURATION>/glad.pdb" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>")

# Copy engine resources to project solution directory to run within IDE.
add_custom_command(
TARGET ${PROJECT_NAME}
POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/LinaEngine/resources" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/resources/")

# Copy project resources to binary directory
add_custom_command(
TARGET ${PROJECT_NAME}
POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/resources" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/resources")

# Copy project resources to project solution directory to run within IDE.
add_custom_command(
TARGET ${PROJECT_NAME}
POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/resources" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/resources/")

add_custom_command(
TARGET ${PROJECT_NAME}
POST_BUILD
COMMAND D:/Softwares/Qt/5.15.1/msvc2019_64/bin/windeployqt.exe --qmldir . ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\\$<CONFIGURATION>\\${PROJECT_NAME}.exe
COMMENT "Deploying QT Modules...")


# "windeployqt.exe --qmldir . ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\\$<CONFIGURATION>\\${PROJECT_NAME}.exe"

#--------------------------------------------------------------------
# Folder structuring in visual studio
#--------------------------------------------------------------------
if(MSVC_IDE)
	foreach(source IN LISTS LINAEDITOR_HEADERS LINAEDITOR_SOURCES)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "${LINAEDITOR_SOURCE_DIR}" "" relative_source_path "${source_path}")
		string(REPLACE "/" "\\" source_path_msvc "${relative_source_path}")
				source_group("${source_path_msvc}" FILES "${source}")
	endforeach()
endif()