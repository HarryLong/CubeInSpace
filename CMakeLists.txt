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
find_package(OpenMP)

# Includes
include_directories(
    ${PROJECT_SOURCE_DIR}
    "include/terragen_file_manager"
    "/usr/include/GL/"
)

link_directories(
    ${PROJECT_SOURCE_DIR}
    "lib/"
)

SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${OpenMP_CXX_FLAGS} -O2 -g -DGL_DEBUG -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS} -O2 -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OpenMP_CXX_FLAGS} -O0 -g -DGL_DEBUG -std=c++11")
SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_VERBOSE_MAKEFILE true )
SET(SOURCE_FILES asset controllers lighting_manager light_properties shape_factory terrain glwidget
window main glheader qtutils view_manager renderer debug grid helper gl_drawable constants settings rays geom scene_manager
glm_rotations glm_utils custom_line_edit acceleration_structure progress_bar_widget pointer_info_dialog temp_edit_dlg
terrain_normals terrain_shade terrain_temperature terrain_daily_illimination texture_element orientation_widget actions
shader_programs controller_widgets water_flow_analyser)

# Shaders
add_subdirectory(shaders)
set(SHADER_RESOURCES ${CMAKE_SOURCE_DIR}/shaders/shaders.qrc)
qt5_add_resources(RESOURCES ${SHADER_RESOURCES})

add_executable(CubeInSpace ${SOURCE_FILES} ${RESOURCES})
# Use the Widgets module from Qt 5.
target_link_libraries(CubeInSpace "/usr/lib/x86_64-linux-gnu/libGLEW.so" "/usr/lib/x86_64-linux-gnu/libGL.so" Qt5::Widgets Qt5::OpenGL Qt5::Core Qt5::Gui GLU TerragenFileManager)
