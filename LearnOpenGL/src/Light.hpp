#pragma once
#include "GameObject.hpp"

namespace Engine
{
	class Light : public GameObject
	{
	public:
		Light(vec3 pColour);
		Light(vec3 pPosition, vec3 pColour);

		void SetColour(vec3 pColour);
		vec3 GetColour() const;

	private:
		vec3 m_lightColour = vec3(1);
	};
}
