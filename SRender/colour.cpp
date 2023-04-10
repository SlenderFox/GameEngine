#include <assert.h>
#include "colour.hpp"
#include "glm/common.hpp"

using glm::vec3;

namespace srender
{
	constexpr float colour::fmod(float _x, float _y) noexcept
	{
		// Error handling
		assert(_y != 0.0f);
		assert(_x != INFINITY || _x != -INFINITY);
		if (_x == 0.0f) return 0.0f;
		if (_y == INFINITY || _y == -INFINITY) return _x;

		// Truncate the division
		int32_t n = (int32_t)(_x / _y);
		return _x - n * _y;
	}

	constexpr void colour::rgbToHsv(
		float _r,
		float _g,
		float _b,
		uint16_t &_outH,
		float &_outS,
		float &_outV
	) noexcept
	{
		// Sanitise inputs
		_r = glm::clamp(_r, 0.0f, 1.0f);
		_g = glm::clamp(_g, 0.0f, 1.0f);
		_b = glm::clamp(_b, 0.0f, 1.0f);
		// Initialise variables
		float Cmax = glm::max(_r, glm::max(_g, _b));
		float Cmin = glm::min(_r, glm::min(_g, _b));
		float delta = Cmax - Cmin;
		// Hue
		if (delta == 0)
			_outH = 0;
		else if (Cmax == _r)
			_outH = (uint16_t)(60.0f * (((_g - _b) / delta) + 360.0f)) % 360;
		else if (Cmax == _g)
			_outH = (uint16_t)(60.0f * (((_b - _r) / delta) + 120.0f)) % 360;
		else if (Cmax == _b)
			_outH = (uint16_t)(60.0f * (((_r - _g) / delta) + 240.0f)) % 360;
		// Saturation
		if (Cmax == 0)
			_outS = 0;
		else
			_outS = delta / Cmax;
		// Value
		_outV = Cmax;
	}

	constexpr vec3 colour::rgbToHsv(vec3 _RGB) noexcept
	{
		uint16_t hue = 0;
		float saturation = 0;
		float value = 0;
		rgbToHsv(_RGB.r, _RGB.g, _RGB.b, hue, saturation, value);
		return { hue, saturation, value };
	}

	constexpr void colour::hsvToRgb(
		uint16_t _h,
		float _s,
		float _v,
		float &_outR,
		float &_outG,
		float &_outB
	) noexcept
	{
		// Sanitise inputs
		_h = _h % 360;
		_s = glm::clamp(_s, 0.0f, 1.0f);
		_v = glm::clamp(_v, 0.0f, 1.0f);
		// Initialise variables
		vec3 result;
		float chroma = _v * _s;
		float min = _v - chroma;
		float hueMod = fmod((float)_h / 60, 2.0f);
		float x = chroma * (1 - glm::abs(hueMod - 1));
		// Find a point along the three bottom faces of the RGB cube (??)
		short intHue = _h / 60;
		switch (intHue)
		{
		case 0:
			result = {chroma, x, 0};
			break;
		case 1:
			result = {x, chroma, 0};
			break;
		case 2:
			result = {0, chroma, x};
			break;
		case 3:
			result = {0, x, chroma};
			break;
		case 4:
			result = {x, 0, chroma};
			break;
		case 5:
			result = {chroma, 0, x};
			break;
		default:
			result = {0, 0, 0};
			break;
		}
		result += min;

		// Not a big fan of doing it this way but it makes the above cleaner and simpler
		_outR = result.r;
		_outB = result.b;
		_outG = result.g;
	}

	vec3 colour::hsvToRgb(const vec3 _HSV) noexcept
	{
		// Don't want a negative number
		uint16_t hue = (uint16_t)glm::max(_HSV.r, 0.0f);
		vec3 result = vec3();
		hsvToRgb(hue, _HSV.y, _HSV.z, result.r, result.g, result.b);
		return result;
	}

	vec3 colour::rgb() const noexcept
	{	return m_RGB; }

	constexpr vec3 colour::hsv() const noexcept
	{	return rgbToHsv(m_RGB); }

	colour::colour() noexcept
	{}

	colour::colour(float _greyscale) noexcept
	: m_RGB(vec3(_greyscale))
	{}

	colour::colour(float _r, float _g, float _b) noexcept
	: m_RGB(vec3(_r, _g, _b))
	{}

	colour::colour(vec3 _colour) noexcept
	: m_RGB(_colour)
	{}

	colour::colour(const colour &_other)
	{	m_RGB = _other.m_RGB; }

	colour &colour::operator=(const colour &_other)
	{	this->m_RGB = _other.m_RGB; return *this; }

	constexpr colour::colour(colour&& _other) noexcept
	{	m_RGB = std::move(_other.m_RGB); }

	constexpr colour &colour::operator=(colour&& _other) noexcept
	{	this->m_RGB = std::move(_other.m_RGB); return *this; }

	colour black()    noexcept { return colour(0.0f); }
	colour white()    noexcept { return colour(1.0f); }
	colour silver()   noexcept { return colour(0.75f, 0.75f, 0.75f ); }
	colour grey()     noexcept { return colour(0.5f,  0.5f,  0.5f  ); }
	colour darkGrey() noexcept { return colour(0.25f, 0.25f, 0.25f ); }
	colour red()      noexcept { return colour(1.0f,  0.0f,  0.0f  ); }
	colour lime()     noexcept { return colour(0.0f,  1.0f,  0.0f  ); }
	colour blue()     noexcept { return colour(0.0f,  0.0f,  1.0f  ); }
	colour yellow()   noexcept { return colour(1.0f,  1.0f,  0.0f  ); }
	colour cyan()     noexcept { return colour(0.0f,  1.0f,  1.0f  ); }
	colour magenta()  noexcept { return colour(1.0f,  0.0f,  1.0f  ); }
	colour maroon()   noexcept { return colour(0.5f,  0.0f,  0.0f  ); }
	colour green()    noexcept { return colour(0.0f,  0.5f,  0.0f  ); }
	colour navy()     noexcept { return colour(0.0f,  0.0f,  0.5f  ); }
	colour olive()    noexcept { return colour(0.5f,  0.5f,  0.0f  ); }
	colour teal()     noexcept { return colour(0.0f,  0.5f,  0.5f  ); }
	colour purple()   noexcept { return colour(0.5f,  0.0f,  0.5f  ); }

	colour::operator glm::vec3() const
	{	return m_RGB; }

	#define COL_OP_CONST_FLOAT_DEF(OPERAND) \
	colour colour::operator OPERAND(const float &_other) const noexcept\
	{	return colour(m_RGB OPERAND _other); }

	#define COL_OP_CONST_COL_DEF(OPERAND) \
	colour colour::operator OPERAND(const colour &_other) const noexcept\
	{	return colour(m_RGB OPERAND _other.m_RGB); }

	#define COL_OP_CONST_VEC3_DEF(OPERAND) \
	colour colour::operator OPERAND(const vec3 &_other) const noexcept\
	{	return colour(m_RGB OPERAND _other); }

	#define COL_OP_FLOAT_DEF(OPERAND) \
	colour &colour::operator OPERAND(const float &_other) noexcept\
	{	m_RGB OPERAND _other; return *this; }

	#define COL_OP_COL_DEF(OPERAND) \
	colour &colour::operator OPERAND(const colour &_other) noexcept\
	{	m_RGB OPERAND _other.m_RGB; return *this; }

	#define COL_OP_VEC3_DEF(OPERAND) \
	colour &colour::operator OPERAND(const vec3 &_other) noexcept\
	{	m_RGB OPERAND _other; return *this; }

	COL_OP_CONST_FLOAT_DEF(+)
	COL_OP_CONST_FLOAT_DEF(-)
	COL_OP_CONST_FLOAT_DEF(*)
	COL_OP_CONST_FLOAT_DEF(/)
	COL_OP_CONST_COL_DEF(+)
	COL_OP_CONST_COL_DEF(-)
	COL_OP_CONST_COL_DEF(*)
	COL_OP_CONST_COL_DEF(/)
	COL_OP_CONST_VEC3_DEF(+)
	COL_OP_CONST_VEC3_DEF(-)
	COL_OP_CONST_VEC3_DEF(*)
	COL_OP_CONST_VEC3_DEF(/)
	COL_OP_FLOAT_DEF(+=)
	COL_OP_FLOAT_DEF(-=)
	COL_OP_FLOAT_DEF(*=)
	COL_OP_FLOAT_DEF(/=)
	COL_OP_COL_DEF(+=)
	COL_OP_COL_DEF(-=)
	COL_OP_COL_DEF(*=)
	COL_OP_COL_DEF(/=)
	COL_OP_VEC3_DEF(+=)
	COL_OP_VEC3_DEF(-=)
	COL_OP_VEC3_DEF(*=)
	COL_OP_VEC3_DEF(/=)
}
