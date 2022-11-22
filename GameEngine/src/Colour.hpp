#pragma once
#include "Model.hpp"

namespace Engine
{
	struct rgb255
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;

		rgb255(): r(0), g(0), b(0) {}

		rgb255(uint8_t pR, uint8_t pG, uint8_t pB)
		: r(pR), g(pG), b(pB) {}

		rgb255(const glm::vec3 pValue)
		{
			r = (uint8_t)glm::max(pValue.r, 0.0f);
			g = (uint8_t)glm::max(pValue.g, 0.0f);
			b = (uint8_t)glm::max(pValue.b, 0.0f);
		}

		constexpr operator glm::vec3() const noexcept
		{
			return glm::vec3(r, g, b);
		}
	};

	struct hsv
	{
		uint16_t hue;
		float saturation;
		float value;

		hsv(): hue(0), saturation(0), value(0) {}

		hsv(uint16_t pHue, float pSaturation, float pValue)
		: hue(pHue), saturation(pSaturation), value(pValue) {}

		hsv(const glm::vec3 pValue)
		{
			hue = (uint16_t)glm::max(pValue.x, 0.0f);
			saturation = glm::clamp(pValue.y, 0.0f, 1.0f);
			value = glm::clamp(pValue.z, 0.0f, 1.0f);
		}

		constexpr operator glm::vec3() const noexcept
		{
			return glm::vec3((float)hue, saturation, value);
		}
	};

	class Colour
	{
	public:
		_NODISCARD_MSG("Engine::Colour::RGBtoHSV(const glm::vec3)")
		/** (Preferred overload) Converts colour data from RGB to HSV
		 * @param pRGB 0-1 Red, 0-1 Green, 0-1 Blue
		 * @return [hsv] 0-360 Hue, 0-1 Saturation, 0-1 Value
		 */
		static hsv RGBtoHSV(const glm::vec3 pRGB) noexcept;
		_NODISCARD_MSG("Engine::Colour::RGBtoHSV(const rgb255)")
		/** (Alternate overload) Converts colour data from RGB to HSV
		 * @param pRGB 0-255 Red, 0-255 Green, 0-255 Blue
		 * @return [hsv] 0-360 Hue, 0-1 Saturation, 0-1 Value
		 */
		static hsv RGBtoHSV(const rgb255 pRGB) noexcept;
		_NODISCARD_MSG("Engine::Colour::HSVtoRGB(const hsv)")
		/** (Preferred overload) Converts colour data from HSV to RGB
		 * @param pHSV 0-360 Hue, 0-1 Saturation, 0-1 Value
		 * @return [glm::vec3] 0-1 Red, 0-1 Green, 0-1 Blue
		 */
		static glm::vec3 HSVtoRGB(const hsv pHSV) noexcept;
		_NODISCARD_MSG("Engine::Colour::HSVtoRGB(const glm::vec3)")
		/** (Alternate overload) Converts colour data from HSV to RGB
		 * @param pHSV 0-360 Hue, 0-1 Saturation, 0-1 Value
		 * @return [glm::vec3] 0-1 Red, 0-1 Green, 0-1 Blue
		 */
		static glm::vec3 HSVtoRGB(const glm::vec3 pHSV) noexcept;

		_NODISCARD_MSG("Engine::Colour::CreateWithRGB(const glm::vec3)")
		/** (Preferred overload) Create a Colour object from RGB
		 * @param pRGB 0-1 Red, 0-1 Green, 0-1 Blue
		 * @return [Colour] A Colour object
		 */
		static Colour CreateWithRGB(const glm::vec3 pRGB) noexcept;
		_NODISCARD_MSG("Engine::Colour::CreateWithRGB(const rgb255)")
		/** (Alternate overload) Create a Colour object from RGB
		 * @param pRGB 0-255 Red, 0-255 Green, 0-255 Blue
		 * @return [Colour] A Colour object
		 */
		static Colour CreateWithRGB(const rgb255 pRGB) noexcept;
		_NODISCARD_MSG("Engine::Colour::CreateWithHSV(const hsv)")
		/** (Preferred overload) Create a Colour object from HSV
		 * @param pHSV 0-360 Hue, 0-1 Saturation, 0-1 Value
		 * @return [Colour] A Colour object
		 */
		static Colour CreateWithHSV(const hsv pHSV) noexcept;
		_NODISCARD_MSG("Engine::Colour::CreateWithHSV(const glm::vec3)")
		/** (Alternate overload) Create a Colour object from HSV
		 * @param pHSV 0-360 Hue, 0-1 Saturation, 0-1 Value
		 * @return [Colour] A Colour object
		 */
		static Colour CreateWithHSV(const glm::vec3 pHSV) noexcept;

		#pragma region Presets
		static inline Colour Black() noexcept;
		static inline Colour White() noexcept;
		static inline Colour Silver() noexcept;
		static inline Colour Grey() noexcept;
		static inline Colour DarkGrey() noexcept;
		static inline Colour Red() noexcept;
		static inline Colour Lime() noexcept;
		static inline Colour Blue() noexcept;
		static inline Colour Yellow() noexcept;
		static inline Colour Cyan() noexcept;
		static inline Colour Magenta() noexcept;
		static inline Colour Maroon() noexcept;
		static inline Colour Green() noexcept;
		static inline Colour Navy() noexcept;
		static inline Colour Olive() noexcept;
		static inline Colour Teal() noexcept;
		static inline Colour Purple() noexcept;
		#pragma endregion

	private:
		glm::vec3 m_RGB = glm::vec3(0);	// Colour data stored as 0-1 for easy conversion

		// Object must be created using a static function
		Colour() = default;
		Colour(glm::vec3 pColour): m_RGB(pColour) {}

	public:
		~Colour() = default;

		_NODISCARD_MSG("Engine::Colour::RGBvec3()")
		/** Get the colour as RGB stored in a vec3
		 * @return [glm::vec3] 0-1 Red, 0-1 Green, 0-1 Blue
		 */
		constexpr glm::vec3 RGBvec3() const noexcept { return m_RGB; }
		_NODISCARD_MSG("Engine::Colour::RGB255()")
		/** Get the colour as RGB stored in a custom structure
		 * @return [rgb255] 0-255 Red, 0-255 Green, 0-255 Blue
		 */
		rgb255 RGB255() const noexcept;
		_NODISCARD_MSG("Engine::Colour::HSVvec3()")
		/** Get the colour as HSV stored in a vec3
		 * @return [glm::vec3] 0-360 Hue, 0-1 Saturation, 0-1 Value
		 */
		glm::vec3 HSVvec3() const noexcept;
		_NODISCARD_MSG("Engine::Colour::HSV()")
		/** Get the colour as HSV stored in a custom structure
		 * @return [hsv] 0-360 Hue, 0-1 Saturation, 0-1 Value
		 */
		hsv HSV() const noexcept;

		#pragma region Operators
		operator glm::vec3() const { return m_RGB; }

		#define COL_OP_CONST_FLOAT(OPERAND) Colour operator OPERAND(float const& pOther) const noexcept;
		#define COL_OP_CONST_COL(OPERAND) Colour operator OPERAND(Colour const& pOther) const noexcept;
		#define COL_OP_CONST_VEC3(OPERAND) Colour operator OPERAND(glm::vec3 const& pOther) const noexcept;
		#define COL_OP_FLOAT(OPERAND) Colour& operator OPERAND(float const& pOther) noexcept;
		#define COL_OP_COL(OPERAND) Colour& operator OPERAND(Colour const& pOther) noexcept;
		#define COL_OP_VEC3(OPERAND) Colour& operator OPERAND(glm::vec3 const& pOther) noexcept;

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
