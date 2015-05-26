cmake_minimum_required(VERSION 2.8.7)
project(CUBE_IN_SPACE)
include(FindPkgConfig)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "/opt/Qt5.2.1/5.2.1/gcc_64/")

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(MY_SRCS statuswidget.ui)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(GLU REQUIRED)

pkg_check_modules(ImageMagick REQUIRED Magick++-6.Q16)
pkg_check_modules(OpenEXR REQUIRED OpenEXR)

### Set include and link directories and definitions
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${OpenEXR_INCLUDE_DIRS}
    ${ImageMagick_INCLUDE_DIRS}
    "/usr/include/GL/"
)
link_directories(
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/utils
    ${OpenEXR_LIBRARY_DIRS}
    ${ImageMagick_LIBRARY_DIRS}
)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -std=c++11 ")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g -DGL_DEBUG -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -DGL_DEBUG -std=c++11")

SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_VERBOSE_MAKEFILE true )

# The utils library
add_subdirectory(utils)
add_subdirectory(shaders)
#add_subdirectory(common)
add_subdirectory(terragen)

add_executable(CubeInSpace asset time_controller_dialog lighting_manager light_properties shape_factory terrain orientation_compass glwidget window main glheader qtutils view_manager renderer shader_program debug grid helper gl_drawable constants settings ray_drawer geom scene_manager)
# Use the Widgets module from Qt 5.
target_link_libraries(CubeInSpace "/usr/lib/x86_64-linux-gnu/libGLEW.so" "/usr/lib/x86_64-linux-gnu/libGL.so" Qt5::Widgets Qt5::OpenGL Qt5::Core Qt5::Gui GLU Utils TerragenFileManager)
