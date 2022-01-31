#pragma once
#include "GameObject.hpp"

namespace Engine
{
	class Light : GameObject
	{
	public:
		Light(glm::vec3 pColour);

		glm::vec3 GetColour() const;
		void SetColour(glm::vec3 pColour);

	private:
		glm::vec3 m_lightColour = glm::vec3(1);
	};
}
