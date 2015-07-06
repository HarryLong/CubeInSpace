﻿#include "terrain.h"
#include <iostream>
#include <cfloat>
#include "geom.h"
#include <math.h>

#include <cfloat>
#include <QProgressDialog>
#include "temp_edit_dlg.h"
#include "actions.h"
#include "shape_factory.h"

//-------------------------------------------------------------------
#define BASE_TERRAIN_SIZE 100
BaseTerrain::BaseTerrain()
{
    int width(BASE_TERRAIN_SIZE);
    int depth(BASE_TERRAIN_SIZE);
    m_height_data = (float*) malloc(sizeof(int) * width * depth);

//#pragma omp parallel for
//    for(int z = 0; z < depth; z++)
//    {
//        for(int x = 0; x < width; x++)
//        {
//            m_height_data[z*depth+x] = -0.001;
//        }
//    }

    memset(m_height_data, 0, sizeof(int) * width * depth);
    m_header_data.base_height = 0;
    m_header_data.depth = BASE_TERRAIN_SIZE;
    m_header_data.width = BASE_TERRAIN_SIZE;
    m_header_data.height_scale = 1.0f;
    m_header_data.max_height = .0f;
    m_header_data.min_height = .0f;
    m_header_data.scale = 30.0f;
}

BaseTerrain::~BaseTerrain()
{

}

/********************
 * DRAWABLE TERRAIN *
 ********************/
DrawableTerrain::DrawableTerrain() :
    TextureElement<GLfloat>(QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{

}

DrawableTerrain::~DrawableTerrain()
{

}

#include <iostream>
void DrawableTerrain::render()
{
    if(!initalised())
        initGL();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();

    m_vao_constraints.bind();

    f->glDrawElements(GL_TRIANGLE_STRIP, m_indicies.size(), GL_UNSIGNED_INT, (void*)(0)); CE();

    m_vao_constraints.release();

}

void DrawableTerrain::initGL()
{
    m_vao_constraints.create(); CE();
    m_vbo_constraints.create(); CE();
    m_ibo_constraints.create(); CE();

    m_vao_constraints.bind(); CE();

    m_vbo_constraints.bind();  CE();
    m_vbo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);  CE();

    m_ibo_constraints.bind(); CE();
    m_ibo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::DynamicDraw);  CE();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0); CE();
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(0)); CE();
    // Texture coordinate
    f->glEnableVertexAttribArray(1); CE();
    const int sz = 3*sizeof(GLfloat);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(sz)); CE();

    m_vao_constraints.release(); CE();
    m_vbo_constraints.release(); CE();
    m_ibo_constraints.release(); CE();

    fillBuffers();
}

bool DrawableTerrain::prepareTerrainGeometry(const TerragenFile & terragen_file)
{
    clear();

    for (float z (.0f); z < terragen_file.m_header_data.depth ; z++)
    {
        for (float x( 0 ); x < terragen_file.m_header_data.width; x++)
        {
            // 3D Vertex coordinate
            m_verticies.push_back(x);/*- (m_header_data.width/2 * m_header_data.dynamic_scale )*/ // X
            m_verticies.push_back(.0f); // Y (stored in heightmap texture)
            m_verticies.push_back(z);/*- (m_header_data.depth/2 * m_header_data.dynamic_scale )*/ // Z

            // 2D texture coordinate
            m_verticies.push_back((float) x / (float) (terragen_file.m_header_data.width - 1)); // X
            m_verticies.push_back((float) z / (float) (terragen_file.m_header_data.depth - 1)); // Y
        }
    }

    // Indices
    m_indicies.clear();
    for (int z = 0; z < terragen_file.m_header_data.width - 1; z++)
    {
        if (z > 0) // Degenerate begin: repeat first vertex
            m_indicies.push_back((GLuint) (z * terragen_file.m_header_data.width));

        for (int x = 0; x < terragen_file.m_header_data.width; x++)
        {
            // One part of the strip
            m_indicies.push_back((GLuint) ((z * terragen_file.m_header_data.width) + x));
            m_indicies.push_back((GLuint) (((z + 1) * terragen_file.m_header_data.width) + x));
        }

        if (z <  terragen_file.m_header_data.depth - 2)   // Degenerate end: repeat last vertex
            m_indicies.push_back((GLuint) (((z + 1) * terragen_file.m_header_data.width) + (terragen_file.m_header_data.width - 1)));
    }

    fillBuffers();
}

void DrawableTerrain::refreshHeightmapTexture(const TerragenFile & terragen_file)
{
    setData(terragen_file.m_height_data, terragen_file.m_header_data.width, terragen_file.m_header_data.depth);
}

/***********
 * TERRAIN *
 ***********/
const glm::vec4 Terrain::MaterialProperties::_DIFFUSE = glm::vec4(0.7f, 0.6f, 0.5f, 1.0f);
const glm::vec4 Terrain::MaterialProperties::_SPECULAR = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
const glm::vec4 Terrain::MaterialProperties::_AMBIENT = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
Terrain::Terrain(TerrainControllers & terrain_controllers, TemperatureEditDialog * temp_edit_dlg, Actions * overlay_actions) :
    m_terrain_controllers(terrain_controllers), m_overlay_actions(overlay_actions),
    m_selection_rectangle(ShapeFactory::getSelectionRectangle()),
    m_terrain_shade(new TerrainShade), m_terrain_normals(new TerrainNormals),
    m_terrain_min_daily_illumination(new TerrainDailyIllumination),
    m_terrain_max_daily_illumination(new TerrainDailyIllumination),
    m_terrain_temperature(new TerrainTemperature),
    m_terrain_water(new TerrainWater),
    m_temp_edit_dlg(temp_edit_dlg)
{
    establish_connections();
    selected_overlay_changed();
    recalculate_scale();
//    invalidate_temp();
//    invalidate_shade();
//    invalidateIllumination();
}

Terrain::~Terrain()
{
    delete m_terrain_shade;
    delete m_terrain_normals;
    delete m_terrain_min_daily_illumination;
    delete m_terrain_max_daily_illumination;
    delete m_terrain_water;
    delete m_terrain_temperature;
}

void Terrain::establish_connections()
{
    connect(m_terrain_controllers.terrain_scale_le, SIGNAL(valueChanged(int)), this, SLOT(recalculate_scale()));
    connect(m_terrain_controllers.use_default_scale_cb, SIGNAL(clicked(bool)), this, SLOT(recalculate_scale()));
    connect(m_overlay_actions->getActionGroup(), SIGNAL(triggered(QAction*)), this, SLOT(selected_overlay_changed()));
    // Recalculate temperature
    connect(m_temp_edit_dlg, SIGNAL(temperatureValuesChanged(float,float,float,float)), this, SLOT(refreshTemperature(float,float,float,float)));
}

void Terrain::loadBaseTerrain()
{
    setTerrain(BaseTerrain());
}

void Terrain::setTerrain(TerragenFile parsed_terrangen_file)
{
    m_temp_edit_dlg->reset();
    m_terragen_file = parsed_terrangen_file;
    m_terragen_file.summarize();
    recalculate_scale();

    // Remove selection rectangle
    clear_selection_rectangle();

    m_terrain_normals->setTerrainDim(m_terragen_file.m_header_data.width, m_terragen_file.m_header_data.depth);
    m_terrain_normals->setValid(false);

    m_sphere_acceleration_structure.build(m_terragen_file);

    m_drawable_terrain.refreshHeightmapTexture(parsed_terrangen_file);
    m_drawable_terrain.prepareTerrainGeometry(parsed_terrangen_file);

    invalidate_shade();
    invalidate_temp();
    invalidateIllumination();
    invalidate_water();

    emit terrainDimensionsChanged(getWidth(true), getDepth(true), getBaseHeight(true), getMaxHeight(true));
}

void Terrain::clear_selection_rectangle()
{
    m_selection_rectangle->clear();
}

float Terrain::getMaxHeight(bool scaled) const
{
    return (scaled ? m_scale : 1) * m_terragen_file.m_header_data.max_height;
}

float Terrain::getBaseHeight(bool scaled) const
{
    return (scaled ? m_scale : 1) * m_terragen_file.m_header_data.base_height;
}

int Terrain::getWidth(bool scaled) const
{
    return (scaled ? m_scale : 1) * m_terragen_file.m_header_data.width;
}

int Terrain::getDepth(bool scaled) const
{
    return (scaled ? m_scale : 1) * m_terragen_file.m_header_data.depth;
}

const SphericalAccelerationStructure<Hierarchical> & Terrain::getSphereAccelerationStructure()
{
    return m_sphere_acceleration_structure;
}

void Terrain::renderNormals()
{
    m_terrain_normals->render();
    m_terrain_normals->setValid(true);
}

void Terrain::render()
{
    m_drawable_terrain.render();
}

bool Terrain::normalsValid()
{
    return m_terrain_normals->valid();
}

void Terrain::getTerrainDimensions(int & width, int & depth, int & base_height, int & max_height )
{
    width = getWidth(true);
    depth = getDepth(true);
    base_height = getBaseHeight(true);
    max_height = getMaxHeight(true);
}

void Terrain::setMinIlluminationData(GLubyte * data)
{
    m_terrain_min_daily_illumination->setData(data, m_terragen_file.m_header_data.width, m_terragen_file.m_header_data.depth);
}

void Terrain::setMaxIlluminationData(GLubyte * data)
{
    m_terrain_max_daily_illumination->setData(data, m_terragen_file.m_header_data.width, m_terragen_file.m_header_data.depth);
}

void Terrain::refreshShade(QString display_message)
{
    emit processing(display_message);

    int terrain_width(m_terragen_file.m_header_data.width);
    int terrain_depth(m_terragen_file.m_header_data.depth);
    int n_iterations(terrain_depth*terrain_width);

    GLubyte * shade_data = NULL;

    if(!m_terrain_shade->isValid())    {
        // Generate the data
        shade_data = (GLubyte *) malloc(sizeof(GLubyte) * terrain_width * terrain_depth);

        if(m_sun_position[1] <= m_terragen_file.m_header_data.base_height) // Optimization - the sun is set. There will be no light
        {
            memset(shade_data, 0xFF, terrain_depth*terrain_width);
        }
        else
        {
            glm::vec3 dummy_intersection_point;
            int i(0);
            for(int z = 0 ; z < terrain_depth; z++, i+=terrain_width)
            {
                emit intermediate_processing_update((((float)i)/n_iterations)*100);
#pragma omp parallel for
                for(int x = 0; x < terrain_width; x++)
                {
                    glm::vec3 point_on_terrain(glm::vec3(x*m_scale, m_terragen_file(x,z)*m_scale, z*m_scale));
                    glm::vec3 sun_direction(glm::normalize(m_sun_position - point_on_terrain));

                    // First check if the normal is within sun angle
                    int index(z*terrain_depth+x);

                    if(glm::dot((*m_terrain_normals)(x,z), sun_direction) < 0 ||
                            ray_intersect(point_on_terrain, sun_direction, dummy_intersection_point, false)) // Not within direction
                    {
                        shade_data[index] = 0xFF;
                    }
                    else
                    {
                        shade_data[index] = 0x00;
                    }
                }
            }
        }

        m_terrain_shade->setData(shade_data, terrain_width, terrain_depth);
    }

    emit processing_complete();
}

void Terrain::refreshTemperature(float temp_at_zero_june, float lapse_rate_june, float temp_at_zero_dec, float lapse_rate_dec)
{
    int terrain_width(m_terragen_file.m_header_data.width);
    int terrain_depth(m_terragen_file.m_header_data.depth);
    int n_iterations(terrain_depth*terrain_width*2);

    emit processing("Calculating Temperature...");
    int iteration_counter(0);

    // June temperatures
    {
        float temp_at_zero(temp_at_zero_june);
        float lapse_rate(lapse_rate_june);

        GLbyte * temp_data = (GLbyte*) malloc(sizeof(GLbyte) * terrain_width * terrain_depth);

        for(int z = 0 ; z < terrain_depth; z++, iteration_counter += terrain_width)
        {
            emit intermediate_processing_update((((float)iteration_counter)/n_iterations)*100);
#pragma omp parallel for
            for(int x = 0; x < terrain_width; x++)
            {
                int index(z*terrain_depth+x);

                float altitude( get_altitude(glm::vec2(x,z)) );

                float temp(temp_at_zero - ((altitude/1000.0f) * lapse_rate));

                temp_data[index] = (GLbyte) std::min(50.0f,std::max(-50.0f, temp));
            }
        }

        m_terrain_temperature->getJunTemperature()->setData(temp_data, terrain_width, terrain_depth);
    }

    // December temperatures
    {
        float temp_at_zero(temp_at_zero_dec);
        float lapse_rate(lapse_rate_dec);

        GLbyte * temp_data = (GLbyte*) malloc(sizeof(GLbyte) * terrain_width * terrain_depth);
        int i(0);
        for(int z = 0 ; z < terrain_depth; z++, iteration_counter += terrain_width)
        {
            emit intermediate_processing_update((((float)iteration_counter)/n_iterations)*100);
#pragma omp parallel for
            for(int x = 0; x < terrain_width; x++)
            {
                int index(z*terrain_depth+x);

                float altitude( get_altitude(glm::vec2(x,z)) );

                float temp(temp_at_zero - ((altitude/1000.0f) * lapse_rate));

                temp_data[index] = (GLbyte) std::min(50.0f,std::max(-50.0f, temp));
            }
        }
        m_terrain_temperature->getDecTemperature()->setData(temp_data, terrain_width, terrain_depth);
    }
    emit processing_complete();

    (*m_overlay_actions)(OverlayActions::_TEMPERATURE)->setEnabled(true);
}

void Terrain::reset_overlay()
{
    (*m_overlay_actions)(OverlayActions::_NONE)->setChecked(true);
    selected_overlay_changed();
}

void Terrain::refresh_illumination()
{
    emit trigger_daily_illumination_refresh();
}

void Terrain::setSunPosition(float sun_pos_x, float sun_pos_y, float sun_pos_z)
{
    std::cout << "Sun position changed!" << std::endl;
    m_sun_position = glm::vec3(sun_pos_x, sun_pos_y, sun_pos_z);
    invalidate_shade();
}

void Terrain::invalidate_shade()
{
    m_terrain_shade->invalidate();
    if(overlayShade())
        reset_overlay();
}

void Terrain::invalidateIllumination()
{
    m_terrain_min_daily_illumination->invalidate();
    m_terrain_max_daily_illumination->invalidate();

    if(overlayMinDailyIllumination() ||
            overlayMaxDailyIllumination())
    {
        reset_overlay();
    }
}

void Terrain::invalidate_temp()
{
    (*m_overlay_actions)(OverlayActions::_TEMPERATURE)->setEnabled(false);
    m_terrain_temperature->invalidate();

    if(overlayTemperature())
        reset_overlay();
}

void Terrain::invalidate_water()
{
    int sz(sizeof(GLuint) * m_terragen_file.m_header_data.width * m_terragen_file.m_header_data.depth);
    GLuint * base_water = (GLuint*) malloc(sz);

#pragma omp parallel for
    for(int i = 0; i < m_terragen_file.m_header_data.depth * m_terragen_file.m_header_data.width; i++)
    {
        base_water[i] = (GLuint) 100;
    }

//    memset(base_water, 0, sz);

    m_terrain_water->setData(base_water, m_terragen_file.m_header_data.width, m_terragen_file.m_header_data.depth);CE();
}

bool Terrain::traceRay(glm::vec3 start_point, glm::vec3 direction, glm::vec3 & intersection_point)
{
    return ray_intersect(start_point, direction, intersection_point);
}

bool Terrain::ray_intersect(const glm::vec3 & start, const glm::vec3 & direction, glm::vec3 & intersection_point, bool search_closest)
{
    // bounding sphere accel structure
    float scaler(.0f), distance(.0f);
    int tolerance (1);
    bool found(false);

    float min_distance(FLT_MAX);

    // Get all possible spheres
    std::vector<AccelerationSphere> intersecting_spheres(m_sphere_acceleration_structure.get_intersecting_spheres(start, direction, m_scale));

    for(AccelerationSphere intersecting_sphere : intersecting_spheres)
    {
        int x_min(intersecting_sphere.m_center[0] - intersecting_sphere.m_radius);
        int x_max(std::min(getWidth()-1, ((int) intersecting_sphere.m_center[0] + intersecting_sphere.m_radius)));

        int z_min(intersecting_sphere.m_center[2] - intersecting_sphere.m_radius);
        int z_max(std::min(getDepth()-1, ((int) intersecting_sphere.m_center[2] + intersecting_sphere.m_radius)));

        for(int point_in_sphere_x = x_min; point_in_sphere_x < x_max; point_in_sphere_x++)
        {
            for(int point_in_sphere_z = z_min; point_in_sphere_z < z_max; point_in_sphere_z++)
            {
                glm::vec3 point_in_sphere(point_in_sphere_x*m_scale, m_terragen_file(point_in_sphere_x,point_in_sphere_z)*m_scale,
                                          point_in_sphere_z*m_scale);
                if(Geom::length(Geom::diff(start, point_in_sphere)) > 5) // Make sure the point is further than 5 unit distance from the start point
                {
                    Geom::rayPointDist(start, direction, point_in_sphere, scaler, distance);
                    if(distance < tolerance && distance < min_distance)
                    {
                        found = true;
                        min_distance = distance;
                        intersection_point = point_in_sphere;
                        if(!search_closest)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return found;
}

void Terrain::setSelectionRectangle(glm::vec3 min, glm::vec3 max)
{
    m_selection_rectangle->resize(min/m_scale, max/m_scale, m_terragen_file.m_header_data.width, m_terragen_file.m_header_data.depth);
}

Asset* Terrain::getSelectionRect()
{
    return m_selection_rectangle;
}

float Terrain::getScale()
{
    return m_scale;
}

void Terrain::recalculate_scale()
{
    if(m_terrain_controllers.use_default_scale_cb->isChecked())
    {
        m_scale = 1.0f;
        m_terrain_controllers.terrain_scale_le->setText(QString::number(m_terragen_file.m_header_data.scale));
    }
    else
    {
        int requested_scale(m_terrain_controllers.terrain_scale_le->value());
        m_scale = (m_terragen_file.m_header_data.scale/requested_scale);
        m_terrain_controllers.terrain_scale_le->setText(QString::number(requested_scale));
    }
}

DrawableTerrain & Terrain::getDrawableTerrain()
{
    return m_drawable_terrain;
}

TerrainShade * Terrain::getShade()
{
    return m_terrain_shade;
}

TerrainNormals * Terrain::getNormals()
{
    return m_terrain_normals;
}

TerrainTemperature * Terrain::getTemperature()
{
    return m_terrain_temperature;
}

TerrainDailyIllumination * Terrain::getMinDailyIllumination()
{
    return m_terrain_min_daily_illumination;
}

TerrainDailyIllumination * Terrain::getMaxDailyIllumination()
{
    return m_terrain_max_daily_illumination;
}

TerrainWater * Terrain::getTerrainWater()
{
    return m_terrain_water;
}

void Terrain::selected_overlay_changed()
{
    if(overlayShade() && !m_terrain_shade->isValid())
        refreshShade();

    if((overlayMinDailyIllumination() ||
            overlayMaxDailyIllumination()) &&
        (!m_terrain_min_daily_illumination->isValid() || !m_terrain_max_daily_illumination->isValid()))
        refresh_illumination();

    emit activeOverlayChanged();
}

float Terrain::getAltitude(const glm::vec3 & point)
{
    return get_altitude(glm::vec2(point[0]/m_scale, point[2]/m_scale));
}

float Terrain::getSlope(const glm::vec3 & point)
{
    return get_slope(glm::vec2(point[0]/m_scale, point[2]/m_scale));
}

bool Terrain::getTemperatures(const glm::vec3 &point, int month, float & temperature)
{
    return get_temperatures(glm::vec2(point[0]/m_scale, point[2]/m_scale), month, temperature);
}

bool Terrain::getDailyIlluminations(const glm::vec3 & point, int & min, int & max)
{
    return get_daily_illuminations(glm::vec2(point[0]/m_scale, point[2]/m_scale), min, max);
}

int Terrain::getWaterHeight(const glm::vec3 & point)
{
    return get_water_height(glm::vec2(point[0]/m_scale, point[2]/m_scale));
}

const TerragenFile & Terrain::getHeightMap()
{
    return m_terragen_file;
}

bool Terrain::isShaded(const glm::vec3 &point, bool &shaded)
{
    return is_shaded(glm::vec2(point[0]/m_scale, point[2]/m_scale), shaded);
}

float Terrain::get_altitude(const glm::vec2 & point)
{
    return m_terragen_file(point[0], point[1]) * m_scale * m_terragen_file.m_header_data.scale;
}

float Terrain::get_slope(const glm::vec2 & point)
{
    glm::vec3 normal((*m_terrain_normals)(point[0],point[1]));
    glm::vec3 vertical_vector(glm::vec3(0,1,0));

    return Geom::toDegrees(std::abs(std::acos(glm::dot(normal, vertical_vector))));
}

bool Terrain::get_temperatures(const glm::vec2 &point, int month, float & temperature)
{
    if(m_terrain_temperature->isValid())
    {
        float jun_temperature = (*m_terrain_temperature->getJunTemperature())(point[0], point[1]);
        float dec_temperature = (*m_terrain_temperature->getDecTemperature())(point[0], point[1]);
        float temp_diff = jun_temperature - dec_temperature;

        float jun_percentage = (6.0-(abs(6-month)))/6.0;

        temperature = dec_temperature + (jun_percentage * temp_diff);

        return true;
    }

    return false;
}

bool Terrain::get_daily_illuminations(const glm::vec2 & point, int & min, int & max)
{
    if(m_terrain_min_daily_illumination->isValid() && m_terrain_max_daily_illumination->isValid())
    {
        min = (*m_terrain_min_daily_illumination)(point[0], point[1]);
        max = (*m_terrain_max_daily_illumination)(point[0], point[1]);
        return true;
    }

    return false;
}

int Terrain::get_water_height(const glm::vec2 & point)
{
    return (*m_terrain_water)(point[0], point[1]);
}

bool Terrain::is_shaded(const glm::vec2 &point, bool &shaded)
{
    if(m_terrain_shade->isValid())
    {
        shaded = (*m_terrain_shade)(point[0], point[1]);
        return true;
    }

    return false;
}

bool Terrain::overlayNone() const
{
    return (*m_overlay_actions)(OverlayActions::_NONE)->isChecked();
}

bool Terrain::overlaySlope() const
{
    return (*m_overlay_actions)(OverlayActions::_SLOPE)->isChecked();
}

bool Terrain::overlayAltitude() const
{
    return (*m_overlay_actions)(OverlayActions::_ALTITUDE)->isChecked();
}

bool Terrain::overlayShade() const
{
    return (*m_overlay_actions)(OverlayActions::_SHADE)->isChecked();
}

bool Terrain::overlayTemperature() const
{
    return (*m_overlay_actions)(OverlayActions::_TEMPERATURE)->isChecked();
}

bool Terrain::overlayMinDailyIllumination() const
{
    return (*m_overlay_actions)(OverlayActions::_MIN_DAILY_ILLUMINATION)->isChecked();
}

bool Terrain::overlayMaxDailyIllumination() const
{
    return (*m_overlay_actions)(OverlayActions::_MAX_DAILY_ILLUMINATION)->isChecked();
}
