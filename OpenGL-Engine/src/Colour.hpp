/**
 * @file Colour.hpp
 * @author SlenderFox
 * @date 2022-09-11
 * @copyright Copyright (c) 2022
 */
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

		rgb255(uint8_t pR, uint8_t pG, uint8_t pB): r(pR), g(pG), b(pB) {}

		rgb255(glm::vec3 pValue)
		{
			r = (uint8_t)glm::max(pValue.r, 0.0f);
			g = (uint8_t)glm::max(pValue.g, 0.0f);
			b = (uint8_t)glm::max(pValue.b, 0.0f);
		}

		operator glm::vec3() const
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

		hsv(glm::vec3 pValue)
		{
			hue = (uint16_t)glm::max(pValue.x, 0.0f);
			saturation = glm::clamp(pValue.y, 0.0f, 1.0f);
			value = glm::clamp(pValue.z, 0.0f, 1.0f);
		}

		operator glm::vec3() const
		{
			return glm::vec3(hue, saturation, value);
		}
	};

	class Colour
	{
	public:
		/// @brief (Preferred overload) Converts colour data from RGB to HSV
		/// @param pRGB 0-1 Red, 0-1 Green, 0-1 Blue
		/// @return [hsv] 0-360 Hue, 0-1 Saturation, 0-1 Value
		static hsv RGBtoHSV(glm::vec3 pRGB);
		/// @brief (Alternate overload) Converts colour data from RGB to HSV
		/// @param pRGB 0-255 Red, 0-255 Green, 0-255 Blue
		/// @return [hsv] 0-360 Hue, 0-1 Saturation, 0-1 Value
		static hsv RGBtoHSV(rgb255 pRGB);
		/// @brief (Preferred overload) Converts colour data from HSV to RGB
		/// @param pHSV 0-360 Hue, 0-1 Saturation, 0-1 Value
		/// @return [glm::vec3] 0-1 Red, 0-1 Green, 0-1 Blue
		static glm::vec3 HSVtoRGB(hsv pHSV);
		/// @brief (Alternate overload) Converts colour data from HSV to RGB
		/// @param pHSV 0-360 Hue, 0-1 Saturation, 0-1 Value
		/// @return [glm::vec3] 0-1 Red, 0-1 Green, 0-1 Blue
		static glm::vec3 HSVtoRGB(glm::vec3 pHSV);

		/// @brief (Preferred overload) Create a Colour object from RGB
		/// @param pRGB 0-1 Red, 0-1 Green, 0-1 Blue
		/// @return [Colour] A Colour object
		static Colour CreateWithRGB(glm::vec3 pRGB);
		/// @brief (Alternate overload) Create a Colour object from RGB
		/// @param pRGB 0-255 Red, 0-255 Green, 0-255 Blue
		/// @return [Colour] A Colour object
		static Colour CreateWithRGB(rgb255 pRGB);
		/// @brief (Preferred overload) Create a Colour object from HSV
		/// @param pHSV 0-360 Hue, 0-1 Saturation, 0-1 Value
		/// @return [Colour] A Colour object
		static Colour CreateWithHSV(hsv pHSV);
		/// @brief (Alternate overload) Create a Colour object from HSV
		/// @param pHSV 0-360 Hue, 0-1 Saturation, 0-1 Value
		/// @return [Colour] A Colour object
		static Colour CreateWithHSV(glm::vec3 pHSV);

		#pragma region Presets
		static Colour Black();
		static Colour White();
		static Colour Silver();
		static Colour Grey();
		static Colour DarkGrey();
		static Colour Red();
		static Colour Lime();
		static Colour Blue();
		static Colour Yellow();
		static Colour Cyan();
		static Colour Magenta();
		static Colour Maroon();
		static Colour Green();
		static Colour Navy();
		static Colour Olive();
		static Colour Teal();
		static Colour Purple();
		#pragma endregion

	private:
		glm::vec3 m_RGB = glm::vec3(0);	// Colour data stored as 0-1 for easy conversion

		// Object must be created using a static function
		Colour() = default;
		Colour(glm::vec3 pColour): m_RGB(pColour) {}

	public:
		~Colour() = default;

		/// @brief Get the colour as RGB stored in a vec3
		/// @return [glm::vec3] 0-1 Red, 0-1 Green, 0-1 Blue
		glm::vec3 RGBvec3() const { return m_RGB; }
		/// @brief Get the colour as RGB stored in a custom structure
		/// @return [rgb255] 0-255 Red, 0-255 Green, 0-255 Blue
		rgb255 RGB255();
		/// @brief Get the colour as HSV stored in a vec3
		/// @return [glm::vec3] 0-360 Hue, 0-1 Saturation, 0-1 Value
		glm::vec3 HSVvec3();
		/// @brief Get the colour as HSV stored in a custom structure
		/// @return [hsv] 0-360 Hue, 0-1 Saturation, 0-1 Value
		hsv HSV();

		#pragma region Operators
		operator glm::vec3() const { return m_RGB; }
		Colour operator +(const float& pOther) const;
		Colour operator -(const float& pOther) const;
		Colour operator *(const float& pOther) const;
		Colour operator /(const float& pOther) const;
		Colour operator +(const Colour& pOther) const;
		Colour operator -(const Colour& pOther) const;
		Colour operator *(const Colour& pOther) const;
		Colour operator /(const Colour& pOther) const;
		Colour operator +(const glm::vec3& pOther) const;
		Colour operator -(const glm::vec3& pOther) const;
		Colour operator *(const glm::vec3& pOther) const;
		Colour operator /(const glm::vec3& pOther) const;
		Colour& operator +=(const float& pOther);
		Colour& operator -=(const float& pOther);
		Colour& operator *=(const float& pOther);
		Colour& operator /=(const float& pOther);
		Colour& operator +=(const Colour& pOther);
		Colour& operator -=(const Colour& pOther);
		Colour& operator *=(const Colour& pOther);
		Colour& operator /=(const Colour& pOther);
		Colour& operator +=(const glm::vec3& pOther);
		Colour& operator -=(const glm::vec3& pOther);
		Colour& operator *=(const glm::vec3& pOther);
		Colour& operator /=(const glm::vec3& pOther);
		#pragma endregion
	};
}
