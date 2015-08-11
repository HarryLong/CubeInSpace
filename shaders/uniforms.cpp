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
const char * Uniforms::Texture::_TEMPERATURE_JUN = "jun_temperature_texture";
const char * Uniforms::Texture::_TEMPERATURE_DEC = "dec_temperature_texture";
const char * Uniforms::Texture::_MIN_DAILY_ILLUMINATION = "min_daily_illumination_texture";
const char * Uniforms::Texture::_MAX_DAILY_ILLUMINATION = "max_daily_illumination_texture";
const char * Uniforms::Texture::_SOIL_INFILTRATION_RATE = "soil_infiltration_rate_texture";
const char * Uniforms::Texture::_CLUSTER_MEMBERSHIP = "cluster_membership_texture";
const char * Uniforms::Texture::_MONTHLY_SOIL_HUMIDITY = "monthly_soil_humidity_texture";
const char * Uniforms::Texture::_WEIGHTED_AVG_SOIL_HUMIDITY = "weighted_avg_soil_humidity_texture";
const char * Uniforms::Texture::_JAN_WEIGHTED_AVG_SOIL_HUMIDITY = "jan_weighted_avg_soil_humidity_texture";
const char * Uniforms::Texture::_FEB_WEIGHTED_AVG_SOIL_HUMIDITY = "feb_weighted_avg_soil_humidity_texture";
const char * Uniforms::Texture::_MAR_WEIGHTED_AVG_SOIL_HUMIDITY = "mar_weighted_avg_soil_humidity_texture";
const char * Uniforms::Texture::_APR_WEIGHTED_AVG_SOIL_HUMIDITY = "apr_weighted_avg_soil_humidity_texture";
const char * Uniforms::Texture::_MAY_WEIGHTED_AVG_SOIL_HUMIDITY = "may_weighted_avg_soil_humidity_texture";
const char * Uniforms::Texture::_JUN_WEIGHTED_AVG_SOIL_HUMIDITY = "jun_weighted_avg_soil_humidity_texture";
const char * Uniforms::Texture::_JUL_WEIGHTED_AVG_SOIL_HUMIDITY = "jul_weighted_avg_soil_humidity_texture";
const char * Uniforms::Texture::_AUG_WEIGHTED_AVG_SOIL_HUMIDITY = "aug_weighted_avg_soil_humidity_texture";
const char * Uniforms::Texture::_SEP_WEIGHTED_AVG_SOIL_HUMIDITY = "sep_weighted_avg_soil_humidity_texture";
const char * Uniforms::Texture::_OCT_WEIGHTED_AVG_SOIL_HUMIDITY = "oct_weighted_avg_soil_humidity_texture";
const char * Uniforms::Texture::_NOV_WEIGHTED_AVG_SOIL_HUMIDITY = "nov_weighted_avg_soil_humidity_texture";
const char * Uniforms::Texture::_DEC_WEIGHTED_AVG_SOIL_HUMIDITY = "dec_weighted_avg_soil_humidity_texture";
const char * Uniforms::Texture::_WEIGHTED_AVG_SOIL_HUMIDITIES[12] = {
    Uniforms::Texture::_JAN_WEIGHTED_AVG_SOIL_HUMIDITY,
    Uniforms::Texture::_FEB_WEIGHTED_AVG_SOIL_HUMIDITY,
    Uniforms::Texture::_MAR_WEIGHTED_AVG_SOIL_HUMIDITY,
    Uniforms::Texture::_APR_WEIGHTED_AVG_SOIL_HUMIDITY,
    Uniforms::Texture::_MAY_WEIGHTED_AVG_SOIL_HUMIDITY,
    Uniforms::Texture::_JUN_WEIGHTED_AVG_SOIL_HUMIDITY,
    Uniforms::Texture::_JUL_WEIGHTED_AVG_SOIL_HUMIDITY,
    Uniforms::Texture::_AUG_WEIGHTED_AVG_SOIL_HUMIDITY,
    Uniforms::Texture::_SEP_WEIGHTED_AVG_SOIL_HUMIDITY,
    Uniforms::Texture::_OCT_WEIGHTED_AVG_SOIL_HUMIDITY,
    Uniforms::Texture::_NOV_WEIGHTED_AVG_SOIL_HUMIDITY,
    Uniforms::Texture::_DEC_WEIGHTED_AVG_SOIL_HUMIDITY,
};

// OVERLAY
const char * Uniforms::Overlay::_ACTIVE = "overlay_active";
const char * Uniforms::Overlay::_TEXTURE = "overlay_texture";
const char * Uniforms::Overlay::_NONE = "overlay.none";
const char * Uniforms::Overlay::_SLOPE = "overlay.slope";
const char * Uniforms::Overlay::_ALTITUDE = "overlay.altitude";
const char * Uniforms::Overlay::_SHADE = "overlay.shade";
const char * Uniforms::Overlay::_TEMPERATURE = "overlay.temperature";
const char * Uniforms::Overlay::_MIN_DAILY_ILLUMINATION = "overlay.daily_illumination_min";
const char * Uniforms::Overlay::_MAX_DAILY_ILLUMINATION = "overlay.daily_illumination_max";
const char * Uniforms::Overlay::_SOIL_INFILTRATION_RATE = "overlay.soil_infiltration_rate";
const char * Uniforms::Overlay::_MONTHLY_SOIL_HUMIDITY = "overlay.monthly_soil_humidity";
const char * Uniforms::Overlay::_WEIGHTED_AVG_SOIL_HUMIDITY = "overlay.weighted_avg_soil_humidity";

// CLUSTERING
const char * Uniforms::Clustering::_N_CLUSTERS = "n_clusters";
const char * Uniforms::Clustering::_JUN_TEMPERATURES = "cluster_data_jun_temperatures";
const char * Uniforms::Clustering::_DEC_TEMPERATURES = "cluster_data_dec_temperatures";
const char * Uniforms::Clustering::_MIN_ILLUMINATION = "cluster_data_min_illumination";
const char * Uniforms::Clustering::_MAX_ILLUMINATION = "cluster_data_max_illumination";
const char * Uniforms::Clustering::_SLOPE = "cluster_data_slope";
const char * Uniforms::Clustering::_JAN_SOIL_HUMIDITY = "cluster_data_jan_soil_humidity";
const char * Uniforms::Clustering::_FEB_SOIL_HUMIDITY = "cluster_data_feb_soil_humidity";
const char * Uniforms::Clustering::_MAR_SOIL_HUMIDITY = "cluster_data_mar_soil_humidity";
const char * Uniforms::Clustering::_APR_SOIL_HUMIDITY = "cluster_data_apr_soil_humidity";
const char * Uniforms::Clustering::_MAY_SOIL_HUMIDITY = "cluster_data_may_soil_humidity";
const char * Uniforms::Clustering::_JUN_SOIL_HUMIDITY = "cluster_data_jun_soil_humidity";
const char * Uniforms::Clustering::_JUL_SOIL_HUMIDITY = "cluster_data_jul_soil_humidity";
const char * Uniforms::Clustering::_AUG_SOIL_HUMIDITY = "cluster_data_aug_soil_humidity";
const char * Uniforms::Clustering::_SEP_SOIL_HUMIDITY = "cluster_data_sep_soil_humidity";
const char * Uniforms::Clustering::_OCT_SOIL_HUMIDITY = "cluster_data_oct_soil_humidity";
const char * Uniforms::Clustering::_NOV_SOIL_HUMIDITY = "cluster_data_nov_soil_humidity";
const char * Uniforms::Clustering::_DEC_SOIL_HUMIDITY = "cluster_data_dec_soil_humidity";
const char * Uniforms::Clustering::_SOIL_HUMIDITIES[12] = {
    Uniforms::Clustering::_JAN_SOIL_HUMIDITY,
    Uniforms::Clustering::_FEB_SOIL_HUMIDITY,
    Uniforms::Clustering::_MAR_SOIL_HUMIDITY,
    Uniforms::Clustering::_APR_SOIL_HUMIDITY,
    Uniforms::Clustering::_MAY_SOIL_HUMIDITY,
    Uniforms::Clustering::_JUN_SOIL_HUMIDITY,
    Uniforms::Clustering::_JUL_SOIL_HUMIDITY,
    Uniforms::Clustering::_AUG_SOIL_HUMIDITY,
    Uniforms::Clustering::_SEP_SOIL_HUMIDITY,
    Uniforms::Clustering::_OCT_SOIL_HUMIDITY,
    Uniforms::Clustering::_NOV_SOIL_HUMIDITY,
    Uniforms::Clustering::_DEC_SOIL_HUMIDITY,
};

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

