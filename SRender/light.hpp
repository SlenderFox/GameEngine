#pragma once
#include "entity.hpp"

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
/** A light source
 * @todo Overhaul, perhaps make it a component that can attach to an entity
 * @todo Perhaps have all lights stored in a static vector here
 * @todo Overhaul light rendering in shader
 * @todo Min and max clipped brightness
 * @todo Iris, instant brightness response
 * @todo Retina, slower brightness response
 */
class light : public entity
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

public:
	light(
		const type _type = type::directional,
		const colour _colour = colour::white(),
		glm::mat4 inTransform = glm::mat4(1)
	) noexcept;
	~light() = default;

	/** Wrapper function that passes to setForward
	 * @param _direction Value to be set
	 */
	void setDirection (const glm::vec3 _direction) noexcept;
	void setType (type _type) noexcept;
	void setColour (colour _colour) noexcept;
	void setLinear (float _value) noexcept;
	void setQuadratic (float _value) noexcept;
	void setAngle (float _value) noexcept; // In degrees
	void setBlur (float _value) noexcept;  // In degrees

	_NODISCARD float getAngle() const noexcept;
	_NODISCARD float getBlur() const noexcept;
	/** Wrapper function to getForward
	 * @return [constexpr glm::vec4] The forward direction
	 */
	_NODISCARD glm::vec4 getDirection() const noexcept;
	_NODISCARD type getType() const noexcept;
	_NODISCARD colour getColour() const noexcept;
	_NODISCARD float getLinear() const noexcept;
	_NODISCARD float getQuadratic() const noexcept;
	_NODISCARD float getAngleRaw() const noexcept;
	_NODISCARD float getBlurRaw() const noexcept;
};
}
