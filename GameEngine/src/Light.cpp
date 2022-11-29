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
		const LightType inType,
		const Colour inColour,
		mat4 inTransform
	) noexcept
		: m_type(inType)
		, m_lightColour(inColour)
	{
		Transform::SetTransform(&inTransform);
		if (inType == LightType::Spot)
		{ m_angle = cos(radians(m_angle)); }
	}

	void Light::SetDirection (const glm::vec3 inDirection) noexcept
	{ Transform::SetForward(inDirection); }

	void Light::SetType (LightType inType) noexcept
	{ m_type = inType; }

	void Light::SetColour (Colour inColour) noexcept
	{ m_lightColour = inColour; }

	void Light::SetLinear (float inValue) noexcept
	{ m_linear = inValue; }

	void Light::SetQuadratic (float inValue) noexcept
	{ m_quadratic = inValue; }

	void Light::SetAngle (float inValue) noexcept
	{ m_angle = inValue; }

	void Light::SetBlur (float inValue) noexcept
	{ m_blur = inValue; }

	float Light::GetAngle() const noexcept
	{ return cos(radians(m_angle)); }

	float Light::GetBlur() const noexcept
	{ return sin(radians(90 * m_blur)); }

	glm::vec4 Light::GetDirection()	const noexcept
	{ return Transform::GetForward(); }

	LightType Light::GetType() const noexcept
	{ return m_type; }

	Colour Light::GetColour() const noexcept
	{ return m_lightColour; }

	float Light::GetLinear() const noexcept
	{ return m_linear; }

	float Light::GetQuadratic() const noexcept
	{ return m_quadratic; }

	float Light::GetAngleRaw() const noexcept
	{ return m_angle; }

	float Light::GetBlurRaw() const noexcept
	{ return m_blur; }
}
