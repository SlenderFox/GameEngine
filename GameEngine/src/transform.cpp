#pragma region
#include "transform.hpp"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::normalize;
using glm::cross;
#pragma endregion

namespace engine
{
	transform::transform() noexcept
	: m_transform(glm::mat4(1))
	{}

	transform::transform(const glm::mat4 inValue) noexcept
	: m_transform(inValue)
	{}

	// Copy, takes lvalue
	transform::transform(transform const &inOther) noexcept
	{ m_transform = inOther.m_transform; }

	transform &transform::operator=(transform const &inOther) noexcept
	{ this->m_transform = inOther.m_transform; return *this; }

	// Move, take rvalue
	transform::transform(transform&& inOther) noexcept
	{ m_transform = std::move(inOther.m_transform); }

	transform &transform::operator=(transform&& inOther) noexcept
	{ this->m_transform = std::move(inOther.m_transform); return *this; }

	void transform::setTransform(const glm::mat4 *inValue) noexcept
	{ m_transform = *inValue; }

	void transform::setPosition(const vec3 inValue) noexcept
	{
		m_transform[3][0] = inValue.x;
		m_transform[3][1] = inValue.y;
		m_transform[3][2] = inValue.z;
	}

	void transform::translate(const vec3 inValue) noexcept
	{
		m_transform[3][0] += inValue.x;
		m_transform[3][1] += inValue.y;
		m_transform[3][2] += inValue.z;
	}

	void transform::setAxes(
		const vec3 inRight,
		const vec3 inUp,
		const vec3 inForward
	) noexcept
	{
		m_transform[0] = vec4(normalize(inRight), 0);
		m_transform[1] = vec4(normalize(inUp), 0);
		m_transform[2] = vec4(normalize(inForward), 0);
	}

	void transform::setRight(const vec3 inRight) noexcept
	{
		vec3 forward = cross(inRight, vec3(0, 1, 0));
		vec3 up = cross(inRight, forward);
		setAxes(inRight, up, forward);
	}

	void transform::setUp(const vec3 inUp) noexcept
	{
		vec3 right = cross(inUp, vec3(0, 0, 1));
		vec3 forward = cross(inUp, right);
		setAxes(right, inUp, forward);
	}

	void transform::setForward(const vec3 inForward) noexcept
	{
		vec3 right = cross(vec3(0, 1, 0), inForward);
		vec3 up = cross(inForward, right);
		setAxes(right, up, inForward);
	}

	glm::mat4 transform::getTransform() const noexcept
	{ return m_transform; }

	glm::vec4 transform::getPosition() const noexcept
	{ return m_transform[3]; }

	vec4 transform::getRight() const noexcept
	// The transform is horizontally reversed
	{ return -m_transform[0]; }

	vec4 transform::getUp() const noexcept
	{ return m_transform[1]; }

	vec4 transform::getForward() const noexcept
	// The transform is horizontally reversed
	{ return -m_transform[2]; }
}
