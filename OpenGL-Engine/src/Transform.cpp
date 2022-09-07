#include "Transform.hpp"

namespace Engine
{
	#pragma region Constructors
	Transform::Transform() : m_transform(mat4(1)) { }

	Transform::Transform(mat4 pValue) : m_transform(pValue) {}

	Transform::Transform(const Transform& pOther)
	{
		m_transform = pOther.GetTransform();
	}

	Transform::Transform(Transform&& pOther) noexcept
	{
		m_transform = pOther.GetTransform();
	}

	Transform& Transform::operator=(const Transform& pOther)
	{
		Transform* newObj = new Transform(pOther.GetTransform());
		return *newObj;
	}

	Transform& Transform::operator=(Transform&& pOther) noexcept
	{
		Transform* newObj = new Transform(pOther.GetTransform());
		return *newObj;
	}
	#pragma endregion

	#pragma region Setters
	void Transform::SetTransform(mat4 pValue)
	{
		m_transform = pValue;
	}

	void Transform::SetPosition(vec3 pValue)
	{
		m_transform[3] = vec4(pValue, m_transform[3][3]);
	}

	void Transform::SetPosition(vec4 pValue)
	{
		m_transform[3] = pValue;
	}

	void Transform::Translate(vec3 pValue)
	{
		m_transform[3] = vec4((vec3)m_transform[3] + pValue, m_transform[3][3]);
	}

	void Transform::SetRight(vec3 pValue)
	{
		m_transform[0] = vec4(pValue, 0);
	}

	void Transform::SetUp(vec3 pValue)
	{
		m_transform[1] = vec4(pValue, 0);
	}

	void Transform::SetForward(vec3 pValue)
	{
		m_transform[2] = vec4(pValue, 0);
	}
	#pragma endregion
	#pragma region Getters
	mat4 Transform::GetTransform() const
	{
		return m_transform;
	}

	vec4 Transform::GetPosition() const
	{
		return m_transform[3];
	}

	vec3 Transform::GetRight() const
	{
		// The transform is horizontally reversed
		return -(vec3)m_transform[0];
	}

	vec3 Transform::GetUp() const
	{
		return (vec3)m_transform[1];
	}

	vec3 Transform::GetForward() const
	{
		// The transform is horizontally reversed
		return -(vec3)m_transform[2];
	}
	#pragma endregion
}
