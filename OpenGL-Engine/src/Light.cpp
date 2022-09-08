#pragma region
#include "Light.hpp"
#include <math.h>
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::cos;
using glm::radians;
#pragma endregion

namespace Engine
{
	#pragma region Constructors
	Light::Light(LightType pType, vec3 pColour) : m_type(pType), m_lightColour(pColour)
	{
		m_angle = cos(radians(20.0f));
	}

	Light::Light(LightType pType, mat4 pTransform) : m_type(pType), m_lightColour(vec3(1))
	{
		SetTransform(pTransform);
		m_angle = cos(radians(20.0f));
	}

	Light::Light(LightType pType, mat4 pTransform, vec3 pColour) : m_type(pType), m_lightColour(pColour)
	{
		SetTransform(pTransform);
		m_angle = cos(radians(20.0f));
	}
	#pragma endregion
	#pragma region Setters
	Light* Light::SetType(LightType pType)
	{
		m_type = pType;
		return this;
	}

	Light* Light::SetCamPosition(vec3 pPosition)
	{
		SetPosition(pPosition);
		return this;
	}

	Light* Light::SetDirection(vec3 pDirection)
	{
		SetForward(pDirection);
		return this;
	}

	Light* Light::SetColour(vec3 pColour)
	{
		m_lightColour = pColour;
		return this;
	}

	Light* Light::SetLinear(float pValue)
	{
		m_linear = pValue;
		return this;
	}

	Light* Light::SetQuadratic(float pValue)
	{
		m_quadratic = pValue;
		return this;
	}

	Light* Light::SetAngle(float pValue)
	{
		m_angle = pValue;
		return this;
	}

	Light* Light::SetBlur(float pValue)
	{
		m_blur = pValue;
		return this;
	}
	#pragma endregion
	#pragma region Getters
	vec4 Light::GetDirection() const
	{
		return vec4(GetForward(), 0);
	}
	
	float Light::GetAngle()
	{
		return std::cosf(glm::radians(m_angle));
	}
	
	float Light::GetBlur()
	{
		return std::sinf(glm::radians(90 * m_blur));
	}
	#pragma endregion
}
