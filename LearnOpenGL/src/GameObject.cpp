#include "GameObject.hpp"

namespace Engine
{
	GameObject::GameObject()
	{
		m_transform = mat4(1);
	}

	GameObject::GameObject(mat4 pValue)
	{
		m_transform = pValue;
	}

	#pragma region Copy constructors
	GameObject::GameObject(const GameObject& pOther)
	{
		m_transform = pOther.GetTransform();
	}

	GameObject::GameObject(GameObject&& pOther) noexcept
	{
		m_transform = pOther.GetTransform();
	}

	GameObject& GameObject::operator=(const GameObject& pOther)
	{
		GameObject* newObj = new GameObject(pOther.GetTransform());
		return *newObj;
	}

	GameObject& GameObject::operator=(GameObject&& pOther) noexcept
	{
		GameObject* newObj = new GameObject(pOther.GetTransform());
		return *newObj;
	}
	#pragma endregion

	#pragma region Setters
	void GameObject::SetTransform(mat4 pValue)
	{
		m_transform = pValue;
	}
	#pragma endregion
	#pragma region Getters
	mat4 GameObject::GetTransform() const
	{
		return m_transform;
	}
	#pragma endregion
}
