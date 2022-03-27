#pragma once
#include "Transform.hpp"

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
	public:
		Light(LightType pType);
		Light(LightType pType, mat4 pTransform);
		Light(LightType pType, vec4 pPosition);
		Light(LightType pType, vec3 pDirection);
		Light(LightType pType, mat4 pTransform, vec3 pColour);
		Light(LightType pType, vec4 pPosition, vec3 pColour);
		Light(LightType pType, vec3 pDirection, vec3 pColour);
		Light(LightType pType, vec4 pPosition, vec3 pDirection, vec3 pColour);
		~Light() {}

		void SetColour(vec3 pColour);
		vec3 GetColour() const;
		vec3 GetDirection() const;

	private:
		LightType m_type;
		vec3 m_lightColour;
	};
}
