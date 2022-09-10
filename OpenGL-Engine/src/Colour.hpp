#pragma once
#include "Camera.hpp"

namespace Engine
{
	struct rgb255
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;

		rgb255() : r(0), g(0), b(0) {}

		rgb255(uint8_t pR, uint8_t pG, uint8_t pB)
		 : r(pR), g(pG), b(pB) {}

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

		hsv() : hue(0), saturation(0), value(0) {}

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
	// Static
	public:
		static hsv RGBtoHSV(glm::vec3 pRGB);
		static hsv RGBtoHSV(rgb255 pRGB);
		static glm::vec3 HSVtoRGB(glm::vec3 pHSV);
		static glm::vec3 HSVtoRGB(hsv pHSV);

		static Colour CreateWithRGB(glm::vec3 pRGB);
		static Colour CreateWithRGB(rgb255 pRGB);
		static Colour CreateWithHSV(glm::vec3 pHSV);
		static Colour CreateWithHSV(uint16_t pHue, float pSaturation, float pValue);

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

	// Member
	private:
		glm::vec3 m_RGB = glm::vec3(0);	// Colour data stored as 0-1 for easy conversion

		// Object must be created using a static function
		Colour() = default;
		Colour(glm::vec3 pColour);

	public:
		~Colour() = default;
		operator glm::vec3() const { return m_RGB; }

		glm::vec3 RGBvec3() const { return m_RGB; }
		rgb255 RGB255();
		glm::vec3 HSVvec3();
		hsv HSV();
	};
}
