#ifndef COMPUTE_SHADER_PROGRAMS
#define COMPUTE_SHADER_PROGRAMS

#include <QOpenGLShaderProgram>

/**************************
 * COMPUTE SHADER PROGRAM *
 **************************/
class ComputeShaderProgram : public QOpenGLShaderProgram
{
    Q_OBJECT
public:
    ComputeShaderProgram(const QString compute_shader_filename);
    ~ComputeShaderProgram();
    void compileAndLink();

private:
    const QString m_compute_shader_filename;
};

/*******************************
 * WATER FLUX GENERATOR SHADER *
 *******************************/
class WaterFluxGeneratorShader : public ComputeShaderProgram
{
public:
    WaterFluxGeneratorShader();
    ~WaterFluxGeneratorShader();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};

/*******************************
 * WATER FLUX GENERATOR SHADER *
 *******************************/
class WaterComparatorShader : public ComputeShaderProgram
{
public:
    WaterComparatorShader();
    ~WaterComparatorShader();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};

/***********************************
 * SOIL HUMIDITY CALCULATOR SHADER *
 ***********************************/
class SoilHumidityCalculatorShader : public ComputeShaderProgram
{
public:
    SoilHumidityCalculatorShader();
    ~SoilHumidityCalculatorShader();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};

/****************************************
 * SLOPE BASED SOIL INFILTRATION SHADER *
 ****************************************/
class SlopeBasedSoilInfiltrationShader : public ComputeShaderProgram
{
public:
    SlopeBasedSoilInfiltrationShader();
    ~SlopeBasedSoilInfiltrationShader();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};

/**********************************
 * OVERLAY TEXTURE CREATOR SHADER *
 **********************************/
class OverlayTextureCreatorShaders : public ComputeShaderProgram
{
public:
    class Slope  : public ComputeShaderProgram
    {
    public:
        Slope();
        ~Slope();
    };
    class Altitude  : public ComputeShaderProgram
    {
    public:
        Altitude();
        ~Altitude();
    };
    class Shade  : public ComputeShaderProgram
    {
    public:
        Shade();
        ~Shade();
    };
    class Temperature  : public ComputeShaderProgram
    {
    public:
        Temperature();
        ~Temperature();
    };
    class DailyIllumination  : public ComputeShaderProgram
    {
    public:
        DailyIllumination();
        ~DailyIllumination();
    };
    class SoilInfiltrationRate  : public ComputeShaderProgram
    {
    public:
        SoilInfiltrationRate();
        ~SoilInfiltrationRate();
    };
    class MonthlySoilHumidity  : public ComputeShaderProgram
    {
    public:
        MonthlySoilHumidity();
        ~MonthlySoilHumidity();
    };
    class WeightedAvgSoilHumidity  : public ComputeShaderProgram
    {
    public:
        WeightedAvgSoilHumidity();
        ~WeightedAvgSoilHumidity();
    };

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};

/*********************************
 * AGGREGATE HEIGHT WATER SHADER *
 *********************************/
class AggregateHeightShader : public ComputeShaderProgram
{
public:
    AggregateHeightShader();
    ~AggregateHeightShader();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};

/**************************
 * BORDER OVERLAP REDUCER *
 **************************/
class BorderOverlapReducer : public ComputeShaderProgram
{
public:
    BorderOverlapReducer();
    ~BorderOverlapReducer();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};

/****************************
 * WEIGHTED AVERAGE CREATOR *
 ****************************/
class WeightedAverageCalculator : public ComputeShaderProgram
{
public:
    WeightedAverageCalculator();
    ~WeightedAverageCalculator();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};

/********************
 * NORMALS TO SLOPE *
 ********************/
class NormalsToSlopeConverter : public ComputeShaderProgram
{
public:
    NormalsToSlopeConverter();
    ~NormalsToSlopeConverter();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};

/**************************
 * CLOSEST CLUSTER FINDER *
 **************************/
class ClosestClusterFinder : public ComputeShaderProgram
{
public:
    ClosestClusterFinder();
    ~ClosestClusterFinder();

    static const int _GROUP_SIZE_X;
    static const int _GROUP_SIZE_Y;
    static const int _GROUP_SIZE_Z;
};

#endif //COMPUTE_SHADER_PROGRAMS
