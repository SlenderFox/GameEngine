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
	Transform::Transform() noexcept
	: m_transform(glm::mat4(1))
	{}

	Transform::Transform(const glm::mat4 inValue) noexcept
	: m_transform(inValue)
	{}

	// Copy, takes lvalue
	Transform::Transform(Transform const &inOther) noexcept
	{ m_transform = inOther.m_transform; }

	Transform &Transform::operator=(Transform const &inOther) noexcept
	{ this->m_transform = inOther.m_transform; return *this; }

	// Move, take rvalue
	Transform::Transform(Transform&& inOther) noexcept
	{ m_transform = std::move(inOther.m_transform); }

	Transform &Transform::operator=(Transform&& inOther) noexcept
	{ this->m_transform = std::move(inOther.m_transform); return *this; }

	void Transform::SetTransform(const glm::mat4 *inValue) noexcept
	{ m_transform = *inValue; }

	void Transform::SetPosition(const vec3 inValue) noexcept
	{
		m_transform[3][0] = inValue.x;
		m_transform[3][1] = inValue.y;
		m_transform[3][2] = inValue.z;
	}

	void Transform::Translate(const vec3 inValue) noexcept
	{
		m_transform[3][0] += inValue.x;
		m_transform[3][1] += inValue.y;
		m_transform[3][2] += inValue.z;
	}

	void Transform::SetAxes(
		const vec3 inRight,
		const vec3 inUp,
		const vec3 inForward
	) noexcept
	{
		m_transform[0] = vec4(normalize(inRight), 0);
		m_transform[1] = vec4(normalize(inUp), 0);
		m_transform[2] = vec4(normalize(inForward), 0);
	}

	void Transform::SetRight(const vec3 inRight) noexcept
	{
		vec3 forward = cross(inRight, vec3(0, 1, 0));
		vec3 up = cross(inRight, forward);
		SetAxes(inRight, up, forward);
	}

	void Transform::SetUp(const vec3 inUp) noexcept
	{
		vec3 right = cross(inUp, vec3(0, 0, 1));
		vec3 forward = cross(inUp, right);
		SetAxes(right, inUp, forward);
	}

	void Transform::SetForward(const vec3 inForward) noexcept
	{
		vec3 right = cross(vec3(0, 1, 0), inForward);
		vec3 up = cross(inForward, right);
		SetAxes(right, up, inForward);
	}

	glm::mat4 Transform::GetTransform() const noexcept
	{ return m_transform; }

	glm::vec4 Transform::GetPosition() const noexcept
	{ return m_transform[3]; }

	vec4 Transform::GetRight() const noexcept
	// The transform is horizontally reversed
	{ return -m_transform[0]; }

	vec4 Transform::GetUp() const noexcept
	{ return m_transform[1]; }

	vec4 Transform::GetForward() const noexcept
	// The transform is horizontally reversed
	{ return -m_transform[2]; }
}
