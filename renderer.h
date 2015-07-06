#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <map>
#include <vector>
#include <QDir>
#include "terrain.h"
#include "constants.h"
#include "light_properties.h"
#include <QMatrix>
#include "grid.h"
#include "transform.h"

class QOpenGLShaderProgram;
class TerrainWater;
class WaterFluxGeneratorShader;

class Renderer {
public:
    Renderer();
    ~Renderer();
    void init();

    void renderTerrain(QOpenGLShaderProgram * shader, const Transform & p_transforms, Terrain * terrain, const LightProperties & sunlight_properties,
                       int month);
    void renderTerrainElements(QOpenGLShaderProgram * shader, const Transform & p_transforms, const std::vector<Asset*> & p_assets, Terrain * terrain);
    void renderAssets(QOpenGLShaderProgram * shader, const Transform & p_transforms, const std::vector<Asset*> & p_assets);
    void calculateNormals(QOpenGLShaderProgram * shader, Terrain * terrain);

    void tmp_function1(WaterFluxGeneratorShader * shader, Terrain * terrain);

private:
    void reset_overlays(QOpenGLShaderProgram * shader);
};

#endif // RENDERER_H
