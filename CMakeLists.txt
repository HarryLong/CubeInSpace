cmake_minimum_required(VERSION 2.8)
project(CUBE_IN_SPACE)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(MY_SRCS statuswidget.ui)

#find_library(LIBGL_VAR "GL" "/usr/lib/nvidia-319")

#message(${LIBGL_VAR})

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(GLU REQUIRED)

include_directories("/usr/include/GL/")
#include_directories("include/QtOpenGL" "include/QtCore" "include/QtWidgets" "include/QtGui")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCC_GL_DEBUG_ -Wall -g -std=c++11") #DEBUG
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -std=c++11")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} "-O2 -g")
SET(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "-O2")
SET(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "-O0 -g")

# The utils library
add_subdirectory(utils)
add_subdirectory(shaders)
link_directories(${CUBE_IN_SPACE_BIN_DIR}/utils)

add_executable(CubeInSpace helloGL glwidget window main sceneimpl abstractscene glheader cube globject controlwidget statuswidget qtutils matrixwidget floatingpointslider)
# Use the Widgets module from Qt 5.
target_link_libraries(CubeInSpace "/usr/lib/x86_64-linux-gnu/libGL.so" Qt5::Widgets Qt5::OpenGL Qt5::Core Qt5::Gui GLU Utils)
