#include "colour.hpp"

using glm::vec3;

namespace srender
{
	constexpr float colour::fmod(float x, float y) noexcept
	{
		// Error handling
		if (x == 0.0f && y != 0.0f) return 0.0f;
		assert(x != INFINITY || x != -INFINITY);
		assert(y != 0);
		if (y == INFINITY || y == -INFINITY) return x;

		// Truncate the division
		int32_t n = (int32_t)(x / y);
		return x - n * y;
	}

	constexpr void colour::rgbToHsv(
		float inR,
		float inG,
		float inB,
		uint16_t &outH,
		float &outS,
		float &outV
	) noexcept
	{
		// Sanitise inputs
		inR = glm::clamp(inR, 0.0f, 1.0f);
		inG = glm::clamp(inG, 0.0f, 1.0f);
		inB = glm::clamp(inB, 0.0f, 1.0f);
		// Initialise variables
		float Cmax = glm::max(inR, glm::max(inG, inB));
		float Cmin = glm::min(inR, glm::min(inG, inB));
		float delta = Cmax - Cmin;
		// Hue
		if (delta == 0)
			outH = 0;
		else if (Cmax == inR)
			outH = (uint16_t)(60.0f * (((inG - inB) / delta) + 360.0f)) % 360;
		else if (Cmax == inG)
			outH = (uint16_t)(60.0f * (((inB - inR) / delta) + 120.0f)) % 360;
		else if (Cmax == inB)
			outH = (uint16_t)(60.0f * (((inR - inG) / delta) + 240.0f)) % 360;
		// Saturation
		if (Cmax == 0)
			outS = 0;
		else
			outS = delta / Cmax;
		// Value
		outV = Cmax;
	}

	constexpr vec3 colour::rgbToHsv(vec3 inRGB) noexcept
	{
		uint16_t hue = 0;
		float saturation = 0;
		float value = 0;
		rgbToHsv(inRGB.r, inRGB.g, inRGB.b, hue, saturation, value);
		return { hue, saturation, value };
	}

	constexpr void colour::hsvToRgb(
		uint16_t inH,
		float inS,
		float inV,
		float &outR,
		float &outG,
		float &outB
	) noexcept
	{
		// Sanitise inputs
		inH = inH % 360;
		inS = glm::clamp(inS, 0.0f, 1.0f);
		inV = glm::clamp(inV, 0.0f, 1.0f);
		// Initialise variables
		vec3 result;
		float chroma = inV * inS;
		float min = inV - chroma;
		float two = 2.0f;
		float hueMod = fmod((float)inH / 60, 2.0f);
		float x = chroma * (1 - glm::abs(hueMod - 1));
		// Find a point along the three bottom faces of the RGB cube (??)
		short intHue = inH / 60;
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
		outR = result.r;
		outB = result.b;
		outG = result.g;
	}

	vec3 colour::hsvToRgb(const vec3 inHSV) noexcept
	{
		// Don't want a negative number
		uint16_t hue = (uint16_t)glm::max(inHSV.r, 0.0f);
		vec3 result = vec3();
		hsvToRgb(hue, inHSV.y, inHSV.z, result.r, result.g, result.b);
		return result;
	}

	constexpr colour black()    noexcept { return colour(0.0f); }
	constexpr colour white()    noexcept { return colour(1.0f); }
	constexpr colour silver()   noexcept { return colour(0.75f, 0.75f, 0.75f ); }
	constexpr colour grey()     noexcept { return colour(0.5f,  0.5f,  0.5f  ); }
	constexpr colour darkGrey() noexcept { return colour(0.25f, 0.25f, 0.25f ); }
	constexpr colour red()      noexcept { return colour(1.0f,  0.0f,  0.0f  ); }
	constexpr colour lime()     noexcept { return colour(0.0f,  1.0f,  0.0f  ); }
	constexpr colour blue()     noexcept { return colour(0.0f,  0.0f,  1.0f  ); }
	constexpr colour yellow()   noexcept { return colour(1.0f,  1.0f,  0.0f  ); }
	constexpr colour cyan()     noexcept { return colour(0.0f,  1.0f,  1.0f  ); }
	constexpr colour magenta()  noexcept { return colour(1.0f,  0.0f,  1.0f  ); }
	constexpr colour maroon()   noexcept { return colour(0.5f,  0.0f,  0.0f  ); }
	constexpr colour green()    noexcept { return colour(0.0f,  0.5f,  0.0f  ); }
	constexpr colour navy()     noexcept { return colour(0.0f,  0.0f,  0.5f  ); }
	constexpr colour olive()    noexcept { return colour(0.5f,  0.5f,  0.0f  ); }
	constexpr colour teal()     noexcept { return colour(0.0f,  0.5f,  0.5f  ); }
	constexpr colour purple()   noexcept { return colour(0.5f,  0.0f,  0.5f  ); }

	vec3 colour::rgb() const noexcept
	{ return m_RGB; }

	constexpr vec3 colour::hsv() const noexcept
	{ return rgbToHsv(m_RGB); }

	constexpr colour::colour() noexcept
	{}

	constexpr colour::colour(float inGreyscale) noexcept
	: m_RGB(vec3(inGreyscale))
	{}

	constexpr colour::colour(float inR, float inG, float inB) noexcept
	: m_RGB(vec3(inR, inG, inB))
	{}

	constexpr colour::colour(vec3 inColour) noexcept
	: m_RGB(inColour)
	{}

	colour::colour(const colour &pOther)
	{ m_RGB = pOther.m_RGB; }

	colour &colour::operator=(const colour &pOther)
	{ this->m_RGB = pOther.m_RGB; return *this; }

	constexpr colour::colour(colour&& pOther) noexcept
	{ m_RGB = std::move(pOther.m_RGB); }

	constexpr colour &colour::operator=(colour&& pOther) noexcept
	{ this->m_RGB = std::move(pOther.m_RGB); return *this; }

	colour::operator glm::vec3() const { return m_RGB; }

	#define COL_OP_CONST_FLOAT_DEF(OPERAND) \
	colour colour::operator OPERAND(const float &pOther) const noexcept\
	{ return colour(m_RGB OPERAND pOther); }

	#define COL_OP_CONST_COL_DEF(OPERAND) \
	constexpr colour colour::operator OPERAND(const colour &pOther) const noexcept\
	{ return colour(m_RGB OPERAND pOther.m_RGB); }

	#define COL_OP_CONST_VEC3_DEF(OPERAND) \
	colour colour::operator OPERAND(const vec3 &pOther) const noexcept\
	{ return colour(m_RGB OPERAND pOther); }

	#define COL_OP_FLOAT_DEF(OPERAND) \
	constexpr colour &colour::operator OPERAND(const float &pOther) noexcept\
	{ m_RGB OPERAND pOther; return *this; }

	#define COL_OP_COL_DEF(OPERAND) \
	constexpr colour &colour::operator OPERAND(const colour &pOther) noexcept\
	{ m_RGB OPERAND pOther.m_RGB; return *this; }

	#define COL_OP_VEC3_DEF(OPERAND) \
	constexpr colour &colour::operator OPERAND(const vec3 &pOther) noexcept\
	{ m_RGB OPERAND pOther; return *this; }

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
