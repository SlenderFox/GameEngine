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
}