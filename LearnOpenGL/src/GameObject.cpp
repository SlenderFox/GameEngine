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

	GameObject::GameObject(const GameObject& pOther)
	{
		m_transform = pOther.GetTransform();
	}

	GameObject::GameObject(GameObject&& pOther)
	{
		m_transform = pOther.GetTransform();
	}

	GameObject& GameObject::operator=(const GameObject& pOther)
	{
		GameObject* newObj = new GameObject(pOther.GetTransform());
		return *newObj;
	}

	GameObject& GameObject::operator=(GameObject&& pOther)
	{
		GameObject* newObj = new GameObject(pOther.GetTransform());
		return *newObj;
	}

	mat4 GameObject::GetTransform() const
	{
		return m_transform;
	}

	void GameObject::SetTransform(mat4 pValue)
	{
		m_transform = pValue;
	}
}
