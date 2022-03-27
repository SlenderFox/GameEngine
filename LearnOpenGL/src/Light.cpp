#include "Light.hpp"

namespace Engine
{
	Light::Light(LightType pType) : m_type(pType)
	{
		//m_transform = mat4(1);
	}

	Light::Light(LightType pType, mat4 pTransform) : m_type(pType)
	{
		SetTransform(pTransform);
	}

	Light::Light(LightType pType, vec4 pPosition) : m_type(pType)
	{
		//m_transform = mat4(1);
		SetPosition(pPosition);
	}

	Light::Light(LightType pType, vec3 pDirection) : m_type(pType)
	{
		//m_transform = mat4(1);
		SetForward(pDirection);
	}

	Light::Light(LightType pType, mat4 pTransform, vec3 pColour) : m_type(pType), m_lightColour(pColour)
	{
		SetTransform(pTransform);
	}

	Light::Light(LightType pType, vec4 pPosition, vec3 pColour) : m_type(pType), m_lightColour(pColour)
	{
		//m_transform = mat4(1);
		SetPosition(pPosition);
	}

	Light::Light(LightType pType, vec3 pDirection, vec3 pColour) : m_type(pType), m_lightColour(pColour)
	{
		//m_transform = mat4(1);
		SetForward(pDirection);
	}

	Light::Light(LightType pType, vec4 pPosition, vec3 pDirection, vec3 pColour) : m_type(pType), m_lightColour(pColour)
	{
		//m_transform = mat4(1);
		SetPosition(pPosition);
		SetForward(pDirection);
	}
	
	void Light::SetColour(vec3 pColour)
	{
		m_lightColour = pColour;
	}

	vec3 Light::GetColour() const
	{
		return m_lightColour;
	}

	vec3 Light::GetDirection() const
	{
		return GetForward();
	}
}