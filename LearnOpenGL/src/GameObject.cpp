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

	void GameObject::SetPosition(vec3 pValue)
	{
		m_transform[3] = vec4(pValue, m_transform[3][3]);
	}

	void GameObject::Translate(vec3 pValue)
	{
		m_transform[3] = vec4((vec3)m_transform[3] + pValue, m_transform[3][3]);
	}

	void GameObject::SetRight(vec3 pValue)
	{
		m_transform[0] = vec4(pValue, 0);
	}

	void GameObject::SetUp(vec3 pValue)
	{
		m_transform[1] = vec4(pValue, 0);
	}

	void GameObject::SetForward(vec3 pValue)
	{
		m_transform[2] = vec4(pValue, 0);
	}
	#pragma endregion
	#pragma region Getters
	mat4 GameObject::GetTransform() const
	{
		return m_transform;
	}

	vec3 GameObject::GetPosition() const
	{
		return (vec3)m_transform[3];
	}

	vec3 GameObject::GetRight() const
	{
		// The camera is horizontally reversed
		return -(vec3)m_transform[0];
	}

	vec3 GameObject::GetUp() const
	{
		return (vec3)m_transform[1];
	}

	vec3 GameObject::GetForward() const
	{
		// The camera is horizontally reversed
		return -(vec3)m_transform[2];
	}
	#pragma endregion
}
