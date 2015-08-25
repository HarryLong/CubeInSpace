#include "compute_shader_programs.h"
#include "../gl_core/glheader.h"

/**************************
 * COMPUTE SHADER PROGRAM *
 **************************/
ComputeShaderProgram::ComputeShaderProgram(const QString compute_shader_filename) :
    m_compute_shader_filename(compute_shader_filename)
{

}

ComputeShaderProgram::~ComputeShaderProgram()
{

}

void ComputeShaderProgram::compileAndLink()
{
    bool succ (addShaderFromSourceFile(QOpenGLShader::Compute, m_compute_shader_filename)); CE();
    if (!succ) qWarning() << "Shader compile log:" << log();

    succ = link();
    if (!succ) qWarning() << "Shader compile log:" << log();
}

/*******************************
 * WATER FLUX GENERATOR SHADER *
 *******************************/
const int WaterFluxGeneratorShader::_GROUP_SIZE_X = 32;
const int WaterFluxGeneratorShader::_GROUP_SIZE_Y = 32;
const int WaterFluxGeneratorShader::_GROUP_SIZE_Z = 1;
WaterFluxGeneratorShader::WaterFluxGeneratorShader() : ComputeShaderProgram(":/water_flux.comp")
{

}

WaterFluxGeneratorShader::~WaterFluxGeneratorShader()
{

}


/*******************************
 * WATER FLUX GENERATOR SHADER *
 *******************************/
const int WaterComparatorShader::_GROUP_SIZE_X = 32;
const int WaterComparatorShader::_GROUP_SIZE_Y = 32;
const int WaterComparatorShader::_GROUP_SIZE_Z = 1;
WaterComparatorShader::WaterComparatorShader() : ComputeShaderProgram(":/water_heightmap_comparator.comp")
{

}

WaterComparatorShader::~WaterComparatorShader()
{

}

/************************************
 * STANDING WATER CALCULATOR SHADER *
 ************************************/
const int StandingWaterCalculator::_GROUP_SIZE_X = 32;
const int StandingWaterCalculator::_GROUP_SIZE_Y = 32;
const int StandingWaterCalculator::_GROUP_SIZE_Z = 1;
StandingWaterCalculator::StandingWaterCalculator() : ComputeShaderProgram(":/standing_water_calculator.comp")
{

}

StandingWaterCalculator::~StandingWaterCalculator()
{

}


/***********************************
 * SOIL HUMIDITY CALCULATOR SHADER *
 ***********************************/
const int SoilHumidityCalculatorShader::_GROUP_SIZE_X = 32;
const int SoilHumidityCalculatorShader::_GROUP_SIZE_Y = 32;
const int SoilHumidityCalculatorShader::_GROUP_SIZE_Z = 1;
SoilHumidityCalculatorShader::SoilHumidityCalculatorShader() : ComputeShaderProgram(":/soil_humidity_calculator.comp")
{

}

SoilHumidityCalculatorShader::~SoilHumidityCalculatorShader()
{

}

/****************************************
 * SLOPE BASED SOIL INFILTRATION SHADER *
 ****************************************/
const int SlopeBasedSoilInfiltrationShader::_GROUP_SIZE_X = 32;
const int SlopeBasedSoilInfiltrationShader::_GROUP_SIZE_Y = 32;
const int SlopeBasedSoilInfiltrationShader::_GROUP_SIZE_Z = 1;
SlopeBasedSoilInfiltrationShader::SlopeBasedSoilInfiltrationShader() : ComputeShaderProgram(":/slope_based_soil_infiltration.comp")
{

}

SlopeBasedSoilInfiltrationShader::~SlopeBasedSoilInfiltrationShader()
{

}

/**********************************
 * OVERLAY TEXTURE CREATOR SHADER *
 **********************************/
const int OverlayTextureCreatorShaders::_GROUP_SIZE_X = 32;
const int OverlayTextureCreatorShaders::_GROUP_SIZE_Y = 32;
const int OverlayTextureCreatorShaders::_GROUP_SIZE_Z = 1;
OverlayTextureCreatorShaders::Slope::Slope() : ComputeShaderProgram(":/overlay_texture_creator_slope.comp") {}
OverlayTextureCreatorShaders::Slope::~Slope() {}

OverlayTextureCreatorShaders::Altitude::Altitude() : ComputeShaderProgram(":/overlay_texture_creator_altitude.comp") {}
OverlayTextureCreatorShaders::Altitude::~Altitude() {}

OverlayTextureCreatorShaders::Shade::Shade() : ComputeShaderProgram(":/overlay_texture_creator_shade.comp") {}
OverlayTextureCreatorShaders::Shade::~Shade() {}

OverlayTextureCreatorShaders::Temperature::Temperature() : ComputeShaderProgram(":/overlay_texture_creator_temperature.comp") {}
OverlayTextureCreatorShaders::Temperature::~Temperature() {}

OverlayTextureCreatorShaders::DailyIllumination::DailyIllumination() : ComputeShaderProgram(":/overlay_texture_creator_daily_illumination.comp") {}
OverlayTextureCreatorShaders::DailyIllumination::~DailyIllumination() {}

OverlayTextureCreatorShaders::SoilInfiltrationRate::SoilInfiltrationRate() : ComputeShaderProgram(":/overlay_texture_creator_soil_infiltration_rate.comp") {}
OverlayTextureCreatorShaders::SoilInfiltrationRate::~SoilInfiltrationRate() {}

OverlayTextureCreatorShaders::MonthlySoilHumidity::MonthlySoilHumidity() : ComputeShaderProgram(":/overlay_texture_creator_soil_humidity.comp") {}
OverlayTextureCreatorShaders::MonthlySoilHumidity::~MonthlySoilHumidity() {}

OverlayTextureCreatorShaders::WeightedAvgSoilHumidity::WeightedAvgSoilHumidity() : ComputeShaderProgram(":/overlay_texture_creator_weighted_avg_soil_humidity.comp") {}
OverlayTextureCreatorShaders::WeightedAvgSoilHumidity::~WeightedAvgSoilHumidity() {}

OverlayTextureCreatorShaders::Clusters::Clusters() : ComputeShaderProgram(":/overlay_texture_creator_cluster.comp") {}
OverlayTextureCreatorShaders::Clusters::~Clusters() {}

/*********************************
 * AGGREGATE HEIGHT WATER SHADER *
 *********************************/
const int AggregateHeightShader::_GROUP_SIZE_X = 32;
const int AggregateHeightShader::_GROUP_SIZE_Y = 32;
const int AggregateHeightShader::_GROUP_SIZE_Z = 1;
AggregateHeightShader::AggregateHeightShader() : ComputeShaderProgram(":/aggregate_height_setter.comp")
{

}

AggregateHeightShader::~AggregateHeightShader()
{

}

/**************************
 * BORDER OVERLAP REDUCER *
 **************************/
const int BorderOverlapReducer::_GROUP_SIZE_X = 32;
const int BorderOverlapReducer::_GROUP_SIZE_Y = 32;
const int BorderOverlapReducer::_GROUP_SIZE_Z = 1;
BorderOverlapReducer::BorderOverlapReducer() : ComputeShaderProgram(":/border_overlap_reducer.comp")
{

}

BorderOverlapReducer::~BorderOverlapReducer()
{

}

/****************************
 * WEIGHTED AVERAGE CREATOR *
 ****************************/
const int WeightedAverageCalculator::_GROUP_SIZE_X = 32;
const int WeightedAverageCalculator::_GROUP_SIZE_Y = 32;
const int WeightedAverageCalculator::_GROUP_SIZE_Z = 1;
WeightedAverageCalculator::WeightedAverageCalculator() : ComputeShaderProgram(":/weighted_avg_calculator.comp")
{

}

WeightedAverageCalculator::~WeightedAverageCalculator()
{

}

/********************
 * NORMALS TO SLOPE *
 ********************/
const int NormalsToSlopeConverter::_GROUP_SIZE_X = 32;
const int NormalsToSlopeConverter::_GROUP_SIZE_Y = 32;
const int NormalsToSlopeConverter::_GROUP_SIZE_Z = 1;
NormalsToSlopeConverter::NormalsToSlopeConverter() : ComputeShaderProgram(":/normals_to_slope_converter.comp")
{

}

NormalsToSlopeConverter::~NormalsToSlopeConverter()
{

}

/*********************
 * K-MEANS CLUSTERER *
 *********************/
const int KMeansClustererShaders::ClosestClusterFinder::_GROUP_SIZE_X = 16;
const int KMeansClustererShaders::ClosestClusterFinder::_GROUP_SIZE_Y = 16;
const int KMeansClustererShaders::ClosestClusterFinder::_GROUP_SIZE_Z = 1;
KMeansClustererShaders::ClosestClusterFinder::ClosestClusterFinder() : ComputeShaderProgram(":/closest_cluster_finder.comp") { }
KMeansClustererShaders::ClosestClusterFinder::~ClosestClusterFinder() { }

const int KMeansClustererShaders::MeansRecalculator::_GROUP_SIZE_X = 32;
const int KMeansClustererShaders::MeansRecalculator::_GROUP_SIZE_Y = 1;
const int KMeansClustererShaders::MeansRecalculator::_GROUP_SIZE_Z = 1;
KMeansClustererShaders::MeansRecalculator::MeansRecalculator() : ComputeShaderProgram(":/means_recalculator.comp") {}
KMeansClustererShaders::MeansRecalculator::~MeansRecalculator() {}
