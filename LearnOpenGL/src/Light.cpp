#include "Light.hpp"

namespace Engine
{
    Light::Light(glm::vec3 pColour)
    {
        m_lightColour = pColour;
    }

    glm::vec3 Light::GetColour() const
    {
        return m_lightColour;
    }

    void Light::SetColour(glm::vec3 pColour)
    {
        m_lightColour = pColour;
    }
}