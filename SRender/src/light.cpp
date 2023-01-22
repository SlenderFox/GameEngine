#pragma region
#include "light.hpp"
#include <math.h>

using glm::mat4;
using glm::cos;
using glm::sin;
using glm::radians;
#pragma endregion

namespace srender
{
	light::light(
		const lightType inType,
		const colour inColour,
		mat4 inTransform
	) noexcept
		: m_type(inType)
		, m_lightColour(inColour)
	{
		transform::setTransform(&inTransform);
		if (inType == lightType::Spot)
		{ m_angle = cos(radians(m_angle)); }
	}

	void light::setDirection (const glm::vec3 inDirection) noexcept
	{ transform::setForward(inDirection); }

	void light::setType (lightType inType) noexcept
	{ m_type = inType; }

	void light::setColour (colour inColour) noexcept
	{ m_lightColour = inColour; }

	void light::setLinear (float inValue) noexcept
	{ m_linear = inValue; }

	void light::setQuadratic (float inValue) noexcept
	{ m_quadratic = inValue; }

	void light::setAngle (float inValue) noexcept
	{ m_angle = inValue; }

	void light::setBlur (float inValue) noexcept
	{ m_blur = inValue; }

	float light::getAngle() const noexcept
	{ return cos(radians(m_angle)); }

	float light::getBlur() const noexcept
	{ return sin(radians(90 * m_blur)); }

	glm::vec4 light::getDirection()	const noexcept
	{ return transform::getForward(); }

	lightType light::getType() const noexcept
	{ return m_type; }

	colour light::getColour() const noexcept
	{ return m_lightColour; }

	float light::getLinear() const noexcept
	{ return m_linear; }

	float light::getQuadratic() const noexcept
	{ return m_quadratic; }

	float light::getAngleRaw() const noexcept
	{ return m_angle; }

	float light::getBlurRaw() const noexcept
	{ return m_blur; }
}
