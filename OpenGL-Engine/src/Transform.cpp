#include "Transform.hpp"
using glm::normalize;
using glm::cross;

namespace Engine
{
	#pragma region Constructors
	Transform::Transform() : m_transform(mat4(1)) {}

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

	void Transform::Translate(vec3 pValue)
	{
		m_transform[3] = vec4((vec3)m_transform[3] + pValue, m_transform[3][3]);
	}

	void Transform::SetAxes(vec3 pRight, vec3 pUp, vec3 pForward)
	{
		m_transform[0] = vec4(normalize(pRight), 0);
		m_transform[1] = vec4(normalize(pUp), 0);
		m_transform[2] = vec4(normalize(pForward), 0);
	}

	void Transform::SetRight(vec3 pRight)
	{
		//m_transform[0] = vec4(normalize(pRight), 0);
		vec3 forward = cross(pRight, vec3(0, 1, 0));
		vec3 up = cross(pRight, forward);
		SetAxes(pRight, up, forward);
	}

	void Transform::SetUp(vec3 pUp)
	{
		//m_transform[1] = vec4(normalize(pUp), 0);
		vec3 right = cross(pUp, vec3(0, 0, 1));
		vec3 forward = cross(pUp, right);
		SetAxes(right, pUp, forward);
	}

	void Transform::SetForward(vec3 pForward)
	{
		//m_transform[2] = vec4(normalize(pForward), 0);
		vec3 right = cross(vec3(0, 1, 0), pForward);
		vec3 up = cross(pForward, right);
		SetAxes(right, up, pForward);
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
