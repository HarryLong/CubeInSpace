#include "uniforms.h"

// TRANSFORMS
const char * Uniforms::Transform::_PROJECTION = "transform.projMat";
const char * Uniforms::Transform::_VIEW = "transform.viewMat";
const char * Uniforms::Transform::_MODEL  = "transform.mtwMat";
const char * Uniforms::Transform::_SCALE = "transform.scale";

// TERRAIN
const char * Uniforms::Terrain::_MIN_HEIGHT = "min_height";
const char * Uniforms::Terrain::_MAX_HEIGHT = "max_height";
const char * Uniforms::Terrain::_BASE_HEIGHT = "base_height";
const char * Uniforms::Terrain::_MATERIAL_AMBIENT = "terrain_material_ambient";
const char * Uniforms::Terrain::_MATERIAL_DIFFUSE = "terrain_material_diffuse";
const char * Uniforms::Terrain::_SCALE = "terrain_scale";

// WATER
const char * Uniforms::Water::_MATERIAL_AMBIENT = "water_material_ambient";
const char * Uniforms::Water::_MATERIAL_DIFFUSE = "water_material_diffuse";
const char * Uniforms::Water::_RENDER = "render_water";

// TEXTURE
const char * Uniforms::Texture::_TERRAIN_HEIGHTMAP = "terrain_height_map_texture";
const char * Uniforms::Texture::_WATER_HEIGHTMAP = "water_heightmap";
const char * Uniforms::Texture::_NORMALS = "normals_texture";
const char * Uniforms::Texture::_SLOPE = "slope_texture";
const char * Uniforms::Texture::_SHADE = "shade_texture";
const char * Uniforms::Texture::_TEMPERATURE = "temperature_texture";
const char * Uniforms::Texture::_DAILY_ILLUMINATION = "illumination_texture";
const char * Uniforms::Texture::_SOIL_INFILTRATION_RATE = "soil_infiltration_rate_texture";
const char * Uniforms::Texture::_CLUSTER_MEMBERSHIP = "cluster_membership_texture";
const char * Uniforms::Texture::_MONTHLY_SOIL_HUMIDITY = "monthly_soil_humidity_texture";
const char * Uniforms::Texture::_WEIGHTED_AVG_SOIL_HUMIDITY = "weighted_avg_soil_humidity_texture";
const char * Uniforms::Texture::_OVERLAY = "overlay_texture";

// OVERLAY
const char * Uniforms::Overlay::_ACTIVE = "overlay_active";
const char * Uniforms::Overlay::_NONE = "overlay.none";
const char * Uniforms::Overlay::_SLOPE = "overlay.slope";
const char * Uniforms::Overlay::_ALTITUDE = "overlay.altitude";
const char * Uniforms::Overlay::_SHADE = "overlay.shade";
const char * Uniforms::Overlay::_TEMPERATURE = "overlay.temperature";
const char * Uniforms::Overlay::_ILLUMINATION = "overlay.illumination";
const char * Uniforms::Overlay::_SOIL_INFILTRATION_RATE = "overlay.soil_infiltration_rate";
const char * Uniforms::Overlay::_MONTHLY_SOIL_HUMIDITY = "overlay.monthly_soil_humidity";
const char * Uniforms::Overlay::_WEIGHTED_AVG_SOIL_HUMIDITY = "overlay.weighted_avg_soil_humidity";
const char * Uniforms::Overlay::_CLUSTERS = "overlay.clusters";

// CLUSTERING
const char * Uniforms::Clustering::_N_CLUSTERS = "n_clusters";
const char * Uniforms::Clustering::_FINAL_CLUSTERING_ITERATION = "final_clustering_iteration";
const char * Uniforms::Clustering::_SLOPE_CLUSTER_DATA = "slope_cluster_data_texture";
const char * Uniforms::Clustering::_TEMPERATURE_CLUSTER_DATA = "temperature_cluster_data_texture";
const char * Uniforms::Clustering::_DAILY_ILLUMINATION_CLUSTER_DATA = "daily_illumination_cluster_data_texture";
const char * Uniforms::Clustering::_WEIGHTED_AVG_SOIL_HUMIDITY_CLUSTER_DATA = "weighted_avg_soil_humidity_cluster_data_texture";

// LIGHTING
const char * Uniforms::Lighting::_POSITION = "light_position";
const char * Uniforms::Lighting::_DIFFUSE_COLOR = "light_diffuse_color";
const char * Uniforms::Lighting::_AMBIENT_COLOR = "light_ambient_color";

// ASSETS
const char * Uniforms::Assets::_UNIFORM_COLOR = "uniform_color";
const char * Uniforms::Assets::_USE_UNIFORM_COLOR = "use_uniform_color";

// TIMING
const char * Uniforms::Timing::_MONTH = "month";

// COUNTERS
const char * Uniforms::Counter::_VERTICIES_EMPTIED = "verticies_emptied_counter";

// RAINFALL
const char * Uniforms::Rainfall::_RAINFALL = "rainfall";
const char * Uniforms::Rainfall::_INTENSITY = "rainfall_intensity";

// SLOPE BASED SOIL INFILTRATION
const char * Uniforms::SlopeBasedSoilInfiltration::_MIN_SLOPE = "min_slope";

// WATER EDIT
const char * Uniforms::WaterEdit::_ABSOLUTE_HEIGHT = "absolute_height";

// BORDER OVERLAP REDUCTION
const char * Uniforms::BorderOverlapReduction::_OVERLAP_MODE = "overlap_mode";

