#pragma region 
#pragma once
#include <string>

using std::string;
#pragma endregion

namespace Engine
{
	class Texture
	{
	public:
		Texture();
		Texture(string pLocation);
		void Destroy(bool pValidate);
		/**
		 * @brief Currently loads a set of hard-coded images from file
		 */
		void LoadImages();
		/**
		 * @brief Loads an image from a file into vram (I'm assuming) as a texture
		 * 
		 * @param pPath The location of the image file
		 * @return uint8_t The ID for the texture (max 32 textures so this will be more than enough)
		 */
		static uint8_t LoadTexture(const char* pPath);
		
		//bool GetLoaded() const { return m_textureLoaded; }

		unsigned int m_idTEX0 = 0U, m_idTEX1 = 0U, m_idTEX2 = 0U;

	private:
		//uint32_t BitMap;
		//bool m_textureLoaded = false;
	};
}
