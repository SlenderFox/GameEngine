/*
 * Bibliography:
 * https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
 * https://www.rapidtables.com/web/color/RGB_Color.html
 * https://cs.stackexchange.com/questions/64549/convert-hsv-to-rgb-colors
 * https://www.codespeedy.com/hsv-to-rgb-in-cpp/
 */

#pragma once
#include "Model.hpp"
#if !__has_include("glm/glm.hpp")
	#include "glm/glm.hpp"
#endif

namespace Engine
{
	/** An object used to store and convert colour information
	 * @note Filled to the brim with constexpr to try and calculate as much as possible at compile time
	*/
	class Colour
	{
		glm::vec3 m_RGB = glm::vec3(0);	// Colour data stored as 0-1 for easy conversion

		/** My own fmod function to have constexpr*/
		_NODISCARD static constexpr
		float fmod(float x, float y)
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

	public:
		#pragma region Conversion
		/** Converts colour data from RGB to HSV
		 * @param inR Input red
		 * @param inG Input green
		 * @param inB Input blue
		 * @param outH Output hue
		 * @param outS Output saturation
		 * @param outV Output value
		 */
		_NODISCARD static constexpr
		void RGBtoHSV(
			float inR,
			float inG,
			float inB,
			uint16_t& outH,
			float& outS,
			float& outV
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

		/** Converts colour data from RGB to HSV
		 * @param inRGB 0-1 Red, 0-1 Green, 0-1 Blue
		 * @return [glm::vec3] 0-360 Hue, 0-1 Saturation, 0-1 Value
		 */
		_NODISCARD static constexpr
		glm::vec3 RGBtoHSV(glm::vec3 inRGB) noexcept
		{
			uint16_t hue = 0;
			float saturation = 0;
			float value = 0;
			RGBtoHSV(inRGB.r, inRGB.g, inRGB.b, hue, saturation, value);
			return { hue, saturation, value };
		}

		/** Converts colour data from HSV to RGB
		 * @param inH Input hue
		 * @param inS Input saturation
		 * @param inV Input value
		 * @param outR Output red
		 * @param outG Output green
		 * @param outB Output blue
		 */
		_NODISCARD static constexpr
		void HSVtoRGB(
			uint16_t inH,
			float inS,
			float inV,
			float& outR,
			float& outG,
			float& outB
		) noexcept
		{
			// Sanitise inputs
			inH = inH % 360;
			inS = glm::clamp(inS, 0.0f, 1.0f);
			inV = glm::clamp(inV, 0.0f, 1.0f);
			// Initialise variables
			glm::vec3 result;
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

		/** Converts colour data from HSV to RGB
		 * @param inHSV 0-360 Hue, 0-1 Saturation, 0-1 Value
		 * @return [glm::vec3] 0-1 Red, 0-1 Green, 0-1 Blue
		 */
		_NODISCARD static constexpr
		glm::vec3 HSVtoRGB(const glm::vec3 inHSV) noexcept
		{
			// Don't want a negative number
			uint16_t hue = (uint16_t)glm::max(inHSV.r, 0.0f);
			glm::vec3 result = glm::vec3();
			HSVtoRGB(hue, inHSV.y, inHSV.z, result.r, result.g, result.b);
			return result;
		}
		#pragma endregion

		#pragma region Presets
		static constexpr Colour Black()		noexcept { return Colour(0.0f); }
		static constexpr Colour White()		noexcept { return Colour(1.0f); }
		static constexpr Colour Silver()		noexcept { return Colour(0.75f,	0.75f,	0.75f	); }
		static constexpr Colour Grey()		noexcept { return Colour(0.5f,	0.5f,		0.5f	); }
		static constexpr Colour DarkGrey()	noexcept { return Colour(0.25f,	0.25f,	0.25f	); }
		static constexpr Colour Red()			noexcept { return Colour(1.0f,	0.0f,		0.0f	); }
		static constexpr Colour Lime()		noexcept { return Colour(0.0f,	1.0f,		0.0f	); }
		static constexpr Colour Blue()		noexcept { return Colour(0.0f,	0.0f,		1.0f	); }
		static constexpr Colour Yellow()		noexcept { return Colour(1.0f,	1.0f,		0.0f	); }
		static constexpr Colour Cyan()		noexcept { return Colour(0.0f,	1.0f,		1.0f	); }
		static constexpr Colour Magenta()	noexcept { return Colour(1.0f,	0.0f,		1.0f	); }
		static constexpr Colour Maroon()		noexcept { return Colour(0.5f,	0.0f,		0.0f	); }
		static constexpr Colour Green()		noexcept { return Colour(0.0f,	0.5f,		0.0f	); }
		static constexpr Colour Navy()		noexcept { return Colour(0.0f,	0.0f,		0.5f	); }
		static constexpr Colour Olive()		noexcept { return Colour(0.5f,	0.5f,		0.0f	); }
		static constexpr Colour Teal()		noexcept { return Colour(0.0f,	0.5f,		0.5f	); }
		static constexpr Colour Purple()		noexcept { return Colour(0.5f,	0.0f,		0.5f	); }
		#pragma endregion

		/** Get the colour as RGB stored in a vec3
		 * @return [glm::vec3] 0-1 Red, 0-1 Green, 0-1 Blue
		 */
		_NODISCARD constexpr
		glm::vec3 RGBvec3() const noexcept { return m_RGB; }

		/** Get the colour as HSV stored in a vec3
		 * @return [glm::vec3] 0-360 Hue, 0-1 Saturation, 0-1 Value
		 */
		_NODISCARD constexpr
		glm::vec3 HSVvec3() const noexcept { return RGBtoHSV(m_RGB); }

		#pragma region Constructors
		constexpr Colour() {}
		constexpr Colour(float inGreyscale): m_RGB(glm::vec3(inGreyscale)) {}
		constexpr Colour(float inR, float inG, float inB): m_RGB(glm::vec3(inR, inG, inB)) {}
		constexpr Colour(glm::vec3 inColour): m_RGB(inColour) {}
		~Colour() = default;
		#pragma endregion

		#pragma region Operators
		operator glm::vec3() const { return m_RGB; }

		#define COL_OP_CONST_FLOAT(OPERAND) constexpr Colour operator OPERAND(float const& pOther) const noexcept\
		{ return Colour(m_RGB OPERAND pOther); }
		#define COL_OP_CONST_COL(OPERAND) constexpr Colour operator OPERAND(Colour const& pOther) const noexcept\
		{ return Colour(m_RGB OPERAND pOther.m_RGB); }
		#define COL_OP_CONST_VEC3(OPERAND) Colour operator OPERAND(glm::vec3 const& pOther) const noexcept\
		{ return Colour(m_RGB OPERAND pOther); }
		#define COL_OP_FLOAT(OPERAND) constexpr Colour& operator OPERAND(float const& pOther) noexcept\
		{ m_RGB OPERAND pOther; return *this; }
		#define COL_OP_COL(OPERAND) constexpr Colour& operator OPERAND(Colour const& pOther) noexcept\
		{ m_RGB OPERAND pOther.m_RGB; return *this; }
		#define COL_OP_VEC3(OPERAND) constexpr Colour& operator OPERAND(glm::vec3 const& pOther) noexcept\
		{ m_RGB OPERAND pOther; return *this; }

		COL_OP_CONST_FLOAT(+)
		COL_OP_CONST_FLOAT(-)
		COL_OP_CONST_FLOAT(*)
		COL_OP_CONST_FLOAT(/)
		COL_OP_CONST_COL(+)
		COL_OP_CONST_COL(-)
		COL_OP_CONST_COL(*)
		COL_OP_CONST_COL(/)
		COL_OP_CONST_VEC3(+)
		COL_OP_CONST_VEC3(-)
		COL_OP_CONST_VEC3(*)
		COL_OP_CONST_VEC3(/)
		COL_OP_FLOAT(+=)
		COL_OP_FLOAT(-=)
		COL_OP_FLOAT(*=)
		COL_OP_FLOAT(/=)
		COL_OP_COL(+=)
		COL_OP_COL(-=)
		COL_OP_COL(*=)
		COL_OP_COL(/=)
		COL_OP_VEC3(+=)
		COL_OP_VEC3(-=)
		COL_OP_VEC3(*=)
		COL_OP_VEC3(/=)
		#pragma endregion
	};
}
