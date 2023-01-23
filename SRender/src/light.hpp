#pragma once
#include "entity.hpp"

namespace srender
{
	enum class lightType: uint8_t
	{
		Directional,
		Point,
		Spot
	};

	class light: public entity
	{
	private:
		lightType m_type;
		colour m_lightColour;
		float m_linear = 0.045f;     // For point and spot lights
		float m_quadratic = 0.0075f; // For point and spot lights
		float m_angle = 10.0f;       // Only for spotlights
		float m_blur = 0.23f;        // Only for spotlights

	public:
		light(
			const lightType inType = lightType::Directional,
			const colour inColour = colour::white(),
			glm::mat4 inTransform = glm::mat4(1)
		) noexcept;
		~light() = default;

		/** Wrapper function that passes to setForward
		 * @param pDirection Value to be set
		 */
		void setDirection (const glm::vec3 inDirection) noexcept;
		void setType (lightType inType) noexcept;
		void setColour (colour inColour) noexcept;
		void setLinear (float inValue) noexcept;
		void setQuadratic (float inValue) noexcept;
		void setAngle (float inValue) noexcept; // In degrees
		void setBlur (float inValue) noexcept;  // In degrees

		[[nodiscard]] float getAngle() const noexcept;
		[[nodiscard]] float getBlur() const noexcept;
		/** Wrapper function to getForward
		 * @return [constexpr glm::vec4] The forward direction
		 */
		[[nodiscard]] glm::vec4 getDirection() const noexcept;
		[[nodiscard]] lightType getType() const noexcept;
		[[nodiscard]] colour getColour() const noexcept;
		[[nodiscard]] float getLinear() const noexcept;
		[[nodiscard]] float getQuadratic() const noexcept;
		[[nodiscard]] float getAngleRaw() const noexcept;
		[[nodiscard]] float getBlurRaw() const noexcept;
			};
}
