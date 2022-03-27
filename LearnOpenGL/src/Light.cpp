#include "Light.hpp"

namespace Engine
{
	#pragma region Constructors
	// Generic
	Light::Light(LightType pType)
	 : m_type(pType)
	{
		//m_transform = mat4(1);
		m_lightColour = vec3(1);
		m_angle = glm::cos(glm::radians(20.0f));
	}
	// Generic
	Light::Light(LightType pType, mat4 pTransform)
	 : m_type(pType)
	{
		SetTransform(pTransform);
		m_lightColour = vec3(1);
		m_angle = glm::cos(glm::radians(20.0f));
	}
	// Generic
	Light::Light(LightType pType, mat4 pTransform, vec3 pColour)
	 : m_type(pType), m_lightColour(pColour)
	{
		SetTransform(pTransform);
		m_angle = glm::cos(glm::radians(20.0f));
	}
	// Directional
	Light::Light(LightType pType, vec3 pDirection)
	 : m_type(pType)
	{
		//m_transform = mat4(1);
		SetForward(pDirection);
		m_lightColour = vec3(1);
		m_angle = glm::cos(glm::radians(20.0f));
	}
	// Directional
	Light::Light(LightType pType, vec3 pDirection, vec3 pColour)
	 : m_type(pType), m_lightColour(pColour)
	{
		//m_transform = mat4(1);
		SetForward(pDirection);
		m_angle = glm::cos(glm::radians(20.0f));
	}
	// Point
	Light::Light(LightType pType, vec4 pPosition)
	 : m_type(pType)
	{
		//m_transform = mat4(1);
		SetPosition(pPosition);
		m_lightColour = vec3(1);
		m_angle = glm::cos(glm::radians(20.0f));
	}
	// Point
	Light::Light(LightType pType, vec4 pPosition, vec3 pColour)
	 : m_type(pType), m_lightColour(pColour)
	{
		//m_transform = mat4(1);
		SetPosition(pPosition);
		m_angle = glm::cos(glm::radians(20.0f));
	}
	// Spot
	Light::Light(LightType pType, mat4 pTransform, float pAngle, float pSharpness)
	 : m_type(pType), m_angle(pAngle), m_sharpness(pSharpness)
	{
		SetTransform(pTransform);
		m_lightColour = vec3(1);
	}
	// Spot
	Light::Light(LightType pType, mat4 pTransform, vec3 pColour, float pAngle, float pSharpness)
	 : m_type(pType), m_lightColour(pColour), m_angle(pAngle), m_sharpness(pSharpness)
	{
		SetTransform(pTransform);
	}
	// Spot
	Light::Light(LightType pType, vec4 pPosition, vec3 pDirection, vec3 pColour, float pAngle, float pSharpness)
	 : m_type(pType), m_lightColour(pColour), m_angle(pAngle), m_sharpness(pSharpness)
	{
		//m_transform = mat4(1);
		SetPosition(pPosition);
		SetForward(pDirection);
	}
	#pragma endregion
	
	void Light::SetColour(vec3 pColour)
	{
		m_lightColour = pColour;
	}
	
	#pragma region Getters
	LightType Light::GetType() const
	{
		return m_type;
	}

	vec3 Light::GetColour() const
	{
		return m_lightColour;
	}

	vec4 Light::GetDirection() const
	{
		return vec4(GetForward(), 0);
	}
	
	float Light::GetAngle() const
	{
		return m_angle;
	}
	
	float Light::GetSharpness() const
	{
		return m_sharpness;
	}
	#pragma endregion
}