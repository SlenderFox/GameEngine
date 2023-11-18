#include "light.hpp"
#include "glm/detail/func_trigonometric.inl"

using glm::mat4;
using glm::cos;
using glm::sin;
using glm::radians;

namespace srender
{
light::light(
	const light::type _type,
	const colour _colour,
	mat4 inTransform
) noexcept
	: m_type(_type)
	, m_lightColour(_colour)
{
	transform::setTransform(&inTransform);
	if (_type == light::type::spot)
	{	m_angle = cos(radians(m_angle)); }
}

void light::setDirection (const glm::vec3 _direction) noexcept
{	transform::setForward(_direction); }

void light::setType (light::type _type) noexcept
{	m_type = _type; }

void light::setColour (colour _colour) noexcept
{	m_lightColour = _colour; }

void light::setLinear (float _value) noexcept
{	m_linear = _value; }

void light::setQuadratic (float _value) noexcept
{	m_quadratic = _value; }

void light::setAngle (float _value) noexcept
{	m_angle = _value; }

void light::setBlur (float _value) noexcept
{	m_blur = _value; }

float light::getAngle() const noexcept
{	return cos(radians(m_angle)); }

float light::getBlur() const noexcept
{	return sin(radians(90 * m_blur)); }

glm::vec4 light::getDirection()	const noexcept
{	return transform::getForward(); }

light::type light::getType() const noexcept
{	return m_type; }

colour light::getColour() const noexcept
{	return m_lightColour; }

float light::getLinear() const noexcept
{	return m_linear; }

float light::getQuadratic() const noexcept
{	return m_quadratic; }

float light::getAngleRaw() const noexcept
{	return m_angle; }

float light::getBlurRaw() const noexcept
{	return m_blur; }
}
