/*
 * Bibliography:
 * https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
 * https://www.ra_idtables.com/web/color/RGB_Color.html
 * https://cs.stackexchange.com/questions/64549/convert-hsv-to-rgb-colors
 * https://www.codespeedy.com/hsv-to-rgb-in-cpp/
 */

#pragma once
#include "glm/vec3.hpp"

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
	/** An object used to store and convert colour information
	 * @note Filled to the brim with constexpr to try and calculate as much as possible at compile time
	*/
	struct colour
	{
	private:
		glm::vec3 m_RGB = glm::vec3(0);	// Colour data stored as 0-1 for easy conversion

	public:
		/** Converts colour data from RGB to HSV
		 * @param _r Input red
		 * @param _g Input green
		 * @param _b Input blue
		 * @param _outH Output hue
		 * @param _outS Output saturation
		 * @param _outV Output value
		 */
		static constexpr void rgbToHsv(
			float _r,
			float _g,
			float _b,
			uint16_t &_outH,
			float &_outS,
			float &_outV
		) noexcept;

		/** Converts colour data from RGB to HSV
		 * @param _RGB 0-1 Red, 0-1 Green, 0-1 Blue
		 * @return [glm::vec3] 0-360 Hue, 0-1 Saturation, 0-1 Value
		 */
		_NODISCARD static constexpr glm::vec3 rgbToHsv(glm::vec3 _RGB) noexcept;

		/** Converts colour data from HSV to RGB
		 * @param _h Input hue
		 * @param _s Input saturation
		 * @param _v Input value
		 * @param _outR Output red
		 * @param _outG Output green
		 * @param _outB Output blue
		 */
		static constexpr void hsvToRgb(
			uint16_t _h,
			float _s,
			float _v,
			float &_outR,
			float &_outG,
			float &_outB
		) noexcept;

		/** Converts colour data from HSV to RGB
		 * @param _HSV 0-360 Hue, 0-1 Saturation, 0-1 Value
		 * @return [glm::vec3] 0-1 Red, 0-1 Green, 0-1 Blue
		 */
		_NODISCARD static glm::vec3 hsvToRgb(const glm::vec3 _HSV) noexcept;

		/** Get the colour as RGB stored in a vec3
		 * @return [glm::vec3] 0-1 Red, 0-1 Green, 0-1 Blue
		 */
		_NODISCARD glm::vec3 rgb() const noexcept;

		/** Get the colour as HSV stored in a vec3
		 * @return [glm::vec3] 0-360 Hue, 0-1 Saturation, 0-1 Value
		 */
		_NODISCARD constexpr glm::vec3 hsv() const noexcept;

		colour() noexcept;
		colour(float _greyscale) noexcept;
		colour(float _r, float _g, float _b) noexcept;
		colour(glm::vec3 _colour) noexcept;

		~colour() = default;

		// Copy, takes lvalue
		colour(const colour &_other);
		colour &operator=(const colour &_other);
		// Move, take rvalue
		constexpr colour(colour&& _other) noexcept;
		constexpr colour &operator=(colour&& _other) noexcept;

		_NODISCARD static colour black()    noexcept;
		_NODISCARD static colour white()    noexcept;
		_NODISCARD static colour silver()   noexcept;
		_NODISCARD static colour grey()     noexcept;
		_NODISCARD static colour darkGrey() noexcept;
		_NODISCARD static colour red()      noexcept;
		_NODISCARD static colour lime()     noexcept;
		_NODISCARD static colour blue()     noexcept;
		_NODISCARD static colour yellow()   noexcept;
		_NODISCARD static colour cyan()     noexcept;
		_NODISCARD static colour magenta()  noexcept;
		_NODISCARD static colour maroon()   noexcept;
		_NODISCARD static colour green()    noexcept;
		_NODISCARD static colour navy()     noexcept;
		_NODISCARD static colour olive()    noexcept;
		_NODISCARD static colour teal()     noexcept;
		_NODISCARD static colour purple()   noexcept;

		operator glm::vec3() const;

		#define COL_OP_CONST_FLOAT_DEC(OPERAND) _NODISCARD colour operator OPERAND(const float&) const noexcept;
		#define COL_OP_CONST_COL_DEC(OPERAND) _NODISCARD colour operator OPERAND(const colour&) const noexcept;
		#define COL_OP_CONST_VEC3_DEC(OPERAND) _NODISCARD colour operator OPERAND(const glm::vec3&) const noexcept;
		#define COL_OP_FLOAT_DEC(OPERAND) _NODISCARD colour &operator OPERAND(const float&) noexcept;
		#define COL_OP_COL_DEC(OPERAND) _NODISCARD colour &operator OPERAND(const colour&) noexcept;
		#define COL_OP_VEC3_DEC(OPERAND) _NODISCARD colour &operator OPERAND(const glm::vec3&) noexcept;

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
