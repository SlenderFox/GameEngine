#pragma once
#include <glm/glm.hpp>

using glm::vec3;
using glm::vec4;
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
		virtual void SetPosition(vec3 pValue);
		virtual void Translate(vec3 pValue);
		virtual void SetRight(vec3 pValue);
		virtual void SetUp(vec3 pValue);
		virtual void SetForward(vec3 pValue);
		#pragma endregion
		#pragma region Getters
		virtual mat4 GetTransform() const;
		virtual vec3 GetPosition() const;
		virtual vec3 GetRight() const;
		virtual vec3 GetUp() const;
		virtual vec3 GetForward() const;
		#pragma endregion

	protected:
		mat4 m_transform = mat4(1);
	};
}
