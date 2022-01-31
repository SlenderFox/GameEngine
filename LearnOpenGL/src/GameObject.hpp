#pragma once
#include <glm/glm.hpp>

using glm::mat4;

namespace Engine
{
	class GameObject
	{
	public:
		GameObject();
		GameObject(mat4 pValue);
		// Copy constructors
		GameObject(const GameObject& pOther);
		GameObject(GameObject&& pOther);
		GameObject& operator=(const GameObject& pOther);
		GameObject& operator=(GameObject&& pOther);

		virtual mat4 GetTransform() const;
		virtual void SetTransform(mat4 pValue);

	protected:
		mat4 m_transform = mat4(1);
	};
}
