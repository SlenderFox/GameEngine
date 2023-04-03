#include "transform.hpp"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::normalize;
using glm::cross;

namespace srender
{
	transform::transform() noexcept
	: m_transform(glm::mat4(1))
	{}

	transform::transform(const glm::mat4 _value) noexcept
	: m_transform(_value)
	{}

	// Copy, takes lvalue
	transform::transform(transform const &_other) noexcept
	{	m_transform = _other.m_transform; }

	transform &transform::operator=(transform const &_other) noexcept
	{	this->m_transform = _other.m_transform; return *this; }

	// Move, take rvalue
	transform::transform(transform&& _other) noexcept
	{	m_transform = std::move(_other.m_transform); }

	transform &transform::operator=(transform&& _other) noexcept
	{	this->m_transform = std::move(_other.m_transform); return *this; }

	void transform::setTransform(const glm::mat4 *_value) noexcept
	{	m_transform = *_value; }

	void transform::setPosition(const vec3 _value) noexcept
	{
		m_transform[3][0] = _value.x;
		m_transform[3][1] = _value.y;
		m_transform[3][2] = _value.z;
	}

	void transform::translate(const vec3 _value) noexcept
	{
		m_transform[3][0] += _value.x;
		m_transform[3][1] += _value.y;
		m_transform[3][2] += _value.z;
	}

	void transform::setAxes(
		const vec3 _right,
		const vec3 _up,
		const vec3 _forward
	) noexcept
	{
		m_transform[0] = vec4(normalize(_right), 0);
		m_transform[1] = vec4(normalize(_up), 0);
		m_transform[2] = vec4(normalize(_forward), 0);
	}

	void transform::setRight(const vec3 _right) noexcept
	{
		vec3 forward = cross(_right, vec3(0, 1, 0));
		vec3 up = cross(_right, forward);
		setAxes(_right, up, forward);
	}

	void transform::setUp(const vec3 _up) noexcept
	{
		vec3 right = cross(_up, vec3(0, 0, 1));
		vec3 forward = cross(_up, right);
		setAxes(right, _up, forward);
	}

	void transform::setForward(const vec3 _forward) noexcept
	{
		vec3 right = cross(vec3(0, 1, 0), _forward);
		vec3 up = cross(_forward, right);
		setAxes(right, up, _forward);
	}

	glm::mat4 transform::getTransform() const noexcept
	{	return m_transform; }

	glm::vec4 transform::getPosition() const noexcept
	{	return m_transform[3]; }

	vec4 transform::getRight() const noexcept
	// The transform is horizontally reversed
	{	return -m_transform[0]; }

	vec4 transform::getUp() const noexcept
	{	return m_transform[1]; }

	vec4 transform::getForward() const noexcept
	// The transform is horizontally reversed
	{	return -m_transform[2]; }
}
