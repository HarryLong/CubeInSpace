#include "light_properties.h"

/********************
 * LIGHT PROPERTIES *
 ********************/
LightProperties::LightProperties(glm::vec4 position, glm::vec4 diffuse_color, glm::vec4 specular_color, glm::vec4 ambient_color) :
        m_position(position), m_diffuse_color(diffuse_color), m_specular_color(specular_color), m_ambient_color(ambient_color)
{

}

LightProperties::~LightProperties()
{
}

glm::vec4 LightProperties::getPosition() const
{
    return m_position;
}
