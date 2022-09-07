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
		vec3 m_lightColour = vec3(1);
		float m_linear = 0.045f;		// For point and spot lights
		float m_quadratic = 0.0075f;	// For point and spot lights
		float m_angle = 10.0f;			// Only for spotlights
		float m_blur = 0.23f;			// Only for spotlights

	public:
		#pragma region Constructors
		Light(LightType pType);										// Generic
		Light(LightType pType, vec3 pColour);						// Generic
		Light(LightType pType, mat4 pTransform);					// Generic
		Light(LightType pType, mat4 pTransform, vec3 pColour);		// Generic
		//Light(LightType pType, vec3 pDirection);					// Directional
		//Light(LightType pType, vec3 pDirection, vec3 pColour);		// Directional
		//Light(LightType pType, vec4 pPosition);						// Point
		//Light(LightType pType, vec4 pPosition, vec3 pColour);		// Point
		//Light(LightType pType, mat4 pTransform, float pAngle, float pBlur);									// Spot
		//Light(LightType pType, mat4 pTransform, vec3 pColour, float pAngle, float pBlur);					// Spot
		//Light(LightType pType, vec4 pPosition, vec3 pDirection, vec3 pColour, float pAngle, float pBlur);	// Spot
		~Light() {}
		#pragma endregion
		#pragma region Setters
		Light* SetType(LightType pType);
		Light* SetCamPosition(vec3 pPosition);
		Light* SetDirection(vec3 pDirection);
		Light* SetColour(vec3 pColour);
		Light* SetLinear(float pValue);
		Light* SetQuadratic(float pValue);
		Light* SetAngle(float pValue);	// In degrees
		Light* SetBlur(float pValue);	// In degrees
		#pragma endregion
		#pragma region Getters
		LightType GetType() const { return m_type; }
		vec4 GetDirection() const;
		vec3 GetColour() const { return m_lightColour; }
		float GetLinear() const { return m_linear; }
		float GetQuadratic() const { return m_quadratic; }
		float GetAngle();
		float GetAngleRaw() const { return m_angle; }
		float GetBlur();
		float GetBlurRaw() const { return m_blur; }
		#pragma endregion
	};
}
