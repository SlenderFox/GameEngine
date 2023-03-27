/*
 * Bibliography:
 * https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
 * https://www.rapidtables.com/web/color/RGB_Color.html
 * https://cs.stackexchange.com/questions/64549/convert-hsv-to-rgb-colors
 * https://www.codespeedy.com/hsv-to-rgb-in-cpp/
 */

#pragma once
#include "model.hpp"
#if !__has_include("glm/glm.hpp")
	#include "glm/glm.hpp"
#endif

namespace srender
{
	/** An object used to store and convert colour information
	 * @note Filled to the brim with constexpr to try and calculate as much as possible at compile time
	*/
	class colour
	{
		glm::vec3 m_RGB = glm::vec3(0);	// Colour data stored as 0-1 for easy conversion

		/** My own fmod function to have constexpr*/
		[[nodiscard]] static constexpr
		float fmod(float x, float y) noexcept;

	public:
		/** Converts colour data from RGB to HSV
		 * @param inR Input red
		 * @param inG Input green
		 * @param inB Input blue
		 * @param outH Output hue
		 * @param outS Output saturation
		 * @param outV Output value
		 */
		static constexpr
		void rgbToHsv(
			float inR,
			float inG,
			float inB,
			uint16_t &outH,
			float &outS,
			float &outV
		) noexcept;

		/** Converts colour data from RGB to HSV
		 * @param inRGB 0-1 Red, 0-1 Green, 0-1 Blue
		 * @return [glm::vec3] 0-360 Hue, 0-1 Saturation, 0-1 Value
		 */
		[[nodiscard]] static constexpr
		glm::vec3 rgbToHsv(glm::vec3 inRGB) noexcept;

		/** Converts colour data from HSV to RGB
		 * @param inH Input hue
		 * @param inS Input saturation
		 * @param inV Input value
		 * @param outR Output red
		 * @param outG Output green
		 * @param outB Output blue
		 */
		static constexpr
		void hsvToRgb(
			uint16_t inH,
			float inS,
			float inV,
			float &outR,
			float &outG,
			float &outB
		) noexcept;

		/** Converts colour data from HSV to RGB
		 * @param inHSV 0-360 Hue, 0-1 Saturation, 0-1 Value
		 * @return [glm::vec3] 0-1 Red, 0-1 Green, 0-1 Blue
		 */
		[[nodiscard]] static
		glm::vec3 hsvToRgb(const glm::vec3 inHSV) noexcept;

		static colour black()    noexcept { return colour(0.0f); }
		static colour white()    noexcept { return colour(1.0f); }
		static colour silver()   noexcept { return colour(0.75f, 0.75f, 0.75f ); }
		static colour grey()     noexcept { return colour(0.5f,  0.5f,  0.5f  ); }
		static colour darkGrey() noexcept { return colour(0.25f, 0.25f, 0.25f ); }
		static colour red()      noexcept { return colour(1.0f,  0.0f,  0.0f  ); }
		static colour lime()     noexcept { return colour(0.0f,  1.0f,  0.0f  ); }
		static colour blue()     noexcept { return colour(0.0f,  0.0f,  1.0f  ); }
		static colour yellow()   noexcept { return colour(1.0f,  1.0f,  0.0f  ); }
		static colour cyan()     noexcept { return colour(0.0f,  1.0f,  1.0f  ); }
		static colour magenta()  noexcept { return colour(1.0f,  0.0f,  1.0f  ); }
		static colour maroon()   noexcept { return colour(0.5f,  0.0f,  0.0f  ); }
		static colour green()    noexcept { return colour(0.0f,  0.5f,  0.0f  ); }
		static colour navy()     noexcept { return colour(0.0f,  0.0f,  0.5f  ); }
		static colour olive()    noexcept { return colour(0.5f,  0.5f,  0.0f  ); }
		static colour teal()     noexcept { return colour(0.0f,  0.5f,  0.5f  ); }
		static colour purple()   noexcept { return colour(0.5f,  0.0f,  0.5f  ); }

		/** Get the colour as RGB stored in a vec3
		 * @return [glm::vec3] 0-1 Red, 0-1 Green, 0-1 Blue
		 */
		[[nodiscard]]
		glm::vec3 rgb() const noexcept;

		/** Get the colour as HSV stored in a vec3
		 * @return [glm::vec3] 0-360 Hue, 0-1 Saturation, 0-1 Value
		 */
		[[nodiscard]] constexpr
		glm::vec3 hsv() const noexcept;

		colour() noexcept;
		colour(float inGreyscale) noexcept;
		colour(float inR, float inG, float inB) noexcept;
		colour(glm::vec3 inColour) noexcept;

		~colour() = default;

		// Copy, takes lvalue
		colour(const colour &pOther);
		colour &operator=(const colour &pOther);
		// Move, take rvalue
		constexpr colour(colour&& pOther) noexcept;
		constexpr colour &operator=(colour&& pOther) noexcept;

		operator glm::vec3() const;

		#define COL_OP_CONST_FLOAT_DEC(OPERAND) colour operator OPERAND(const float&) const noexcept;
		#define COL_OP_CONST_COL_DEC(OPERAND) colour operator OPERAND(const colour&) const noexcept;
		#define COL_OP_CONST_VEC3_DEC(OPERAND) colour operator OPERAND(const glm::vec3&) const noexcept;
		#define COL_OP_FLOAT_DEC(OPERAND) colour &operator OPERAND(const float&) noexcept;
		#define COL_OP_COL_DEC(OPERAND) colour &operator OPERAND(const colour&) noexcept;
		#define COL_OP_VEC3_DEC(OPERAND) colour &operator OPERAND(const glm::vec3&) noexcept;

		COL_OP_CONST_FLOAT_DEC(+)
		COL_OP_CONST_FLOAT_DEC(-)
		COL_OP_CONST_FLOAT_DEC(*)
		COL_OP_CONST_FLOAT_DEC(/)
		COL_OP_CONST_COL_DEC(+)
		COL_OP_CONST_COL_DEC(-)
		COL_OP_CONST_COL_DEC(*)
		COL_OP_CONST_COL_DEC(/)
		COL_OP_CONST_VEC3_DEC(+)
		COL_OP_CONST_VEC3_DEC(-)
		COL_OP_CONST_VEC3_DEC(*)
		COL_OP_CONST_VEC3_DEC(/)
		COL_OP_FLOAT_DEC(+=)
		COL_OP_FLOAT_DEC(-=)
		COL_OP_FLOAT_DEC(*=)
		COL_OP_FLOAT_DEC(/=)
		COL_OP_COL_DEC(+=)
		COL_OP_COL_DEC(-=)
		COL_OP_COL_DEC(*=)
		COL_OP_COL_DEC(/=)
		COL_OP_VEC3_DEC(+=)
		COL_OP_VEC3_DEC(-=)
		COL_OP_VEC3_DEC(*=)
		COL_OP_VEC3_DEC(/=)
	};
}
