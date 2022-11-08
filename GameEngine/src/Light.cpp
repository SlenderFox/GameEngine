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
	Light::Light(
		const LightType pType,
		const Colour pColour,
		mat4 pTransform)
		: m_type(pType),
		m_lightColour(pColour)
	{
		Transform::SetTransform(&pTransform);
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
