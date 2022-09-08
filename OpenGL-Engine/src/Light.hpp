#pragma once
#include "Camera.hpp"

namespace Engine
{
	enum class LightType : uint8_t
	{
		Directional,
		Point,
		Spot
	};

	class Light : public Transform
	{
	private:
		LightType m_type;
		glm::vec3 m_lightColour;
		float m_linear = 0.045f;		// For point and spot lights
		float m_quadratic = 0.0075f;	// For point and spot lights
		float m_angle = 10.0f;			// Only for spotlights
		float m_blur = 0.23f;			// Only for spotlights

	public:
		#pragma region Constructors
		Light(LightType pType, glm::vec3 pColour = glm::vec3(1));
		Light(LightType pType, glm::mat4 pTransform = glm::mat4(1));
		Light(LightType pType, glm::mat4 pTransform = glm::mat4(1), glm::vec3 pColour = glm::vec3(1));
		~Light() {}
		#pragma endregion
		#pragma region Setters
		Light* SetType(LightType pType);
		Light* SetCamPosition(glm::vec3 pPosition);
		Light* SetDirection(glm::vec3 pDirection);
		Light* SetColour(glm::vec3 pColour);
		Light* SetLinear(float pValue);
		Light* SetQuadratic(float pValue);
		Light* SetAngle(float pValue);	// In degrees
		Light* SetBlur(float pValue);	// In degrees
		#pragma endregion
		#pragma region Getters
		LightType GetType() const { return m_type; }
		glm::vec4 GetDirection() const;
		glm::vec3 GetColour() const { return m_lightColour; }
		float GetLinear() const { return m_linear; }
		float GetQuadratic() const { return m_quadratic; }
		float GetAngle();
		float GetAngleRaw() const { return m_angle; }
		float GetBlur();
		float GetBlurRaw() const { return m_blur; }
		#pragma endregion
	};
}
