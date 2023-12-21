#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::radians;
using glm::atan;
using glm::tan;
using glm::perspective;
using glm::degrees;
using glm::inverse;

namespace srender
{
camera::camera(float _aspectRatio) noexcept
{
	mat4 id = mat4(1.0f);
	setAspectRatio(_aspectRatio);
	setTransform(&id);
	setFovH(75.0f);
}

camera::camera(
	float _aspectRatio,
	float _fovH
) noexcept
{
	mat4 id = mat4(1.0f);
	setAspectRatio(_aspectRatio);
	setTransform(&id);
	setFovH(_fovH);
}

camera::camera(
	float _aspectRatio,
	mat4 _transform
) noexcept
{
	setAspectRatio(_aspectRatio);
	setTransform(&_transform);
	setFovH(75.0f);
}

camera::camera(
	float _aspectRatio,
	float _fovH,
	mat4 _transform
) noexcept
{
	setAspectRatio(_aspectRatio);
	setTransform(&_transform);
	setFovH(_fovH);
}

camera::camera(
	float _aspectRatio,
	vec3 _from,
	vec3 _to,
	vec3 _up
) noexcept
{
	setAspectRatio(_aspectRatio);
	lookAt(_from, _to, _up);
	setFovH(75.0f);
}

camera::camera(
	float _aspectRatio,
	float _fovH,
	vec3 _from,
	vec3 _to,
	vec3 _up
) noexcept
{
	setAspectRatio(_aspectRatio);
	lookAt(_from, _to, _up);
	setFovH(_fovH);
}

void camera::lookAt(
	vec3 _from,
	vec3 _to,
	vec3 _up
) noexcept
{
	m_view = glm::lookAt(_from, _to, _up);
	setView(&m_view);
}

void camera::modifyFovH(float _value) noexcept
{
	m_fovH += _value;
	if (m_fovH > 160.0f)
		m_fovH = 160.0f;
	else if (m_fovH < 1.0f)
		m_fovH = 1.0f;
	updateFovV();
}

void camera::modifyFovV(float _value) noexcept
{
	m_fovV += _value;
	if (m_fovV > 160.0f)
		m_fovV = 160.0f;
	else if (m_fovV < 1.0f)
		m_fovV = 1.0f;
	updateFovH();
}

void camera::updateFovH() noexcept
{
	m_fovH = degrees(2 * atan(tan(radians(m_fovV) * 0.5f) * m_aspectRatio));
	setProjection(m_fovV);
}

void camera::updateFovV() noexcept
{
	m_fovV = degrees(2 * atan(tan(radians(m_fovH) * 0.5f) * (1 / m_aspectRatio)));
	setProjection(m_fovV);
}

void camera::setTransform(const mat4 *_value) noexcept
{
	transform::setTransform(_value);
	m_view = inverse(getTransform());
}

void camera::setView(mat4 *_value) noexcept
{
	m_view = *_value;
	mat4 inv = inverse(m_view);
	transform::setTransform(&inv);
}

void camera::setProjection(float inFovV) noexcept
{
	m_projection = perspective(radians(inFovV), m_aspectRatio, 0.1f, 500.0f);
}

void camera::setProjection(glm::mat4 *_value) noexcept
{	m_projection = *_value; }

void camera::setAspectRatio(float _aspectRatio) noexcept
{	m_aspectRatio = _aspectRatio; }

void camera::setPosition(vec3 _value) noexcept
{
	transform::setPosition(_value);
	m_view = inverse(getTransform());
}

void camera::translate(vec3 _value) noexcept
{
	transform::translate(_value);
	m_view = inverse(getTransform());
}

void camera::setAxes(
	vec3 _right,
	vec3 _up,
	vec3 _forward
) noexcept
{
	transform::setAxes(_right, _up, _forward);
	m_view = inverse(getTransform());
}

void camera::setFovH(float _fovH) noexcept
{
	m_fovH = _fovH;
	if (m_fovH > 120.0f)
		m_fovH = 120.0f;
	else if (m_fovH < 1.0f)
		m_fovH = 1.0f;
	updateFovV();
}

void camera::setFovV(float inFovV) noexcept
{
	m_fovV = inFovV;
	if (m_fovV > 120.0f)
		m_fovV = 120.0f;
	else if (m_fovV < 1.0f)
		m_fovV = 1.0f;
	updateFovH();
}

mat4 camera::getWorldToCameraMatrix() const noexcept
{	return m_projection * m_view; }

constexpr glm::mat4 camera::getView() const noexcept
{	return m_view; }

constexpr glm::mat4 camera::getProjection() const noexcept
{	return m_projection; }
}
