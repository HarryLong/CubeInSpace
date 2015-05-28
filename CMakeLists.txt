cmake_minimum_required(VERSION 2.8.7)
project(Masters)
include(FindPkgConfig)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(GLU REQUIRED)

# Includes
include_directories(
    ${PROJECT_SOURCE_DIR}
    "/usr/include/GL/"
)
# Libraries
link_directories(
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/utils
)

SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g -DGL_DEBUG -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -DGL_DEBUG -std=c++11")
SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_VERBOSE_MAKEFILE true )
SET(SOURCE_FILES asset time_controller_dialog lighting_manager light_properties shape_factory terrain orientation_compass glwidget
window main glheader qtutils view_manager renderer shader_program debug grid helper gl_drawable constants settings rays geom scene_manager
glm_rotations glm_utils)

# Shaders
add_subdirectory(shaders)
set(SHADER_RESOURCES ${CMAKE_SOURCE_DIR}/shaders/shaders.qrc)
qt5_add_resources(RESOURCES ${SHADER_RESOURCES})

add_subdirectory(terragen)

add_executable(CubeInSpace ${SOURCE_FILES} ${RESOURCES})
# Use the Widgets module from Qt 5.
target_link_libraries(CubeInSpace "/usr/lib/x86_64-linux-gnu/libGLEW.so" "/usr/lib/x86_64-linux-gnu/libGL.so" Qt5::Widgets Qt5::OpenGL Qt5::Core Qt5::Gui GLU TerragenFileManager)
