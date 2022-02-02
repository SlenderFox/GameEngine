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

		#pragma region Copy constructors
		GameObject(const GameObject& pOther);
		GameObject(GameObject&& pOther) noexcept;
		GameObject& operator=(const GameObject& pOther);
		GameObject& operator=(GameObject&& pOther) noexcept;
		#pragma endregion

		#pragma region Setters
		virtual void SetTransform(mat4 pValue);
		#pragma endregion
		#pragma region Getters
		virtual mat4 GetTransform() const;
		#pragma endregion

	protected:
		mat4 m_transform = mat4(1);
	};
}
