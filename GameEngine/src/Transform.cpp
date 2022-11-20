#pragma region
#include "Transform.hpp"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::normalize;
using glm::cross;
#pragma endregion

namespace Engine
{
	#pragma region Constructors
	Transform::Transform(): m_transform(mat4(1)) {}

	Transform::Transform(const mat4 pValue): m_transform(pValue) {}

	Transform::Transform(Transform const& pOther)
	{
		m_transform = pOther.GetTransform();
	}

	Transform& Transform::operator=(Transform const& pOther)
	{
		Transform* newObj = new Transform(pOther.GetTransform());
		return *newObj;
	}

	Transform::Transform(Transform&& pOther) noexcept
	{
		m_transform = pOther.GetTransform();
	}

	Transform& Transform::operator=(Transform&& pOther) noexcept
	{
		Transform* newObj = new Transform(pOther.GetTransform());
		return *newObj;
	}
	#pragma endregion

	#pragma region Setters
	void Transform::SetPosition(const vec3 pValue) noexcept
	{
		m_transform[3] = vec4(pValue, m_transform[3][3]);
	}

	void Transform::Translate(vec3 pValue) noexcept
	{
		m_transform[3] = vec4((vec3)m_transform[3] + pValue, m_transform[3][3]);
	}

	void Transform::SetAxes(
		const vec3 pRight,
		const vec3 pUp,
		const vec3 pForward) noexcept
	{
		m_transform[0] = vec4(normalize(pRight), 0);
		m_transform[1] = vec4(normalize(pUp), 0);
		m_transform[2] = vec4(normalize(pForward), 0);
	}

	void Transform::SetRight(const vec3 pRight) noexcept
	{
		vec3 forward = cross(pRight, vec3(0, 1, 0));
		vec3 up = cross(pRight, forward);
		SetAxes(pRight, up, forward);
	}

	void Transform::SetUp(const vec3 pUp) noexcept
	{
		vec3 right = cross(pUp, vec3(0, 0, 1));
		vec3 forward = cross(pUp, right);
		SetAxes(right, pUp, forward);
	}

	void Transform::SetForward(const vec3 pForward) noexcept
	{
		vec3 right = cross(vec3(0, 1, 0), pForward);
		vec3 up = cross(pForward, right);
		SetAxes(right, up, pForward);
	}
	#pragma endregion

	#pragma region Getters
	constexpr vec4 Transform::GetRight() const noexcept
	{
		// The transform is horizontally reversed
		return vec4(-(vec3)m_transform[0], 0);
	}

	constexpr vec4 Transform::GetUp() const noexcept
	{
		return vec4((vec3)m_transform[1], 0);
	}

	constexpr vec4 Transform::GetForward() const noexcept
	{
		// The transform is horizontally reversed
		return vec4(-(vec3)m_transform[2], 0);
	}
	#pragma endregion
}
