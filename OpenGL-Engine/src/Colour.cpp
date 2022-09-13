#pragma region
/**
 * @file Colour.cpp
 * @author SlenderFox
 * @date 2022-09-11
 * @copyright Copyright (c) 2022
 * Bibliography:
 * https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
 * https://www.rapidtables.com/web/color/RGB_Color.html
 * https://cs.stackexchange.com/questions/64549/convert-hsv-to-rgb-colors
 * https://www.codespeedy.com/hsv-to-rgb-in-cpp/
 */

#include "Colour.hpp"

using std::fmodf;
using glm::vec3;
using glm::clamp;
using glm::max;
using glm::min;
using glm::abs;
#pragma endregion

namespace Engine
{
	// ---Static---

	hsv Colour::RGBtoHSV(glm::vec3 pRGB)
	{
		// Sanitise inputs
		float red = clamp(pRGB.r, 0.0f, 1.0f);
		float green = clamp(pRGB.g, 0.0f, 1.0f);
		float blue = clamp(pRGB.b, 0.0f, 1.0f);
		// Initialise variables
		hsv result;
		float Cmax = max(red, max(green, blue));
		float Cmin = min(red, min(green, blue));
		float delta = Cmax - Cmin;
		// Hue
		if (delta == 0)
			result.hue = 0;
		else if (Cmax == red)
			result.hue = (uint16_t)(60.0f * (((green - blue) / delta) + 360.0f)) % 360;
		else if (Cmax == green)
			result.hue = (uint16_t)(60.0f * (((blue - red) / delta) + 120.0f)) % 360;
		else if (Cmax == blue)
			result.hue = (uint16_t)(60.0f * (((red - green) / delta) + 240.0f)) % 360;
		// Saturation
		if (Cmax == 0)
			result.saturation = 0;
		else
			result.saturation = delta / Cmax;
		// Value
		result.value = Cmax;
		// Done
		return result;
	}

	hsv Colour::RGBtoHSV(rgb255 pRGB)
	{
		return RGBtoHSV((vec3)pRGB / 255.0f);
	}

	vec3 Colour::HSVtoRGB(hsv pHSV)
	{
		// Sanitise inputs
		uint16_t hue = pHSV.hue % 360;
		float saturation = clamp(pHSV.saturation, 0.0f, 1.0f);
		float value = clamp(pHSV.value, 0.0f, 1.0f);
		// Initialise variables
		vec3 result;
		float chroma = value * saturation;
		float min = value - chroma;
		float hueMod = fmodf((float)hue / 60, 2.0f);
		float x = chroma * (1 - abs(hueMod - 1));
		// Find a point along the three bottom faces of the RGB cube (??)
		short intHue = hue / 60;
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
		// Dont
		return result;
	}

	vec3 Colour::HSVtoRGB(glm::vec3 pHSV)
	{
		uint16_t hue = (uint16_t)max(pHSV.r, 0.0f);
		// No need to sanitise inputs
		return HSVtoRGB(hsv(hue, pHSV.g, pHSV.b));
	}

	Colour Colour::CreateWithRGB(vec3 pRGB)
	{
		Colour result;
		result.m_RGB = clamp(pRGB, 0.0f, 1.0f);
		return result;
	}

	Colour Colour::CreateWithRGB(rgb255 pRGB)
	{
		Colour result;
		vec3 rgb = (vec3)pRGB / 255.0f;
		result.m_RGB = clamp(rgb, 0.0f, 1.0f);
		return result;
	}

	Colour Colour::CreateWithHSV(hsv pHSV)
	{
		Colour result;
		// No need to sanitise inputs
		result.m_RGB = HSVtoRGB(pHSV);
		return result;
	}
	
	Colour Colour::CreateWithHSV(vec3 pHSV)
	{
		uint16_t hue = (uint16_t)max(pHSV.r, 0.0f);
		// Lazily pass to other function
		return CreateWithHSV(hsv(hue, pHSV.g, pHSV.b));
	}

	#pragma region Presets
	Colour Colour::Black()		{ return Colour(vec3(0)); }
	Colour Colour::White()		{ return Colour(vec3(1)); }
	Colour Colour::Silver()		{ return Colour(vec3(0.75f,	0.75f,	0.75f)); }
	Colour Colour::Grey()		{ return Colour(vec3(0.5f,	0.5f,	0.5f)); }
	Colour Colour::DarkGrey()	{ return Colour(vec3(0.25f,	0.25f,	0.25f)); }
	Colour Colour::Red()		{ return Colour(vec3(1,		0,		0)); }
	Colour Colour::Lime()		{ return Colour(vec3(0,		1,		0)); }
	Colour Colour::Blue() 		{ return Colour(vec3(0,		0,		1)); }
	Colour Colour::Yellow()		{ return Colour(vec3(1,		1,		0)); }
	Colour Colour::Cyan()		{ return Colour(vec3(0,		1,		1)); }
	Colour Colour::Magenta()	{ return Colour(vec3(1,		0,		1)); }
	Colour Colour::Maroon()		{ return Colour(vec3(0.5f,	0,		0)); }
	Colour Colour::Green()		{ return Colour(vec3(0,		0.5f,	0)); }
	Colour Colour::Navy()		{ return Colour(vec3(0,		0,		0.5f)); }
	Colour Colour::Olive()		{ return Colour(vec3(0.5f,	0.5f,	0)); }
	Colour Colour::Teal()		{ return Colour(vec3(0,		0.5f,	0.5f)); }
	Colour Colour::Purple()		{ return Colour(vec3(0.5f,	0,		0.5f)); }
	#pragma endregion

	// ---Member---

	rgb255 Colour::RGB255()
	{
		return m_RGB * 255.0f;
	}

	vec3 Colour::HSVvec3()
	{
		return RGBtoHSV(m_RGB);
	}
	
	#pragma region Operators
	Colour Colour::operator +(const float& pOther) const { return Colour::CreateWithRGB(m_RGB + pOther); }
	Colour Colour::operator -(const float& pOther) const { return Colour::CreateWithRGB(m_RGB - pOther); }
	Colour Colour::operator *(const float& pOther) const { return Colour::CreateWithRGB(m_RGB * pOther); }
	Colour Colour::operator /(const float& pOther) const { return Colour::CreateWithRGB(m_RGB / pOther); }
	Colour Colour::operator +(const Colour& pOther) const { return Colour::CreateWithRGB(m_RGB + pOther.m_RGB); }
	Colour Colour::operator -(const Colour& pOther) const { return Colour::CreateWithRGB(m_RGB - pOther.m_RGB); }
	Colour Colour::operator *(const Colour& pOther) const { return Colour::CreateWithRGB(m_RGB * pOther.m_RGB); }
	Colour Colour::operator /(const Colour& pOther) const { return Colour::CreateWithRGB(m_RGB / pOther.m_RGB); }
	Colour Colour::operator +(const glm::vec3& pOther) const { return Colour::CreateWithRGB(m_RGB + pOther); }
	Colour Colour::operator -(const glm::vec3& pOther) const { return Colour::CreateWithRGB(m_RGB - pOther); }
	Colour Colour::operator *(const glm::vec3& pOther) const { return Colour::CreateWithRGB(m_RGB * pOther); }
	Colour Colour::operator /(const glm::vec3& pOther) const { return Colour::CreateWithRGB(m_RGB / pOther); }
	Colour& Colour::operator +=(const float& pOther) { m_RGB += pOther; return *this; }
	Colour& Colour::operator -=(const float& pOther) { m_RGB -= pOther; return *this; }
	Colour& Colour::operator *=(const float& pOther) { m_RGB *= pOther; return *this; }
	Colour& Colour::operator /=(const float& pOther) { m_RGB /= pOther; return *this; }
	Colour& Colour::operator +=(const Colour& pOther) { m_RGB += pOther.m_RGB; return *this; }
	Colour& Colour::operator -=(const Colour& pOther) { m_RGB -= pOther.m_RGB; return *this; }
	Colour& Colour::operator *=(const Colour& pOther) { m_RGB *= pOther.m_RGB; return *this; }
	Colour& Colour::operator /=(const Colour& pOther) { m_RGB /= pOther.m_RGB; return *this; }
	Colour& Colour::operator +=(const glm::vec3& pOther) { m_RGB += pOther; return *this; }
	Colour& Colour::operator -=(const glm::vec3& pOther) { m_RGB -= pOther; return *this; }
	Colour& Colour::operator *=(const glm::vec3& pOther) { m_RGB *= pOther; return *this; }
	Colour& Colour::operator /=(const glm::vec3& pOther) { m_RGB /= pOther; return *this; }
	#pragma endregion
}
