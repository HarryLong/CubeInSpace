cmake_minimum_required(VERSION 2.8.7)
project(Masters)
include(FindPkgConfig)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(GLU REQUIRED)
find_package(OpenMP REQUIRED)

# Includes
include_directories(
    ${PROJECT_SOURCE_DIR}
    "${PROJECT_SOURCE_DIR}/include/"
    "/usr/include/GL/"
)

link_directories(
    ${PROJECT_SOURCE_DIR}
    "lib/"
)

SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${OpenMP_CXX_FLAGS} -O2 -g -DGL_DEBUG -std=c++11")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OpenMP_CXX_FLAGS} -O2 -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${OpenMP_CXX_FLAGS} -O0 -g -DGL_DEBUG -std=c++11")
SET(CMAKE_BUILD_TYPE Debug)

SET(RESOURCES_SOURCE_FILES resources/terrain_shade resources/terrain_daily_illumination resources/terrain_temperature
resources/terrain_water resources/resource_wrapper)
SET(LIGHTING_SOURCE_FILES lighting/lighting_manager lighting/light_properties)
SET(DIALOGS_SOURCE_FILES dialogs/rainfall_edit_dlg dialogs/temp_edit_dlg dialogs/settings_dlg dialogs/pointer_info_dlg dialogs/custom_line_edit dialogs/dialogs)
SET(SHADER_SOURCE_FILES shaders/shader_programs shaders/shaders shaders/uniforms)
SET(TERRAIN_SOURCE_FILES terrain/drawable_terrain terrain/terrain terrain/terrain_normals terrain/acceleration_structure)
SET(WIDGETS_SOURCE_FILES widgets/progress_bar_widget widgets/controller_widgets widgets/orientation_widget)
SET(GL_ASSETS_SOURCE_FILES gl_assets/asset gl_assets/grid gl_assets/rays gl_assets/shape_factory gl_assets/sun_asset)
SET(GL_CORE_SOURCE_FILES gl_core/camera gl_core/gl_drawable gl_core/glheader gl_core/renderer gl_core/transform gl_core/glm_wrapper gl_core/glwidget)
SET(GL_TEXTURE_SOURCE_FILES gl_texture/texture_element gl_texture/counter_texture)

SET(SOURCE_FILES
${RESOURCES_SOURCE_FILES}
${LIGHTING_SOURCE_FILES}
${DIALOGS_SOURCE_FILES}
${SHADER_SOURCE_FILES}
${TERRAIN_SOURCE_FILES}
${WIDGETS_SOURCE_FILES}
${GL_ASSETS_SOURCE_FILES}
${GL_CORE_SOURCE_FILES}
${GL_TEXTURE_SOURCE_FILES}
window main debug geom actions )

# Shaders
add_subdirectory(shaders)
set(SHADER_RESOURCES ${CMAKE_SOURCE_DIR}/shaders/shaders.qrc)
qt5_add_resources(RESOURCES ${SHADER_RESOURCES})

add_executable(CubeInSpace ${SOURCE_FILES} ${RESOURCES})
# Use the Widgets module from Qt 5.
target_link_libraries(CubeInSpace "/usr/lib/x86_64-linux-gnu/libGLEW.so" "/usr/lib/x86_64-linux-gnu/libGL.so" Qt5::Widgets Qt5::OpenGL Qt5::Core Qt5::Gui GLU TerragenFileManager)
