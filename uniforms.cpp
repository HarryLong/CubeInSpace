#include "uniforms.h"

// TRANSFORMS
const char * Uniforms::Transform::_PROJECTION = "transform.projMat";
const char * Uniforms::Transform::_VIEW = "transform.viewMat";
const char * Uniforms::Transform::_MODEL  = "transform.mtwMat";
const char * Uniforms::Transform::_SCALE = "transform.scale";

// TERRAIN
const char * Uniforms::Terrain::_MAX_HEIGHT = "max_height";
const char * Uniforms::Terrain::_BASE_HEIGHT = "base_height";
const char * Uniforms::Terrain::_MATERIAL_AMBIENT = "terrain_material_ambient";
const char * Uniforms::Terrain::_MATERIAL_DIFFUSE = "terrain_material_diffuse";

// WATER
const char * Uniforms::Water::_MATERIAL_AMBIENT = "water_material_ambient";
const char * Uniforms::Water::_MATERIAL_DIFFUSE = "water_material_diffuse";

// TEXTURE
const char * Uniforms::Texture::_TERRAIN_HEIGHTMAP = "terrain_height_map_texture";
const char * Uniforms::Texture::_WATER_HEIGHTMAP = "water_height_map_texture";
const char * Uniforms::Texture::_NORMALS = "normals_texture";
const char * Uniforms::Texture::_SHADE = "shade_texture";
const char * Uniforms::Texture::_TEMPERATURE_JUN = "jun_temperature_texture";
const char * Uniforms::Texture::_TEMPERATURE_DEC = "dec_temperature_texture";
const char * Uniforms::Texture::_MIN_DAILY_ILLUMINATION = "min_daily_illumination_texture";
const char * Uniforms::Texture::_MAX_DAILY_ILLUMINATION = "max_daily_illumination_texture";

// OVERLAY
const char * Uniforms::Overlay::_NONE = "overlay.none";
const char * Uniforms::Overlay::_SLOPE = "overlay.slope";
const char * Uniforms::Overlay::_ALTITUDE = "overlay.altitude";
const char * Uniforms::Overlay::_SHADE = "overlay.shade";
const char * Uniforms::Overlay::_TEMPERATURE = "overlay.temperature";
const char * Uniforms::Overlay::_MIN_DAILY_ILLUMINATION = "overlay.daily_illumination_min";
const char * Uniforms::Overlay::_MAX_DAILY_ILLUMINATION = "overlay.daily_illumination_max";

// LIGHTING
const char * Uniforms::Lighting::_POSITION = "light_position";
const char * Uniforms::Lighting::_DIFFUSE_COLOR = "light_diffuse_color";
const char * Uniforms::Lighting::_AMBIENT_COLOR = "light_ambient_color";

// ASSETS
const char * Uniforms::Assets::_UNIFORM_COLOR = "uniform_color";
const char * Uniforms::Assets::_USE_UNIFORM_COLOR = "use_uniform_color";

// TIMING
const char * Uniforms::Timing::_MONTH= "month";
