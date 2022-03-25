#include "Light.hpp"

namespace Engine
{
	Light::Light(vec3 pColour)
	{
		m_lightColour = pColour;
	}
	
	Light::Light(vec3 pPosition, vec3 pColour)
	{
		SetPosition(pPosition);
		m_lightColour = pColour;
	}

	void Light::SetColour(vec3 pColour)
	{
		m_lightColour = pColour;
	}

	vec3 Light::GetColour() const
	{
		return m_lightColour;
	}

	LightDirectional::LightDirectional(vec3 pColour)
	{
		m_direction = glm::vec3(0, -1, 0);
		m_lightColour = pColour;
	}

	LightDirectional::LightDirectional(vec3 pDirection, vec3 pColour)
	{
		m_direction = pDirection;
		m_lightColour = pColour;
	}

	void LightDirectional::SetDirection(vec3 pValue)
	{
		m_direction = pValue;
	}

	vec3 LightDirectional::GetDirection() const
	{
		return m_direction;
	}
}