#pragma once
#include "Entity.hpp"

namespace Engine
{
	enum class LightType: uint8_t
	{
		Directional,
		Point,
		Spot
	};

	class Light: public Entity
	{
	private:
		LightType m_type;
		Colour m_lightColour;
		float m_linear = 0.045f;     // For point and spot lights
		float m_quadratic = 0.0075f; // For point and spot lights
		float m_angle = 10.0f;       // Only for spotlights
		float m_blur = 0.23f;        // Only for spotlights

	public:
		Light(
			const LightType inType = LightType::Directional,
			const Colour inColour = Colour::White(),
			glm::mat4 inTransform = glm::mat4(1)
		) noexcept;
		~Light() = default;

		#pragma region Setters
		/** Wrapper function that passes to SetForward
		 * @param pDirection Value to be set
		 */
		void SetDirection (const glm::vec3 inDirection) noexcept;
		void SetType (LightType inType) noexcept;
		void SetColour (Colour inColour) noexcept;
		void SetLinear (float inValue) noexcept;
		void SetQuadratic (float inValue) noexcept;
		void SetAngle (float inValue) noexcept; // In degrees
		void SetBlur (float inValue) noexcept;  // In degrees
		#pragma endregion

		#pragma region Getters
		_NODISCARD float GetAngle() const noexcept;
		_NODISCARD float GetBlur() const noexcept;
		/** Wrapper function to GetForward
		 * @return [constexpr glm::vec4] The forward direction
		 */
		_NODISCARD glm::vec4 GetDirection() const noexcept;
		_NODISCARD LightType GetType() const noexcept;
		_NODISCARD Colour GetColour() const noexcept;
		_NODISCARD float GetLinear() const noexcept;
		_NODISCARD float GetQuadratic() const noexcept;
		_NODISCARD float GetAngleRaw() const noexcept;
		_NODISCARD float GetBlurRaw() const noexcept;
		#pragma endregion
	};
}
