#pragma region
#include "Light.hpp"
#include <math.h>

using glm::mat4;
using glm::cos;
using glm::sin;
using glm::radians;
#pragma endregion

namespace Engine
{
	Light::Light(LightType pType, Colour pColour): m_type(pType), m_lightColour(pColour)
	{
		m_angle = cos(radians(20.0f));
	}

	Light::Light(LightType pType, mat4 pTransform, Colour pColour): m_type(pType), m_lightColour(pColour)
	{
		Transform::SetTransform(pTransform);
		m_angle = cos(radians(20.0f));
	}

	float Light::GetAngle() const noexcept
	{
		return cos(radians(m_angle));
	}
	
	float Light::GetBlur() const noexcept
	{
		return sin(radians(90 * m_blur));
	}
}
