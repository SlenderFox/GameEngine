#pragma once
#include "colour.hpp"
#include "glm/vec4.hpp"

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
/** A light source
 * @todo Overhaul, perhaps make it a component that can attach to an entity
 * @todo Perhaps have all lights stored in a static vector here
 */
class light
{
public:
	enum class type: uint8_t
	{
		directional,
		point,
		spot
	};

private:
	type m_type;
	colour m_lightColour;
	float m_linear = 0.045f;     // For point and spot lights
	float m_quadratic = 0.0075f; // For point and spot lights
	float m_angle = 10.0f;       // Only for spotlights
	float m_blur = 0.23f;        // Only for spotlights
	glm::vec4 m_position = glm::vec4(0);
	glm::vec4 m_forward = glm::vec4(0);

public:
	light(
		const type _type = type::directional,
		const colour _colour = colour::white()
	) noexcept;
	~light() = default;

	void setType(type _type) noexcept;
	void setColour(colour _colour) noexcept;
	void setLinear(float _value) noexcept;
	void setQuadratic(float _value) noexcept;
	void setAngle(float _value) noexcept; // In degrees
	void setBlur(float _value) noexcept;  // In degrees
	void setPosition(glm::vec4 _value) noexcept;
	void setForward(glm::vec4 _value) noexcept;

	_NODISCARD float getAngle() const noexcept;
	_NODISCARD float getBlur() const noexcept;
	_NODISCARD type getType() const noexcept;
	_NODISCARD colour getColour() const noexcept;
	_NODISCARD float getLinear() const noexcept;
	_NODISCARD float getQuadratic() const noexcept;
	_NODISCARD float getAngleRaw() const noexcept;
	_NODISCARD float getBlurRaw() const noexcept;
	glm::vec4 getPosition() const noexcept;
	glm::vec4 getForward() const noexcept;
};
}
