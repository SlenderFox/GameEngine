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
		float m_linear = 0.045f;		// For point and spot lights
		float m_quadratic = 0.0075f;	// For point and spot lights
		float m_angle = 10.0f;			// Only for spotlights
		float m_blur = 0.23f;			// Only for spotlights

	public:
		Light(
			const LightType pType = LightType::Directional,
			const Colour pColour = Colour::White(),
			glm::mat4 pTransform = glm::mat4(1));
		~Light() {}

		#pragma region Setters
		/** Wrapper function that passes to SetForward
		 * @param pDirection Value to be set
		 */
		void SetDirection (const glm::vec3 pDirection)	noexcept { Transform::SetForward(pDirection); }
		constexpr void SetType		 (LightType pType)		noexcept { m_type = pType; }
		constexpr void SetColour	 (Colour pColour)			noexcept { m_lightColour = pColour; }
		constexpr void SetLinear	 (float pValue)			noexcept { m_linear = pValue; }
		constexpr void SetQuadratic (float pValue)			noexcept { m_quadratic = pValue; }
		constexpr void SetAngle		 (float pValue)			noexcept { m_angle = pValue; }	// In degrees
		constexpr void SetBlur		 (float pValue)			noexcept { m_blur = pValue; }		// In degrees
		#pragma endregion

		#pragma region Getters
		float GetAngle() const noexcept;
		float GetBlur() const noexcept;
		/** Wrapper function to GetForward
		 * @return [constexpr glm::vec4] The forward direction
		 */
		glm::vec4	GetDirection()	const noexcept { return Transform::GetForward(); }
		constexpr LightType	GetType()		const noexcept { return m_type; }
		constexpr Colour		GetColour()		const noexcept { return m_lightColour; }
		constexpr float		GetLinear()		const noexcept { return m_linear; }
		constexpr float		GetQuadratic()	const noexcept { return m_quadratic; }
		constexpr float		GetAngleRaw()	const noexcept { return m_angle; }
		constexpr float		GetBlurRaw()	const noexcept { return m_blur; }
		#pragma endregion
	};
}
