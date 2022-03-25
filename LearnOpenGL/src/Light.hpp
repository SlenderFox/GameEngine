#pragma once
#include "Transform.hpp"

namespace Engine
{
	class Light : public Transform
	{
	public:
		Light() {}
		Light(vec3 pColour);
		Light(vec3 pPosition, vec3 pColour);

		void SetColour(vec3 pColour);
		vec3 GetColour() const;

	protected:
		vec3 m_lightColour;
	};

	class LightDirectional : public Light
	{
	public:
		LightDirectional(vec3 pColour);
		LightDirectional(vec3 pDirection, vec3 pColour);

		void SetDirection(vec3 pDirection);
		vec3 GetDirection() const;

	private:
		vec3 m_direction;
	};

	class LightPoint : public Light
	{

	};

	class LightSpot : public Light
	{

	};
}
